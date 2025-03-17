#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds a pre-computed sine wave
#define SINE_BUFFER_SIZE 500
uint16_t sine_buffer[SINE_BUFFER_SIZE] = {0};

// Sample data configurations
// Note: this is a 32 kB buffer (about 25% of RAM)
#define SAMPLING_FREQUENCY 16000     // 16 kHz sampling rate
#define BUFFER_SIZE 16000            // one second worth of data
#define COUNTERTOP ((16000000) / (SAMPLING_FREQUENCY * 2))
uint16_t samples[BUFFER_SIZE] = {0}; // stores PWM duty cycle values

/*** Initialization & handling code ***/

static void _gpio_init(void) {
  // Initialize pins
  // Microphone pin MUST be high drive
  nrf_gpio_pin_dir_set(LED_MIC, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_cfg(LED_MIC, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
               NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0H1, NRF_GPIO_PIN_NOSENSE);

  // Enable microphone
  nrf_gpio_pin_set(LED_MIC);
}

static void _pwm_init(void) {
  // Initialize the PWM
  // SPEAKER_OUT is the output pin, mark the others as NRFX_PWM_PIN_NOT_USED
  // Set the clock to 16 MHz
  // Set a countertop value based on sampling frequency and repetitions
  // TODO
  nrfx_pwm_config_t config = {
      .output_pins = {SPEAKER_OUT, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED,
                      NRFX_PWM_PIN_NOT_USED},
      .irq_priority = 0,
      .base_clock = NRF_PWM_CLK_16MHz,
      .count_mode = NRF_PWM_MODE_UP,
      .load_mode = NRF_PWM_LOAD_COMMON,
      .step_mode = NRF_PWM_STEP_AUTO,
      .top_value = COUNTERTOP};
  nrfx_pwm_init(&PWM_INST, &config, NULL);
}

static void _compute_sine_wave(uint16_t max_value) {
  for (int i = 0; i < SINE_BUFFER_SIZE; i++) {
    // what percent into the sine wave are we?
    float percent = (float)i / (float)SINE_BUFFER_SIZE;

    // get sine value
    float twopi = 2.0 * 3.14159;
    float radians = twopi * percent;
    float sine_value = sin(radians);

    // translate from "-1 to 1" into "0 to 2"
    float abs_sine = sine_value + 1.0;

    // scale from "0 to 2" into "0 to 1"
    float mag_1_sine = abs_sine / 2.0;

    // scale to the max_value desired ("0 to max_value")
    // and truncate to an integer value
    uint16_t value = (uint16_t)(max_value * mag_1_sine);

    // save value in buffer
    sine_buffer[i] = value;
  }
}

void sound_init(){
    // initialize GPIO
    _gpio_init();

    // initialize the PWM
    _pwm_init();

    // compute the sine wave values
    // You should pass in COUNTERTOP-1 here as the maximum value
    _compute_sine_wave(COUNTERTOP -1); // TODO: put a value in here
}



void play_note(uint16_t frequency) {
  float step_size =
      (float)SINE_BUFFER_SIZE * (float)frequency / (float)SAMPLING_FREQUENCY;

  float counter = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {

    samples[i] = sine_buffer[(int)counter];

    counter += step_size;

    if (counter > SINE_BUFFER_SIZE) {
      counter -= SINE_BUFFER_SIZE;
    }
  };

  nrf_pwm_sequence_t pwm_sequence = {
      .values.p_common = (uint16_t *)samples,
      .length = BUFFER_SIZE,
      .repeats = 1,
      .end_delay = 0,
  };

  nrfx_pwm_flag_t flags = NRFX_PWM_FLAG_LOOP;
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, flags);
}

void stop_note(){
    nrfx_pwm_stop(&PWM_INST, true);
}