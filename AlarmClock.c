/* ******** ADCTestMain.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 1/28/2018
 * Sampling ADC Channel 0 and Time Jitter Calculations
 * Lab 2
 * Saadallah
 * 1/31/2018
 */

// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include <stdio.h>
#include "string.h"
#include "ST7735.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "Timer3.h"
#include "Sound.h"
#include "Globals.h"
#include "Time.h"
#include "AlarmClock.h"
#include "Display.h"
#include "Switch.h"

uint8_t current_State = MAINTOGGLE; //default state is Main Screen with Toggle Menu

//Global Variables for Representing Time
uint8_t global_Hour = 0; uint8_t global_Min = 0; uint8_t global_Sec = 0;
uint8_t alarm1_Hour = 0; uint8_t alarm1_Min = 0; uint8_t alarm1_Sec = 0; uint8_t alarm1_Enable = 1;
uint8_t alarm2_Hour = 0; uint8_t alarm2_Min = 0; uint8_t alarm2_Sec = 0; uint8_t alarm2_Enable = 1;
uint8_t snooze_Hour = 0; uint8_t snooze_Min = 0; uint8_t snooze_Sec = 0; uint8_t snooze_Enable = 1;
uint8_t stopwatch_Hour = 0; uint8_t stopwatch_Min = 0; uint8_t stopwatch_Sec = 0; uint8_t stopwatch_Enable = 0;

uint8_t clock_Mode = 0; //Global variable for determining which clock face (0-3)

uint8_t interrupt_Flag = 0;

uint8_t alarm_Lock = 0;

uint8_t dayTime = 1;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int main(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	ST7735_InitR(INITR_REDTAB);
	SysTick_Init();
	Timer3_Init();
	Sound_Init();
	Switches_Init();
	EnableInterrupts();
	displayDigits(10, 29);
}

int main2(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	ST7735_InitR(INITR_REDTAB);
	SysTick_Init();
	Timer3_Init();
	Sound_Init();
	Switches_Init();
	EnableInterrupts();
	displayWeatherIcon(955);
}

int main1(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	ST7735_InitR(INITR_REDTAB);
	SysTick_Init();
	Timer3_Init();
	Sound_Init();
	Switches_Init();
	EnableInterrupts();
	alarm1_Hour = 0;
	alarm1_Min = 1;
	alarm1_Sec = 0;
	alarm1_Enable = 0;

	while(1){
		if(alarm_Lock) {
			switch(current_State) {
				case ALARMSNOOZE:
					displayAlarmLockSnooze();
					break;
				case ALARMSTOP:
					displayAlarmLockStop();
					break;
			}
			Sound_On();
			for(int i = 0; i < 100000; i++) {}
		}
		else {
			switch(current_State) {
				case MAINTOGGLE:
					displayMain("Toggle");
					break;
				case MAINSETTIME:
					displayMain("Set Time");
					break;
				case MAINALARM1:
					displayMain("Alarm 1");
					break;
				case MAINALARM2:
					displayMain("Alarm 2");
					break;
				case MAINSTOPWATCH:
					displayMain("Stopwatch");
					break;
				case SETTIMEHHOVER:
					displaySetTimeHourSelect();
					break;
				case SETTIMEH:
					displaySetTimeHour();
					break;
				case SETTIMEMHOVER:
					displaySetTimeMinSelect();
					break;
				case SETTIMEM:
					displaySetTimeMin();
					break;
				case SETTIMEBACK:
					displaySetTimeBack();
					break;
				case A1HHOVER:
					displayAlarm1HourSelect();
					break;
				case A1H:
					displayAlarm1Hour();
					break;
				case A1MHOVER:
					displayAlarm1MinSelect();
					break;
				case A1M:
					displayAlarm1Min();
				case A1ENABLE:
					displayAlarm1Enable();
					break;
				case A1BACK:
					displayAlarm1Back();
					break;
				case A2HHOVER:
					displayAlarm2HourSelect();
					break;
				case A2H:
					displayAlarm2Hour();
					break;
				case A2MHOVER:
					displayAlarm2MinSelect();
					break;
				case A2M:
					displayAlarm2Min();
				case A2ENABLE:
					displayAlarm2Enable();
					break;
				case A2BACK:
					displayAlarm2Back();
					break;
				case SWTOGGLE:
					displaySWSRS();
					break;
				case SWRESET:
					displaySWReset();
					break;
				case SWBACK:
					displaySWBack();
					break;
				case ALARMSNOOZE:
					displayAlarmLockSnooze();
					break;
				case ALARMSTOP:
					displayAlarmLockStop();
					break;
		}
	}
	while(interrupt_Flag == 0) {
		if(alarm_Lock) {	
				Sound_On();
				for(int i = 0; i < 100000; i++) {}
			}
	}
	interrupt_Flag = 0;
		
	}

}






