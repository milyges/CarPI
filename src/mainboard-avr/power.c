#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "power.h" 

#define POWER_MONITOR_DDR     DDRD
#define POWER_MONITOR_PORT    PORTD
#define POWER_MONITOR_PIN     PD1

#define POWER_MAIN_DDR        DDRC
#define POWER_MAIN_PORT       PORTC
#define POWER_MAIN_PIN        PC0

#define POWER_RADIO_DDR       DDRD
#define POWER_RADIO_PORT      PORTD
#define POWER_RADIO_PIN       PIND
#define POWER_RADIO_PINNO     PD3

#define POWER_INGITION_DDR    DDRD
#define POWER_INGITION_PORT   PORTD
#define POWER_INGITION_PIN    PIND
#define POWER_INGITION_PINNO  PD2

ISR(INT0_vect) {
	
}

void power_init(void) {
	POWER_MONITOR_PORT &= ~(1 << POWER_MONITOR_PIN);
	POWER_MONITOR_DDR |= (1 << POWER_MONITOR_PIN);
	
	POWER_MAIN_PORT &= ~(1 << POWER_MAIN_PIN);
	POWER_MAIN_DDR |= (1 << POWER_MAIN_PIN);
	
	POWER_RADIO_DDR &= ~(1 << POWER_RADIO_PINNO);
	POWER_RADIO_PORT |= (1 << POWER_RADIO_PINNO);
	
	POWER_INGITION_DDR &= ~(1 << POWER_INGITION_PINNO);
	POWER_INGITION_PORT |= (1 << POWER_INGITION_PINNO);
	
	/* Przerwanie */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	MCUCR &= ~((1 << ISC01) | (1 << ISC00));
	
}

void power_set_monitor(char enabled) {
	if (enabled)
		POWER_MONITOR_PORT |= (1 << POWER_MONITOR_PIN);
	else
		POWER_MONITOR_PORT &= ~(1 << POWER_MONITOR_PIN);
}

void power_set_main(char enabled) {
	if (enabled)
		POWER_MAIN_PORT |= (1 << POWER_MAIN_PIN);
	else
		POWER_MAIN_PORT &= ~(1 << POWER_MAIN_PIN);
}

char power_radio_state(void) {	
	return ((POWER_RADIO_PIN & (1 << POWER_RADIO_PINNO)) == 0);
}

char power_ignition_state(void) {
	return ((POWER_INGITION_PIN & (1 << POWER_INGITION_PINNO)) == 0);
}

void power_sleep(void) {
	GIMSK |= (1 << INT0);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
	GIMSK &= ~(1 << INT0);
}
