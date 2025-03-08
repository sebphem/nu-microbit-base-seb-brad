#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "microbit_v2.h"

void led_matrix_init(void);

// Function declarations
void drawState(void* _unused);
void updateVoltage(float voltage_1, float voltage_2);
void updateLEDState(void* _unused);