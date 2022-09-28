#pragma once
#include "Time.h"
#include <iostream>
using namespace std;

enum Type
{
	Normal,
	Special,
	VIP
};


class Cargo
{
	Time readyTime; //Preparation Time
	const int cargoID; //Cargo ID
	int unloadTime;	//in hours
	Type cargoType; //Cargo ID
	double deliveryDistance; //Delivery Distance
	int cost; //Cost
	static int maxWaitTime; //MaxWaitTime
	int waitTime; //Time a cargo is loading and waiting
	static int autoPromotion;   //in days
	float cargoPriority; //Priority to be enqueued in truck
	bool isPromoted; //checks whether the cargo is loading so as not to apply max wait rule
	bool isMaxWait; //checks if cargo has surprassed the max wait time
	Time deliveryTime; //Time for a cargo to be delivered
	int deliveryTruckID; //ID of the truck that delivered the cargo (For statistics) 

public:

	Cargo(Type cargoT, Time readyT, int ID, double delDis, int unloadT, int cargocost); //Constuctor used in input file
	Cargo(int ID); //Dummy Constructor for operator overloading used in cancellation and promotion
	void setType(Type cargoT); 
	Type getType();
	void incrementWaitTime(); //In order to keep track of it
	void setWaitTime(int);
	int getWaitTime();
	int getAutoPromotion();
	int getID();
	bool operator == (Cargo& inputCargo);
	void setCost(int newcost);
	int getCost();
	void static setAutoPromotion(int);
	void static setMaxWaitTime(int);
	void ChangeMaxWait();
	bool checkMaxWait();
	int getMaxWaitTime();
	float calculatePriority(); //priority to be enqueued in the truck
	void promote(); //For AutoP
	bool getPromotion();
	double getDeliveryDistance();
	int getUnloadTime();
	Time getReadyTime(); //returns preparation time
	Time getDeliveryTime(); //returns when a Cargo is supposed to be delivered
	void setDeliveryTime(Time delTime); 
	void setDeliveryTruckID(int ID); //Sets ID of the truck in order to be used in statistics
	int getDeliveryTruckID();
	friend ostream& operator << (ostream& out, Cargo** cargoInput);
};