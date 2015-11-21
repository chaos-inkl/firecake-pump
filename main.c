#include "uart.h"
#include "servos.h"
#include "pumps.h"
#include "config.h"



int main(void) {
    uart_init();
    servos_init();
    /*
    pumps_init();

    *idle_ddr &= ~idle_mask;
    *setup_ddr &= ~setup_mask;
    *ready_ddr |= ready_mask;
    */
    uart_puts("PumpControl 0.1 ready\n\r");

    /*while(1) {
        pumps_run();

        if(*setup_pin & setup_mask) {
            for(uint8_t i = 0; i < PumpCount; i++) {
                pump_enter_setup(i);
            }
        }

        // Signal if there is pump ready for dispensing
        uint8_t ready = 0;
        for(uint8_t i = 0; i < PumpCount; i++) {
            if(pumps[i].state == PUMP_FULL) {
                ready = 1;
                break;
            }
        }
        if(ready) {
            *ready_port |= ready_mask;
        }
        else {
            *ready_port &= ~ready_mask;
        }

        //If we are idle, try to fill all pumps
        if(*idle_pin & idle_mask) {
            for(uint8_t i = 0; i < PumpCount; i++) {
                if(pumps[i].state == PUMP_DISPENSE) {
                    pump_enter_fill(i);
                }
            }
        }
    }*/

}
