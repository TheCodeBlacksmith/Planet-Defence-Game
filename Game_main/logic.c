#include "logic.h"
#include "gba.h"
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images/saucerAlien.h"
#include "images/fighter.h"


 extern GBAState state;

void initializeAppState(AppState* appState) {
    // Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.

    appState ->gameOver = 0;

    appState ->player_health = 100;


    initializeEnemyState(appState);

    appState ->laserAmount = 0;

    appState ->start_next_round = 0;

}

//This function will initialize enemies with their attributes and return the full struct array of them
void initializeEnemyState(AppState *appState) {
   for(int i = 0; i < MAX_ENEMIES; i++){
            appState ->enemies[i].enemy_skip = 0;
            appState ->enemies[i].enemy_health = 10;
            appState ->enemies[i].enemy_x = rand() % 230 ;
            appState ->enemies[i].enemy_y = 0 ;
    }
}


// This function processes the current app state and returns the new (i.e. next)
// state of the application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* This function gets called
     * every frame.
     */
    if(state == 1){ //START_NODRAW
        if(KEY_JUST_PRESSED(BUTTON_A,keysPressedNow, keysPressedBefore)){
            keysPressedBefore = keysPressedNow;
            state = 2;
        }
        //the position of the player and the initial amount of points are only set and the complete start of the game
         currentAppState ->player_x = 120;
        currentAppState ->player_y = 140;
        currentAppState ->points = 0;

    } else if (state == 2){ // APP_INIT
    
    } else if (state == 3){ // APP
         if(KEY_JUST_PRESSED(BUTTON_SELECT,keysPressedNow, keysPressedBefore)){ //resets to start screen
            keysPressedBefore = keysPressedNow;
            state = 1;
        }
        //KEY_JUST_PRESSED(BUTTON_UP,keysPressedNow, keysPressedBefore)
        if(KEY_DOWN(BUTTON_UP, BUTTONS)  && currentAppState ->player_y > 0){
            keysPressedBefore = keysPressedNow;
            currentAppState ->player_y--;
        }
        //KEY_JUST_PRESSED(BUTTON_DOWN,keysPressedNow, keysPressedBefore)
        if(KEY_DOWN(BUTTON_DOWN, BUTTONS) && currentAppState ->player_y < 142){
            keysPressedBefore = keysPressedNow;
            currentAppState ->player_y++;
        }
        //KEY_JUST_PRESSED(BUTTON_RIGHT,keysPressedNow, keysPressedBefore)
        if(KEY_DOWN(BUTTON_RIGHT, BUTTONS)  && currentAppState ->player_x < 228){
            keysPressedBefore = keysPressedNow;
            currentAppState ->player_x++;
        }
        //KEY_JUST_PRESSED(BUTTON_LEFT,keysPressedNow, keysPressedBefore)
        if(KEY_DOWN(BUTTON_LEFT, BUTTONS) && currentAppState ->player_x > 0){
            keysPressedBefore = keysPressedNow;
            currentAppState ->player_x--;
        }
        if(KEY_JUST_PRESSED(BUTTON_A,keysPressedNow, keysPressedBefore) && currentAppState ->player_y > 0){
            keysPressedBefore = keysPressedNow;
            drawNewLaser(currentAppState, currentAppState ->player_x, currentAppState ->player_y);
        }

        //loop for moving the enemies
        for(int i = 0; i < MAX_ENEMIES; i++){
                if(currentAppState ->enemies[i].enemy_y < 140 ){
                            currentAppState -> enemies[i].enemy_y = currentAppState ->enemies[i].enemy_y + 1;
                } else {
                    currentAppState ->start_next_round = 1;
                }
        }

        //loop for moving the lasers
        for(int i = 0; i < currentAppState ->laserAmount; i++){
                if(currentAppState ->lasers[i].laser_y > 0){
                            currentAppState ->lasers[i].laser_y = currentAppState ->lasers[i].laser_y - 1;
                }
        }

        //loop for checking if enemies hit a laser
            for(int e = 0; e < MAX_ENEMIES; e++){
                for(int i = 0; i < currentAppState ->laserAmount; i++){
                if(currentAppState ->enemies[e].enemy_skip != 1 &&
                   (currentAppState ->lasers[i].laser_y - currentAppState ->enemies[e].enemy_y <= 10 && currentAppState ->lasers[i].laser_x - currentAppState ->enemies[e].enemy_x >= 0) &&
                   (currentAppState ->lasers[i].laser_x - currentAppState ->enemies[e].enemy_x <= 10   && currentAppState ->lasers[i].laser_y - currentAppState ->enemies[e].enemy_y >= 0)){
                              currentAppState ->enemies[e].enemy_health = 0;
                              currentAppState ->enemies[e].enemy_skip = 1;
                              drawRectDMA(currentAppState ->enemies[e].enemy_y-1, currentAppState ->enemies[e].enemy_x, SAUCERALIEN_WIDTH, SAUCERALIEN_HEIGHT, BLACK);

                              currentAppState ->lasers[i].laser_hit = 1;
                              drawRectDMA(currentAppState ->lasers[i].laser_y + 1, currentAppState ->lasers[i].laser_x, 1, 3, BLACK);
                              currentAppState ->points++;
                }
             }
          }

        for(int e = 0; e < MAX_ENEMIES; e++){

                if(currentAppState ->enemies[e].enemy_skip != 1 &&
                   (currentAppState ->player_y - currentAppState ->enemies[e].enemy_y <= 5 && currentAppState ->player_y - currentAppState ->enemies[e].enemy_y >= -5) &&
                   (currentAppState ->player_x - currentAppState ->enemies[e].enemy_x <= 10   && currentAppState ->player_x - currentAppState ->enemies[e].enemy_x >= -10)){
                currentAppState ->gameOver = 1;
                }

          }

          if(currentAppState ->gameOver){
            state = 4;
          }

           if(currentAppState ->start_next_round == 1){ // start the next round
            state = 2;
          }

    } else if (state == 4){ // APP_EXIT
         if(KEY_JUST_PRESSED(BUTTON_SELECT,keysPressedNow, keysPressedBefore)){ //resets to start screen
            keysPressedBefore = keysPressedNow;
            state = 1;
        }


    } else if (state == 5){ // APP_EXIT_NODRAW

    }
    //undraws old and draws new laser positions
   if(state == 3){
    undrawAllLaser(currentAppState);
    drawAllLaser(currentAppState);
   }

   //prints the current score;
    if(state == 3){
    drawRectDMA(148,210,30,10,BLACK);
    char points[50];
    sprintf(points, "Pts: %d", currentAppState ->points);
    drawString(180,149, points, YELLOW);
    }
     //undraws old and draws new enemy positions
    if(state == 3){
    waitForVBlank();

    undrawEnemyState(*currentAppState);
    drawEnemyState(*currentAppState);

    volatile int x = 0;
      while(x < 10000){
        x++;
      }
    }


    AppState nextAppState = *currentAppState;
    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);
    return nextAppState;
}
