#define F_CPU 8000000UL 
#include <avr/io.h> 
#include <avr/iotn45.h>
#include <util/delay.h> 

#define PWM PB0
#define DIM PB1
#define BCKLIGT PB3
#define OFF PB4

int brightness = 0;

int main(void) 
{
	/* set input and output ports */
	DDRB &= ~(1<<BCKLIGT);
	DDRB |= (1<<PWM)|(1<<OFF);

	// Timer0, Set OC0A on comp. match (inv). Mode 3: Fast PWM 
	TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(1<<WGM00); 
	// 1:8 presc. 
	TCCR0B = (1<<CS01); 

	ADCSRA |= (1 << ADEN)| // Analog-Digital enable bit
		      (1 << ADPS1)| // set prescaler to 8 (clock / 8)
		      (1 << ADPS2); // set prescaler to 8 (clock / 8)

	ADMUX &= 0xF0; //Clearing all 4 mux

	ADMUX |= (1 << ADLAR)| // AD result store in (more significant bit in ADCH)
		     (1 << MUX0); // Choose AD input ADC1 (PB1)

    
	for (;;) {
		if (bit_is_clear(PINB, BCKLIGT)) {
			PORTB |= (1 << OFF);
		} else {
			PORTB &= ~(1 << OFF);
		}

		ADCSRA |= (1 << ADEN); // Analog-Digital enable bit
		ADCSRA |= (1 << ADSC); // Discarte first conversion
    	while (ADCSRA & (1 << ADSC)); // wait until conversion is done
    	ADCSRA |= (1 << ADSC); // start single conversion
    	while (ADCSRA & (1 << ADSC)) // wait until conversion is done
    	ADCSRA &= ~(1<<ADEN); // shut down the ADC

		brightness = 240 - ADCH;
		brightness /= 2;
		OCR0A = brightness; 
		_delay_ms (3);
   	} 
	return 0;
}
