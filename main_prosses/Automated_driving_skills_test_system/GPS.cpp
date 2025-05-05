#include "GPS.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
using namespace std;
GPS::GPS()
{
}
void GPS::GPSplay()
{
	// Create an input file stream
	std::ifstream inputFile("GPS.txt");
	if (!inputFile.is_open()) {
		std::cerr << "didn't success to open the file: GPS.txt" << std::endl;
		return;
	}
	string line;
	//read the file line by line
	while (std::getline(inputFile, line)) {
		// Create a stringstream object to easily parse the current line
		std::stringstream ss(line);
		double latitude, longitude;
		char delimiter;
		if (ss >> latitude >> delimiter >> longitude && delimiter == ',') {
			setLatitude(latitude);
			setLongitude(longitude);
			std::cout << "Latitude: " << getLatitude() << ", Longitude: " << getLongitude() << std::endl;
			// Pause the execution of the current thread for one second
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else {
			std::cerr << "invalid line at the file: " << line << std::endl;
		}
	}
	inputFile.close();

}