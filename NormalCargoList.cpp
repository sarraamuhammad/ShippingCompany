#include "NormalCargoList.h"

Cargo* NormalCargoList::RetrieveCargoFromID(Cargo* inputCargo) //to retrieve cargo needed by its ID
{
	Node<Cargo*>* temp = Head;
	while (temp)
	{
		if (*(temp->getItem()) == *inputCargo) //if found
		{
			Cargo* retrievedCargo = temp->getItem(); //set retrieved with found element
			DeleteNode(temp->getItem()); //deleting cargo from list
			return retrievedCargo; //return found cargo
		}
		temp = temp->getNext();
	}
	return nullptr;
}
