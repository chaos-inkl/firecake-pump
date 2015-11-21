#include "pumps.h"

#include "uart.h"
#include "servos.h"

struct Pump pumps[2];

void pumps_init() {
    for(uint8_t i = 0; i < PumpCount; i++){
        *pumps[i].empty_ddr &= ~pumps[i].empty_mask;
        *pumps[i].full_ddr &= ~pumps[i].full_mask;

        *pumps[i].dir_ddr |= pumps[i].dir_mask;
        *pumps[i].step_ddr |= pumps[i].dir_mask;

        pumps[i].state = PUMP_UNKNOWN;
    }
}

void pumps_run() {
    for(uint8_t i = 0; i < PumpCount; i++) {
        switch(pumps[i].state) {
            case PUMP_UNKNOWN:
                break;
            case PUMP_SETUP:
                pump_do_setup(i);
                break;
            case PUMP_EMPTY:
                pump_do_empty(i);
                break;
            case PUMP_FILL:
                pump_do_fill(i);
                break;
            case PUMP_FULL:
                pump_do_full(i);
                break;
            case PUMP_WAIT:
                pump_do_wait(i);
                break;
            case PUMP_DISPENSE:
                break;
            default:
                break;
        }
    }
}

/*
 * Pump setup state
 * ================
 */
void pump_enter_setup(uint8_t i) {
    uart_debug("Entering setup");
    servo_set_min(pumps[i].servo);

    // Start stepper and setup direction if pump is not empty already
    if(*pumps[i].empty_pin | pumps[i].empty_mask) {
        pumps[i].state = PUMP_SETUP;
        *pumps[i].dir_port |= pumps[i].dir_mask;
    }
    else {
        pump_enter_empty(i);
    }
}

void pump_do_setup(uint8_t i) {
    // Wait for pump to become empty
    if(*pumps[i].empty_pin | pumps[i].empty_mask) {
        pump_enter_empty(i);
    }
}


/*
 * Empty state
 * ===========
 * Marker state, does nothing
 */
void pump_enter_empty(uint8_t i) {
    uart_debug("Entering empty");
    pumps[i].state = PUMP_EMPTY;
}

void pump_do_empty(uint8_t i) {
    // We are empty so we just go to filling
    pump_enter_fill(i);
}


/*
 * Filling sate
 * ============
 */
void pump_enter_fill(uint8_t i) {
    uart_debug("Entering fill");
    pumps[i].state = PUMP_FILL;
    servo_set_min(pumps[i].servo);
    *pumps[i].dir_port &= ~pumps[i].dir_mask;
}

void pump_do_fill(uint8_t i) {
    // Wait for pump to become full
    if(*pumps[i].full_pin | pumps[i].full_mask) {
        pump_enter_wait(i);
    }
}

/*
 * Wait state
 * ==========
 * Wait for the syringe to become full
 */

void pump_enter_wait(uint8_t i) {
    uart_debug("Entering wait");
    pumps[i].state = PUMP_WAIT;
    //TODO: Save time here
}

void pump_do_wait(uint8_t i) {
    //TODO: Check time here
    pump_enter_full(i);
}


/*
 * Full state
 * ==========
 * Maker state not much happening here
 */
void pump_enter_full(uint8_t i) {
    uart_debug("Entering full");
    pumps[i].state = PUMP_FULL;
}

void pump_do_full(uint8_t i) {
    //Reserved for future use
}

/*
 * Dispense state
 * ==========
 */
void pump_enter_dispense(uint8_t i) {
    uart_debug("Entering dispense");
    pumps[i].state = PUMP_DISPENSE;
    servo_set_max(pumps[i].servo);
    *pumps[i].dir_port |= pumps[i].dir_mask;
}

void pump_do_dispense(uint8_t i) {
    // Wait for pump to become empty
    if(*pumps[i].empty_pin | pumps[i].empty_mask) {
        pump_enter_empty(i);
    }
}
