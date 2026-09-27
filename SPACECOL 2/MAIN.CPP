// ================================================================
//             SPACE COLONY SURVIVAL  -  10 DAY EDITION
//          SY OOP Project  |  Turbo C++ / Borland C++ + BGI
// ================================================================
#include "GAME.H"
#include <stdlib.h>
#include <time.h>

// A bigger stack, because the graphics screens use local char arrays.
#ifdef __TURBOC__
extern unsigned _stklen = 16384U;
#endif

void main()
{
    srand(time(0));      // different random events every game

    Game game;           // object of the Game class (constructor runs here)
    game.run();          // menu -> campaign -> endings
}                        // destructor ~Game() runs here and deletes the crew
