#ifndef SOUND_H
#define SOUND_H

// ================================================================
// SOUND.H - simple PC-speaker sound effects
// Uses only sound(), nosound() and delay() from <dos.h>.
// ================================================================

void setSoundOn(int on);     // 1 = sound on, 0 = sound off
int  isSoundOn();

void playTone(int frequency, int milliseconds);

void playClick();            // button selected
void playTypeSound();        // one letter typed on screen
void playScanSound();        // one scanner beep
void playRepairSound();      // welding / repair noise
void playLaserSound();       // defense laser
void playWarningSound();     // danger alarm
void playSuccessSound();     // small victory melody
void playErrorSound();       // something went wrong

#endif
