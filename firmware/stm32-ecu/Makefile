CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -IApp/include -Iconfig

all:
	$(CC) $(CFLAGS) App/src/vehicle_state.c App/src/fault_manager.c -o build/stm32_ecu_demo

clean:
	rm -f build/stm32_ecu_demo
