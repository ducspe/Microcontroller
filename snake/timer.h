#ifndef _TIMER_H_
#define _TIMER_H

void wait(); // software wait function
//-----------------------------------------------------------------------------------------------
void configTimer(void); // configure the timer
void timerWait(unsigned int usec); //usec is the value in �s  after which the timer reaches time-out.

#endif

