#include "GPS.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#define M_PI 3.14159265358979323846

using namespace std;
GPS::GPS() {};

void GPS::GPSplay()
{
	// Create an input file stream
	std::ifstream inputFile("GPS.txt");
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
		if (ss >> longitude >> delimiter >> latitude && delimiter == ',') {
			//setLatitude(latitude);
			//setLongitude(longitude);
			Vector2f gpsVector(longitude, latitude);
			// set data to kalman filter
			kalman->updateGPS(gpsVector);

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