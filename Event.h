#pragma once
#include<iostream>
#include "Cargo.h"
#include "Company.h"
#include <fstream>
using namespace std;

class Company;

class Event
{
protected:

	Time eventTime;
	Company* pCompany;

public:

	Event(Company* pComp);
	virtual void Execute() = 0;	//Will call each event
	virtual void ReadEvent(ifstream& inputFile) = 0; //Reads the event from the input file
	Time getTime(); //gets time of event
};