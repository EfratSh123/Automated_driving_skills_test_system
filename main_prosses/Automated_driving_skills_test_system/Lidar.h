#pragma once
#include "Car.h" // Ensure Car is included
#include "globalFunc.h"
#include <mutex>
using namespace std;

class Lidar : public Car // Explicitly inherit from Car
{
private:
	//זווית אופקית (Azimuth)
	double Azimuth;
	mutex mtx_Azimuth;
	//טווח (Range)
	double Range;
	mutex mtx_Range;

	globalFunc globalPrint;

public:
	void processLidarData(Car& car);
	Lidar();
	void setAzimuth(double Azimuth) { lock_guard<std::mutex> lock(mtx_Azimuth); this->Azimuth = Azimuth; }
	double getAzimuth() { lock_guard<std::mutex> lock(mtx_Azimuth); return Azimuth; }
	void setRange(double Range) { lock_guard<std::mutex> lock(mtx_Range); this->Range = Range; }
	double getRange() { lock_guard<std::mutex> lock(mtx_Range); return Range; }
};