
#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_


// define MACROs for the display registers
#define MCNTR 	0x00
#define SPIR  	0x01
#define SHCR	0x02
#define SMRR	0x03
#define SCPR	0x04
#define SLDR	0x08
#define SUDR	0x09
#define SLCR	0x0A
#define SUCR	0x0B
#define WDDI	0x0C
#define BCI		0x0E
#define BSI		0x0F
#define RDDI	0x0D
//---------------------------------------------------------------------------------------------------------------------

void lcd_datcmd(unsigned char addr_code, unsigned char value); // this function will be used for printing purposes
void portConfig();// this function will be used to configure all the ports of the Microkontroller
void NVICConfig();// this function will be used to configure NVIC
void disp_onoff(short on_off);// this function will be used to turn on and off the display
void configDisplay();// this function will be used to configure the display
void configUART(void); // this function will be used for configuring the UART

//----------------------------------------------------------------------------------------------------------------------
void printMatrix(unsigned char p[128][30]); // this function will be used to print the screen matrix on the display

#endif /* SYSCONFIG_H_ */
