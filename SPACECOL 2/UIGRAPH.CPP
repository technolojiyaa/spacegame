// ================================================================
// UIGRAPH.CPP - reusable BGI building blocks
// These functions only DRAW things. The game rules live in the
// classes (Colony, CrewMember, SpaceObject, Game ...).
// ================================================================
#include "UIGRAPH.H"
#include "SOUND.H"
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int skipTyping = 0;      // 1 = player pressed a key: show text instantly

// ======================= SETUP =======================

void initGraphicsMode()
{
    int gd = DETECT, gm, error;

    // Try the most common BGI folders one after another.
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    error = graphresult();

    if(error != grOk)
    {
        gd = DETECT;
        initgraph(&gd, &gm, "C:\\TC\\BGI");
        error = graphresult();
    }
    if(error != grOk)
    {
        gd = DETECT;
        initgraph(&gd, &gm, "");        // EGAVGA.BGI copied next to the program
        error = graphresult();
    }
    if(error != grOk)
    {
        printf("Graphics error: %s\n", grapherrormsg(error));
        printf("Change the BGI path in initGraphicsMode() (UIGRAPH.CPP).\n");
        getch();
        exit(1);
    }
}

void closeGraphicsMode()
{
    nosound();
    closegraph();
}

// ======================= KEYBOARD =======================

// Arrow keys send TWO codes: 0 (or 224) and then a scan code.
// We return 256 + scan code so arrows never clash with letters.
int readKey()
{
    int key = getch();
    if(key == 0 || key == 224)
        key = 256 + getch();
    return key;
}

void waitEnterKey()
{
    int key = 0;
    while(key != KEY_ENTER && key != KEY_SPACE)
        key = readKey();
    skipTyping = 0;
    playClick();
}

void waitForEnter(int y)
{
    int key = 0, blink = 0;

    centerText("[ PRESS ENTER ]", y, LIGHTCYAN);
    while(key != KEY_ENTER && key != KEY_SPACE)
    {
        while(!kbhit())                 // blink while waiting
        {
            if(blink == 0)
                centerText("[ PRESS ENTER ]", y, DARKGRAY);
            else
                centerText("[ PRESS ENTER ]", y, LIGHTCYAN);
            blink = 1 - blink;
            delay(250);
        }
        key = readKey();
    }
    skipTyping = 0;
    playClick();
}

// ======================= BASIC DRAWING =======================

void drawBorder()
{
    drawBorder(LIGHTCYAN);
}

void drawBorder(int color)
{
    setcolor(color);
    rectangle(4, 4, 635, 475);
    setcolor(BLUE);
    rectangle(7, 7, 632, 472);

    setfillstyle(SOLID_FILL, color);    // small corner blocks
    bar(1, 1, 9, 9);
    bar(630, 1, 638, 9);
    bar(1, 470, 9, 478);
    bar(630, 470, 638, 478);
}

void drawStarField()
{
    int i, x, y;

    for(i = 0; i < 90; i++)
    {
        x = 12 + (i * 131) % 616;
        y = 12 + (i * 71 + i * i) % 456;

        if(i % 5 == 0)
            putpixel(x, y, WHITE);
        else if(i % 2 == 0)
            putpixel(x, y, LIGHTGRAY);
        else
            putpixel(x, y, DARKGRAY);
    }
}

// ---------- moving stars (used on the main menu) ----------
#define MAX_STARS 30

int starX[MAX_STARS], starY[MAX_STARS], starSpeed[MAX_STARS], starShown[MAX_STARS];
int starsReady = 0;

void resetStars()
{
    int i;
    for(i = 0; i < MAX_STARS; i++)
    {
        if(starsReady == 0)
        {
            starX[i] = 12 + rand() % 616;
            starY[i] = 14 + rand() % 452;
            starSpeed[i] = 1 + rand() % 3;
        }
        starShown[i] = 0;
    }
    starsReady = 1;
}

void moveStars()
{
    int i, color;

    if(starsReady == 0)
        resetStars();

    for(i = 0; i < MAX_STARS; i++)
    {
        color = DARKGRAY;
        if(starSpeed[i] == 2) color = LIGHTGRAY;
        if(starSpeed[i] == 3) color = WHITE;

        // erase the old star (only if nothing else was drawn over it)
        if(starShown[i] == 1 && getpixel(starX[i], starY[i]) == color)
            putpixel(starX[i], starY[i], BLACK);

        starX[i] = starX[i] - starSpeed[i];
        if(starX[i] < 12)
            starX[i] = 627;

        // draw only on empty (black) space, so text is never damaged
        starShown[i] = 0;
        if(getpixel(starX[i], starY[i]) == BLACK)
        {
            putpixel(starX[i], starY[i], color);
            starShown[i] = 1;
        }
    }
}

// Clears the screen and draws the standard sci-fi frame with a header.
void drawScreenFrame(char title[])
{
    skipTyping = 0;                      // new screen: type text slowly again
    cleardevice();
    resetStars();
    drawStarField();
    drawBorder();

    setfillstyle(SOLID_FILL, BLUE);
    bar(12, 12, 627, 38);
    setcolor(LIGHTCYAN);
    rectangle(12, 12, 627, 38);
    line(20, 35, 80, 35);               // small decoration lines
    line(559, 35, 619, 35);

    drawTitle(title, 18, YELLOW, 2);
}

void drawTitle(char text[], int y)
{
    drawTitle(text, y, YELLOW, 3);
}

void drawTitle(char text[], int y, int color, int size)
{
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(CENTER_TEXT, TOP_TEXT);

    if(size >= 3)                        // shadow for big titles
    {
        setcolor(BLUE);
        outtextxy(323, y + 3, text);
    }
    setcolor(color);
    outtextxy(320, y, text);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, TOP_TEXT);
}

void centerText(char text[], int y, int color)
{
    centerText(text, 320, y, color);
}

void centerText(char text[], int x, int y, int color)
{
    settextjustify(CENTER_TEXT, TOP_TEXT);
    setcolor(color);
    outtextxy(x, y, text);
    settextjustify(LEFT_TEXT, TOP_TEXT);
}

// Prints text one letter at a time, like an old computer terminal.
// Pressing any key prints the rest of the screen's text at once.
void typeText(int x, int y, char text[], int color)
{
    int i;
    char letter[2];

    letter[1] = '\0';
    setcolor(color);

    for(i = 0; text[i] != '\0'; i++)
    {
        letter[0] = text[i];
        outtextxy(x + i * 8, y, letter);

        if(skipTyping == 0)
        {
            if(text[i] != ' ')
                playTypeSound();
            else
                delay(8);

            if(kbhit())
            {
                readKey();
                skipTyping = 1;
            }
        }
    }
}

void drawPanel(int x1, int y1, int x2, int y2, char title[])
{
    int w;

    setfillstyle(SOLID_FILL, BLACK);
    bar(x1, y1, x2, y2);
    setcolor(BLUE);
    rectangle(x1 + 3, y1 + 3, x2 - 3, y2 - 3);
    setcolor(LIGHTCYAN);
    rectangle(x1, y1, x2, y2);

    if(title[0] != '\0')                 // little title tab on top
    {
        w = textwidth(title) + 12;
        setfillstyle(SOLID_FILL, CYAN);
        bar(x1 + 10, y1 - 5, x1 + 10 + w, y1 + 6);
        setcolor(BLACK);
        outtextxy(x1 + 16, y1 - 3, title);
    }
}

// A keyboard "button". selected = 1 draws it lit up.
void drawButton(int x, int y, int w, char label[], int selected)
{
    if(selected == 1)
    {
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        bar(x, y, x + w, y + 22);
        setcolor(WHITE);
        rectangle(x - 2, y - 2, x + w + 2, y + 24);
        setcolor(BLACK);
    }
    else
    {
        setfillstyle(SOLID_FILL, BLACK);
        bar(x - 2, y - 2, x + w + 2, y + 24);
        setcolor(CYAN);
        rectangle(x, y, x + w, y + 22);
        setcolor(WHITE);
    }

    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(x + w / 2, y + 8, label);
    settextjustify(LEFT_TEXT, TOP_TEXT);
}

// Colour chosen automatically: green = good, yellow = careful, red = danger
void drawProgressBar(int x, int y, int w, int percent)
{
    int color = LIGHTGREEN;
    if(percent < 60) color = YELLOW;
    if(percent < 30) color = LIGHTRED;
    drawProgressBar(x, y, w, percent, color);
}

// A segmented retro bar:  [#######---] 70%
void drawProgressBar(int x, int y, int w, int percent, int color)
{
    int i, blocks, blockW, bx;
    char text[10];

    if(percent < 0) percent = 0;
    if(percent > 100) percent = 100;

    blockW = (w - 4) / 10;
    blocks = percent / 10;

    setfillstyle(SOLID_FILL, BLACK);     // clear the old bar and number
    bar(x, y, x + w + 44, y + 12);
    setcolor(CYAN);
    rectangle(x, y, x + w, y + 12);

    for(i = 0; i < 10; i++)
    {
        bx = x + 3 + i * blockW;
        if(i < blocks)
            setfillstyle(SOLID_FILL, color);
        else
            setfillstyle(SOLID_FILL, DARKGRAY);
        bar(bx, y + 3, bx + blockW - 3, y + 9);
    }

    sprintf(text, "%d%%", percent);
    setcolor(WHITE);
    outtextxy(x + w + 8, y + 3, text);
}

void drawStatBar(int x, int y, char label[], int value)
{
    setcolor(LIGHTCYAN);
    outtextxy(x, y + 3, label);
    drawProgressBar(x + 100, y, 180, value);
}

// Thin strip under the header: left text, middle text, right text.
void drawStatusBar(char left[], char middle[], char right[])
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(12, 42, 627, 60);
    setcolor(CYAN);
    rectangle(12, 42, 627, 60);

    setcolor(WHITE);
    outtextxy(22, 48, left);
    centerText(middle, 48, YELLOW);

    settextjustify(RIGHT_TEXT, TOP_TEXT);
    setcolor(LIGHTGREEN);
    outtextxy(617, 48, right);
    settextjustify(LEFT_TEXT, TOP_TEXT);
}

void drawPlanet(int x, int y, int r, int color)
{
    setcolor(DARKGRAY);
    ellipse(x, y, 0, 180, r + 18, r / 3);      // back half of the ring
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, r, r);
    setcolor(LIGHTGRAY);
    ellipse(x, y, 180, 360, r + 18, r / 3);    // front half of the ring
}

void drawWarningSign(int x, int y)
{
    setcolor(YELLOW);
    line(x, y - 30, x - 34, y + 28);
    line(x - 34, y + 28, x + 34, y + 28);
    line(x + 34, y + 28, x, y - 30);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(x - 11, y - 6, "!");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

// ======================= READY-MADE SCREENS =======================

void drawMainMenuScreen()
{
    cleardevice();
    resetStars();
    drawStarField();
    drawBorder();

    drawPlanet(565, 95, 34, MAGENTA);
    drawPlanet(75, 400, 20, BROWN);

    setcolor(LIGHTCYAN);                 // title box
    rectangle(100, 40, 540, 190);
    setcolor(BLUE);
    rectangle(104, 44, 536, 186);

    drawTitle("SPACE COLONY", 58, LIGHTCYAN, 4);
    drawTitle("SURVIVAL", 102, YELLOW, 5);
    centerText("-  LOST BEYOND THE STARS  -", 160, LIGHTGRAY);
    centerText("ARROW KEYS / W S TO MOVE     ENTER TO SELECT", 205, DARKGRAY);
}

// Shows a vertical list of buttons and lets the player pick one.
// Arrow keys (or W/S) move, ENTER/SPACE confirms, number keys pick directly.
// Returns the index of the chosen item (0 = first).
int chooseButton(char items[][40], int count, int x, int y, int w,
                 int gap, int numbered, int animate)
{
    int i, selected = 0, key, chosen = -1;
    char label[50];

    while(chosen == -1)
    {
        for(i = 0; i < count; i++)
        {
            if(numbered == 1)
                sprintf(label, "[%d] %s", i + 1, items[i]);
            else
                strcpy(label, items[i]);
            drawButton(x, y + i * gap, w, label, i == selected);
        }

        if(animate == 1)
        {
            while(!kbhit())
            {
                moveStars();
                delay(40);
            }
        }

        key = readKey();

        if(key == KEY_UP || key == 'w' || key == 'W')
        {
            selected--;
            if(selected < 0) selected = count - 1;
            playClick();
        }
        else if(key == KEY_DOWN || key == 's' || key == 'S')
        {
            selected++;
            if(selected >= count) selected = 0;
            playClick();
        }
        else if(key >= '1' && key < '1' + count)
        {
            selected = key - '1';
            chosen = selected;
        }
        else if(key == KEY_ENTER || key == KEY_SPACE)
        {
            chosen = selected;
        }
    }

    // "press" animation so it feels like a real button
    if(numbered == 1)
        sprintf(label, "[%d] %s", chosen + 1, items[chosen]);
    else
        strcpy(label, items[chosen]);
    for(i = 0; i < 2; i++)
    {
        drawButton(x, y + chosen * gap, w, label, 0);
        delay(60);
        drawButton(x, y + chosen * gap, w, label, 1);
        playClick();
        delay(60);
    }
    skipTyping = 0;
    return chosen;
}

// Story screen with a mission log and answer buttons.
// Returns the chosen option number (1, 2, 3 ...).
int showEventScreen(char title[], char lines[][70], int lineCount,
                    char options[][40], int optionCount)
{
    int i, bottom, y;

    drawScreenFrame("INCOMING TRANSMISSION");
    drawTitle(title, 50, LIGHTCYAN, 2);

    bottom = 90 + lineCount * 18 + 14;
    drawPanel(30, 80, 609, bottom, "MISSION LOG");

    setfillstyle(SOLID_FILL, LIGHTRED);  // red "alert" light
    setcolor(RED);
    fillellipse(592, 94, 5, 5);

    for(i = 0; i < lineCount; i++)
        typeText(46, 96 + i * 18, lines[i], WHITE);

    y = bottom + 18;
    centerText("COMMANDER, WHAT ARE YOUR ORDERS?", y, YELLOW);

    return chooseButton(options, optionCount, 140, y + 22, 360, 32, 1, 0) + 1;
}

// Flashing red danger screen.
void showWarning(char title[], char text[], char reward[])
{
    int i;

    drawScreenFrame(title);
    drawWarningSign(320, 120);

    for(i = 0; i < 6; i++)
    {
        if(i % 2 == 0)
        {
            drawBorder(LIGHTRED);
            drawTitle("!! WARNING !!", 175, LIGHTRED, 4);
        }
        else
        {
            drawBorder(RED);
            drawTitle("!! WARNING !!", 175, YELLOW, 4);
        }
        playTone(i % 2 == 0 ? 880 : 440, 140);
    }

    centerText(text, 250, WHITE);
    centerText(reward, 280, LIGHTRED);
    waitForEnter(430);
}

// Calm information screen (no alarm).
void showNotice(char title[], char text[], char reward[])
{
    drawScreenFrame(title);
    drawPanel(60, 150, 579, 300, "STATUS");
    typeText(80, 190, text, WHITE);
    centerText(reward, 240, YELLOW);
    playTone(600, 80);
    playTone(800, 80);
    waitForEnter(430);
}

// Result panel drawn at the bottom of an action screen.
void showResultCard(char title[], char line1[], char line2[],
                    char reward[], int good)
{
    int color = LIGHTGREEN;
    if(good == 0)
        color = LIGHTRED;

    drawPanel(30, 330, 609, 466, "RESULT");
    drawTitle(title, 344, color, 2);
    centerText(line1, 370, WHITE);
    centerText(line2, 386, LIGHTGRAY);
    centerText(reward, 408, YELLOW);

    if(good == 1)
        playSuccessSound();
    else
        playErrorSound();

    waitForEnter(440);
}

void showChapterTitle(int chapter, char name[], char days[])
{
    int i;
    char text[20];

    cleardevice();
    drawStarField();

    setcolor(BLUE);                       // lines sweep out from the middle
    for(i = 0; i <= 300; i = i + 15)
    {
        line(320 - i, 150, 320 + i, 150);
        line(320 - i, 330, 320 + i, 330);
        delay(12);
    }

    sprintf(text, "CHAPTER %d", chapter);
    drawTitle(text, 175, LIGHTCYAN, 4);
    playTone(400, 120);
    playTone(600, 120);

    drawTitle(name, 230, YELLOW, 3);
    playTone(800, 220);

    centerText(days, 285, WHITE);
    drawBorder();
    waitForEnter(420);
}

void showDayBanner(int day, int maxDay, char title[])
{
    int p;
    char text[20];

    drawScreenFrame("MISSION CLOCK");
    sprintf(text, "DAY %d / %d", day, maxDay);
    drawTitle(text, 140, WHITE, 5);
    drawTitle(title, 215, YELLOW, 2);

    centerText("RUNNING SYSTEM CHECK...", 280, LIGHTGREEN);
    for(p = 0; p <= 100; p = p + 10)
    {
        drawProgressBar(200, 300, 200, p, LIGHTGREEN);
        playTone(900 + p * 5, 20);
        delay(30);
    }
    centerText("ALL SYSTEMS REPORTING", 325, LIGHTGREEN);
    waitForEnter(420);
}
