#include "InterruptHandlers.h"
#include "snake.h"
#include "sysconfig.h"
#include "lm3s9b92.h"
#include "stdio.h"
// For Timer:

// Timer Interrupt Handling Function:
/** at each  counter wrap around we will:
 *1 clear the matrix
 *2 update the coordinates of the snake blocks
 *3 print the matrix on the display
**/
void timerIntHandler(void){
	clearMatrix(screenmatrix);//clear the screenmatrix
	movesnake();//update coordinates of the snake blocks and insert them into the screenmatrix

	//code to see the corners on the screen and check the coordinates of the screen margins
	//-----------------------------------
	insertBlock(0,0,screenmatrix);
	insertBlock(0,120,screenmatrix);
	insertBlock(29,0,screenmatrix);
	insertBlock(29,120,screenmatrix);
	//------------------------------------

	printMatrix(screenmatrix);//print the screenmatrix on the display
}


// For UART:

unsigned char newChar; // This global variable is used to store the pressed key, which will determine the snake movement direction

//This function contains the UART interrupt handling code:
void UARTIntHandler(void)
{
	newChar=UART2_DR_R; // store the pressed key
	printf("%x typed\n ",newChar); // print the hex ASCII value of the pressed key in the console
}





