#include "gba.h"

//set pixel for if(videoBuffer(x,y) == RED then...)

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {

    // a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if the app is too fast)
    while(SCANLINECOUNTER > 160)
    ;
    // a while loop that keeps going until we're in vBlank:
    while(SCANLINECOUNTER < 160)
    ;
    // Finally, increment the vBlank counter:
    vBlankCounter++;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(x,y,240)] = color; //set the specific index (x,y) to the color
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    for (int i = 0; i < height; i++) {
            DMA[3].src = &color;
            DMA[3].dst = &videoBuffer[OFFSET(i + x, y, 240)];
            DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_ON;
    }
}

void drawFullScreenImageDMA(u16 *image) {
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 240*160 | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
}

/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to start drawing the image at
* @param c column to start drawing the image at
* @param width width of the image * @param height height of the image
* @param image pointer to the first element of the image
*/
void drawImage3 (int r, int c, int width, int height, const u16* image) {
     for (int i = 0; i < height; i++) {
		DMA[3].src = image + OFFSET(i,0,width);
		DMA[3].dst = videoBuffer + OFFSET(i+r, c, 240);
		DMA[3].cnt = width | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	}
}

void drawImageDMA(int x, int y, int width, int height, u16 *image) {
	for (int i = 0; i < height; i++) {
		DMA[3].src = image + OFFSET(i,0,width);
		DMA[3].dst = videoBuffer + OFFSET(i+x, y, 240);
		DMA[3].cnt = width | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	}
}

void fillScreenDMA(volatile u16 color) {
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | 240*160;
    // above line activates DMA, (since its a color) fixes the src of where to draw from,
    // moves the "place" to draw to on-screen, sets the size to draw (will have to be the size of the photo for non-full screen)
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(row+r, col+c, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}
//  void getPixel(int x, int y) {
//     return videoBuffer[OFFSET(x,y,240)]; //return the specific index (x,y) to the color
// }
//..............
//  if(getPixel(x,y) == RED){
//  ....
// }
//-------------------------------------------------------------------------------------------------------------
//for (int i = 0; i < 5; i++) {
//    if (getPixel(state->player.x + i, state->player.y + 5) != BLACK) {
//        state->gameOver = 1;
//    }
//    if (getPixel(state->player.x + i, state->player.y - 1) != BLACK) {
//        state->gameOver = 1;
//    }
//    if (getPixel(state->player.x + 5, state->player.y + i) != BLACK) {
//        state->gameOver = 1;
//    }
//    if (getPixel(state->player.x - 1, state->player.y + i) != BLACK) {
//        state->gameOver = 1;
//    }
//}
