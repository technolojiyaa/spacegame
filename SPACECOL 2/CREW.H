#ifndef CREW_H
#define CREW_H

// ================================================================
// CREW.H - the crew class family
//
//                    CrewMember              (base class)
//        ____________/ |    |    |  \_________
//       |              |    |    |            |
//   Engineer        Doctor Scientist Pilot  SecurityOfficer
//       |                                     |
//  SpecialEngineer <-- RepairAbility     EliteSecurityOfficer
//
//  Hierarchical : CrewMember -> 5 different roles
//  Single       : Doctor : public CrewMember
//  Multilevel   : CrewMember -> SecurityOfficer -> EliteSecurityOfficer
//  Multiple     : SpecialEngineer : public Engineer, public RepairAbility
//  Hybrid       : SpecialEngineer mixes hierarchical + multiple
// ================================================================

class Colony;    // forward declaration (full class is in COLONY.H)

class CrewMember
{
protected:
    char name[30];
    char role[30];
    int health;
    int alive;

public:
    CrewMember();
    CrewMember(char n[], char r[]);
    virtual ~CrewMember();           // virtual destructor: safe delete through base pointer

    void setHealth(int h);
    int  getHealth();
    int  isAlive();
    void damage(int amount);
    void heal(int amount);

    char* getName();
    char* getRole();

    // VIRTUAL FUNCTIONS -> RUNTIME POLYMORPHISM
    // crew[i]->useAbility(...) calls the version of the REAL object type.
    virtual void specialAbility(char resultText[]);
    virtual void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);

    friend class SaveSystem;         // FRIEND CLASS (used to save health/alive)
};

class Engineer : public CrewMember
{
public:
    Engineer(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

class Doctor : public CrewMember          // SINGLE INHERITANCE
{
public:
    Doctor(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

class Scientist : public CrewMember
{
public:
    Scientist(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

class SecurityOfficer : public CrewMember
{
public:
    SecurityOfficer(char n[]);
    SecurityOfficer(char n[], char r[]);   // constructor overloading
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

// MULTILEVEL INHERITANCE: CrewMember -> SecurityOfficer -> EliteSecurityOfficer
class EliteSecurityOfficer : public SecurityOfficer
{
public:
    EliteSecurityOfficer(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

class Pilot : public CrewMember
{
public:
    Pilot(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

// A small "skill" class used to build the multiple inheritance example
class RepairAbility
{
public:
    void extraRepair(char resultText[]);
};

// MULTIPLE INHERITANCE (Engineer + RepairAbility).
// Engineer already belongs to the CrewMember family, so this is
// also HYBRID INHERITANCE.
class SpecialEngineer : public Engineer, public RepairAbility
{
public:
    SpecialEngineer(char n[]);
    void specialAbility(char resultText[]);
    void useAbility(Colony &colony, CrewMember *team[], int count, char rewardText[]);
};

#endif
