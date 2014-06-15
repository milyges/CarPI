#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "power.h"
#include "i2c.h"
#include "pilot.h"

#define AVR_INT_DDR    DDRC
#define AVR_INT_PORT   PORTC
#define AVR_INT_PIN    PC2

uint8_t __registers[REG_MAX];

void avr_int(void) {
	AVR_INT_PORT |= (1 << AVR_INT_PIN);
	_delay_ms(20);
	AVR_INT_PORT &= ~(1 << AVR_INT_PIN);
}

void avr_int_init(void) {
	AVR_INT_DDR |= (1 << AVR_INT_PIN);
}

void temp_read(void) {
	uint16_t val;
	
	/* Termometr podpięty do ADC6 */
	ADMUX = (0 << REFS1) | (1 << REFS0) | (1 << MUX2) | (1 << MUX1);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	while(ADCSRA & (1 << ADSC)) ;  /* Czekamy na zakonczenie konwersji */
	val = ADC;
	
	__registers[REG_TEMP_LOW] = val & 0x7F;
	__registers[REG_TEMP_HIGH] = (val >> 7) & 0x7F;
}

void init(void) {	
	pilot_init();
	power_init();
	i2c_init();
	avr_int_init();
	sei();
}

int main(void) {
	uint8_t old_radio_state = 0, old_ignition_state = 0, old_power_state = REG_POWER_STATE_MASK;
	uint8_t radio_state, ignition_state, timeout, i, oldkey = 0, key, timer;
	
	_delay_ms(100);
	
	init();
	
	_delay_ms(1000);
	
	for(i = 0; i < REG_MAX; i++) {
		__registers[i] = 0x00;
	}
		
	timer = 0;
	while(1) {
		/* Co 256 * 50ms odczytaj temperaturę */
		if (timer++ == 0) {
			temp_read();
			avr_int();
		}
		
		radio_state = power_radio_state();
		ignition_state = power_ignition_state();

		if ((old_radio_state != radio_state) || (old_ignition_state != ignition_state)) {
			if (ignition_state)
				__registers[REG_POWER_STATE] |= REG_POWER_IGNITION;
			else
				__registers[REG_POWER_STATE] &= ~REG_POWER_IGNITION;
			
			if (radio_state)
				__registers[REG_POWER_STATE] |= REG_POWER_RADIO;
			else
				__registers[REG_POWER_STATE] &= ~REG_POWER_RADIO;
				
			/* Włączenie zapłonu lub radia */
			if ((radio_state) || (ignition_state)) {
				power_set_main(1);
				power_set_monitor(1);
				__registers[REG_POWER_STATE] |= REG_POWER_ENABLED;
				avr_int();
			}
			
			/* Wyłączenie zapłonu i radia */
			if ((!radio_state) && (!ignition_state)) {
				/* Zasilanie główne odcinamy tylko jak rpi będzie chciało */
				power_set_monitor(0);
				avr_int();
			}
			
			old_radio_state = radio_state;
			old_ignition_state = ignition_state;
			
		}
		
		key = pilot_loop();
		
		if (key != 0) {
			cli();
			for(i = 0; i < 8; i++) {
				if (!__registers[REG_KEY_0 + i]) {
					__registers[REG_KEY_0 + i] = key;					
					break;
				}
			}
			sei();
			
			avr_int();
		}
		else if (oldkey != 0) { /* Puszczenie klawisza */
			avr_int();
		}		
		
		oldkey = key;
		
		if (__registers[REG_POWER_STATE] != old_power_state) {
			if ((__registers[REG_POWER_STATE] & REG_POWER_ENABLED) == 0) { /* Wyłączenie zasilania RPi i uśpienie procka */
				timeout = 7;
				while(timeout--) {
					_delay_ms(1000);
				}
				power_set_main(0);
				power_set_monitor(0);
				_delay_ms(500);
				
				/* Jeżeli między czasie nie zostało włączone radio ani zapłon - usypiamy procka */
				if ((!power_radio_state()) && (!power_ignition_state())) {
					power_sleep();
				}
			}
			
			old_power_state = __registers[REG_POWER_STATE];
		}
		
		_delay_ms(50);
	}
}
