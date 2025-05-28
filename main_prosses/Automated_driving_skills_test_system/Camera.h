#pragma once
#include <windows.h>
#include "Car.h"
#include <functional>
#include <string>
#include <map>
#include "IMU.h"
#include <thread>
#include <vector>
#include "globalFunc.h"
#include <mutex>
using namespace std;

class Camera
{
public:
	typedef void (Camera::* CameraFunction)();
	typedef void (Camera::* SpeedLimitFunction)();
	Camera();
	void speedLimit(float speed);
	vector<string> readFileLines(const string& filePath);
	string getLatestFile(const string& folderPath);
	void stopSign();
	void greenLight();
	void redLight();
	void pedestrians();
	void crosswalk();
	void processDetections();
	void processDetectionFile(
		const string& filePath,
		const unordered_map<string, function<void()>>& objectToActionMap,
		float minConfidence
	);
	void processYOLODetections(
		const string& yoloDataPath,
		const unordered_map<string, function<void()>>& yoloObjectToActionMap,
		float minConfidence
	);
	void processCNNDetections(
		const string& rcnnDataPath,
		const unordered_map<string, function<void()>>& cnnObjectToActionMap,
		float minConfidence
	);
private:
	Car car;
	IMU* imu;
	globalFunc globalPrint;
	unordered_map<string, function<void()>> yoloObjectToActionMap = {
	{"pedestrian", [&]() { Camera::pedestrians(); }},
	{"crosswalk",  [&]() { Camera::crosswalk(); }}
	};
	unordered_map<string, function<void()>> cnnObjectToActionMap = {
	{"red_light_left",      [&]() { if (car.getDirection() == "left")       Camera::redLight(); }},
	{"red_light_right",     [&]() { if (car.getDirection() == "right")      Camera::redLight(); }},
	{"red_light",           [&]() { Camera::redLight(); }},
	{"red_light_straight",  [&]() { if (car.getDirection() == "straight")   Camera::redLight(); }},
	{"green_light",         [&]() { Camera::greenLight(); }},
	{"green_light_left",    [&]() { if (car.getDirection() == "left")       Camera::greenLight(); }},
	{"green_light_right",   [&]() { if (car.getDirection() == "right")      Camera::greenLight(); }},
	{"green_light_straight",[&]() { if (car.getDirection() == "straight") Camera::greenLight(); }},
	{"stop",				[&]() { Camera::stopSign(); }},
	{"stop_trafic_singh",   [&]() { Camera::stopSign(); }},
	{"speed_limit_20",      [&]() { Camera::speedLimit(20); }},
	{"speed_limit_30",      [&]() { Camera::speedLimit(30); }},
	{"speed_limit_60",     [&]() { Camera::speedLimit(60); } },
	{"speed_limit_80",      [&]() { Camera::speedLimit(80); }},
	{"speed_limit_100",     [&]() { Camera::speedLimit(100); }},
	{"speed_limit_120",     [&]() { Camera::speedLimit(120); }},
	// תמרור הולכי רגל, יעזור מתי שלא מזהה טוב את המעבר חציה
	//{"pedestrian",     [&]() { Camera::pedestrians(); }},
	};
};