/*
 * Stepper.c
 *
 * Created: 4/13/2018 9:51:32 AM
 * Author : Oji
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile unsigned int ADCvalue;

// CTC delay subroutine
void CTCdelay(unsigned int adc)
{
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS10); // CTC mode | set prescaler to 1
	TCNT1 = 0; // intialize counter
	OCR1A = adc; // compare value
}

// Initialize ADC
void ADCinit(void)
{
	ADMUX|=(1<<REFS0);
	//start conversion | enable adc | enable interrupts | select 128 prescaler | auto trigger enable
	ADCSRA|=(1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADATE);
	ADCSRB=0; //set adc in free running mode
}

// interrupt ADC
ISR (ADC_vect)
{
	ADCvalue=ADC; //reads all 16-bits of ADCH:ADCL (0-1023 from pot)
	PORTB = 0x66; //first step to be outputted
	CTCdelay(ADCvalue); // Delay for ADC value
	PORTB = 0xCC;  //second step to be outputted
	CTCdelay(ADCvalue); // Delay for ADC value
	PORTB = 0x99; //third step to be outputted
	CTCdelay(ADCvalue); // Delay for ADC value
	PORTB = 0x33; // fourth step to be outputted
	CTCdelay(ADCvalue); // Delay for ADC value
}

int main()
{
	DDRC = 0x00; //make PORTC input pins
	DDRD = 0xFF; //make PORTD output pin
	ADCinit(); //initialize adc
	sei(); //enable global interrupts
	while(1);
	return 0;
}
