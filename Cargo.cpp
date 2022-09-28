#include "Cargo.h"

int Cargo::autoPromotion = -1; //initialization before reading from file
int Cargo::maxWaitTime = -1;  //initialization before reading from file

Cargo::Cargo(Type cargoT, Time readyT, int ID, double delDis, int unloadT, int cargocost) : cargoID(ID) //constructor for needed initializations with file inputs
{
	cargoType = cargoT;
	readyTime = readyT;
	deliveryDistance = delDis;
	waitTime = 0;
	unloadTime = unloadT;
	cost = cargocost;
	isPromoted = false;
	isMaxWait = false;
}

void Cargo::setDeliveryTruckID(int ID)
{
	deliveryTruckID = ID;
}

int Cargo::getDeliveryTruckID()
{
	return deliveryTruckID;
}

Cargo::Cargo(int ID) :cargoID(ID) //constructor with only ID for promotion and cancellation
{
	waitTime = 0; //initializing wait time of each cargo
}

void Cargo::setDeliveryTime(Time inputTime)
{
	deliveryTime = inputTime;
}

Time Cargo::getDeliveryTime()
{
	return deliveryTime;
}

void Cargo::setType(Type cargoT) //normal,vip, or special
{
	cargoType = cargoT;
}

Type Cargo::getType()
{
	return cargoType;
}

void Cargo::incrementWaitTime() //incrementing wait time of cargos
{
	waitTime = waitTime + 1;
}

int Cargo::getWaitTime()
{
	return waitTime;
}

int Cargo::getAutoPromotion()
{
	return autoPromotion;
}

int Cargo::getID()
{
	return cargoID;
}

bool Cargo ::operator == (Cargo& inputCargo) //operator overloading for comparing cargo IDs
{
	if (cargoID == inputCargo.getID())
		return true;
	else
		return false;
}

void Cargo::setCost(int newcost) //setting cost in case of promotion
{
	cost = newcost;
}

int Cargo::getCost()
{
	return cost;
}

void Cargo::setAutoPromotion(int autoP)
{
	Cargo::autoPromotion = autoP;
}

void Cargo::setMaxWaitTime(int maxW)
{
	Cargo::maxWaitTime = maxW;
}

float Cargo::calculatePriority() //calculating priority of each VIP cargo
{
	float prepTime = readyTime.timeToInteger();
	cargoPriority = cost / (deliveryDistance * prepTime); //as cost increases,cargo priority increases
	return cargoPriority * 100;								//as distance decreases and ready time is earlier, can be delivered faster so profit/priority increases 
}

void Cargo::promote()
{
	isPromoted = true;
}

bool Cargo::getPromotion()
{
	return isPromoted;
}

double Cargo::getDeliveryDistance()
{
	return deliveryDistance;
}

int Cargo::getUnloadTime()
{
	return unloadTime;
}

Time Cargo::getReadyTime()
{
	return readyTime;
}


int Cargo::getMaxWaitTime()
{
	return maxWaitTime;
}

void Cargo::ChangeMaxWait()
{
	isMaxWait = true;
}

bool Cargo::checkMaxWait()
{
	return isMaxWait;
}

void Cargo::setWaitTime(int input)
{
	waitTime = input;
}