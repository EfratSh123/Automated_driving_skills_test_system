#include "Lidar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <limits>
#include <cmath>
#include "Car.h"

using namespace std;
Lidar::Lidar() 
{
}

// פונקציה לעיבוד נתוני לידאר, מציאת מרחק מינימלי ובדיקת מרחק בטוח
void Lidar::processLidarData(Car& car) {
	string filePath = "Lidar.txt";
    ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
		globalPrint.printError("Error opening file: " + filePath);
        return;
    }
    string line;
    int scanIndex = 0;

    // לסדר תנאי
    while (getline(inputFile, line)) {
        // יצירת זרם מחרוזת כדי לפצל את השורה למרחקים וזוויות
        istringstream ss(line);
        string token;
        float minDistance = numeric_limits<float>::max(); // Initialize the minimum distance to the largest possible value
        float minAngle = 0.0f; // Initialize minimum distance angle
        bool vehicleDetected = false; // Flag if a car is detected nearby

        while (ss >> token) {
            size_t colonPos = token.find(':');

			if (colonPos == string::npos) continue; // invalid format

			string angleStr = token.substr(0, colonPos); // extract angle
			string distanceStr = token.substr(colonPos + 1); // extract distance

            try {
                // convert to float
                float angle = stof(angleStr);
                float distance = stof(distanceStr);

                if (angle >= -30.0f && angle <= 30.0f) {
                    if (distance < minDistance) {
                        minDistance = distance;
                        minAngle = angle;
                    }
                }
            }
            catch (...) {
                continue;
            }
        }

        // After the points in the current scan are processed
        if (minDistance < 25.0f) {
			globalPrint.print("Scan " + to_string(scanIndex) +
				": Closest object at angle " + to_string(minAngle) +
				"°, distance " + to_string(minDistance) + "m");

            // Checking whether the distance is safe
            float speedKmH = car.getSpeed();
            float safeDistance = (speedKmH / 10.0f) + 2.0f; // נוסחה משוערת

            if (minDistance < safeDistance) {
				globalPrint.print("  Warning! Too close for current speed of " +
					to_string(speedKmH) + " km/h (safe distance: " +
					to_string(safeDistance) + "m).");

                // הערה: המרחק לעצם הקרוב ביותר קטן מהמרחק הבטוח במהירות הנוכחית.
                // להוריד נקודות
            }
            vehicleDetected = true;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        scanIndex++;
    }

    inputFile.close();
}