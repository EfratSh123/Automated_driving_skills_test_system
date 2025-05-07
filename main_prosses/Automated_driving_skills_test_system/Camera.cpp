#include "Camera.h"
#include <iostream>
#include "Car.h"
#include <thread>
#include <iostream>
#include <cmath>
#include <chrono> // לספירת זמן
#include <functional>
using namespace std;

Camera::Camera() : car()
{
	//constructor
}

//----------------Functions of Speed Limit----------------
void Camera::speedLimit20TraficSingh() {
	speedLimit(20);
}

void Camera::speedLimit30TraficSingh() {
	speedLimit(30);
}

void Camera::speedLimit(float speed) {
	cout << "Speed limit detected: " << speed << " km/h" << endl;
	// Set the car's speed limit to 20 km/h
	car.setMaxSpeed(speed);
	// Check if the car's current speed is above the new speed limit
	if (car.getSpeed() > car.getMaxSpeed()) {
		float speed = car.getSpeed() - car.getMaxSpeed();
		// *****בדיקה האם הדרך עירונית או בינעירונית*******
		// אפשר ע"י גוגל מפות
		// בדרך עירונית- עד 20 קמש, ביעירונית- 25 קמש
		if (1) {
			//דרך עירונית// && speed > 20 && speed < 25 || //דרך בינעירונית// && speed > 25) {
			cout << "The vehicle's speed exceeds the speed limit" << endl;
			//isPass = false;
		}
	}
}

// פונקציית עזר פרטית למעקב אחר תהליך שינוי מהירות
void Camera::monitorSpeedChangeProcess(
	string reason,                              // הסיבה לשינוי המהירות (למשל, "רמזור אדום", "מעבר חציה")
	std::function<bool(float)> isTargetReached, // פונקציה (למבדא) שמקבלת את המהירות הנוכחית ומחזירה true אם היעד הושג
	float maxDuration,                          // משך הזמן המקסימלי (בשניות) להשגת היעד
	float targetSpeed = 0.0f                    // מהירות היעד (ברירת מחדל היא 0 עבור עצירה)
) {
	cout << "Monitoring speed change due to " << reason << ". Target speed: " << targetSpeed << " km/h. Max duration: " << maxDuration << " seconds." << endl;
	// שמירת זמן ההתחלה של תהליך המעקב ברזולוציה גבוהה.
	auto startTime = chrono::high_resolution_clock::now();
	float currentCarSpeed = car.getSpeed();
	float previousCarSpeed = currentCarSpeed;

	// לולאת while שתמשיך להתבצע כל עוד הפונקציה 'isTargetReached' מחזירה false (כלומר, היעד עדיין לא הושג).
	while (!isTargetReached(currentCarSpeed)) {
		this_thread::sleep_for(chrono::seconds(1));
		currentCarSpeed = car.getSpeed();
		auto currentTime = chrono::high_resolution_clock::now();
		// חישוב משך הזמן שחלף מתחילת המעקב ועד הזמן הנוכחי, והמרתו לשניות.
		auto duration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);

		// בדיקה האם הרכב מאט
		if (isCarDecelerating(previousCarSpeed, currentCarSpeed)) {
			cout << "The car is decelerating. Speed: " << currentCarSpeed << " km/h" << endl;
		}
		// אם הרכב לא מאט והיעד עדיין לא הושג.
		else if (!isTargetReached(currentCarSpeed)) {
			cout << "The car is not progressing towards the target speed. Speed: " << currentCarSpeed << " km/h" << endl;
		}

		// עדכון המהירות עבור האיטרציה הבאה.
		previousCarSpeed = currentCarSpeed;

		// בדיקה האם משך הזמן שחלף גדול מידי.
		if (duration.count() >= maxDuration) {
			// הדפסת אזהרה שהרכב לא הגיע ליעד בזמן, ומתחילה פעולת בלימה.
			cout << "Warning: Car did not reach target within " << maxDuration << " seconds. Initiating forced action." << endl;

			// פעולת הבלימה או ההאטה
			if (targetSpeed < previousCarSpeed) {
				if (targetSpeed == 0.0f) {
					// בלימה (האטה ל-0 קמ"ש) - קצב מהיר יותר
					float emergencyDecelerationRate = 27.0f;
					car.setSpeed(max(targetSpeed, currentCarSpeed - emergencyDecelerationRate));
					// הורדת נקודות על הבלימה שהיתה יכולה להיות האטה
					// בדיקה כמה להוריד**************************************
				}
				else {
					// להוריד נקודות- לא האט כמו שצריך
				}
			}
			currentCarSpeed = car.getSpeed();
			// אם היעד הושג- יוצאים מהלולאה
			if (isTargetReached(currentCarSpeed)) break;
		}
	}
	cout << "The car reached the target due to " << reason << ". Final speed: " << currentCarSpeed << " km/h." << endl;
}
void Camera::stopping(string reason) {
	monitorSpeedChangeProcess(reason, [](float speed) { return speed <= 0; }, 2.5f, 0.0f);
}

void Camera::slowdown(string reason) {
	float speedLimit = car.getMaxSpeed();
	float targetSpeed = speedLimit;
	// במקרים של האטה בעיר או במקומות אחרים, אפשר להגדיר גם מהירות יעד קבועה כמו 30 קמ"ש
	if (reason == "crosswalk") {
		targetSpeed = 30.0f; // האטה במקרה של מעבר חציה
	}
	else if (reason == "before turn") {
		targetSpeed = 40.0f; // האטה לפני פניה
	}
	// קריאה לפונקציה שמבצעת את ההאטה
	monitorSpeedChangeProcess(reason, [targetSpeed](float speed) { return speed <= targetSpeed; }, 4.0f, targetSpeed);
}

// check if the car is decelerating
bool Camera::isCarDecelerating(float previousSpeed, float currentSpeed)
{
	return previousSpeed > currentSpeed;
}

//----------------green trafic light----------------
void Camera::stopTraficSingh() {
	//זימון פונקציית עצירה, כולל סיבת עצירה
	stopping("stop trafic singh");
}

//----------------green trafic light----------------
void Camera::greenLight() {
	cout << "Traffic light is green. Proceeding." << endl;
}

//----------------red trafic light----------------
void Camera::redLight() {
	//זימון פונקציית עצירה, כולל סיבת עצירה
	stopping("red light");
	//*****************************************************************
	//בדיקה האם להתייחס למרחק הרמזור ממני וכן האם הרמזור שייך לנתיב שלי
}

//----------------yellow trafic light----------------
void Camera::redLight() {
	cout << "Traffic light is yellow." << endl;
	//לכאורה צריך לבדוק את המרחק ולפי זה להגיד האם צריך להמשיך ולהספיק את הרמזור או לעצור

}

//----------------Pedestrians----------------
void Camera::pedestrians() {
	cout << "Pedestrian detected. Stopping." << endl;
	//זימון פונקציית עצירה, כולל סיבת עצירה
	//stopping("pedestrian");
	//בדיקה האם להתייחס למרחק ממני וכן האם הוא חוצה את הכביש שלי
}

//----------------crosswalk----------------
void Camera::crosswalk() {
	cout << "Crosswalk detected. Stopping." << endl;
	// זימון פונקציית עצירה או האטה- תלוי אם יש אנשים או לא
	slowdown("crosswalk");
	//**********************************************************************
	//האם הזימון של העצירה יהיה מכאן?
	//stopping("crosswalk");
}