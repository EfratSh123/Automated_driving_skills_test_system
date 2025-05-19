//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <cmath>
//#include <thread>
//#include <chrono>
//#include "DrivingScenarios.h"
//#include "IMUSensor.h"
//
//using namespace std;
//
//void DrivingScenarios::SpeedCar(int maxSpeed)
//{
//    print("SpeedCar function now play");
//
//    if (GetcurrentSpeed() < maxSpeed)
//    {
//
//        string line;
//        ifstream inputFile(GetPathOfSpeed()); // Open input file for reading
//        ofstream outputFile("temp.txt", ios::trunc); // Create temporary output file
//        double speedX, speedY, deltaX, deltaY, speed;
//
//        if (!inputFile.is_open())
//        {
//            cerr << "Error opening file IMU" << endl;
//
//        }
//
//        if (getline(inputFile, line))
//        {
//            istringstream iss(line);
//            iss >> speedX >> speedY;
//            speed = sqrt((speedX * speedX) + (speedY * speedY));
//            deltaX = GetaccelerationSpeed() * (speedX / speed);
//            deltaY = GetaccelerationSpeed() * (speedY / speed);
//            speedX = speedX + deltaX;
//            speedY = speedY + deltaY;
//            outputFile << speedX << " " << speedY << endl;
//        }
//
//        outputFile << inputFile.rdbuf();
//        inputFile.close();
//        outputFile.close();
//
//        if (std::remove(GetPathOfSpeed().c_str()) != 0)
//        {
//            std::cerr << "Error remove: Unable to remove file." << std::endl;
//        }
//
//        if (std::rename("temp.txt", GetPathOfSpeed().c_str()) != 0)
//        {
//            std::cerr << "Error Rename: Unable to rename file." << std::endl;
//        }
//    }
//}
//
//void DrivingScenarios::SlowdownCar(int MinSpeed)
//{
//    print("SlowdownCar function now play");
//
//    if (GetcurrentSpeed() > MinSpeed)
//    {
//        string line;
//        ifstream inputFile(GetPathOfSpeed());
//        ofstream outputFile("temp.txt", ios::trunc);
//        double speedX, speedY, deltaX, deltaY, speed;
//        if (!inputFile.is_open())
//        {
//            cerr << "Error opening file IMU" << endl;
//            return;
//        }
//
//        if (getline(inputFile, line))
//        {
//            istringstream iss(line);
//            iss >> speedX >> speedY;
//            speed = sqrt((speedX * speedX) + (speedY * speedY));
//            deltaX = GetaccelerationSpeed() * (speedX / speed);
//            deltaY = GetaccelerationSpeed() * (speedY / speed);
//            speedX = speedX - deltaX;
//            speedY = speedY - deltaY;
//            outputFile << speedX << " " << speedY << endl; // Write the modified number to the temporary file 
//        }
//
//        outputFile << inputFile.rdbuf();
//        inputFile.close();
//        outputFile.close();
//
//        if (std::remove(GetPathOfSpeed().c_str()) != 0)
//        {
//            std::cerr << "Error remove: Unable to remove file." << std::endl;
//        }
//
//        if (std::rename("temp.txt", GetPathOfSpeed().c_str()) != 0)
//        {
//            std::cerr << "Error Rename: Unable to rename file." << std::endl;
//        }
//    }
//}
//
//void IMUSensor::calculateSpeed(DrivingScenarios& carpoint)
//{
//    double prevSpeed = 0.0, acceleration; // Initialize previous speed variable
//    string line;
//    string filepath = "C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensor.txt";
//
//    while (isRunning)
//    {
//
//        ifstream inputFile(filepath); // Open the text file for reading
//        if (!inputFile.is_open())
//        {
//            cerr << "Error opening file IMU in IMU" << endl;
//        }
//
//        if (getline(inputFile, line)) // Read a line from the file
//        {
//            istringstream iss(line);
//            iss >> speedX >> speedY; // Extract speed X, speed Y, and time from the line
//            speed = sqrt((speedX * speedX) + (speedY * speedY)); // Calculate the total speed
//            acceleration = (speed - prevSpeed) / dt; // Calculate acceleration
//            print("acceleration: " + to_string(acceleration));
//            print("speed: " + to_string(speed));
//            carpoint.SetaccelerationSpeed(acceleration);
//            carpoint.SetcurrentSpeed(speed);
//            distance = (speed * 1000 / 3600 * GettimeSensor()); // 1000=1 k"m, 3600 secoend=1 hour
//            carpoint.Setdistance(distance);
//            print("Distance covered in current iteration: " + to_string(carpoint.Getdistance()) + " meters");
//            prevSpeed = speed; // Update previous speed to current speed
//            inputFile.close(); // Close the file
//
//            // Reopen the file in write mode to remove the first line
//            ofstream tempFile("C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensorTemp.txt");
//            ifstream inputFile2("C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensor.txt"); // Reopen the original file for reading
//
//            // Skip the first line
//            if (inputFile2.good() && tempFile.good()) 
//            {
//                getline(inputFile2, line); // Skip the first line
//
//
//                // Copy the remaining lines to the temporary file
//                while (getline(inputFile2, line))
//                {
//                    tempFile << line << endl;
//                }
//
//                tempFile.close();
//                inputFile2.close();
//
//                // Delete the original file
//                remove("C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensor.txt");
//
//                // Rename the temporary file to the original filename
//                rename("C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensorTemp.txt", "C:/Users/USER/Documents/פרוייקט/AlgoritemOfAoutoCar/src/IMUsensor.txt");
//            }
//        }
//         this_thread::sleep_for(chrono::seconds(1)); // Wait for 1 second between iterations
//    }
//}