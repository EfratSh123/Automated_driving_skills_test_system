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
#include "IMU.h"
#include <windows.h>
#include <map>
extern int grade; 
using namespace std;
#undef max // Prevent conflict with std::min or other methods named 'min'
IMU imu1;
extern bool onLidar;
extern bool pass; 
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
    while (onLidar) {
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
                if (angle >= -30.0f && angle <= 30.0f && distance < minDistance) {
                        minDistance = distance;
                        minAngle = angle;
                }
            }
            catch (...) {
                continue;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        scanIndex++;
    }

    inputFile.close();
}

void Lidar::monitorSafeDistance(Car& car)
{
    // מהירות הרכב הנוכחית
    float speed = car.getSpeed();
    // טבלת מרחקי עצירה (תגובה + בלימה) לפי מהירות בקמ"ש
    map<int, float> stopDistances = {
        {10, 4}, {20, 8}, {30, 12}, {40, 18} , {50, 24}, {60, 31}, {70, 39},
        {80, 47}, {90, 57}, {100, 68}, {110, 79}, {120, 90},  {130, 102}, {140, 115}
    };
    // עיגול המהירות כלפי מעלה ל-10 הקרוב כדי להתאים לטבלה
    int roundedSpeed = static_cast<int>(ceil(speed / 10.0f) * 10);
    // בדיקה אם המהירות קיימת בטווח הנתמך
    if (stopDistances.find(roundedSpeed) == stopDistances.end()) {
        globalPrint.printError("Speed out of supported range.");
        return;
    }
    // קבלת מרחק העצירה הכולל לפי הטבלה
    float totalStopDist = stopDistances[roundedSpeed];
    // הגדרת מרחק בטוח ומרחק סכנה מתוך מרחק העצירה הכולל
    float safeDistance = totalStopDist * 0.8f;    // מתחת לזה - לא בטוח
    float dangerDistance = totalStopDist * 0.4f;  // מתחת לזה - סכנת התנגשות
    int closeSeconds = 0;  // מונה שניות של מרחק לא בטוח
    float currentDistance = car.getCarDistanceFront(); // מרחק מהרכב מלפנים
    // לולאה: כל עוד המרחק קטן מהבטוח אבל לא מסוכן - נחשב כמה זמן לא בטוח
    while (currentDistance < safeDistance && currentDistance > dangerDistance) {
        closeSeconds++;
        this_thread::sleep_for(std::chrono::seconds(1));
        currentDistance = car.getCarDistanceFront(); // עדכון מרחק מהחיישן
    }
    // אם נכנס למרחק סכנה - קריאה מיידית לאירוע חריג
    if (currentDistance <= dangerDistance) {
        globalPrint.print("Danger! Too close to the vehicle ahead.");
        imu1.manageDrivingEvent("too_near_distance");
        pass = false;
    }
    // אם החזיק מרחק לא בטוח לאורך זמן - הורדת ניקוד
    else if (closeSeconds > 5) {
        globalPrint.print("Maintained unsafe distance for " + std::to_string(closeSeconds) + " seconds.");
        grade -= closeSeconds * 0.35;
    }
}
