#pragma once
class Time
{
	int day;
	int hour;

public:

	Time(int inputDay = 0, int inputHour = 0);
	void setTime(int inputDay = 0, int inputHour = 0);

	void setDay(int inputDay);

	void setHour(int inputHour);

	int getDay();

	int getHour();

	void operator ++ ();

	Time operator + (Time inputTime);

	bool operator > (Time inputTime);

	bool operator < (Time inputTime);

	bool operator >= (Time inputTime);

	bool operator <= (Time inputTime);

	bool operator == (Time inputTime);

	bool operator != (Time inputTime);

	Time operator + (int inputHours);

	int timeToInteger();

	void integerToTime(int convTime);


};