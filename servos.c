#include "servos.h"

#include <util/delay.h>

void servos_init() {
	for(uint8_t i = 0; i < ServoCount; i++) {
		*servos[i].ddr |= servos[i].mask;
		*servos[i].port &= servos[i].mask;
	}

	for(uint8_t i = 0; i < ServoCount; i++) {
		servo_set_min(i);
	}
}

void servo_set_min(uint8_t i) {
	for(uint8_t t = 0; t < 20; t++) {
		*servos[i].port |= servos[i].mask;
		_delay_us(900);
		*servos[i].port &= ~servos[i].mask;
		_delay_ms(15);
	}
}

void servo_set_max(uint8_t i) {
	for(uint8_t t = 0; t < 20; t++) {
		*servos[i].port |= servos[i].mask;
		_delay_ms(2);
		*servos[i].port &= ~servos[i].mask;
		_delay_ms(15);
	}
}
