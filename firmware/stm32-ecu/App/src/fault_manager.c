#include "fault_manager.h"

static bool g_faulted = false;
static fault_code_t g_active_fault = FAULT_NONE;

bool fault_manager_is_faulted(void) {
    return g_faulted;
}

void fault_manager_clear(void) {
    g_faulted = false;
    g_active_fault = FAULT_NONE;
}

void fault_manager_set(fault_code_t fault) {
    g_active_fault = fault;
    g_faulted = true;
}

const char *fault_manager_name(fault_code_t fault) {
    switch (fault) {
        case FAULT_NONE: return "NONE";
        case FAULT_ESTOP: return "ESTOP";
        case FAULT_BMS_TIMEOUT: return "BMS_TIMEOUT";
        case FAULT_MC_TIMEOUT: return "MC_TIMEOUT";
        case FAULT_THROTTLE_PLAUSIBILITY: return "THROTTLE_PLAUSIBILITY";
        case FAULT_PRECHARGE: return "PRECHARGE";
        case FAULT_OVER_VOLTAGE: return "OVER_VOLTAGE";
        case FAULT_UNDER_VOLTAGE: return "UNDER_VOLTAGE";
        case FAULT_OVER_TEMPERATURE: return "OVER_TEMPERATURE";
        default: return "UNKNOWN";
    }
}
