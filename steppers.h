#ifndef STEPPERS_H
#define STEPPERS_H


#include <avr/io.h>
#include <avr/interrupt.h>

enum StepperModes {STEPPER_OFF, STEPPER_INTERN, STEPPER_EXTERN};
enum StepperDirs {STEPPER_FORWARD, STEPPER_BACKWARD};

struct StepperConfig {
	const uint8_t step_mask;
	volatile uint8_t *const step_port;
	volatile uint8_t *const step_ddr;

	const uint8_t dir_mask;
	volatile uint8_t *const dir_port;
	volatile uint8_t *const dir_ddr;

	uint8_t limit_min_mask;
	volatile uint8_t *limit_min_pin;
	volatile uint8_t *limit_min_port;
	volatile uint8_t *limit_min_ddr;

	uint8_t limit_max_mask;
	volatile uint8_t *limit_max_pin;
	volatile uint8_t *limit_max_port;
	volatile uint8_t *limit_max_ddr;
};

static const struct StepperConfig steppers[] = {
	{
		(1 << PD4), &PORTD, &DDRD,			// Step
		(1 << PD5), &PORTD, &DDRD,			// Dir
		(1 << PD6), &PIND, &PORTD, &DDRD,	// Limit min
		(1 << PD7), &PIND, &PORTD, &DDRD,	// Limit max
	},
	{
		(1 << PB2), &PORTB, &DDRB,			// Step
		(1 << PB3), &PORTB, &DDRB,			// Dir
		(1 << PB4), &PINB, &PORTB, &DDRB,	// Limit min
		(1 << PB5), &PINB, &PORTB, &DDRB,	// Limit max
	}
};

static const uint8_t StepperCount = 2;

static const uint8_t external_step_mask = (1 << PD2);
static volatile uint8_t *const external_step_pin = &PIND;
static volatile uint8_t *const external_step_ddr = &DDRD;

static const uint8_t external_dir_mask = (1 << PD3);
static volatile uint8_t *const external_dir_pin = &PIND;
static volatile uint8_t *const external_dir_ddr = &DDRD;

static const uint8_t steppers_disable_mask = (1 << PB0);
static volatile uint8_t *const steppers_disable_port = &PORTB;
static volatile uint8_t *const steppers_disable_ddr = &DDRB;


void steppers_init(void);

void stepper_set_dir(uint8_t stepper, enum StepperDirs dir);
void stepper_set_mode(uint8_t stepper, enum StepperModes mode);

uint32_t now(void);

static inline uint32_t since(uint32_t then) {
	return now() - then;
}

static inline uint8_t stepper_at_min(uint8_t stepper) {
	if(stepper < StepperCount) {
		if(*steppers[stepper].limit_min_pin & steppers[stepper].limit_min_mask) {
			return 1;
		}
	}
	return 0;
}

static inline uint8_t stepper_at_max(uint8_t stepper) {
	if(stepper < StepperCount) {
		if(*steppers[stepper].limit_max_pin & steppers[stepper].limit_max_mask) {
			return 1;
		}
	}
	return 0;
}


#endif
