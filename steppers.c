#include "steppers.h"

/*
 * F_CPU is 16Mhz
 * Prescaler is 256
 * => 62.5khz or 62500 Timer ticks per second
 * Preload is 224
 * => One overflow every 32 timer ticks
 * => 1953.125 overflows per Second
 * => Toggle in each overflow -> ~976Hz clock
 */
const uint8_t preload = 240;

uint16_t ticks;
uint32_t timestamp;

enum StepperModes stepper_modes[2];
enum StepperDirs stepper_dirs[2];

void steppers_init(void) {
	TCNT0 = preload;	// Preload for 32 ticks to overflow
	TIMSK0 |= (1 << TOIE0);
	TCCR0B = (1 << CS02);	// Prescaler 256

	for(uint8_t i = 0; i < StepperCount; i++) {
		*steppers[i].step_ddr |= steppers[i].step_mask;
		*steppers[i].dir_ddr |= steppers[i].dir_mask;

		*steppers[i].limit_min_ddr &= ~(steppers[i].limit_min_mask);
		*steppers[i].limit_min_port |= (steppers[i].limit_min_mask);

		*steppers[i].limit_max_ddr &= ~(steppers[i].limit_max_mask);
		*steppers[i].limit_max_port |= (steppers[i].limit_max_mask);

		stepper_modes[i] = STEPPER_OFF;
		stepper_dirs[i] = STEPPER_BACKWARD;
	}

	ticks = 0;
	timestamp = 0;
}

void stepper_set_dir(uint8_t stepper, enum StepperDirs dir) {
	if(stepper < StepperCount) {
		stepper_dirs[stepper] = dir;

		if(dir == STEPPER_BACKWARD) {
			*steppers[stepper].dir_port |= steppers[stepper].dir_mask;
		}
		else {
			*steppers[stepper].dir_port &= ~steppers[stepper].dir_mask;
		}
	}
}

void stepper_set_mode(uint8_t stepper, enum StepperModes mode) {
	if(stepper < StepperCount) {
		stepper_modes[stepper] = mode;
	}
}

uint32_t now() {
	return timestamp;
}

ISR(TIMER0_OVF_vect) {
	TCNT0 = preload;

	for(uint8_t i = 0; i < StepperCount; i++) {
		if(stepper_modes[i] == STEPPER_INTERN &&
			((stepper_dirs[i] == STEPPER_BACKWARD && !stepper_at_min(i)) ||
			(stepper_dirs[i] == STEPPER_FORWARD && !stepper_at_max(i)))) {
				*steppers[i].step_port ^= steppers[i].step_mask;
		}
	}

	ticks++;
	if(ticks > 1954) {
		timestamp++;
		ticks = 0;
	}
};
