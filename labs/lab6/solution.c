// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

//TODO
//1) Make PF2 an output and make PF4 an input (enable PUR for PF4). 	GPIO_PORTF_DIR_R = 0x0E;
//2) The system starts with the LED ON (make PF2 =1). 	GPIO_PORTF_PF2 = 1;
//3) Delay for about 100 ms
//4) If the switch is pressed (PF4 is 0), then toggle the LED once, else turn the LED ON.
//5) Repeat steps 3 and 4 over and over.

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC)) //Port F
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTF_LOCK_R 	(*((volatile unsigned long *)0x4C4F434B)) //unlock Port F
#define GPIO_PORTF_CR_R 	(*((volatile unsigned long *)0x1F)) //Allow changes to PF0-PF4
#define GPIO_PORTF_PF2		(*((volatile unsigned long *)0x40025010)) //Port F Pin 2
#define GPIO_PORTF_PF4		(*((volatile unsigned long *)0x40025040)) //Port F Pin 4
//#define SYSCTL_RCGC2_R     	0x00000020  // port F Clock Gating Control

//variables
unsigned long In; //Input from PF4
unsigned long Out; //Output from PF2 (blue LED)
unsigned long delay_time = 300000;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void PortF_init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020; //set clock for port F
	delay = SYSCTL_RCGC2_R; //delay to let the clock stabilize
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x00;
  	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AFSEL_R = 0x00;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R = 0x1F;
	
	GPIO_PORTF_PF2 = 4;
	
}

void Blink_LED() {
	int i;
	for (i = 0; i < delay_time; i++) {
			GPIO_PORTF_PF2 = 4;
	}
	for (i = 0; i < delay_time; i++) {
			GPIO_PORTF_PF2 = 0;
	}
}

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	PortF_init();
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here
		In = GPIO_PORTF_PF4; //Read PF4
		if (!In) {
			Blink_LED();
		}
		else {
			In = In>>2; //Shift into PF2
			GPIO_PORTF_PF2 = In;
		}
  }
}
