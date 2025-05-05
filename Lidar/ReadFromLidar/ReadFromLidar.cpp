#include <iostream> //For standard input/output
#include <fstream> //For reading from files
#include <string> //For std::string
#include <sstream> //For string stream parsing
#include <limits> //For float max

int main() {
    //Path to the LIDAR data file
    std::ifstream inputFile("C:/Users/User/ProjectEfratSh/Lidar/lidar_drive_simulation_with_angles.txt");

    //Check if the file opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "can't open the file." << std::endl;
        return 1;
    }

    std::string line;
    int scanIndex = 0;

    //Read file lines
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);//Turn line into word stream
        std::string token;

        float minDistance = std::numeric_limits<float>::max(); //Initialize min distance
        float minAngle = 0.0f; //Angle where min distance occurred

        //Split line into points
        while (ss >> token) {
            size_t colonPos = token.find(':'); //Find position of ':'

            if (colonPos == std::string::npos) continue; //Skip malformed

            std::string angleStr = token.substr(0, colonPos);     //Get angle part
            std::string distanceStr = token.substr(colonPos + 1); //Get distance part

            try {
                float angle = std::stof(angleStr);       //Convert angle to float
                float distance = std::stof(distanceStr); //Convert distance to float

                //Filter angles to ±30°
                if (angle >= -30.0f && angle <= 30.0f) {
                    if (distance < minDistance) {
                        minDistance = distance; //Update minimum distance
                        minAngle = angle;       //Save angle
                    }
                }

            }
            catch (...) {
                //Handle invalid number format
                continue;
            }
        }

        //Print only if a vehicle is detected
        if (minDistance < 25.0f) {
            std::cout << "Scan " << scanIndex
                << ": Detected vehicle at angle " << minAngle
                << "°, distance " << minDistance << "m" << std::endl;
        }

        scanIndex++; //Next scan
    }

    inputFile.close(); //Close file when done
    return 0;
}
