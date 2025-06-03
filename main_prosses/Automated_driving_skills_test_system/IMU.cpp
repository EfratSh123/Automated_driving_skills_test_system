#include "IMU.h"
extern bool flagSpeed;
extern bool pass;
extern int grade;
#include <vector>
#include <numeric>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
IMU::IMU()
{
}
IMU::IMU(Car* car) : carPtr(car), Acceleration(0.0), GyroX(0.0), GyroY(0.0)
{
}
// הפונקציה שעוקבת אחר שינוי מהירות, ואם תוך פרק זמן מוגדר לא הושגה המהירות - מבוצעת בלימת חירום.
void IMU::monitorSpeedChangeProcess(string reason,
	float targetSpeed,
	float maxDurationSeconds,
	float emergencyDecelerationRate) {
	globalPrint.print("IMU: Monitoring speed change for reason: " + reason);
	unsigned int secondsPassed = 0;
	// נעקוב אחרי הזמן שעבר ומהירות הנהג עד שמגיעים למהירות היעד או שנגמר הזמן
	while (secondsPassed < maxDurationSeconds && !is_target_reached(carPtr->getSpeed(), targetSpeed)) {
		this_thread::sleep_for(chrono::seconds(1));
		secondsPassed++;
	}
	// בדיקה אם הושגה מהירות היעד
	if (is_target_reached(carPtr->getSpeed(), targetSpeed)) {
		globalPrint.print("IMU: Target speed reached in time. Final speed: " + to_string(carPtr->getSpeed()));
		return;
	}
	// אחרת, בלימת חירום והורדת ניקוד
	globalPrint.print("IMU: Target not reached in time. Triggering gradual emergency braking.");
	pass = false;
	grade -= 10;
	while (carPtr->getSpeed() > targetSpeed) {
		float speedNow = carPtr->getSpeed();
		float newSpeed = max(targetSpeed, speedNow - emergencyDecelerationRate);
		carPtr->setSpeed(newSpeed);
		globalPrint.print("IMU: Emergency braking... Speed: " + to_string(newSpeed));
		this_thread::sleep_for(chrono::seconds(1));
	}
	globalPrint.print("IMU: Emergency braking complete. Final speed: " + to_string(carPtr->getSpeed()));
	return;
}
// ניהול אירוע נהיגה שדורש תגובה מהנהג
void IMU::manageDrivingEvent(string reason) {
	float target_speed = get_target_speed_for_reason(reason);
	unsigned long time_limit = get_time_limit_for_reason(reason);
	monitorSpeedChangeProcess(reason, target_speed , time_limit,  25);
}
// מחזירה מהירות יעד לפי הסיבה
float IMU::get_target_speed_for_reason(string reason) {
	if (reason == "red_light" || reason == "stop" || reason == "pedestrian") return 0.0; 
	return 20;
}
// מחזירה מגבלת זמן להתאמה לפי הסיבה
unsigned long IMU::get_time_limit_for_reason(string reason) {
	if (reason == "red_light" || reason == "stop" || reason == "pedestrian") return 2000;
	return 3000;
}

void IMU::monitorMaxSpeed(Car &c)
{
	bool flag = false; 
	// נעקוב כל עוד המהירות גבוהה מהמותרת
	while (c.getSpeed() > c.getMaxSpeed()) {
		globalPrint.print("Warning: Speed exceeds limit! Current: " + to_string(c.getSpeed()) + " km/h, Max: "
							+ to_string(c.getMaxSpeed()) + " km/h");
		this_thread::sleep_for(chrono::seconds(1));
		flag = true;
	}
	if (flag) {
		// הורדת ניקוד על מהירות מופזרת
		int way = 21; // Urban
		if (c.getMaxSpeed() > 60) // Interurban
			way = 26;
		if ((c.getSpeed() - c.getMaxSpeed()) > way) {
			pass = false;
		}
	}
}

// Function that checks if the vehicle is decelerating.
bool IMU::isCarDecelerating(float previousSpeed, float currentSpeed) {
	return previousSpeed > currentSpeed;
}

// בדיקה אם מהירות היעד הושגה
bool IMU::is_target_reached(float current_speed, float target_speed) {
	return abs(current_speed - target_speed) < 0.3;
}

// חישוב סטיית תקן של נתונים
float IMU::computeStdDev(const std::vector<float>& data) {
	float mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
	float sumSq = 0.0;
	for (float val : data) {
		sumSq += (val - mean) * (val - mean);
	}
	return std::sqrt(sumSq / data.size());
}
// פונקציה שמחשבת רעש חיישן IMU על סמך נתונים מקובץ IMU.txt
void IMU::computeSensorNoise(GPS* gps)
{
	ifstream inputFile("IMU.txt");
	if (!inputFile.is_open()) {
		globalPrint.printError("Failed to open IMU.txt");
		AccelNoise = 0.1;
		GyroNoise = 0.1;
		return;
	}
	vector<float> accelData;
	vector<float> gyroVecData;
	string line;
	while (getline(inputFile, line)) {
		stringstream ss(line);
		float acceleration, currentSpeed, gx, gy;
		if (ss >> acceleration >> currentSpeed >> gx >> gy) 
		{
			accelData.push_back(acceleration);
			float gyroMagnitude = std::sqrt(gx * gx + gy * gy);
			gyroVecData.push_back(gyroMagnitude);
			if (!accelData.empty() && !gyroVecData.empty()) {
				AccelNoise = computeStdDev(accelData);
				GyroNoise = computeStdDev(gyroVecData);
			}
			else {
				globalPrint.printError("IMU.txt contains insufficient or invalid data");
				AccelNoise = 0.1;
				GyroNoise = 0.1;
			}
			gps->setAccelNoise(AccelNoise);
			gps->setGyroNoise(GyroNoise);
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
	inputFile.close();
}


void IMU::IMUplay()
{
	float speed;
	// Create an input file stream
	ifstream inputFile("IMU.txt");
	if (!inputFile.is_open()) {
		globalPrint.printError("didn't success to open the file: IMU.txt");
		return;
	}
	string line;
	//read the file line by line
	while (getline(inputFile, line)) {
		stringstream ss(line);
		float acceleration, currentSpeed, gyroX, gyroY;

		if (ss >> acceleration >> currentSpeed >> gyroX >> gyroY ) {
			setAcceleration(acceleration);
			carPtr->setSpeed(currentSpeed);
			setGyroX(gyroX);
			setGyroY(gyroY);

		////
			// חישוב גודל וקטור המהירות הזויתית במישוק XY
			speed = sqrt((getGyroX() * getGyroX()) + (getGyroY() * getGyroY())); // Calculate the total speed
			// מחשב את התאוצה הזוויתית על ידי מציאת השינוי במהירות הזוויתית ביחס למהירות הזוויתית הקודמת ומחלק אותו במרווח הזמן (dt)
			acceleration = (speed - carPtr->getPrevSpeed()) / dt; // Calculate acceleration
			// המרת המהירות הזויתית לקמש והכפלתה בזמן שעבר
			distance = (speed * 1000 / 3600 * getStartTime()); // 1000=1 k"m, 3600 secoend=1 hour

			carPtr->setDistance(carPtr->getDistance() + distance); // Update the speed the car has traveled
		////

			globalPrint.print(
				string("Acceleration: ") + to_string(getAcceleration()) +
				", speed: " + to_string(carPtr->getSpeed()) +
				", GyroX: " + to_string(getGyroX()) +
				", GyroY: " + to_string(getGyroY())
			);

			// Pause the execution of the current thread for one second
			this_thread::sleep_for(chrono::seconds(1));
			updateStartTimeByOneSecond();
		}
		else {
			globalPrint.printError("Invalid line in IMU.txt: " + line);
		}
	}
	inputFile.close();
}

