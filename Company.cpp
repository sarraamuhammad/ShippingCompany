#include "Company.h"
#include"ReadyEvent.h"
#include"CancellationEvent.h"
#include"PromotionEvent.h"
#include "Event.h"
#include <Windows.h>

Company::Company() //constructor for needed initializations
{
	currentTime.setTime(1, 0);  //setting start time of simulation with day 1 and hour 0

	WaitingNormalCargos = new NormalCargoList;	//Linked list for normal cargos to be able to retrieve to promote or cancel easily
	WaitingVIPCargos = new LinkedPriorityQueue<Cargo*>; //Pri-Q to sort by priority
	WaitingSpecialCargos = new LinkedQueue<Cargo*>; //Normal Queue as it only needs time of arrival

	DeliveredNormalCargos = new LinkedQueue<Cargo*>;	//List of delivered normal cargos to count
	DeliveredSpecialCargos = new LinkedQueue<Cargo*>; //List of delivered vip cargos to count 
	DeliveredVIPCargos = new LinkedQueue<Cargo*>;	//List of delivered special cargos to count 

	userInterface = new UI(this); //for printing info on screen and getting user input

	WaitingNormalTrucks = new LinkedQueue<Truck*>;	//keeping track of empty normal trucks
	WaitingSpecialTrucks = new LinkedQueue<Truck*>; //keeping track of empty special trucks
	WaitingVIPTrucks = new LinkedQueue<Truck*>;	//keeping track of empty vip trucks

	LoadingTrucks = new LinkedPriorityQueue<Truck*>;

	Events = new LinkedQueue<Event*>;	//creating a queue for events according to time 

	MovingTrucks = new LinkedPriorityQueue<Truck*>; //creating a queue for moving trucks

	inCheckUpNormalTrucks = new LinkedQueue<Truck*>; //keeping track of normal trucks in maintenence
	inCheckUpVIPTrucks = new LinkedQueue<Truck*>; //keeping track of special trucks in maintenence	
	inCheckUpSpecialTrucks = new LinkedQueue<Truck*>; //keeping track of vip trucks in maintenence

	statisticsQueue = new LinkedPriorityQueue<Cargo*>;

	isLoadingNormalCargo = false;
	isLoadingSpecialCargo = false;
	isLoadingVIPCargo = false;

	TotalWaitTime = 0;
	totalAutoPromoted = 0;
	TotalActiveTime = 0;
	totalUtilization = 0;
}

Company::~Company() //destructor
{
	Cargo* tempCargo = nullptr;
	Truck* tempTruck = nullptr;
	delete WaitingNormalCargos;
	delete WaitingVIPCargos;
	delete WaitingSpecialCargos;
	while (DeliveredNormalCargos->dequeue(tempCargo))
	{
		delete tempCargo;
		tempCargo = nullptr;
	}
	delete DeliveredNormalCargos;
	while (DeliveredSpecialCargos->dequeue(tempCargo))
	{
		delete tempCargo;
		tempCargo = nullptr;
	}
	delete DeliveredSpecialCargos;
	while (DeliveredVIPCargos->dequeue(tempCargo))
	{
		delete tempCargo;
		tempCargo = nullptr;
	}
	delete DeliveredVIPCargos;
	delete userInterface;
	while (WaitingNormalTrucks->dequeue(tempTruck))
	{
		delete tempTruck;
		tempTruck = nullptr;
	}
	delete WaitingNormalTrucks;
	while (WaitingSpecialTrucks->dequeue(tempTruck))
	{
		delete tempTruck;
		tempTruck = nullptr;
	}
	delete WaitingSpecialTrucks;
	while (WaitingVIPTrucks->dequeue(tempTruck))
	{
		delete tempTruck;
		tempTruck = nullptr;
	}
	delete WaitingVIPTrucks;
	delete Events;

	delete MovingTrucks;
	delete inCheckUpNormalTrucks;
	delete inCheckUpVIPTrucks;
	delete inCheckUpSpecialTrucks;
	delete LoadingTrucks;
	delete statisticsQueue;
}

NormalCargoList* Company::getWaitingNormalCargos() //count of waiting normal cargos
{
	return WaitingNormalCargos;
}

LinkedPriorityQueue<Cargo*>* Company::getWaitingVIPCargos() //count of waiting vip cargos
{
	return WaitingVIPCargos;
}

LinkedQueue<Cargo*>* Company::getWaitingSpecialCargos()	//count of waiting special cargos
{
	return WaitingSpecialCargos;
}

LinkedQueue<Cargo*>* Company::getDeliveredNormalCargos() //count of delivered normal cargos
{
	return DeliveredNormalCargos;
}

LinkedQueue<Cargo*>* Company::getDeliveredVIPCargos() //count of delivered vip cargos
{
	return DeliveredVIPCargos;
}

LinkedQueue<Cargo*>* Company::getDeliveredSpecialCargos()	//count of delivered special cargos
{
	return DeliveredSpecialCargos;
}

LinkedQueue<Truck*>* Company::getWaitingSpecialTrucks()  //count of empty special trucks
{
	return WaitingSpecialTrucks;
}

LinkedQueue<Truck*>* Company::getWaitingVIPTrucks()	//count of empty vip trucks
{
	return WaitingVIPTrucks;
}

LinkedQueue<Truck*>* Company::getWaitingNormalTrucks()	//count of empty normal trucks
{
	return WaitingNormalTrucks;
}

LinkedQueue<Truck*>* Company::getInCheckUpNormalTrucks()	//count of in checkup normal trucks
{
	return inCheckUpNormalTrucks;
}

LinkedQueue<Truck*>* Company::getInCheckUpVIPTrucks()	//count of in checkup vip trucks
{
	return inCheckUpVIPTrucks;
}

LinkedQueue<Truck*>* Company::getInCheckUpSpecialTrucks()	//count of in checkup special trucks
{
	return inCheckUpSpecialTrucks;
}

Time Company::getCurrentTime()	//getting current time step to check for events happening
{
	return currentTime;
}


LinkedPriorityQueue <Truck*>* Company::getMovingTrucks() //getting queue of moving trucks
{
	return MovingTrucks;
}

LinkedPriorityQueue<Truck*>* Company::getLoadingTrucks()	//getting queue of Loading trucks
{
	return LoadingTrucks;
}

void Company::readTruckInfo()  //reading truck info from file
{
	int numNormal, numSpecial, numVIP;	//number of trucks of each type
	int nSpeed, sSpeed, vSpeed;		//speed of trucks of each type
	int normalCap, specialCap, vipCap;	//capacity of trucks of each type
	int normalCheckUpDuration, specialCheckUpDuration, vipCheckUpDuration, journeysBeforeCheckUp;	//checkup durations & number of trips before checkup
	inputFile >> numNormal >> numSpecial >> numVIP;  //reading data from file and initializing
	inputFile >> nSpeed >> sSpeed >> vSpeed;
	inputFile >> normalCap >> specialCap >> vipCap;
	inputFile >> journeysBeforeCheckUp;
	inputFile >> normalCheckUpDuration >> specialCheckUpDuration >> vipCheckUpDuration;
	Truck::setJourneysBeforeCheckUp(journeysBeforeCheckUp);
	Truck* newTruckPtr;

	for (int i = 0; i < numNormal; i++)  //loop with number of normal trucks to create each one and enqueue in its suitable queue
	{
		Type truckType = Normal;
		Truck* newTruckPtr = new Truck(truckType, nSpeed, normalCap, normalCheckUpDuration);
		WaitingNormalTrucks->enqueue(newTruckPtr);
	}

	for (int i = 0; i < numSpecial; i++)	//loop with number of special trucks to create each one and enqueue in its suitable queue
	{
		Type truckType = Special;
		Truck* newTruckPtr = new Truck(truckType, sSpeed, specialCap, specialCheckUpDuration);
		WaitingSpecialTrucks->enqueue(newTruckPtr);
	}

	for (int i = 0; i < numVIP; i++)	//loop with number of vip trucks to create each one and enqueue in its suitable queue
	{
		Type truckType = VIP;
		Truck* newTruckPtr = new Truck(truckType, vSpeed, vipCap, vipCheckUpDuration);
		WaitingVIPTrucks->enqueue(newTruckPtr);
	}
}

void Company::readCargoInfo()	//reading cargo info from file
{
	int autoP, maxW;	//auto promotion and max wait times
	inputFile >> autoP >> maxW;
	Cargo::setAutoPromotion(autoP);
	Cargo::setMaxWaitTime(maxW);
}

void  Company::readEvent()	//reading event list from file
{
	int numOfEvents;  //number of events
	char event_type;  //each event type
	Event* evnPtr;
	inputFile >> numOfEvents;
	for (int i = 0; i < numOfEvents; i++) //loop on number of events to create each one individually and enqueue in event list
	{
		inputFile >> event_type;
		switch (event_type)	//switch case for event type to call appropriate function
		{
		case 'R':
			evnPtr = new ReadyEvent(this);
			evnPtr->ReadEvent(inputFile);	//ready event created
			break;
		case 'X':
			evnPtr = new CancellationEvent(this);
			evnPtr->ReadEvent(inputFile);	//cancellation event created
			break;
		case 'P':
			evnPtr = new PromotionEvent(this);
			evnPtr->ReadEvent(inputFile);	//promotion event created
			break;
		}
		Events->enqueue(evnPtr);	//event enqueued in event list

	}


}
void Company::readInputFile()	//reading input file from device
{
	userInterface->printMessage("Enter your file name");
	FileName = userInterface->getUserInput();	//recieving file name from user
	inputFile.open(FileName + ".txt");
	readTruckInfo(); //readng trucks' info 
	readCargoInfo();	//reading cargos' info
	readEvent();	//reading list of events
}

void Company::Simulate() //simulation function for phase 1
{
	modeOfOperation = userInterface->getModeOfOperation(); //getting mode of operation (interactive-step by step-silent)
	readInputFile();
	while (!inputFile.is_open()) //check if file name exists
	{
		userInterface->printMessage("Error! File not found, please enter valid file name"); //if not, repeat process untill valid file entered
		readInputFile();
	}
	CreateOutputFile(); //creating output file


	Event* eventPtr;

	while (!Events->isEmpty()) //checking if there is still an event to be executed
	{
		if (Events->peek(eventPtr))
		{
			while (eventPtr->getTime() == currentTime) //checking if time of event in front of queue arrived
			{
				Events->dequeue(eventPtr); //if yes, dequeue event and execute
				eventPtr->Execute();
				delete eventPtr; //deleting executed event
				eventPtr = nullptr;
				if (!Events->peek(eventPtr))
					break;
			}
			MoveTrucksToMoving(); //moving trucks from loading to moving (if exists and appropriate) 
			AssignCargo();	//start assigning cargos to trucks (if available and adheres to loading rule)
			deliveryProcess(); //keeping track of delivery process
			if (modeOfOperation == 1) //checking chosen mode of operation to print appropriately
			{
				userInterface->printInfo(); //printing updated info if there is any actions occured during timestep
				userInterface->printMessage("Press 'Enter' to continue:");
				userInterface->getUserInput();

			}
			else if (modeOfOperation == 2)
			{
				userInterface->printInfo();
				userInterface->printMessage("");
				Sleep(1000);
			}
			EndHour();
		}
	}
	while (!WaitingVIPCargos->isEmpty() || !WaitingSpecialCargos->isEmpty() || !WaitingNormalCargos->isEmpty() || !MovingTrucks->isEmpty() || !LoadingTrucks->isEmpty() || !inCheckUpNormalTrucks->isEmpty() || !inCheckUpVIPTrucks->isEmpty() || !inCheckUpSpecialTrucks->isEmpty()) //after all events execution, incrementing timestep 
	{
		//untill all cargos are out of waiting lists and loaded and delivered

		MoveTrucksToMoving();	//checking if there are trucks in loading that can be moved to moving
		AssignCargo();	//assign remaining cargos if appropriate
		deliveryProcess(); //keeping track of delivery process
		if (modeOfOperation == 1) //checking chosen mode of operation to print appropriately
		{
			userInterface->printInfo();
			userInterface->printMessage("Press 'Enter' to continue:");
			userInterface->getUserInput();
		}
		else if (modeOfOperation == 2)
		{
			userInterface->printInfo();
			userInterface->printMessage("");
			Sleep(1000);
		}
		EndHour();
	}

	if (modeOfOperation != 3)
	{
		userInterface->printInfo();
	}

	CalculateStatistics(currentTime); //calculating statistics at end of simulation 

	userInterface->printMessage("");
	if (modeOfOperation == 3)
	{
		userInterface->printMessage("Silent Mode");
		userInterface->printMessage("Simulation Starts...");
	}
	userInterface->printMessage("Simulation ends, output file created");
}

void Company::EndHour()  //incrementing waiting time of cargos
{
	int Nsize;
	int Ssize;
	int Vsize;
	Nsize = WaitingNormalCargos->getCount();
	Ssize = WaitingSpecialCargos->getCount();
	Vsize = WaitingVIPCargos->getCount();
	Cargo* retrievedCargo;
	Truck* tempTruck;

	currentTime = currentTime + 1;
	for (int i = 0; i < Nsize; i++)
	{
		WaitingNormalCargos->RemoveFirst(retrievedCargo); //removing first waiting normal cargo to be put in delivered (for phase 1, it will be 1st cargo to be assigned to truck in ph2)
		retrievedCargo->incrementWaitTime();
		if ((retrievedCargo->getWaitTime() / 24) == retrievedCargo->getAutoPromotion()) //auto promoting cargo if time exceeded auto promotion time
		{
			retrievedCargo->promote(); //promoting cargo, changing its type and enqueue in VIP cargos queue
			retrievedCargo->setType(VIP);
			WaitingVIPCargos->enqueue(retrievedCargo, retrievedCargo->calculatePriority());
		}
		else
		{
			if (retrievedCargo->getWaitTime() >= retrievedCargo->getMaxWaitTime()) //checking if cargo reached it max wait time and changing its status
				retrievedCargo->ChangeMaxWait();

			WaitingNormalCargos->InsertEnd(retrievedCargo); //reinserting cargo in normal list if not auto-promoted
		}
	}

	for (int i = 0; i < Ssize; i++)
	{
		WaitingSpecialCargos->dequeue(retrievedCargo); //incrementing wait time of waiting cargos
		retrievedCargo->incrementWaitTime();

		if (retrievedCargo->getWaitTime() >= retrievedCargo->getMaxWaitTime())
			retrievedCargo->ChangeMaxWait();

		WaitingSpecialCargos->enqueue(retrievedCargo);
	}

	LinkedQueue<Cargo*> temp;
	for (int i = 0; i < Vsize; i++)
	{
		WaitingVIPCargos->dequeue(retrievedCargo);  ////incrementing wait time of waiting cargos
		retrievedCargo->incrementWaitTime();
		temp.enqueue(retrievedCargo);
	}

	while (!temp.isEmpty())
	{
		temp.dequeue(retrievedCargo);
		WaitingVIPCargos->enqueue(retrievedCargo, retrievedCargo->calculatePriority());
	}

	if (!inCheckUpNormalTrucks->isEmpty()) //checking trucks in check-up queue 
	{

		//returning truck in waiting queue if it finished
		inCheckUpNormalTrucks->peek(tempTruck);
		if (tempTruck->getCheckUpTime() == currentTime)
		{
			inCheckUpNormalTrucks->dequeue(tempTruck);
			WaitingNormalTrucks->enqueue(tempTruck);
		}
	}

	if (!inCheckUpVIPTrucks->isEmpty())
	{
		inCheckUpVIPTrucks->peek(tempTruck);
		if (tempTruck->getCheckUpTime() == currentTime)
		{
			inCheckUpVIPTrucks->dequeue(tempTruck);
			WaitingVIPTrucks->enqueue(tempTruck);
		}
	}

	if (!inCheckUpSpecialTrucks->isEmpty())
	{
		inCheckUpSpecialTrucks->peek(tempTruck);
		if (tempTruck->getCheckUpTime() == currentTime)
		{
			inCheckUpSpecialTrucks->dequeue(tempTruck);
			WaitingSpecialTrucks->enqueue(tempTruck);
		}
	}
}

void Company::AssignCargo() //assignment process of all cargos in order: VIP,Special,Normal
{
	if (currentTime.getHour() >= 5 && currentTime.getHour() <= 23)
	{
		AssignVIPCargo();
		AssignSpecialCargo();
		AssignNormalCargo();
	}
}

void Company::AssignVIPCargo() //assignment process of VIP cargo
{

	Cargo* Temp = nullptr;
	Truck* tempTruck = nullptr;
	WaitingVIPCargos->peek(Temp);
	if (Temp) //in case of existing waiting VIP cargos
	{
		int WaitingVIPCargoCount = WaitingVIPCargos->getCount();
		int waitVIPTrucks = WaitingVIPTrucks->getCount();
		int waitNormalTrucks = WaitingNormalTrucks->getCount();
		int waitSpecialTrucks = WaitingSpecialTrucks->getCount();
		if (waitVIPTrucks && !isLoadingVIPCargo) //checking availability of waiting VIP trucks and if there's a truck loading VIP cargos
		{
			WaitingVIPTrucks->peek(tempTruck);
			int VIPTruckCapacity = tempTruck->getCapacity();
			if (WaitingVIPCargoCount >= VIPTruckCapacity) //starts loading if waiting cargos => truck capacity
			{
				int totalCargosLoaded = 0;
				WaitingVIPTrucks->dequeue(tempTruck);
				while (VIPTruckCapacity)
				{
					WaitingVIPCargos->dequeue(Temp);
					Temp->setDeliveryTruckID(tempTruck->getID()); //setting Truck ID inside each cargo for delivery statistics
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance())); //enqueue cargo inside queue in truck according to distance to sort
					totalCargosLoaded++;
					VIPTruckCapacity--;
				}

				tempTruck->setMovingTime(currentTime);
				isLoadingVIPCargo = true;
				tempTruck->setStartLoadTime(currentTime); //for calculating active time
				tempTruck->setTCargosDelivered(totalCargosLoaded); //for utilization equation
				LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger())); //enqueue truck inside loading queue according to moving time
			}
		}

		else if (waitNormalTrucks && !isLoadingVIPCargo) //if no VIP trucks are available, checking normal trucks with same process as above
		{
			WaitingNormalTrucks->peek(tempTruck);
			int NormalTruckCapacity = tempTruck->getCapacity();
			if (WaitingVIPCargoCount >= NormalTruckCapacity)
			{
				int totalCargosLoaded = 0;
				WaitingNormalTrucks->dequeue(tempTruck);
				while (NormalTruckCapacity)
				{
					WaitingVIPCargos->dequeue(Temp);
					Temp->setDeliveryTruckID(tempTruck->getID());
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
					totalCargosLoaded++;
					NormalTruckCapacity--;
				}
				tempTruck->setMovingTime(currentTime);
				isLoadingVIPCargo = true;
				tempTruck->setStartLoadTime(currentTime);
				tempTruck->setTCargosDelivered(totalCargosLoaded);
				LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
			}
		}
		else if (waitSpecialTrucks && !isLoadingVIPCargo) //if no VIP trucks are available, checking Special trucks with same process as above
		{
			WaitingSpecialTrucks->peek(tempTruck);
			int SpecialTruckCapacity = tempTruck->getCapacity();
			if (WaitingVIPCargoCount >= SpecialTruckCapacity)
			{
				int totalCargosLoaded = 0;
				WaitingSpecialTrucks->dequeue(tempTruck);
				while (SpecialTruckCapacity)
				{
					WaitingVIPCargos->dequeue(Temp);
					Temp->setDeliveryTruckID(tempTruck->getID());
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
					totalCargosLoaded++;
					SpecialTruckCapacity--;
				}
				tempTruck->setMovingTime(currentTime);
				isLoadingVIPCargo = true;
				tempTruck->setStartLoadTime(currentTime);
				tempTruck->setTCargosDelivered(totalCargosLoaded);
				LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
			}
		}
	}
}




void Company::AssignSpecialCargo() //assignment process of Special cargo
{
	Cargo* Temp = nullptr;
	Truck* tempTruck = nullptr;
	WaitingSpecialCargos->peek(Temp);
	if (Temp)//in case of existing waiting Special cargos
	{

		int WaitingSpecialCargoCount = WaitingSpecialCargos->getCount();
		if (!WaitingSpecialTrucks->isEmpty())  //checking availability of waiting Special trucks and if there's a truck loading Special cargos
		{
			WaitingSpecialTrucks->peek(tempTruck);
			int SpecialTruckCapacity = tempTruck->getCapacity();
			if (!isLoadingSpecialCargo)
			{
				if (WaitingSpecialCargoCount >= SpecialTruckCapacity) //starts loading if waiting cargos => truck capacity
				{
					int totalCargosLoaded = 0;
					WaitingSpecialTrucks->dequeue(tempTruck);
					while (SpecialTruckCapacity)
					{
						WaitingSpecialCargos->dequeue(Temp);
						Temp->setDeliveryTruckID(tempTruck->getID()); //setting Truck ID inside each cargo for delivery statistics
						tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));//enqueue cargo inside queue in truck according to distance to sort
						totalCargosLoaded++;
						SpecialTruckCapacity--;
					}
					tempTruck->setMovingTime(currentTime);
					isLoadingSpecialCargo = true;
					tempTruck->setStartLoadTime(currentTime);//for calculating active time
					tempTruck->setTCargosDelivered(totalCargosLoaded);//for utilization equation
					LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));//enqueue truck inside loading queue according to moving time
				}
			}
		}

		if (!WaitingSpecialCargos->isEmpty()) //checking if there's still waiting cargos inside queue
		{
			WaitingSpecialCargos->peek(Temp);
			if (Temp->checkMaxWait()) //checking if cargos reached max wait time
			{
				if (!WaitingSpecialTrucks->isEmpty()) //if there's any special trucks available,starts loading
					WaitingSpecialTrucks->dequeue(tempTruck);
				else
					return;

				int SpecialTruckCapacity = tempTruck->getCapacity();
				int totalCargosLoaded = 0;
				while (SpecialTruckCapacity && WaitingSpecialCargoCount) //starts loading max wait cargos and others if available
				{
					WaitingSpecialCargos->dequeue(Temp);
					Temp->setDeliveryTruckID(tempTruck->getID());
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
					SpecialTruckCapacity--;
					WaitingSpecialCargoCount--;
					totalCargosLoaded++;
				}
				tempTruck->setMovingTime(currentTime);
				tempTruck->changeIsCarryingMaxWait(true);
				tempTruck->setStartLoadTime(currentTime);
				tempTruck->setTCargosDelivered(totalCargosLoaded);
				LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
			}
		}

	}
}

void Company::AssignNormalCargo()
{
	Cargo* Temp = nullptr;
	Truck* tempTruck = nullptr;
	if (!WaitingNormalCargos->isEmpty())
	{
		int WaitingNormalCargoCount = WaitingNormalCargos->getCount();
		if (!WaitingNormalTrucks->isEmpty()) //checks for normal trucks
		{
			WaitingNormalTrucks->peek(tempTruck);
			int NormalTruckCapacity = tempTruck->getCapacity();
			if (!isLoadingNormalCargo) //checks that there is no normal cargo that is loading
			{
				if (WaitingNormalCargoCount >= NormalTruckCapacity) //handling loading rule
				{
					int totalCargosLoaded = 0;
					WaitingNormalTrucks->dequeue(tempTruck);
					while (NormalTruckCapacity) //adding cargo to truck
					{
						WaitingNormalCargos->RemoveFirst(Temp);
						Temp->setDeliveryTruckID(tempTruck->getID());
						tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
						NormalTruckCapacity--;
						totalCargosLoaded++;
					}

					tempTruck->setMovingTime(currentTime); //sets moving time for truck
					isLoadingNormalCargo = true; //turns on the flag in order not to load any more normal cargos
					tempTruck->setStartLoadTime(currentTime); //used later in statistics
					tempTruck->setTCargosDelivered(totalCargosLoaded); 
					LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger())); //enqueue by moving time in loading trucks
				}
			}

			if (!WaitingNormalCargos->isEmpty()) //condition for max wait
			{

				WaitingNormalCargos->RemoveFirst(Temp);
				if (Temp->checkMaxWait()) //checks for max wait
				{
					if (!WaitingNormalTrucks->isEmpty()) // to choose which truck to enqueue in
						WaitingNormalTrucks->dequeue(tempTruck);
					else if (!WaitingVIPTrucks->isEmpty())
						WaitingVIPTrucks->dequeue(tempTruck);
					else
						return;
					int totalCargosLoaded = 0;
					Temp->setDeliveryTruckID(tempTruck->getID());
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance())); //enqueues the first cargo then the rest
					totalCargosLoaded++;
					NormalTruckCapacity--;
					WaitingNormalCargoCount--;
					while (NormalTruckCapacity && WaitingNormalCargoCount) //Not only taking the max wait cargo but taking any more cargos if exist
					{
						WaitingNormalCargos->RemoveFirst(Temp);
						Temp->setDeliveryTruckID(tempTruck->getID());
						tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
						NormalTruckCapacity--;
						WaitingNormalCargoCount--;
						totalCargosLoaded++;

					}
					tempTruck->setMovingTime(currentTime); //same as above
					tempTruck->changeIsCarryingMaxWait(true);
					tempTruck->setStartLoadTime(currentTime);
					tempTruck->setTCargosDelivered(totalCargosLoaded);
					LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
				}
				else //if it is not max wait
					WaitingNormalCargos->InsertBeg(Temp);
			}

		}
		else if (!WaitingVIPTrucks->isEmpty()) //Normal assignment whether there are no normal trucks to enqueue in VIP
		{

			WaitingVIPTrucks->peek(tempTruck);
			int VIPTruckCapacity = tempTruck->getCapacity();
			if (!isLoadingNormalCargo) //checks if there are no normal cargos loading
			{
				if (WaitingNormalCargoCount >= VIPTruckCapacity) //handling loading conditions
				{
					int totalCargosLoaded = 0;
					WaitingVIPTrucks->dequeue(tempTruck);
					while (VIPTruckCapacity) //enqueue process
					{
						WaitingNormalCargos->RemoveFirst(Temp);
						Temp->setDeliveryTruckID(tempTruck->getID());
						tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
						VIPTruckCapacity--;
						totalCargosLoaded++;
					}

					tempTruck->setMovingTime(currentTime); //same as above
					isLoadingNormalCargo = true;
					tempTruck->setStartLoadTime(currentTime);
					tempTruck->setTCargosDelivered(totalCargosLoaded);
					LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
				}

			}
			if (!WaitingNormalCargos->isEmpty()) //checks for max wait time
			{

				WaitingNormalCargos->RemoveFirst(Temp);
				if (Temp->checkMaxWait()) //this time it enqueues only in VIP as there are no more normal trucks available and the rest is the same as above
				{

					if (!WaitingVIPTrucks->isEmpty())
						WaitingVIPTrucks->dequeue(tempTruck);
					else
						return;
					int totalCargosLoaded = 0;
					Temp->setDeliveryTruckID(tempTruck->getID());
					tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
					totalCargosLoaded++;
					VIPTruckCapacity--;
					WaitingNormalCargoCount--;
					while (VIPTruckCapacity && WaitingNormalCargoCount)
					{
						WaitingNormalCargos->RemoveFirst(Temp);
						Temp->setDeliveryTruckID(tempTruck->getID());
						tempTruck->getLoadedCargos()->enqueue(Temp, (1000.0 / Temp->getDeliveryDistance()));
						VIPTruckCapacity--;
						WaitingNormalCargoCount--;
						totalCargosLoaded++;

					}
					tempTruck->setMovingTime(currentTime); //same as above
					tempTruck->changeIsCarryingMaxWait(true);
					tempTruck->setStartLoadTime(currentTime);
					tempTruck->setTCargosDelivered(totalCargosLoaded);
					LoadingTrucks->enqueue(tempTruck, -(tempTruck->getMovingTime().timeToInteger()));
				}
				else
					WaitingNormalCargos->InsertBeg(Temp);
			}
		}
	}
}

void Company::CreateOutputFile() //creates output file
{
	outputFile.open(FileName + "Output.txt");
	outputFile << "CDT" << "\t" << "ID" << "\t" << "PT" << "\t" << "WT" << "\t" << "TID" << endl;

}


void Company::CalculateStatistics(Time Tsim)
{
	Cargo* tempCargo;
	Truck* tempTruck;
	int NormalCargos = DeliveredNormalCargos->getCount(); //Gets count of all cargos of every type
	int SpecialCargos = DeliveredSpecialCargos->getCount();
	int VIPCargos = DeliveredVIPCargos->getCount();

	int NormalTrucks = WaitingNormalTrucks->getCount() + inCheckUpNormalTrucks->getCount(); //gets count of all trucks
	int SpecialTrucks = WaitingSpecialTrucks->getCount() + inCheckUpSpecialTrucks->getCount();
	int VIPTrucks = WaitingVIPTrucks->getCount() + inCheckUpVIPTrucks->getCount();

	while (!WaitingNormalTrucks->isEmpty()) //getting statistics of normal trucks
	{
		WaitingNormalTrucks->dequeue(tempTruck);
		TotalActiveTime += tempTruck->getActiveTime();
		totalUtilization += tempTruck->TruckUtilization(Tsim);
	}

	while (!WaitingVIPTrucks->isEmpty()) //getting statistics of VIP trucks
	{
		WaitingVIPTrucks->dequeue(tempTruck);
		TotalActiveTime += tempTruck->getActiveTime();
		totalUtilization += tempTruck->TruckUtilization(Tsim);
	}

	while (!WaitingSpecialTrucks->isEmpty()) //getting statistics of Special trucks
	{
		WaitingSpecialTrucks->dequeue(tempTruck);
		TotalActiveTime += tempTruck->getActiveTime();
		totalUtilization += tempTruck->TruckUtilization(Tsim);
	}

	while (!DeliveredNormalCargos->isEmpty()) //enqueueing cargos in statistics queue in the order in which they got delivered
	{
		DeliveredNormalCargos->dequeue(tempCargo);
		statisticsQueue->enqueue(tempCargo, -(tempCargo->getDeliveryTime().timeToInteger()));

	}
	while (!DeliveredSpecialCargos->isEmpty())
	{
		DeliveredSpecialCargos->dequeue(tempCargo);
		statisticsQueue->enqueue(tempCargo, -(tempCargo->getDeliveryTime().timeToInteger()));

	}
	while (!DeliveredVIPCargos->isEmpty())
	{
		DeliveredVIPCargos->dequeue(tempCargo);
		statisticsQueue->enqueue(tempCargo, -(tempCargo->getDeliveryTime().timeToInteger()));

	}
	while (!statisticsQueue->isEmpty()) //Emptying statistics queue and calculating more statistics and printing in output file
	{
		statisticsQueue->dequeue(tempCargo); //checks for autoP
		if (tempCargo->getPromotion())
		{
			totalAutoPromoted++;
		}
		TotalWaitTime += tempCargo->getWaitTime(); //printing Delivery time
		outputFile << tempCargo->getDeliveryTime().getDay() << ":" << tempCargo->getDeliveryTime().getHour() << "\t";
		outputFile << tempCargo->getID() << "\t" << tempCargo->getReadyTime().getDay() << ":" << tempCargo->getReadyTime().getHour() << "\t"; //printing ID and preparation time
		Time temp;
		temp.integerToTime(tempCargo->getWaitTime()); //printing wait time and Truck ID
		outputFile << temp.getDay() << ":" << temp.getHour() << "\t" << tempCargo->getDeliveryTruckID() << endl;
	}


	int avgWT = TotalWaitTime / (NormalCargos + SpecialCargos + VIPCargos);
	Time AvgWT;
	AvgWT.integerToTime(avgWT);

	outputFile << "-------------------------------------------------" << endl;
	outputFile << "-------------------------------------------------" << endl;

	outputFile << "Cargos: " << (NormalCargos + SpecialCargos + VIPCargos) << " [N: " << NormalCargos //printing number of cargos
		<< ", S: " << SpecialCargos << ", V: " << VIPCargos << "]" << endl;
	outputFile << "Cargo Avg Wait = " << AvgWT.getDay() << ":" << AvgWT.getHour() << endl; //printing average wait
	outputFile << "Auto-promoted Cargos: " << (totalAutoPromoted / (NormalCargos + totalAutoPromoted)) * 100 << "%" << endl; //printing autoP percentage
	outputFile << "Trucks: " << (NormalTrucks + SpecialTrucks + VIPTrucks) << " [N: " << NormalTrucks << ", S: "//printing trucks
		<< SpecialTrucks << ", V: " << VIPTrucks << "]" << endl;
	outputFile << "Average Active Time = " << (TotalActiveTime / (Tsim.timeToInteger() * (NormalTrucks + SpecialTrucks + VIPTrucks))) * 100 << "%" << endl; //printing average wait time
	outputFile << "Average Utilization = " << (totalUtilization / (NormalTrucks + SpecialTrucks + VIPTrucks)) * 100 << "%" << endl; //printing average utilization



}
void Company::MoveTrucksToMoving()
{
	Truck* tempTruck = nullptr;
	Cargo* tempCargo = nullptr;
	LoadingTrucks->peek(tempTruck); 
	while (tempTruck && tempTruck->getMovingTime() == currentTime) //checks whether it is the time move any moving trucks
	{
		tempTruck->getLoadedCargos()->peek(tempCargo); //checks if there are any more loading cargos
		if (!tempTruck->isCarryingMaxWait()) //returns the loading condition to false so that other cargos start to load
		{
			switch (tempCargo->getType())
			{
			case Normal: isLoadingNormalCargo = false;
				break;
			case VIP: isLoadingVIPCargo = false;
				break;
			case Special: isLoadingSpecialCargo = false;
				break;
			}
		}
		LoadingTrucks->dequeue(tempTruck);
		tempTruck->setArrivalTime(); //sets arrival time of truck
		tempTruck->changeIsCarryingMaxWait(false); //checks the condition of carrying max wait to false as we already benefitied from it and don't need it any more
		MovingTrucks->enqueue(tempTruck, tempTruck->calculateCargoDelivery(currentTime)); //enqueues in moving trucks in the order of first delivery
		tempTruck = nullptr;
		LoadingTrucks->peek(tempTruck); //checks if there are any more trucks
	}
}

void Company::deliveryProcess()
{
	Truck* tempTruck = nullptr;
	Cargo* tempCargo = nullptr;
	if (!MovingTrucks->isEmpty()) //checks if the queue is empty
	{
		MovingTrucks->peek(tempTruck);
		tempTruck->getLoadedCargos()->peek(tempCargo);


		while ((tempTruck && tempTruck->getArrivalTime() == currentTime) || (tempCargo && tempCargo->getDeliveryTime() == currentTime)) //checks if there are any trucks delivering currently or arriving
		{
			if (tempCargo && tempCargo->getDeliveryTime() == currentTime) //if the cargo is being delivered currently
			{
				MovingTrucks->dequeue(tempTruck);
				tempTruck->getLoadedCargos()->dequeue(tempCargo);
				switch (tempCargo->getType()) //checks where to deliver the cargo
				{
				case Normal:
					tempCargo->setDeliveryTime(currentTime);
					DeliveredNormalCargos->enqueue(tempCargo);
					break;
				case VIP:
					tempCargo->setDeliveryTime(currentTime);
					DeliveredVIPCargos->enqueue(tempCargo);
					break;
				case Special:
					tempCargo->setDeliveryTime(currentTime);
					DeliveredSpecialCargos->enqueue(tempCargo);
					break;
				}
				if (!tempTruck->getLoadedCargos()->isEmpty()) //checks if there are any more cargos in order to enqueue according to the arriving time of the cargo
				{
					MovingTrucks->enqueue(tempTruck, tempTruck->calculateCargoDelivery(currentTime));

				}
				else //enqueues according to arrival time if the truck has delivered all of its cargos
				{
					tempTruck->setActiveTime(currentTime); //sets active time of the truck
					MovingTrucks->enqueue(tempTruck, -(tempTruck->getArrivalTime().timeToInteger()));
				}
			}
			else if (tempTruck && tempTruck->getArrivalTime() == currentTime) //checks whether the truck is arriving
			{

				tempTruck->incrementJournies(); //incrementing number of journeys taken by the truck
				if (tempTruck->getDeliveries() == tempTruck->getJourneysBeforeCheckup()) //checks whether the truck is supposed to enter in checkup
				{
					MovingTrucks->dequeue(tempTruck);
					tempTruck->setCheckUpTime(currentTime); //checks when the truck is supposed to leave the checkup
					tempTruck->resetDeliveries(); //resets number of deliveries to zero for future times to enter in check up
					switch (tempTruck->getType()) //checks which queue to enter
					{
					case Normal:

						inCheckUpNormalTrucks->enqueue(tempTruck);
						break;
					case VIP:

						inCheckUpVIPTrucks->enqueue(tempTruck);
						break;
					case Special:

						inCheckUpSpecialTrucks->enqueue(tempTruck);
						break;
					}
				}
				else //the truck is re-entering the waiting queue
				{
					MovingTrucks->dequeue(tempTruck);
					switch (tempTruck->getType()) //checks which waiting queue to enter
					{
					case Normal:

						WaitingNormalTrucks->enqueue(tempTruck);
						break;
					case VIP:

						WaitingVIPTrucks->enqueue(tempTruck);
						break;
					case Special:

						WaitingSpecialTrucks->enqueue(tempTruck);
						break;
					}
				}
			}
			if (MovingTrucks->peek(tempTruck)) //checks whether there are any more trucks in waiting
			{
				if (!tempTruck->getLoadedCargos()->peek(tempCargo)) //checks for cargos
					tempCargo = nullptr;
			}
			else 
			{
				tempTruck = nullptr;
			}
		}

	}

}