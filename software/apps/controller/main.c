// error app
//
// Cause an error that then gets printed

#include <stdbool.h>
#include <stdint.h>
// #include <math.h>
#include <stdio.h>

#include "app_timer.h"
#include "nrf.h"
#include "microbit_v2.h"
#include "nrf_delay.h"

#include "buttons.h"
#include "sound_driver.h"
#include "lsm303agr.h"
#define freq_var 30

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
APP_TIMER_DEF(controller_update_timer);

static void do_controller_state(void *_unused)
{
  // printf("start joystick measurement\n");
  printf("%f\n", joystick_val);
  printf("%d %d\n", button_state[0],button_state[1]);
  if(button_state[0] && !button_state[1]){
    uint16_t press_len = debounce_timer[0]/8;
    play_note(390 + ((press_len < 300)?press_len: 300) + (rand() % (freq_var)));
    // printf("press len: %d", press_len);
  } else{
    stop_note();
  }
}

void init_lsm(){
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = I2C_INTERNAL_SCL;
  i2c_config.sda = I2C_INTERNAL_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  if (nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config) != NRF_SUCCESS) {
    printf("I2C init failed!\n");
  }
  lsm303agr_init(&twi_mngr_instance);
  init_joystick();
}

int main(void) {
  printf("Board started!\n");
  //global app timer init
  app_timer_init();

  printf("sound driver initializing...\n");
  sound_init();
  printf("sound driver initialized!\n");

  printf("lsm driver initializing...\n");
  init_lsm();
  printf("lsm driver initialized!\n");

  printf("button driver initializing!\n");
  button_init();
  printf("button driver initialized!\n");

  app_timer_create(&controller_update_timer, APP_TIMER_MODE_REPEATED, do_controller_state);
  app_timer_start(controller_update_timer, APP_TIMER_TICKS(100), NULL);

  // for(uint16_t i = 440; i < 1000; i+= 20){
  //   play_note(i);
  //   nrf_delay_ms(100);
  // }

  while(1){
    nrf_delay_ms(10000);
    printf("looping\n");
  }
}

