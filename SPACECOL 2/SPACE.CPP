// ================================================================
// SPACE.CPP - Alien, Asteroid and Artifact
// Each class draws itself and affects the colony in its own way.
// ================================================================
#include "SPACE.H"
#include "COLONY.H"
#include <graphics.h>
#include <stdio.h>
#include <string.h>

int SpaceObject::discovered = 0;     // static member must be defined once

SpaceObject::SpaceObject(char n[], int t)
{
    strcpy(name, n);
    threat = t;
    discovered++;                    // every new object = one more discovery
}

SpaceObject::~SpaceObject()
{
}

char* SpaceObject::getType()      { return "UNKNOWN"; }
char* SpaceObject::getName()      { return name; }
int   SpaceObject::getThreat()    { return threat; }

char* SpaceObject::getThreatText()
{
    if(threat >= 3)
        return "HIGH";
    if(threat == 2)
        return "MEDIUM";
    return "LOW";
}

int SpaceObject::getDiscovered()            { return discovered; }
void SpaceObject::setDiscovered(int count)  { discovered = count; }

// ======================= ALIEN =======================
Alien::Alien(char n[]) : SpaceObject(n, 3) { }
Alien::Alien(char n[], int t) : SpaceObject(n, t) { }

char* Alien::getType() { return "ALIEN LIFEFORM"; }

void Alien::interact(char resultText[])
{
    sprintf(resultText, "The %s reacts to your presence!", name);
}

void Alien::applyEffect(Colony &colony, char rewardText[])
{
    colony.damageHull(10);
    strcpy(rewardText, "ALIEN ATTACK!  HULL -10");
}

void Alien::draw(int x, int y)
{
    setcolor(LIGHTGREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x, y + 28, 14, 16);           // body
    fillellipse(x, y - 8, 22, 24);            // head

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - 9, y - 10, 6, 9);         // big eyes
    fillellipse(x + 9, y - 10, 6, 9);

    setcolor(LIGHTGREEN);
    line(x - 10, y - 30, x - 18, y - 44);     // antennae
    line(x + 10, y - 30, x + 18, y - 44);
    circle(x - 18, y - 46, 3);
    circle(x + 18, y - 46, 3);
}

// ======================= ASTEROID =======================
Asteroid::Asteroid(char n[]) : SpaceObject(n, 2) { }

char* Asteroid::getType() { return "MINERAL ROCK"; }

void Asteroid::interact(char resultText[])
{
    sprintf(resultText, "You mine the %s for useful metal.", name);
}

void Asteroid::applyEffect(Colony &colony, char rewardText[])
{
    colony.addMaterials(20);
    strcpy(rewardText, "MATERIALS +20");
}

void Asteroid::draw(int x, int y)
{
    int shape[16];

    shape[0] = x - 32;  shape[1] = y - 8;
    shape[2] = x - 14;  shape[3] = y - 30;
    shape[4] = x + 14;  shape[5] = y - 28;
    shape[6] = x + 32;  shape[7] = y - 6;
    shape[8] = x + 26;  shape[9] = y + 22;
    shape[10] = x + 2;  shape[11] = y + 32;
    shape[12] = x - 24; shape[13] = y + 24;
    shape[14] = x - 32; shape[15] = y - 8;

    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, BROWN);
    fillpoly(8, shape);

    setcolor(DARKGRAY);                        // craters
    circle(x - 8, y - 6, 7);
    circle(x + 12, y + 10, 5);
    circle(x + 12, y - 16, 3);
}

// ======================= ARTIFACT =======================
Artifact::Artifact(char n[]) : SpaceObject(n, 1) { }

char* Artifact::getType() { return "ALIEN TECHNOLOGY"; }

void Artifact::interact(char resultText[])
{
    sprintf(resultText, "The %s glows as you study its symbols.", name);
}

void Artifact::applyEffect(Colony &colony, char rewardText[])
{
    colony.addResearch(15);
    strcpy(rewardText, "RESEARCH +15");
}

void Artifact::draw(int x, int y)
{
    int shape[10];

    shape[0] = x;       shape[1] = y - 36;
    shape[2] = x + 26;  shape[3] = y;
    shape[4] = x;       shape[5] = y + 36;
    shape[6] = x - 26;  shape[7] = y;
    shape[8] = x;       shape[9] = y - 36;

    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, BLUE);
    fillpoly(5, shape);                        // crystal body

    setcolor(LIGHTMAGENTA);
    circle(x, y, 10);                          // glowing rings
    circle(x, y, 16);

    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    fillellipse(x, y, 4, 4);                   // bright core
    line(x - 26, y, x + 26, y);
}
