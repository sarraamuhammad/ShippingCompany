#pragma once
#include "LinkedQueue.h"
#include "LinkedPriorityQueue.h"
#include "LinkedList.h"
#include "NormalCargoList.h"
#include "Cargo.h"
#include "Truck.h"
#include "UI.h"
#include <iostream>
#include <fstream>
using namespace std;

class UI;		//forward declarations for needed classes inside Company class
class ReadyEvent;
class CancellationEvent;
class PromotionEvent;
class Event;


class Company
{
private:
	Time currentTime; //current time step in simulation


	//lists of cargos and trucks of all types and status

	LinkedPriorityQueue<Cargo*>* WaitingVIPCargos;
	NormalCargoList* WaitingNormalCargos;
	LinkedQueue<Cargo*>* WaitingSpecialCargos;

	LinkedQueue<Cargo*>* DeliveredNormalCargos;
	LinkedQueue<Cargo*>* DeliveredVIPCargos;
	LinkedQueue<Cargo*>* DeliveredSpecialCargos;

	LinkedQueue<Truck*>* WaitingNormalTrucks;
	LinkedQueue<Truck*>* WaitingVIPTrucks;
	LinkedQueue<Truck*>* WaitingSpecialTrucks;
	LinkedPriorityQueue<Truck*>* LoadingTrucks;

	//flags for checking that only one type of cargo is loading at the same time (excluding max wait rule)

	bool isLoadingVIPCargo;
	bool isLoadingNormalCargo;
	bool isLoadingSpecialCargo;

	//list of events

	LinkedQueue<Event*>* Events;

	int modeOfOperation;

	UI* userInterface;

	ifstream inputFile;
	ofstream outputFile;
	string FileName;

	LinkedPriorityQueue<Truck*>* MovingTrucks;

	LinkedQueue<Truck*>* inCheckUpNormalTrucks;
	LinkedQueue<Truck*>* inCheckUpSpecialTrucks;
	LinkedQueue<Truck*>* inCheckUpVIPTrucks;

	//collecting delivered cargos for statistics

	LinkedPriorityQueue<Cargo*>* statisticsQueue;

	int TotalWaitTime; //calculating total wait time of all cargos 
	float TotalActiveTime; //calculating total active time of all trucks
	int totalAutoPromoted; //calculating total count of auto-promoted cargos
	float totalUtilization; //calculating total utilization of all trucks

public:
	Company();
	~Company();

	NormalCargoList* getWaitingNormalCargos();
	LinkedPriorityQueue<Cargo*>* getWaitingVIPCargos();
	LinkedQueue<Cargo*>* getWaitingSpecialCargos();

	LinkedQueue<Cargo*>* getDeliveredNormalCargos();
	LinkedQueue<Cargo*>* getDeliveredVIPCargos();
	LinkedQueue<Cargo*>* getDeliveredSpecialCargos();

	Time getCurrentTime();

	LinkedQueue<Truck*>* getWaitingNormalTrucks();
	LinkedQueue<Truck*>* getWaitingVIPTrucks();
	LinkedQueue<Truck*>* getWaitingSpecialTrucks();
	LinkedPriorityQueue<Truck*>* getLoadingTrucks();

	LinkedPriorityQueue <Truck*>* getMovingTrucks();

	LinkedQueue<Truck*>* getInCheckUpNormalTrucks();
	LinkedQueue<Truck*>* getInCheckUpVIPTrucks();
	LinkedQueue<Truck*>* getInCheckUpSpecialTrucks();


	void readTruckInfo();
	void readCargoInfo();
	void readEvent();
	void readInputFile();
	void Simulate();
	void EndHour();
	void AssignCargo();
	void AssignVIPCargo();
	void AssignNormalCargo();
	void AssignSpecialCargo();
	void CreateOutputFile();
	void CalculateStatistics(Time Tsim);
	void MoveTrucksToMoving();
	void deliveryProcess();

};