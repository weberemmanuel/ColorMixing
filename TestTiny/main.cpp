/*
 * TestTiny.cpp
 *
 * Created: 2/12/2017 5:17:14 PM
 * Author : Emmanuel Weber
 */ 

#define F_CPU 960000

#include <avr/io.h>
#include <util/delay.h>

void pwm_write (int val);
int adc_read (void);

int main(void) {
	int adc_val;
	DDRB = (1<<PB1); //Set PB1 as output
	PORTB = 0;
	ADMUX |= (1 << MUX1);   //Select input channel ADC2 (PB4). See Table 14-3 of ATtiny13 Datasheet.
	ADMUX |= (1 << ADLAR);   //Left-Adjust result.

	// Set clock prescaler (/8). See table 14-4.
	// (1<<ADEN) must be set to 1 to allow ADC.
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
	TCCR0B |= (1 << CS01) | (1 << CS00); //PWM uses timer0, so set timer0 prescaler (table 11-9)
	TCCR0A |= (1 << WGM01) | (1 << WGM00); 	//Set to 'Fast PWM' mode. (Table 11-8)

	// Clear OC0B output on compare match, upwards counting.
	TCCR0A |= (1 << COM0B1);

	while (1){
		adc_val = adc_read();
		pwm_write(adc_val);
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

void pwm_write (int val)
{
	OCR0B = val;
}