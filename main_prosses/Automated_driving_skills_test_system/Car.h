#ifndef CAR_H
#define CAR_H
#include <windows.h>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <string>
using namespace std;
//car status can be: driving, parking, reverse, neutral
enum class CarStatus {D,P,R,N};
class Car {
public:
	// Constructor with default values
	Car() : status(CarStatus::P), meters(0), signal(false) {}
private:
	CarStatus status;
	int meters; // Counts the remaining meters until the next turn on the road
	mutex mtx_meters;
	// סופר את המטרים שהרכב עבר מהפנייה הקודמת בכביש
	int distance; // Count the meters the vehicle has traveled since the previous turn on the road
	mutex mtx_distance;
	float speed; // Speed in km/h
	mutex mtx_speed;
	float  prevSpeed;
	mutex mtx_prevSpeed;
	float maxSpeed = 80; // Maximum speed in km/h
	mutex mtx_maxSpeed;
	string direction;
	mutex mtx_direction;
	bool signal;
	float laneDeviation; // Lane deviation in meters
	float carDistanceFront; // מרחק מהרכב לפני הכי קרוב אלי
	mutex mtx_carDistanceFront;
	//get and set
public:
	void setStatus(CarStatus s) { status = s; }
	CarStatus getStatus() { return status; }
	void setMeters(int m) { lock_guard<std::mutex> lock(mtx_meters); meters = m; }
	int getMeters() { lock_guard<std::mutex> lock(mtx_meters); return meters; }
	void setSpeed(float s) { lock_guard<std::mutex> lock(mtx_speed); prevSpeed = speed; speed = s; }
	float getSpeed() { lock_guard<std::mutex> lock(mtx_speed); return speed; }
	void setMaxSpeed(float speed) { lock_guard<std::mutex> lock(mtx_maxSpeed); maxSpeed = speed; }
	float getMaxSpeed() { lock_guard<std::mutex> lock(mtx_maxSpeed); return maxSpeed; }
	string getDirection() { lock_guard<std::mutex> lock(mtx_direction); return direction; }
	void setDirection(string d) { lock_guard<std::mutex> lock(mtx_direction); direction = d; }
	void setSignal(bool s) { signal = s; }
	bool getSignal() { return signal; }
	float getLaneDeviation() { return laneDeviation; }
	void setLaneDeviation(float deviation) { laneDeviation = deviation; }
	float getPrevSpeed() { lock_guard<std::mutex> lock(mtx_prevSpeed); return prevSpeed; }
	int getDistance() { lock_guard<std::mutex> lock(mtx_distance); return distance; }
	// שליחת מרחק רק אם המרחק קטן מהמרחק שנשאר
	void setDistance(int d) {
		lock_guard<std::mutex> lock(mtx_distance);
		if (d < meters)
			distance = d;
	}
	float getCarDistanceFront(){ lock_guard<std::mutex> lock(mtx_carDistanceFront); return carDistanceFront; }
	void setCarDistanceFront(float disFront) { lock_guard<std::mutex> lock(mtx_carDistanceFront); carDistanceFront = disFront; }
};
#endif
