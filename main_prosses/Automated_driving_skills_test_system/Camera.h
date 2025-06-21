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
	Camera();
	// פונקציה חדשה לאתחול המאפים עם אובייקט Car
	void initializeMaps(Car& carRef);

	void speedLimit(float speed, Car& car);
	vector<string> readFileLines(const string& filePath);
	void stopSign( Car& car);
	void greenLight( Car& car);
	void redLight(Car& car);
	void pedestrians(Car& car);
	void crosswalk(Car& car);
	void processDetectionFile(
		string& filePath,
		unordered_map<string, function<void(Car&)>>& objectToActionMap,
		float minConfidence,
		Car& car
	);

	unordered_map<string, function<void(Car&)>>& getYoloObjectToActionMap() { lock_guard<std::mutex> lock(mtx_yoloObjectToActionMap); return yoloObjectToActionMap; };
	unordered_map<string, function<void(Car&)>>& getCnnObjectToActionMap() { lock_guard<std::mutex> lock(mtx_cnnObjectToActionMap); return cnnObjectToActionMap; };

	void runCNNModelLoop();
	void runYoloModelLoop();
	void processCNNFilesLoop(
		string& rcnnDataPath,
		unordered_map<string, function<void(Car&)>>& cnnObjectToActionMap,
		float minConfidence,
		Car& car
	);

	void processYoloFilesLoop(
		string& yoloDataPath,
		unordered_map<string, function<void(Car&)>>& yoloObjectToActionMap,
		float minConfidence,
		Car& car
	);

private:
	IMU* imu;
	globalFunc globalPrint;
	unordered_map<string, function<void(Car&)>> yoloObjectToActionMap;
	mutex mtx_yoloObjectToActionMap;

	unordered_map<string, function<void(Car&)>> cnnObjectToActionMap;
	mutex mtx_cnnObjectToActionMap;
};