#include "GPS.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#define M_PI 3.14159265358979323846

using namespace std;
GPS::GPS()
{
}
void GPS::GPSplay()
{
	// Create an input file stream
	std::ifstream inputFile("GPS.txt");

	KalmanFilter KF(this->Latitude, this->Longitude, 0.0, this->AccelNoise, this->GyroNoise, 3.0 , 0.1f); //3.0- רעש חיישן GPS ממוצע

	if (!inputFile.is_open()) {
		globalPrint.printError("didn't success to open the file: GPS.txt");
		return;
	}
	string line;
	//read the file line by line
	while (std::getline(inputFile, line)) {
		// Create a stringstream object to easily parse the current line
		std::stringstream ss(line);
		float latitude, longitude;
		char delimiter;
		if (ss >> latitude >> delimiter >> longitude && delimiter == ',') {
            
			setLatitude(latitude);
			setLongitude(longitude);

			
			globalPrint.print("latitude: " + std::to_string(getLatitude()) + ", longitude: " + std::to_string(getLongitude()));
			// Pause the execution of the current thread for one second
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else {
			globalPrint.printError("invalid line at the file: " + line);
		}
	}
	inputFile.close();

}



//
//namespace {
//	// משתנים סטטיים לנקודת ייחוס
//	float gps_ref_latitude = 0.0;
//	float gps_ref_longitude = 0.0;
//
//	constexpr float EARTH_RADIUS = 6378137.0; // רדיוס כדור הארץ במטרים
//}
//
//void set_gps_reference(float latitude, float longitude) {
//	gps_ref_latitude = latitude;
//	gps_ref_longitude = longitude;
//}
//
//Eigen::Vector2d gps_to_local_xy(float latitude, float longitude) {
//	constexpr float DEG_TO_RAD = M_PI / 180.0;
//
//	float dLat = (latitude - gps_ref_latitude) * DEG_TO_RAD;
//	float dLon = (longitude - gps_ref_longitude) * DEG_TO_RAD;
//
//	float x = dLon * EARTH_RADIUS * std::cos(gps_ref_latitude * DEG_TO_RAD);
//	float y = dLat * EARTH_RADIUS;
//
//	return Eigen::Vector2d(x, y);
//}