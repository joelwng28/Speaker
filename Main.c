// Main.c
// Runs on LM4F120/TM4C123
// Initialize all drivers and runs for loop to allow audio
// Samuel Zhang and Joel Wang
// 2/19/18

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void); // low power mode

#include <stdint.h>
#include "DAC.h"
#include "SysTick.h"
#include "PLL.h"
#include "Switch.h"
#include "Timer1.h"
#include "Music.h"
#include "../inc/tm4c123gh6pm.h"
#include "Sensor.h"
#include "Timer2.h"
#include "Sensor.h"
#include "Display.h"
#include "ST7735.h"
#include "Display.h"
#include "Globals.h"


#define PF1 (*((volatile uint32_t *)0x40025008))

Song global_Song;

//PortF_Init
//initialize heartbeat using PF1
//Input: None
//Output: None
void PortF_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x20;				// activate port F
	while((SYSCTL_PRGPIO_R&0x0020) == 0){};// ready?
	GPIO_PORTF_DIR_R |= 0x0E;					// make PF3-1 output (PF3-1 built-in LEDs)
	GPIO_PORTF_AFSEL_R &= ~0x0E;			// disable alt funct on PF3-1
	GPIO_PORTF_DEN_R |= 0x0E;					// enable digital I/O on PF3-1
																		// configure PF3-1 as GPIO
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
	GPIO_PORTF_AMSEL_R = 0;						// disable analog functionality on PF
}
	
void switchMain(void) {
	DisableInterrupts();
	PortF_Init();
	PLL_Init(Bus80MHz);
	Switches_Init();
	PF1 = 0x00;
	EnableInterrupts();
	
	while(1) {}
	
}

void timerMain(void) {
	DisableInterrupts();
	PortF_Init();
	PLL_Init(Bus80MHz);
	Timer2_Init();
	EnableInterrupts();
	
	while(1) {}
}

void digitsMain(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	ST7735_InitR(INITR_REDTAB);
	displayDigits(10, 29);
}

void weatherMain(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	ST7735_InitR(INITR_REDTAB);
	displayWeatherIcon(955);
}

void sensorMain(void) {
	DisableInterrupts();
	PLL_Init(Bus80MHz);
	PortF_Init();
	Sensor_Init();
	SysTick_Init();
	EnableInterrupts();
	
	while(1) {
	SysTick_Wait(4800000);
		if(Check_For_Movement()) {
			PF1 = 0x00;
		}
		else {
			PF1 = 0x02;
		}
	}
	
}


int main(void) {
	DisableInterrupts();
	PortF_Init();
	PLL_Init(Bus80MHz);
	Switches_Init();
	PF1 = 0x00;
	EnableInterrupts();
	
	while(1) {}
	
}
