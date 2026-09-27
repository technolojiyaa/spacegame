#ifndef COLONY_H
#define COLONY_H

// ================================================================
// COLONY.H
// Demonstrates ENCAPSULATION: all resources are private.
// The only way to change them is through the public functions,
// which always keep every value between 0 and 100.
// ================================================================

class Colony
{
private:
    int oxygen;
    int energy;
    int food;
    int materials;
    int hull;
    int research;
    int defense;

    void limit(int &value);          // private helper: keeps value in 0..100

public:
    Colony();                         // constructor: values right after the crash

    void useOxygen(int amount);
    void addOxygen(int amount);

    void useEnergy(int amount);
    void addEnergy(int amount);

    void useFood(int amount);
    void addFood(int amount);

    void useMaterials(int amount);
    void addMaterials(int amount);

    int  repairHull(int amount);      // returns 1 if repaired, 0 if not enough materials
    void damageHull(int amount);

    // FUNCTION OVERLOADING: same name, different parameters
    int repair();                     // small standard repair (+10 hull)
    int repair(int amount);           // repair by any amount

    void addResearch(int amount);
    void addDefense(int amount);

    int getOxygen();
    int getEnergy();
    int getFood();
    int getMaterials();
    int getHull();
    int getResearch();
    int getDefense();

    int isColonyDead();

    // FRIEND CLASS: SaveSystem may read/write the private data to save it
    friend class SaveSystem;
};

#endif
