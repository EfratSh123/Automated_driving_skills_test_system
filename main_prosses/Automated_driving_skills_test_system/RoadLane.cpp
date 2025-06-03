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
extern int grade; 

// Function to calculate the path deviation in meters, based on a binary image of the path
float RoadLane::calculateLaneDeviation(const cv::Mat& binaryImg) {
    const float realLaneWidthMeters = 3.5f;  // רוחב נתיב קבוע
    int y = binaryImg.rows * 3 / 4; // אזור בו נחפש את הנתיב ברבע התחתון של התמונה
    int leftEdge = -1, rightEdge = -1; // right and Left lane edges
    int sumX = 0, count = 0;
    // Loop over the pixels in the selected row
    for (int x = 0; x < binaryImg.cols; ++x) {
		// check if this pixel is part of the lane - white pixel
        if (binaryImg.at<uchar>(y, x) > 128) {
            sumX += x;
            count++;
			// update left and right edges
            if (leftEdge == -1) leftEdge = x;
            rightEdge = x;
        }
    }
    // In case no lane or partial lane is detected - לא זוהה נתיב או זוהה נתיב חלקי
    if (leftEdge == rightEdge) {
		globalPrint.printError("Warning: Incomplete or missing lane detection. Deviation not computed.");
        return 0.0f;
    }
	int laneWidthPixels = rightEdge - leftEdge; // Width of the lane in pixels
    // Filter out unrealistically narrow lanes (likely noise) - סינון נתיבים לא מציאותיים
    if (laneWidthPixels < 30) {
		globalPrint.printError("Warning: Detected lane width too narrow (" + std::to_string(laneWidthPixels) + " pixels). Skipping computation.");
        return 0.0f;
    }
    int laneCenter = sumX / count; 
    int imageCenter = binaryImg.cols / 2;
	int deviationPixels = laneCenter - imageCenter;
    float metersPerPixel = realLaneWidthMeters / static_cast<float>(laneWidthPixels); // Calculating how many meters each pixel is worth
	return deviationPixels * metersPerPixel; // returning the deviation in meters
}

// Function that runs the path recognition process on a video
int RoadLane::runLaneDetection(Car& c) {
    VideoCapture cap("C:\\Users\\User\\ProjectEfratSh\\main_prosses\\111.mp4");
    if (!cap.isOpened()) {
		globalPrint.printError("Error opening video file.");
        return -1;
    }
    int frameCount = 0;
    Mat frame;
    while (cap.read(frame))
    {
        // Preparing the frame for input to the lane recognition model - שמירת הפריים כתמונה זמנית
        ostringstream filename;
        filename << "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\temp\\input.jpg";
        imwrite(filename.str(), frame);
		// Run the lane detection model using Python
        ostringstream command;
        command << "py -3.13 "
            << "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\test2.py"
            << "--img "
            << "\"" << filename.str() << "\" "
            << "--model "
            << "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\log\\best_model.pth";
        int result = system(command.str().c_str());
        if (result != 0)
        {
			globalPrint.printError("Lane detection failed on frame " + std::to_string(frameCount));
        }

        //this_thread::sleep_for(chrono::seconds(1));
         
		// Reading the binary output image
        string outputPath = "C:\\Users\\User\\ProjectEfratSh\\lanenet-lane-detection-pytorch-main\\binary_output.jpg";
        Mat resultImg = imread(outputPath, IMREAD_GRAYSCALE);
        if (!resultImg.empty()) {
			// Calculate the lane deviation
            float deviation = calculateLaneDeviation(resultImg);
            c.setLaneDeviation(deviation);
			globalPrint.print("Frame " + frameCount + std::to_string(deviation) + " m");
        }
        else {
			globalPrint.printError("Could not read binary output for frame " + frameCount);
        }
        frameCount++;
		// remove the temporary image
        //fs::remove(filename.str());

        ////אם סטיה לא תקינה לעשות WHILE כל עוד הסטיה לא תקינה ולעלות טיימר של מונה שניות וכשיוצאים מהלולאה להוריד בניקוד לפי חישוב
        //int timeDeviation = 0;
        //while (c.getLaneDeviation() > 0.4f) {
		//	globalPrint.print("Warning: Lane deviation too high: " + to_string(c.getLaneDeviation()) + " m");
		//	this_thread::sleep_for(chrono::seconds(1));
		//	timeDeviation++;
		//}
    }

    // free video file
    cap.release();
    return 0;
}

RoadLane::RoadLane()
{
}

void RoadLane::DeviationDuration(Car &c) {
    int timeDeviation = 0;
    while (c.getLaneDeviation() > 0.4f) {
        globalPrint.print("Warning: Lane deviation too high: " + to_string(c.getLaneDeviation()) + " m");
        this_thread::sleep_for(chrono::seconds(1));
        timeDeviation++;
    }
    // לא בוצע מעבר נתיב אלא סטייה מהנתיב
	if (timeDeviation > 6) {
		globalPrint.print("Lane deviation duration: " + to_string(timeDeviation) + " seconds.");
        grade -= timeDeviation * 0.3 + 5;
	}
    timeDeviation = 0;
}