#include "Truck.h"

int Truck::numberOfTrucks = 0; //initializing before reading from file
int Truck::journeysBeforeCheckup = 0; //initializing before reading from file

Truck::Truck(Type tType, int tSpeed, int tCapacity, int checkUpD) //constructor for needed initializations with file inputs
{
	LoadedCargos = new LinkedPriorityQueue<Cargo*>;
	numberOfTrucks++; //for truck ID, as each truck gets created it has an ID assigned
	checkup = false;
	isCarryMaxWait = false;
	truckID = numberOfTrucks;
	truckType = tType;
	speed = tSpeed;
	truckCapacity = tCapacity;
	numberOfDeliveries = 0;
	checkupDuration = checkUpD;
	totalDeliveries = 0;
	tCargosDelivered = 0;
	activeTime = 0;
	startLoadTime = 0;
	utilization = 0;
	totalLoadingTime = 0;

}

Type Truck::getType()
{
	return truckType;
}

bool Truck::isCarryingMaxWait()	//checking whether cargo is carrying cargos that reached max wait limit
{
	return isCarryMaxWait;
}

void Truck::changeIsCarryingMaxWait(bool check)	//changing status of truck if carrying cargos that reached max wait limit
{
	isCarryMaxWait = check;
}

void Truck::setStartLoadTime(Time currentTime) //time at which truck starts loading its cargos
{
	startLoadTime = currentTime;
}

void Truck::setActiveTime(Time currentTime) //setting active time of trucks for satistics calculation
{
	Time temp;
	temp.integerToTime(startLoadTime.timeToInteger() + totalLoadingTime);
	if (temp.getHour() >= 0 && temp.getHour() <= 4)
		activeTime += currentTime.timeToInteger() - startLoadTime.timeToInteger() - 5;
	else
		activeTime += currentTime.timeToInteger() - startLoadTime.timeToInteger();
}

int Truck::getJourneysBeforeCheckup()
{
	return journeysBeforeCheckup;
}

int Truck::getDeliveries() //returns number of deliveries done by truck
{
	return numberOfDeliveries;
}

void Truck::incrementJournies() //incrementing number of journies after every delivery
{
	numberOfDeliveries++;
	totalDeliveries++;
}

void Truck::setTCargosDelivered(int count) //incrementing number of cargos delivered by each truck
{
	tCargosDelivered += count;
}

float Truck::TruckUtilization(Time Tsim) //truck utilization equation for statistics
{
	if (totalDeliveries != 0)
	{
		utilization = (((float)tCargosDelivered) / (truckCapacity * totalDeliveries)) * ((float)activeTime / Tsim.timeToInteger());
	}
	else //denominator is zero, cannot calculate
	{
		utilization = 0;
	}
	return utilization;
}

void Truck::setJourneysBeforeCheckUp(int journeys)
{
	Truck::journeysBeforeCheckup = journeys;
}

int Truck::getID()
{
	return truckID;
}

LinkedPriorityQueue<Cargo*>* Truck::getLoadedCargos() //returning queue of cargos inside truck
{
	return LoadedCargos;
}

int Truck::getCapacity()
{
	return truckCapacity;
}

int Truck::calculateDeliveryInterval() //calculating delivery interval of truck to calculate arrival time

{
	int countLoaded = LoadedCargos->getCount();
	double greatestDistance = 0;
	int loadingTime = 0;
	Cargo* Temp;
	for (int i = 0; i < countLoaded; i++) //getting greatest distance travelled by truck
	{
		LoadedCargos->dequeue(Temp);
		loadingTime += Temp->getUnloadTime(); //incrementing unload time
		if (i == countLoaded - 1)
		{
			greatestDistance = Temp->getDeliveryDistance();
		}

		LoadedCargos->enqueue(Temp);
	}
	deliveryInterval = 2 * ceil(greatestDistance / speed) + loadingTime;
	return deliveryInterval;
}

float Truck::calculateCargoDelivery(Time currentTime) //calculating cargo delivery time to sort trucks inside moving queue
{
	Time tempTime;
	Cargo* Temp = nullptr;
	Cargo* Temp2 = nullptr;
	int numberOfLoadedCargos = LoadedCargos->getCount() - 1; //decrementing count of cargos to be delivered
	float cargoDeliveryDistance = 0;
	int cargoInterval;
	int PreviousUnloadTimes = totalLoadingTime;
	if (LoadedCargos->peek(Temp))
	{
		LoadedCargos->dequeue(Temp);
		for (int i = 0; i < numberOfLoadedCargos; i++)
		{
			LoadedCargos->dequeue(Temp2);
			PreviousUnloadTimes -= Temp2->getUnloadTime();
			LoadedCargos->enqueue(Temp2);
		}
		cargoDeliveryDistance = Temp->getDeliveryDistance();
		cargoInterval = ceil(float(cargoDeliveryDistance) / speed) + PreviousUnloadTimes + movingTime.timeToInteger();
		tempTime.integerToTime(cargoInterval);
		Temp->setDeliveryTime(tempTime);
		LoadedCargos->enqueue(Temp, 1000 / Temp->getDeliveryDistance()); //enqueue cargo again according to distance
		float returnValue = -cargoInterval;
		return returnValue;
	}
}

Truck::~Truck()
{
	delete LoadedCargos;
}

void Truck::setMovingTime(Time currentTime) // setting time of departure of truck
{
	totalLoadingTime = 0;
	int countLoaded = LoadedCargos->getCount();
	LinkedQueue<Cargo*> TempQueue;
	Cargo* Temp;
	for (int i = 0; i < countLoaded; i++) //calculating total loading time
	{
		LoadedCargos->dequeue(Temp);
		totalLoadingTime += Temp->getUnloadTime();
		TempQueue.enqueue(Temp);
	}
	while (!TempQueue.isEmpty())
	{
		TempQueue.dequeue(Temp);
		LoadedCargos->enqueue(Temp);
	}

	movingTime.integerToTime(totalLoadingTime);
	movingTime = movingTime + currentTime;
	if (movingTime.getHour() >= 0 && movingTime.getHour() <= 4) //if moving time within off-hours, shifting to start at on-hours
	{
		movingTime.setHour(5 + movingTime.getHour());
	}

	for (int i = 0; i < countLoaded; i++)
	{
		LoadedCargos->dequeue(Temp);
		Temp->setWaitTime(Temp->getWaitTime() + movingTime.timeToInteger() - currentTime.timeToInteger());
		TempQueue.enqueue(Temp);
	}
	while (!TempQueue.isEmpty())
	{
		TempQueue.dequeue(Temp);
		LoadedCargos->enqueue(Temp);
	}
}

void Truck::setArrivalTime() //setting arrival time of truck at company
{
	int dInterval = calculateDeliveryInterval();
	arrivalTime.integerToTime(dInterval);
	arrivalTime = movingTime + arrivalTime;
}

void Truck::setCheckUpTime(Time currentTime) //setting checkup duration and time truck will leave the checkup queue
{
	checkUpTime.integerToTime(checkupDuration);
	checkUpTime = checkUpTime + currentTime;
}

int Truck::getActiveTime() //returning total active time of truck
{
	return activeTime;
}

Time Truck::getMovingTime()
{
	return movingTime;
}

Time Truck::getArrivalTime()
{
	return arrivalTime;
}

Time Truck::getCheckUpTime()
{
	return checkUpTime;
}


void Truck::resetDeliveries() //resetting number of deliveries after checkup
{
	numberOfDeliveries = 0;
}