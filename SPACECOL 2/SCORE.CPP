// ================================================================
// SCORE.CPP
// ================================================================
#include "SCORE.H"

int Score::highScore = 0;      // static member definition

Score::Score()
{
    survivalScore = 0;
    missionScore = 0;
}

void Score::addSurvival(int amount)
{
    survivalScore = survivalScore + amount;
}

void Score::addMission(int amount)
{
    missionScore = missionScore + amount;
}

int Score::getTotal()
{
    return survivalScore + missionScore;
}

// OPERATOR OVERLOADING: adds two Score objects part by part.
// Used at the end of every day:  score = score + dayScore;
Score Score::operator+(Score s)
{
    Score result;
    result.survivalScore = survivalScore + s.survivalScore;
    result.missionScore = missionScore + s.missionScore;
    return result;
}

void Score::updateHighScore(int total)
{
    if(total > highScore)
        highScore = total;
}

int Score::getHighScore()
{
    return highScore;
}

// FRIEND FUNCTION: reads the private members directly
int calculateFinalScore(Score &s)
{
    return s.survivalScore + s.missionScore;
}
