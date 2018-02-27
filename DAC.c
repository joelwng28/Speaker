// DAC.c
// DAC Driver
// Lab 5
// Samuel Zhang and Joel Wang
// 2/19/18

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"


//********DAC_Init*****************
// Initialize Max5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock rate less than 20 MHz
void DAC_Init(uint16_t data){
  SYSCTL_RCGCSSI_R |= 0x01;       // activate SSI0
  SYSCTL_RCGCGPIO_R |= 0x01;      // activate port A
  while((SYSCTL_PRGPIO_R&0x01) == 0){};// ready?
  GPIO_PORTA_AFSEL_R |= 0x2C;     // enable alt funct on PA2,3,5
  GPIO_PORTA_DEN_R |= 0x2C;       // configure PA2,3,5 as SSI
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;
  GPIO_PORTA_AMSEL_R = 0;         // disable analog functionality on PA
  SSI0_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI0_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI0_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI0_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI0_DR_R = data;               // load 'data' into transmit FIFO
  SSI0_CR1_R |= 0x00000002;       // enable SSI

}



//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t voltage){   
  while((SSI0_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
  SSI0_DR_R = voltage; }                // data out, no reply
