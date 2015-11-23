#include "pumps.h"

#include "uart.h"
#include "servos.h"
#include "steppers.h"

enum PumpState pump_states[2];
uint32_t pump_wait_start[2];

void pumps_init() {
    for(uint8_t i = 0; i < PumpCount; i++){
        pump_states[i] = PUMP_UNKNOWN;
    }
}

void pumps_run() {
    for(uint8_t i = 0; i < PumpCount; i++) {
        switch(pump_states[i]) {
            case PUMP_UNKNOWN:
                break;
            case PUMP_SETUP:
                pump_do_setup(i);
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
    servo_set_max(pumps[i].servo);

    // Start stepper and setup direction if pump is not empty already
    if(!stepper_at_max(i)) {
        stepper_set_mode(pumps[i].stepper, STEPPER_INTERN);
        stepper_set_dir(pumps[i].stepper, STEPPER_FORWARD);
        pump_states[i] = PUMP_SETUP;
    }
    else {
        pump_enter_fill(i);
    }
}

void pump_do_setup(uint8_t i) {
    // Wait for pump to become empty
    if(stepper_at_max(i)) {
        pump_enter_fill(i);
    }
}

/*
 * Filling sate
 * ============
 */
void pump_enter_fill(uint8_t i) {
    uart_debug("Entering fill");
    pump_states[i] = PUMP_FILL;

    servo_set_min(pumps[i].servo);
    stepper_set_mode(pumps[i].stepper, STEPPER_INTERN);
    stepper_set_dir(pumps[i].stepper, STEPPER_BACKWARD);
}

void pump_do_fill(uint8_t i) {
    // Wait for pump to become full
    if(stepper_at_min(pumps[i].stepper)) {
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
    pump_states[i] = PUMP_WAIT;

    stepper_set_mode(pumps[i].stepper, STEPPER_OFF);

    pump_wait_start[i] = now();
}

void pump_do_wait(uint8_t i) {
    if(since(pump_wait_start[i]) > 30) {
        pump_enter_full(i);
    }
}


/*
 * Full state
 * ==========
 * Maker state not much happening here
 */
void pump_enter_full(uint8_t i) {
    uart_debug("Entering full");
    pump_states[i] = PUMP_FULL;
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
    pump_states[i] = PUMP_DISPENSE;
    servo_set_max(pumps[i].servo);

    stepper_set_mode(pumps[i].stepper, STEPPER_EXTERN);
    stepper_set_dir(pumps[i].stepper, STEPPER_FORWARD);
}

void pump_do_dispense(uint8_t i) {
    // Wait for pump to become empty
    if(stepper_at_max(pumps[i].stepper)) {
        pump_enter_fill(i);
    }
}
