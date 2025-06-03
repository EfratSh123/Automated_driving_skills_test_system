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
#include "GPS.h"
#include <mutex>
using namespace std;	
class IMU
{
public:
	IMU();
	IMU(Car* car);
private:
	Car* carPtr;
	float Acceleration;
	mutex mtx_Acceleration;
	float AccelNoise, GyroNoise;
	// קצב דגימה מהחיישן
	float dt = 0.1f;
	float distance;
	// מהירות הזוויתית סביב ציר X, Y
	float GyroX;
	mutex mtx_GyroX;
	float GyroY;
	mutex mtx_GyroY;
	globalFunc globalPrint;
	chrono::steady_clock::time_point startTime;
	mutex mtx_startTime;

public:
	void computeSensorNoise(GPS* gps);
	float computeStdDev(const std::vector<float>& data);
	void setAcceleration(float Acceleration) { lock_guard<std::mutex> lock(mtx_Acceleration); this->Acceleration = Acceleration; }
	float getAcceleration() { lock_guard<std::mutex> lock(mtx_Acceleration); return Acceleration; }
	void setGyroX(float GyroX) { lock_guard<std::mutex> lock(mtx_GyroX); this->GyroX = GyroX; }
	float getGyroX() { lock_guard<std::mutex> lock(mtx_GyroX); return GyroX; }
	void setGyroY(float GyroY) { lock_guard<std::mutex> lock(mtx_GyroY); this->GyroY = GyroY; }
	float getGyroY() { lock_guard<std::mutex> lock(mtx_GyroY); return GyroY; }
	float getDistance() {return distance; }
	void setDistance(float distance) {this->distance = distance; }
	void IMUplay();
	// פונקציות מעקב פנימיות
	void monitorSpeedChangeProcess(string reason,
		float targetSpeed,
		float maxDurationSeconds,
		float emergencyDecelerationRate);
	bool isCarDecelerating(float previousSpeed, float currentSpeed);
	static bool is_target_reached(float current_speed, float target_speed);
	// מעקב תמידי אחר מהירות הרכב
	void monitorMaxSpeed(Car &c);
	void manageDrivingEvent(string reason);
	double getStartTime()
	{
		std::lock_guard<std::mutex> lock(mtx_startTime);
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
		startTime.time_since_epoch()).count();
		return duration / 1000.0;
	}
	void updateStartTimeByOneSecond()
	{
		std::lock_guard<std::mutex> lock(mtx_startTime);
		startTime = startTime + std::chrono::seconds(1);
	}
	float get_target_speed_for_reason(string reason);
	unsigned long get_time_limit_for_reason(string reason);


};
