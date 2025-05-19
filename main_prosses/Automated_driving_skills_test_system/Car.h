#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <string>
using namespace std;

//car status can be: driving, parking, reverse, neutral
enum class CarStatus {
    D,
    P,
    R,
    N
};

    class Car {
    public:
        // Constructor with default values
        Car() : status(CarStatus::P), meters(0), signal(false) {}
    private:
        CarStatus status;
        mutex mtx_status;
        int meters; // Counts the remaining meters until the next turn on the road
        mutex mtx_meters;
        // סופר את המטרים שהרכב עבר מהפנייה הקודמת בכביש
		int distance; // Count the meters the vehicle has traveled since the previous turn on the road
        mutex mtx_distance;

		double speed; // Speed in km/h
        mutex mtx_speed;
        double  prevSpeed;
        mutex mtx_prevSpeed;
		double maxSpeed = 80 ; // Maximum speed in km/h
        mutex mtx_maxSpeed;
        string direction;
        mutex mtx_direction;
        bool signal; 
        mutex mtx_signal;
		float laneDeviation; // Lane deviation in meters
        mutex mtx_laneDeviation;

    //get and set
    public:
		void setStatus(CarStatus s) { lock_guard<std::mutex> lock(mtx_status); status = s; }
        CarStatus getStatus() { lock_guard<std::mutex> lock(mtx_status); return status; }
		void setMeters(int m) { lock_guard<std::mutex> lock(mtx_meters); meters = m; }
        int getMeters() { lock_guard<std::mutex> lock(mtx_meters); return meters; }
        void setSpeed(double s) { lock_guard<std::mutex> lock(mtx_speed); prevSpeed = speed; speed = s; }
        double getSpeed() { lock_guard<std::mutex> lock(mtx_speed); return speed; }
        void setMaxSpeed(double speed) { lock_guard<std::mutex> lock(mtx_maxSpeed); maxSpeed = speed; }
        double getMaxSpeed() { lock_guard<std::mutex> lock(mtx_maxSpeed); return maxSpeed; }
        string getDirection() { lock_guard<std::mutex> lock(mtx_direction); return direction; }
        void setDirection(string d) { lock_guard<std::mutex> lock(mtx_direction); direction = d; }
        void setSignal(bool s) { lock_guard<std::mutex> lock(mtx_signal); signal = s; }
		bool getSignal() { lock_guard<std::mutex> lock(mtx_signal); return signal; }
		float getLaneDeviation() { lock_guard<std::mutex> lock(mtx_laneDeviation); return laneDeviation; }
		void setLaneDeviation(float deviation) { lock_guard<std::mutex> lock(mtx_laneDeviation); laneDeviation = deviation; }
		float getPrevSpeed() { lock_guard<std::mutex> lock(mtx_prevSpeed); return prevSpeed; }
		int getDistance() { lock_guard<std::mutex> lock(mtx_distance); return distance; }

        // שליחת מרחק רק אם המרחק קטן מהמרחק שנשאר
		void setDistance(int d) {
            lock_guard<std::mutex> lock(mtx_distance);
            if(d < meters)
                distance = d;
        }
    };
    

#endif // CAR_H
