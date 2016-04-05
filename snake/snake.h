
#ifndef SNAKE_H_
#define SNAKE_H_

//#include  "lm3s9b92.h"
//#include  "stdio.h"

// Create a "Point" type to store the coordinates of the snake/fruit building blocks
typedef struct Point {
	int x;  // horizontal coordinate (to account for the horizontal movement)
	int y;  // vertical coordinate (to account for the vertical movement)
} Point;

//variable declarations:
extern unsigned char screenmatrix[128][30];// this matrix is initialized in snake.c with 0x00 's in all cells

//function declarations:
void clearMatrix(unsigned char p[128][30]); // clear the screenmatrix (we use TV principle of refreshing the display)
void generateFruit(); // generate the random coordinates of the fruit
//----------------------------------------------------------------------

void movesnake(); // updates snake coordinates at each timer wrap around
void insertBlock(int posx, int posy,unsigned char p[128][30]); // inserts a standard 8x8 block on the screen

#endif /* SNAKE_H_ */
