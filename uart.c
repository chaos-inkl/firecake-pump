#include "uart.h"

uint8_t uart_timed_out = 0;

void uart_init(void) {
	UBRR0H = UBRR_VAL >> 8;		//Setting baudrate
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);  // UART TX
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);  // Asynchronous 8N1

	// flush receive buffer
    do
    {
        UDR0;
    }
    while (UCSR0A & (1 << RXC0));

	//reset tx and rx completeflags
	UCSR0A = (1 << RXC0) | (1 << TXC0) | (1 <<  UDRE0);
}

uint8_t uart_getc_timeout(void) {
	uint8_t retries = UART_TIMEOUT;
	uint8_t delays = 0;

	while (!(UCSR0A & (1<<RXC0)) && (retries > 0)) {
		if(delays == 0) {
			retries--;
		}
		delays = (delays + 1) % 250;
		_delay_us(4);
	}

	if(retries > 0) {
		uart_timed_out = 0;
		return UDR0;
	}

	uart_timed_out = 1;
	return 0;
}

uint8_t uart_get_line(char buffer[], uint8_t maxlen) {
	char t = 0;
	uint8_t pos = 0;
	buffer[0] = 0;

	//maxlen needs to be at least big enough for one character + null byte.
	if(maxlen < 2) {
		return 0;
	}

	uart_clear_time_out();

	while(pos < maxlen && t != '\n' && !uart_has_timed_out()) {
		t = uart_getc_timeout();
		buffer[pos] = t;
		pos++;
	}

	// We passed the loop at least once, so pos can not be 0
	if(buffer[pos-1] != '\n') {
		return 0;
	}
	buffer[pos-1] = 0;
	return 1;
}
