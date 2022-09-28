#include "UI.h"
#include <string>
#include<iostream>


UI::UI(Company* pComp)
{
	pCompany = pComp;
}

void UI::printInfo() //printing hourly info on screen
{
	Time currentTime = pCompany->getCurrentTime(); //getting current timestep
	NormalCargoList* NormalCargos = pCompany->getWaitingNormalCargos(); //getting waiting normal cargos
	LinkedPriorityQueue<Cargo*>* VIPCargos = pCompany->getWaitingVIPCargos();	//getting waiting VIP cargos
	LinkedQueue<Cargo*>* SpecialCargos = pCompany->getWaitingSpecialCargos();	//getting waiting special cargos

	LinkedQueue<Cargo*>* DeliveredNormalCargos = pCompany->getDeliveredNormalCargos();	//getting delivered normal cargos
	LinkedQueue<Cargo*>* DeliveredVIPCargos = pCompany->getDeliveredVIPCargos();	//getting delivered VIP cargos
	LinkedQueue<Cargo*>* DeliveredSpecialCargos = pCompany->getDeliveredSpecialCargos();	//getting delivered special cargos

	LinkedQueue<Truck*>* WaitingNormalTrucks = pCompany->getWaitingNormalTrucks();	//getting normal waiting trucks
	LinkedQueue<Truck*>* WaitingVIPTrucks = pCompany->getWaitingVIPTrucks();	//getting vip waiting trucks
	LinkedQueue<Truck*>* WaitingSpecialTrucks = pCompany->getWaitingSpecialTrucks();	//getting special waiting trucks

	LinkedQueue<Truck*>* inCheckUpNormalTrucks = pCompany->getInCheckUpNormalTrucks(); //getting in checkup normal trucks
	LinkedQueue<Truck*>* inCheckUpVIPTrucks = pCompany->getInCheckUpVIPTrucks(); //getting in checkup vip trucks
	LinkedQueue<Truck*>* inCheckUpSpecialTrucks = pCompany->getInCheckUpSpecialTrucks(); //getting in checkup special trucks

	LinkedPriorityQueue<Truck*>* movingTrucks = pCompany->getMovingTrucks(); //getting moving trucks
	LinkedPriorityQueue<Truck*>* loadingTrucks = pCompany->getLoadingTrucks(); //getting loading trucks

	LinkedQueue<Truck*> tempQ; //tempQ to be used in printing moving Cargos
	Truck* tempTruck = nullptr;
	Cargo* tempCargo = nullptr;

	cout << "Current Time (Day:Hour): " << currentTime.getDay() << ":" << currentTime.getHour() << endl; //printing current timestep

	int totalNumberOfCargos = NormalCargos->getCount() + VIPCargos->getCount() + SpecialCargos->getCount();
	cout << totalNumberOfCargos << " Waiting Cargos: ["; NormalCargos->print(); cout << "] ("; //printing number of current waiting cargos and their type
	SpecialCargos->print(); cout << ") {"; VIPCargos->print(); cout << "}" << endl;

	cout << "-----------------------------------" << endl;

	int totalLoadingTrucks = loadingTrucks->getCount();

	cout << totalLoadingTrucks << " Loading Trucks: ";
	loadingTrucks->print();
	cout << endl;

	cout << "-----------------------------------" << endl;

	int totalNumberOfWaitingTrucks = WaitingNormalTrucks->getCount() + WaitingSpecialTrucks->getCount() + WaitingVIPTrucks->getCount();
	cout << totalNumberOfWaitingTrucks << " Empty Trucks: ["; //printing number of empty trucks with their ID and cargos loaded with their type
	WaitingNormalTrucks->print(); cout << "] (";
	WaitingSpecialTrucks->print();
	cout << ") {";
	WaitingVIPTrucks->print();
	cout << "}" << endl;
	//printing number of loading trucks with their ID and cargos loaded with their type

	cout << "-----------------------------------" << endl;

	int totalNumberOfMovingCargos = 0;

	while (!movingTrucks->isEmpty())
	{
		movingTrucks->dequeue(tempTruck);
		totalNumberOfMovingCargos += tempTruck->getLoadedCargos()->getCount();
		tempQ.enqueue(tempTruck);
	}

	while (!tempQ.isEmpty())
	{
		tempQ.dequeue(tempTruck);
		tempTruck->getLoadedCargos()->peek(tempCargo);
		if (tempCargo)
			movingTrucks->enqueue(tempTruck, -( tempCargo->getDeliveryTime().timeToInteger()));
		else
			movingTrucks->enqueue(tempTruck, -(tempTruck->getArrivalTime().timeToInteger()));
		tempCargo = nullptr;
	}

	cout << totalNumberOfMovingCargos << " Moving Cargos: "; //printing number of current moving cargos and their type
	movingTrucks->print();
	cout << endl;
	cout << "-----------------------------------" << endl;

	int totalInCheckupTrucks = inCheckUpNormalTrucks->getCount() + inCheckUpVIPTrucks->getCount() + inCheckUpSpecialTrucks->getCount();
	cout << totalInCheckupTrucks << " In Checkup Trucks: ["; inCheckUpNormalTrucks->print(); cout << "] ("; inCheckUpSpecialTrucks->print();
	cout << ") {"; inCheckUpVIPTrucks->print();cout << "}" << endl;

	cout << "-----------------------------------" << endl;

	int totalNumberOfDeliveredCargos = DeliveredNormalCargos->getCount() + DeliveredVIPCargos->getCount() + DeliveredSpecialCargos->getCount();
	cout << totalNumberOfDeliveredCargos << " Delivered Cargos: {"; DeliveredVIPCargos->print(); cout << "} ["; //printing number of delivered cargos and their type
	DeliveredNormalCargos->print(); cout << "] ("; DeliveredSpecialCargos->print(); cout << ")" << endl;
}

string UI::getUserInput() //getting user input (file name)
{
	string input;
	getline(cin, input);
	return input;
}
int UI::getModeOfOperation() //getting mode of operation to start simulation
{
	
	printMessage("Choose the mode of operation :");
	printMessage("[1] Interactive Mode");
	printMessage("[2] Step by Step");
	printMessage("[3] Silent Mode");
	string chosenOper;
	getline(cin, chosenOper);
	while (chosenOper != "1" && chosenOper != "2" && chosenOper != "3")
	{
		printMessage("Error! Enter a valid number.... Choose the mode of operation :");
		printMessage("[1] Interactive Mode");
		printMessage("[2] Step by Step");
		printMessage("[3] Silent Mode");
		cin >> chosenOper;
	}
	int num = stoi(chosenOper);
	return num;


}
void UI::printMessage(string msg)
{
	cout << msg << endl;

}

ostream& operator << (ostream& out, Cargo* cargoInput) //operator overloading for cout cargo ID
{
	out << cargoInput->getID();
	return out;
}

ostream& operator << (ostream& out, Truck* truckInput) //operator overloading for trucks
{
	out << truckInput->getID();
	Type truckType = truckInput->getType();
	if (!truckInput->getLoadedCargos()->isEmpty())
	{
		switch (truckType)
		{
		case Normal:
			out << "[";
			truckInput->getLoadedCargos()->print();
			out << "]";
			break;

		case VIP:
			out << "{";
			truckInput->getLoadedCargos()->print();
			out << "}";
			break;

		case Special:
			out << "(";
			truckInput->getLoadedCargos()->print();
			out << ")";
			break;
		}
	}
	return out;
}
