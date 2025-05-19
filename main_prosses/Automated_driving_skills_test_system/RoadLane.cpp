#include "RoadLane.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include "Car.h"


namespace fs = std::filesystem;
using namespace cv;
using namespace std;

float RoadLane::calculateLaneDeviation(const cv::Mat& binaryImg) {
    const float realLaneWidthMeters = 3.5f;  // Actual lane width in meters
    int y = binaryImg.rows * 3 / 4;

    int leftEdge = -1, rightEdge = -1;
    int sumX = 0, count = 0;

    for (int x = 0; x < binaryImg.cols; ++x) {
        if (binaryImg.at<uchar>(y, x) > 128) {
            sumX += x;
            count++;
            if (leftEdge == -1) leftEdge = x;
            rightEdge = x;
        }
    }

    // Handle case of no or partial detection
    if (leftEdge == rightEdge) {
		globalPrint.printError("Warning: Incomplete or missing lane detection. Deviation not computed.");
        return 0.0f;
    }

    int laneWidthPixels = rightEdge - leftEdge;

    // Filter out unrealistically narrow lanes (likely noise)
    if (laneWidthPixels < 30) {
		globalPrint.printError("Warning: Detected lane width too narrow (" + std::to_string(laneWidthPixels) + " pixels). Skipping computation.");
        return 0.0f;
    }

    int laneCenter = sumX / count;
    int imageCenter = binaryImg.cols / 2;
    int deviationPixels = laneCenter - imageCenter;

    float metersPerPixel = realLaneWidthMeters / static_cast<float>(laneWidthPixels);
    return deviationPixels * metersPerPixel;
}



int RoadLane::runLaneDetection(Car& c) {
    VideoCapture cap("C:\\Users\\User\\ProjectEfratSh\\main_prosses\\111.mp4");
    if (!cap.isOpened()) {
		globalPrint.printError("Error opening video file.");
        return -1;
    }

    int frameCount = 0;
    Mat frame;

    while (cap.read(frame)) {
        ostringstream filename;
        filename << "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\temp\\input.jpg";

        imwrite(filename.str(), frame);

        ostringstream command;

        command << "py -3.13 "
            << "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\test2.py"
            << "--img "
            << "\"" << filename.str() << "\" "
            << "--model "
            << "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\log\\best_model.pth";

        int result = system(command.str().c_str());

        if (result != 0) {
			globalPrint.printError("Lane detection failed on frame " + std::to_string(frameCount));
        }

        //this_thread::sleep_for(chrono::milliseconds(3000));
        string outputPath = "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\binary_output.jpg";
        Mat resultImg = imread(outputPath, IMREAD_GRAYSCALE);
        if (!resultImg.empty()) {
            float deviation = calculateLaneDeviation(resultImg);
            c.setLaneDeviation(deviation);
			globalPrint.print("Frame " + frameCount + std::to_string(deviation) + " m");
        }
        else {
			globalPrint.printError("Could not read binary output for frame " + frameCount);
        }
        frameCount++;
        //fs::remove(filename.str());
    }
    cap.release();
    return 0;
}

RoadLane::RoadLane()
{
}

