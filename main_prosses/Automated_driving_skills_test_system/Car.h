#ifndef CAR_H
#define CAR_H
#include <windows.h>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <string>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
//car status can be: driving, parking, reverse, neutral
enum class CarStatus {D,P,R,N};
class Car {
public:
	// Constructor with default values
	Car() : status(CarStatus::D), meters(0), signal(false) {}
private:
	CarStatus status;
	int meters; // Counts the remaining meters until the next turn on the road
	mutex mtx_meters;
	// סופר את המטרים שהרכב עבר מהפנייה הקודמת בכביש
	int distance; // Count the meters the vehicle has traveled since the previous turn on the road
	mutex mtx_distance;
	float speed = 0.0 ; // Speed in km/h
	mutex mtx_speed;
	float  prevSpeed = 0.0;
	mutex mtx_prevSpeed;
	float maxSpeed = 80; // Maximum speed in km/h
	mutex mtx_maxSpeed;
	string direction = "straight";
	mutex mtx_direction;
	bool signal = false;
	float laneDeviation = 0; // Lane deviation in meters
	mutex mtx_laneDeviation;
	float carDistanceFront; // מרחק מהרכב לפני הכי קרוב אלי
	mutex mtx_carDistanceFront;
	Vector2f location;
	mutex mtx_location;
	Vector2f prevLocation;
	mutex mtx_prevLocation;

////////////////////////////
	float acceleration;
	mutex mtx_acceleration;
////////////////////////////

public:
	void setStatus(CarStatus s) { status = s; }
	CarStatus getStatus() { return status; }
	void setMeters(int m) { lock_guard<std::mutex> lock(mtx_meters); meters = m; }
	int getMeters() { lock_guard<std::mutex> lock(mtx_meters); return meters; }
	void setSpeed(float s) { lock_guard<std::mutex> lock(mtx_speed); prevSpeed = speed; speed = s; }
	float getSpeed() { lock_guard<std::mutex> lock(mtx_speed); return speed; }
	float getPrevSpeed() { lock_guard<std::mutex> lock(mtx_prevSpeed); return prevSpeed; }
	void setMaxSpeed(float speed) { lock_guard<std::mutex> lock(mtx_maxSpeed); maxSpeed = speed; }
	float getMaxSpeed() { lock_guard<std::mutex> lock(mtx_maxSpeed); return maxSpeed; }
	string getDirection() { lock_guard<std::mutex> lock(mtx_direction); return direction; }
	void setDirection(string d) { lock_guard<std::mutex> lock(mtx_direction); direction = d; }
	void setSignal(bool s) { signal = s; }
	bool getSignal() { return signal; }
	float getLaneDeviation() { lock_guard<std::mutex> lock(mtx_laneDeviation); return laneDeviation; }
	void setLaneDeviation(float deviation) { lock_guard<std::mutex> lock(mtx_laneDeviation); laneDeviation = deviation; }
	int getDistance() { lock_guard<std::mutex> lock(mtx_distance); return distance; }
	// שליחת מרחק רק אם המרחק קטן מהמרחק שנשאר
	void setDistance(int d) {
		lock_guard<std::mutex> lock(mtx_distance);
		if (d < meters)
			distance = d;
	}
	float getCarDistanceFront(){ lock_guard<std::mutex> lock(mtx_carDistanceFront); return carDistanceFront; }
	void setCarDistanceFront(float disFront) { lock_guard<std::mutex> lock(mtx_carDistanceFront); carDistanceFront = disFront; }
	Vector2f getLocation() { lock_guard<std::mutex> lock(mtx_location); return location; }
	void setLocation(Vector2f pos) {
		lock_guard<std::mutex> lock(mtx_location);
		prevLocation = location;
		location = pos;
	}
	Vector2f getPrevLocation() { lock_guard<std::mutex> lock(mtx_prevLocation); return prevLocation; }
	void setAcceleration(float acc) { lock_guard<std::mutex> lock(mtx_acceleration); acceleration = acc; }
	float getAcceleration() { lock_guard<std::mutex> lock(mtx_acceleration); return acceleration; }

};
#endif
