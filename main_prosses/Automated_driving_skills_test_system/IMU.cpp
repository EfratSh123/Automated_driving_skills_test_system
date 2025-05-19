#include "IMU.h"
extern bool flagSpeed;

IMU::IMU()
{
}
IMU::IMU(Car* car) : carPtr(car), Acceleration(0.0), GyroX(0.0), GyroY(0.0)
{
}


// הפונקציה שולטת על מהירות הרכב על ידי האצה או האטה עד להגעה למהירות היעד.
// The function controls the vehicle's speed by accelerating or decelerating to reach the target speed.
void IMU::controlSpeed(double targetSpeed, double accelerationRate, double decelerationRate) {
	globalPrint.print(
		"IMU: Initiating speed control. Target: " + to_string(targetSpeed) +
		" km/h, Acceleration Rate: " + to_string(accelerationRate) +
		" km/h/s, Deceleration Rate: " + to_string(decelerationRate) + " km/h/s");
	// accelerate - האצה
	if (carPtr->getSpeed() < targetSpeed) {
		// until the current speed reaches the target speed.
		while (carPtr->getSpeed() < targetSpeed) {
			// Adding the acceleration rate to the current speed - הוספת קצב האצה למהירות הנוכחית
			carPtr->setSpeed(carPtr->getSpeed() + accelerationRate);
			if (carPtr->getSpeed() > targetSpeed)	carPtr->setSpeed(targetSpeed);
			globalPrint.print("IMU: Accelerating. Current speed: " + to_string(carPtr->getSpeed()) + " km/h");
			// Pausing the process for one second to simulate acceleration time.
			this_thread::sleep_for(chrono::seconds(1)); // הדמיה שלוקח זמן להאיץ
		}
	}
	// If the current speed is higher than the target speed, the vehicle will decelerate.
	else if (carPtr->getSpeed() > targetSpeed) {
		// The loop continues until the current speed reaches the target speed.
		while (carPtr->getSpeed() > targetSpeed) {
			// Subtracting the deceleration rate from the current speed - הפחתת קצב ההאטה מהמהירות הנוכחית
			carPtr->setSpeed(carPtr->getSpeed() - decelerationRate);
			if (carPtr->getSpeed() < targetSpeed) carPtr->setSpeed(targetSpeed);
			globalPrint.print("IMU: Decelerating. Current speed: " + to_string(carPtr->getSpeed()) + " km/h");
			// Pausing the process for one second to simulate deceleration time.
			this_thread::sleep_for(chrono::seconds(1)); // הדמיה שלוקח זמן להאט
		}
	}
	else {
		// If the current speed is already equal to the target speed.
		globalPrint.print("IMU: Current speed is already at the target.");
	}
	globalPrint.print("IMU: Speed control complete. Final speed: " + to_string(carPtr->getSpeed()) + " km/h");
}

// הפונקציה עוקבת אחר תהליך שינוי המהירות עד שהיעד מושג או שהזמן המרבי חלף.
// The function monitors the speed change process until the target is reached or the maximum duration has elapsed.
void IMU::monitorSpeedChangeProcess(
	string reason,
	function<bool(double)> isTargetReached,
	float maxDuration,
	double targetSpeed
) {
	globalPrint.print("IMU: Monitoring speed change due to " + reason + ". Target speed: " + to_string(targetSpeed) + " km/h. Max duration: " + to_string(maxDuration) + " seconds.");
	// Saving the start time of the monitoring process.
	auto startTime = chrono::high_resolution_clock::now();
	double previousCarSpeed = carPtr->getSpeed();
	while (!isTargetReached(carPtr->getSpeed())) {
		this_thread::sleep_for(chrono::seconds(1));
		auto currentTime = chrono::high_resolution_clock::now();
		// Calculating the elapsed duration.
		auto duration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
		// Checking if the vehicle is decelerating.
		if (isCarDecelerating(previousCarSpeed, carPtr->getSpeed())) {
			globalPrint.print("IMU: The car is decelerating. Speed: " + to_string(carPtr->getSpeed()) + " km/h");
		}
		// If the vehicle has not reached the target and is not decelerating.
		else if (!isTargetReached(carPtr->getSpeed())) {
			// Printing a message that the vehicle is not progressing towards the target speed.
			globalPrint.print("IMU: The car is not progressing towards the target speed. Speed: " + to_string(carPtr->getSpeed()) + " km/h");
		}

		previousCarSpeed = carPtr->getSpeed();

		// Checking if the maximum duration has elapsed.
		if (duration.count() >= maxDuration) {
			// הדפסת אזהרה שהרכב לא הגיע ליעד בזמן והחלה פעולה מאולצת.
			// Printing a warning that the vehicle did not reach the target in time and initiating a forced action.
			globalPrint.print("IMU: Warning: Car did not reach target within " + to_string(maxDuration) + " seconds. Initiating forced action.");
			// If the target speed is lower than the previous speed (should decelerate).
			if (targetSpeed < previousCarSpeed) {
				// If the target speed is 0 (stopping).
				if (targetSpeed == 0.0f) {
					// Emergency deceleration rate - קצב האטת חירום
					double emergencyDecelerationRate = 27.0f;
					// עדכון המהירות בהתאם לקצב ההאטה המקסימלי, לא לרדת מתחת ל-0.
					// Updating the speed according to the maximum deceleration rate, not going below 0.
					carPtr->setSpeed(max(targetSpeed, carPtr->getSpeed() - emergencyDecelerationRate));
					// ************************* הורדת ניקוד *************************************
				}
				else {
					// אם מהירות היעד גדולה מ-0 (האטה רגילה נכשלה).
					// If the target speed is greater than 0 (normal deceleration failed).
					// כאן תוסיף את הלוגיקה להורדת ניקוד על אי-האטה נכונה (אם רלוונטי ל-IMU)
					// Here you would add the logic for deducting points for incorrect deceleration (if relevant to IMU).
				}
			}
		}
	}
	globalPrint.print("IMU: The car reached the target due to " + reason + ". Final speed: " + to_string(carPtr->getSpeed()) + " km/h.");
}

void IMU::stoppingImu(string reason) {
	monitorSpeedChangeProcess(reason, [](double currentSpeed) { return currentSpeed <= 0; }, 2.5f, 0.0f);
}

void IMU::slowdownImu(string reason) {
	double targetSpeed = 0.0f;
	if (reason == "crosswalk") {
		targetSpeed = 30.0f;
	}
	else if (reason == "before turn") {
		targetSpeed = 40.0f;
	}
	monitorSpeedChangeProcess(reason, [targetSpeed](double currentSpeed) { return currentSpeed <= targetSpeed; }, 4.0f, targetSpeed);
}

void IMU::monitorMaxSpeed()
{
	while (flagSpeed) {
		if (carPtr != nullptr) {
			double currentSpeed = carPtr->getSpeed();
			double maxSpeed = carPtr->getMaxSpeed();
			if (currentSpeed > maxSpeed) {
				globalPrint.print("IMU: Warning! Vehicle speed (" + to_string(currentSpeed) + " km/h) exceeds maximum speed (" + to_string(maxSpeed) + " km/h).");
				// ************************* הורדת נקודות על מהירות גבוהה מהמותר *************************************
				globalPrint.print("IMU: Points deducted for exceeding maximum speed.");
			}
		}
		else {
			globalPrint.print("IMU: Error! Car pointer is null in monitorMaxSpeed.");
			// אולי כדאי לצאת מהלולאה או לטפל במצב הזה בצורה אחרת
			break;
		}
		// בדיקה כל פרק זמן מסוים (למשל, כל שנייה)
		this_thread::sleep_for(chrono::seconds(1));
	}
}

// Function that checks if the vehicle is decelerating.
bool IMU::isCarDecelerating(double previousSpeed, double currentSpeed) {
	return previousSpeed > currentSpeed;
}

void IMU::IMUplay()
{
	double speed;
	// Create an input file stream
	ifstream inputFile("IMU.txt");
	if (!inputFile.is_open()) {
		globalPrint.printError("didn't success to open the file: IMU.txt");
		return;
	}
	string line;
	//read the file line by line
	while (getline(inputFile, line)) {
		stringstream ss(line);
		double acceleration, currentSpeed, gyroX, gyroY;

		if (ss >> acceleration >> currentSpeed >> gyroX >> gyroY ) {
			setAcceleration(acceleration);
			carPtr->setSpeed(currentSpeed);
			setGyroX(gyroX);
			setGyroY(gyroY);

		////
			// חישוב גודל וקטור המהירות הזויתית במישוק XY
			speed = sqrt((getGyroX() * getGyroX()) + (getGyroY() * getGyroY())); // Calculate the total speed
			// מחשב את התאוצה הזוויתית על ידי מציאת השינוי במהירות הזוויתית ביחס למהירות הזוויתית הקודמת ומחלק אותו במרווח הזמן (dt)
			acceleration = (speed - carPtr->getPrevSpeed()) / dt; // Calculate acceleration
			// המרת המהירות הזויתית לקמש והכפלתה בזמן שעבר
			distance = (speed * 1000 / 3600 * GettimeSensor()); // 1000=1 k"m, 3600 secoend=1 hour

			carPtr->setDistance(carPtr->getDistance() + distance); // Update the speed the car has traveled
		////

			globalPrint.print(
				string("Acceleration: ") + to_string(getAcceleration()) +
				", speed: " + to_string(carPtr->getSpeed()) +
				", GyroX: " + to_string(getGyroX()) +
				", GyroY: " + to_string(getGyroY())
			);

			// Pause the execution of the current thread for one second
			this_thread::sleep_for(chrono::seconds(1));
		}
		else {
			globalPrint.printError("Invalid line in IMU.txt: " + line);
		}
	}
	inputFile.close();
}
