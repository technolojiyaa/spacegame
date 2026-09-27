#ifndef GAME_H
#define GAME_H

// ================================================================
// GAME.H - the Game class runs the whole 10-day campaign
//
// COMPOSITION: a Game HAS a Colony, a Score and 5 crew members.
// The crew is an ARRAY OF BASE-CLASS POINTERS (CrewMember *crew[5]),
// filled with different derived objects using new.
// ================================================================

#include "COLONY.H"
#include "CREW.H"
#include "SPACE.H"
#include "SCORE.H"

#define MAX_DAYS   10
#define CREW_SIZE  5

class Game
{
private:
    Colony colony;
    CrewMember *crew[CREW_SIZE];
    Score score;

    int day;             // 1..10
    int chapter;         // 1..5  (2 days per chapter)
    int communication;   // Communication Array repair progress 0..100
    int alienCore;       // 1 = the Alien Core has been found
    int eventDone;       // 1 = today's story event is finished
    int actionsLeft;     // command actions left today (2 per day)
    int gameOver;
    int quitToMenu;

    // upgrades unlocked by research
    int upScanner;       // research 20
    int upRecycler;      // research 40
    int upShield;        // research 60

    // achievements
    int achFirstContact;
    int achFixer;
    int achScientist;
    int achSurvivor;
    int achSignalFound;
    int achColonySavior;

    // ---- setup ----
    void createCrew();
    void deleteCrew();
    void resetGame();

    // ---- day flow (GAME.CPP) ----
    void playCampaign();
    void playDay();
    void commandPhase();
    int  doCommand(int command);
    void endOfDay();
    void chapterComplete(int finishedChapter);
    void checkGameOver();
    void checkUpgrades();
    void pauseMenu();

    // ---- screens (GAME.CPP) ----
    void showMainScreen();
    void drawCommandButtons(int selected);
    int  waitCommandKey();
    void endGame(char title[], char reason[]);
    void showFinalStats(char endingName[], int success);
    void showAchievements();

    // ---- the 6 commands, each with its own visual interaction ----
    int cmdRepair();
    int cmdExplore();
    int cmdResearch();
    int cmdCrew();
    int cmdDefend();
    int cmdResources();

    // ---- helpers ----
    int   aliveCrew();
    int   randomAliveCrew();
    int   battleDamage(int hits, int perMiss);
    void  addCommunication(int amount);
    char* chapterName(int ch);
    char* dayTitle(int d);
    char* exploreSite();

    // ---- the 10 story days (EVENTS.CPP) ----
    void runDayEvent();
    void dayOne();
    void dayTwo();
    void dayThree();
    void dayFour();
    void dayFive();
    void daySix();
    void daySeven();
    void dayEight();
    void dayNine();
    void dayTen();
    void finalDecision();

public:
    Game();
    ~Game();

    void run();
    void mainMenu();
    void newGame();
    void resumeGame();
    void showHighScores();
    void showInstructions();

    friend class SaveSystem;     // FRIEND CLASS: may save our private data
};

#endif
