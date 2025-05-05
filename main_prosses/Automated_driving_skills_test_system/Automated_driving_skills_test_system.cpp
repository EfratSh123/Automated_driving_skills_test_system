// Automated_driving_skills_test_system.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "IMU.h"
#include "Lidar.h"
#include "GPS.h"
#include "Car.h"

int main()
{
   //לפתוח קובץ הוראות ולקבל אותם לתוך מבנה
	//לולאה שכל פעם מחלצת משפט ושמה את המטרים בתוך משתנה ואת הכיוון
	GPS gps;
	Car car;
	Lidar Lidar;
	IMU imu;
	gps.GPSplay();
	Lidar.Lidarplay();
	imu.IMUplay();
	double distance = 200.0;
	while (distance > 0) 
	{
		distance -= imu.getAcceleration();
		if (distance < 0) {
			distance = 0;
		}
		//get
		car.meters = distance;
		car.speed = imu.getSpeed();


	}	

	
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
