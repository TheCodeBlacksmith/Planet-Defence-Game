#include "draw.h"
#include "gba.h"

#include <stdio.h>

#include "images/saucerAlien.h"
#include "images/fighter.h"
#include "images/gameStartBkg.h"
#include "images/gameOverBkg.h"


// This function will be used to draw everything about the app at the START state
// including the background and whatnot.
void fullDrawStartState(void) {
                drawFullScreenImageDMA((u16*) gameStartBkg);
               drawCenteredString(119,130,0,0,"Press A to start",RED);
}

// This function will be used to draw everything about the app at the APP_EXIT state
// including the background and whatnot.
void fullDrawOverState(AppState *state) {
                drawFullScreenImageDMA((u16*) gameOverBkg);
               drawCenteredString(119,130,0,0,"Press SELECT to restart",RED);

                char points[50];
                sprintf(points, "Score: %d", state ->points);
                drawCenteredString(119,150, 0, 0, points, WHITE);
}

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    fillScreenDMA(BLACK);
    drawAppState(state);
    UNUSED(state);
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame.
void undrawAppState(AppState *state) {
    //drawRectDMA(state ->player_x,state ->player_y,FIGHTER_WIDTH,FIGHTER_HEIGHT,BLACK);
        drawRectDMA(state ->player_y -1,state ->player_x -1,FIGHTER_WIDTH+2,FIGHTER_HEIGHT+2,BLACK);

    UNUSED(state);
}

// This function will be used to draw things that might have moved in a frame.
void drawAppState(AppState *state) {
    drawImageDMA(state ->player_y, state ->player_x, FIGHTER_WIDTH, FIGHTER_HEIGHT, (u16*)fighter);
    UNUSED(state);
}

// This function will draw all of the enemies
void drawEnemyState(AppState state){
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(state.enemies[i].enemy_y < 140 && state.enemies[i].enemy_health >= 10 && state.enemies[i].enemy_skip != 1){
                drawImageDMA(state.enemies[i].enemy_y, state.enemies[i].enemy_x, SAUCERALIEN_WIDTH, SAUCERALIEN_HEIGHT, (u16*)saucerAlien);

            } else{
                if(state.enemies[i].enemy_skip != 1){
                state.enemies[i].enemy_skip = 1;
                state.enemies[i].enemy_health = 0;
                drawRectDMA(state.enemies[i].enemy_y, state.enemies[i].enemy_x, SAUCERALIEN_WIDTH, SAUCERALIEN_HEIGHT, BLACK);
                }

            }
    }
}

// This function will undraw all of the enemies
void undrawEnemyState(AppState state){
    for(int i = 0; i < MAX_ENEMIES; i++){
            if(state.enemies[i].enemy_health >= 10  && state.enemies[i].enemy_skip != 1){
                      drawRectDMA(state.enemies[i].enemy_y - 1, state.enemies[i].enemy_x, SAUCERALIEN_WIDTH, SAUCERALIEN_HEIGHT, BLACK);
            }

    }
}

// This function will add a new lazer
void drawNewLaser(AppState *state, int x, int y){
     state ->laserAmount+=1;
     state ->lasers[(state ->laserAmount) -1].laser_hit = 0;
     state ->lasers[(state ->laserAmount) -1].laser_x = x + 4  ;
     state ->lasers[(state ->laserAmount) -1].laser_y = y - 5 ;
}

// This function will draw all lazers
void drawAllLaser(AppState *state){
     for(int i = 0; i < state ->laserAmount; i++){
        if(state ->lasers[i].laser_y > 0 && state ->lasers[i].laser_hit != 1){
                drawRectDMA(state ->lasers[i].laser_y, state ->lasers[i].laser_x, 1, 3, RED);

        } else {
            if(state ->lasers[i].laser_hit != 1){
            state ->lasers[i].laser_hit = 1;
            drawRectDMA(state ->lasers[i].laser_y, state ->lasers[i].laser_x, 1, 3, BLACK);
            }
        }
     }
UNUSED(state);
}

// This function will draw all lazers
void undrawAllLaser(AppState *state){
     for(int i = 0; i < state ->laserAmount; i++){
        if(state ->lasers[i].laser_hit != 1){
            drawRectDMA( state ->lasers[i].laser_y + 1, state ->lasers[i].laser_x , 1, 3, BLACK);
        }
     }
     UNUSED(state);
}
