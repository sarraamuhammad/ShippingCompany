#include "PromotionEvent.h"

PromotionEvent::PromotionEvent(Company* pcomp) :Event(pcomp) //constructor for promotion event
{
}

void PromotionEvent::ReadEvent(ifstream& inputFile)  //reading event from file
{
	int day, hour;
	inputFile >> day; //reading time of event
	inputFile.ignore();
	inputFile >> hour >> promoteID >> extraCost; //reading ID of cargo to be promoted with extra cost
	if (hour >= 0 && hour <= 4)
	{
		hour = 5;
	}
	eventTime.setHour(hour);
	eventTime.setDay(day);
}

void PromotionEvent::Execute()
{
	NormalCargoList* normalPtr = pCompany->getWaitingNormalCargos();
	LinkedPriorityQueue<Cargo*>* vipPtr = pCompany->getWaitingVIPCargos();
	Cargo temp(promoteID); //temp cargo with ID of one to be promoted
	Cargo* tempCargo = &temp;
	Cargo* retrievedCargo = normalPtr->RetrieveCargoFromID(tempCargo); //retrieving cargo to be promoted from normal list and removing its data from list
	if (!retrievedCargo)
		return;
	retrievedCargo->setCost(retrievedCargo->getCost() + extraCost); //updating cost of promoted cargo
	retrievedCargo->setType(VIP);
	vipPtr->enqueue(retrievedCargo, retrievedCargo->calculatePriority()); //adding promoted cargo to VIP list
}