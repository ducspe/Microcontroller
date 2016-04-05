
#include "timer.h"
#include "lm3s9b92.h"
#include "snake.h"
//Software wait function:
//Is used to create a small delay so that the ports are correctly configured.
void wait(){
	int tmp;
	for(tmp=0;tmp<10;tmp++);
}
//----------------------------------------------------------------------------------------------
//Function for configuring the timer
void configTimer(void)
{
	int waitcycle=0;
	// configure Timer 0
	SYSCTL_RCGC1_R |= (1<<16);
	waitcycle++; // wait for timer 0 activation
	TIMER0_CTL_R &= ~0x0001; // disable Timer 0
	TIMER0_CFG_R = 0x04; // 2 x 16-bit mode
	TIMER0_TAMR_R = 0x02; // periodic mode
	TIMER0_TAPR_R = 123-1; // prescaler PR= ceil(16000000/2^16*0.5)-1
}
//----------------------------------------------------------------------------------------------
// Hardware wait function:
void timerWait(unsigned int usec) //usec is the value in µs  after which the timer reaches time-out.
{

	TIMER0_TAILR_R= (usec*16)/123; //  Set the interval load value
	TIMER0_CTL_R |= 0x0001; // Enable the timer
	while((TIMER0_RIS_R & (1<<0))==0); // Wait for the time-out flag of the timer to be set.
	TIMER0_ICR_R |= (1<<0); //clear the interrupt flag that was set previously.
	TIMER0_CTL_R &= ~0x0001; //disable the timer
}







