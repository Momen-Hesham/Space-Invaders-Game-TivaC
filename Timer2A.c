#include "..//tm4c123gh6pm.h"
#include "Timer2A.h"
#include "Sound.h"
//Array Of Vlaues used to creat sinwave to generate sound
//const unsigned char SineWave[16] = {4,4,6,8,10,12,14,15,15,14,12,10,8,6,4,2};
//unsigned char Index=0;

void (*fun_ptr)(void) ;
void Timer2A_Init(void (*PlayFunction)(void),unsigned long period)
{ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
//  TimerCount = 0;
//  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
	fun_ptr=PlayFunction;
}

void Timer2A_Handler(void)
{ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
	(*fun_ptr)();
}
