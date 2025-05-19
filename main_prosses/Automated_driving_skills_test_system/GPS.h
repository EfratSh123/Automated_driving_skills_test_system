#pragma once
#include "globalFunc.h"
#include <mutex>
using namespace std;

class GPS
{
public:
	GPS();
	void GPSplay();
private:
	//че шезб (Latitude)
	double Latitude;
	mutex mtx_latitude;
	//че аешк (Longitude)
	double Longitude;
	mutex mtx_longitude;
	void setLatitude(double Latitude) { lock_guard<std::mutex> lock(mtx_latitude); this->Latitude = Latitude; }
	double getLatitude() { lock_guard<std::mutex> lock(mtx_latitude); return Latitude; }
	void setLongitude(double Longitude) { lock_guard<std::mutex> lock(mtx_longitude); this->Longitude = Longitude; }
	double getLongitude() { lock_guard<std::mutex> lock(mtx_longitude); return Longitude; }
	globalFunc globalPrint;
};
