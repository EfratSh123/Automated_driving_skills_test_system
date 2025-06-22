//#ifndef KALMANFILTER_H
//#define KALMANFILTER_H
//#include <vector>
//#include <Eigen/Dense>
//
//// קדימות לקבצים חיצוניים - רק הצהרות קדימה כאן (לא כולל את הקבצים עצמם)
//class IMU;
//class GPS;
//
//class KalmanFilter {
//public:
//	// constractor with restart matrices
//    KalmanFilter(float initial_x, float initial_y, float initial_yaw,
//        float imu_accel_noise, float imu_gyro_noise,
//        float gps_pos_noise, float time_step);
//
//    void predict(IMU& imu_data);
//    void update(GPS& gps_data);
//    // פונקציית עיבוד הכוללת חיזוי ועדכון
//    std::vector<float> process(IMU& imu_data, GPS& gps_data);
//private:
//    // משתני המצב ומטריצות
//    Eigen::VectorXd state;               // מצב: [x, y, vx, vy, yaw, yaw_rate_bias]
//    Eigen::MatrixXd process_covariance;  // Q - קוואריאנס של רעש תהליך
//    Eigen::MatrixXd measurement_covariance; // R - קוואריאנס של רעש מדידה
//    Eigen::MatrixXd error_covariance;    // P - קוואריאנס שגיאה
//    float dt;                            // מרווח זמן בין מדידות
//
//};
//
//#endif
