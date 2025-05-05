#include "IMU.h"

IMU::IMU()
{
}

void IMU::IMUplay()
{
    // Create an input file stream
    std::ifstream inputFile("IMU.txt");
    if (!inputFile.is_open()) {
        std::cerr << "didn't success to open the file: IMU.txt" << std::endl;
		return;
    }
    string line;
	//read the file line by line
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        double acceleration, currentSpeed, gyroX, gyroY, gyroZ;

        if (ss >> acceleration >> currentSpeed >> gyroX >> gyroY >> gyroZ) {
            setAcceleration(acceleration);
            setSpeed(currentSpeed);
            setGyroX(gyroX);
            setGyroY(gyroY);
            setGyroZ(gyroZ);

            std::cout << "Acceleration: " << getAcceleration() << ", speed: " << getSpeed()
                << ", GyroX: " << getGyroX() << ", GyroY: " << getGyroY()
                << ", GyroZ: " << getGyroZ() << std::endl;

            // Pause the execution of the current thread for one second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::cerr << "invalid line at the file: " << line << std::endl;
        }
    }
    inputFile.close();
}
