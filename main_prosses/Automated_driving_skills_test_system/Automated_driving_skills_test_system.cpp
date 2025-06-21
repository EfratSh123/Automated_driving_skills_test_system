#include <iostream>
#include "IMU.h"
#include "Lidar.h"
#include "GPS.h"
#include "Car.h"
#include "Camera.h"
#include "EKF.h"
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
mutex mtx_grade;
int flagPrint = 0;
bool onyolo = true;
bool onCnn = true;
bool onLidar = true;
bool onGPS = true;
bool onIMU = true;
bool onRoadLane = true;
bool onLane = true;
bool flagSpeed = 0;
Car car;
GPS gps;
Lidar lidar;
IMU imu;
Camera camera;
RoadLane roadLane;
DrivingRoute drivingRoute;
EKF ekf;
int main()
{
	// create driving routes
	drivingRoute.getDirections("32.75514497433894,35.09988438222732", "32.750610905702,35.091979224811844");
	vector<string> drivingRouteLines = drivingRoute.getInstructions();

	string rcnnDataPath = "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\Automated_driving_skills_test_system\\output_video\\output_text_FasterRCNN";
	string yoloDataPath = "C:\\Users\\User\\ProjectEfratSh\\main_prosses\\Automated_driving_skills_test_system\\output_video\\output_text_YOLO";
	camera.initializeMaps(car);

	// initialize kalman filter varables
	float initial_x = 0, initial_y = 0; // מיקום התחלתי של הרכב במטרים
	ekf.init(initial_x, initial_y); // init EKF with initial position
	imu.setKalman(&ekf); 
	gps.setKalman(&ekf);

	// play sensors and threads
	thread imu_play(&IMU::IMUplay, &imu, ref(car));
	thread gps_play(&GPS::GPSplay, &gps);
	thread Lidar_play(&Lidar::processLidarData, &lidar, ref(car)); // בדיקת מרחק מרכב לפני
	thread lidarCarDistanceFront(&Lidar::monitorSafeDistance, &lidar, ref(car));
	thread maxSpeed(&IMU::monitorMaxSpeed, &imu, ref(car)); // מהירות מקסימלית מותרת
	thread roadLaneRunner(&RoadLane::runLaneDetection, &roadLane); // נתיב נסיעה
	thread roadLaneProcessor(&RoadLane::processLaneOutputLoop, &roadLane, ref(car)); // נתיב נסיעה

	//thread DeviationDurationThread(&RoadLane::DeviationDuration, &roadLane, ref(car)); // משך סטיה ממרכז הנתיב

	thread cnnRunner(&Camera::runCNNModelLoop, &camera);
	thread cnnProcessor(&Camera::processCNNFilesLoop, &camera, ref(rcnnDataPath), ref(camera.getCnnObjectToActionMap()), 0.7f, ref(car));
	thread yoloRunner(&Camera::runYoloModelLoop, &camera);
	thread yoloProcessor(&Camera::processYoloFilesLoop, &camera, ref(yoloDataPath), ref(camera.getYoloObjectToActionMap()), 0.7f, ref(car));

	globalFunc globalPrint;

	//מעבר על הלולאה של ההוראות
	for (const string& line : drivingRouteLines)
	{
		istringstream iss(line.substr(4)); // צור זרם ממחרוזת החלק שאחרי "in: "
		string distanceStr, units, direction;

		cout << "check line: " << line << endl;
		
		if (line.length() <= 4) {
			cout << "too short line: " << line << endl;
			continue;
		}
		//istringstream iss(line.substr(4));
		//string distanceStr, units, direction;
		if (!(iss >> distanceStr >> units)) {
			cout << "invalid format in kine: " << line << endl;
			continue;
		}
		//iss >> distanceStr >> units >> ws; // קרא את המרחק, היחידות ודלג על רווחים לבנים
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
			std::ostringstream oss;
			oss << "direction: " << direction << ", meters: " << fixed << setprecision(2) << meters;
			globalPrint.print(oss.str());
		}
		catch (const invalid_argument& e) {
			globalPrint.printError("Invalid distance: " + line);
		}
		catch (const out_of_range& e) {
			globalPrint.printError("Too great a distance: " + line);
		}

		while (car.getDistance() < car.getMeters()); // כל עוד לא הגיע זמן הפניה הבאה

		this_thread::sleep_for(chrono::seconds(1));
	}
	onyolo = false;
	onCnn = false;
	onGPS = false;
	onLane = false;
	onLidar = false;
	onRoadLane = false;
	onIMU = false;
	flagSpeed = 1;
	imu_play.join();
	gps_play.join();
	Lidar_play.join();
	yoloRunner.join();
	yoloProcessor.join();
	cnnRunner.join();
	cnnProcessor.join();
	maxSpeed.join();
	roadLaneProcessor.join();
	roadLaneRunner.join();

	//DeviationDurationThread.join();
	lidarCarDistanceFront.join();

	if (grade >= 90)
		globalPrint.print("Congratulations! The student passed the Driving Test.");
	else
		globalPrint.print("The student did not pass the Driving Test this time. Please continue practicing.");
	return 0;
}