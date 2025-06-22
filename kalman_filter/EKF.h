#pragma once
#include <Eigen/Dense>
#include <shared_mutex>
#include "Car.h"
using namespace std;
using namespace Eigen;

class EKF {
public:
    EKF();
    void init(float initial_x, float initial_y);
    void predict(float dt, float ax, float yaw_rate_input, Car& car);
    void updateGPS(const Vector2f& position);
    void updateIMU(float ax, float yaw_rate);
    void update(const VectorXf& z, const MatrixXf& H, const MatrixXf& R);
    void updatingCarVariables(const Matrix<float, 6, 1>& x, Car& car);

private:
    shared_mutex updateMutex;
    shared_mutex predictMutex;

    Matrix<float, 6, 1> x; // [x, y, vx, ax, yaw, yaw_rate]
    Matrix<float, 6, 6> P;
    Matrix<float, 6, 6> F;
    Matrix<float, 6, 6> Q;
    Matrix<float, 6, 1> B; // רק ax משפיע ישירות

    Matrix<float, 2, 6> H_gps;
    Matrix2f R_gps;

    Matrix<float, 2, 6> H_imu;
    Matrix2f R_imu;

};
