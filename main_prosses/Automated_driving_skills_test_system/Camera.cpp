#include "Camera.h"
#include <iostream>
#include "Car.h" 
#include <thread>
#include <iostream>
#include <cmath>
using namespace std;

Camera::Camera() : car()
{
	//constructor
}


//----------------Functions of Speed Limit----------------
void Camera::speedLimit20() {
	speedLimit(20);
}
void Camera::speedLimit30() {
	speedLimit(30);
}
void Camera::speedLimit(float speed) {
	cout << "Speed limit detected: " << speed << " km/h" << endl;
	// Set the car's speed limit to 20 km/h
	car.setMaxSpeed(speed);
	// Check if the car's current speed is above the new speed limit
	if (car.getSpeed() > car.getMaxSpeed()) {
		// Calculate the time it will take to decelerate.
		float timeToDecelerate = (car.getSpeed() - car.getMaxSpeed()) / 3.0f;  // deceleration is 3 m/s^2
		// Apply gradual deceleration over the calculated time
		float deltaTime = 0.5f; //update every 0.5 seconds
		for (float time = 0; time < timeToDecelerate; time += deltaTime) {
			applyGradualDeceleration(car.getMaxSpeed(), deltaTime);
		}
		//***************** ��� ����� �� ���? *****************
	}
}
// Function to calculate stopping distance
float Camera::calculateStoppingDistance(float currentSpeed) {
	float reactionTime = 1.5f;
	float decelerationRate = 5.0f;
	float reactionDistance = currentSpeed * (reactionTime / 3.6f);
	float brakingDistance = (currentSpeed * currentSpeed) / (2.0f * decelerationRate * 3.6f * 3.6f);
	return reactionDistance + brakingDistance;
}
// Function to calculate deceleration (����)
// ����� ��� �����
float Camera::calculateDeceleration(float targetSpeed) {
	if (car.getSpeed() > targetSpeed) {
		float stoppingDistance = calculateStoppingDistance(car.getSpeed());
		//convert from KM to m/s
		float timeToStop = stoppingDistance / (car.getSpeed() / 3.6f);
		// the function return the deceleration in m/s^2
		// ��� ������ ��������� ������ �����, �� ����, ���� ����� ������ m/s�
		return (car.getSpeed() / 3.6f) / timeToStop;
	}
	return 0.0f;
}
// ������� ������ (Emergency Braking)
void Camera::applyEmergencyBraking(float deltaTime) {
	if (car.getSpeed() > 0.0f) {
		// ���� ����, ������ ������ �����.  ���� 7.0f ����� ���� �� 7 ��� ������ ������,
		// ��� ���� ������ ����� ������� �����.
		float emergencyDeceleration = 7.0f;
		car.setSpeed(car.getSpeed() - emergencyDeceleration * deltaTime * 3.6f);
		if (car.getSpeed() < 0.0f) {
			car.setSpeed(0.0f); // ��� �������� �� ����� �������
		}
		std::cout << "braking! Speed: " << car.getSpeed() << " km/h" << std::endl;
	}
}
// ������� ����� ������� (Gradual Deceleration)
void Camera::applyGradualDeceleration(float targetSpeed, float deltaTime) {
	if (car.getSpeed() > targetSpeed) {
		// ���� �����, ������ ����� �����. ���� 3.0f ����� ���� �� 3 ��� ������ ������,
		// ��� ���� ����� �����.
		float deceleration = 3.0f;
		car.setSpeed(car.getSpeed() - deceleration * deltaTime * 3.6f);
		if (car.getSpeed() < targetSpeed) {
			car.setSpeed(targetSpeed);
		}
		std::cout << "Gradual deceleration. Speed: " << car.getSpeed() << " km/h" << std::endl;
	}
}


//----------------stop----------------
void Camera::stop() {
	car.setMaxSpeed(0);
	// Check if the car's current speed is above the new speed limit
	if (car.getSpeed() > car.getMaxSpeed()) {
		// Calculate the time it will take to decelerate.
		float timeToDecelerate = (car.getSpeed() - car.getMaxSpeed()) / 7.0f;  // deceleration is 3 m/s^2
		// Apply gradual deceleration over the calculated time
		float deltaTime = 0.5f; //update every 0.5 seconds
		for (float time = 0; time < timeToDecelerate; time += deltaTime) {
			applyEmergencyBraking(deltaTime);
		}
		//***************** ��� ����� �� ���-��"�? *****************
	}
	cout << "The vehicle stopped at a stop sign." << endl;
}


//----------------green trafic light----------------
void Camera::greenLight() {
	cout << "Traffic light is green. Proceeding." << endl;
}

