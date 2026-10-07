#include "vehicle_state.h"

static vehicle_state_t g_vehicle_state = VEHICLE_STATE_OFF;

vehicle_state_t vehicle_state_get(void) {
    return g_vehicle_state;
}

void vehicle_state_set(vehicle_state_t next_state) {
    g_vehicle_state = next_state;
}

const char *vehicle_state_name(vehicle_state_t state) {
    switch (state) {
        case VEHICLE_STATE_OFF: return "OFF";
        case VEHICLE_STATE_INIT: return "INIT";
        case VEHICLE_STATE_PRECHARGE: return "PRECHARGE";
        case VEHICLE_STATE_READY: return "READY";
        case VEHICLE_STATE_DRIVE: return "DRIVE";
        case VEHICLE_STATE_REGEN: return "REGEN";
        case VEHICLE_STATE_FAULT: return "FAULT";
        case VEHICLE_STATE_SHUTDOWN: return "SHUTDOWN";
        default: return "UNKNOWN";
    }
}
