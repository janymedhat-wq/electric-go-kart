#ifndef VEHICLE_STATE_H
#define VEHICLE_STATE_H

#include <stdint.h>

typedef enum {
    VEHICLE_STATE_OFF = 0,
    VEHICLE_STATE_INIT,
    VEHICLE_STATE_PRECHARGE,
    VEHICLE_STATE_READY,
    VEHICLE_STATE_DRIVE,
    VEHICLE_STATE_REGEN,
    VEHICLE_STATE_FAULT,
    VEHICLE_STATE_SHUTDOWN
} vehicle_state_t;

vehicle_state_t vehicle_state_get(void);
void vehicle_state_set(vehicle_state_t next_state);
const char *vehicle_state_name(vehicle_state_t state);

#endif
