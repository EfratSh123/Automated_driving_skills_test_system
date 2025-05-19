#pragma once
#include "Car.h"
#include <functional> // Include this header for std::function
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
	// הגדרות ה-typedef צריכות להיות בתוך המחלקה
	typedef void (Camera::* CameraFunction)();
	typedef void (Camera::* SpeedLimitFunction)();

	Camera();
	// Function of Speed Limit
	void speedLimit20TraficSingh();
	void speedLimit30TraficSingh();
	void speedLimit(float speed);
	//void monitorSpeedChangeProcess(
	//	std::string reason,
	//	std::function<bool(float)> isTargetReached,
	//	float maxDuration,
	//	float targetSpeed = 0.0f
	//);
	void stopping(string reason);
	void slowdown(string reason);

	vector<string> readFileLines(const string& filePath);
	string getLatestFile(const string& folderPath);


	//bool isCarDecelerating(float previousSpeed, float currentSpeed);
	void stopSign();
	void greenLight();
	void redLight();
	//void yellowLight();
	void pedestrians();
	void crosswalk();

	void processDetections();

	void processDetectionFile(
		const string& filePath,
		const unordered_map<string, CameraFunction>& objectToActionMap,
		float minConfidence
	);
	void processYOLODetections(
		const string& yoloDataPath,
		const unordered_map<string, CameraFunction>& yoloObjectToActionMap,
		float minConfidence
	);
	void processCNNDetections(
		const string& rcnnDataPath,
		const unordered_map<string, CameraFunction>& cnnObjectToActionMap,
		float minConfidence
	);

private:
	Car car;
	IMU* imu;
	globalFunc globalPrint;
};