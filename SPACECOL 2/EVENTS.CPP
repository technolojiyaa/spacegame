// ================================================================
// EVENTS.CPP - the 10 story days (member functions of Game)
//
//  CHAPTER 1  CRASH               Day 1  Crash landing, reactor
//                                 Day 2  Oxygen leak
//  CHAPTER 2  UNKNOWN SIGNAL      Day 3  Unknown signal + object
//                                 Day 4  First contact
//  CHAPTER 3  THE UNKNOWN         Day 5  Abandoned station
//                                 Day 6  Meteor storm
//  CHAPTER 4  COLONY CRISIS       Day 7  Alien attack
//                                 Day 8  Life support failure
//  CHAPTER 5  FINAL TRANSMISSION  Day 9  The Alien Core + array repair
//                                 Day 10 Final transmission + ending
//
// Every event:  story -> choice -> VISUAL INTERACTION -> result
// ================================================================
#include "GAME.H"
#include "UIGRAPH.H"
#include "INTERACT.H"
#include "SOUND.H"
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* Game::dayTitle(int d)
{
    static char titles[11][25] = {
        "", "CRASH LANDING", "OXYGEN LEAK", "UNKNOWN SIGNAL", "FIRST CONTACT",
        "ABANDONED STATION", "METEOR STORM", "ALIEN ATTACK", "SYSTEM FAILURE",
        "THE ALIEN CORE", "FINAL TRANSMISSION"
    };
    if(d < 1 || d > MAX_DAYS)
        return titles[0];
    return titles[d];
}

void Game::runDayEvent()
{
    switch(day)
    {
        case 1:  dayOne();   break;
        case 2:  dayTwo();   break;
        case 3:  dayThree(); break;
        case 4:  dayFour();  break;
        case 5:  dayFive();  break;
        case 6:  daySix();   break;
        case 7:  daySeven(); break;
        case 8:  dayEight(); break;
        case 9:  dayNine();  break;
        case 10: dayTen();   break;
    }
}

// ======================= CHAPTER 1: CRASH =======================

void Game::dayOne()
{
    int choice;
    char lines[3][70] = {
        "ARK-7 has crash-landed on the unknown planet KEPLER-X.",
        "The main reactor is cracked and leaking power.",
        "Alarms are sounding on every deck."
    };
    char options[3][40] = {
        "REPAIR THE REACTOR",
        "SEARCH THE WRECKAGE FIRST",
        "SHUT DOWN THE REACTOR"
    };

    choice = showEventScreen("DAY 1: CRASH LANDING", lines, 3, options, 3);

    if(choice == 1)
    {
        colony.addEnergy(20);
        score.addSurvival(10);
        showRepair("MAIN REACTOR", 65, "ENERGY +20   SCORE +10");
    }
    else if(choice == 2)
    {
        colony.addMaterials(25);
        colony.useEnergy(15);
        score.addMission(5);
        showExplore("SHIP WRECKAGE", NULL,
                    "Spare parts found, but the reactor kept leaking.",
                    "MATERIALS +25   ENERGY -15");
    }
    else
    {
        colony.useEnergy(20);
        showWarning("REACTOR OFFLINE", "The reactor is safe, but the colony lost a lot of power.",
                    "ENERGY -20");
    }
}

void Game::dayTwo()
{
    int choice;
    char text[80];
    char lines[3][70] = {
        "A crack in the main oxygen tank is venting air into space.",
        "Oxygen levels are dropping every minute.",
        "Someone has to fix it now."
    };
    char options[3][40] = {
        "SEAL THE LEAK YOURSELF",
        "SEND ENGINEER JAKE",
        "IGNORE IT"
    };

    choice = showEventScreen("DAY 2: OXYGEN LEAK", lines, 3, options, 3);

    if(choice == 2 && crew[0]->isAlive() == 0)
        choice = 1;                               // Jake cannot help

    if(choice == 1)
    {
        if(colony.getMaterials() >= 10)
        {
            colony.useMaterials(10);
            colony.addOxygen(15);
            showRepair("OXYGEN TANK", 55, "OXYGEN +15   MATERIALS -10");
        }
        else
        {
            colony.addOxygen(5);
            showRepair("OXYGEN TANK", 55, "NO MATERIALS - TAPE FIX ONLY.  OXYGEN +5");
        }
    }
    else if(choice == 2)
    {
        crew[0]->specialAbility(text);            // virtual call
        crew[0]->damage(10);
        colony.addOxygen(20);
        showCrewTask(crew[0]->getName(), crew[0]->getRole(), text,
                     "OXYGEN +20   JAKE HEALTH -10 (BURNS)");
    }
    else
    {
        colony.useOxygen(20);
        showWarning("OXYGEN DROPPING", "The leak kept venting air all day.", "OXYGEN -20");
    }
}

// ======================= CHAPTER 2: UNKNOWN SIGNAL =======================

void Game::dayThree()
{
    int choice, who;
    SpaceObject *device;
    char lines[3][70] = {
        "Sensors pick up a strange pulsing signal.",
        "It comes from an unknown object that landed near the colony.",
        "It hums and glows with a cold blue light."
    };
    char options[3][40] = {
        "OPEN IT",
        "SCAN IT",
        "IGNORE IT"
    };

    choice = showEventScreen("DAY 3: UNKNOWN SIGNAL", lines, 3, options, 3);

    device = new Artifact("PULSING DEVICE");      // dynamic object

    if(choice == 1)
    {
        if(rand() % 2 == 0)
        {
            colony.addMaterials(20);
            showExplore("UNKNOWN OBJECT", device, "Inside you find rare alien metal.",
                        "MATERIALS +20");
        }
        else
        {
            who = randomAliveCrew();
            if(who >= 0)
                crew[who]->damage(15);
            colony.damageHull(5);
            showWarning("ENERGY BURST!", "The object exploded with energy when it was opened.",
                        "HULL -5   ONE CREW MEMBER HEALTH -15");
        }
    }
    else if(choice == 2)
    {
        colony.addResearch(10);
        addCommunication(10);
        score.addMission(5);
        achSignalFound = 1;
        showScanner(device, "+10 RESEARCH   +10 COMM ARRAY   +5 SCORE");
    }
    else
    {
        showNotice("SIGNAL IGNORED", "The signal fades away... but it will be back.", "NO CHANGE");
    }

    delete device;
}

void Game::dayFour()
{
    int choice, strength, hits, damage;
    char text[80], reward[60];
    char lines[3][70] = {
        "A tall glowing creature walks up to the colony fence.",
        "It carries a device just like the one from yesterday.",
        "It raises one hand... and waits."
    };
    char options[3][40] = {
        "TRY TO COMMUNICATE",
        "DEFEND THE COLONY",
        "HIDE AND WAIT"
    };

    choice = showEventScreen("DAY 4: FIRST CONTACT", lines, 3, options, 3);

    if(choice == 1)
    {
        strength = showCommunication("ALIEN FREQUENCY");
        if(strength >= 60)
        {
            achFirstContact = 1;
            addCommunication(15);
            colony.addResearch(5);
            score.addMission(20);
            showResultCard("FIRST CONTACT!", "The creature answers with a friendly signal.",
                           "It shares part of its language with you.",
                           "COMM ARRAY +15   RESEARCH +5   SCORE +20", 1);
        }
        else
        {
            score.addMission(5);
            showResultCard("SIGNAL LOST", "The creature could not understand you.",
                           "It walks back into the dark.", "SCORE +5", 0);
        }
    }
    else if(choice == 2)
    {
        hits = showDefense("ALIEN CONTACT DETECTED", ENEMY_ALIEN, upShield);
        damage = battleDamage(hits, 3);
        colony.damageHull(damage);
        score.addMission(hits * 3);
        sprintf(text, "You hit %d of %d alien drones.", hits, ENEMY_COUNT);
        sprintf(reward, "HULL -%d   SCORE +%d", damage, hits * 3);
        showResultCard(damage == 0 ? "THREAT NEUTRALIZED" : "BASE DAMAGED", text,
                       "The creature is gone. It will remember this.", reward, damage == 0);
    }
    else
    {
        colony.useEnergy(10);
        showNotice("LIGHTS OUT", "You hid in the dark until the creature walked away.",
                   "ENERGY -10");
    }
}

// ======================= CHAPTER 3: THE UNKNOWN =======================

void Game::dayFive()
{
    int choice, r, amount, who;
    SpaceObject *found;
    char result[80], reward[70];
    char lines[3][70] = {
        "On the ridge stands an abandoned research station.",
        "Its lights are off, but machines still hum inside.",
        "It was built by the same aliens. Nobody is home."
    };
    char options[3][40] = {
        "EXPLORE THE STATION",
        "SCAN IT FROM A DISTANCE",
        "STRIP IT FOR PARTS"
    };

    choice = showEventScreen("DAY 5: ABANDONED STATION", lines, 3, options, 3);

    if(choice == 1)
    {
        // A BASE CLASS POINTER can hold any of the three derived objects
        r = rand() % 3;
        if(r == 0)      found = new Artifact("ALIEN DATA CORE");
        else if(r == 1) found = new Alien("GUARD DRONE", 3);
        else            found = new Asteroid("CRYSTAL ORE");

        found->interact(result);                  // runtime polymorphism
        found->applyEffect(colony, reward);
        colony.addResearch(10);
        strcat(reward, "   RESEARCH +10");
        score.addMission(10);
        showExplore("ABANDONED STATION", found, result, reward);
        delete found;
    }
    else if(choice == 2)
    {
        found = new Artifact("STATION CORE");
        amount = 15;
        if(upScanner == 1)
            amount = 25;                          // Alien Scanner bonus
        colony.addResearch(amount);
        if(upScanner == 1)
            sprintf(reward, "RESEARCH +%d  (ALIEN SCANNER BONUS)", amount);
        else
            sprintf(reward, "RESEARCH +%d", amount);
        showScanner(found, reward);
        delete found;
    }
    else
    {
        colony.addMaterials(30);
        who = randomAliveCrew();
        if(who >= 0)
            crew[who]->damage(10);
        showExplore("ABANDONED STATION", NULL, "Your crew pulled out cables, plates and bolts.",
                    "MATERIALS +30   ONE CREW MEMBER HEALTH -10");
    }
}

void Game::daySix()
{
    int choice, hits, damage, who;
    char text[80], reward[60];
    char lines[3][70] = {
        "Radar shows a storm of meteors heading for the colony!",
        "Impact in ten minutes.",
        "The hull will not survive a direct hit for long."
    };
    char options[3][40] = {
        "RAISE SHIELDS AND SHOOT",
        "REINFORCE THE HULL",
        "RISK IT"
    };

    choice = showEventScreen("DAY 6: METEOR STORM", lines, 3, options, 3);

    if(choice == 1)
    {
        colony.useEnergy(10);
        hits = showDefense("METEOR STORM INCOMING", ENEMY_METEOR, upShield);
        damage = battleDamage(hits, 4);
        colony.damageHull(damage);
        score.addMission(hits * 3);
        sprintf(text, "You destroyed %d of %d meteors.", hits, ENEMY_COUNT);
        sprintf(reward, "HULL -%d   ENERGY -10   SCORE +%d", damage, hits * 3);
        showResultCard(damage == 0 ? "STORM SURVIVED" : "METEOR HITS", text,
                       "The storm passes over the colony.", reward, damage == 0);
    }
    else if(choice == 2)
    {
        if(colony.getMaterials() >= 20)
        {
            colony.useMaterials(20);
            colony.damageHull(5);
            showRepair("HULL PLATING", 100 - colony.getHull(),
                       "MATERIALS -20   HULL -5 (ONLY SMALL DAMAGE)");
        }
        else
        {
            colony.damageHull(20);
            showWarning("NO MATERIALS!", "There was nothing to reinforce the hull with.",
                        "HULL -20");
        }
    }
    else
    {
        colony.damageHull(25);
        who = randomAliveCrew();
        if(who >= 0)
            crew[who]->damage(10);
        showWarning("METEOR IMPACT!", "Meteors smashed straight into the colony.",
                    "HULL -25   ONE CREW MEMBER HEALTH -10");
    }
}

// ======================= CHAPTER 4: COLONY CRISIS =======================

void Game::daySeven()
{
    int choice, hits, damage;
    char text[80], reward[70];
    char lines[3][70] = {
        "The alien device has called something from the sky.",
        "Alien ships are diving toward the colony!",
        "This is not the friendly creature from before."
    };
    char options[3][40] = {
        "ACTIVATE THE DEFENSE GRID",
        "SEND TORRES TO LEAD THE FIGHT",
        "EVACUATE THE SECTOR"
    };

    choice = showEventScreen("DAY 7: ALIEN ATTACK", lines, 3, options, 3);

    if(choice == 2 && crew[3]->isAlive() == 0)
        choice = 1;                               // Torres cannot help

    if(choice == 1)
    {
        colony.useEnergy(10);
        hits = showDefense("ALIEN ATTACK", ENEMY_ALIEN, upShield);
        damage = battleDamage(hits, 5);
        colony.damageHull(damage);
        score.addMission(hits * 4);
        sprintf(text, "You shot down %d of %d alien ships.", hits, ENEMY_COUNT);
        sprintf(reward, "HULL -%d   ENERGY -10   SCORE +%d", damage, hits * 4);
        showResultCard(damage == 0 ? "ATTACK REPELLED" : "COLONY DAMAGED", text, "", reward,
                       damage == 0);
    }
    else if(choice == 2)
    {
        crew[3]->specialAbility(text);            // EliteSecurityOfficer's version
        showCrewTask(crew[3]->getName(), crew[3]->getRole(), text, "TORRES TAKES COMMAND");
        hits = showDefense("ALIEN ATTACK", ENEMY_ALIEN, upShield);
        damage = battleDamage(hits, 5) / 2;       // Torres halves the damage
        colony.damageHull(damage);
        crew[3]->damage(15);
        score.addMission(hits * 5 + 10);
        sprintf(text, "Torres and the grid hit %d of %d ships.", hits, ENEMY_COUNT);
        sprintf(reward, "HULL -%d   TORRES HEALTH -15   SCORE +%d", damage, hits * 5 + 10);
        showResultCard("ATTACK REPELLED", text, "", reward, 1);
    }
    else
    {
        colony.useFood(15);
        colony.damageHull(15);
        showWarning("SECTOR EVACUATED", "Everyone is safe, but the aliens wrecked the empty sector.",
                    "FOOD -15   HULL -15");
    }
}

void Game::dayEight()
{
    int choice;
    char lines[3][70] = {
        "Life support is failing. Oxygen keeps dropping.",
        "The science team thinks alien technology could fix it.",
        "Something must be done today."
    };
    char options[3][40] = {
        "RESEARCH AN ALIEN FIX",
        "EMERGENCY REPAIR",
        "BURN FOOD FOR OXYGEN"
    };

    colony.useOxygen(15);
    showWarning("LIFE SUPPORT FAILURE", "The attack damaged life support. Oxygen fell sharply.",
                "OXYGEN -15");

    choice = showEventScreen("DAY 8: SYSTEM FAILURE", lines, 3, options, 3);

    if(choice == 1)
    {
        colony.addResearch(10);
        if(colony.getResearch() >= 30)
        {
            colony.addOxygen(30);
            showResearch("ALIEN LIFE SUPPORT", "Alien crystals now clean the colony air.",
                         "OXYGEN +30   RESEARCH +10");
        }
        else
        {
            colony.addOxygen(10);
            showResearch("ALIEN LIFE SUPPORT", "Not enough research (need 30). Only a small fix.",
                         "OXYGEN +10   RESEARCH +10");
        }
    }
    else if(choice == 2)
    {
        if(colony.getMaterials() >= 20)
        {
            colony.useMaterials(20);
            colony.addOxygen(25);
            showRepair("LIFE SUPPORT", 80, "OXYGEN +25   MATERIALS -20");
        }
        else
        {
            colony.addOxygen(10);
            showRepair("LIFE SUPPORT", 80, "NOT ENOUGH MATERIALS.  OXYGEN +10");
        }
    }
    else
    {
        colony.useFood(20);
        colony.addOxygen(20);
        showResourceWork("EMERGENCY CONVERTER", LIGHTCYAN, "OXYGEN +20   FOOD -20");
    }
}

// ======================= CHAPTER 5: FINAL TRANSMISSION =======================

void Game::dayNine()
{
    int choice, hits;
    SpaceObject *core;
    char text[80];
    char lines[4][70] = {
        "The Communication Array could reach Earth, but it needs power.",
        "Scans show an ALIEN CORE inside a crashed alien ship nearby.",
        "Tomorrow is the last chance to send a signal.",
        "(To ESCAPE instead, you need Energy 30 and Hull 30 tomorrow.)"
    };
    char options[3][40] = {
        "SEND ENGINEER TEAM",
        "SEND SECURITY TEAM",
        "SCAN FOR THE CORE"
    };

    choice = showEventScreen("DAY 9: THE ALIEN CORE", lines, 4, options, 3);

    core = new Artifact("ALIEN CORE");

    if(choice == 1)
    {
        if(crew[0]->isAlive() == 1 || rand() % 2 == 0)
        {
            alienCore = 1;
            crew[0]->damage(15);
            showExplore("ALIEN WRECK", core, "The engineers carefully cut the core free.",
                        "ALIEN CORE FOUND   JAKE HEALTH -15");
        }
        else
            showWarning("CORE NOT FOUND", "Without Jake, the team could not remove the core.",
                        "NO CORE");
    }
    else if(choice == 2)
    {
        hits = showDefense("CORE GUARDIANS", ENEMY_ALIEN, upShield);
        if(hits >= 4)
        {
            alienCore = 1;
            sprintf(text, "You hit %d guardians and grabbed the core.", hits);
            showResultCard("CORE CAPTURED", text, "", "ALIEN CORE FOUND", 1);
        }
        else
        {
            crew[3]->damage(25);
            sprintf(text, "Only %d guardians hit. The team had to run.", hits);
            showResultCard("MISSION FAILED", text, "", "NO CORE   TORRES HEALTH -25", 0);
        }
    }
    else
    {
        if(upScanner == 1)
        {
            alienCore = 1;
            showScanner(core, "CORE LOCATED AND RECOVERED SAFELY");
        }
        else
        {
            showScanner(core, "SCANNER TOO WEAK - THE SIGNAL KEEPS FADING");
            if(rand() % 2 == 0)
            {
                alienCore = 1;
                showNotice("LUCKY FIND", "The crew followed the weak signal and found the core!",
                           "ALIEN CORE FOUND");
            }
            else
                showWarning("CORE NOT FOUND", "Without the Alien Scanner the core stayed hidden.",
                            "NO CORE");
        }
    }

    delete core;

    // repair the Communication Array
    if(alienCore == 1)
    {
        achFixer = 1;
        score.addMission(20);
        showRepair("COMMUNICATION ARRAY", 100 - communication,
                   "ALIEN CORE INSTALLED   COMM ARRAY +40");
        addCommunication(40);
    }
    else
    {
        showRepair("COMMUNICATION ARRAY", 100 - communication,
                   "NO CORE - LOW POWER REPAIR   COMM ARRAY +15");
        addCommunication(15);
    }
}

void Game::dayTen()
{
    int strength;
    char text[80];
    char lines[4][70] = {
        "This is it, Commander. The array is ready to transmit.",
        "First align the signal. Then choose the colony's fate.",
        "",
        ""
    };
    char options[1][40] = { "ALIGN THE SIGNAL" };

    sprintf(lines[3], "ALIEN CORE: %s     COMM ARRAY: %d%%",
            alienCore == 1 ? "YES" : "NO", communication);

    showEventScreen("DAY 10: FINAL TRANSMISSION", lines, 4, options, 1);

    strength = showCommunication("FINAL TRANSMISSION");
    addCommunication(strength / 4);
    sprintf(text, "Signal strength %d%%. Comm Array now at %d%%.", strength, communication);
    showResultCard("ARRAY CHARGED", text, "", "COMM ARRAY BOOSTED", strength >= 50);

    finalDecision();
}

void Game::finalDecision()
{
    int choice, success = 0;
    char endingName[30];
    char lines[5][70] = {
        "Where will this story end, Commander?",
        "",
        "EARTH  : needs the Alien Core and Comm Array 60%",
        "ESCAPE : needs pilot KAI alive, Energy 30 and Hull 30",
        "ALIENS : needs the Alien Core and First Contact or Research 50"
    };
    char options[3][40] = {
        "CONTACT EARTH",
        "ESCAPE THE PLANET",
        "COMMUNICATE WITH THE ALIENS"
    };

    choice = showEventScreen("THE FINAL DECISION", lines, 5, options, 3);

    if(choice == 1)
    {
        if(alienCore == 1 && communication >= 60)
        {
            success = 1;
            strcpy(endingName, "RESCUED BY EARTH");
            showEndingEarth();
        }
        else
            showEndingFailure("TRANSMISSION FAILED", "The signal was too weak to reach Earth.");
    }
    else if(choice == 2)
    {
        if(crew[4]->isAlive() == 1 && colony.getEnergy() >= 30 && colony.getHull() >= 30)
        {
            success = 1;
            strcpy(endingName, "ESCAPED KEPLER-X");
            showEndingEscape();
        }
        else
            showEndingFailure("LAUNCH FAILED", "The damaged ship could not leave the planet.");
    }
    else
    {
        if(alienCore == 1 && (achFirstContact == 1 || colony.getResearch() >= 50))
        {
            success = 1;
            strcpy(endingName, "ALIEN ALLIANCE");
            showEndingAliens();
        }
        else
            showEndingFailure("CONTACT FAILED", "The aliens did not understand your message.");
    }

    if(success == 1)
    {
        score.addMission(100);
        if(aliveCrew() == CREW_SIZE)
            achSurvivor = 1;
        if(colony.getHull() >= 50)
            achColonySavior = 1;
    }
    else
    {
        score.addMission(20);
        strcpy(endingName, "LOST IN SILENCE");
    }

    gameOver = 1;
    showFinalStats(endingName, success);
}
