/* ******** Switch.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/20/2018
 * switch init, debouncing, and function control
 * Lab 5
 * Saadallah
 * 2/20/2018
 */

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "Music.h"
#include "Main.h"
#include "SysTick.h"
#include "ADCSWTrigger.h"


#define PE4 (*((volatile uint32_t *)0x40024040))
#define PB4 (*((volatile uint32_t *)0x40005040))
#define PC4 (*((volatile uint32_t *)0x40006040))
#define PD1 (*((volatile uint32_t *)0x40007008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//flags for timer to handle interrupts
static uint8_t PEIntFlag = 0;
static uint8_t PCIntFlag = 0;
static uint8_t PBIntFlag = 0;
static uint8_t PDIntFlag = 0;

static uint8_t Play_Mode = 0; // high = currently playing

//registers for holding previous state of ports to ensure no debouncing
volatile static unsigned long PELast;
volatile static unsigned long PCLast;
volatile static unsigned long PBLast;
volatile static unsigned long PDLast;


//*************Timer0Arm********************************************
//  Initializes one-shot 20ms timer for debouncing
//  Inputs: None
//  Output: none
static void Timer0Arm(void){
	SYSCTL_RCGCTIMER_R |= 0x01;
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER0_TAILR_R = 320000;      // 4) 20ms reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
}

//*************GPIOArm********************************************
//  Reenables switch interrupts
//  Inputs: None
//  Output: none
void GPIOArm(void) {
	if(PEIntFlag) {
		PELast = PE4;					//switch state
		PEIntFlag = 0;
		GPIO_PORTE_ICR_R = 0x10;      //clear flag4
		GPIO_PORTE_IM_R |= 0x10;      //arm interrupt on PE4 
		NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5

	}
	else if(PCIntFlag) {
		PCLast = PC4;					//switch state
		PCIntFlag = 0;
		GPIO_PORTC_ICR_R = 0x10;      //clear flag4
		GPIO_PORTC_IM_R |= 0x10;      //arm interrupt on PC4 
		NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5

	}
	else if(PBIntFlag) {
		PBLast = PB4;					//switch state
		PBIntFlag = 0;
		GPIO_PORTB_ICR_R = 0x10;      //clear flag4
		GPIO_PORTB_IM_R |= 0x10;      //arm interrupt on PB4 

		NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
		
	}
	
	else if(PDIntFlag) {
		PDLast = PD1;					//switch state
		PDIntFlag = 0;
		GPIO_PORTD_ICR_R = 0x02;      //clear flag4
		GPIO_PORTD_IM_R |= 0x02;      //arm interrupt on PD1 

		NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
		
	}
}

//*************Timer0A_Handler********************************************
//  Disarms Timer0A and reeanbles appropriate switch interrupt
//  Inputs: None
//  Output: none
void Timer0A_Handler(void){
  TIMER0_IMR_R = 0x00000000;    	// disarm timeout interrupt
  GPIOArm();   										// start GPIO
}


//*************Switches_Init********************************************
//  Initialize "Up" "Down" and "Select" Switches
//  Inputs: None
//  Output: none
void Switches_Init(void) {
	//initialize PE4 (this will be the switch for "Select")
	SYSCTL_RCGCGPIO_R |= 0x16; 	//activate clock for Port E and Port C and Port B
	SYSCTL_RCGCGPIO_R |= 0x08; 	//activate clock for Port D
	GPIO_PORTE_DIR_R &= ~0x10; 	//make PE4 in
	GPIO_PORTE_DEN_R |= 0x10;		//digital enable PE4
	GPIO_PORTE_AFSEL_R &= ~0x10; 	// disable alt funct on PE4
	GPIO_PORTE_AMSEL_R &= ~0x10;	// disable analog on PE4
	GPIO_PORTE_PCTL_R &= ~0x000F0000; // configure PE4 as GPIO
	GPIO_PORTE_IS_R &= ~0x10;		//PE4 is edge sensitive
	GPIO_PORTE_IBE_R |= 0x10;	//PE4 is both edges
	GPIO_PORTE_IM_R |= 0x10; 		//arm interrupt on PE4
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	//initialize PC4 (this will be the switch for "Up")
	GPIO_PORTC_DIR_R &= ~0x10; 	//make PC4 in
	GPIO_PORTC_DEN_R |= 0x10;		//digital enable PC4
	GPIO_PORTC_AFSEL_R &= ~0x10; 	// disable alt funct on PC4
	GPIO_PORTC_AMSEL_R &= ~0x10;	// disable analog on PC4
	GPIO_PORTC_PCTL_R &= ~0x000F0000; // configure PE4 as GPIO
	GPIO_PORTC_IS_R &= ~0x10;		//PC4 is edge sensitive
	GPIO_PORTC_IBE_R |= 0x10;	//PC4 is both edges
	GPIO_PORTC_IM_R |= 0x10; 		//arm interrupt on PC4
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5
	//initialize PB4 (this will be the siwtch for "Down")
	GPIO_PORTB_DIR_R &= ~0x10; 	//make PB4 in
	GPIO_PORTB_DEN_R |= 0x10;		//digital enable PB4
	GPIO_PORTB_AFSEL_R &= ~0x10; 	// disable alt funct on PB4
	GPIO_PORTB_AMSEL_R &= ~0x10;	// disable analog on PB4
	GPIO_PORTB_PCTL_R &= ~0x000F0000; // configure PB4 as GPIO
	GPIO_PORTB_IS_R &= ~0x10;		//PB4 is edge sensitive
	GPIO_PORTB_IBE_R |= 0x10;	//PB4 is both edges
	GPIO_PORTB_IM_R |= 0x10; 		//arm interrupt on PB4
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
	//initialize PD1
	GPIO_PORTD_DIR_R &= ~0x02; 	//make PD1 in
	GPIO_PORTD_DEN_R |= 0x02;		//digital enable PD1
	GPIO_PORTD_AFSEL_R &= ~0x02; 	// disable alt funct on PD1
	GPIO_PORTD_AMSEL_R &= ~0x02;	// disable analog on PD1
	GPIO_PORTD_PCTL_R &= ~0x000000F0; // configure PD1 as GPIO
	GPIO_PORTD_IS_R &= ~0x02;		//PD1 is edge sensitive
	GPIO_PORTD_IBE_R |= 0x02;	//PD1 is both edges
	GPIO_PORTD_IM_R |= 0x02; 		//arm interrupt on PD1
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
	
	NVIC_EN0_R = 16;				//Enable interrupts for PortE
	NVIC_EN0_R = 8;				//Enable interrupts for PortD
	NVIC_EN0_R = 4;				//Enable interrupts for PortC
	NVIC_EN0_R = 2;				//Enable interrupts for PortB

	PELast = PE4;					//Set initial state
	PBLast = PB4;					//Set initial state
	PCLast = PC4;					//Set initial state
	PDLast = PD1;					//Set initial state
}

long sr;	//for critical sections

//*************Play********************************************
//  Plays music if music was paused
//  Inputs: None
//  Output: none
void Play(void) {
	Play_Mode = 1;
	//initialize timers to begin playing
	
	TIMER1_CTL_R = 0x01;
	NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
	NVIC_ST_CTRL_R = 0x7;
}

//*************Pause********************************************
// Pauses music
//  Inputs: None
//  Output: none
void Pause(void) {
	Play_Mode = 0;
	
	//disable interrupts
	NVIC_ST_CTRL_R = 0x0;
	TIMER1_CTL_R = 0x00000000;
}
//*************Rewind********************************************
//  Play music from the start
//  Inputs: None
//  Output: none
void Rewind(void) {
	Pause();
	noteIndex = 0;
	NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
	duration_Counter = 0;
}

//*************Mode********************************************
//  Changes the voice/instrument of the sound
//  Inputs: None
//  Output: none
void Mode(void) {
	if(instrument_Mode != 4) {
		instrument_Mode++;
	}
	else {
		instrument_Mode = 0;
	}
	
}

//*************SetPointWADC********************************************
//  use ADC value to set where music starts up
//  Inputs: None
//  Output: none
void SetPointWADC(void) {
	uint32_t adcSample = ADC0_InSeq3();
	duration_Counter = 0;
	noteIndex = (adcSample * global_Song.note_Num)/4096;
	NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
	
}


//*************GPIOPortE_Handler********************************************
//  Completes Play/Pause Functionality
//  Inputs: None
//  Output: none
void GPIOPortE_Handler(void) {
	GPIO_PORTE_IM_R &= ~0x10;		//disarm intterupt on PE4
	PEIntFlag = 1;
	if(!PELast) {
		sr = StartCritical();						//start critical section
		if(Play_Mode == 0){ Play(); }
		else if(Play_Mode == 1) { Pause(); }
		
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer0Arm();								//start one shot
}

//*************GPIOPortC_Handler********************************************
//  Completes Rewinds Functionality
//  Inputs: None
//  Output: none
void GPIOPortC_Handler(void) {
	GPIO_PORTC_IM_R &= ~0x10;		//disarm intterupt on PC4
	PCIntFlag = 1;
	if(!PCLast) {
		sr = StartCritical();						//start critical section
		Rewind();
		EndCritical(sr);				//end critical section
	}
	else {}
	
	Timer0Arm();								//start one shot
}

//*************GPIOPortB_Handler********************************************
//  Completes Change Mode Functionality
//  Inputs: None
//  Output: none
void GPIOPortB_Handler(void) {
	GPIO_PORTB_IM_R &= ~0x10;		//disarm intterupt on PB4
	PBIntFlag = 1;
	if(!PBLast) {
		sr = StartCritical();						//start critical section
		Mode();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer0Arm();								//start one shot
}

//*************GPIOPortD_Handler********************************************
//  Completes Set ADC Functionality
//  Inputs: None
//  Output: none
void GPIOPortD_Handler(void) {
	GPIO_PORTD_IM_R &= ~0x02;		//disarm intterupt on PB4
	PDIntFlag = 1;
	if(!PDLast) {
		sr = StartCritical();						//start critical section
		SetPointWADC();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer0Arm();								//start one shot
}






