#pragma once
#include <windows.h>

#include "Car.h" // Ensure Car is included
#include "globalFunc.h"
#include <mutex>
using namespace std;

class Lidar : public Car // Explicitly inherit from Car
{
private:
	//זווית אופקית (Azimuth)
	float Azimuth;
	mutex mtx_Azimuth;
	//טווח (Range)
	float Range;
	mutex mtx_Range;

	globalFunc globalPrint;

public:
	void processLidarData(Car& car);
	Lidar();
	void setAzimuth(float Azimuth) { lock_guard<std::mutex> lock(mtx_Azimuth); this->Azimuth = Azimuth; }
	float getAzimuth() { lock_guard<std::mutex> lock(mtx_Azimuth); return Azimuth; }
	void setRange(float Range) { lock_guard<std::mutex> lock(mtx_Range); this->Range = Range; }
	float getRange() { lock_guard<std::mutex> lock(mtx_Range); return Range; }
	void monitorSafeDistance(Car& car) {};
};
