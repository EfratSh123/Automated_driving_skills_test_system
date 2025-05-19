#include <iostream>
#include "IMU.h"
#include "Lidar.h"
#include "GPS.h"
#include "Car.h"
#include "Camera.h"
#include <iostream>
#include "RoadSpeedInfo.h"
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

int main()
{
	// play sensors
	thread imu_play(&IMU::IMUplay, &imu);
	thread gps_play(&GPS::GPSplay, &gps); // ���� ������ ���� ����?
	thread Lidar_play(&Lidar::processLidarData, &lidar, ref(car)); // ����� ���� ���� ����
	thread yolo(&Camera::processYOLODetections, &camera); 
	thread CNN(&Camera::processCNNDetections, &camera);
	thread maxSpeed(&IMU::monitorMaxSpeed, &imu); // ������ �������� �����
	thread roadLaneTread(&RoadLane::runLaneDetection, &roadLane, ref(car)); // ���� �����
	globalFunc globalPrint;

	// ����� ����� ������ ������ ������ �������: ����� ������
	const string inputFilename = "driving_instructions.txt";
	ifstream inputFile(inputFilename);
	string line;
	if (!inputFile.is_open()) {
		globalPrint.print("can't open the file: " + inputFilename);
		return 1;
	}
	//���� �� ������ �� �������
	while (std::getline(inputFile, line))
	{
		istringstream iss(line.substr(4)); // ��� ��� ������� ���� ����� "in: "
		string distanceStr;
		string units;

		string direction;
		iss >> distanceStr >> units >> ws; // ��� �� �����, ������� ���� �� ������ �����
		double meters = 0.0;
		try {
			double distance = stod(distanceStr);
			meters = distance;
			if (units == "km") {
				meters = distance * 1000.0;
			}
			else {
				globalPrint.print("Unknown distance units in a row: " + line);
				continue; // ���� ����� ����
			}
			car.setMeters(meters);

			// ��� �� ��� ����� ������
			// ***************�����- �� ����� �� �� ����� ���: ����� �� ������ �����****************
			getline(iss >> ws, direction);
			car.setDirection(direction);
			//globalPrint.print("direction: " + direction + ", meters: " + fixed + setprecision(2) + meters);
			cout << "direction: " << direction << ", meters: " << fixed << setprecision(2) << meters << endl;
		}
		catch (const invalid_argument& e) {
			globalPrint.print("Invalid distance: " + line);
		}
		catch (const out_of_range& e) {
			globalPrint.print("Too great a distance: " + line);
		}

		while (car.getDistance() < car.getMeters());
		

		
	
		//sleep 1
	}
	inputFile.close();
	// close threads
	onyolo = false;
	onCnn = false;
	onGPS = false;
	onLidar = false;
	onRoadLane = false;
	onIMU = false;
	flagSpeed = 1;
	inputFile.close();
	imu_play.join();
	gps_play.join();
	Lidar_play.join();
	yolo.join();
	CNN.join();
	maxSpeed.join();
	roadLaneTread.join();


	//�����  ����� ������/������ �����
	 
	
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
	return 0;
}