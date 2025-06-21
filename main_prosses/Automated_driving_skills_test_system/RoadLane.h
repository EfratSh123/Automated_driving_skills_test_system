#pragma once
#include <windows.h>

#include <opencv2/opencv.hpp>
#include "Car.h"
#include "globalFunc.h"
#include <mutex>
using namespace std;
extern bool onLane;

class RoadLane
{
public:
	RoadLane();
	float calculateLaneDeviation(const cv::Mat& binaryImg);
	void DeviationDuration(Car& c);
	//int runLaneDetection(Car& c);
	int processLaneOutputLoop(Car& c);
	void runLaneDetection();
private:
	globalFunc globalPrint;
};

