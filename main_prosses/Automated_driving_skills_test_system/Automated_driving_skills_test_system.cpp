#include <iostream>
#include "IMU.h"
#include "Lidar.h"
#include "GPS.h"
#include "Car.h"
#include "Camera.h"
#include <iostream>
#include "DrivingRoute.h"
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <iomanip>
#include "globalFunc.h"
#include <functional>
#include "RoadLane.h"
using namespace std;
int grade = 100;
bool pass = true;
int flagPrint = 0;
bool onyolo = true;
bool onCnn = true;
bool onLidar = true;
bool onGPS = true;
bool onIMU = true;
bool onRoadLane = true;
bool flagSpeed = 0;
Car car; 
GPS gps;
Lidar lidar;
IMU imu;
Camera camera;
RoadLane roadLane;
DrivingRoute drivingRoute;
int main()
{
	// create driving routes
	drivingRoute.getDirections("32.75514497433894,35.09988438222732", "32.750610905702,35.091979224811844");
	vector<string> drivingRouteLines = drivingRoute.getInstructions();

	string rcnnDataPath = "C:\\Users\\User\\ProjectEfratSh\\fasterRcnn\\Faster_R-CNN_new_tarin\\orginized_dataset\\output_model\\NewFolder";
	string yoloDataPath = "C:\\Users\\User\\ProjectEfratSh\\YOLO\\Crosswalk_and_Pedestrians_dataset\\detectionsNewVideo";

	// play sensors and threads
	thread CNN(&Camera::processCNNDetections, &camera, rcnnDataPath, camera.getCnnObjectToActionMap(), 0.7);
	thread yolo(&Camera::processYOLODetections, &camera, yoloDataPath, camera.getYoloObjectToActionMap(), 0.7);
	thread imu_play(&IMU::IMUplay, &imu);
	thread gps_play(&GPS::GPSplay, &gps); 
	thread Lidar_play(&Lidar::processLidarData, &lidar, ref(car)); // בדיקת מרחק מרכב לפני
	thread lidarCarDistanceFront(&Lidar::monitorSafeDistance, &lidar, ref(car));
	thread maxSpeed(&IMU::monitorMaxSpeed, &imu, ref(car)); // מהירות מקסימלית מותרת
	thread roadLaneThread(&RoadLane::runLaneDetection, &roadLane, ref(car)); // נתיב נסיעה
	thread DeviationDurationThread(&RoadLane::DeviationDuration, &roadLane, ref(car)); // משך סטיה ממרכז הנתיב

	globalFunc globalPrint;

	//מעבר על הלולאה של ההוראות
	for (const string& line : drivingRouteLines)
	{
		istringstream iss(line.substr(4)); // צור זרם ממחרוזת החלק שאחרי "in: "
		string distanceStr;
		string units;
		string direction;
		iss >> distanceStr >> units >> ws; // קרא את המרחק, היחידות ודלג על רווחים לבנים
		float meters = 0.0;
		try {
			double distance = stod(distanceStr);
			meters = distance;
			if (units == "km") {
				meters = distance * 1000.0;
			}
			//else {
			//	globalPrint.printError("Unknown distance units in a row: " + line);
			//	continue; // pass the next line
			//}
			car.setMeters(meters);
			getline(iss >> ws, direction); // קרא את שאר השורה ככיוון
			car.setDirection(direction);
			//globalPrint.print("direction: " + direction + ", meters: " + fixed + setprecision(2) + meters);
			cout << "direction: " << direction << ", meters: " << fixed << setprecision(2) << meters << endl;
		}
		catch (const invalid_argument& e) {
			globalPrint.printError("Invalid distance: " + line);
		}
		catch (const out_of_range& e) {
			globalPrint.printError("Too great a distance: " + line);
		}

		while (car.getDistance() < car.getMeters()); // כל עוד לא הגיע זמן הפניה הבאה


// ************************ ??צריך את זה ********************
		//double distance = 200.0;
		//while (distance > 0) 
		//{
		//	distance -= imu.getAcceleration();
		//	if (distance < 0) {
		//		distance = 0;
		//	}
		//	//get
		//	//car.meters = distance;
		//	//car.speed = imu.getSpeed();
		//}

		this_thread::sleep_for(chrono::seconds(1));
	}
	onyolo = false;
	onCnn = false;
	onGPS = false;
	onLidar = false;
	onRoadLane = false;
	onIMU = false;
	flagSpeed = 1;
	imu_play.join();
	gps_play.join();
	Lidar_play.join();
	yolo.join();
	CNN.join();
	maxSpeed.join();
	roadLaneThread.join();
	DeviationDurationThread.join();
	lidarCarDistanceFront.join();
	//חישוב  הציון והצגתו / הדפסתו לטסטר
	if (grade >= 90)
		globalPrint.print("Congratulations! The student passed the Driving Test.");
	else
		globalPrint.print("The student did not pass the Driving Test this time. Please continue practicing.");
	return 0;
}