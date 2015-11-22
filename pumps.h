#ifndef PUMPS_H
#define PUMPS_H

#include <stdint.h>

struct Pump {
    const uint8_t stepper;
    const uint8_t servo;
};

enum PumpState {
    PUMP_UNKNOWN,
    PUMP_SETUP,
    PUMP_FILL,
    PUMP_WAIT,
    PUMP_FULL,
    PUMP_DISPENSE
};

static const uint8_t PumpCount = 1;
extern enum PumpState pump_states[1];

static const struct Pump pumps[1] = {
    {0,0}
};

void pumps_init(void);

void pumps_run(void);

void pump_enter_setup(uint8_t i);
void pump_do_setup(uint8_t i);

void pump_enter_fill(uint8_t i);
void pump_do_fill(uint8_t i);

void pump_enter_wait(uint8_t i);
void pump_do_wait(uint8_t i);

void pump_enter_full(uint8_t i);
void pump_do_full(uint8_t i);

void pump_enter_dispense(uint8_t i);
void pump_do_dispense(uint8_t i);

#endif
