#include "gba.h"
#include "logic.h"

//EXTRA--------------------------------------------------------------------------

#define MAX_ENEMIES 8

//--------------------------------------

//DRAWING--------------------------------------------------------------------------

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

//INITIALIZATION--------------------------------------------------------------------------


// This function will initialize enemies with their attributes and return the full struct array of them
void initializeEnemyState(AppState *state);

