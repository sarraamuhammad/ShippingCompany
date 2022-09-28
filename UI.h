#pragma once
#include "Company.h"
#include <iostream>
using namespace std;

class Company;

class UI
{
private:
	Company* pCompany;

public:

	UI(Company* pComp);

	void printInfo();
	int getModeOfOperation();
	string getUserInput();
	void printMessage(string msg);
};