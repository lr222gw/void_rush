#pragma once
#include <chrono>
//git
class DeltaTime {
private:
	std::chrono::steady_clock::time_point t_start;
	std::chrono::steady_clock::time_point t_end;
	double pDt;
	double RealTime;
public:
	DeltaTime();
	void setDeltaTime(double newdelta);
	void restartClock();
	double dt()const;
	double getRealTime()const;
};