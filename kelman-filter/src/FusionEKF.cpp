#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

/**
 * Constructor.
 */
FusionEKF::FusionEKF()//�������� ������ �� ������, ��� ����� ����� ������ �� �������� ������
{
	is_initialized_ = false;// ��� ����� �� ������ �����

	previous_timestamp_ = 0;// ����� ������ ��� ������ �����

    // ����� ������� ������ ����� ������
	R_laser_ = MatrixXd(2, 2);//������ ������ ���� ������ �� ������
	R_radar_ = MatrixXd(3, 3);//������ ������ ���� ������ �� �����
    H_laser_ = MatrixXd(2, 4);// ������ ������ �� ������, ������ �� ���� �� ����� ���� �� ������ ����� ������ �� ������.



    //����� ������ ���� ������ �� ������
    R_laser_ << 0.0225, 0,
            0, 0.0225;

    //����� ������ ���� ������ �� �����
    R_radar_ << 0.09, 0, 0,
            0, 0.0009, 0,
            0, 0, 0.09;
    // Initialize process measurements
    ekf_.P_ = MatrixXd(4, 4);//������ ������ �� ���� ���� ������� ���� ������ � �����: �� ��� ����� ������ ������ �� ������ �������� �� �������� ������
    ekf_.P_ << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1000, 0,
            0, 0, 0, 1000;//����� ������ ���� ������� �� ������

    H_laser_ << 1, 0, 0, 0,
            0, 1, 0, 0;

}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack)
{
    // Initialization
    if (!is_initialized_)
    {
        // first measurement
        cout << "EKF: " << endl;
        ekf_.x_ = VectorXd(4);
        ekf_.x_ << 1, 1, 1, 1;

        if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
        {
            // Convert radar from polar to cartesian coordinates
            // and initialize state.
            // Range
            double rho = measurement_pack.raw_measurements_[0];
            double phi = measurement_pack.raw_measurements_[1];
            double rho_dot = measurement_pack.raw_measurements_[2];

            // Polar coordinate to cartisian coordinate
            double x = rho * cos(phi);
            double y = rho * sin(phi);

            // handle divided by zero error
            if (x < 0.0001) x = 0.0001;
            if (y < 0.0001) y = 0.0001;

            double vx = rho_dot * cos(phi);
            double vy = rho_dot * sin(phi);

            ekf_.x_ << x, y, vx, vy;

        }
        else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER)
        {
            // Initialize state.
            ekf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0;

        }

        // saving previous timestamp
        previous_timestamp_ = measurement_pack.timestamp_;
        // done initializing, no need to predict or update
        is_initialized_ = true;
        return;
    }

    // Prediction
    double dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;
    previous_timestamp_ = measurement_pack.timestamp_;

    // state transsion update
    ekf_.F_ = MatrixXd(4, 4);
    ekf_.F_ << 1, 0, dt, 0,
            0, 1, 0, dt,
            0, 0, 1, 0,
            0, 0, 0, 1;

    double noise_ax = 9.0;
    double noise_ay = 9.0;

    double dt_2 = dt * dt;
    double dt_3 = dt_2 * dt;
    double dt_4 = dt_3 * dt;
    double dt_4_4 = dt_4 / 4;
    double dt_3_2 = dt_3 / 2;

    ekf_.Q_ = MatrixXd(4, 4);

    ekf_.Q_ << dt_4_4 * noise_ax, 0, dt_3_2 * noise_ax, 0,
            0, dt_4_4 * noise_ay, 0, dt_3_2 * noise_ay,
            dt_3_2 * noise_ax, 0, dt_2 * noise_ax, 0,
            0, dt_3_2 * noise_ay, 0, dt_2 * noise_ay;

    ekf_.Predict();

    // Update
    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
    {
        // Radar updates
        ekf_.H_ = tools.CalculateJacobian(ekf_.x_);
        ekf_.R_ = R_radar_;
        ekf_.UpdateEKF(measurement_pack.raw_measurements_);
    }
    else
    {
        // Laser updates
        ekf_.H_ = H_laser_;
        ekf_.R_ = R_laser_;
        ekf_.Update(measurement_pack.raw_measurements_);
    }


    // print the output
    cout << "x_ = " << ekf_.x_ << endl;
    cout << "P_ = " << ekf_.P_ << endl;
}
