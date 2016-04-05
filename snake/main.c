#define  PART_LM3S9B92

#include "sysconfig.h" // for configuring the display, the ports, NVIC and UART
#include "snake.h" // to generate the first fruit on the screen
#include "timer.h" // for configuring the timer and for the wait() function
#include "stdio.h" // for printf()

int main (void)
{
	//Configure all ports
	portConfig();
	wait();
	printf("portConfig\n");
	//-----------------------------------------------------
	//Configure NVIC
	NVICConfig();
	wait();
	printf("NVICConfig\n");
	//-----------------------------------------------------
	//Configure the display:
	configDisplay();
	wait();
	printf("configDisplay\n");
	//-----------------------------------------------------
	//Configure the UART module:
	configUART();
	wait();
	printf("configUART\n");
	// Configure Timer Module:
	configTimer();
	wait();
	printf("timerConfig\n");

	generateFruit();// generate the first fruit in the game

	while(1){
	}

	/**
	  This loop will be periodically interrupted by the Timer Interrupt and the UART Interrupt
	  UART Interrupt has higher priority than the Timer interrupt
	  UART Interrupt sets the direction in which the snake moves.

	  Timer interrupt will:
	  1)update the snake vector using the movesnake() function
	  2)insert the snake blocks in the screenmatrix using the coordonates from the updated snake vector
	  3)print the screenmatrix on the display using the printMatrix(unsigned char p[128][30]) function from "sysconfig.c"
	**/
}



