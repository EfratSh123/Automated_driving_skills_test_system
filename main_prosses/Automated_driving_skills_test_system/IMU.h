#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include "Car.h"
#include "globalFunc.h"
#include <mutex>
using namespace std;	

class IMU
{
public:
	IMU();
	IMU(Car* car);
private:
	Car* carPtr; // מצביע לאובייקט Car
	//******************** האם ליצור לו mutex? **************************
	double Acceleration;
	mutex mtx_Acceleration;
	// קצב דגימה מהחיישן
	double dt = 0.1;
	double distance;
	mutex mtx_distance;
	// מהירות הזוויתית סביב ציר X, Y
	double GyroX;
	mutex mtx_GyroX;
	double GyroY;
	mutex mtx_GyroY;
	globalFunc globalPrint;

public:
	void setAcceleration(double Acceleration) { lock_guard<std::mutex> lock(mtx_Acceleration); this->Acceleration = Acceleration; }
	double getAcceleration() { lock_guard<std::mutex> lock(mtx_Acceleration); return Acceleration; }
	void setGyroX(double GyroX) { lock_guard<std::mutex> lock(mtx_GyroX); this->GyroX = GyroX; }
	double getGyroX() { lock_guard<std::mutex> lock(mtx_GyroX); return GyroX; }
	void setGyroY(double GyroY) { lock_guard<std::mutex> lock(mtx_GyroY); this->GyroY = GyroY; }
	double getGyroY() { lock_guard<std::mutex> lock(mtx_GyroY); return GyroY; }
	double getDistance() { lock_guard<std::mutex> lock(mtx_distance); return distance; }
	void setDistance(double distance) { lock_guard<std::mutex> lock(mtx_distance); this->distance = distance; }

	void IMUplay();

	// פונקציה לשליטה בהאצה, האטה ועצירה
	void controlSpeed(double targetSpeed, double accelerationRate, double decelerationRate);

	// פונקציות מעקב פנימיות
	void monitorSpeedChangeProcess(
		std::string reason,
		std::function<bool(double)> isTargetReached,
		float maxDuration,
		double targetSpeed
	);
	bool isCarDecelerating(double previousSpeed, double currentSpeed);

	// פונקציות ספציפיות לפעולות נהיגה
	void stoppingImu(std::string reason);
	void slowdownImu(std::string reason);
	// מעקב תמידי אחר מהירות הרכב
	void monitorMaxSpeed();

};

