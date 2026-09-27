// ================================================================
// CREW.CPP - crew members and their special abilities
// ================================================================
#include "CREW.H"
#include "COLONY.H"
#include <stdio.h>
#include <string.h>

// ---------------- CrewMember (base class) ----------------
CrewMember::CrewMember()
{
    strcpy(name, "UNKNOWN");
    strcpy(role, "Crew");
    health = 100;
    alive = 1;
}

CrewMember::CrewMember(char n[], char r[])
{
    strcpy(name, n);
    strcpy(role, r);
    health = 100;
    alive = 1;
}

CrewMember::~CrewMember()
{
    // Nothing to free here. Because it is VIRTUAL, deleting a
    // CrewMember* that points to a Doctor also runs ~Doctor().
}

// THIS POINTER: this->health means "the health of THIS object"
void CrewMember::setHealth(int h)
{
    this->health = h;

    if(this->health > 100)
        this->health = 100;

    if(this->health <= 0)
    {
        this->health = 0;
        this->alive = 0;
    }
}

int CrewMember::getHealth()  { return health; }
int CrewMember::isAlive()    { return alive; }
char* CrewMember::getName()  { return name; }
char* CrewMember::getRole()  { return role; }

void CrewMember::damage(int amount)
{
    if(alive == 1)
        setHealth(health - amount);
}

void CrewMember::heal(int amount)
{
    if(alive == 1)
        setHealth(health + amount);
}

void CrewMember::specialAbility(char resultText[])
{
    sprintf(resultText, "%s has no special ability.", name);
}

void CrewMember::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    strcpy(rewardText, "NO EFFECT");
}

// ---------------- Engineer ----------------
Engineer::Engineer(char n[]) : CrewMember(n, "Engineer") { }

void Engineer::specialAbility(char resultText[])
{
    sprintf(resultText, "%s repairs a damaged system.", name);
}

void Engineer::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    if(colony.repair() == 1)                  // overloaded repair() - no argument
        strcpy(rewardText, "HULL +10   MATERIALS -10");
    else
        strcpy(rewardText, "NOT ENOUGH MATERIALS TO REPAIR");
}

// ---------------- Doctor ----------------
Doctor::Doctor(char n[]) : CrewMember(n, "Doctor") { }

void Doctor::specialAbility(char resultText[])
{
    sprintf(resultText, "%s treats every injured crew member.", name);
}

void Doctor::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    int i;
    for(i = 0; i < count; i++)
        team[i]->heal(20);
    strcpy(rewardText, "ALL CREW HEALTH +20");
}

// ---------------- Scientist ----------------
Scientist::Scientist(char n[]) : CrewMember(n, "Scientist") { }

void Scientist::specialAbility(char resultText[])
{
    sprintf(resultText, "%s studies the alien technology.", name);
}

void Scientist::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    colony.addResearch(10);
    strcpy(rewardText, "RESEARCH +10");
}

// ---------------- SecurityOfficer ----------------
SecurityOfficer::SecurityOfficer(char n[]) : CrewMember(n, "Security Officer") { }
SecurityOfficer::SecurityOfficer(char n[], char r[]) : CrewMember(n, r) { }

void SecurityOfficer::specialAbility(char resultText[])
{
    sprintf(resultText, "%s patrols the colony fence.", name);
}

void SecurityOfficer::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    colony.addDefense(15);
    strcpy(rewardText, "DEFENSE +15");
}

// ---------------- EliteSecurityOfficer (multilevel) ----------------
EliteSecurityOfficer::EliteSecurityOfficer(char n[])
    : SecurityOfficer(n, "Elite Security Officer") { }

void EliteSecurityOfficer::specialAbility(char resultText[])
{
    sprintf(resultText, "%s leads a powerful counter-attack.", name);
}

void EliteSecurityOfficer::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    colony.addDefense(25);
    strcpy(rewardText, "DEFENSE +25");
}

// ---------------- Pilot ----------------
Pilot::Pilot(char n[]) : CrewMember(n, "Pilot") { }

void Pilot::specialAbility(char resultText[])
{
    sprintf(resultText, "%s flies a scavenging run over the wreck field.", name);
}

void Pilot::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    colony.addMaterials(15);
    colony.useEnergy(5);
    strcpy(rewardText, "MATERIALS +15   ENERGY -5");
}

// ---------------- RepairAbility ----------------
void RepairAbility::extraRepair(char resultText[])
{
    strcat(resultText, " (Extra repair bonus!)");
}

// ---------------- SpecialEngineer (multiple + hybrid) ----------------
SpecialEngineer::SpecialEngineer(char n[]) : Engineer(n), RepairAbility() { }

void SpecialEngineer::specialAbility(char resultText[])
{
    sprintf(resultText, "%s performs an advanced repair.", name);
    extraRepair(resultText);                  // inherited from RepairAbility
}

void SpecialEngineer::useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[])
{
    if(colony.repair(20) == 1)                // overloaded repair(int)
        strcpy(rewardText, "HULL +20   MATERIALS -20");
    else if(colony.repair() == 1)             // not enough for 20? try 10
        strcpy(rewardText, "HULL +10   MATERIALS -10");
    else
        strcpy(rewardText, "NOT ENOUGH MATERIALS TO REPAIR");
}
