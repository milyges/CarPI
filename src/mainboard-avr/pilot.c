#include <avr/io.h>
#include <util/delay.h>
#include "pilot.h"

//#define PILOT_COL0_DDR     DDRD
//#define PILOT_COL0_PORT    PORTD
//#define PILOT_COL0_PIN     PIND
//#define PILOT_COL0_PINNO   PD5
#define PILOT_COL0_DDR     DDRB
#define PILOT_COL0_PORT    PORTB
#define PILOT_COL0_PIN     PINB
#define PILOT_COL0_PINNO   PB1

//#define PILOT_COL1_DDR     DDRD
//#define PILOT_COL1_PORT    PORTD
//#define PILOT_COL1_PIN     PIND
//#define PILOT_COL1_PINNO   PD7
#define PILOT_COL1_DDR     DDRB
#define PILOT_COL1_PORT    PORTB
#define PILOT_COL1_PIN     PINB
#define PILOT_COL1_PINNO   PB2

//#define PILOT_COL2_DDR     DDRD
//#define PILOT_COL2_PORT    PORTD
//#define PILOT_COL2_PIN     PIND
//#define PILOT_COL2_PINNO   PD6
#define PILOT_COL2_DDR     DDRB
#define PILOT_COL2_PORT    PORTB
#define PILOT_COL2_PIN     PINB
#define PILOT_COL2_PINNO   PB0

//#define PILOT_ROW0_DDR     DDRB
//#define PILOT_ROW0_PORT    PORTB
//#define PILOT_ROW0_PIN     PB1
#define PILOT_ROW0_DDR     DDRD
#define PILOT_ROW0_PORT    PORTD
#define PILOT_ROW0_PIN     PD5

//#define PILOT_ROW1_DDR     DDRB
//#define PILOT_ROW1_PORT    PORTB
//#define PILOT_ROW1_PIN     PB2
#define PILOT_ROW1_DDR     DDRD
#define PILOT_ROW1_PORT    PORTD
#define PILOT_ROW1_PIN     PD7

//#define PILOT_ROW2_DDR     DDRB
//#define PILOT_ROW2_PORT    PORTB
//#define PILOT_ROW2_PIN     PB0
#define PILOT_ROW2_DDR     DDRD
#define PILOT_ROW2_PORT    PORTD
#define PILOT_ROW2_PIN     PD6


#define COL0_GET()         (PILOT_COL0_PIN & (1 << PILOT_COL0_PINNO))
#define COL1_GET()         (PILOT_COL1_PIN & (1 << PILOT_COL1_PINNO))
#define COL2_GET()         (PILOT_COL2_PIN & (1 << PILOT_COL2_PINNO))

#define ROW0_OUT()         PILOT_ROW0_DDR |= (1 << PILOT_ROW0_PIN)
#define ROW0_IN()          PILOT_ROW0_DDR &= ~(1 << PILOT_ROW0_PIN)
#define ROW0_HIGH()        PILOT_ROW0_PORT |= (1 << PILOT_ROW0_PIN)
#define ROW0_LOW()         PILOT_ROW0_PORT &= ~(1 << PILOT_ROW0_PIN)

#define ROW1_OUT()         PILOT_ROW1_DDR |= (1 << PILOT_ROW1_PIN)
#define ROW1_IN()          PILOT_ROW1_DDR &= ~(1 << PILOT_ROW1_PIN)
#define ROW1_HIGH()        PILOT_ROW1_PORT |= (1 << PILOT_ROW1_PIN)
#define ROW1_LOW()         PILOT_ROW1_PORT &= ~(1 << PILOT_ROW1_PIN)

#define ROW2_OUT()         PILOT_ROW2_DDR |= (1 << PILOT_ROW2_PIN)
#define ROW2_IN()          PILOT_ROW2_DDR &= ~(1 << PILOT_ROW2_PIN)
#define ROW2_HIGH()        PILOT_ROW2_PORT |= (1 << PILOT_ROW2_PIN)
#define ROW2_LOW()         PILOT_ROW2_PORT &= ~(1 << PILOT_ROW2_PIN)

void pilot_init(void) {
	/* Kolumny jako wejścia podiągnięte do VCC */
	PILOT_COL0_DDR &= ~(1 << PILOT_COL0_PINNO);
	PILOT_COL1_DDR &= ~(1 << PILOT_COL1_PINNO);
	PILOT_COL2_DDR &= ~(1 << PILOT_COL2_PINNO);
	PILOT_COL0_PORT |= (1 << PILOT_COL0_PINNO);
	PILOT_COL1_PORT |= (1 << PILOT_COL1_PINNO);
	PILOT_COL2_PORT |= (1 << PILOT_COL2_PINNO);
	
	/* Wiersze jako wejścia, stan niski */	
	ROW0_IN();
	ROW1_IN();
	ROW2_IN();
	ROW0_LOW();
	ROW1_LOW();
	ROW2_LOW();
}

uint8_t pilot_loop(void) {
	int8_t roll_state;
	static int8_t old_roll_state = -1;
	uint8_t key = 0;
	
	roll_state = old_roll_state; /* Domyślnie brak ruchu rolki (unikamy stanu nieokreślonego) */
	
	/* ROW0 - wejście impulsatora rolki, przełączamy go w tryb wyjścia */
	ROW0_OUT();	
	_delay_us(20);
	if (!COL0_GET())
		roll_state = 0;
	if (!COL1_GET())
		roll_state = 2;
	if (!COL2_GET())
		roll_state = 1;	
	ROW0_IN(); /* Przełączamy go spowrotem w tryb wejścia, żeby nie zakłócał pozostałych pinów */
	//ROW0_HIGH();
	
	/* To samo dla wiersza 1 */
	ROW1_OUT();
	_delay_us(20);
	if (!COL0_GET())
		key |= PILOT_KEY_VOLUP;
	if (!COL1_GET())
		key |= PILOT_KEY_VOLDOWN;
	if (!COL2_GET())
		key |= PILOT_KEY_LOAD;
	ROW1_IN();
	
	/* I dla wiersza 2 */
	ROW2_OUT();
	_delay_us(20);
	if (!COL0_GET())
		key |= PILOT_KEY_PAUSE;
	if (!COL1_GET())
		key |= PILOT_KEY_SOURCE_L;
	if (!COL2_GET())
		key |= PILOT_KEY_SOURCE_R;
 	ROW2_IN();
	
	/* Obsługa rolki */
	if (roll_state != old_roll_state) {
		if (old_roll_state >= 0) {
			if (((old_roll_state + 1) % 3) == roll_state)
				key |= PILOT_ROLL_PREV;
			else
 				key |= PILOT_ROLL_NEXT;
		}
		
		old_roll_state = roll_state;
	}
	
	return key;
}
