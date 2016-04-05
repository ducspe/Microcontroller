#include "snake.h"
#include "timer.h"
#include "sysconfig.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h" // use the time as a seed for the rand() function
#include "InterruptHandlers.h"
#include "lm3s9b92.h"

// Declare the global variables:
int SNAKE_SIZE = 5; //Global variable that stores the size of the snake (initial size being 5 by default)
unsigned char screenmatrix[128][30] = {0x00}; // screen matrix to be printed at each timer interrupt

//Point data structure is declared in "snake.h"
Point snake[100]={{0,0},{1,0},{2,0},{3,0},{4,0}}; // array storing the coordinates of the snake blocks
Point Fruit; // global variable to store the coordinates of the fruit


//Function:

// clear the screen
void clearMatrix(unsigned char p[128][30]){
    int i,j;
    for(i=0;i < 128; i++){
        for(j=0;j < 30;j++){
            p[i][j] = 0x00;
        }
    }
}

// turn on all the pixels of the screen (currently it is not used in the program)
void fillMatrix(unsigned char p[128][30]){
    int i,j;
    for(i=0;i < 128; i++){
        for(j=0;j < 30;j++){
            p[i][j] = 0xFF;
        }
    }
}

// insert a standard block into the screen matrix
void insertBlock(int posx, int posy,unsigned char p[128][30]){
    int i;
    for(i=0;i<8;i++){
        p[posy+i][posx] = 0xFF;
    }
}


// Prints GO
void printGO(){
	clearMatrix(screenmatrix);

	//create the G

	insertBlock(13,40,screenmatrix);
	insertBlock(14,40,screenmatrix);

	insertBlock(12,40,screenmatrix);
	insertBlock(12,48,screenmatrix);
	insertBlock(12,56,screenmatrix);
	insertBlock(12,64,screenmatrix);
	insertBlock(12,72,screenmatrix);


	insertBlock(13,72,screenmatrix);
	insertBlock(14,72,screenmatrix);

	insertBlock(14,56,screenmatrix);
	insertBlock(14,64,screenmatrix);



	//Create the O

	insertBlock(17,40,screenmatrix);

	insertBlock(16,40,screenmatrix);
	insertBlock(16,48,screenmatrix);
	insertBlock(16,56,screenmatrix);
	insertBlock(16,64,screenmatrix);
	insertBlock(16,72,screenmatrix);

	insertBlock(17,72,screenmatrix);

	insertBlock(18,40,screenmatrix);
	insertBlock(18,48,screenmatrix);
	insertBlock(18,56,screenmatrix);
	insertBlock(18,64,screenmatrix);
	insertBlock(18,72,screenmatrix);

	printMatrix(screenmatrix); // print screenmatrix
	timerWait(2000000); // wait 2 seconds

}

// print OUCH  when you hit yourself or the wall
void printOUCH(){

	//Create the O

	insertBlock(7,40,screenmatrix);

	insertBlock(6,40,screenmatrix);
	insertBlock(6,48,screenmatrix);
	insertBlock(6,56,screenmatrix);
	insertBlock(6,64,screenmatrix);
	insertBlock(6,72,screenmatrix);

	insertBlock(7,72,screenmatrix);

	insertBlock(8,40,screenmatrix);
	insertBlock(8,48,screenmatrix);
	insertBlock(8,56,screenmatrix);
	insertBlock(8,64,screenmatrix);
	insertBlock(8,72,screenmatrix);
	//-----------------------------------------------------------
	//Create the U

	insertBlock(10,40,screenmatrix);
	insertBlock(10,48,screenmatrix);
	insertBlock(10,56,screenmatrix);
	insertBlock(10,64,screenmatrix);
	insertBlock(10,72,screenmatrix);

	insertBlock(11,72,screenmatrix);

	insertBlock(12,40,screenmatrix);
	insertBlock(12,48,screenmatrix);
	insertBlock(12,56,screenmatrix);
	insertBlock(12,64,screenmatrix);
	insertBlock(12,72,screenmatrix);
	//------------------------------------------------------------------
	//create the C

	insertBlock(15,40,screenmatrix);
	insertBlock(16,40,screenmatrix);

	insertBlock(14,40,screenmatrix);
	insertBlock(14,48,screenmatrix);
	insertBlock(14,56,screenmatrix);
	insertBlock(14,64,screenmatrix);
	insertBlock(14,72,screenmatrix);


	insertBlock(15,72,screenmatrix);
	insertBlock(16,72,screenmatrix);

	//Create the H

	insertBlock(18,40,screenmatrix);
	insertBlock(18,48,screenmatrix);
	insertBlock(18,56,screenmatrix);
	insertBlock(18,64,screenmatrix);
	insertBlock(18,72,screenmatrix);

	insertBlock(19,56,screenmatrix);

	insertBlock(20,40,screenmatrix);
	insertBlock(20,48,screenmatrix);
	insertBlock(20,56,screenmatrix);
	insertBlock(20,64,screenmatrix);
	insertBlock(20,72,screenmatrix);


	//Create the !

	insertBlock(22,40,screenmatrix);
	insertBlock(22,48,screenmatrix);
	insertBlock(22,56,screenmatrix);

	insertBlock(22,72,screenmatrix);

	printMatrix(screenmatrix);
	timerWait(2000000); //wait 2 seconds
}


//This function will be useful when we will need to reset the game after a loss
//Come to the configuration:  snake[100]={{0,0},{1,0},{2,0},{3,0},{4,0}}
void resetSnake(){
	int i;
	for(i=0;i<5;i++){
		snake[i].x = i;
		snake[i].y = 0;
	}
	SNAKE_SIZE = 5; // return to initial snake size
	newChar = 'd';// set direction to "right"

}

// This function will create the coordinates of the Fruit block
// These coordinates must be within a certain range
// Later on create a condition to avoid generating the fruit in the snake body or head.
void generateFruit(){
	int i = 0;
	int fruitInBody = 0; // will be used to make sure the fruit is not generated in the snake body
	int x,y;// x and y coordinates of the fruit
	int random_number;

	srand ( time(NULL) ); // use time value as a seed for the random function generator
	random_number = rand(); // create a random number


	// The fruit will never be generate along the display margins.
	// x coordinate will be between 1 and 28
	// y coordinate will be between 8 and 112 (y is a multiple of 8)
	// This is done to decrease the probability of heating the wall when the fruit is generated right on the screen border.

	while(1){

		fruitInBody = 0;
		x = (random_number % 28) + 1; // generate random numbers between 1 and 28
		y = ((random_number % 14) + 1) * 8; // generate random numbers between 8 and 112 which are multiples of 8 !!!

		for(i=0;i<SNAKE_SIZE;i++){
			if((snake[i].x == x) && (snake[i].y == y)){ // check if the random coordinates match the coordinates of one of the snake's blocks
				fruitInBody = 1; // fruitInBody = true
			}
		}
		if(!fruitInBody) {
			break;
		}
		else{
			random_number += 31;
		}// if the fruit is not in the snake body or head, break out of the loop and set the coordinates of the fruit
		 // else iterate through the while loop again after incrementing the random_number by 31(or whatever value)
	}

	//Set Fruit coordinates
	Fruit.x=x;
	Fruit.y=y;

}

void gameoverroutine(){
	//see the coordinates of the wall in the console
	printf("\n x coordinate is: %d\n",snake[SNAKE_SIZE-1].x);
	printf("\n y coordinate is: %d\n",snake[SNAKE_SIZE-1].y);
	//---------------------------------------------------------------------------------------------------------------------------
	while(1) //break out of the loop when 'q' is pressed
	{
		printOUCH(); // Print OUCH
		//UART_READ(); // if you use this function instead of the lines below you get a blinking text instead of a static one
		while(UART2_FR_R & (1<<4)); // wait for Rx FIFO not empty
		newChar=UART2_DR_R; // here the character is stored

		if(newChar == 'q') { // 'q' key is used to resume/restart the game
			resetSnake(); // reset the coordinates of the snake so that it appears on the top-left corner of the screen
			break; // break out of this loop
		}
	}
	printGO(); // print GO after pressing 'q'.
}

void insertSnakeAndFruit(){ // this function will insert the entire snake including the head
	int i;
	// insert the snake into the matrix(including the head)
	for(i=0;i < SNAKE_SIZE;i++){
		insertBlock(snake[i].x,snake[i].y, screenmatrix);
	}

	// insert the fruit into the matrix as well
	insertBlock(Fruit.x, Fruit.y, screenmatrix);

}

void detectSelfCollision(){
	int i;
	for(i=0;i < SNAKE_SIZE-1;i++){ // Note that i will not be set to the (SNAKE_SIZE-1) value, which contains the snake Head.
		if((snake[SNAKE_SIZE-1].x == snake[i].x) && (snake[SNAKE_SIZE-1].y == snake[i].y))
		{   // check if the coordinates of the head are the same as the coordinates of any of the body blocks
			gameoverroutine();
		}
	}
}

void updatesnakebody(){
	int i;
	for(i=0; i < SNAKE_SIZE-1;i++){ // iterate only till the head position(excluding head)
		snake[i].x=snake[i+1].x;
		snake[i].y=snake[i+1].y;
	}
}

void moveright(){
	updatesnakebody();

	// Update head coordinates
	snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-1].x+1; //we move right and keep y unchanged
	snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-1].y;

	//---------------------------------------------------------------------

	// After we updated the entire snake we start checking for the special cases:

	// detect wall collision
	if(snake[SNAKE_SIZE-1].x == 30){ // check the updated head position
		gameoverroutine();
		return; // get out of this function
	}

	//  fruit collision routine
	if ((snake[SNAKE_SIZE-1].x == Fruit.x) && (snake[SNAKE_SIZE-1].y == Fruit.y)){
		SNAKE_SIZE = SNAKE_SIZE + 1; // first increase the snake size

		//Next update the coordinates of the head
		snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-2].x + 1; // increase the x coordinate to move right
		snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-2].y; // y coordinate of the head remains unchanged

		generateFruit(); // generate new fruit after eating the previous one
	}

	//detect self-collision
	detectSelfCollision();

	//----------------------------------------------------------

	//insert the entire snake and the fruit.
	insertSnakeAndFruit();
}

//-------------------------------------------------------------------------------------------------------

void moveleft(){
	updatesnakebody();

	//update head coordinates
	snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-1].x-1; // move left and keep y unchanged
	snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-1].y;

	//detect wall collision
	if(snake[SNAKE_SIZE-1].x == -1){ // check the updated head position
		gameoverroutine();
		return; // get out of this function
	}

	//  fruit collision routine

	if ((snake[SNAKE_SIZE-1].x == Fruit.x) && (snake[SNAKE_SIZE-1].y == Fruit.y)){
		SNAKE_SIZE = SNAKE_SIZE + 1; // first increase the snake size

		// Then update the coordinates of the head
		snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-2].x - 1; // decrease the x coordinate of the head
		snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-2].y;// y coordinate of the head remains unchanged

		generateFruit(); // generate new fruit after eating the previous one

	}

	//detect self-collision
	detectSelfCollision();

	//----------------------------------------------------------
	// insert snake and fruit into the screenmatrix
	insertSnakeAndFruit();
}

void movedown(){
	updatesnakebody();

	//Update head coordinates
	snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-1].x;
	snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-1].y+8; // increase the y coordinate by 8 to go down and keep the x coordinate unchanged

	//detect wall collision
	if(snake[SNAKE_SIZE-1].y == 128)
	{ // check the updated head position
		gameoverroutine();
		return;// get out of this function
	}

	// detect and handle fruit collision

	if ((snake[SNAKE_SIZE-1].x == Fruit.x) && (snake[SNAKE_SIZE-1].y == Fruit.y)){
		SNAKE_SIZE = SNAKE_SIZE + 1; // increase snake size

		//update head position
		snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-2].x; // x coordinate of the head remains unchanged
		snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-2].y + 8;// increase the y coordinate of the head by 8

		generateFruit(); // generate new fruit after eating the previous one

	}


	//detect self-collision
	detectSelfCollision();

	//----------------------------------------------------------
	// insert snake and fruit into the screenmatrix
	insertSnakeAndFruit();
}

void moveup(){
	updatesnakebody();

	// Update head coordinates
	snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-1].x;
	snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-1].y-8;

	//detect wall collision
	if(snake[SNAKE_SIZE-1].y == -8 )
	{ // check the updated head position
		gameoverroutine();
		return; // get out of this function in case of a wall collision
	}

	// detect fruit collision and handle fruit collision

	if ((snake[SNAKE_SIZE-1].x == Fruit.x) && (snake[SNAKE_SIZE-1].y == Fruit.y)){
		SNAKE_SIZE = SNAKE_SIZE + 1; // increase snake size

		//update the head coordinates
		snake[SNAKE_SIZE-1].x = snake[SNAKE_SIZE-2].x; // x coordinate remains unchanged
		snake[SNAKE_SIZE-1].y = snake[SNAKE_SIZE-2].y - 8;// decrease the y coordinate of the head by 8

		generateFruit(); // generate new fruit after eating the previous one

	}

	//detect self-collision
	detectSelfCollision();

	//----------------------------------------------------------
	// insert snake and fruit into the screenmatrix
	insertSnakeAndFruit();

}

// this function will be called in the Timer Interrupt Handling function
void movesnake(){

	if(newChar == 'a'){
		moveleft();
	}
	else if(newChar == 's'){
		movedown();
	}
	else if(newChar == 'd'){
		moveright();
	}
	else if(newChar == 'w'){
		moveup();
	}
}








