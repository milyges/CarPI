#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <string.h>
#include "i2c.h" 

extern uint8_t __registers[];

void i2c_init(void) {
	TWAR = I2C_ADDRESS << 1;
	TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);	
}

ISR(TWI_vect) {
	static uint8_t idx = 0;
	uint8_t tmp;
	
	if ((TWSR & 0xF8) == TW_SR_SLA_ACK) { /* Własny adres potwierdzony */
                TWCR |= (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
		idx = 0;
        }
        else if ((TWSR & 0xF8) == TW_SR_DATA_ACK) { /* Tryb zapisu */
		__registers[REG_POWER_STATE] = TWDR & REG_POWER_STATE_MASK; /* Odczytujemy polecenie */
		TWCR |= (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWEN); /* Bez potwierdzenia */
		idx = 0;
	}
        else if ((TWSR & 0xF8) == TW_ST_DATA_ACK) { /* Tryb odczytu */
		tmp = TWDR;
		
		TWDR = __registers[idx];
		if (idx >= REG_KEY_0) /* Odczytane kody klawiszy zerujemy */
			__registers[idx] = 0x00;		
		idx++;
		
		if (idx < REG_MAX) {
			TWCR |= (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
		}
		else {
			TWCR |= (1 << TWIE) | (1 << TWINT) | (0 << TWEA) | (1 << TWEN);
			idx = 0;
		}		
	}
        else {
		TWCR |= (1 << TWIE) | (1 << TWEA) | (1 << TWEN);
		idx = 0;
	}
}
