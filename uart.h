#ifndef UART_H_
#define UART_H_ UART_H_

#include  <avr/io.h>
#include  <stdlib.h>
#include  <util/delay.h>

#include <string.h>

// Will never ever be set to 0
static const uint8_t debug = 1;

//#define BAUD 76800UL		// baudrate
#define BAUD 38400UL
#define UART_TIMEOUT 100	// Timeout in ms

// Some calculations ...
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // Rounding magic
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Real baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)

#if ((BAUD_ERROR<950) || (BAUD_ERROR>1050))		// Make sure your UBRR_VAL will work
  #error Baudrate error is bigger then 1% !
#endif

extern uint8_t uart_timed_out;

void uart_init(void);
uint8_t uart_getc_timeout(void);
uint8_t uart_get_line(char buffer[], uint8_t maxlen);


static inline void uart_putc(uint8_t data) {
	UDR0 = data;							// write byte to data register
	while (!(UCSR0A & (1<<  UDRE0))); 	// waiting for the uart to finish transmission
	UCSR0A |= (1 <<  UDRE0);
}

static inline void uart_puts(char *data) {
  uint8_t i;
  for(i = 0; i < strlen(data); i++) {
    uart_putc(data[i]);
  }
}

static inline uint8_t uart_getc(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

static inline uint8_t uart_has_timed_out(void) {
	return uart_timed_out;
}

static inline void uart_clear_time_out(void) {
	uart_timed_out = 0;
}

static inline void uart_debug(char *data) {
	if(debug) {

		uart_puts("[debug] ");
		uart_puts(data);
		uart_puts("\n\r");
	}
}

static inline void uart_debug_pump(uint8_t i, char *data) {

	if(debug) {
        char buffer[4]; // 8bit as 3 digits and a terminating zero

        itoa(i,buffer,10);
		uart_puts("[debug ");
        uart_puts(buffer);
        uart_puts("] ");
		uart_puts(data);
		uart_puts("\n\r");
	}
}

#endif
