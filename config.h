#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <avr/io.h>


static uint8_t idle_mask = (1 << PD1);
static volatile uint8_t *idle_pin = &PIND;
static volatile uint8_t *idle_ddr = &DDRD;

static uint8_t setup_mask = (1 << PD2);
static volatile uint8_t *setup_pin = &PIND;
static volatile uint8_t *setup_ddr = &DDRD;

static uint8_t ready_mask = (1 << PD3);
static volatile uint8_t *ready_port = &PORTD;
static volatile uint8_t *ready_ddr = &DDRD;



#endif
