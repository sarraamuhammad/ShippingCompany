#pragma once
#include "Event.h"

class Company;

class PromotionEvent :
    public Event
{
    int promoteID;
    int extraCost;

public:

    PromotionEvent(Company* pcomp);

    void ReadEvent(ifstream& inputFile);

    void Execute();
};