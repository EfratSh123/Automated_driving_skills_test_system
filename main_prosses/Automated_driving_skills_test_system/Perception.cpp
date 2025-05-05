#include "Perception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Perception::Perception() {
    // ����� �� �� ����
}

// ������� �� ����� �� ���� �-YOLO ������ �� ����
void Perception::processYoloOutput(const std::string& yoloFilePath) {
    std::ifstream yoloFile(yoloFilePath);
    std::string line;

    if (yoloFile.is_open()) {
        while (getline(yoloFile, line)) {
            std::stringstream ss(line);
            std::string label;
			//���� �� ������ �������- x,y,����,����,������
            float x, y, width, height, confidence;

            ss >> label >> x >> y >> width >> height >> confidence;

            if (label == "person" and confidence>=0.5) {
                std::cout << "YOLO: ���� ���� ��� ������: " << x << ", " << y << std::endl;
                handlePedestrian(x, y, width, height);
            }
            else if (label == "crosswalk" and confidence >= 0.5) {
                std::cout << "YOLO: ���� ���� ���� ������: " << x << ", " << y << std::endl;
                handleCrosswalk(x, y, width, height);
            }
        }
        yoloFile.close();
    }
    else {
        std::cerr << "�� ����� ����� �� ���� YOLO: " << yoloFilePath << std::endl;
    }
}

void Perception::handlePedestrian(float pedestrianX, float pedestrianY, float pedestrianBoundingBoxWidth, float pedestrianBoundingBoxHeight) {
    // ��� ���� ������� ������ ��� ���� ���� �� ��� ������/�����
    // ������ ���� �����
    std::cout << "����� ����� ��� ������: " << pedestrianX << ", " << pedestrianY << std::endl;
    // ������, ����� �������� ������ ���� �����
    float stoppingDistance = calculateStoppingDistance(50.0f); // 50.0f ��� ����� ������� ������
    std::cout << "���� ����� ����: " << stoppingDistance << " �����." << std::endl;
    // ����� ����� ��� ������ �� ����� ������ ������ ������ �� �����/�����
}

void Perception::handleCrosswalk(float crosswalkX, float crosswalkY, float crosswalkBoundingBoxWidth, float crosswalkBoundingBoxHeight) {
    // 
    // ��� ���� ������� ����� ���� ���� ���� ������ �� �� ����� ��� �����
    std::cout << "����� ����� ���� ������: " << crosswalkX << ", " << crosswalkY << std::endl;
    // ����� ����� �� ����� ��� (BehavioralPlanner) ��� ������ ����
    // ����� ���� �� ������ ����� ��� (���� ������� YOLO ������ �� ������� �����)
}

//����� ���� �����
float Perception::calculateStoppingDistance(float currentSpeed) {
    // ���� �����: dr = v * tr
    float reactionTime = 1.1f;  // ��� ����� ����� �� ���
    float reactionDistance = currentSpeed * reactionTime;
    // ���� �����: db = (v^2) / (2 * a)
    float deceleration = 5.0f; // ���� ������ (��� ������ ������) - ���� ����� ����� ����� ����� �����
    float brakingDistance = (currentSpeed * currentSpeed) / (2 * deceleration);
    // ���� ����� ����: d = dr + db
    float stoppingDistance = reactionDistance + brakingDistance;

    return stoppingDistance;
}