#pragma once
#include "Event.h"

class Company;

class ReadyEvent : public Event
{
	int ID;
	int LT;				//in hours
	Type cargoType;
	double dist;
	int cost;

public:

	ReadyEvent(Company* pcomp);

	void ReadEvent(ifstream& inputFile);

	void Execute();
};