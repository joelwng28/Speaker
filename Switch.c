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


#define PE3 (*((volatile uint32_t *)0x40024020))
#define PB3 (*((volatile uint32_t *)0x40005020))
#define PC6 (*((volatile uint32_t *)0x40006100))
#define PD2 (*((volatile uint32_t *)0x40007010))
	
#define PF1 (*((volatile uint32_t *)0x40025008))

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

//registers for holding previous state of ports to ensure no debouncing
volatile static unsigned long PELast;
volatile static unsigned long PCLast;
volatile static unsigned long PBLast;
volatile static unsigned long PDLast;


//*************Timer0Arm********************************************
//  Initializes one-shot 20ms timer for debouncing
//  Inputs: None
//  Output: none
static void Timer3Arm(void){
	SYSCTL_RCGCTIMER_R |= 0x08;
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER3_TAILR_R = 320000;      // 4) 20ms reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
}

//*************GPIOArm********************************************
//  Reenables switch interrupts
//  Inputs: None
//  Output: none
void GPIOArm(void) {
	if(PEIntFlag) {
		PELast = PE3;					//switch state
		PEIntFlag = 0;
		GPIO_PORTE_ICR_R = 0x08;      //clear flag3
		GPIO_PORTE_IM_R |= 0x08;      //arm interrupt on PE3
		NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5

	}
	else if(PCIntFlag) {
		PCLast = PC6;					//switch state
		PCIntFlag = 0;
		GPIO_PORTC_ICR_R = 0x40;      //clear flag6
		GPIO_PORTC_IM_R |= 0x40;      //arm interrupt on PC6
		NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5

	}
	else if(PBIntFlag) {
		PBLast = PB3;					//switch state
		PBIntFlag = 0;
		GPIO_PORTB_ICR_R = 0x08;      //clear flag3
		GPIO_PORTB_IM_R |= 0x08;      //arm interrupt on PB3 

		NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
		
	}
	
	else if(PDIntFlag) {
		PDLast = PD2;					//switch state
		PDIntFlag = 0;
		GPIO_PORTD_ICR_R = 0x04;      //clear flag2
		GPIO_PORTD_IM_R |= 0x04;      //arm interrupt on PD2 

		NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
		
	}
}

//*************Timer0A_Handler********************************************
//  Disarms Timer0A and reeanbles appropriate switch interrupt
//  Inputs: None
//  Output: none
void Timer3A_Handler(void){
  TIMER3_IMR_R = 0x00000000;    	// disarm timeout interrupt
  GPIOArm();   										// start GPIO
}


//*************Switches_Init********************************************
//  Initialize "Up" "Down" and "Select" Switches
//  Inputs: None
//  Output: none
void Switches_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x16; 	//activate clock for Port E and Port C and Port B
	SYSCTL_RCGCGPIO_R |= 0x08; 	//activate clock for Port D
	//initialize PE3
	GPIO_PORTE_DIR_R &= ~0x08; 	//make PE3 in
	GPIO_PORTE_DEN_R |= 0x08;		//digital enable PE3
	GPIO_PORTE_AFSEL_R &= ~0x08; 	// disable alt funct on PE3
	GPIO_PORTE_AMSEL_R &= ~0x08;	// disable analog on PE3
	GPIO_PORTE_PCTL_R &= ~0x000F000; // configure PE3 as GPIO
	GPIO_PORTE_IS_R &= ~0x08;		//PE3 is edge sensitive
	GPIO_PORTE_IBE_R |= 0x08;	//PE3 is both edges
	GPIO_PORTE_IM_R |= 0x08; 		//arm interrupt on PE3
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	//initialize PC6 
	GPIO_PORTC_DIR_R &= ~0x40; 	//make PC6 in
	GPIO_PORTC_DEN_R |= 0x40;		//digital enable PC6
	GPIO_PORTC_AFSEL_R &= ~0x40; 	// disable alt funct on PC6
	GPIO_PORTC_AMSEL_R &= ~0x40;	// disable analog on PC6
	GPIO_PORTC_PCTL_R &= ~0x0F000000; // configure PC6 as GPIO
	GPIO_PORTC_IS_R &= ~0x40;		//PC6 is edge sensitive
	GPIO_PORTC_IBE_R |= 0x40;	//PC6 is both edges
	GPIO_PORTC_IM_R |= 0x40; 		//arm interrupt on PC6
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5
	//initialize PB3
	GPIO_PORTB_DIR_R &= ~0x08; 	//make PB3 in
	GPIO_PORTB_DEN_R |= 0x08;		//digital enable PB3
	GPIO_PORTB_AFSEL_R &= ~0x08; 	// disable alt funct on PB3
	GPIO_PORTB_AMSEL_R &= ~0x08;	// disable analog on PB3
	GPIO_PORTB_PCTL_R &= ~0x000F000; // configure PB3 as GPIO
	GPIO_PORTB_IS_R &= ~0x08;		//PB3 is edge sensitive
	GPIO_PORTB_IBE_R |= 0x08;	//PB3 is both edges
	GPIO_PORTB_IM_R |= 0x08; 		//arm interrupt on PB3
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
	//initialize PD2
	GPIO_PORTD_DIR_R &= ~0x04; 	//make PD2 in
	GPIO_PORTD_DEN_R |= 0x04;		//digital enable PD2
	GPIO_PORTD_AFSEL_R &= ~0x04; 	// disable alt funct on PD2
	GPIO_PORTD_AMSEL_R &= ~0x04;	// disable analog on PD2
	GPIO_PORTD_PCTL_R &= ~0x00000F00; // configure PD2 as GPIO
	GPIO_PORTD_IS_R &= ~0x04;		//PD2 is edge sensitive
	GPIO_PORTD_IBE_R |= 0x04;	//PD2 is both edges
	GPIO_PORTD_IM_R |= 0x04; 		//arm interrupt on PD2
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
	
	NVIC_EN0_R = 16;				//Enable interrupts for PortE
	NVIC_EN0_R = 8;				//Enable interrupts for PortD
	NVIC_EN0_R = 4;				//Enable interrupts for PortC
	NVIC_EN0_R = 2;				//Enable interrupts for PortB

	PELast = PE3;					//Set initial state
	PBLast = PB3;					//Set initial state
	PCLast = PC6;					//Set initial state
	PDLast = PD2;					//Set initial state
}

long sr;	//for critical sections


void Up(void) {
	PF1 ^= 0x02;
}


void Down(void) {
	PF1 ^= 0x02;
}


void Select(void) {
	PF1 ^= 0x02;
}

void Music(void) {
	PF1 ^= 0x02;
}

//*************GPIOPortE_Handler********************************************
//  Completes Up Functionality
//  Inputs: None
//  Output: none
void GPIOPortE_Handler(void) {
	GPIO_PORTE_IM_R &= ~0x08;		//disarm intterupt on PE3
	PEIntFlag = 1;
	if(!PELast) {
		sr = StartCritical();						//start critical section
		Up();
		
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}

//*************GPIOPortC_Handler********************************************
//  Completes Down Functionality
//  Inputs: None
//  Output: none
void GPIOPortC_Handler(void) {
	GPIO_PORTC_IM_R &= ~0x40;		//disarm intterupt on PC6
	PCIntFlag = 1;
	if(!PCLast) {
		sr = StartCritical();						//start critical section
		Down();
		EndCritical(sr);				//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}

//*************GPIOPortB_Handler********************************************
//  Completes Select Functionality
//  Inputs: None
//  Output: none
void GPIOPortB_Handler(void) {
	GPIO_PORTB_IM_R &= ~0x08;		//disarm intterupt on PB3
	PBIntFlag = 1;
	if(!PBLast) {
		sr = StartCritical();						//start critical section
		Select();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}

//*************GPIOPortD_Handler********************************************
//  Completes Music Functionality
//  Inputs: None
//  Output: none
void GPIOPortD_Handler(void) {
	GPIO_PORTD_IM_R &= ~0x04;		//disarm intterupt on PD2
	PDIntFlag = 1;
	if(!PDLast) {
		sr = StartCritical();						//start critical section
		Music();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}






