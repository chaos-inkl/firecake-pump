#ifndef SERVOS_H
#define SERVOS_H

#include <stdint.h>
#include <avr/io.h>

struct ServoConfig {
    const uint8_t mask;
    volatile uint8_t *const port;
    volatile uint8_t *const ddr;
};

static const struct ServoConfig servos[1] = {
		{(1 << PD5), &PORTD, &DDRD},
};

static const uint8_t ServoCount = 1;



void servos_init(void);

void servo_set_min(uint8_t servo);
void servo_set_max(uint8_t servo);

#endif
