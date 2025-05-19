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
namespace fs = std::filesystem;
fs::path latestFile;

using namespace std;
extern bool onyolo;
extern bool onCnn;

Camera::Camera() : car(), imu(new IMU(&car))
{
}

void Camera::speedLimit20TraficSingh() {
	speedLimit(20);
}

void Camera::speedLimit30TraficSingh() {
	speedLimit(30);
}

// אם יש מהירות מקסימלית לפי קאורדינטות- מיותר
void Camera::speedLimit(float speed) {
	globalPrint.print("Speed limit detected: " + to_string(speed) + " km/h");
	car.setMaxSpeed(speed);
	//// Check if the car's current speed is above the new speed limit
	//if (car.getSpeed() > car.getMaxSpeed()) {
	//	float speed = car.getSpeed() - car.getMaxSpeed();
	//	// *****בדיקה האם הדרך עירונית או בינעירונית*******
	//	// אפשר ע"י גוגל מפות
	//	// בדרך עירונית- עד 20 קמש, ביעירונית- 25 קמש
	//	if (1) {
	//		//דרך עירונית// && speed > 20 && speed < 25 || //דרך בינעירונית// && speed > 25) {
	//		cout << "The vehicle's speed exceeds the speed limit" << endl;
	//		//isPass = false;
	//	}
	//}
}

// Camera::monitorSpeedChangeProcess(
//std::string reason,
//std::function<bool(float)> isTargetReached,
//float maxDuration,
//float targetSpeed
//
//cout << "Monitoring speed change due to " << reason << ". Target speed: " << targetSpeed << " km/h. Max duration: " << maxDuration << " seconds." << endl;
//// שמירת זמן ההתחלה של תהליך המעקב ברזולוציה גבוהה.
//auto startTime = chrono::high_resolution_clock::now();
//float currentCarSpeed = car.getSpeed();
//float previousCarSpeed = currentCarSpeed;
//
//// לולאת while שתמשיך להתבצע כל עוד הפונקציה 'isTargetReached' מחזירה false (כלומר, היעד עדיין לא הושג).
//while (!isTargetReached(currentCarSpeed)) {
//	this_thread::sleep_for(chrono::seconds(1));
//	currentCarSpeed = car.getSpeed();
//	auto currentTime = chrono::high_resolution_clock::now();
//	// חישוב משך הזמן שחלף מתחילת המעקב ועד הזמן הנוכחי, והמרתו לשניות.
//	auto duration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
//
//	// בדיקה האם הרכב מאט
//	if (isCarDecelerating(previousCarSpeed, currentCarSpeed)) {
//		cout << "The car is decelerating. Speed: " << currentCarSpeed << " km/h" << endl;
//	}
//	// אם הרכב לא מאט והיעד עדיין לא הושג.
//	else if (!isTargetReached(currentCarSpeed)) {
//		cout << "The car is not progressing towards the target speed. Speed: " << currentCarSpeed << " km/h" << endl;
//	}
//
//	// עדכון המהירות עבור האיטרציה הבאה.
//	previousCarSpeed = currentCarSpeed;
//
//	// בדיקה האם משך הזמן שחלף גדול מידי.
//	if (duration.count() >= maxDuration) {
//		// הדפסת אזהרה שהרכב לא הגיע ליעד בזמן, ומתחילה פעולת בלימה.
//		cout << "Warning: Car did not reach target within " << maxDuration << " seconds. Initiating forced action." << endl;
//
//		// פעולת הבלימה או ההאטה
//		if (targetSpeed < previousCarSpeed) {
//			if (targetSpeed == 0.0f) {
//				// בלימה (האטה ל-0 קמ"ש) - קצב מהיר יותר
//				float emergencyDecelerationRate = 27.0f;
//				car.setSpeed(max(targetSpeed, currentCarSpeed - emergencyDecelerationRate));
//				// הורדת נקודות על הבלימה שהיתה יכולה להיות האטה
//				// בדיקה כמה להוריד************************************************
//			}
//			else {
//				// להוריד נקודות- לא האט כמו שצריך
//			}
//		}
//		currentCarSpeed = car.getSpeed();
//		// אם היעד הושג- יוצאים מהלולאה
//		if (isTargetReached(currentCarSpeed)) break;
//	}
//}
//cout << "The car reached the target due to " << reason << ". Final speed: " << currentCarSpeed << " km/h." << endl;
//}

void Camera::stopping(string reason) {
	imu->stoppingImu(reason);
}

void Camera::slowdown(string reason) {
	float speedLimit = car.getMaxSpeed();
	
	float targetSpeed = speedLimit;
	if (reason == "crosswalk") {
		targetSpeed = 30.0f;
	}
	else if (reason == "before turn") {
		targetSpeed = 40.0f;
	}
	imu->slowdownImu(reason);
}

void Camera::stopSign() {
	stopping("stop trafic singh");
}

void Camera::greenLight() {
	globalPrint.print("Traffic light is green. Proceeding.");
	// האם לבדוק מהירות- אם הוא היה בעצירה לבדוק אם הוא המשיך והאם להתערב במקרה שלא?
	
	//************************************************************

}

void Camera::redLight() {
	stopping("red light");
	//הנחה- כאשר אני רואה רמזור אדום הוא שייך אלי ואני מיד צריכה לעצור
}

//----------------yellow trafic light----------------
//void Camera::yellowLight() {
//	cout << "Traffic light is yellow." << endl;
//}

void Camera::pedestrians() {
	globalPrint.print("Pedestrian detected. Stopping.");
	//זימון פונקציית עצירה, כולל סיבת עצירה
	//stopping("pedestrian");
	//בדיקה האם להתייחס למרחק ממני וכן האם הוא חוצה את הכביש שלי
}

void Camera::crosswalk() {
	globalPrint.print("Crosswalk detected. Slowing down.");
	// זימון פונקצית האטה, אם יש אנשים הוא יזהה אנשים ויזמן דרכם פונקצית עצירה
	slowdown("crosswalk");

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
	const unordered_map<string, CameraFunction>& yoloObjectToActionMap,
	float minConfidence
) {
	string latestYoloFile;
	while (onyolo) {
		latestYoloFile = getLatestFile(yoloDataPath);
		if (!latestYoloFile.empty()) {
			processDetectionFile(latestYoloFile, yoloObjectToActionMap, minConfidence);
		}
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}
// עיבוד קבצי זיהוי R-CNN
void Camera::processCNNDetections(
	const string& rcnnDataPath,
	const unordered_map<string, CameraFunction>& cnnObjectToActionMap,
	float minConfidence
) {
	string latestRcnnFile;
	while (onCnn) {
		latestRcnnFile = getLatestFile(rcnnDataPath);
		if (!latestRcnnFile.empty()) {
			processDetectionFile(latestRcnnFile, cnnObjectToActionMap, minConfidence);
		}
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

void Camera::processDetectionFile(
	const string& filePath,
	const unordered_map<string, CameraFunction>& objectToActionMap,
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
				if (objectToActionMap.count(objectName)) {
					(this->*objectToActionMap.at(objectName))();
				}
				else if (objectName.rfind("speed_limit", 0) == 0) {
					stringstream ssSpeed(objectName);
					string speedPart1, speedPart2, speedValueStr;
					if (ssSpeed >> speedPart1 >> speedPart2 >> speedValueStr) {
						try {
							float speed = stof(speedValueStr);
							if (speed == 20) {
								speedLimit20TraficSingh();
							}
							else if (speed == 30) {
								speedLimit30TraficSingh();
							}
						}
						catch (const invalid_argument& e) {
							globalPrint.printError("Invalid speed value in line: " + line);
						}
						catch (const out_of_range& e) {
							globalPrint.print("Speed value out of range in line: " + line);
						}
					}
				}
			}
		}
		else {
			globalPrint.printError("Invalid format in detection line: " + line);
		}
	}
}

//void Camera::startDetectionThreads() {
//	onyolo = true;
//	const float MIN_CONFIDENCE = 0.70f;
//	string yoloDataPath = "C:\\Users/User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model\\new_folder\\labels_simple";
//	string rcnnDataPath = "rcnn_detections\\";
//
//	// מפה עבור זיהויי YOLO
//	map<string, CameraFunction> yoloObjectToActionMap = {
//		{"pedestrian", &Camera::pedestrians},
//		{"crosswalk", &Camera::crosswalk}
//	};
//
//	// מפה מאוחדת עבור זיהויי CNN (כולל הגבלות מהירות)
//	map<string, CameraFunction> cnnObjectToActionMap = {
//		{"stop_trafic_singh", &Camera::stopSign},
//		{"green_light", &Camera::greenLight},
//		{"red_light", &Camera::redLight},
//		{"speed_limit_20", &Camera::speedLimit20TraficSingh},
//		{"speed_limit_30", &Camera::speedLimit30TraficSingh},
//	};
//
//	yoloThread = thread(&Camera::processYOLODetections, this,
//		yoloDataPath, yoloObjectToActionMap, MIN_CONFIDENCE);
//
//	cnnThread = thread(&Camera::processCNNDetections, this,
//		rcnnDataPath, cnnObjectToActionMap, MIN_CONFIDENCE);
//
//	cout << "Camera: YOLO and CNN detection threads started." << endl;
//}
//
//
//void Camera::stopDetectionThreads() {
//	onyolo = false;
//	if (yoloThread.joinable()) {
//		yoloThread.join();
//		cout << "Camera: YOLO detection thread stopped." << endl;
//	}
//	if (cnnThread.joinable()) {
//		cnnThread.join();
//		cout << "Camera: CNN detection thread stopped." << endl;
//	}
//}


void Camera::processDetections() {
	const float MIN_CONFIDENCE = 0.70f;
	std::string yoloDataPath = "C:\\Users/User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model\\new_folder\\labels_simple";
	std::string rcnnDataPath = "rcnn_detections\\";

	unordered_map<std::string, CameraFunction> yoloObjectToActionMap = {
	{"pedestrian", &Camera::pedestrians},
	{"crosswalk", &Camera::crosswalk}
	};

	unordered_map<std::string, CameraFunction> cnnObjectToActionMap = {
		{"stop_trafic_singh", &Camera::stopSign},
		{"green_light", &Camera::greenLight},
		{"red_light", &Camera::redLight},
		{"speed_limit_20", &Camera::speedLimit20TraficSingh},
		{"speed_limit_30", &Camera::speedLimit30TraficSingh}
	};

	// קריאה ישירה לפונקציות העיבוד
	processYOLODetections(yoloDataPath, yoloObjectToActionMap, MIN_CONFIDENCE);
	processCNNDetections(rcnnDataPath, cnnObjectToActionMap, MIN_CONFIDENCE);

	globalPrint.print("Camera: YOLO and CNN detections processed sequentially.");
}



//void Camera::processDetections() {
//	onyolo = false;
//	// סף בטחון לזיהוי
//	const float MIN_CONFIDENCE = 0.70f;
//	std::string yoloDataPath = "C:\\Users/User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\try_model\\new_folder\\labels_simple";
//	std::string rcnnDataPath = "rcnn_detections\\";
//
//	// מפה המקשרת שמות עצמים מזוהים לפונקציות המתאימות במחלקה Camera
//	std::map<std::string, CameraFunction> objectToActionMap = {
//		{"stop_trafic_singh", &Camera::stopSign},
//		{"green_light", &Camera::greenLight},
//		{"red_light", &Camera::redLight},
//		{"pedestrian", &Camera::pedestrians},
//		{"crosswalk", &Camera::crosswalk}
//	};
//
//	std::map<std::string, SpeedLimitFunction> speedLimitMap = {
//		{"speed_limit_20", &Camera::speedLimit20TraficSingh},
//		{"speed_limit_30", &Camera::speedLimit30TraficSingh}
//	};
//	//**********************************************************
//	while (onyolo) {
//		// עיבוד קובץ YOLO האחרון
//		std::string latestYoloFile = getLatestFile(yoloDataPath);
//		processDetectionFile(latestYoloFile, objectToActionMap, speedLimitMap, MIN_CONFIDENCE);
//
//		// עיבוד קובץ R-CNN האחרון
//		std::string latestRcnnFile = getLatestFile(rcnnDataPath);
//		processDetectionFile(latestRcnnFile, objectToActionMap, speedLimitMap, MIN_CONFIDENCE);
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(500));
//	}
//}

//void Camera::processDetectionFile(
//	const std::string& filePath,
//	const std::map<std::string, CameraFunction>& objectToActionMap,
//	const std::map<std::string, SpeedLimitFunction>& speedLimitMap,
//	float minConfidence
//) {
//	if (filePath.empty()) {
//		return;
//	}
//	cout << "Processing detection file: " << filePath << endl;
//	std::vector<std::string> lines = readFileLines(filePath);
//	for (const std::string& line : lines) {
//		// Create a stringstream from the current line for parsing
//		std::stringstream ss(line);
//		std::string objectName;
//		float confidence;
//		std::vector<float> boundingBox(4);
//
//		// Try to read the variables
//		if (ss >> objectName >> boundingBox[0] >> boundingBox[1] >> boundingBox[2] >> boundingBox[3] >> confidence) {
//			// Check if the confidence is above the minimum threshold
//			if (confidence >= minConfidence) {
//				cout << "Detected: " << objectName << " (Confidence: " << confidence << ")" << endl;
//				if (objectToActionMap.count(objectName)) {
//					(this->*objectToActionMap.at(objectName))();
//				}
//				// If the object name starts with "speed limit"
//				else if (objectName.rfind("speed limit", 0) == 0) {
//					std::stringstream ssSpeed(objectName);
//					std::string speedPart1, speedPart2, speedValueStr;
//					if (ssSpeed >> speedPart1 >> speedPart2 >> speedValueStr) {
//						std::string speedLimitKey = speedPart1 + " " + speedPart2 + " " + speedValueStr;
//						if (speedLimitMap.count(speedLimitKey)) {
//							try {
//								float speed = std::stof(speedValueStr);
//								(this->*speedLimitMap.at(speedLimitKey))();
//							}
//							catch (const std::invalid_argument& e) {
//								std::cerr << "Invalid speed value in line: " << line << std::endl;
//							}
//							catch (const std::out_of_range& e) {
//								std::cerr << "Speed value out of range in line: " << line << std::endl;
//							}
//						}
//					}
//				}
//			}
//		}
//		else {
//			std::cerr << "Invalid format in detection line: " << line << std::endl;
//		}
//	}
//}