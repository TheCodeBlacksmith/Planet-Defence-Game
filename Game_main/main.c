#include "gba.h"
#include "logic.h"
#include "draw.h"


//#include "myLib.h"

// header files for title screen or exit
// screen images are generated using the nin10kit project. 
#include "images/saucerAlien.h"
#include "images/fighter.h"

#include <stdio.h>
#include <stdlib.h>

GBAState state = START;

int main(void) {
    //  set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;


    // store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    // store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    while (1) {


        // Load the current state of the buttons
        currentButtons = BUTTONS;

        switch(state) {
            case START:
                // Wait for VBlank
                waitForVBlank();

               fullDrawStartState();

                state = START_NODRAW;
                break;
            case START_NODRAW:
                // Check for a button press here to start the app.
                // Start the app by switching the state to APP_INIT.
                nextAppState = processAppState(&currentAppState,previousButtons,currentButtons);
                break;
            case APP_INIT:
               // waitForVBlank();

                // Initialize the app. Switch to the APP state.
                initializeAppState(&currentAppState);

                waitForVBlank();
                // Draw the initial state of the app
                fullDrawAppState(&currentAppState);

                state = APP;

                break;
            case APP:
                // Process the app for one frame, store the next state
                nextAppState = processAppState(&currentAppState,previousButtons,currentButtons);

                // Wait for VBlank before any drawing.
                waitForVBlank();

                // Undraw the previous state
                undrawAppState(&currentAppState);

                // Draw the current state
                drawAppState(&nextAppState);

                // Now set the current state as the next state for the next iter.
                currentAppState = nextAppState;

                // Check if the app is exiting. If it is, then go to the exit state.
                if (nextAppState.gameOver == 1) {
                    state = APP_EXIT;
                } else if(state == 1){
                    state = START;
                } else if(state == 2){
                    state = APP_INIT;
                }

                break;
            case APP_EXIT:
                // Wait for VBlank
                waitForVBlank();

                // Draw the exit / gameover screen
                fullDrawOverState(&currentAppState);

                 // Process the app for one frame, store the next state
                nextAppState = processAppState(&currentAppState,previousButtons,currentButtons);

                if(state == 1){
                    state = START;
                }
                break;
            case APP_EXIT_NODRAW:
                // Check for a button press here to go back to the start screen
                break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
