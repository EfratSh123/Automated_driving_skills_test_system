#pragma once
#include "Car.h"

class Camera
{
public:
	Camera();
	// Function of Speed Limit
	void speedLimit20TraficSingh();
	void speedLimit30TraficSingh();
	void speedLimit(float speed);
	void monitorSpeedChangeProcess(
		string reason,
		std::function<bool(float)> isTargetReached,
		float maxDuration,
		float targetSpeed = 0.0f
	);
	void stopping(string reason);
	void slowdown(string reason);
	bool isCarDecelerating(float previousSpeed, float currentSpeed);
	void stopTraficSingh();	
	void greenLight();
	void redLight();
	void pedestrians();
	void crosswalk();

private:
	Car car; // Declare car as a Car object (member variable)
};
