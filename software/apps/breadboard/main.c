// Breadboard example app
//
// Read from multiple analog sensors and control an RGB LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "voltage_disp.h"

#include "app_timer.h"
#include "nordic_common.h"
#include "nrf_delay.h"
#include "nrfx_saadc.h"

#include "microbit_v2.h"

// Digital outputs
// Breakout pins 13, 14, and 15
// These are GPIO pin numbers that can be used in nrf_gpio_* calls
#define LED_RED   EDGE_P13
#define LED_GREEN EDGE_P14
#define LED_BLUE  EDGE_P15


#define TPS 32768

// Digital inputs
// Breakout pin 16
// These are GPIO pin numbers that can be used in nrf_gpio_* calls
#define SWITCH_IN EDGE_P16

// Analog inputs
// Breakout pins 1 and 2
// These are GPIO pin numbers that can be used in ADC configurations
// AIN1 is breakout pin 1. AIN2 is breakout pin 2.
#define ANALOG_FSR_1_IN  NRF_SAADC_INPUT_AIN0
#define ANALOG_FSR_2_IN  NRF_SAADC_INPUT_AIN1

// ADC channel configurations
// These are ADC channel numbers that can be used in ADC calls
#define ADC_FSR_1_CHANNEL 1
#define ADC_FSR_2_CHANNEL 2

// Global variables
APP_TIMER_DEF(fsr_timer);
APP_TIMER_DEF(drawStateTimer);
APP_TIMER_DEF(updateLEDStateTimer);

// Function prototypes
static void gpio_init(void);
static void adc_init(void);
static float adc_sample_blocking(uint8_t channel);

static void readVoltagesADC(void* _unused) {
  // printf("wtf");

  float voltage_1  = adc_sample_blocking(ADC_FSR_1_CHANNEL);
  // printf("voltage_1: %f\n", voltage_1);
  float voltage_2  = adc_sample_blocking(ADC_FSR_2_CHANNEL);
  // printf("voltage_2: %f\n", voltage_2);
  // updateVoltage(adc_sample_blocking(ADC_FSR_1_CHANNEL), adc_sample_blocking(ADC_FSR_2_CHANNEL));
  updateVoltage(voltage_1, voltage_2);
}

static void saadc_event_callback(nrfx_saadc_evt_t const* _unused) {
  // don't care about saadc events
  // ignore this function
}

static void gpio_init(void) {
  // Initialize output pins
  // TODO



  // Set LEDs off initially
  // TODO

  // Initialize input pin
  // TODO


}

static void adc_init(void) {
  // Initialize the SAADC
  nrfx_saadc_config_t saadc_config = {
    .resolution = NRF_SAADC_RESOLUTION_12BIT,
    .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
    .interrupt_priority = 4,
    .low_power_mode = false,
  };
  ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_event_callback);
  APP_ERROR_CHECK(error_code);

  // Initialize light sensor channel
  nrf_saadc_channel_config_t fsr_channel_1_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_FSR_1_IN);
  error_code = nrfx_saadc_channel_init(ADC_FSR_1_CHANNEL, &fsr_channel_1_config);
  APP_ERROR_CHECK(error_code);

  // Initialize light sensor channel
  nrf_saadc_channel_config_t fsr_channel_2_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_FSR_2_IN);
  error_code = nrfx_saadc_channel_init(ADC_FSR_2_CHANNEL, &fsr_channel_2_config);
  APP_ERROR_CHECK(error_code);
}

static float adc_sample_blocking(uint8_t channel) {
  // read ADC counts (0-4095)
  // this function blocks until the sample is ready
  int16_t adc_counts = 0;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &adc_counts);
  APP_ERROR_CHECK(error_code);

  // convert ADC counts to volts
  // 12-bit ADC with range from 0 to 3.6 Volts
  // TODO

  float adc_voltage_float = MAX(((float)adc_counts) *(3.6/4095.0), 0.0);

  // return voltage measurement
  return adc_voltage_float;
}


int main(void) {
  printf("Board started!\n");
  
  // initialize GPIO
  gpio_init();

  led_matrix_init();

  // initialize ADC
  adc_init();

  // initialize app timers
  app_timer_init();
  app_timer_create(&fsr_timer, APP_TIMER_MODE_REPEATED, readVoltagesADC);
  app_timer_start(fsr_timer, TPS/10, NULL);
  app_timer_create(&drawStateTimer, APP_TIMER_MODE_REPEATED, drawState);
  app_timer_start(drawStateTimer, TPS/500, NULL);


  // loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in `sample_timer_callback()`
    nrf_delay_ms(1000);
  }
}
