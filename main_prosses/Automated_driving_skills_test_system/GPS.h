#pragma once
class GPS
{
public:
	GPS();
	//�� ���� (Latitude)
private:
	double Latitude;
	//�� ���� (Longitude)
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
