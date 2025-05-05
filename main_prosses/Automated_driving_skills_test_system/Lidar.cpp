#include "Lidar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
using namespace std;
Lidar::Lidar()
{
}
void Lidar::Lidarplay()
{
	// Create an input file stream
	std::ifstream inputFile("Lidar.txt");
	if (!inputFile.is_open()) {
		std::cerr << "didn't success to open the file: Lidar.txt" << std::endl;
		return;
	}
	string line;
	//read the file line by line
	while (std::getline(inputFile, line)) {
		// Create a stringstream object to easily parse the current line
		std::stringstream ss(line);
		double azimuth, range;
		char delimiter;
		if (ss >> azimuth >> delimiter >> range && delimiter == ':') {
			setAzimuth(azimuth);
			setRange(range);
			std::cout << "Azimuth: " << getAzimuth() << ", Range: " << getRange() << std::endl;
			// Pause the execution of the current thread for one second
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else {
			std::cerr << "invalid line at the file: " << line << std::endl;
		}
	}
	inputFile.close();
}