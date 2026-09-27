#ifndef INTERACT_H
#define INTERACT_H

// ================================================================
// INTERACT.H - the visual interaction screens
//
// Every action has its own mini sequence:
//     screen appears -> player presses a key -> animation -> result
//
// showScanner() and showExplore() receive a SpaceObject POINTER and
// call obj->draw(...) and obj->getType(). Which picture appears
// depends on the real object (Alien / Asteroid / Artifact):
// this is RUNTIME POLYMORPHISM.
// ================================================================

#include "SPACE.H"

void showCrashIntro();

void showScanner(SpaceObject *obj, char reward[]);
void showRepair(char systemName[], int damage, char reward[]);
void showExplore(char placeName[], SpaceObject *found, char resultText[], char reward[]);
void showResearch(char topic[], char resultText[], char reward[]);
void showUnlock(char techName[], char description[]);
void showCrewTask(char name[], char role[], char resultText[], char reward[]);
void showResourceWork(char task[], int color, char reward[]);

int  showDefense(char title[], int enemyType, int shieldOn);  // returns targets hit (0..6)
int  showCommunication(char title[]);                          // returns signal strength (0..100)

void drawColonyBase(int x, int y);
void drawTower(int x, int baseY);

void showEndingEarth();
void showEndingEscape();
void showEndingAliens();
void showEndingFailure(char title[], char reason[]);

#define ENEMY_ALIEN   0
#define ENEMY_METEOR  1
#define ENEMY_COUNT   6

#endif
