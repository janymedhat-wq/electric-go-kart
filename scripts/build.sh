#!/usr/bin/env bash
set -euo pipefail

echo "Building STM32 ECU demo..."
cd firmware/stm32-ecu
make
