#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    FAULT_NONE = 0,
    FAULT_ESTOP,
    FAULT_BMS_TIMEOUT,
    FAULT_MC_TIMEOUT,
    FAULT_THROTTLE_PLAUSIBILITY,
    FAULT_PRECHARGE,
    FAULT_OVER_VOLTAGE,
    FAULT_UNDER_VOLTAGE,
    FAULT_OVER_TEMPERATURE
} fault_code_t;

bool fault_manager_is_faulted(void);
void fault_manager_clear(void);
void fault_manager_set(fault_code_t fault);
const char *fault_manager_name(fault_code_t fault);

#endif
