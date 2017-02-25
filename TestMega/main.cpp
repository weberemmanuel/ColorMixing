/*
 * TestMega.cpp
 *
 * Created: 2/18/2017 9:54:44 AM
 * Author : Emmanuel Weber
 */ 

#define F_CPU 800000UL 

#include <avr/io.h>
#include <util/delay.h>

int  adc_read (void);
void pwm_write (int val);

int main(void) 
{
	int adc_val;
	DDRB  = (1<<PB1) |(1<<PB2) |(1<<PB3)  ; //Set PB1 as output

	ADMUX = 0;
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS0) | (1 << ADEN);


	ADMUX |= (1 << ADLAR);   //Left-Adjust result.

	// Set clock prescaler (/8). See table 14-4.
	// (1<<ADEN) must be set to 1 to allow ADC.
	

    // timer 1
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);
	TCCR1A |= (1 << COM1A1);    // Enable OCR1A 
	TCCR1A |= (1 << COM1B1);    // Enable OCR1B

	// timer 2
	TCCR2A |= (1 << WGM20);
	TCCR2A |= (1 << WGM21);
	TCCR2B |= (1 << CS21);
	TCCR2A |= (1 << COM2A1);    // Enable OCR2A

    // Loop forever
	while (1)
	{
		// ADC5 -> OCR1A
		ADMUX |= (1 << MUX2) | (1 << MUX0);
		ADMUX &= ~(1 << MUX1);
		adc_val = adc_read();
		OCR1A = adc_val;

		// ADC4 -> OCR1B;
		ADMUX |= (1 << MUX2);
		ADMUX &= ~((1 << MUX1)| (1 << MUX0));
		adc_val = adc_read();
		OCR1B = adc_val;

		// ADC3 -> OCR2A
		ADMUX |= (1 << MUX1) | (1 << MUX0);
		ADMUX &= ~(1 << MUX2);
		adc_val = adc_read();
		OCR2A = adc_val;
	}
	return 0;
}

int adc_read (void)
{
	// Start the conversion
	ADCSRA |= (1 << ADSC);
	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));   //(1<<ADSC) returns 0 when the conversion is complete.
	return ADCH;
}

