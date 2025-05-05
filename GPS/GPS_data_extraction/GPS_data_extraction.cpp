#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

// ������� ����� �� ���� �� �� ���� ������ NMEA �������
double nmeaToDecimal(std::string nmeaCoord, char direction, bool isLongitude) {
    if (nmeaCoord.empty()) {
        return 0.0;
    }
    int degreesLength = isLongitude ? 3 : 2;
    double degrees = std::stod(nmeaCoord.substr(0, degreesLength));
    double minutes = std::stod(nmeaCoord.substr(degreesLength)) / 60.0;
    double decimal = degrees + minutes;
    if (direction == 'S' || direction == 'W') {
        decimal = -decimal;
    }
    return decimal;
}

int main() {
    std::ifstream inputFile("GPS_data.txt"); // �� ���� ����
    std::ofstream outputFile("gps_coordinates.txt"); // �� ���� ����

    if (!inputFile.is_open()) {
        std::cerr << "�� ������ ����� �� ���� ����!" << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "�� ������ ����� �� ���� ����!" << std::endl;
        inputFile.close();
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find("$GPRMC") == 0) {
            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> segments;
            while (std::getline(ss, segment, ',')) {
                segments.push_back(segment);
            }

            if (segments.size() >= 7 && segments[2] == "A") { // ������� ��� ����� ���� �������� ��� "A" (���)
                std::string latitudeNMEA = segments[3];
                char latitudeDirection = segments[4][0];
                std::string longitudeNMEA = segments[5];
                char longitudeDirection = segments[6][0];

                double latitudeDecimal = nmeaToDecimal(latitudeNMEA, latitudeDirection, false); // false �� �� �� ����
                double longitudeDecimal = nmeaToDecimal(longitudeNMEA, longitudeDirection, true);  // true �� �� �� ����

                outputFile << std::fixed << std::setprecision(6) << latitudeDecimal << "," << longitudeDecimal << std::endl;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "������������ ����� ������ ����� gps_coordinates.txt" << std::endl;

    return 0;
}