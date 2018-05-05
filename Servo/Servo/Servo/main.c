/*
 * Servo.c
 *
 * Created: 4/13/2018 12:07:53 PM
 * Author : Oji
 */ 

#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int ADCvalue;
// Initialize ADC
void ADCinit(void)
{
	ADMUX |= (1<<REFS0);
	//start conversion | enable adc | enable interrupts | select 128 prescaler | auto trigger enable
	ADCSRA |= (1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADATE);
	ADCSRB = 0; // set adc in free running mode
	TCCR1A |= (1<<COM1A1)|(1<<WGM11); // Set timer1 in fast PWM mode | OCR1A clear on up and set down
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11); //prescaler of 1| fast pwm 
	ICR1 = 19000; //set to 19000 (50Mhz)
}

// Interrupt ADC
ISR (ADC_vect)
{
	ADCvalue=ADC;	//reads all 16-bits of ADCH:ADCL (0-1023 from pot)
	if(ADCvalue == 0){
		OCR1A = 1000; // 0 Degree
	}
	else if(ADCvalue == 5){
		OCR1A = 2000; // 180 Degree
	}
	_delay_ms(10);
}

int main()
{
	DDRC = 0x00; //make PORTC input pins
	DDRB = 0xFF; //make PORTB output
	ADCinit(); //initizlize ADC
	sei(); //set global interrupts
	while(1);
	return 0;
}