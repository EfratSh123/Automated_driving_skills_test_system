#pragma once
class GPS
{
public:
	GPS();
	//че шезб (Latitude)
private:
	double Latitude;
	//че аешк (Longitude)
private:
	double Longitude;
public:
	void setLatitude(double Latitude) { this->Latitude = Latitude; }
public:
	double getLatitude() { return Latitude; }
public:
	void setLongitude(double Longitude) { this->Longitude = Longitude; }
public:
	double getLongitude() { return Longitude; }
public:
	void GPSplay();

};
