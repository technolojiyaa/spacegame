// ================================================================
// GAME.CPP - menus, the main game screen, the 6 commands and the
// day / chapter flow. The 10 story events are in EVENTS.CPP.
//
// One day of the game:
//   day banner -> STORY EVENT (choice + visual interaction)
//   -> 2 COMMAND ACTIONS on the main screen (each has a visual)
//   -> END OF DAY report (resources are used up)
// ================================================================
#include "GAME.H"
#include "UIGRAPH.H"
#include "INTERACT.H"
#include "SOUND.H"
#include "SAVESYS.H"
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================= CONSTRUCTOR / DESTRUCTOR =======================

Game::Game()
{
    createCrew();
    resetGame();
    Score::updateHighScore(SaveSystem::loadHighScore());
}

Game::~Game()
{
    deleteCrew();
}

// DYNAMIC MEMORY: every crew member is created with new ...
void Game::createCrew()
{
    crew[0] = new SpecialEngineer("JAKE");        // multiple + hybrid inheritance
    crew[1] = new Doctor("ELENA");                // single inheritance
    crew[2] = new Scientist("MIRA");
    crew[3] = new EliteSecurityOfficer("TORRES"); // multilevel inheritance
    crew[4] = new Pilot("KAI");
}

// ... and destroyed with delete. The VIRTUAL destructor makes sure the
// right derived destructor runs even though crew[] holds base pointers.
void Game::deleteCrew()
{
    int i;
    for(i = 0; i < CREW_SIZE; i++)
    {
        delete crew[i];
        crew[i] = NULL;
    }
}

void Game::resetGame()
{
    colony = Colony();             // the constructor gives the starting values
    score = Score();

    day = 1;
    chapter = 1;
    communication = 0;
    alienCore = 0;
    eventDone = 0;
    actionsLeft = 2;
    gameOver = 0;
    quitToMenu = 0;

    upScanner = 0;
    upRecycler = 0;
    upShield = 0;

    achFirstContact = 0;
    achFixer = 0;
    achScientist = 0;
    achSurvivor = 0;
    achSignalFound = 0;
    achColonySavior = 0;

    SpaceObject::setDiscovered(0);
}

// ======================= MENUS =======================

void Game::run()
{
    initGraphicsMode();
    mainMenu();
    closeGraphicsMode();
}

void Game::mainMenu()
{
    int choice, running = 1;
    char items[6][40] = {
        "NEW GAME",
        "RESUME",
        "HIGH SCORES",
        "INSTRUCTIONS",
        "SOUND: ON",
        "EXIT"
    };

    while(running == 1)
    {
        if(isSoundOn() == 1)
            strcpy(items[4], "SOUND: ON");
        else
            strcpy(items[4], "SOUND: OFF");

        drawMainMenuScreen();
        choice = chooseButton(items, 6, 220, 232, 200, 36, 0, 1);

        if(choice == 0)      newGame();
        else if(choice == 1) resumeGame();
        else if(choice == 2) showHighScores();
        else if(choice == 3) showInstructions();
        else if(choice == 4) setSoundOn(1 - isSoundOn());
        else                 running = 0;
    }
}

void Game::newGame()
{
    deleteCrew();                  // fresh crew objects for a fresh game
    createCrew();
    resetGame();

    showCrashIntro();
    playCampaign();
}

void Game::resumeGame()
{
    deleteCrew();
    createCrew();
    resetGame();

    if(SaveSystem::loadGame(*this) == 0)      // *this = this Game object
    {
        showNotice("NO SAVED GAME", "No saved game was found. Start a NEW GAME first.",
                   "THE GAME AUTO-SAVES AFTER EVERY CHAPTER");
        resetGame();
        return;
    }

    gameOver = 0;
    quitToMenu = 0;
    showNotice("SAVE LOADED", "Colony data restored. Welcome back, Commander.", "");
    playCampaign();
}

void Game::showHighScores()
{
    int scores[MAX_SCORES];
    int count, i;
    char text[40];
    char rank[5][6] = { "1ST", "2ND", "3RD", "4TH", "5TH" };

    count = SaveSystem::loadHighScores(scores, MAX_SCORES);

    drawScreenFrame("HIGH SCORES");
    drawTitle("HALL OF COMMANDERS", 60, LIGHTCYAN, 3);
    drawPanel(150, 110, 489, 380, "TOP 5");

    if(count == 0)
        centerText("NO SCORES YET - FINISH A GAME FIRST!", 230, LIGHTGRAY);

    for(i = 0; i < count; i++)
    {
        sprintf(text, "%s  .........  %5d", rank[i], scores[i]);
        drawTitle(text, 140 + i * 46, i == 0 ? YELLOW : WHITE, 2);
    }
    waitForEnter(420);
}

void Game::showInstructions()
{
    int i;
    char mission[6][70] = {
        "Your colony ship crashed on the planet KEPLER-X.",
        "Survive 10 DAYS (5 chapters). Every day has a story event.",
        "After the event you get 2 COMMAND ACTIONS on the main screen.",
        "Each night the colony uses OXYGEN, FOOD and ENERGY.",
        "If OXYGEN or HULL reach 0, or all crew die: GAME OVER.",
        "Find the ALIEN CORE and fix the COMMUNICATION ARRAY to win."
    };
    char controls[4][70] = {
        "ARROW KEYS / W A S D  : move between buttons",
        "ENTER or SPACE        : press the selected button",
        "1 - 6                 : choose an option or command directly",
        "P = PAUSE / SAVE      N = END THE DAY EARLY"
    };

    drawScreenFrame("INSTRUCTIONS");
    drawPanel(25, 60, 614, 200, "MISSION");
    for(i = 0; i < 6; i++)
    {
        setcolor(WHITE);
        outtextxy(40, 78 + i * 19, mission[i]);
    }

    drawPanel(25, 220, 614, 330, "CONTROLS");
    for(i = 0; i < 4; i++)
    {
        setcolor(LIGHTGREEN);
        outtextxy(40, 238 + i * 20, controls[i]);
    }

    drawPanel(25, 346, 614, 432, "TIPS");
    setcolor(YELLOW);
    outtextxy(40, 360, "RESEARCH unlocks upgrades at 20, 40 and 60 points.");
    outtextxy(40, 378, "ENDINGS: EARTH needs the Alien Core + Comm Array 60%.");
    outtextxy(40, 396, "ESCAPE needs pilot KAI, Energy 30, Hull 30. ALIENS needs");
    outtextxy(40, 414, "the Alien Core + First Contact (or Research 50).");
    waitForEnter(448);
}

// ======================= CAMPAIGN FLOW =======================

void Game::playCampaign()
{
    while(day <= MAX_DAYS && gameOver == 0 && quitToMenu == 0)
        playDay();
}

void Game::playDay()
{
    char daysText[30];

    chapter = (day + 1) / 2;

    if(eventDone == 0)
    {
        if(day % 2 == 1)                    // first day of a chapter
        {
            sprintf(daysText, "DAY %d - %d", day, day + 1);
            showChapterTitle(chapter, chapterName(chapter), daysText);
        }
        showDayBanner(day, MAX_DAYS, dayTitle(day));

        runDayEvent();
        eventDone = 1;
        if(gameOver == 1)                   // Day 10 ends the game here
            return;

        checkUpgrades();
        checkGameOver();
        if(gameOver == 1)
            return;
    }

    commandPhase();
    if(gameOver == 1 || quitToMenu == 1)
        return;

    endOfDay();
}

// The main screen: player chooses 2 commands per day.
void Game::commandPhase()
{
    int key, selected = 0, used;

    while(actionsLeft > 0 && gameOver == 0 && quitToMenu == 0)
    {
        showMainScreen();
        drawCommandButtons(selected);

        key = waitCommandKey();
        while(key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN)
        {
            if(key == KEY_LEFT && selected % 3 > 0)  selected--;
            if(key == KEY_RIGHT && selected % 3 < 2) selected++;
            if(key == KEY_UP && selected >= 3)       selected = selected - 3;
            if(key == KEY_DOWN && selected < 3)      selected = selected + 3;
            playClick();
            drawCommandButtons(selected);
            key = waitCommandKey();
        }

        if(key == 'p' || key == 'P')
        {
            pauseMenu();
        }
        else if(key == 'n' || key == 'N')
        {
            actionsLeft = 0;                // skip the remaining actions
        }
        else
        {
            if(key >= '1' && key <= '6')
                selected = key - '1';

            drawCommandButtons(-1);         // "press" effect
            delay(60);
            drawCommandButtons(selected);
            playClick();

            used = doCommand(selected);
            if(used == 1)
            {
                actionsLeft--;
                checkUpgrades();
                checkGameOver();
            }
        }
    }
}

int Game::doCommand(int command)
{
    switch(command)
    {
        case 0: return cmdRepair();
        case 1: return cmdExplore();
        case 2: return cmdResearch();
        case 3: return cmdCrew();
        case 4: return cmdDefend();
        case 5: return cmdResources();
    }
    return 0;
}

void Game::endOfDay()
{
    int i, n, oxygenLoss, foodLoss, energyLoss;
    int finishedChapter = chapter;
    char title[30];
    char lines[7][70];
    Score dayScore;

    oxygenLoss = 8;
    if(upRecycler == 1)
        oxygenLoss = 4;
    foodLoss = aliveCrew() * 2;
    energyLoss = 6;

    colony.useOxygen(oxygenLoss);
    colony.useFood(foodLoss);
    colony.useEnergy(energyLoss);

    sprintf(lines[0], "Life support used %d oxygen.", oxygenLoss);
    sprintf(lines[1], "The crew ate %d food.", foodLoss);
    sprintf(lines[2], "The colony used %d energy for the night.", energyLoss);
    n = 3;

    if(colony.getFood() == 0)
    {
        for(i = 0; i < CREW_SIZE; i++)
            crew[i]->damage(15);
        strcpy(lines[n], "NO FOOD LEFT! Every crew member lost 15 health.");
        n++;
    }
    if(colony.getEnergy() == 0)
    {
        colony.useOxygen(10);
        strcpy(lines[n], "NO POWER! Life support lost 10 extra oxygen.");
        n++;
    }

    dayScore.addSurvival(10 + aliveCrew() * 2);
    score = score + dayScore;                  // OPERATOR OVERLOADING
    sprintf(lines[n], "Day survived: +%d score.   Total score: %d",
            dayScore.getTotal(), score.getTotal());
    n++;

    // ---- night report screen ----
    sprintf(title, "END OF DAY %d", day);
    drawScreenFrame(title);
    drawPanel(30, 70, 609, 92 + n * 22, "NIGHT REPORT");
    for(i = 0; i < n; i++)
        typeText(48, 90 + i * 22, lines[i], i < 3 ? WHITE : YELLOW);

    drawPanel(110, 250, 529, 392, "COLONY AFTER THE NIGHT");
    drawStatBar(140, 268, "OXYGEN", colony.getOxygen());
    drawStatBar(140, 292, "ENERGY", colony.getEnergy());
    drawStatBar(140, 316, "FOOD", colony.getFood());
    drawStatBar(140, 340, "HULL", colony.getHull());
    drawStatBar(140, 364, "COMM ARRAY", communication);
    waitForEnter(430);

    checkGameOver();
    if(gameOver == 1)
        return;

    day++;
    eventDone = 0;
    actionsLeft = 2;

    if(day % 2 == 1)                           // days 2, 4, 6, 8 end a chapter
        chapterComplete(finishedChapter);
}

void Game::chapterComplete(int finishedChapter)
{
    int i;
    char title[40];
    char lines[5][70];

    sprintf(lines[0], "Days survived   : %d / %d", day - 1, MAX_DAYS);
    sprintf(lines[1], "Crew alive      : %d / %d", aliveCrew(), CREW_SIZE);
    sprintf(lines[2], "Research        : %d", colony.getResearch());
    sprintf(lines[3], "Comm Array      : %d%%", communication);
    sprintf(lines[4], "Score           : %d", score.getTotal());

    drawScreenFrame("CHAPTER COMPLETE");
    sprintf(title, "CHAPTER %d COMPLETE", finishedChapter);
    drawTitle(title, 70, LIGHTGREEN, 3);
    drawTitle(chapterName(finishedChapter), 110, YELLOW, 2);
    playSuccessSound();

    drawPanel(120, 150, 519, 300, "CHAPTER REPORT");
    for(i = 0; i < 5; i++)
        typeText(150, 172 + i * 22, lines[i], WHITE);

    SaveSystem::saveGame(*this);               // AUTO-SAVE (file handling)
    centerText("PROGRESS AUTO-SAVED", 330, LIGHTCYAN);
    waitForEnter(420);
}

void Game::checkGameOver()
{
    if(gameOver == 1)
        return;

    if(colony.isColonyDead() == 1)
    {
        if(colony.getOxygen() <= 0)
            endGame("OXYGEN DEPLETED", "The air ran out. The crew could not survive.");
        else
            endGame("COLONY DESTROYED", "The hull collapsed under the damage.");
    }
    else if(aliveCrew() == 0)
    {
        endGame("CREW LOST", "Every member of the crew has died.");
    }
}

void Game::checkUpgrades()
{
    int research = colony.getResearch();

    if(upScanner == 0 && research >= 20)
    {
        upScanner = 1;
        showUnlock("ALIEN SCANNER", "Scans are stronger. You can scan for the Alien Core on Day 9.");
    }
    if(upRecycler == 0 && research >= 40)
    {
        upRecycler = 1;
        showUnlock("OXYGEN RECYCLER", "The colony now uses only half as much oxygen each night.");
    }
    if(upShield == 0 && research >= 60)
    {
        upShield = 1;
        colony.addDefense(20);
        showUnlock("PLASMA SHIELD", "Enemies move slower in battles. DEFENSE +20.");
    }
    if(upScanner == 1 && upRecycler == 1 && upShield == 1)
        achScientist = 1;
}

void Game::pauseMenu()
{
    int choice;
    char items[5][40] = {
        "RESUME",
        "SAVE GAME",
        "SOUND ON / OFF",
        "SAVE AND QUIT TO MENU",
        "QUIT WITHOUT SAVING"
    };

    drawPanel(160, 110, 479, 400, "PAUSED");
    drawTitle("GAME PAUSED", 130, YELLOW, 2);
    choice = chooseButton(items, 5, 180, 168, 280, 38, 1, 0);

    if(choice == 1 || choice == 3)
    {
        SaveSystem::saveGame(*this);
        centerText("GAME SAVED", 370, LIGHTGREEN);
        playSuccessSound();
        delay(500);
    }
    if(choice == 2)
        setSoundOn(1 - isSoundOn());
    if(choice == 3 || choice == 4)
        quitToMenu = 1;
}

// ======================= MAIN GAME SCREEN =======================

void Game::showMainScreen()
{
    int i, y;
    char left[40], middle[40], right[50], text[50];

    drawScreenFrame("SPACE COLONY SURVIVAL");

    sprintf(left, "DAY %d / %d   CREW %d/%d", day, MAX_DAYS, aliveCrew(), CREW_SIZE);
    sprintf(middle, "SCORE %d", score.getTotal());
    sprintf(right, "CHAPTER %d: %s", chapter, chapterName(chapter));
    drawStatusBar(left, middle, right);

    // ---- colony status ----
    drawPanel(15, 72, 372, 302, "COLONY STATUS");
    drawStatBar(28, 88, "OXYGEN", colony.getOxygen());
    drawStatBar(28, 111, "ENERGY", colony.getEnergy());
    drawStatBar(28, 134, "FOOD", colony.getFood());
    drawStatBar(28, 157, "MATERIALS", colony.getMaterials());
    drawStatBar(28, 180, "HULL", colony.getHull());
    drawStatBar(28, 203, "DEFENSE", colony.getDefense());
    drawStatBar(28, 226, "RESEARCH", colony.getResearch());
    drawStatBar(28, 249, "COMM ARRAY", communication);

    setcolor(LIGHTGRAY);
    outtextxy(28, 280, "TECH:");
    setcolor(upScanner == 1 ? LIGHTGREEN : DARKGRAY);
    outtextxy(76, 280, "SCANNER");
    setcolor(upRecycler == 1 ? LIGHTGREEN : DARKGRAY);
    outtextxy(140, 280, "RECYCLER");
    setcolor(upShield == 1 ? LIGHTGREEN : DARKGRAY);
    outtextxy(212, 280, "SHIELD");
    setcolor(alienCore == 1 ? LIGHTMAGENTA : DARKGRAY);
    outtextxy(270, 280, "ALIEN CORE");

    // ---- crew ----
    drawPanel(382, 72, 624, 302, "CREW");
    for(i = 0; i < CREW_SIZE; i++)
    {
        y = 84 + i * 42;
        setcolor(crew[i]->isAlive() == 1 ? WHITE : LIGHTRED);
        outtextxy(394, y, crew[i]->getName());
        setcolor(LIGHTCYAN);
        outtextxy(394, y + 11, crew[i]->getRole());
        if(crew[i]->isAlive() == 1)
            drawProgressBar(394, y + 22, 150, crew[i]->getHealth());
        else
        {
            setcolor(LIGHTRED);
            outtextxy(394, y + 24, "-- LOST --");
        }
    }

    // ---- command menu ----
    drawPanel(15, 314, 624, 468, "COMMAND MENU");
    sprintf(text, "ACTIONS LEFT TODAY: %d", actionsLeft);
    setcolor(YELLOW);
    outtextxy(30, 330, text);
    setcolor(LIGHTGRAY);
    outtextxy(330, 330, "[ARROWS] MOVE   [ENTER] SELECT");

    drawButton(30, 418, 180, "[P] PAUSE / SAVE", 0);
    drawButton(230, 418, 180, "[N] END DAY", 0);
}

void Game::drawCommandButtons(int selected)
{
    int i;
    char labels[6][20] = {
        "[1] REPAIR", "[2] EXPLORE", "[3] RESEARCH",
        "[4] CREW",   "[5] DEFEND",  "[6] RESOURCES"
    };

    for(i = 0; i < 6; i++)
        drawButton(30 + (i % 3) * 200, 350 + (i / 3) * 32, 180, labels[i], i == selected);
}

// Waits for a useful key. While waiting, low resources blink a warning.
int Game::waitCommandKey()
{
    int key, blink = 0;
    char warning[40];

    strcpy(warning, "");
    if(colony.getOxygen() < 25)       strcpy(warning, "!! OXYGEN CRITICAL !!");
    else if(colony.getHull() < 25)    strcpy(warning, "!! HULL CRITICAL !!");
    else if(colony.getFood() < 20)    strcpy(warning, "!! FOOD RUNNING OUT !!");
    else if(colony.getEnergy() < 20)  strcpy(warning, "!! ENERGY LOW !!");

    if(warning[0] != '\0')
        playTone(880, 80);

    while(1)
    {
        while(!kbhit())
        {
            if(warning[0] != '\0')
            {
                centerText(warning, 520, 426, blink == 0 ? LIGHTRED : YELLOW);
                blink = 1 - blink;
            }
            delay(250);
        }

        key = readKey();
        if(key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN)
            return key;
        if(key == 'a' || key == 'A') return KEY_LEFT;
        if(key == 'd' || key == 'D') return KEY_RIGHT;
        if(key == 'w' || key == 'W') return KEY_UP;
        if(key == 's' || key == 'S') return KEY_DOWN;
        if(key >= '1' && key <= '6') return key;
        if(key == KEY_ENTER || key == KEY_SPACE) return key;
        if(key == 'p' || key == 'P' || key == 'n' || key == 'N') return key;
    }
}

// ======================= THE 6 COMMANDS =======================
// Each returns 1 if the action was used, 0 if it was cancelled.

int Game::cmdRepair()
{
    if(colony.getHull() >= 100)
    {
        showNotice("NO REPAIR NEEDED", "The hull is already at 100%.", "NO ACTION USED");
        return 0;
    }
    if(colony.getMaterials() < 15)
    {
        showWarning("NOT ENOUGH MATERIALS", "A hull repair needs 15 materials. Try EXPLORE or the crew.",
                    "NO ACTION USED");
        return 0;
    }

    showRepair("HULL PLATING", 100 - colony.getHull(), "HULL +15   MATERIALS -15");
    colony.repair(15);                 // overloaded repair(int)
    score.addSurvival(5);
    return 1;
}

int Game::cmdExplore()
{
    SpaceObject *found = NULL;         // BASE CLASS POINTER
    char result[80], reward[60], fullReward[70];
    int r;

    if(colony.getEnergy() < 5)
    {
        showWarning("NO ENERGY", "The rover needs 5 energy to drive.", "NO ACTION USED");
        return 0;
    }
    colony.useEnergy(5);

    r = rand() % 4;
    if(r == 0)      found = new Asteroid("MINERAL ROCK");
    else if(r == 1) found = new Artifact("ALIEN RELIC");
    else if(r == 2) found = new Alien("SCOUT DRONE", 3);

    if(found != NULL)
    {
        found->interact(result);                // RUNTIME POLYMORPHISM:
        found->applyEffect(colony, reward);     // the real object's version runs
    }
    else
    {
        strcpy(result, "An old supply crate from the crash still has food inside.");
        strcpy(reward, "FOOD +15   OXYGEN +10");
        colony.addFood(15);
        colony.addOxygen(10);
    }

    sprintf(fullReward, "%s   ENERGY -5", reward);
    showExplore(exploreSite(), found, result, fullReward);

    if(found != NULL)
        delete found;                           // free the dynamic object
    score.addMission(5);
    return 1;
}

int Game::cmdResearch()
{
    int next;
    char topic[30], result[70];

    if(colony.getEnergy() < 10)
    {
        showWarning("NO ENERGY", "The lab computer needs 10 energy.", "NO ACTION USED");
        return 0;
    }
    colony.useEnergy(10);
    colony.addResearch(10);

    next = 0;
    if(upScanner == 0)       { next = 20; strcpy(topic, "SCANNER DESIGN"); }
    else if(upRecycler == 0) { next = 40; strcpy(topic, "AIR RECYCLER"); }
    else if(upShield == 0)   { next = 60; strcpy(topic, "PLASMA SHIELD"); }
    else                       strcpy(topic, "ALIEN SAMPLES");

    if(next > 0)
        sprintf(result, "Research %d / %d needed for the next upgrade.",
                colony.getResearch(), next);
    else
        strcpy(result, "All upgrades built. The data still earns score.");

    showResearch(topic, result, "RESEARCH +10   ENERGY -10");
    score.addMission(5);
    return 1;
}

int Game::cmdCrew()
{
    int i, choice;
    char items[6][40];
    char result[80], reward[60];

    for(i = 0; i < CREW_SIZE; i++)
    {
        if(crew[i]->isAlive() == 1)
            sprintf(items[i], "%-7s %-22s %3d", crew[i]->getName(),
                    crew[i]->getRole(), crew[i]->getHealth());
        else
            sprintf(items[i], "%-7s %-22s LOST", crew[i]->getName(), crew[i]->getRole());
    }
    strcpy(items[5], "BACK");

    drawScreenFrame("CREW QUARTERS");
    centerText("CHOOSE A CREW MEMBER TO USE THEIR SPECIAL ABILITY", 55, YELLOW);
    setcolor(LIGHTGRAY);
    outtextxy(200, 78, "NAME    ROLE                HEALTH");

    drawPanel(60, 318, 579, 400, "ABILITIES");
    setcolor(LIGHTCYAN);
    outtextxy(80, 336, "ENGINEER  : repairs the hull   DOCTOR   : heals all crew");
    outtextxy(80, 356, "SCIENTIST : +10 research       SECURITY : +defense");
    outtextxy(80, 376, "PILOT     : scavenges materials (costs 5 energy)");

    choice = chooseButton(items, 6, 140, 95, 360, 34, 1, 0);

    if(choice == 5)
        return 0;
    if(crew[choice]->isAlive() == 0)
    {
        showWarning("CREW MEMBER LOST", "This crew member can no longer help the colony.", "NO ACTION USED");
        return 0;
    }

    // RUNTIME POLYMORPHISM: same call, different behaviour for each role
    crew[choice]->useAbility(colony, crew, CREW_SIZE, reward);
    crew[choice]->specialAbility(result);
    showCrewTask(crew[choice]->getName(), crew[choice]->getRole(), result, reward);
    return 1;
}

int Game::cmdDefend()
{
    int hits;
    char line1[50], reward[50];

    if(colony.getEnergy() < 5)
    {
        showWarning("NO ENERGY", "The defense grid needs 5 energy.", "NO ACTION USED");
        return 0;
    }
    colony.useEnergy(5);

    hits = showDefense("DEFENSE DRILL", ENEMY_ALIEN, upShield);
    colony.addDefense(hits * 4);
    score.addMission(hits * 2);

    sprintf(line1, "Training targets destroyed: %d of %d", hits, ENEMY_COUNT);
    sprintf(reward, "DEFENSE +%d   ENERGY -5", hits * 4);
    showResultCard(hits >= 4 ? "DRILL PASSED" : "DRILL FINISHED", line1,
                   "Practice makes the defense grid stronger.", reward, 1);
    return 1;
}

int Game::cmdResources()
{
    int choice;
    char items[4][40] = {
        "HYDROPONICS   FOOD +20  ENERGY -5",
        "OXYGEN PLANT  OXYGEN +20  ENERGY -5",
        "SOLAR PANELS  ENERGY +20",
        "BACK"
    };

    drawScreenFrame("RESOURCE CONTROL");
    drawPanel(90, 60, 549, 160, "CURRENT LEVELS");
    drawStatBar(130, 80, "OXYGEN", colony.getOxygen());
    drawStatBar(130, 104, "FOOD", colony.getFood());
    drawStatBar(130, 128, "ENERGY", colony.getEnergy());
    centerText("CHOOSE WHAT TO PRODUCE TODAY", 180, YELLOW);

    choice = chooseButton(items, 4, 120, 205, 400, 38, 1, 0);

    if(choice == 3)
        return 0;

    if(choice == 2)
    {
        colony.addEnergy(20);
        showResourceWork("SOLAR PANELS", YELLOW, "ENERGY +20");
        return 1;
    }

    if(colony.getEnergy() < 5)
    {
        showWarning("NO ENERGY", "The machines need 5 energy. Use SOLAR PANELS first.", "NO ACTION USED");
        return 0;
    }
    colony.useEnergy(5);

    if(choice == 0)
    {
        colony.addFood(20);
        showResourceWork("HYDROPONICS BAY", GREEN, "FOOD +20   ENERGY -5");
    }
    else
    {
        colony.addOxygen(20);
        showResourceWork("OXYGEN PLANT", LIGHTCYAN, "OXYGEN +20   ENERGY -5");
    }
    return 1;
}

// ======================= ENDING / STATS =======================

void Game::endGame(char title[], char reason[])
{
    gameOver = 1;
    showEndingFailure(title, reason);
    showFinalStats("GAME OVER", 0);
}

void Game::showFinalStats(char endingName[], int success)
{
    int i, total;
    char lines[7][70];
    char text[40];

    total = calculateFinalScore(score);        // FRIEND FUNCTION
    Score::updateHighScore(total);             // STATIC MEMBER FUNCTION
    SaveSystem::saveHighScore(total);          // FILE HANDLING

    sprintf(lines[0], "Days survived      : %d / %d", day, MAX_DAYS);
    sprintf(lines[1], "Crew alive         : %d / %d", aliveCrew(), CREW_SIZE);
    sprintf(lines[2], "Oxygen %d   Energy %d   Food %d", colony.getOxygen(),
            colony.getEnergy(), colony.getFood());
    sprintf(lines[3], "Hull %d   Materials %d   Defense %d", colony.getHull(),
            colony.getMaterials(), colony.getDefense());
    sprintf(lines[4], "Research %d   Comm Array %d%%", colony.getResearch(), communication);
    sprintf(lines[5], "Objects discovered : %d", SpaceObject::getDiscovered());
    sprintf(lines[6], "Alien Core found   : %s", alienCore == 1 ? "YES" : "NO");

    drawScreenFrame("FINAL REPORT");
    drawTitle(endingName, 52, success == 1 ? LIGHTGREEN : LIGHTRED, 3);
    drawPanel(70, 95, 569, 280, "MISSION STATISTICS");
    for(i = 0; i < 7; i++)
        typeText(95, 115 + i * 22, lines[i], WHITE);

    sprintf(text, "FINAL SCORE: %d", total);
    drawTitle(text, 300, YELLOW, 3);
    sprintf(text, "BEST SCORE: %d", Score::getHighScore());
    centerText(text, 340, LIGHTCYAN);
    if(total >= Score::getHighScore())
        centerText("*** NEW HIGH SCORE! ***", 360, LIGHTGREEN);

    playSuccessSound();
    waitForEnter(430);
    showAchievements();
}

void Game::showAchievements()
{
    int i, got[6];
    char names[6][20] = { "FIRST CONTACT", "FIXER", "SCIENTIST",
                          "SURVIVOR", "SIGNAL FOUND", "COLONY SAVIOR" };
    char info[6][50] = {
        "Talk to the alien on Day 4",
        "Recover the Alien Core",
        "Unlock all 3 research upgrades",
        "Win with all 5 crew alive",
        "Scan the unknown signal on Day 3",
        "Win with the hull at 50% or more"
    };

    got[0] = achFirstContact;  got[1] = achFixer;       got[2] = achScientist;
    got[3] = achSurvivor;      got[4] = achSignalFound; got[5] = achColonySavior;

    drawScreenFrame("ACHIEVEMENTS");
    drawPanel(40, 60, 599, 400, "MEDALS");
    for(i = 0; i < 6; i++)
    {
        setcolor(got[i] == 1 ? YELLOW : DARKGRAY);
        setfillstyle(SOLID_FILL, got[i] == 1 ? YELLOW : DARKGRAY);
        fillellipse(75, 92 + i * 50, 10, 10);            // medal
        setcolor(got[i] == 1 ? LIGHTGREEN : DARKGRAY);
        outtextxy(100, 84 + i * 50, names[i]);
        outtextxy(460, 84 + i * 50, got[i] == 1 ? "UNLOCKED" : "LOCKED");
        setcolor(got[i] == 1 ? WHITE : DARKGRAY);
        outtextxy(100, 100 + i * 50, info[i]);
    }
    waitForEnter(430);
}

// ======================= HELPERS =======================

int Game::aliveCrew()
{
    int i, count = 0;
    for(i = 0; i < CREW_SIZE; i++)
        if(crew[i]->isAlive() == 1)
            count++;
    return count;
}

// Returns the index of a random living crew member, or -1.
int Game::randomAliveCrew()
{
    int i, start;
    if(aliveCrew() == 0)
        return -1;
    start = rand() % CREW_SIZE;
    for(i = 0; i < CREW_SIZE; i++)
        if(crew[(start + i) % CREW_SIZE]->isAlive() == 1)
            return (start + i) % CREW_SIZE;
    return -1;
}

// Damage after a battle: every missed target hurts, defense protects.
int Game::battleDamage(int hits, int perMiss)
{
    int damage = (ENEMY_COUNT - hits) * perMiss - colony.getDefense() / 10;
    if(damage < 0)
        damage = 0;
    return damage;
}

void Game::addCommunication(int amount)
{
    communication = communication + amount;
    if(communication > 100) communication = 100;
    if(communication < 0)   communication = 0;
}

char* Game::chapterName(int ch)
{
    static char names[6][25] = { "", "CRASH", "UNKNOWN SIGNAL", "THE UNKNOWN",
                                 "COLONY CRISIS", "FINAL TRANSMISSION" };
    if(ch < 1 || ch > 5)
        return names[0];
    return names[ch];
}

char* Game::exploreSite()
{
    static char sites[6][20] = { "", "CRASH SITE", "SIGNAL RIDGE", "DARK CRATER",
                                 "ALIEN RUINS", "CORE CANYON" };
    if(chapter < 1 || chapter > 5)
        return sites[1];
    return sites[chapter];
}
