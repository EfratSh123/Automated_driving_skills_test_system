#pragma once
#include <windows.h>
#include "globalFunc.h"
#include "KalmanFilter.h"
#include <mutex>
#include <cmath>
#include <Eigen/Dense>
using namespace std;

class GPS
{
public:
	GPS();
	void GPSplay();
	void setLatitude(float Latitude) { lock_guard<std::mutex> lock(mtx_latitude); this->Latitude = Latitude; }
	float getLatitude() { lock_guard<std::mutex> lock(mtx_latitude); return Latitude; }
	void setLongitude(float Longitude) { lock_guard<std::mutex> lock(mtx_longitude); this->Longitude = Longitude; }
	float getLongitude() { lock_guard<std::mutex> lock(mtx_longitude); return Longitude; }

	void setGyroNoise(float groNoise) { lock_guard<std::mutex> lock(mtx_GyroNoise); this->GyroNoise = groNoise; }
	float getGyroNoise() { lock_guard<std::mutex> lock(mtx_GyroNoise); return GyroNoise; }
	void setAccelNoise(float accelNoise) { lock_guard<std::mutex> lock(mtx_AccelNoise); this->AccelNoise = accelNoise; }
	float getAccelNoise() { lock_guard<std::mutex> lock(mtx_AccelNoise); return AccelNoise; }

	//// פונקציה שממירה קואורדינטות GPS למערכת מקומית XY
	//Eigen::Vector2d gps_to_local_xy(double latitude, double longitude);
	//// פונקציה שמגדירה את נקודת הייחוס (הנקודה ממנה מתחילים לחשב את המרחקים)
	//void set_gps_reference(double latitude, double longitude);
private:
	//קו רוחב (Latitude)
	float Latitude;
	mutex mtx_latitude;
	//קו אורך (Longitude)
	float Longitude;
	mutex mtx_longitude;
	globalFunc globalPrint;
	float GyroNoise;
	mutex mtx_GyroNoise;
	float AccelNoise;
	mutex mtx_AccelNoise;
};
