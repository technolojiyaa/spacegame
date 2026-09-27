#ifndef SCORE_H
#define SCORE_H

// ================================================================
// SCORE.H
// Demonstrates OPERATOR OVERLOADING, a STATIC MEMBER and a
// FRIEND FUNCTION.
// ================================================================

class Score
{
private:
    int survivalScore;       // points for staying alive
    int missionScore;        // points for mission choices

    static int highScore;    // ONE copy shared by every Score object

public:
    Score();

    void addSurvival(int amount);
    void addMission(int amount);
    int  getTotal();

    // OPERATOR OVERLOADING: total = total + todayScore;
    Score operator+(Score s);

    static void updateHighScore(int total);
    static int  getHighScore();

    // FRIEND FUNCTION: not a member, but may read the private data
    friend int calculateFinalScore(Score &s);
};

#endif
