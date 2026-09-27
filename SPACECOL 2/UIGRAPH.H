#ifndef UIGRAPH_H
#define UIGRAPH_H

// ================================================================
// UIGRAPH.H - small reusable BGI drawing building blocks
// (named UIGRAPH so it never clashes with Turbo C++'s <graphics.h>)
//
// Screen size: 640 x 480 (VGA). The default BGI font is 8x8 pixels,
// so size 2 = 16 pixels high, size 4 = 32 pixels high, and so on.
// ================================================================

// Key codes returned by readKey(). Arrow keys return 256 + scan code.
#define KEY_ENTER  13
#define KEY_ESC    27
#define KEY_SPACE  32
#define KEY_UP     328
#define KEY_LEFT   331
#define KEY_RIGHT  333
#define KEY_DOWN   336

// ---------- setup / keyboard ----------
void initGraphicsMode();
void closeGraphicsMode();
int  readKey();
void waitEnterKey();                // waits for ENTER or SPACE, no text
void waitForEnter(int y);           // blinking "[ PRESS ENTER ]" at height y

// ---------- basic drawing ----------
void drawBorder();                  // FUNCTION OVERLOADING:
void drawBorder(int color);         // default colour / chosen colour
void drawStarField();
void resetStars();
void moveStars();
void drawScreenFrame(char title[]);
void drawTitle(char text[], int y);                     // overloaded
void drawTitle(char text[], int y, int color, int size);
void centerText(char text[], int y, int color);         // overloaded
void centerText(char text[], int x, int y, int color);
void typeText(int x, int y, char text[], int color);
void drawPanel(int x1, int y1, int x2, int y2, char title[]);
void drawButton(int x, int y, int w, char label[], int selected);
void drawProgressBar(int x, int y, int w, int percent);            // overloaded
void drawProgressBar(int x, int y, int w, int percent, int color);
void drawStatBar(int x, int y, char label[], int value);
void drawStatusBar(char left[], char middle[], char right[]);
void drawPlanet(int x, int y, int r, int color);
void drawWarningSign(int x, int y);

// ---------- ready-made screens ----------
void drawMainMenuScreen();
int  chooseButton(char items[][40], int count, int x, int y, int w,
                  int gap, int numbered, int animate);
int  showEventScreen(char title[], char lines[][70], int lineCount,
                     char options[][40], int optionCount);
void showWarning(char title[], char text[], char reward[]);
void showNotice(char title[], char text[], char reward[]);
void showResultCard(char title[], char line1[], char line2[],
                    char reward[], int good);
void showChapterTitle(int chapter, char name[], char days[]);
void showDayBanner(int day, int maxDay, char title[]);

#endif
