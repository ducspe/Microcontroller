#ifndef INTERRUPTHANDLERS_H_
#define INTERRUPTHANDLERS_H_

//For timer:
void timerIntHandler();// Handle the Timer Interrupt

// For UART:

extern unsigned char newChar; // This global variable is used to store the pressed key, which will determine the snake movement direction
void UARTIntHandler(void);// Handle the UART Interrupt

#endif
