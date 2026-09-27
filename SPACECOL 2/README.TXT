==================================================================
          SPACE COLONY SURVIVAL  -  10 DAY EDITION
         SY OOP Project  |  Turbo C++ / Borland C++ + BGI
==================================================================

CONTENTS
  1. What changed from the old version
  2. Files
  3. How to compile in Turbo C++   (READ THIS - the method changed)
  4. Controls
  5. How one day works + the 10-day story
  6. Visual interactions (what each action shows)
  7. Sound
  8. OOP concepts -> where to find them (for the viva)
  9. Class diagram
 10. Save file / high score file
 11. What to test
 12. How this was tested, and known risks
 13. Likely viva questions


------------------------------------------------------------------
1. WHAT CHANGED FROM THE OLD VERSION
------------------------------------------------------------------
KEPT (same idea, same classes):
  Colony, CrewMember family, SpaceObject family, Score, SaveSystem,
  Game, the crash story, achievements, save/resume, high scores.

CHANGED:
  - 15 days -> 10 days, 5 chapters of 2 days each. Every day has
    its own story event.
  - The game is now visual. Every choice opens its own mini screen:
    scanner, repair bay, exploration, research terminal, defense
    battle, communication array, crew task, resource production.
  - New main game screen with status bars, crew health bars and a
    6-button COMMAND MENU (2 actions per day).
  - Animated main menu (moving stars), chapter title screens,
    day banners, warning screens, 3 different ending animations
    plus a failure ending, final report, achievements screen.
  - PC speaker sound effects (sound / nosound / delay).
  - Research unlocks 3 upgrades (Alien Scanner, Oxygen Recycler,
    Plasma Shield) that really change the game.
  - Top 5 high score table instead of only one best score.
  - Save file now also stores upgrades, achievements, research,
    defense and the exact point inside the day.

OOP ADDED OR MADE REAL (not just "declared"):
  - SpaceObject now has 3 pure virtual functions: interact(),
    draw() and applyEffect(). The scanner and explore screens call
    obj->draw() - each object draws its own picture.
  - CrewMember::useAbility() is virtual. Each role does something
    different on the CREW command (repair, heal, research ...).
  - SpecialEngineer (multiple/hybrid) and EliteSecurityOfficer
    (multilevel) are now real crew members in the game.
  - Score operator+ is used every night: score = score + dayScore;
  - SaveSystem is a FRIEND CLASS of Game, Colony and CrewMember.
  - SpaceObject::discovered is a static counter shown in the report.

BUGS FIXED FROM THE OLD CODE:
  - game.h had   #include "space.h">   (stray '>').
  - New Game after crew members died: setHealth(100) did not set
    alive back to 1, so dead crew stayed dead in the next game.
    Now the crew objects are deleted and created again with new.
  - The unused CombatAbility class was removed (it did nothing).


------------------------------------------------------------------
2. FILES
------------------------------------------------------------------
  MAIN.CPP        main(): creates the Game object and runs it
  GAME.H          Game class (menus, days, commands, screens)
  GAME.CPP        menus, main screen, 6 commands, day/chapter flow
  EVENTS.CPP      the 10 story days + final decision (Game members)
  COLONY.H/.CPP   resources: oxygen, energy, food, materials,
                  hull, research, defense (encapsulation)
  CREW.H/.CPP     CrewMember + Engineer, Doctor, Scientist,
                  SecurityOfficer, EliteSecurityOfficer, Pilot,
                  RepairAbility, SpecialEngineer
  SPACE.H/.CPP    abstract SpaceObject + Alien, Asteroid, Artifact
  SCORE.H/.CPP    score, operator+, static high score, friend fn
  SAVESYS.H/.CPP  file handling: SAVE.DAT and HISCORE.DAT
  UIGRAPH.H/.CPP  small drawing blocks: drawBorder, drawPanel,
                  drawTitle, drawButton, drawProgressBar,
                  drawStarField, drawStatusBar, event screens ...
  INTERACT.H/.CPP the visual interaction screens: showScanner,
                  showRepair, showExplore, showResearch,
                  showDefense, showCommunication, endings ...
  SOUND.H/.CPP    playClick, playScanSound, playRepairSound,
                  playWarningSound, playSuccessSound, playErrorSound

  11 .CPP files + 9 .H files. All names are DOS 8.3 safe.


------------------------------------------------------------------
3. HOW TO COMPILE IN TURBO C++  (IMPORTANT - READ)
------------------------------------------------------------------
The old MAIN.CPP did  #include "game.cpp"  etc. and you compiled
only MAIN.CPP. That does NOT work any more: the game is now about
4,800 lines, and one Turbo C++ source file can hold at most 64 KB
of machine code. So we use a PROJECT, which compiles each .CPP file
separately. This is the normal Turbo C++ way.

STEP 1  Copy all .CPP and .H files into one folder,
        e.g. C:\TURBOC3\BIN\SPACECOL

STEP 2  Open Turbo C++. Menu:  Project -> Open project
        Type a name:  SPACECOL.PRJ   and press Enter.

STEP 3  Project -> Add item. Add ALL 11 .CPP files:
        MAIN, GAME, EVENTS, COLONY, CREW, SPACE, SCORE, SAVESYS,
        UIGRAPH, INTERACT, SOUND. (Do NOT add the .H files.)

STEP 4  Options -> Compiler -> Code generation -> Model: LARGE
        (Needed. Small model has only 64 KB for ALL code.)

STEP 5  Options -> Linker -> Libraries -> tick [X] Graphics library

STEP 6  Options -> Directories: check Include and Library folders,
        e.g.  C:\TURBOC3\INCLUDE   and   C:\TURBOC3\LIB

STEP 7  Compile -> Build all.   Then Run -> Run (Ctrl+F9).

BGI FOLDER: initGraphicsMode() in UIGRAPH.CPP tries these by itself:
        C:\TURBOC3\BGI,  then C:\TC\BGI,  then the current folder.
If your BGI folder is somewhere else, change the path there, or
copy EGAVGA.BGI into the folder you run the game from.

COMMON ERRORS
  "Undefined symbol _initgraph"      -> Step 5 (graphics library)
  "Undefined symbol Game::..."       -> a .CPP file is missing from
                                        the project (Step 3)
  "Segment ... exceeds 64K" /
  "Out of memory"                    -> Step 4 (Large model)
  "Graphics error: ... BGI"          -> BGI path (see above)
  Old save does not resume           -> normal: old 15-day saves
                                        are rejected on purpose

DOSBOX: if animations are slow, press Ctrl+F12 a few times to give
DOSBox more speed (or set cycles=auto in dosbox.conf).


------------------------------------------------------------------
4. CONTROLS (keyboard "buttons")
------------------------------------------------------------------
  Arrow keys or W A S D   move between buttons
  ENTER or SPACE          press the lit button
  1 - 6                   pick an option / command directly
  P                       pause menu (resume, save, sound, quit)
  N                       end the day early (skip actions)
  Inside screens:         ENTER = scan / weld / analyze / start
  Defense battle:         A / D (or arrows) move, SPACE fires
  Communication array:    A / D (or arrows) tune, ENTER locks
Any key while text is typing shows the rest of the text at once.


------------------------------------------------------------------
5. HOW ONE DAY WORKS + THE 10-DAY STORY
------------------------------------------------------------------
  DAY BANNER  ->  STORY EVENT (choose 1/2/3)  ->  VISUAL INTERACTION
  ->  RESULT (+/- resources, score)  ->  MAIN SCREEN: 2 COMMAND
  ACTIONS (each with its own visual)  ->  END OF DAY night report
  (oxygen, food, energy are used up)  ->  next day.
  Every 2 days: CHAPTER COMPLETE screen + automatic save.

  CH 1 CRASH               Day 1  Crash landing, reactor damaged
                           Day 2  Oxygen leak
  CH 2 UNKNOWN SIGNAL      Day 3  Unknown signal: open/scan/ignore
                           Day 4  First contact with an alien
  CH 3 THE UNKNOWN         Day 5  Abandoned alien station
                           Day 6  Meteor storm
  CH 4 COLONY CRISIS       Day 7  Alien attack
                           Day 8  Life support failure
  CH 5 FINAL TRANSMISSION  Day 9  Get the Alien Core, repair array
                           Day 10 Align signal + final decision

  FINAL DECISION (Day 10):
    CONTACT EARTH  needs Alien Core + Comm Array 60%
    ESCAPE         needs pilot KAI alive, Energy 30, Hull 30
    ALIENS         needs Alien Core + (First Contact or Research 50)
  If the need is not met, the FAILURE ending plays.

  GAME OVER at any time if Oxygen = 0, Hull = 0 or all crew die.

  COMMANDS (main screen, 2 per day):
    1 REPAIR     hull +15, costs 15 materials
    2 EXPLORE    random find (asteroid / relic / alien / crate), -5 energy
    3 RESEARCH   research +10, -10 energy (upgrades at 20/40/60)
    4 CREW       use one crew member's special ability
    5 DEFEND     defense drill mini-game, raises DEFENSE
    6 RESOURCES  make food, oxygen or energy
  A command that cannot be done (not enough energy/materials) shows
  a warning and does NOT use up your action.


------------------------------------------------------------------
6. VISUAL INTERACTIONS
------------------------------------------------------------------
Every screen follows the same pattern:
   draw the scene -> show a key "button" -> player presses it ->
   animation with sound -> RESULT card with the reward.

  SCANNER        radar circle, the object drawn in the middle,
  showScanner()  [ENTER] START SCAN -> rotating radar beam and a
                 scan line move over the object, progress bar,
                 beeps, OBJECT/TYPE/THREAT appear at 40/70/100%.
  REPAIR         damaged machine with red crack and gauges.
  showRepair()   Tap ENTER again and again: sparks fly, the crack
                 turns green piece by piece, integrity bar fills.
  EXPLORE        station on the ground, rover. [ENTER] ENTER AREA ->
  showExplore()  rover drives, zoom into the building, corridor,
                 search bar, the found object is drawn.
  RESEARCH       green terminal. [ENTER] ANALYZE -> scrolling hex
  showResearch() data, analysis bar, "NEW DATA DECODED".
  DEFENSE        alert screen -> charging bar -> real mini-game:
  showDefense()  6 enemies fall in 5 lanes, move the turret with
                 A/D, fire the laser with SPACE. Misses = damage.
  COMM ARRAY     tower with blinking beacon + oscilloscope wave.
  showComm...()  Tune with A/D: the wave gets big, clean and green
                 near the right frequency. ENTER locks the signal.
  CREW TASK      astronaut portrait, [ENTER] ASSIGN TASK, progress.
  RESOURCES      a tank fills up with liquid and bubbles.
  ENDINGS        Earth: signal pulse flies to Earth.
                 Escape: countdown 3-2-1 and rocket launch.
                 Aliens: mothership, light beam, alien message.
                 Failure: red alarm and TV static.

ANIMATION TRICK (good viva point): setwritemode(XOR_PUT).
A line drawn twice in XOR mode disappears and the picture behind
it comes back. The radar beam, scan line, sparks and laser use this,
so they move over a drawing without destroying it.
Other animations just erase with a black bar() and draw again.


------------------------------------------------------------------
7. SOUND  (SOUND.CPP)
------------------------------------------------------------------
Only sound(freq), delay(ms), nosound() are used, through one
function playTone(). Sound can be turned ON/OFF from the main menu
or the pause menu. When it is off, delays still run, so animations
keep the same speed.
  playClick        button press          playLaserSound  laser
  playTypeSound    typing text           playWarningSound alarm
  playScanSound    scanner beep          playSuccessSound melody
  playRepairSound  welding               playErrorSound  failure


------------------------------------------------------------------
8. OOP CONCEPTS -> WHERE TO FIND THEM
------------------------------------------------------------------
 1 Classes & objects   Colony, CrewMember, SpaceObject, Score,
                       SaveSystem, Game.  "Game game;" in MAIN.CPP
 2 Encapsulation       Colony: all resources private; changed only
                       by useOxygen(), addFood() ... which keep 0-100
 3 Abstraction         SpaceObject (abstract); Game hides the whole
                       game behind run()
 4 Constructors        Colony() gives crash values; CrewMember(n, r);
                       Alien(char[]) and Alien(char[], int)
 5 Destructors         virtual ~CrewMember(), virtual ~SpaceObject(),
                       ~Game() deletes the crew
 6 this pointer        CrewMember::setHealth();  SaveSystem::saveGame(*this)
 7 Function overload   Colony::repair() / repair(int);
                       drawBorder() / drawBorder(int);
                       drawProgressBar(4 args) / (5 args);
                       drawTitle, centerText
 8 Operator overload   Score::operator+  used in Game::endOfDay()
 9 Friend function     calculateFinalScore(Score&) in showFinalStats()
   Friend class        SaveSystem in Game, Colony, CrewMember
10 Single inheritance  Doctor : public CrewMember
11 Multilevel          CrewMember -> SecurityOfficer -> EliteSecurityOfficer
12 Hierarchical        CrewMember -> 5 roles; SpaceObject -> 3 objects
13 Multiple / hybrid   SpecialEngineer : public Engineer, public RepairAbility
14 Virtual functions   specialAbility(), useAbility(), getType()
15 Pure virtual        SpaceObject::interact(), draw(), applyEffect() = 0
16 Runtime polymorph.  crew[i]->useAbility(...) in Game::cmdCrew();
                       found->draw() / applyEffect() in cmdExplore(),
                       obj->draw() inside showScanner()
17 Object pointers     SpaceObject *found;  CrewMember *crew[5]
18 new / delete        createCrew()/deleteCrew(); every SpaceObject in
                       cmdExplore(), dayThree(), dayFive(), dayNine()
19 Arrays of pointers  CrewMember *crew[CREW_SIZE]
20 Static members      Score::highScore, SpaceObject::discovered,
                       static functions of SaveSystem
21 File handling       SAVESYS.CPP: ofstream / ifstream
22 BGI graphics        UIGRAPH.CPP, INTERACT.CPP, SPACE.CPP draw()

Unit 6 topics are NOT used: no exceptions, no try/catch/throw,
no templates, no STL.


------------------------------------------------------------------
9. CLASS DIAGRAM
------------------------------------------------------------------
  Game ---has---> Colony
       ---has---> Score
       ---has---> CrewMember *crew[5]
       ---uses--> SpaceObject* (created with new, deleted after use)
       ---uses--> SaveSystem (friend)

  CrewMember
     +-- Engineer ------------+
     |                        +-- SpecialEngineer (+ RepairAbility)
     +-- Doctor
     +-- Scientist
     +-- Pilot
     +-- SecurityOfficer
            +-- EliteSecurityOfficer

  SpaceObject (abstract)
     +-- Alien   +-- Asteroid   +-- Artifact

  Crew used in the game:
     JAKE = SpecialEngineer   ELENA = Doctor   MIRA = Scientist
     TORRES = EliteSecurityOfficer            KAI = Pilot


------------------------------------------------------------------
10. SAVE FILE / HIGH SCORE FILE
------------------------------------------------------------------
SAVE.DAT (plain text, created where the .EXE runs):
  line 1  SCS10                    (file tag - old saves rejected)
  line 2  day chapter comm core eventDone actionsLeft
  line 3  scanner recycler shield  (upgrades)
  line 4  6 achievement flags
  line 5  oxygen energy food materials hull research defense
  line 6  score objectsDiscovered
  7-11    health alive             (one line per crew member)
HISCORE.DAT: top 5 scores, one per line, highest first.
Delete both files to reset everything.
The game auto-saves at the end of every chapter, and whenever you
choose SAVE in the pause menu.


------------------------------------------------------------------
11. WHAT TO TEST
------------------------------------------------------------------
 [ ] Main menu: stars move, arrows/W/S move the light, ENTER works,
     SOUND: ON/OFF toggles, EXIT quits.
 [ ] New Game: crash animation -> Chapter 1 -> Day 1 banner.
 [ ] Starting values: O2 80, Energy 60, Food 70, Materials 30,
     Hull 70, Defense 10, all 5 crew at 100.
 [ ] Try each action once: scanner, repair (tap ENTER), explore,
     research, defense battle, communication tuning, crew, resources.
 [ ] REPAIR with less than 15 materials -> warning, action NOT used.
 [ ] End of day: oxygen -8 (-4 after Oxygen Recycler), food -2 per
     living crew member, energy -6.
 [ ] Research 20 / 40 / 60 -> "NEW TECHNOLOGY UNLOCKED" screen.
 [ ] After Day 2, 4, 6, 8: CHAPTER COMPLETE + "PROGRESS AUTO-SAVED".
 [ ] P -> SAVE AND QUIT -> main menu -> RESUME: same day, same
     values, same actions left, today's event not repeated.
 [ ] Let oxygen or hull reach 0 -> red failure screen + GAME OVER.
 [ ] Day 10: try all 3 endings (and a failed one).
 [ ] Final report -> achievements -> HIGH SCORES shows the top 5,
     still there after closing and restarting the game.


------------------------------------------------------------------
12. HOW THIS WAS TESTED, AND KNOWN RISKS
------------------------------------------------------------------
Turbo C++ itself was not available where this was built. So the
exact same source files were compiled with g++ against small
stand-in versions of graphics.h / conio.h / dos.h that draw into a
640x480 16-colour picture (with the same 8x8 font). Every screen was
saved as an image and checked by eye. Then:
  - 50 complete auto-played games (all endings reached),
  - 400+ sessions of random key presses,
  all with AddressSanitizer (finds memory errors) - 0 errors.
  Save -> quit -> resume was checked value by value.

What could still differ on a real Turbo C++ machine:
  - small compiler messages (warnings like "parameter never used"
    are harmless),
  - the BGI folder path,
  - speed of animations (depends on DOSBox cycles).
The code avoids everything modern: no bool, no string class,
no STL, no templates, only old headers (iostream.h, fstream.h,
graphics.h, conio.h, dos.h), and every int stays below 32767.


------------------------------------------------------------------
13. LIKELY VIVA QUESTIONS (short answers)
------------------------------------------------------------------
Q: Why is ~CrewMember() virtual?
A: crew[] holds CrewMember pointers to Doctor, Pilot ... objects.
   delete crew[i] must run the destructor of the real object.

Q: Why can't you write  new SpaceObject("X") ?
A: It has pure virtual functions, so it is an abstract class.

Q: Where is runtime polymorphism?
A: Game::cmdCrew():  crew[choice]->useAbility(...). The same line
   repairs (Engineer), heals (Doctor) or adds research (Scientist).

Q: Why is SaveSystem a friend class? Doesn't it break encapsulation?
A: It needs to write every private value to the file. Friendship is
   given only to that one trusted class, instead of adding many
   public getters/setters for everything.

Q: Why is the multilevel/multiple inheritance useful here?
A: EliteSecurityOfficer is a better SecurityOfficer (+25 defense
   instead of +15). SpecialEngineer is an Engineer that also has the
   RepairAbility skill (bigger repair).

Q: How does the scanner beam move without erasing the picture?
A: setwritemode(XOR_PUT): drawing the same line twice removes it.

Q: What does the static member SpaceObject::discovered count?
A: How many space objects were created in this game. There is one
   shared copy for the whole class, not one per object.
