#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
using namespace std;	
class IMU
{
public:
	IMU();
private:
	double Acceleration;
private:
	double speed;
private:
	double GyroX;
private:
	double GyroY;
private:
	double GyroZ;

public:
	void setAcceleration(double Acceleration) { this->Acceleration = Acceleration; }
public:
	double getAcceleration() { return Acceleration; }
public:
	void setSpeed(double speed) { this->speed = speed; }
public:
	double getSpeed() { return speed; }
public:
	void setGyroX(double GyroX) { this->GyroX = GyroX; }
public:
	double getGyroX() { return GyroX; }
public:
	void setGyroY(double GyroY) { this->GyroY = GyroY; }
public:
	double getGyroY() { return GyroY; }
public:
	void setGyroZ(double GyroZ) { this->GyroZ = GyroZ; }
public:
	double getGyroZ() { return GyroZ; }
	void IMUplay();
};

