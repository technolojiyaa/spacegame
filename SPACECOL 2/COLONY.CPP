// ================================================================
// COLONY.CPP - the colony's resources (encapsulation)
// ================================================================
#include "COLONY.H"

// Constructor: the colony has just crashed, so nothing is at 100%.
Colony::Colony()
{
    oxygen = 80;
    energy = 60;
    food = 70;
    materials = 30;
    hull = 70;
    research = 0;
    defense = 10;
}

// Private helper used by every function below.
void Colony::limit(int &value)
{
    if(value < 0)
        value = 0;
    if(value > 100)
        value = 100;
}

void Colony::useOxygen(int amount)    { oxygen = oxygen - amount;       limit(oxygen); }
void Colony::addOxygen(int amount)    { oxygen = oxygen + amount;       limit(oxygen); }

void Colony::useEnergy(int amount)    { energy = energy - amount;       limit(energy); }
void Colony::addEnergy(int amount)    { energy = energy + amount;       limit(energy); }

void Colony::useFood(int amount)      { food = food - amount;           limit(food); }
void Colony::addFood(int amount)      { food = food + amount;           limit(food); }

void Colony::useMaterials(int amount) { materials = materials - amount; limit(materials); }
void Colony::addMaterials(int amount) { materials = materials + amount; limit(materials); }

void Colony::addResearch(int amount)  { research = research + amount;   limit(research); }
void Colony::addDefense(int amount)   { defense = defense + amount;     limit(defense); }

// Repairing the hull costs the same amount of materials.
int Colony::repairHull(int amount)
{
    if(materials < amount)
        return 0;

    materials = materials - amount;
    hull = hull + amount;
    limit(hull);
    return 1;
}

void Colony::damageHull(int amount)
{
    hull = hull - amount;
    limit(hull);
}

// FUNCTION OVERLOADING - version 1: no argument, standard repair
int Colony::repair()
{
    return repairHull(10);
}

// FUNCTION OVERLOADING - version 2: repair by a chosen amount
int Colony::repair(int amount)
{
    return repairHull(amount);
}

int Colony::getOxygen()    { return oxygen; }
int Colony::getEnergy()    { return energy; }
int Colony::getFood()      { return food; }
int Colony::getMaterials() { return materials; }
int Colony::getHull()      { return hull; }
int Colony::getResearch()  { return research; }
int Colony::getDefense()   { return defense; }

int Colony::isColonyDead()
{
    if(oxygen <= 0 || hull <= 0)
        return 1;
    return 0;
}
