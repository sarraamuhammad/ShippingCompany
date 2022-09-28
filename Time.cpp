#include "Time.h"

Time::Time(int inputDay, int inputHour) //constructor for time
{
	setDay(inputDay);
	setHour(inputHour);
}

void Time::setTime(int inputDay, int inputHour)
{
	day = inputDay;
	setHour(inputHour);
}

void Time::setDay(int inputDay)
{
	day = inputDay;
}

void Time::setHour(int inputHour) //setting hour within limit
{
	if (inputHour >= 0 && inputHour <= 23)
		hour = inputHour;
	else
		hour = 0;
}

int Time::getDay()
{
	return day;
}

int Time::getHour()
{
	return hour;
}

void Time ::operator ++ () //operator overloading for ++ to increment
{
	hour++;
	if (hour > 23) //checking if hours completed a day
	{
		hour = hour - 24;
		day = day++;
	}
}

Time Time ::operator + (Time inputTime) //operator overloading for + to add times together
{
	int newday = 0;
	int	newhour = hour + inputTime.hour;
	while (newhour > 23) //checking if hours completed a day
	{
		newday++;
		newhour = newhour - 24;
	}
	newday = newday + day + inputTime.day;
	Time newTime(newday, newhour); //setting new time with updates times
	return newTime;
}

bool Time ::operator > (Time inputTime) //operator overloading for time comparison
{
	if (day > inputTime.day)
	{
		return true;
	}
	else if (day == inputTime.day)
	{
		if (hour > inputTime.hour)
			return true;
	}
	return false;
}

bool Time ::operator < (Time inputTime) //operator overloading for time comparison
{
	if (day < inputTime.day)
	{
		return true;
	}
	else if (day == inputTime.day)
	{
		if (hour < inputTime.hour)
			return true;
	}
	return false;
}

bool Time ::operator >= (Time inputTime) //operator overloading for time comparison
{
	if (day > inputTime.day)
	{
		return true;
	}
	else if (day == inputTime.day)
	{
		if (hour >= inputTime.hour)
			return true;
	}
	return false;
}

bool Time ::operator <= (Time inputTime) //operator overloading for time comparison
{
	if (day < inputTime.day)
	{
		return true;
	}
	else if (day == inputTime.day)
	{
		if (hour <= inputTime.hour)
			return true;
	}
	return false;
}

bool Time ::operator == (Time inputTime) //operator overloading for time comparison
{
	if (day == inputTime.day && hour == inputTime.hour)
		return true;
	else
		return false;
}

bool Time ::operator != (Time inputTime) //operator overloading for time comparison
{
	if (day != inputTime.day || hour != inputTime.hour)
		return true;
	else
		return false;
}

Time Time ::operator + (int inputHours) //operator overloading for hours addition
{
	int newday = day;
	int newhour = hour + inputHours;
	while (newhour > 23)
	{
		newhour -= 24;
		newday++;
	}
	Time newTime(newday, newhour);
	return newTime;
}

int Time::timeToInteger() //converting time to number for calculations
{
	return hour + day * 24;
}

void Time::integerToTime(int convTime)
{
	day = 0;
	while (convTime > 23) //checking if hours completed a day
	{
		day++;
		convTime = convTime - 24;
	}
	hour = convTime;
}
