#ifndef PUMPS_H
#define PUMPS_H

#include <stdint.h>

struct Pump {
    volatile enum {
        PUMP_UNKNOWN,
        PUMP_SETUP,
        PUMP_EMPTY,
        PUMP_FILL,
        PUMP_WAIT,
        PUMP_FULL,
        PUMP_DISPENSE
    } state;

    uint8_t empty_mask;
    volatile uint8_t *empty_pin;
    volatile uint8_t *empty_ddr;

    uint8_t full_mask;
    volatile uint8_t *full_pin;
    volatile uint8_t *full_ddr;

    uint8_t dir_mask;
    volatile uint8_t *dir_port;
    volatile uint8_t *dir_ddr;

    uint8_t step_mask;
    volatile uint8_t *step_port;
    volatile uint8_t *step_ddr;

    uint8_t servo;
    uint8_t servo_fill_pos;
    uint8_t servo_dispense_pos;
};

static const uint8_t PumpCount = 2;
extern struct Pump pumps[2];

void pumps_init(void);

void pumps_run(void);

void pump_enter_setup(uint8_t i);
void pump_do_setup(uint8_t i);

void pump_enter_empty(uint8_t i);
void pump_do_empty(uint8_t i);

void pump_enter_fill(uint8_t i);
void pump_do_fill(uint8_t i);

void pump_enter_wait(uint8_t i);
void pump_do_wait(uint8_t i);

void pump_enter_full(uint8_t i);
void pump_do_full(uint8_t i);

#endif
