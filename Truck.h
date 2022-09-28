#pragma once
#include "Time.h"
#include "Cargo.h"
#include "LinkedQueue.h"
#include"LinkedPriorityQueue.h"
#include <iostream>
using namespace std;

class Truck
{
	Type truckType;
	static int numberOfTrucks;
	int truckCapacity;
	int activeTime; //keeping track of truck active time for statistics
	int numberOfDeliveries; //keeping track of deliveries done by each truck
	int speed;
	int checkupDuration;
	static int journeysBeforeCheckup;
	bool checkup;  //checking if truck needs maintenence
	int truckID;
	int totalDeliveries; //keeping track of deliveries done by each truck
	int tCargosDelivered; //counting number of total cargos delivered
	LinkedPriorityQueue<Cargo*>* LoadedCargos; //to store cargos loaded by a truck
	double deliveryInterval;
	Time movingTime; //departure time of truck to start delivery process
	Time arrivalTime; //arrival time after delivering and returning to company;
	Time checkUpTime; //start of check up time for truck
	bool isCarryMaxWait;  //checks whether truck is carrying cargos which reached max wait limit
	Time startLoadTime; //time at which truck starts loading its cargos
	float utilization; //utilization for statistics
	int totalLoadingTime; //keeping track of loading time to calculate moving time



public:

	Truck(Type tType, int tSpeed, int tCapacity, int checkUpDuration);
	~Truck();
	Type getType();
	void static setJourneysBeforeCheckUp(int journeys);
	void setStartLoadTime(Time currentTime);

	void changeIsCarryingMaxWait(bool check);
	bool isCarryingMaxWait();
	void incrementJournies();
	int getJourneysBeforeCheckup();
	int getDeliveries();
	void setActiveTime(Time currentTime);
	int getActiveTime();
	float TruckUtilization(Time Tsim);
	int getID();
	int getCapacity();
	void setTCargosDelivered(int count);
	int calculateDeliveryInterval();
	float calculateCargoDelivery(Time currentTime);
	void setMovingTime(Time currentTime);
	Time getMovingTime();
	void setArrivalTime();
	Time getArrivalTime();
	void setCheckUpTime(Time currentTime);
	Time getCheckUpTime();
	void resetDeliveries();
	LinkedPriorityQueue<Cargo*>* getLoadedCargos();
	friend ostream& operator << (ostream& out, Truck** truckInput);
};