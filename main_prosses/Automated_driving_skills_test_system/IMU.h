#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include "Car.h"
#include "globalFunc.h"
#include "EKF.h"
#include "GPS.h"
#include <mutex>
using namespace std;	
class IMU
{
public:
	IMU();
private:
	// מרחק שהרכב עבר מהפניה הקודמת
	float distance;
	//float AccelNoise, GyroNoise;

	float accelX;
	mutex mtx_accelX;
	float accelY;
	mutex mtx_accelY;
	// קצב דגימה מהחיישן
	float dt = 0.1f;
	// מהירות זוויתית סביב ציר Y
	float GyroX;
	mutex mtx_GyroX;

	globalFunc globalPrint;
	chrono::steady_clock::time_point startTime;
	mutex mtx_startTime;

	EKF* kalman;
public:
	void setAcceX(float AccelX) { lock_guard<std::mutex> lock(mtx_accelX); this->accelX = AccelX; }
	float getAccelX() { lock_guard<std::mutex> lock(mtx_accelX); return accelX; }
	void setAcceY(float AccelY) { lock_guard<std::mutex> lock(mtx_accelY); this->accelY = AccelY; }
	float getAccelY() { lock_guard<std::mutex> lock(mtx_accelY); return accelY; }
	void setGyroX(float GyroY) { lock_guard<std::mutex> lock(mtx_GyroX); this->GyroX = GyroY; }
	float getGyroX() { lock_guard<std::mutex> lock(mtx_GyroX); return GyroX; }
	float getDistance() {return distance; }
	void setDistance(float distance) {this->distance = distance; }
	void IMUplay(Car& car);
	// פונקציות מעקב פנימיות
	void monitorSpeedChangeProcess(string reason,
		float targetSpeed,
		float maxDurationSeconds,
		float emergencyDecelerationRate, 
		Car& car);
	bool isCarDecelerating(float previousSpeed, float currentSpeed);
	static bool is_target_reached(float current_speed, float target_speed);
	// מעקב תמידי אחר מהירות הרכב
	void monitorMaxSpeed(Car &c);
	void manageDrivingEvent(string reason, Car& car);
	float get_target_speed_for_reason(string reason);
	unsigned long get_time_limit_for_reason(string reason);

	void setKalman(EKF* k) { kalman = k; }

};
