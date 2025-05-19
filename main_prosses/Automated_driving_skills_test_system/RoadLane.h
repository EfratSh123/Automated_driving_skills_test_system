#pragma once
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
	int runLaneDetection(Car& c);
private:
	globalFunc globalPrint;

};

