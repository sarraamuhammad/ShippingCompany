#pragma once
#include "LinkedList.h"
#include "Cargo.h"
class NormalCargoList : public LinkedList<Cargo*>
{
public:
	Cargo* RetrieveCargoFromID(Cargo* inputCargo);
};