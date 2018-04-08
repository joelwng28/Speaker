 /* ******** AlarmClock.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/6/2018
 * global main methods
 * Lab 3
 * Saadallah
 * 2/6/2018
 */
 
#include <stdint.h>
extern uint8_t global_Hour;
extern uint8_t current_State;

extern uint8_t global_Hour; extern uint8_t global_Min; extern uint8_t global_Sec;
extern uint8_t alarm1_Hour; extern uint8_t alarm1_Min; extern uint8_t alarm1_Sec; extern uint8_t alarm1_Enable;
extern uint8_t alarm2_Hour; extern uint8_t alarm2_Min; extern uint8_t alarm2_Sec; extern uint8_t alarm2_Enable;
extern uint8_t snooze_Hour; extern uint8_t snooze_Min; extern uint8_t snooze_Sec; extern uint8_t snooze_Enable;
extern uint8_t stopwatch_Hour; extern uint8_t stopwatch_Min; extern uint8_t stopwatch_Sec; extern uint8_t stopwatch_Enable;

extern uint8_t clock_Mode;

extern uint8_t interrupt_Flag;

extern uint8_t alarm_Lock;

extern uint8_t dayTime;

//State Values
#define MAINTOGGLE 			0
#define MAINSETTIME 		1
#define MAINALARM1			2
#define MAINALARM2			3
#define MAINSTOPWATCH		4
#define	SETTIMEHHOVER		5
#define SETTIMEH				6
#define SETTIMEMHOVER		7
#define SETTIMEM				8
#define SETTIMEBACK			9
#define A1HHOVER				 	10
#define A1H				 			11
#define A1MHOVER					12
#define A1M							13
#define A1ENABLE				14
#define A1BACK					15
#define A2HHOVER				16
#define A2H							17
#define A2MHOVER				18
#define A2M							19
#define A2ENABLE				20
#define A2BACK					21
#define SWTOGGLE				22
#define SWRESET					23
#define SWBACK					24
#define ALARMSNOOZE			25
#define ALARMSTOP				26

