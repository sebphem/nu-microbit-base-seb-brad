#pragma once

#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"


#define SINE_BUFFER_SIZE 500
#define SAMPLING_FREQUENCY 16000
#define BUFFER_SIZE 16000
#define COUNTERTOP ((16000000) / (SAMPLING_FREQUENCY * 2))

extern const nrfx_pwm_t PWM_INST;
extern uint16_t sine_buffer[SINE_BUFFER_SIZE];
extern uint16_t samples[BUFFER_SIZE];

void sound_init();
void play_note(uint16_t frequency);
void stop_note();