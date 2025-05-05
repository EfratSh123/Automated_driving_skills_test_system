#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include "Globals.h" //include the global var 'grade' from Globals.h
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
        int meters; // Counts the remaining meters until the next turn on the road (e.g., 100, 50, 0, -5)
		double speed; // Speed in km/h
		double maxSpeed = 80 ; // Maximum speed in km/h
        string direction;
        bool signal;                  
        //get and set
    public:
		void setStatus(CarStatus s) { status = s; }
        CarStatus getStatus() { return status; }
		void setMeters(int m) { meters = m; }
        int getMeters() { return meters; }
        void setSpeed(double s) { speed = s; }
        double getSpeed() { return speed; }
        void setMaxSpeed(double speed) { maxSpeed = speed; }
        double getMaxSpeed() { return maxSpeed; }
		void setDirection(string d) { direction = d; }
        void setSignal(bool s) { signal = s; }
        string getDirection() { return direction; }
		bool getSignal() { return signal; }




    };
    

#endif // CAR_H
