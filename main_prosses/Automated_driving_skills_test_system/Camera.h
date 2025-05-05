#pragma once
#include "Car.h"

class Camera
{
public:
	Camera();
	// Function of Speed Limit
	void speedLimit20();
	void speedLimit30();
	void speedLimit(float speed);
	void checkSpeed();
	float calculateStoppingDistance(float currentSpeed);
	float calculateDeceleration(float targetSpeed);
	void applyEmergencyBraking(float deltaTime);
	void applyGradualDeceleration(float targetSpeed, float deltaTime);
	
	void stop();

	void greenLight();

private:
	Car car; // Declare car as a Car object (member variable)
};
