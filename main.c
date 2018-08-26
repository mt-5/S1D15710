#include <avr/io.h>
#include "S1D15710.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000

#define BTN1  !(PINB & 1<<PB0)
#define ENCODER_A    !(PINB & 1<<PB1)
#define ENCODER_B    !(PINB & 1<<PB2)
#define BTN2  !(PINB & 1<<PB3)

volatile char up;
volatile unsigned char pos=1;
volatile unsigned char speed=1;

int main(void)
{
	
	DDRC = 0xff;
/*	PORTC = 0xff;
	
	DDRB= 0x00;
	PORTB= 0xFF;
	
	TCCR0 |= (1<<CS01) ;  //enable timer for buttons
	TIMSK |= (1<<TOIE0);           
	sei(); */
	
	LCD_Init();
	LCD_Clear(1);
    LCD_WriteText("MACIEJ :)", 90,27,1);
	LCD_DrawRect(0,0,218,59,0,1);
	while(1);
	return 0;  
}   
/*ISR(TIMER0_OVF_vect)             // 1ms for manual movement
{
	if(up) {
		if(ENCODER_A && !ENCODER_B) {
			for(uint8_t i=pos;i < pos+speed;i++)
				LCD_DrawLine(i,10,i,50,1);
			pos+=speed;
			up=0;
		}	
		else if(ENCODER_B && !ENCODER_A) {
			pos-=speed;
			for(uint8_t i=pos;i < pos+speed;i++)
				LCD_DrawLine(i,10,i,50,1);
			up=0;
		}
		
		if(BTN1) { 
			speed++;
			up=0;
		}
		else if(BTN2) {
			if(speed > 0) speed--;
			up=0;
		}
	}
	else {
		if(ENCODER_A | ENCODER_B | BTN1 | BTN2)
			up= 0;
		else
			up= 1;
	}
}*/



