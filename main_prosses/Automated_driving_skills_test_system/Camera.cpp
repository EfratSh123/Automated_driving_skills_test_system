#include "Camera.h"
#include <iostream>
#include "Car.h"
#include "IMU.h"
#include <thread>
#include <iostream>
#include <cmath>
#include <chrono> 
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <windows.h>
#undef min 
extern int grade;
extern std::mutex mtx_grade;

namespace fs = std::filesystem;
fs::path latestFile;
using namespace std;
extern bool onyolo;
extern bool onCnn;

Camera::Camera() {};

// פונקציה חדשה לאתחול המאפים
void Camera::initializeMaps(Car& carRef) {
	// אתחול yoloObjectToActionMap
	{
		lock_guard<std::mutex> lock(mtx_yoloObjectToActionMap);
		yoloObjectToActionMap = {
			{"pedestrian", [this](Car& car) { this->pedestrians(car); }},
			{"crosswalk",  [this](Car& car) { this->crosswalk(car); }}
		};
	}

	// אתחול cnnObjectToActionMap
	{
		lock_guard<std::mutex> lock(mtx_cnnObjectToActionMap);
		cnnObjectToActionMap = {
			{"red_light_left",      [this](Car& car) { if (car.getDirection() == "left") this->redLight(car); }},
			{"red_light_right",     [this](Car& car) { if (car.getDirection() == "right") this->redLight(car); }},
			{"red_light",           [this](Car& car) { this->redLight(car); }},
			{"red_light_straight",  [this](Car& car) { if (car.getDirection() == "straight") this->redLight(car); }},
			{"green_light",         [this](Car& car) { this->greenLight(car); }},
			{"green_light_left",    [this](Car& car) { if (car.getDirection() == "left") this->greenLight(car); }},
			{"green_light_right",   [this](Car& car) { if (car.getDirection() == "right") this->greenLight(car); }},
			{"green_light_straight",[this](Car& car) { if (car.getDirection() == "straight") this->greenLight(car); }},
			{"stop",                [this](Car& car) { this->stopSign(car); }},
			{"stop_trafic_singh",   [this](Car& car) { this->stopSign(car); }},
			{"speed_limit_20",      [this](Car& car) { this->speedLimit(20, car); }},
			{"speed_limit_30",      [this](Car& car) { this->speedLimit(30, car); }},
			{"speed_limit_60",      [this](Car& car) { this->speedLimit(60, car); }},
			{"speed_limit_80",      [this](Car& car) { this->speedLimit(80, car); }},
			{"speed_limit_100",     [this](Car& car) { this->speedLimit(100,car); }},
			{"speed_limit_120",     [this](Car& car) { this->speedLimit(120,car); }},
			// {"pedestrian",        [&carRef, this]() { this->pedestrians(carRef); }}, // תמרור הולכי רגל, יעזור מתי שלא מזהה טוב את המעבר חציה
		};
	}
}

void Camera::speedLimit(float speed, Car& car) {
	globalPrint.print("Speed limit detected: " + to_string(speed) + " km/h");
	car.setMaxSpeed(speed);
}

void Camera::stopSign(Car& car) {
	imu->manageDrivingEvent("stop", ref(car));
}

void Camera::greenLight(Car& car) {
	globalPrint.print("Traffic light is green. Proceeding.");
	// אם לאחר שתי שניות הוא עדיין לא המשיך למרות שאין לפניו רכב שתוקע אותו- להוריד ניקוד
	this_thread::sleep_for(chrono::seconds(2));
	if (car.getSpeed() <= 0 && car.getCarDistanceFront() > 50) {
		std::lock_guard<std::mutex> lock(mtx_grade);
		grade -= 5; // הורדת ניקוד על עצירה מיותרת ברמזור ירוק
	}
}

void Camera::redLight(Car& car) {
	imu->manageDrivingEvent("red_light", ref(car));
}

void Camera::pedestrians(Car& car) {
	globalPrint.print("Pedestrian detected. Stopping.");
	// בדיקת מיקום האנשים- אם הוא על הכביש לעצור
	// *************************************************************************************
	//imu->manageDrivingEvent("pedestrian");
}

void Camera::crosswalk(Car& car) {
	globalPrint.print("Crosswalk detected. Slowing down.");
	// זימון פונקצית האטה, אם יש אנשים הוא יזהה אנשים ויזמן דרכם פונקצית עצירה
	// *************************************************************************************
	imu->manageDrivingEvent("crosswalk", ref(car));
}

// פונקציה עזר לקריאת קובץ טקסט והחזרת וקטור של שורות
vector<string>  Camera::readFileLines(const string& filePath) {
	ifstream file(filePath);
	string line;
	vector<string> lines;
	if (file.is_open()) {
		while (getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else {
		globalPrint.printError("Unable to open file: " + filePath);
	}
	return lines;
}

void Camera::processDetectionFile(
	string& filePath,
	unordered_map<string, function<void(Car&)>>& objectToActionMap,
	float minConfidence, 
	Car& car
) {
	if (filePath.empty()) return;

	globalPrint.print("Processing detection file: " + filePath);
	vector<string> lines = readFileLines(filePath);
	for (const string& line : lines) {
		stringstream ss(line);
		string objectName;
		float x, y, w, h, confidence;
		if (ss >> objectName >> confidence >> x >> y >> w >> h) {
			std::cout << "Parsed: " << objectName << " " << confidence << " " << x << " " << y << " " << w << " " << h << std::endl;
			if (confidence >= minConfidence && (objectToActionMap.count(objectName)))
				objectToActionMap.at(objectName)(car);
			else
				globalPrint.printError("Invalid format in detection line: " + line);
		}
		else {
			std::cout << "Failed to parse line!" << std::endl;
		}
	}
}

void Camera::runCNNModelLoop() {
	string videoPath = "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\videoTest.mp4";
	string cnnScript = "C:\\Users\\User\\ProjectEfratSh\\fasterRcnn\\Faster_R-CNN_new_tarin\\orginized_dataset\\try_model_on_video.py";
	string cnnCommand = "python \"" + cnnScript + "\" --video \"" + videoPath + "\"";
	system(cnnCommand.c_str());
}

void Camera::runYoloModelLoop() {
	string videoPath = "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\videoTest.mp4";
	string yoloScript = "C:\\Users\\User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model_video_text.py";
	string yoloCommand = "python \"" + yoloScript + "\" --video \"" + videoPath + "\"";
	system(yoloCommand.c_str());
}

void Camera::processCNNFilesLoop(
	string& rcnnDataPath,
	unordered_map<string, function<void(Car&)>>& cnnObjectToActionMap,
	float minConfidence, 
	Car& car
) {
	while (onCnn) {
		string latestFile = globalPrint.getLatestFile(rcnnDataPath);

		if (!latestFile.empty()) {
			processDetectionFile(latestFile, cnnObjectToActionMap, minConfidence, ref(car));

			// מחיקת הקובץ לאחר עיבוד
			if (remove(latestFile.c_str()) != 0) {
				globalPrint.printError("Failed to delete file: " + latestFile);
			}
			else {
				globalPrint.print("Deleted file: " + latestFile);
			}
		}
		else {
			// אין קובץ חדש – נחכה רגע
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
}

void Camera::processYoloFilesLoop(
	string& yoloDataPath,
	unordered_map<string, function<void(Car&)>>& yoloObjectToActionMap,
	float minConfidence, 
	Car& car
) {
	while (onCnn) {
		string latestFile = globalPrint.getLatestFile(yoloDataPath);

		if (!latestFile.empty()) {
			processDetectionFile(latestFile, yoloObjectToActionMap, minConfidence, ref(car));

			// מחיקת הקובץ לאחר עיבוד
			if (remove(latestFile.c_str()) != 0) {
				globalPrint.printError("Failed to delete file: " + latestFile);
			}
			else {
				globalPrint.print("Deleted file: " + latestFile);
			}
		}
		else {
			// אין קובץ חדש – נחכה רגע
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
}
