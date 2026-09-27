#ifndef SAVESYS_H
#define SAVESYS_H

// ================================================================
// SAVESYS.H - FILE HANDLING
// SAVE.DAT     : the saved game
// HISCORE.DAT  : the top 5 scores
//
// SaveSystem is declared as a FRIEND CLASS inside Game, Colony and
// CrewMember, so it can read and write their private data directly.
// All its functions are STATIC: we call them without an object,
// e.g.  SaveSystem::saveGame(*this);
// ================================================================

class Game;

#define MAX_SCORES 5

class SaveSystem
{
public:
    static void saveGame(Game &game);
    static int  loadGame(Game &game);          // returns 1 if loaded

    static void saveHighScore(int finalScore);
    static int  loadHighScores(int scores[], int maxCount);   // returns count
    static int  loadHighScore();                              // best score
};

#endif
