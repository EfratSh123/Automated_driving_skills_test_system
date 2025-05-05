#include "Perception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Perception::Perception() {
    // אתחול אם יש צורך
}

// פונקציה זו קוראת את קובץ ה-YOLO ומעבדת את הפלט
void Perception::processYoloOutput(const std::string& yoloFilePath) {
    std::ifstream yoloFile(yoloFilePath);
    std::string line;

    if (yoloFile.is_open()) {
        while (getline(yoloFile, line)) {
            std::stringstream ss(line);
            std::string label;
			//מידע על התיבות התחומות- x,y,רוחב,גובה,אמינות
            float x, y, width, height, confidence;

            ss >> label >> x >> y >> width >> height >> confidence;

            if (label == "person" and confidence>=0.5) {
                std::cout << "YOLO: זיהה הולך רגל במיקום: " << x << ", " << y << std::endl;
                handlePedestrian(x, y, width, height);
            }
            else if (label == "crosswalk" and confidence >= 0.5) {
                std::cout << "YOLO: זיהה מעבר חציה במיקום: " << x << ", " << y << std::endl;
                handleCrosswalk(x, y, width, height);
            }
        }
        yoloFile.close();
    }
    else {
        std::cerr << "לא הצליח לפתוח את קובץ YOLO: " << yoloFilePath << std::endl;
    }
}

void Perception::handlePedestrian(float pedestrianX, float pedestrianY, float pedestrianBoundingBoxWidth, float pedestrianBoundingBoxHeight) {
    // כאן תהיה הלוגיקה לבדיקה האם הולך הרגל על שפת המדרכה/בכביש
    // וחישוב מרחק עצירה
    std::cout << "טיפול בהולך רגל במיקום: " << pedestrianX << ", " << pedestrianY << std::endl;
    // לדוגמה, קריאה לפונקציה לחישוב מרחק עצירה
    float stoppingDistance = calculateStoppingDistance(50.0f); // 50.0f היא דוגמה למהירות נוכחית
    std::cout << "מרחק עצירה נדרש: " << stoppingDistance << " מטרים." << std::endl;
    // תצטרך לקבוע איך המיקום של התיבה התוחמת מתייחס למיקום על הכביש/מדרכה
}

void Perception::handleCrosswalk(float crosswalkX, float crosswalkY, float crosswalkBoundingBoxWidth, float crosswalkBoundingBoxHeight) {
    // 
    // כאן תהיה הלוגיקה להאטה לפני מעבר חציה ובדיקה אם יש הולכי רגל באזור
    std::cout << "טיפול במעבר חציה במיקום: " << crosswalkX << ", " << crosswalkY << std::endl;
    // תצטרך לתקשר עם מודול אחר (BehavioralPlanner) כדי להתחיל להאט
    // ולקבל מידע על נוכחות הולכי רגל (אולי מתוצאות YOLO נוספות או חיישנים אחרים)
}

//חישוב מרחק עצירה
float Perception::calculateStoppingDistance(float currentSpeed) {
    // מרחק תגובה: dr = v * tr
    float reactionTime = 1.1f;  // זמן תגובה ממוצע של נהג
    float reactionDistance = currentSpeed * reactionTime;
    // מרחק בלימה: db = (v^2) / (2 * a)
    float deceleration = 5.0f; // האטה ממוצעת (מטר לשנייה בריבוע) - ניתן לשנות בהתאם לתנאי הכביש והרכב
    float brakingDistance = (currentSpeed * currentSpeed) / (2 * deceleration);
    // מרחק עצירה כולל: d = dr + db
    float stoppingDistance = reactionDistance + brakingDistance;

    return stoppingDistance;
}