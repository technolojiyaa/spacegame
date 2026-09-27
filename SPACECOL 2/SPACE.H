#ifndef SPACE_H
#define SPACE_H

// ================================================================
// SPACE.H - things the crew can find on the planet
//
//                 SpaceObject   (ABSTRACT class)
//                      |
//           +----------+----------+
//           |          |          |
//         Alien     Asteroid   Artifact
//
// SpaceObject has PURE VIRTUAL functions ( = 0 ), so we can never
// write "new SpaceObject". We always create a real type:
//
//      SpaceObject *obj = new Alien("SCOUT DRONE");
//      obj->interact(text);    // Alien's version runs
//      obj->draw(x, y);        // Alien picture is drawn
//      delete obj;
// ================================================================

class Colony;

class SpaceObject
{
protected:
    char name[30];
    int threat;                    // 1 = LOW, 2 = MEDIUM, 3 = HIGH

    static int discovered;         // STATIC MEMBER: shared by all objects

public:
    SpaceObject(char n[], int t);
    virtual ~SpaceObject();

    // PURE VIRTUAL FUNCTIONS -> abstract class
    virtual void interact(char resultText[]) = 0;
    virtual void draw(int x, int y) = 0;
    virtual void applyEffect(Colony &colony, char rewardText[]) = 0;

    // normal VIRTUAL function (has a default body, may be overridden)
    virtual char* getType();

    char* getName();
    int   getThreat();
    char* getThreatText();

    static int  getDiscovered();
    static void setDiscovered(int count);
};

class Alien : public SpaceObject
{
public:
    Alien(char n[]);               // constructor overloading:
    Alien(char n[], int t);        // default threat or chosen threat
    void interact(char resultText[]);
    void draw(int x, int y);
    void applyEffect(Colony &colony, char rewardText[]);
    char* getType();
};

class Asteroid : public SpaceObject
{
public:
    Asteroid(char n[]);
    void interact(char resultText[]);
    void draw(int x, int y);
    void applyEffect(Colony &colony, char rewardText[]);
    char* getType();
};

class Artifact : public SpaceObject
{
public:
    Artifact(char n[]);
    void interact(char resultText[]);
    void draw(int x, int y);
    void applyEffect(Colony &colony, char rewardText[]);
    char* getType();
};

#endif
