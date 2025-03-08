// I2C sensors app
//
// Read from I2C accelerometer/magnetometer on the Microbit

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "lsm303agr.h"

#include "app_timer.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
APP_TIMER_DEF(timer);

void do_temp_thing(void unused)
{
  float temp = lsm303agr_read_temperature();
  printf("Temperature: %f\n", temp);
}

void do_accel_thing(voidunused)
{
  lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();
  printf("Accel: x=%f y=%f z=%f\n", accel.x_axis, accel.y_axis, accel.z_axis);
}

void do_mag_thing(void unused)
{
  lsm303agr_measurement_t mag = lsm303agr_read_magnetometer();
  printf("Mag: x=%f y=%f z=%f\n", mag.x_axis, mag.y_axis, mag.z_axis);
}

void do_tilt_thing(voidunused)
{
  float phi = get_tilt_from_acc(lsm303agr_read_accelerometer());
  printf("phi: %f\n", phi);
}

int main(void)
{
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  // WARNING!!
  // These are NOT the correct pins for external I2C communication.
  // If you are using QWIIC or other external I2C devices, the are
  // connected to EDGE_P19 (a.k.a. I2C_QWIIC_SCL) and EDGE_P20 (a.k.a. I2C_QWIIC_SDA)
  i2c_config.scl = I2C_INTERNAL_SCL;
  i2c_config.sda = I2C_INTERNAL_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  lsm303agr_init(&twi_mngr_instance);

  // Initialize app timer
  app_timer_init();
  app_timer_create(&timer, APP_TIMER_MODE_REPEATED, do_tilt_thing);
  app_timer_start(timer, 32768 / 2, NULL);

  // Loop forever
  while (1)
  {
    // Don't put a
    nrf_delay_ms(1000);
  }
};