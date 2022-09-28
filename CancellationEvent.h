#pragma once
#include "Event.h"
#include <iostream>
using namespace std;

class Company;

class CancellationEvent :public Event
{
	int ID;

public:

	CancellationEvent(Company* pcomp);

	void ReadEvent(ifstream& inputFile);

	void Execute();
};

