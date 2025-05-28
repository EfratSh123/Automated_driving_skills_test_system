#include "KalmanFilter.h"
#define _USE_MATH_DEFINES // חובה עבור M_PI ב-Visual Studio, לפני כלול <cmath>
#include <windows.h>
#include <cmath>   
#include <iostream>
#include <vector>
#include <Eigen/Dense> // וודא ש-Eigen מותקן ונגיש לפרויקט שלך
#include <cmath>          // Include cmath after defining _USE_MATH_DEFINES
#include <mutex>      // עבור std::lock_guard ו-std::mutex (בגלל פעולות ה-GET שלך)
#define M_PI 3.14159265358979323846

// כלול את קבצי הכותרת של המחלקות IMU ו-GPS שלך
#include "IMU.h" // וודא שזהו הנתיב הנכון לקובץ הכותרת של מחלקת ה-IMU שלך
#include "GPS.h" // וודא שזהו הנתיב הנכון לקובץ הכותרת של מחלקת ה-GPS שלך


// מרחב שמות Eigen לשימוש נוח
using Eigen::MatrixXd;
using Eigen::VectorXd;

// --- פונקציות עזר גלובליות ---
// Global helper functions

// המרת מעלות לרדיאנים
// Converts degrees to radians
float deg2rad(float deg) {
    return deg * M_PI / 180.0;
}

// נקודת ייחוס גלובלית למערכת קואורדינטות מקומית (תוגדר בקריאת ה-GPS הראשונה)
// Global reference point for local coordinate system (set by the first GPS reading)
float global_initial_lat = 0.0;
float global_initial_lon = 0.0;
bool global_initial_ref_set = false;

// Converts GPS coordinates (Lat, Lon) to local XY (meters)
VectorXd gps_to_local_xy(float lat, float lon) {
    if (!global_initial_ref_set) {
        global_initial_lat = lat;
        global_initial_lon = lon;
        global_initial_ref_set = true;
    }
    float dLat = lat - global_initial_lat;
    float dLon = lon - global_initial_lon;
    // Approximation: 1 degree of latitude ~ 111,000 meters
    float x_meters = dLon * (111000.0 * cos(deg2rad(global_initial_lat))); 
    float y_meters = dLat * 111000.0;
    VectorXd local_xy(2);
    local_xy << x_meters, y_meters;
    return local_xy;
}


class KalmanFilter {
public:
    // מטריצות ווקטורים של מסנן קלמן
    VectorXd state;             // מצב: [x, y, vx, vy, yaw, yaw_rate_bias]
    // State: [x, y, vx, vy, yaw, yaw_rate_bias]
    MatrixXd process_covariance; // מטריצת קוואריאנס של רעש התהליך (Q)
    // Process noise covariance matrix (Q)
    MatrixXd measurement_covariance; // מטריצת קוואריאנס של רעש המדידה (R)
    // Measurement noise covariance matrix (R)
    MatrixXd error_covariance;   // מטריצת קוואריאנס של השגיאה (P)
    // Error covariance matrix (P)
    float dt;                   // מרווח זמן בין דגימות IMU
    // Time step between IMU readings

    KalmanFilter(float initial_x, float initial_y, float initial_yaw,
        float imu_accel_noise, float imu_gyro_noise,
        float gps_pos_noise, float time_step)
        : dt(time_step) {
        // מצב: [x, y, vx, vy, yaw, yaw_rate_bias]
        state = VectorXd(6);
        state << initial_x, initial_y, 0, 0, initial_yaw, 0; // מהירות והטיית ג'ירו התחלתיות הן 0
        // מטריצת קוואריאנס של רעש התהליך (Q) - גודל 6x6
        process_covariance = MatrixXd(6, 6);
        process_covariance <<
            0.5 * dt * dt * imu_accel_noise, 0, 0, 0, 0, 0,
            0, 0.5 * dt * dt * imu_accel_noise, 0, 0, 0, 0,
            0, 0, dt* imu_accel_noise, 0, 0, 0,
            0, 0, 0, dt* imu_accel_noise, 0, 0,
            0, 0, 0, 0, dt* imu_gyro_noise, 0,
            0, 0, 0, 0, 0, dt* imu_gyro_noise;
        // מטריצת קוואריאנס של רעש המדידה (R) - גודל 2x2 (עבור GPS x, y)
        measurement_covariance = MatrixXd(2, 2);
        measurement_covariance << gps_pos_noise, 0,
            0, gps_pos_noise;
        // מטריצת קוואריאנס של השגיאה (P) - אתחול כמטריצת יחידה
        error_covariance = MatrixXd::Identity(6, 6);
    }

    // פונקציית מסנן קלמן מורחב (EKF) - מחולקת לשלבים
    // Extended Kalman Filter (EKF) function - divided into phases
    // שימו לב: הפרמטרים IMU& ו-GPS& אינם const, כדי לאפשר קריאה לפעולות GET שאינן const.
    // Note: IMU& and GPS& parameters are not const, to allow calling non-const GET operations.
    std::vector<float> process(IMU& imu_data, GPS& gps_data) {
        // --- 2. שלב החיזוי (Prediction Phase) - ניזון מקריאות IMU ---
        // 2. Prediction Phase - Fed by IMU readings
        predict(imu_data);

        // --- 3. שלב העדכון (Update Phase) - ניזון מקריאות GPS (אם זמינות) ---
        // 3. Update Phase - Fed by GPS readings (if available)
        // בדיקה פשוטה אם יש נתוני GPS (אם קו רוחב/אורך שניהם 0, נניח שאין מדידה תקפה)
        // Simple check if GPS data is available (if both lat/lon are 0, assume no valid measurement)
        if (gps_data.getLatitude() != 0 || gps_data.getLongitude() != 0) { // שימוש ב-getLatitude()/getLongitude()
            update(gps_data);
        }
        // החזרת המיקום המשוערך האחרון
        // Return the last estimated position
        std::vector<float> result;
        result.push_back(state(0));
        result.push_back(state(1));
        return result;
    }

private:
    // פונקציית חיזוי
    // Prediction function
    // שימו לב: הפרמטר IMU& אינו const, כדי לאפשר קריאה לפעולות GET שאינן const.
    // Note: IMU& parameter is not const, to allow calling non-const GET operations.
    void predict(IMU& imu_data) {
        // מודל דינמי בלתי ליניארי f(x, u)
        float x = state(0);
        float y = state(1);
        float vx = state(2);
        float vy = state(3);
        float yaw = state(4);
        float yaw_rate_bias = state(5);

        // GyroY הוא קצב הסבסוב (שימוש בפעולת GET)
        float yaw_rate_from_imu = imu_data.getGyroY();
        // Acceleration היא התאוצה קדימה (שימוש בפעולת GET)
        float acceleration_from_imu = imu_data.getAcceleration();

        VectorXd predicted_state(6);
        predicted_state(0) = x + vx * dt + 0.5 * acceleration_from_imu * dt * dt * cos(yaw);
        predicted_state(1) = y + vy * dt + 0.5 * acceleration_from_imu * dt * dt * sin(yaw);
        predicted_state(2) = vx + acceleration_from_imu * dt * cos(yaw);
        predicted_state(3) = vy + acceleration_from_imu * dt * sin(yaw);
        predicted_state(4) = yaw + (yaw_rate_from_imu - yaw_rate_bias) * dt;
        predicted_state(5) = yaw_rate_bias;

        // חישוב מטריצת יעקוביאן של פונקציית המעבר (F_jacobian)
        MatrixXd F_jacobian(6, 6);
        F_jacobian <<
            1, 0, dt, 0, (-vx * dt * sin(yaw) + 0.5 * acceleration_from_imu * dt * dt * (-sin(yaw))), 0,
            0, 1, 0, dt, (vx * dt * cos(yaw) + 0.5 * acceleration_from_imu * dt * dt * cos(yaw)), 0,
            0, 0, 1, 0, (-acceleration_from_imu * dt * sin(yaw)), 0,
            0, 0, 0, 1, (acceleration_from_imu * dt * cos(yaw)), 0,
            0, 0, 0, 0, 1, -dt,
            0, 0, 0, 0, 0, 1;

        // חיזוי קוואריאנס השגיאה: P = F * P * F^T + Q
        error_covariance = F_jacobian * error_covariance * F_jacobian.transpose() + process_covariance;
        state = predicted_state;
    }

    // שימו לב: הפרמטר GPS& אינו const, כדי לאפשר קריאה לפעולות GET שאינן const.
    void update(GPS& gps_data) {
        // Convert GPS coordinates to local XY (using GET operations)
        VectorXd local_xy_gps = gps_to_local_xy(gps_data.getLatitude(), gps_data.getLongitude());
        VectorXd measurement(2);
        measurement << local_xy_gps(0), local_xy_gps(1);
        // Predicted measurement vector h(x)
        VectorXd predicted_measurement(2);
        predicted_measurement << state(0), state(1);
        // Calculate Innovation
        VectorXd innovation = measurement - predicted_measurement;
        // Calculate Jacobian matrix of the measurement function (H_jacobian)
        MatrixXd H_jacobian(2, 6);
        H_jacobian <<
            1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0;
        // Calculate Innovation covariance: S = H * P * H^T + R
        MatrixXd innovation_covariance = H_jacobian * error_covariance * H_jacobian.transpose() + measurement_covariance;
        // Calculate Kalman Gain: K = P * H^T * S^-1
        MatrixXd kalman_gain = error_covariance * H_jacobian.transpose() * innovation_covariance.inverse();
        // Update state: x = x + K * innovation
        state = state + kalman_gain * innovation;
        // Update error covariance: P = (I - K * H) * error_covariance;
        error_covariance = (MatrixXd::Identity(6, 6) - kalman_gain * H_jacobian) * error_covariance;
    }
};

