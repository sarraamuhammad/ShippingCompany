#include "ReadyEvent.h"

ReadyEvent::ReadyEvent(Company* pcomp) :Event(pcomp) //constructor of ready event
{
}

void ReadyEvent::ReadEvent(ifstream& inputFile) //reading event from file
{
	char inputCargoType; //reading cargo type from file
	inputFile >> inputCargoType;

	switch (inputCargoType) //switch case on input to set type of cargo to be prepared
	{

	case 'N':
		cargoType = Normal;
		break;

	case 'S':
		cargoType = Special;
		break;

	case 'V':
		cargoType = VIP;
		break;
	}

	int day, hour;
	inputFile >> day; //reading time of event
	inputFile.ignore();
	inputFile >> hour >> ID >> dist >> LT >> cost; //reading info of cargo
	if (hour >= 0 && hour <= 4)
	{
		hour = 5;
	}
	eventTime.setHour(hour);
	eventTime.setDay(day);


}

void ReadyEvent::Execute() //executing event
{

	LinkedList<Cargo*>* normalPtr = pCompany->getWaitingNormalCargos();
	LinkedQueue<Cargo*>* specialPtr = pCompany->getWaitingSpecialCargos();
	LinkedPriorityQueue<Cargo*>* vipPtr = pCompany->getWaitingVIPCargos();

	Cargo* newCargo = new Cargo(cargoType, eventTime, ID, dist, LT, cost); //creating new cargo with info aquired from file
	switch (cargoType)
	{

	case Normal: //if cargo is normal, inserting in end of normal cargo list
		normalPtr->InsertEnd(newCargo);
		break;

	case Special: //if cargo is special, enqueue cargo in its queue
		specialPtr->enqueue(newCargo);
		break;

	case VIP: //if cargo is vip, enqueue cargo in its queue according to its priority
		vipPtr->enqueue(newCargo, newCargo->calculatePriority());
		break;
	}
}