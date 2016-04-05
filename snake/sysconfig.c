#include "sysconfig.h"
#include "lm3s9b92.h"
#include "stdio.h"
#include "inc/hw_types.h" 		// required for SysCtlClockSet
#include "driverlib/sysctl.h"

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

// this function will be used for printing purposes
void lcd_datcmd(unsigned char addr_code, unsigned char value)
{
	int busy=0;

	do {
		GPIO_PORTE_DATA_R |=(1<<2);   			// RS = 1
		GPIO_PORTE_DATA_R |=(1<<3);				// RW = 1
		SysCtlDelay(4);
		GPIO_PORTE_DATA_R |=(1<<0);				// enable = 1
		SysCtlDelay(12);
		busy =((GPIO_PORTE_DATA_R&0x02)>>1);    // save busy flag
		GPIO_PORTE_DATA_R &=~1;				    // set enable to 0
		SysCtlDelay(12);
	}
	while(busy);

	GPIO_PORTE_DATA_R |=(1<<2);
	GPIO_PORTE_DATA_R &=~(1<<3);
	SysCtlDelay(4);
	GPIO_PORTE_DATA_R |=(1<<0);
	SysCtlDelay(12);
	GPIO_PORTD_DATA_R =addr_code;
	SysCtlDelay(6);
	GPIO_PORTE_DATA_R &=~1;
	SysCtlDelay(12);


	GPIO_PORTE_DATA_R &=~(1<<2);
	GPIO_PORTE_DATA_R &=~(1<<3);
	SysCtlDelay(6);
	GPIO_PORTE_DATA_R |=(1<<0);
	SysCtlDelay(12);
	GPIO_PORTD_DATA_R =value;
	SysCtlDelay(4);
	GPIO_PORTE_DATA_R &=~1;
	SysCtlDelay(12);

}

// this function will be used to configure all the ports of the Microkontroller
void portConfig(){

	SysCtlClockSet(SYSCTL_SYSDIV_12_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SYSCTL_RCGC2_R |=  (1<<3)|(1<<4)|(1<<5)|(1<<8); // enable port D, PORT E, PORT F, PORT J
	SYSCTL_RCGC2_R |= 0x00000040;
	SYSCTL_RCGC1_R |= (0x00000004);

	GPIO_PORTG_DEN_R |= 0x01; // Enable PG0
	GPIO_PORTG_DIR_R &= ~0x01; // Set direction of PG0 to output
	GPIO_PORTG_AFSEL_R |= 0x001; // Use alternate function select because we will use UART
	GPIO_PORTG_PCTL_R |= 0x00000001;

	GPIO_PORTD_DEN_R=0xFF;	 	// Enable all the bits of port D
	GPIO_PORTD_DIR_R = 0xFF;	// All the bits are outputs

	GPIO_PORTE_DEN_R=0x0F; // Enable Port E bits
	GPIO_PORTE_DIR_R = 0x0D; // The busy bit is input , the rest of the bits are outputs

	UART2_IM_R = 1<<4;      // activate UART2 Receiver

}

void NVICConfig(){
	NVIC_ST_RELOAD_R = 40000000-1; 		// setting systick reload value to 500ms
	NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_ENABLE|NVIC_ST_CTRL_INTEN); // Enable interrupt for timer
	NVIC_EN1_R |= 1<<(33-32);           // enable UART2 IRQ in NVIC(IRQ 33)
}


// This function will be used to turn on and off the display
void disp_onoff(short on_off)
{
	if (on_off!=0)
		lcd_datcmd(MCNTR,0x32);
	else
		lcd_datcmd(MCNTR,0x12);
}

// this function will be used to configure the display
void configDisplay(){
	disp_onoff(0); // turn off display
	lcd_datcmd( SPIR , 0x07);
	lcd_datcmd( SHCR, 0x1D);
	lcd_datcmd( SMRR, 0x7F);
	lcd_datcmd( SCPR, 0x00);
	lcd_datcmd( SLDR, 0x00);
	lcd_datcmd( SUDR, 0x00);
	lcd_datcmd( SLCR, 0x00);
	lcd_datcmd( SUCR, 0x00);
	disp_onoff(1); // turn on display
}

//--------------------------------------------------------------------------------------------------------------------
//For configuring the UART
void configUART(void)

{
	int waitCycle = 0; // dummy variable for a small delay time
	SYSCTL_RCGC1_R |= 0x00000004; // switch on clock for UART2
	waitCycle++; // short delay for stable clock
	UART2_CTL_R &= ~0x0001; // disable UART2 for configuration
	UART2_IBRD_R = 8; // 16M/16/115200
	UART2_FBRD_R = 44; // set DIVFRAC of BRD
	UART2_LCRH_R = 0x00000060; // serial format 8N1
	UART2_CTL_R |= 0x0001; // re-enable UART2
}



//---------------------------------------------------------------------------------------------------------------------

// this function will be used to print the screen matrix on the display
// use the TV refresh principle

void printMatrix(unsigned char p[128][30]){

	int i,j;
	disp_onoff(0); // turn off the display

	lcd_datcmd( SLCR, 0x00);	// cursor is set to beginning of display
	lcd_datcmd( SUCR, 0x00);

	for (i=0;i<128;i++){
		for (j=0;j<30;j++){
			lcd_datcmd( WDDI, p[i][j]); // print the contents of the matrix on the display
		}
	}
	disp_onoff(1);  // turn on the display

}



