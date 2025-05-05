#pragma once
//Identifying situations based on sensors and cameras
#ifndef PERCEPTION_H
#define PERCEPTION_H
#include <string>
#include <vector>

class Perception
{
public:
    Perception();
    void processYoloOutput(const std::string& yoloFilePath);

private:
    //Pedestrian
    void handlePedestrian(float pedestrianX, float pedestrianY, float pedestrianBoundingBoxWidth, float pedestrianBoundingBoxHeight);
	//crosswalk
    void handleCrosswalk(float crosswalkX, float crosswalkY, float crosswalkBoundingBoxWidth, float crosswalkBoundingBoxHeight);
    //Stopping distance
    float calculateStoppingDistance(float currentSpeed);
};
#endif