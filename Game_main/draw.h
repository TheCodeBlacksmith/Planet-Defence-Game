#ifndef DRAW_H
#define DRAW_H

#include "logic.h"


// This function will be used to draw everything about the state of the app
// including the background and whatnot.
void fullDrawAppState(AppState *state);

// This function will be used to undraw (i.e. erase) things that might
// move in a frame.
void undrawAppState(AppState *state);

// This function will be used to draw things that might have moved in a frame.
void drawAppState(AppState *state);



//EXTRA--------------------------------------------------------------------------

// This function will be used to draw everything about the START screen of the game
// including the background and whatnot.
void fullDrawStartState(void);

// This function will be used to draw everything about the ApPP_EXIT screen of the game
// including the background and whatnot.
void fullDrawOverState(AppState *state);

// This function will draw all of the enemies
void drawEnemyState(AppState state);

// This function will undraw all of the enemies
void undrawEnemyState(AppState state);

// This function will add a new lazer
void drawNewLaser(AppState* state, int x, int y);

// This function will draw all lazers
void drawAllLaser(AppState* state);

// This function will draw all lazers
void undrawAllLaser(AppState* state);

#endif
