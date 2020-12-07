#ifndef LOGIC_H
#define LOGIC_H
#include "gba.h"



////EXTRA--------------------------------------------------------------------------
//
#define MAX_ENEMIES 8
////--------------------------------------
typedef struct {
    int enemy_health;
    int enemy_x;
    int enemy_y;
    int enemy_skip;
} EnemyState;

typedef struct {
    int laser_x;
    int laser_y;
    int laser_hit;
} Laser;

//maintains the current screen is showing
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int points;


    int player_health;

    int player_x;
    int player_y;

    EnemyState enemies[MAX_ENEMIES];

    int laserAmount;
    Laser lasers[100];

    int start_next_round;
} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);



//EXTRA--------------------------------------------------------------------------
// This function will initialize enemies with their attributes and return the full struct array of them
void initializeEnemyState(AppState *state);

#endif
