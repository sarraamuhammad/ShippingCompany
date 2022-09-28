#include "Event.h"

Event::Event(Company* pComp)
{
	pCompany = pComp;
}

Time Event::getTime() //returns time of event
{
	return eventTime;
}