#pragma once
#include <windows.h>

#include <opencv2/opencv.hpp>
#include "Car.h"
#include "globalFunc.h"
#include <mutex>
using namespace std;

class RoadLane
{
public:
	RoadLane();
	float calculateLaneDeviation(const cv::Mat& binaryImg);
	void DeviationDuration(Car& c);
	int runLaneDetection(Car& c);
private:
	globalFunc globalPrint;

};

