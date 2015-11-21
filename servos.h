#ifndef SERVOS_H
#define SERVOS_H

#include <stdint.h>
#include <avr/io.h>

struct ServoConfig {
    uint8_t mask;
    volatile uint8_t *port;
    volatile uint8_t *ddr;
};

static const struct ServoConfig servos[2] = {
		{(1 << PB1), &PORTB, &DDRB},
		{(1 << PB2), &PORTB, &DDRB}
};

static const uint8_t ServoCount = 2;



void servos_init(void);

void servo_set_min(uint8_t servo);
void servo_set_max(uint8_t servo);

#endif
