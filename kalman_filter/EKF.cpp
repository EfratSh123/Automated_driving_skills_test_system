#include "EKF.h"
#include <cmath>

using namespace Eigen;

EKF::EKF() {}

// אתחול של המסנן עם מיקום התחלתי
void EKF::init(float initial_x, float initial_y) {
    // וקטור מצב בגודל 6: [x, y, vx, ax, yaw, yaw_rate]
    x = Matrix<float, 6, 1>::Zero();
    x(0) = initial_x;
    x(1) = initial_y;

    // מטריצת השונות (covariance) של המצב – זהות, עם חוסר ודאות גבוה ב־yaw
    P = Matrix<float, 6, 6>::Identity();
    P(4, 4) = 100.0f; // חוסר ודאות ב-yaw

    // מטריצת המעבר (transition matrix) – מתארת איך המצב משתנה עם הזמן
    F = Matrix<float, 6, 6>::Identity();

    // מטריצת רעש תהליך (process noise)
    Q = Matrix<float, 6, 6>::Identity() * 0.01f;

    // מטריצת בקרה – מתארת את השפעת התאוצה על המצב
    B = Matrix<float, 6, 1>::Zero();

    // מטריצת תצפית של GPS – מודדת רק את x ו־y
    H_gps = Matrix<float, 2, 6>::Zero();
    H_gps(0, 0) = 1.0f;
    H_gps(1, 1) = 1.0f;
    // מטריצת שונות מדידת GPS
    R_gps = Matrix2f::Identity() * 2.0f;

    // מטריצת תצפית של IMU – מודדת תאוצה לאורך הציר ומהירות זוויתית
    H_imu = Matrix<float, 2, 6>::Zero();
    H_imu(0, 3) = 1.0f; // ax
    H_imu(1, 5) = 1.0f; // yaw_rate
    // מטריצת שונות מדידת IMU
    R_imu = Matrix2f::Identity() * 0.5f;
}

// שלב החיזוי של הקלמן: עדכון לפי תאוצה ומהירות זוויתית
void EKF::predict(float dt, float ax, float yaw_rate_input, Car& car) {
    // עדכון מטריצת המעבר לפי dt
    F.setIdentity();                
    F(0, 2) = dt;                     // x מתעדכן לפי vx
    F(0, 3) = 0.5f * dt * dt;        // x מתעדכן לפי ax
    F(2, 3) = dt;                   // vx מתעדכן לפי ax
    F(4, 5) = dt;                  // yaw מתעדכן לפי yaw_rate

    // עדכון מטריצת הבקרה B – איך התאוצה משפיעה על המצב
    B.setZero();
    B(0) = 0.5f * dt * dt;
    B(2) = dt;
    B(3) = 1.0f;

    // עדכון מצב ו־ covariance לפי המודל
    x = F * x + B * ax;
    P = F * P * F.transpose() + Q;

    // עדכון של yaw ו־yaw_rate ישירות מה־IMU
    x(5) = yaw_rate_input;
    x(4) += yaw_rate_input * dt;

    // עדכון משתני הרכב בפועל
	updatingCarVariables(x, ref(car));
}

// עדכון מדידה מה־GPS (מדידה של מיקום)
void EKF::updateGPS(const Vector2f& position) {
    update(position, H_gps, R_gps);
}
// עדכון מדידה מה־IMU (מדידה של תאוצה ו־yaw rate)
void EKF::updateIMU(float ax, float yaw_rate) {
    Vector2f z;
    z << ax, yaw_rate;
    update(z, H_imu, R_imu);
}

// פונקציית עדכון כללית – מבצעת את שלב המדידה של המס
void EKF::update(const VectorXf& z, const MatrixXf& H, const MatrixXf& R) {
    unique_lock<shared_mutex> lock(updateMutex);
    VectorXf y = z - H * x;
    MatrixXf S = H * P * H.transpose() + R;
    MatrixXf K = P * H.transpose() * S.inverse();       // מטריצת קלמן
    x = x + K * y;                                     // עדכון המצב
    P = (MatrixXf::Identity(x.rows(), x.rows()) - K * H) * P;   // עדכון השונות
}

// פונקציה שמעדכנת את האובייקט Car לפי מצב המסנן
void EKF::updatingCarVariables(const Matrix<float, 6, 1>& x, Car& car) {
    Vector2f pos(x(0), x(1));
    car.setLocation(pos);
    car.setSpeed(std::abs(x(2)) * 3.6f); // המרה מ-m/s ל-km/h
    car.setAcceleration(std::abs(x(3)));
}
