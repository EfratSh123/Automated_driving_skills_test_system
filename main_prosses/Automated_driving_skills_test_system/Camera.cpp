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

namespace fs = std::filesystem;
fs::path latestFile;
extern bool pass; 
using namespace std;
extern bool onyolo;
extern bool onCnn;

Camera::Camera() : car(), imu(new IMU(&car))
{
}

void Camera::speedLimit(float speed) {
	globalPrint.print("Speed limit detected: " + to_string(speed) + " km/h");
	car.setMaxSpeed(speed);
}

void Camera::stopSign() {
	imu->manageDrivingEvent("stop");
}

void Camera::greenLight() {
	globalPrint.print("Traffic light is green. Proceeding.");
	if (car.getSpeed() <= 0) {
		// *****************
	}
}

void Camera::redLight() {
	imu->manageDrivingEvent("red_light");
}

void Camera::pedestrians() {
	globalPrint.print("Pedestrian detected. Stopping.");
	// בדיקת מיקום האנשים- אם הוא על הכביש לעצור
	// ********************************************
	//imu->manageDrivingEvent("pedestrian");
}

void Camera::crosswalk() {
	globalPrint.print("Crosswalk detected. Slowing down.");
	// זימון פונקצית האטה, אם יש אנשים הוא יזהה אנשים ויזמן דרכם פונקצית עצירה
	// *********************************************
	imu->manageDrivingEvent("crosswalk");
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

// פונקציה לקבלת הנתיב של הקובץ האחרון שנוצר בתיקייה
string Camera::getLatestFile(const string& folderPath) {
	fs::path latestFile;
	auto latestWriteTime = fs::file_time_type::min();
	if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
		for (const auto& entry : fs::directory_iterator(folderPath)) {
			if (fs::is_regular_file(entry.path())) {
				auto writeTime = fs::last_write_time(entry.path());
				if (writeTime > latestWriteTime) {
					latestWriteTime = writeTime;
					latestFile = entry.path();
				}
			}
		}
	}
	return latestFile.string();
}

//עיבוד קבצי זיהוי YOLO
void Camera::processYOLODetections(
	const string& yoloDataPath,
	const unordered_map<string, function<void()>>& yoloObjectToActionMap,
	float minConfidence
) {
	// זימון מודל יולו על הסרטה
	string videoPath = "C:\\Users\\User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\12345.mp4";
	string yoloScript = "C:\\Users\\User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model_video.py";
	string yoloCommand = "python \"" + yoloScript + "\" --video \"" + videoPath + "\"";
	system(yoloCommand.c_str());

	string latestYoloFile;
	while (onyolo) {
		latestYoloFile = getLatestFile(yoloDataPath);
		if (!latestYoloFile.empty()) {
			processDetectionFile(latestYoloFile, yoloObjectToActionMap, minConfidence);
		}
		this_thread::sleep_for(chrono::seconds(1));
	}
}
// עיבוד קבצי זיהוי R-CNN
void Camera::processCNNDetections(
	const string& rcnnDataPath,
	const unordered_map<string, function<void()>>& cnnObjectToActionMap,
	float minConfidence
) {
	string latestRcnnFile;
	// זימון מודל CNN על ההסרטה
	string videoPath = "C:\\Users\\User\\ProjectEfratSh\\fasterRcnn\\Faster_R-CNN_new_tarin\\orginized_dataset\\211.mp4";
	string cnnScript = "C:\\Users\\User\\ProjectEfratSh\\fasterRcnn\\Faster_R-CNN_new_tarin\\orginized_dataset\\valid.py";
	string cnnCommand = "python \"" + cnnScript + "\" --video \"" + videoPath + "\"";
	system(cnnCommand.c_str());

	while (onCnn) {
		latestRcnnFile = getLatestFile(rcnnDataPath);
		if (!latestRcnnFile.empty()) {
			processDetectionFile(latestRcnnFile, cnnObjectToActionMap, minConfidence);
		}
		this_thread::sleep_for(chrono::seconds(1));
	}
}

void Camera::processDetectionFile(
	const string& filePath,
	const unordered_map<string, function<void()>>& objectToActionMap,
	float minConfidence
) {
	if (filePath.empty()) {
		return;
	}
	globalPrint.print("Processing detection file: " + filePath);
	vector<string> lines = readFileLines(filePath);
	for (const string& line : lines) {
		stringstream ss(line);
		string objectName;
		float confidence;
		vector<float> boundingBox(4);
		if (ss >> objectName >> boundingBox[0] >> boundingBox[1] >> boundingBox[2] >> boundingBox[3] >> confidence) {
			if (confidence >= minConfidence) {
				globalPrint.print("Detected: " + objectName + " (Confidence: " + to_string(confidence) + ")");
				// חיפוש במפה
				if (objectToActionMap.count(objectName)) {
					objectToActionMap.at(objectName)();
				}
				else {
					globalPrint.printError("Invalid format in detection line: " + line);
				}
			}
		}
	}
}

void Camera::processDetections() {
	const float MIN_CONFIDENCE = 0.70f;
	string yoloDataPath = "C:\\Users/User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model\\new_folder\\labels_simple";
	string rcnnDataPath = "C:\\Users\\User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\labels_simple";
	// קריאה ישירה לפונקציות העיבוד
	processYOLODetections(yoloDataPath, yoloObjectToActionMap, MIN_CONFIDENCE);
	processCNNDetections(rcnnDataPath, cnnObjectToActionMap, MIN_CONFIDENCE);
	globalPrint.print("Camera: YOLO and CNN detections processed sequentially.");
}