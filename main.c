#include "uart.h"
#include "servos.h"
#include "pumps.h"
#include "config.h"
#include "steppers.h"


int main(void) {
    uart_init();
    servos_init();
    steppers_init();
    pumps_init();

    *idle_ddr &= ~idle_mask;
    *idle_port |= idle_mask;
    *setup_ddr &= ~setup_mask;
    *setup_port |= setup_mask;
    *ready_ddr |= ready_mask;


    sei();

    uart_puts("PumpControl 0.1 ready\n\r");


    uint8_t active_pump = 0;

    while(1) {
        pumps_run();

        // Setup is active low
        if(!(*setup_pin & setup_mask)) {
            for(uint8_t i = 0; i < PumpCount; i++) {
                if(pump_states[i] != PUMP_SETUP) {
                     pump_enter_setup(i);
                }
            }
        }

        /*
         * Check if we need a new active pump
         * if full -> nice we've got a fresh one
         * if dispense -> still some stuff left in this one
         */
        if(pump_states[active_pump] != PUMP_FULL && pump_states[active_pump] != PUMP_DISPENSE) {
            for(uint8_t i = 0; i < PumpCount; i++) {
                if(pump_states[i] == PUMP_FULL) {
                    active_pump = i;
                    break;
                }
            }
        }

        // We've selected a new active pump, now we need to start it
        if(pump_states[active_pump] == PUMP_FULL) {
            uart_debug_pump(active_pump, "is new acitve pump");
            pump_enter_dispense(active_pump);
        }

        /*
         * We are ready if we picked a new active pump and set it to dispense,
         * or if the old one still has stuff left in it and is in dispense.
         */
        if(pump_states[active_pump] == PUMP_DISPENSE) {
            *ready_port |= ready_mask;
        }
        else {
            *ready_port &= ~(ready_mask);
        }


        /*
         * If we are idle, try to fill all pumps
         * If the active pump is also refilled, a other full one will be chosen
         * in the next iteration.
         */
        if(*idle_pin & idle_mask) {
            for(uint8_t i = 0; i < PumpCount; i++) {
                if(pump_states[i] == PUMP_DISPENSE) {
                    uart_debug("Idle state detected, refilling");
                    pump_enter_fill(i);
                }
            }
        }
    }

}
