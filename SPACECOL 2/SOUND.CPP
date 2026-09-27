// ================================================================
// SOUND.CPP - simple PC-speaker sound effects
// sound(f)  starts a tone of f Hz
// delay(ms) waits
// nosound() stops the tone
// ================================================================
#include "SOUND.H"
#include <dos.h>

int soundOn = 1;     // global switch, changed from the main menu

void setSoundOn(int on)
{
    soundOn = on;
}

int isSoundOn()
{
    return soundOn;
}

// Every sound effect is built from this one function.
// When sound is OFF we still wait, so animations keep the same speed.
void playTone(int frequency, int milliseconds)
{
    if(soundOn == 1)
        sound(frequency);
    delay(milliseconds);
    nosound();
}

void playClick()
{
    playTone(1200, 15);
}

void playTypeSound()
{
    playTone(2000, 8);
}

void playScanSound()
{
    playTone(1800, 25);
    playTone(2400, 15);
}

void playRepairSound()
{
    playTone(300, 20);
    playTone(520, 15);
    playTone(260, 20);
}

void playLaserSound()
{
    int f;
    for(f = 2000; f > 400; f = f - 200)
        playTone(f, 5);
}

void playWarningSound()
{
    int i;
    for(i = 0; i < 3; i++)
    {
        playTone(880, 110);
        playTone(440, 110);
    }
}

void playSuccessSound()
{
    playTone(523, 90);     // C
    playTone(659, 90);     // E
    playTone(784, 90);     // G
    playTone(1046, 180);   // high C
}

void playErrorSound()
{
    playTone(220, 150);
    playTone(160, 250);
}
