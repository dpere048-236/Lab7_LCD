/*	Author: dpere048
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;

void TimerOn() {
	  TCCR1B = 0x0B;
	  OCR1A = 125;
	  TIMSK1 = 0x02; 
	  TCNT1=0;
	  _avr_timer_cntcurr = _avr_timer_M;
	  SREG |= 0x80;
}
void TimerOff() {
 TCCR1B = 0x00;
}
void TimerISR() {
 TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect) {
 _avr_timer_cntcurr--;
 if (_avr_timer_cntcurr == 0) {
  TimerISR();
  _avr_timer_cntcurr = _avr_timer_M;
 }
}
void TimerSet(unsigned long M) {
 _avr_timer_M = M;
 _avr_timer_cntcurr = _avr_timer_M;
}
enum States{start, up, down, zero} state;

unsigned char buttonA;
unsigned char buttonB;

unsigned char tmpC;


void Tick(){
	buttonA = ~PINA & 0x01;
	buttonB = ~PINA & 0x02;
	
	switch(state){
		case start:
			if(buttonA && !buttonB){
				state = up;
			}
			else if(!buttonA && buttonB){
				state = down;
			}
			else if(buttonA && buttonB){
				state = zero;
			}
			else
				state = start;
			break;
		case up:
			if(buttonA && !buttonB){
				state = up;
			}
			else if(buttonA && buttonB){
				state = zero;
			}
			else
				state = start;
			break;
		case down:
			if(!buttonA && buttonB){
				state = down;
			}
			else if(buttonA && buttonB){
				state = zero;
			}
			else
				state = start;
			
			break;
		case zero:
			if(buttonA && buttonB){
				state = zero;
			}
			else
				state = start;
			break;
	
	}
	switch(state){
		case startt:
			break;
		case up:
			if(tmpC < 9)
				tmpC = tmpC + 1;
			break;
		case down:
			if(tmpC > 0)
				tmpC = tmpC - 1;
			break;
		case zero:
			tmpC = 0;
			break;
	
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(1000);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	state = start;
	tmpC = 0x00;
    /* Insert your solution below */
    while (1) {
	LCD_Cursor(1);
	Tick();
	LCD_WriteData(tmpc + '0');
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
