#include "IMU.h"
extern bool flagSpeed;
extern int grade;
extern std::mutex mtx_grade;

#include <vector>
#include <numeric>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Car.h"
extern bool onLidar; 
IMU::IMU() {}
// הפונקציה שעוקבת אחר שינוי מהירות, ואם תוך פרק זמן מוגדר לא הושגה המהירות - מבוצעת בלימת חירום.
void IMU::monitorSpeedChangeProcess(string reason,
	float targetSpeed,
	float maxDurationSeconds,
	float emergencyDecelerationRate,
	Car& car) {
	globalPrint.print("IMU: Monitoring speed change for reason: " + reason);
	unsigned int secondsPassed = 0;
	// נעקוב אחרי הזמן שעבר ומהירות הנהג עד שמגיעים למהירות היעד או שנגמר הזמן

	float dd = car.getSpeed();

	while (secondsPassed < maxDurationSeconds && !is_target_reached(car.getSpeed(), targetSpeed)) {
		this_thread::sleep_for(chrono::milliseconds(100));
		secondsPassed++;
	}
	// בדיקה אם הושגה מהירות היעד
	if (is_target_reached(car.getSpeed(), targetSpeed)) {
		globalPrint.print("IMU: Target speed reached in time. Final speed: " + to_string(car.getSpeed()));
		return;
	}
	// אחרת, בלימת חירום והורדת ניקוד
	globalPrint.print("IMU: Target not reached in time. Triggering gradual emergency braking.");
	lock_guard<std::mutex> lock(mtx_grade);
	grade -= 10;
	while (car.getSpeed() > targetSpeed) {
		float speedNow = car.getSpeed();
		float newSpeed = max(targetSpeed, speedNow - emergencyDecelerationRate);
		car.setSpeed(newSpeed);
		globalPrint.print("IMU: Emergency braking... Speed: " + to_string(newSpeed));
		this_thread::sleep_for(chrono::seconds(1));
	}
	globalPrint.print("IMU: Emergency braking complete. Final speed: " + to_string(car.getSpeed()));
	return;
}
// ניהול אירוע נהיגה שדורש תגובה מהנהג
void IMU::manageDrivingEvent(string reason, Car& car) {
	float target_speed = get_target_speed_for_reason(reason);
	unsigned long time_limit = get_time_limit_for_reason(reason);
	monitorSpeedChangeProcess(reason, target_speed , time_limit,  25, car);
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

void IMU::monitorMaxSpeed(Car& car)
{
	bool flag = false; 
	// נעקוב כל עוד המהירות גבוהה מהמותרת
	while (car.getSpeed() > car.getMaxSpeed()) {
		globalPrint.print("Warning: Speed exceeds limit! Current: " + to_string(car.getSpeed()) + " km/h, Max: "
							+ to_string(car.getMaxSpeed()) + " km/h");
		this_thread::sleep_for(chrono::seconds(1));
		flag = true;
	}
	if (flag) {
		// הורדת ניקוד על מהירות מופזרת
		int way = 21; // Urban
		if (car.getMaxSpeed() > 60) // Interurban
			way = 26;
		if ((car.getSpeed() - car.getMaxSpeed()) > way) {
			extern std::mutex mtx_grade;
			grade -= 7;
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

void IMU::IMUplay(Car& car)
{
	// מבנה הקובץ: 
	// AccelX, AccelY ,GyroX
	ifstream inputFile("IMU.txt");
	if (!inputFile.is_open()) {
		globalPrint.printError("didn't success to open the file: IMU.txt");
		return;
	}
	string line;
	while (onLidar && getline(inputFile, line)) {
		stringstream ss(line);
		float accelX, accelY,  gyroX;
		char comma;
		if (ss >> accelX >> comma >> accelY >> comma >> gyroX) {
			if (kalman) {
				kalman->predict(dt, accelX, gyroX, ref(car)); // שלב תחזית
				kalman->updateIMU(accelX, gyroX);   // שלב עדכון
			}
			float speed = car.getSpeed(); // distance in km/h
			distance += (speed * 1000.0f / 3600.0f) * dt;  // 1000=1 k"m, 3600 secoend=1 hour המרחק שעבר מאז הקריאה האחרונה
			car.setDistance(car.getDistance() + distance); // Update the distance the car has traveled

			globalPrint.print(
				string("Acceleration: ") + to_string(getAccelX()) + to_string(getAccelX()) +
				", speed: " + to_string(car.getSpeed()) +
				", GyroX: " + to_string(getGyroX())
			);

			// Pause the execution of the current thread for one second
		}
		else {
			globalPrint.printError("Invalid line in IMU.txt: " + line);
		}
		this_thread::sleep_for(std::chrono::milliseconds(100));

	}
	inputFile.close();
}

