#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <avr/io.h>


static const uint8_t idle_mask = (1 << PC3);
static volatile uint8_t *const idle_pin = &PINC;
static volatile uint8_t *const idle_port = &PORTC;
static volatile uint8_t *const idle_ddr = &DDRC;

static uint8_t setup_mask = (1 << PC4);
static volatile uint8_t *const setup_pin = &PINC;
static volatile uint8_t *const setup_port = &PORTC;
static volatile uint8_t *const setup_ddr = &DDRC;

static uint8_t ready_mask = (1 << PC5);
static volatile uint8_t *const ready_port = &PORTC;
static volatile uint8_t *const ready_ddr = &DDRC;



#endif
