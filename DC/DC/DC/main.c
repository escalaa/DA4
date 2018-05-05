/*
 * DC.c
 *
 * Created: 4/16/2018 6:00:33 PM
 * Author : Oji
 */ 

#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile unsigned int pushed = 0;  // if the motor is on or off
volatile unsigned int ADCvalue;
// Initiate ADC
void ADCinit(void)
{
	ADMUX|=(1<<REFS0);	//use Vcc as ref (0.1uF cap  attached)
	ADCSRA|=(1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADATE); // start conversion | enable adc| enable interrupts | select 128 prescaler | auto trigger enable | ADC0 used
	ADCSRB=0; //initialize ADC in free running mode
	TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00); // the use timer0 in fast PWM mode and set OC0A on bottom then to clear on up counting
	TCCR0B|=(1<<CS00); //set the prescaler of 1 to be selected
}

// Interrupt Potentiometer
ISR (ADC_vect)
{
	ADCvalue=ADC;	//reads all 16-bits of ADCH:ADCL (0-1023 from pot)
	ADCvalue=ADCvalue/4; //divide ADC value by 4 to get 0-225 value
	OCR0A=ADCvalue; //set this value to the OCR0A register
	_delay_ms(10); //delay to allow OCR0A to hold
}

// Interrupt Push Button
ISR (INT0_vect)
{	
	if(pushed == 0){
		pushed = 1;
	}
	else{
		OCR0A = 0;
		pushed = 0;
	}
	_delay_ms(10);
}

int main()
{
	DDRC = 0x00; //make PORTC input pins
	DDRD = 0xFB; //PIND2 as Input and the rest of PORTD as output
	
	ADCinit(); //initialize 328 (ADC/timers)
	sei(); //enable global interrupts
	while(1);
	return 0;
}
