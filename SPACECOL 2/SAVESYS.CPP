// ================================================================
// SAVESYS.CPP - FILE HANDLING with ofstream / ifstream
// ================================================================
#include "SAVESYS.H"
#include "GAME.H"
#include <fstream.h>
#include <string.h>

// ---------------- save the whole game ----------------
void SaveSystem::saveGame(Game &game)
{
    int i;
    ofstream outFile("SAVE.DAT");

    outFile << "SCS10" << "\n";      // file tag: "Space Colony Survival, 10 days"

    // progress
    outFile << game.day << " " << game.chapter << " "
            << game.communication << " " << game.alienCore << " "
            << game.eventDone << " " << game.actionsLeft << "\n";

    // upgrades
    outFile << game.upScanner << " " << game.upRecycler << " "
            << game.upShield << "\n";

    // achievements
    outFile << game.achFirstContact << " " << game.achFixer << " "
            << game.achScientist << " " << game.achSurvivor << " "
            << game.achSignalFound << " " << game.achColonySavior << "\n";

    // colony resources (private data, allowed because we are a friend)
    outFile << game.colony.oxygen << " " << game.colony.energy << " "
            << game.colony.food << " " << game.colony.materials << " "
            << game.colony.hull << " " << game.colony.research << " "
            << game.colony.defense << "\n";

    // score and discoveries
    outFile << game.score.getTotal() << " "
            << SpaceObject::getDiscovered() << "\n";

    // crew health and status
    for(i = 0; i < CREW_SIZE; i++)
        outFile << game.crew[i]->health << " " << game.crew[i]->alive << "\n";

    outFile.close();
}

// ---------------- load the whole game ----------------
int SaveSystem::loadGame(Game &game)
{
    int i, total, found;
    char tag[20];
    ifstream inFile("SAVE.DAT");

    if(!inFile)
        return 0;

    inFile.width(20);               // never read more than the array can hold
    inFile >> tag;
    if(strcmp(tag, "SCS10") != 0)   // old 15-day save or broken file
    {
        inFile.close();
        return 0;
    }

    inFile >> game.day >> game.chapter >> game.communication
           >> game.alienCore >> game.eventDone >> game.actionsLeft;

    inFile >> game.upScanner >> game.upRecycler >> game.upShield;

    inFile >> game.achFirstContact >> game.achFixer >> game.achScientist
           >> game.achSurvivor >> game.achSignalFound >> game.achColonySavior;

    inFile >> game.colony.oxygen >> game.colony.energy >> game.colony.food
           >> game.colony.materials >> game.colony.hull
           >> game.colony.research >> game.colony.defense;

    inFile >> total >> found;
    game.score = Score();
    game.score.addSurvival(total);
    SpaceObject::setDiscovered(found);

    for(i = 0; i < CREW_SIZE; i++)
        inFile >> game.crew[i]->health >> game.crew[i]->alive;

    inFile.close();

    if(game.day < 1 || game.day > MAX_DAYS)
        return 0;
    return 1;
}

// ---------------- top 5 high scores ----------------
int SaveSystem::loadHighScores(int scores[], int maxCount)
{
    int count = 0;
    ifstream inFile("HISCORE.DAT");

    if(!inFile)
        return 0;

    while(count < maxCount && inFile >> scores[count])
        count++;

    inFile.close();
    return count;
}

// Inserts the new score in the right place (list is high -> low).
void SaveSystem::saveHighScore(int finalScore)
{
    int scores[MAX_SCORES + 1];
    int count, i, pos;

    count = loadHighScores(scores, MAX_SCORES);

    // find where the new score belongs
    pos = count;
    for(i = count - 1; i >= 0; i--)
        if(finalScore > scores[i])
            pos = i;

    if(pos >= MAX_SCORES)           // not good enough for the top 5
        return;

    if(count < MAX_SCORES)
        count++;

    for(i = count - 1; i > pos; i--) // move lower scores down one place
        scores[i] = scores[i - 1];
    scores[pos] = finalScore;

    ofstream outFile("HISCORE.DAT");
    for(i = 0; i < count; i++)
        outFile << scores[i] << "\n";
    outFile.close();
}

int SaveSystem::loadHighScore()
{
    int scores[MAX_SCORES];
    if(loadHighScores(scores, MAX_SCORES) == 0)
        return 0;
    return scores[0];
}
