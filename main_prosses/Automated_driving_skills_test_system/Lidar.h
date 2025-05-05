#pragma once
class Lidar
{
public:
	Lidar();
//זווית אופקית (Azimuth)
private:
	double Azimuth;
//טווח (Range)
private:
	double Range;
	//get and set methods
public:
	void setAzimuth(double Azimuth) { this->Azimuth = Azimuth; }
public:
	double getAzimuth() { return Azimuth; }
public:
	void setRange(double Range) { this->Range = Range; }
public:
	double getRange() { return Range; }
public:
	void Lidarplay();
};

