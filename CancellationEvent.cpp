#include "CancellationEvent.h"

CancellationEvent::CancellationEvent(Company* pcomp) :Event(pcomp) //constructor for cancellation event
{
}

void CancellationEvent::ReadEvent(ifstream& inputFile)//reading event from file
{

	int day, hour;
	inputFile >> day;//reading time of event
	inputFile.ignore();
	inputFile >> hour >> ID;//reading ID of cargo to cancel
	if (hour >= 0 && hour <= 4)
	{
		hour = 5;
	}
	eventTime.setHour(hour);
	eventTime.setDay(day);


}

void CancellationEvent::Execute() //executing event
{
	NormalCargoList* normalPtr = pCompany->getWaitingNormalCargos();
	Cargo temp(ID);
	Cargo* tempCargo = &temp;
	normalPtr->RetrieveCargoFromID(tempCargo); //removing cargo to be cancelled from list and deleting its info
}