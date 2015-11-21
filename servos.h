#ifndef SERVOS_H
#define SERVOS_H

#include <stdint.h>

    void servos_init(void);

    void servo_set_pos(uint8_t servo, uint8_t pos);

#endif
