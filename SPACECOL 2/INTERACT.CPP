// ================================================================
// INTERACT.CPP - visual interaction screens for every action
//
// Pattern used by every screen:
//   1. draw the scene          (machine / radar / station ...)
//   2. show a keyboard button  ([ENTER] SCAN, [ENTER] WELD ...)
//   3. wait for the player
//   4. play a short animation  (with sound)
//   5. show the result card
//
// Animation trick used here: setwritemode(XOR_PUT).
// A line drawn twice in XOR mode disappears and the picture under
// it comes back. That is how the scanner beam, sparks and laser move
// without destroying the drawing behind them.
// ================================================================
#include "INTERACT.H"
#include "UIGRAPH.H"
#include "SOUND.H"
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159

// ======================= CRASH INTRO =======================

void drawFallingShip(int x, int y, int show)
{
    setfillstyle(SOLID_FILL, BLACK);
    if(show == 0)
    {
        bar(x - 38, y - 16, x + 18, y + 10);
        return;
    }
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(x, y, 15, 6);                 // ship body
    setcolor(YELLOW);                         // fire trail
    line(x - 15, y, x - 32, y - 10);
    line(x - 14, y + 3, x - 28, y - 4);
    setcolor(LIGHTRED);
    line(x - 14, y - 3, x - 36, y - 14);
}

void showCrashIntro()
{
    int i, x, y;

    cleardevice();
    drawStarField();
    drawBorder();

    setcolor(BROWN);                          // the planet surface
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(320, 650, 470, 250);
    setcolor(DARKGRAY);
    ellipse(200, 430, 0, 360, 30, 7);
    ellipse(480, 445, 0, 360, 40, 9);

    typeText(30, 24, "YEAR 2187. COLONY SHIP ARK-7 ENTERS AN UNKNOWN SYSTEM...", LIGHTCYAN);
    typeText(30, 42, "ENGINE FAILURE! ARK-7 IS FALLING TOWARD PLANET KEPLER-X!", LIGHTRED);

    x = 40;
    y = 80;
    for(i = 0; i < 30; i++)                   // the ship falls
    {
        drawFallingShip(x, y, 1);
        playTone(900 - i * 25, 30);
        drawFallingShip(x, y, 0);
        x = x + 11;
        y = y + 10;
    }

    for(i = 4; i <= 90; i = i + 6)            // impact explosion
    {
        if(i % 12 == 4)
            setcolor(YELLOW);
        else
            setcolor(LIGHTRED);
        circle(x, y + 15, i);
        playTone(80 + rand() % 120, 20);
    }

    for(i = 0; i < 6; i++)                    // alarm
    {
        drawBorder(i % 2 == 0 ? LIGHTRED : RED);
        drawTitle("!! CRASH LANDING !!", 110, i % 2 == 0 ? YELLOW : LIGHTRED, 3);
        playTone(i % 2 == 0 ? 880 : 440, 150);
    }

    drawPanel(50, 160, 589, 300, "SHIP COMPUTER");
    typeText(70, 180, "Hull breach. Reactor damaged. Oxygen leaking.", WHITE);
    typeText(70, 200, "You are the COMMANDER of the surviving colony.", WHITE);
    typeText(70, 220, "Keep your 5 crew members alive for 10 days,", WHITE);
    typeText(70, 240, "repair the Communication Array and call for help.", WHITE);
    typeText(70, 270, "GOOD LUCK, COMMANDER.", YELLOW);
    waitForEnter(440);
}

// ======================= SCANNER =======================

void drawRadar(int cx, int cy, int r)
{
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(cx, cy, r, r);
    circle(cx, cy, r * 2 / 3);
    circle(cx, cy, r / 3);
    line(cx - r, cy, cx + r, cy);
    line(cx, cy - r, cx, cy + r);
    setcolor(LIGHTGREEN);
    circle(cx, cy, r + 3);
}

void revealField(int y, char text[], int color)
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(400, y - 2, 600, y + 10);
    setcolor(color);
    outtextxy(400, y, text);
}

void showScanner(SpaceObject *obj, char reward[])
{
    int percent, angle, x2, y2, sy, threatColor;
    int cx = 165, cy = 190, r = 115;
    char text[80];

    drawScreenFrame("SCANNER");
    drawRadar(cx, cy, r);
    obj->draw(cx, cy);                        // POLYMORPHISM: the object draws itself

    drawPanel(310, 62, 609, 318, "SCAN DATA");
    setcolor(LIGHTCYAN);
    outtextxy(325, 88, "OBJECT :");
    outtextxy(325, 112, "TYPE   :");
    outtextxy(325, 136, "THREAT :");
    revealField(88, "??????", DARKGRAY);
    revealField(112, "??????", DARKGRAY);
    revealField(136, "??????", DARKGRAY);

    setcolor(LIGHTCYAN);
    outtextxy(325, 170, "SCAN PROGRESS");
    drawProgressBar(325, 186, 200, 0, LIGHTCYAN);
    drawButton(345, 240, 230, "[ENTER] START SCAN", 1);
    centerText("TARGET LOCKED.  PRESS ENTER TO START THE SCAN.", 350, YELLOW);

    waitEnterKey();
    drawButton(345, 240, 230, "SCANNING...", 0);

    angle = 0;
    for(percent = 0; percent <= 100; percent = percent + 2)
    {
        x2 = cx + (int)(r * cos(angle * PI / 180.0));
        y2 = cy + (int)(r * sin(angle * PI / 180.0));
        sy = cy - 45 + (percent % 25) * 4;

        setwritemode(XOR_PUT);                // draw beam + scan line
        setcolor(LIGHTGREEN);
        line(cx, cy, x2, y2);
        setcolor(LIGHTCYAN);
        line(cx - 45, sy, cx + 45, sy);
        delay(35);
        setcolor(LIGHTGREEN);                 // draw again = erase
        line(cx, cy, x2, y2);
        setcolor(LIGHTCYAN);
        line(cx - 45, sy, cx + 45, sy);
        setwritemode(COPY_PUT);

        angle = angle + 15;

        if(percent % 10 == 0)
        {
            drawProgressBar(325, 186, 200, percent, LIGHTCYAN);
            playScanSound();
        }
        if(percent == 40)
            revealField(88, obj->getName(), WHITE);
        if(percent == 70)
            revealField(112, obj->getType(), WHITE);
        if(percent == 100)
        {
            threatColor = LIGHTGREEN;
            if(obj->getThreat() == 2) threatColor = YELLOW;
            if(obj->getThreat() >= 3) threatColor = LIGHTRED;
            revealField(136, obj->getThreatText(), threatColor);
        }
    }

    drawButton(345, 240, 230, "SCAN COMPLETE", 1);
    obj->interact(text);                      // POLYMORPHISM again
    showResultCard("SCAN COMPLETE", text, "NEW INFORMATION ADDED TO THE DATABASE", reward, 1);
}

// ======================= REPAIR =======================

// The crack across the machine, as offsets from the machine centre
int crackDX[7] = { -70, -45, -22, 0, 22, 45, 70 };
int crackDY[7] = { 5, 22, 8, 28, 12, 30, 15 };

void drawMachine(int x, int y)
{
    int i;

    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x - 95, y - 80, x + 95, y + 80);
    rectangle(x - 95, y - 80, x + 95, y + 80);

    setfillstyle(SOLID_FILL, BLUE);           // pipes
    bar(x - 125, y - 50, x - 96, y - 38);
    bar(x + 96, y + 40, x + 125, y + 52);

    setcolor(LIGHTGRAY);                      // air vent
    for(i = 0; i < 5; i++)
        line(x - 70, y + 48 + i * 6, x + 70, y + 48 + i * 6);
}

void drawGauges(int x, int y, int percent)
{
    int i, gx, color;
    double a;

    color = LIGHTGREEN;
    if(percent < 50)
        color = LIGHTRED;

    for(i = 0; i < 2; i++)
    {
        gx = x - 45 + i * 90;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(gx, y - 40, 22, 22);
        a = (180 - percent * 180 / 100) * PI / 180.0;   // needle angle
        setcolor(color);
        line(gx, y - 40, gx + (int)(18 * cos(a)), y - 40 - (int)(18 * sin(a)));
    }

    setcolor(color);                          // warning lamp
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y - 66, 6, 6);
}

void drawCrack(int x, int y, int segments, int color)
{
    int i;
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    setcolor(color);
    for(i = 0; i < segments && i < 6; i++)
        line(x + crackDX[i], y + crackDY[i], x + crackDX[i + 1], y + crackDY[i + 1]);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void showSparks(int x, int y)
{
    int i, k, ex[8], ey[8];

    setwritemode(XOR_PUT);
    for(k = 0; k < 3; k++)
    {
        if(k % 2 == 0) setcolor(YELLOW);
        else setcolor(WHITE);

        for(i = 0; i < 8; i++)
        {
            ex[i] = x - 16 + rand() % 33;
            ey[i] = y - 16 + rand() % 33;
            line(x, y, ex[i], ey[i]);
        }
        playRepairSound();
        for(i = 0; i < 8; i++)                // same lines again = erase
            line(x, y, ex[i], ey[i]);
    }
    setwritemode(COPY_PUT);
}

void showRepairStatus(int integrity)
{
    char text[30];

    setfillstyle(SOLID_FILL, BLACK);
    bar(345, 98, 600, 112);
    bar(345, 196, 600, 210);

    sprintf(text, "DAMAGE : %d%%", 100 - integrity);
    setcolor(integrity < 50 ? LIGHTRED : YELLOW);
    outtextxy(345, 102, text);

    setcolor(LIGHTCYAN);
    outtextxy(345, 140, "INTEGRITY");
    drawProgressBar(345, 158, 180, integrity);

    if(integrity >= 100)      { setcolor(LIGHTGREEN); outtextxy(345, 200, "STATUS : ONLINE"); }
    else if(integrity >= 60)  { setcolor(YELLOW);     outtextxy(345, 200, "STATUS : UNSTABLE"); }
    else                      { setcolor(LIGHTRED);   outtextxy(345, 200, "STATUS : CRITICAL"); }
}

void showRepair(char systemName[], int damage, char reward[])
{
    int integrity, start, fixed, key;
    int cx = 170, cy = 200;

    if(damage < 20) damage = 20;
    if(damage > 90) damage = 90;
    integrity = 100 - damage;
    start = integrity;

    drawScreenFrame("REPAIR BAY");
    drawTitle(systemName, 50, LIGHTCYAN, 2);
    drawMachine(cx, cy);
    drawGauges(cx, cy, integrity);
    drawCrack(cx, cy, 6, LIGHTRED);

    drawPanel(330, 80, 609, 310, "SYSTEM STATUS");
    showRepairStatus(integrity);
    drawButton(360, 250, 220, "[ENTER] WELD", 1);
    centerText("TAP ENTER AGAIN AND AGAIN TO WELD THE DAMAGE CLOSED", 350, YELLOW);

    while(integrity < 100)
    {
        key = readKey();
        if(key == KEY_ENTER || key == KEY_SPACE)
        {
            fixed = (integrity - start) * 6 / (100 - start);
            drawButton(360, 250, 220, "[ENTER] WELD", 0);
            showSparks(cx + crackDX[fixed], cy + crackDY[fixed]);
            drawButton(360, 250, 220, "[ENTER] WELD", 1);

            integrity = integrity + 8 + rand() % 8;
            if(integrity > 100)
                integrity = 100;

            fixed = (integrity - start) * 6 / (100 - start);
            drawCrack(cx, cy, fixed, LIGHTGREEN);    // welded part turns green
            drawGauges(cx, cy, integrity);
            showRepairStatus(integrity);
        }
    }

    drawButton(360, 250, 220, "SYSTEM ONLINE", 1);
    showResultCard("REPAIR COMPLETE", "The system is back online.", "", reward, 1);
}

// ======================= EXPLORE =======================

void drawRover(int x, int y, int show)
{
    setfillstyle(SOLID_FILL, BLACK);
    if(show == 0)
    {
        bar(x - 26, y - 26, x + 26, y + 10);
        return;
    }
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x - 20, y - 12, x + 20, y);
    rectangle(x - 20, y - 12, x + 20, y);
    setfillstyle(SOLID_FILL, CYAN);
    bar(x + 6, y - 10, x + 16, y - 4);        // window
    setcolor(LIGHTGRAY);
    line(x - 12, y - 12, x - 16, y - 22);     // antenna
    circle(x - 16, y - 23, 2);
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(x - 13, y + 4, 5, 5);         // wheels
    fillellipse(x, y + 4, 5, 5);
    fillellipse(x + 13, y + 4, 5, 5);
}

void drawStructure(int x)
{
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    pieslice(x, 300, 0, 180, 80);             // main dome
    bar(x - 140, 250, x - 80, 300);           // side modules
    rectangle(x - 140, 250, x - 80, 300);
    bar(x + 80, 262, x + 130, 300);
    rectangle(x + 80, 262, x + 130, 300);

    setfillstyle(SOLID_FILL, YELLOW);         // one window still lit
    bar(x - 30, 262, x - 18, 272);
    setfillstyle(SOLID_FILL, BLACK);
    bar(x + 12, 262, x + 24, 272);
    bar(x - 12, 278, x + 12, 300);            // doorway

    setcolor(LIGHTGRAY);
    line(x + 30, 228, x + 55, 180);           // antenna
    circle(x + 55, 176, 5);
    setcolor(LIGHTRED);                       // damage marks
    line(x - 60, 240, x - 40, 250);
    line(x + 40, 236, x + 60, 246);
}

void drawCrate(int x, int y)
{
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, BROWN);
    bar(x - 30, y - 22, x + 30, y + 22);
    rectangle(x - 30, y - 22, x + 30, y + 22);
    line(x - 30, y - 22, x + 30, y + 22);
    line(x - 30, y + 22, x + 30, y - 22);
}

void showExplore(char placeName[], SpaceObject *found, char resultText[], char reward[])
{
    int x, k, good = 1;
    char text[80];

    drawScreenFrame("EXPLORATION");
    drawTitle(placeName, 50, LIGHTCYAN, 2);
    drawPlanet(110, 120, 22, DARKGRAY);

    setfillstyle(SOLID_FILL, BROWN);           // ground
    bar(12, 301, 627, 325);
    drawStructure(440);
    drawRover(60, 290, 1);

    drawButton(215, 345, 210, "[ENTER] ENTER AREA", 1);
    centerText("THE ROVER IS READY TO EXPLORE", 385, YELLOW);
    waitEnterKey();

    for(x = 60; x < 282; x = x + 6)            // rover drives to the door
    {
        drawRover(x, 290, 0);
        drawRover(x + 6, 290, 1);
        playTone(90 + (x % 12) * 5, 20);
        delay(15);
    }

    for(k = 1; k <= 9; k++)                    // zoom into the building
    {
        setcolor(k % 2 == 0 ? CYAN : BLUE);
        rectangle(320 - k * 32, 180 - k * 14, 320 + k * 32, 180 + k * 14);
        playTone(200 + k * 60, 30);
    }

    setfillstyle(SOLID_FILL, BLACK);           // inside: a corridor
    bar(12, 42, 627, 328);
    setcolor(BLUE);
    rectangle(250, 120, 390, 230);
    line(12, 42, 250, 120);
    line(627, 42, 390, 120);
    line(12, 328, 250, 230);
    line(627, 328, 390, 230);

    centerText("SEARCHING THE AREA...", 60, LIGHTGREEN);
    for(k = 0; k <= 100; k = k + 10)
    {
        drawProgressBar(220, 300, 200, k, LIGHTGREEN);
        playTone(1000 + k * 8, 25);
        delay(40);
    }

    setfillstyle(SOLID_FILL, BLACK);
    bar(252, 122, 388, 228);
    if(found != NULL)
    {
        found->draw(320, 175);                 // POLYMORPHISM
        sprintf(text, "FOUND: %s  (%s)", found->getName(), found->getType());
        if(found->getThreat() >= 3)
            good = 0;
    }
    else
    {
        drawCrate(320, 175);
        strcpy(text, "FOUND: SUPPLY CRATE");
    }

    setfillstyle(SOLID_FILL, BLACK);
    bar(60, 244, 580, 258);
    centerText(text, 247, good == 1 ? LIGHTGREEN : LIGHTRED);

    if(good == 0)
    {
        playWarningSound();
        showResultCard("HOSTILE CONTACT!", resultText, "", reward, 0);
    }
    else
        showResultCard("AREA EXPLORED", resultText, "", reward, 1);
}

// ======================= RESEARCH =======================

void showResearch(char topic[], char resultText[], char reward[])
{
    int p, i;
    char rows[6][50];
    char words[5][10] = { "DNA", "ENERGY", "METAL", "SIGNAL", "CRYSTAL" };

    drawScreenFrame("RESEARCH TERMINAL");
    drawPanel(30, 60, 609, 318, "LAB COMPUTER");
    typeText(50, 82, "> SAMPLE LOADED :", LIGHTGREEN);
    typeText(198, 82, topic, WHITE);
    typeText(50, 100, "> AWAITING ANALYSIS COMMAND...", LIGHTGREEN);
    drawButton(210, 272, 220, "[ENTER] ANALYZE", 1);
    centerText("PRESS ENTER TO START THE ANALYSIS", 350, YELLOW);
    waitEnterKey();
    drawButton(210, 272, 220, "ANALYZING...", 0);

    for(i = 0; i < 6; i++)
        strcpy(rows[i], "");

    for(p = 0; p <= 100; p = p + 4)
    {
        for(i = 0; i < 5; i++)                  // scroll the data up
            strcpy(rows[i], rows[i + 1]);
        sprintf(rows[5], "%03X %03X %03X %03X   %s", rand() % 4096, rand() % 4096,
                rand() % 4096, rand() % 4096, words[rand() % 5]);

        setfillstyle(SOLID_FILL, BLACK);
        bar(48, 120, 592, 232);
        for(i = 0; i < 6; i++)
        {
            setcolor(i == 5 ? LIGHTGREEN : GREEN);
            outtextxy(50, 124 + i * 18, rows[i]);
        }

        setcolor(LIGHTCYAN);
        outtextxy(50, 246, "ANALYZING");
        drawProgressBar(140, 244, 300, p, LIGHTGREEN);
        playTone(1200 + rand() % 800, 12);
        delay(30);
    }

    drawButton(210, 272, 220, "ANALYSIS COMPLETE", 1);
    showResultCard("NEW DATA DECODED", resultText, "", reward, 1);
}

void drawChip(int x, int y)
{
    int i;
    setcolor(LIGHTGRAY);
    for(i = -30; i <= 30; i = i + 15)          // pins
    {
        line(x - 55, y + i, x + 55, y + i);
        line(x + i, y - 55, x + i, y + 55);
    }
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x - 40, y - 40, x + 40, y + 40);
    rectangle(x - 40, y - 40, x + 40, y + 40);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(x - 15, y - 15, x + 15, y + 15);
}

void showUnlock(char techName[], char description[])
{
    int i;

    drawScreenFrame("RESEARCH BREAKTHROUGH");
    drawChip(320, 130);

    for(i = 0; i < 6; i++)
    {
        drawTitle("NEW TECHNOLOGY UNLOCKED", 215, i % 2 == 0 ? LIGHTGREEN : YELLOW, 2);
        playTone(600 + i * 150, 80);
        delay(60);
    }
    drawTitle(techName, 255, LIGHTCYAN, 3);
    centerText(description, 305, WHITE);
    playSuccessSound();
    waitForEnter(430);
}

// ======================= CREW TASK =======================

void drawAstronaut(int x, int y)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x - 44, y + 34, x - 30, y + 82);      // backpack
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x - 30, y + 30, x + 30, y + 95);      // suit
    rectangle(x - 30, y + 30, x + 30, y + 95);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y, 36, 36);                // helmet
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y + 2, 24, 16);            // visor
    setcolor(LIGHTCYAN);
    arc(x - 6, y - 2, 100, 170, 14);          // shine
    setfillstyle(SOLID_FILL, LIGHTRED);
    bar(x - 10, y + 50, x + 10, y + 60);      // badge
}

void showCrewTask(char name[], char role[], char resultText[], char reward[])
{
    int p, step = 0;

    drawScreenFrame("CREW ASSIGNMENT");
    drawPanel(30, 60, 609, 318, "CREW MEMBER");
    drawAstronaut(140, 130);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(250, 95, name);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(LIGHTCYAN);
    outtextxy(250, 132, role);
    setcolor(LIGHTGRAY);
    outtextxy(250, 160, "SPECIAL ABILITY READY");

    drawButton(260, 270, 240, "[ENTER] ASSIGN TASK", 1);
    centerText("PRESS ENTER TO SEND THIS CREW MEMBER", 350, YELLOW);
    waitEnterKey();
    drawButton(260, 270, 240, "WORKING...", 0);

    setcolor(YELLOW);
    outtextxy(250, 200, "TASK IN PROGRESS");
    for(p = 0; p <= 100; p = p + 10)
    {
        drawProgressBar(250, 220, 250, p, LIGHTGREEN);
        playTone(step == 0 ? 150 : 200, 40);      // footsteps
        step = 1 - step;
        delay(60);
    }

    drawButton(260, 270, 240, "TASK COMPLETE", 1);
    showResultCard("TASK COMPLETE", resultText, "", reward, 1);
}

// ======================= RESOURCE PRODUCTION =======================

void showResourceWork(char task[], int color, char reward[])
{
    int level, i;

    drawScreenFrame("RESOURCE CONTROL");
    drawTitle(task, 50, LIGHTCYAN, 2);

    setfillstyle(SOLID_FILL, BLUE);            // pump
    bar(100, 220, 170, 300);
    setcolor(LIGHTCYAN);
    rectangle(100, 220, 170, 300);
    setfillstyle(SOLID_FILL, DARKGRAY);        // pipes
    bar(170, 240, 240, 252);
    bar(400, 270, 440, 282);

    setcolor(LIGHTGRAY);                       // tank
    rectangle(240, 90, 400, 300);
    rectangle(243, 93, 397, 297);

    drawPanel(440, 90, 609, 200, "OUTPUT");
    drawProgressBar(455, 130, 100, 0, color);

    drawButton(210, 345, 220, "[ENTER] START", 1);
    centerText("PRESS ENTER TO START PRODUCTION", 385, YELLOW);
    waitEnterKey();
    drawButton(210, 345, 220, "RUNNING...", 0);

    for(level = 0; level <= 100; level = level + 5)
    {
        setfillstyle(SOLID_FILL, color);
        bar(246, 294 - level * 2, 394, 294);
        for(i = 0; i < 4; i++)                  // bubbles
            putpixel(250 + rand() % 140, 294 - rand() % (level * 2 + 1), WHITE);
        drawProgressBar(455, 130, 100, level, color);
        playTone(300 + level * 6, 25);
        delay(40);
    }

    showResultCard("PRODUCTION COMPLETE", task, "", reward, 1);
}

// ======================= DEFENSE =======================

int laneX(int lane)
{
    return 120 + lane * 100;
}

void drawEnemy(int x, int y, int type, int show)
{
    setfillstyle(SOLID_FILL, BLACK);
    if(show == 0)
    {
        bar(x - 17, y - 13, x + 17, y + 13);
        return;
    }

    if(type == ENEMY_ALIEN)                    // small flying saucer
    {
        setcolor(LIGHTGREEN);
        setfillstyle(SOLID_FILL, GREEN);
        fillellipse(x, y + 3, 15, 6);
        setcolor(LIGHTCYAN);
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        fillellipse(x, y - 3, 6, 5);
        putpixel(x - 8, y + 3, YELLOW);
        putpixel(x + 8, y + 3, YELLOW);
    }
    else                                        // burning meteor
    {
        setcolor(YELLOW);
        line(x - 3, y - 9, x - 7, y - 13);
        line(x, y - 9, x, y - 13);
        line(x + 3, y - 9, x + 7, y - 13);
        setcolor(LIGHTGRAY);
        setfillstyle(SOLID_FILL, BROWN);
        fillellipse(x, y, 10, 9);
    }
}

void drawTurret(int x, int show)
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(x - 16, 382, x + 16, 400);
    if(show == 1)
    {
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(x - 12, 392, x + 12, 400);
        bar(x - 3, 382, x + 3, 392);
        setcolor(LIGHTCYAN);
        rectangle(x - 12, 392, x + 12, 400);
    }
}

void drawColonyBase(int x, int y)
{
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x - 290, y, x + 290, y + 14);          // ground
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, BLUE);
    pieslice(x - 160, y, 0, 180, 36);          // three domes
    pieslice(x, y, 0, 180, 50);
    pieslice(x + 160, y, 0, 180, 36);
    setfillstyle(SOLID_FILL, YELLOW);          // windows
    bar(x - 8, y - 26, x + 8, y - 16);
    bar(x - 166, y - 16, x - 154, y - 8);
    bar(x + 154, y - 16, x + 166, y - 8);
}

void drawDefenseHud(int hits, int breaches)
{
    char text[60];
    setfillstyle(SOLID_FILL, BLACK);
    bar(14, 44, 625, 58);
    sprintf(text, "TARGETS HIT: %d    BREACHES: %d", hits, breaches);
    setcolor(WHITE);
    outtextxy(24, 47, text);
    setcolor(LIGHTGRAY);
    outtextxy(390, 47, "[A][D] MOVE  [SPACE] FIRE");
}

void fireLaser(int x, int topY)
{
    setwritemode(XOR_PUT);
    setcolor(LIGHTRED);
    line(x, 381, x, topY);
    line(x + 1, 381, x + 1, topY);
    playLaserSound();
    line(x, 381, x, topY);                     // same lines again = erase
    line(x + 1, 381, x + 1, topY);
    setwritemode(COPY_PUT);
}

void showExplosion(int x, int y)
{
    int r;
    for(r = 3; r <= 15; r = r + 3)
    {
        setcolor(r % 2 == 0 ? YELLOW : LIGHTRED);
        circle(x, y, r);
        playTone(100 + r * 10, 12);
    }
    setfillstyle(SOLID_FILL, BLACK);
    bar(x - 17, y - 16, x + 17, y + 16);
}

// A small shooting mini-game. Returns how many of the 6 targets were hit.
int showDefense(char title[], int enemyType, int shieldOn)
{
    int ex[ENEMY_COUNT], ey[ENEMY_COUNT], active[ENEMY_COUNT];
    int i, key, p, target, speed;
    int lane = 2, hits = 0, breaches = 0, left = ENEMY_COUNT;

    // ---------- 1. alert screen ----------
    drawScreenFrame("DEFENSE GRID");
    drawTitle(title, 55, LIGHTRED, 3);
    drawColonyBase(320, 300);
    for(i = 0; i < 3; i++)
    {
        drawEnemy(200 + i * 120, 125, enemyType, 1);
        setcolor(LIGHTRED);
        setlinestyle(DASHED_LINE, 0, NORM_WIDTH);
        line(200 + i * 120, 140, 320 + (i - 1) * 60, 240);
        setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    }
    centerText("CONTROLS:  [A] LEFT   [D] RIGHT   [SPACE] FIRE LASER", 335, YELLOW);
    drawButton(200, 360, 240, "[ENTER] ACTIVATE DEFENSE", 1);
    playWarningSound();
    waitEnterKey();

    // ---------- 2. charging ----------
    setfillstyle(SOLID_FILL, BLACK);
    bar(14, 328, 625, 468);
    centerText("DEFENSE SYSTEMS CHARGING...", 345, LIGHTCYAN);
    for(p = 0; p <= 100; p = p + 10)
    {
        drawProgressBar(210, 368, 200, p, LIGHTCYAN);
        playTone(300 + p * 8, 35);
    }
    centerText("DEFENSE ONLINE", 400, LIGHTGREEN);
    playSuccessSound();
    delay(300);

    // ---------- 3. battle ----------
    drawScreenFrame("DEFENSE GRID");
    drawColonyBase(320, 455);
    setcolor(DARKGRAY);
    for(i = 0; i < 5; i++)
        line(laneX(i) - 6, 378, laneX(i) + 6, 378);
    drawDefenseHud(0, 0);

    speed = 3;
    if(shieldOn == 1)
        speed = 2;                             // Plasma Shield slows enemies

    for(i = 0; i < ENEMY_COUNT; i++)
    {
        ex[i] = laneX(rand() % 5);
        ey[i] = 70 - i * 70;                   // start one after another
        active[i] = 1;
    }
    drawTurret(laneX(lane), 1);

    while(left > 0)
    {
        // move every enemy down
        for(i = 0; i < ENEMY_COUNT; i++)
        {
            if(active[i] == 0)
                continue;
            if(ey[i] >= 70)
                drawEnemy(ex[i], ey[i], enemyType, 0);
            ey[i] = ey[i] + speed;

            if(ey[i] >= 360)                    // reached the colony
            {
                active[i] = 0;
                left--;
                breaches++;
                drawBorder(LIGHTRED);
                playErrorSound();
                drawBorder();
                drawDefenseHud(hits, breaches);
            }
            else if(ey[i] >= 70)
                drawEnemy(ex[i], ey[i], enemyType, 1);
        }

        // read the keyboard without waiting
        while(kbhit())
        {
            key = readKey();
            if((key == 'a' || key == 'A' || key == KEY_LEFT) && lane > 0)
            {
                drawTurret(laneX(lane), 0);
                lane--;
                drawTurret(laneX(lane), 1);
            }
            else if((key == 'd' || key == 'D' || key == KEY_RIGHT) && lane < 4)
            {
                drawTurret(laneX(lane), 0);
                lane++;
                drawTurret(laneX(lane), 1);
            }
            else if(key == KEY_SPACE || key == KEY_ENTER)
            {
                target = -1;                    // lowest enemy in this lane
                for(i = 0; i < ENEMY_COUNT; i++)
                    if(active[i] == 1 && ex[i] == laneX(lane) && ey[i] >= 70 &&
                       (target == -1 || ey[i] > ey[target]))
                        target = i;

                if(target == -1)
                    fireLaser(laneX(lane), 62);
                else
                {
                    fireLaser(laneX(lane), ey[target]);
                    drawEnemy(ex[target], ey[target], enemyType, 0);
                    showExplosion(ex[target], ey[target]);
                    active[target] = 0;
                    left--;
                    hits++;
                    drawDefenseHud(hits, breaches);
                }
            }
        }
        delay(35);
    }

    if(breaches == 0)
        drawTitle("ALL TARGETS DESTROYED", 200, LIGHTGREEN, 2);
    else
        drawTitle("ATTACK OVER", 200, YELLOW, 2);
    delay(600);
    return hits;
}

// ======================= COMMUNICATION ARRAY =======================

void drawTower(int x, int baseY)
{
    int i, y, w, lastW;
    int top = baseY - 170;

    setcolor(LIGHTGRAY);
    line(x - 30, baseY, x - 6, top);           // two legs
    line(x + 30, baseY, x + 6, top);

    lastW = 30;
    for(i = 1; i <= 5; i++)                    // cross braces
    {
        y = baseY - i * 28;
        w = 30 - (24 * i * 28) / 170;
        line(x - w, y, x + w, y);
        line(x - lastW, y + 28, x + w, y);
        lastW = w;
    }

    setfillstyle(SOLID_FILL, LIGHTGRAY);       // dish
    fillellipse(x, top - 6, 26, 8);
    line(x, top - 14, x, top - 34);            // antenna
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x - 40, baseY, x + 40, baseY + 8);     // foot
}

void drawSignalWaves(int x, int y, int count)
{
    int i;
    for(i = 1; i <= 4; i++)
    {
        if(i <= count)
            setcolor(LIGHTGREEN);
        else
            setcolor(BLACK);
        arc(x, y, 0, 40, 10 + i * 10);
        arc(x, y, 320, 360, 10 + i * 10);
        arc(x, y, 140, 220, 10 + i * 10);
    }
}

// Oscilloscope: a clean big wave = strong signal, noisy small wave = weak
void drawWave(int strength, int phase)
{
    int k, px, py, lastX, lastY, amp, noise;

    setfillstyle(SOLID_FILL, BLACK);
    bar(272, 98, 592, 192);
    setcolor(DARKGRAY);
    line(272, 145, 592, 145);

    amp = 6 + strength * 36 / 100;
    noise = (100 - strength) / 3;

    setcolor(LIGHTRED);
    if(strength >= 40) setcolor(YELLOW);
    if(strength >= 70) setcolor(LIGHTGREEN);

    lastX = 272;
    lastY = 145;
    for(k = 0; k <= 40; k++)
    {
        px = 272 + k * 8;
        py = 145 + (int)(amp * sin((k + phase) * 0.5));
        if(noise > 0)
            py = py + rand() % (noise + 1) - noise / 2;
        if(py < 99)  py = 99;
        if(py > 191) py = 191;
        line(lastX, lastY, px, py);
        lastX = px;
        lastY = py;
    }
}

void drawTuner(int pos)
{
    int x, tri[8];
    char text[30];

    x = 290 + pos * 29 / 10;                   // 0..100 -> 290..580
    setfillstyle(SOLID_FILL, BLACK);
    bar(282, 268, 592, 290);

    tri[0] = x;      tri[1] = 270;
    tri[2] = x - 6;  tri[3] = 284;
    tri[4] = x + 6;  tri[5] = 284;
    tri[6] = x;      tri[7] = 270;
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillpoly(4, tri);

    sprintf(text, "FREQ %d.%d MHz", 140 + pos / 2, (pos % 2) * 5);
    setfillstyle(SOLID_FILL, BLACK);
    bar(440, 244, 600, 256);
    setcolor(WHITE);
    outtextxy(440, 246, text);
}

// Tuning mini-game. Returns the final signal strength (0..100).
int showCommunication(char title[])
{
    int i, target, pos, strength, key = 0, phase = 0, blink = 0;
    int beaconY = 305 - 170 - 38;

    drawScreenFrame("COMMUNICATION ARRAY");
    drawTitle(title, 50, LIGHTCYAN, 2);
    drawTower(130, 305);

    drawPanel(255, 80, 609, 318, "SIGNAL CONSOLE");
    setcolor(LIGHTCYAN);
    outtextxy(275, 206, "SIGNAL STRENGTH");
    outtextxy(275, 246, "FREQUENCY TUNER");
    setcolor(CYAN);
    line(290, 265, 580, 265);
    for(i = 0; i <= 10; i++)
        line(290 + i * 29, 259, 290 + i * 29, 265);

    centerText("[A] / LEFT = TUNE DOWN    [D] / RIGHT = TUNE UP    [ENTER] = LOCK", 345, YELLOW);
    centerText("Move the tuner until the wave is big, clean and GREEN, then lock it.", 365, LIGHTGRAY);

    target = 15 + rand() % 70;
    pos = 10;
    if(target < 50)
        pos = 90;                               // always start far away

    while(key != KEY_ENTER)
    {
        strength = 100 - abs(pos - target) * 4;
        if(strength < 0)
            strength = 0;

        drawWave(strength, phase);
        drawProgressBar(275, 222, 220, strength);
        drawTuner(pos);
        drawSignalWaves(130, beaconY, strength / 25);

        setcolor(blink == 0 ? LIGHTRED : RED);  // blinking beacon
        setfillstyle(SOLID_FILL, blink == 0 ? LIGHTRED : RED);
        fillellipse(130, beaconY, 4, 4);
        blink = 1 - blink;
        phase++;
        delay(70);

        while(kbhit() && key != KEY_ENTER)
        {
            key = readKey();
            if(key == 'a' || key == 'A' || key == KEY_LEFT)
            {
                if(pos > 0) pos = pos - 2;
                playTone(300 + strength * 8, 8);
            }
            else if(key == 'd' || key == 'D' || key == KEY_RIGHT)
            {
                if(pos < 100) pos = pos + 2;
                playTone(300 + strength * 8, 8);
            }
            else if(key == KEY_SPACE)
                key = KEY_ENTER;
        }
    }

    strength = 100 - abs(pos - target) * 4;
    if(strength < 0)
        strength = 0;

    setfillstyle(SOLID_FILL, BLACK);
    bar(14, 330, 625, 380);
    if(strength >= 70)
    {
        drawTitle("SIGNAL LOCKED", 340, LIGHTGREEN, 2);
        playSuccessSound();
    }
    else
    {
        drawTitle("WEAK SIGNAL LOCK", 340, LIGHTRED, 2);
        playErrorSound();
    }
    delay(500);
    return strength;
}

// ======================= ENDINGS =======================

void drawEarth(int x, int y, int r)
{
    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(x, y, r, r);
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x - 14, y - 10, 16, 11);
    fillellipse(x + 16, y + 14, 13, 9);
    fillellipse(x + 8, y - 26, 9, 5);
    setcolor(WHITE);
    arc(x, y, 30, 80, r - 6);
    arc(x, y, 200, 250, r - 10);
    centerText("EARTH", x, y + r + 28, LIGHTCYAN);
}

void showEndingEarth()
{
    int i, p, px, py;

    drawScreenFrame("FINAL TRANSMISSION");
    drawTower(110, 330);
    drawEarth(530, 130, 50);

    typeText(200, 180, "COMMUNICATION ARRAY ONLINE", LIGHTGREEN);
    typeText(200, 200, "SIGNAL LOCKED ON EARTH", LIGHTGREEN);
    typeText(200, 220, "TRANSMITTING...", YELLOW);

    for(p = 0; p <= 100; p = p + 5)            // signal pulse flies to Earth
    {
        px = 110 + p * 37 / 10;
        py = 122 + p / 12;
        setcolor(LIGHTCYAN);
        circle(px, py, 6);
        circle(px, py, 3);
        drawProgressBar(200, 245, 200, p, LIGHTGREEN);
        playTone(1500 + p * 10, 30);
        setcolor(BLACK);
        circle(px, py, 6);
        circle(px, py, 3);
    }

    for(i = 0; i < 3; i++)                     // Earth answers
    {
        setcolor(i % 2 == 0 ? LIGHTCYAN : WHITE);
        circle(530, 130, 58 + i * 8);
        playTone(800 + i * 200, 90);
    }

    centerText("SIGNAL RECEIVED BY EARTH", 280, LIGHTGREEN);
    centerText("\"ARK-7, RESCUE SHIPS ARE ON THE WAY.\"", 300, WHITE);

    for(i = 0; i < 6; i++)
    {
        drawTitle("MISSION COMPLETE", 345, i % 2 == 0 ? YELLOW : LIGHTGREEN, 4);
        playTone(500 + i * 100, 90);
    }
    playSuccessSound();
    waitForEnter(430);
}

void drawShip(int x, int y, int flame)
{
    int body[10], fin[6];

    if(flame == 1)
    {
        fin[0] = x - 9;  fin[1] = y + 23;
        fin[2] = x + 9;  fin[3] = y + 23;
        fin[4] = x;      fin[5] = y + 40 + rand() % 12;
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, rand() % 2 == 0 ? YELLOW : LIGHTRED);
        fillpoly(3, fin);
    }

    body[0] = x;       body[1] = y - 40;
    body[2] = x + 14;  body[3] = y - 12;
    body[4] = x + 14;  body[5] = y + 22;
    body[6] = x - 14;  body[7] = y + 22;
    body[8] = x - 14;  body[9] = y - 12;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillpoly(5, body);

    setfillstyle(SOLID_FILL, RED);
    fin[0] = x - 14; fin[1] = y + 4;
    fin[2] = x - 26; fin[3] = y + 24;
    fin[4] = x - 14; fin[5] = y + 22;
    fillpoly(3, fin);
    fin[0] = x + 14; fin[2] = x + 26; fin[4] = x + 14;
    fillpoly(3, fin);

    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(x, y - 8, 5, 5);               // window
}

void showEndingEscape()
{
    int i, y;
    char text[5];

    drawScreenFrame("ESCAPE");
    setfillstyle(SOLID_FILL, BROWN);           // planet ground
    bar(12, 420, 627, 470);
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, BLUE);
    pieslice(110, 420, 0, 180, 30);            // the colony left behind
    pieslice(180, 420, 0, 180, 22);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(290, 412, 350, 420);                   // launch pad
    drawShip(320, 370, 0);

    centerText("LAUNCH SEQUENCE STARTED", 80, LIGHTCYAN);
    for(i = 3; i >= 1; i--)
    {
        sprintf(text, "%d", i);
        setfillstyle(SOLID_FILL, BLACK);
        bar(270, 120, 370, 170);
        drawTitle(text, 125, YELLOW, 5);
        playTone(700, 200);
        delay(500);
    }
    setfillstyle(SOLID_FILL, BLACK);
    bar(14, 70, 625, 175);
    playTone(1200, 300);

    for(y = 370; y > 70; y = y - 8)            // lift off
    {
        drawShip(320, y, 1);
        playTone(80 + (370 - y) / 2, 25);
        setfillstyle(SOLID_FILL, BLACK);
        bar(292, y - 42, 348, y + 54 < 411 ? y + 54 : 411);   // do not erase the pad
    }

    setcolor(WHITE);                           // speed streaks
    for(i = 0; i < 30; i++)
    {
        y = 50 + rand() % 250;
        line(20 + i * 20, y, 20 + i * 20, y + 25);
        playTone(2000 - i * 50, 10);
    }

    setfillstyle(SOLID_FILL, BLACK);           // clean space for the text
    bar(30, 140, 610, 245);
    drawTitle("ESCAPE SUCCESSFUL", 150, LIGHTGREEN, 4);
    centerText("The crew leaves KEPLER-X behind.", 210, WHITE);
    centerText("Course set for Earth. The long journey home begins.", 230, WHITE);
    playSuccessSound();
    waitForEnter(390);
}

void showEndingAliens()
{
    int i, k;

    drawScreenFrame("FIRST CONTACT");
    drawColonyBase(320, 430);

    setcolor(LIGHTGREEN);                      // alien mothership
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(320, 90, 60, 24);
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(320, 112, 170, 26);

    for(i = 0; i <= 20; i++)                   // light beam comes down
    {
        setcolor(i % 2 == 0 ? LIGHTGREEN : GREEN);
        line(300 + i * 2, 138, 180 + i * 14, 372);
        playTone(300 + i * 30, 25);
    }

    for(k = 0; k < 6; k++)                     // blinking lights
    {
        for(i = 0; i < 9; i++)
        {
            setcolor((i + k) % 2 == 0 ? YELLOW : LIGHTRED);
            setfillstyle(SOLID_FILL, (i + k) % 2 == 0 ? YELLOW : LIGHTRED);
            fillellipse(180 + i * 35, 116, 4, 4);
        }
        playTone(400 + k * 80, 100);
    }

    drawPanel(130, 180, 509, 320, "ALIEN MESSAGE");
    typeText(150, 202, "<> ~~ /\\ :: <> ~~ /\\ :: <>", LIGHTGREEN);
    typeText(150, 226, "TRANSLATING...", YELLOW);
    typeText(150, 250, "\"WE HEARD YOU. YOU ARE NOT ALONE.\"", WHITE);
    typeText(150, 274, "Together you will build a new home here.", LIGHTCYAN);

    setfillstyle(SOLID_FILL, BLACK);
    bar(90, 330, 550, 362);
    drawTitle("ALLIANCE FORMED", 334, YELLOW, 3);
    playSuccessSound();
    waitForEnter(452);
}

void showEndingFailure(char title[], char reason[])
{
    int i, k;

    cleardevice();
    drawWarningSign(320, 60);
    for(i = 0; i < 8; i++)
    {
        drawBorder(i % 2 == 0 ? LIGHTRED : RED);
        drawTitle(title, 120, i % 2 == 0 ? LIGHTRED : WHITE, 3);
        playTone(400 - i * 35, 120);
    }
    centerText(reason, 185, WHITE);

    for(k = 0; k < 1500; k++)                  // TV static = dead signal
    {
        putpixel(40 + rand() % 560, 215 + rand() % 130, rand() % 2 == 0 ? DARKGRAY : LIGHTGRAY);
        if(k % 150 == 0)
            playTone(100 + rand() % 50, 10);
    }

    centerText("...THE COLONY FALLS SILENT...", 370, LIGHTGRAY);
    waitForEnter(430);
}
