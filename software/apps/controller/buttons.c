#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrfx_gpiote.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "app_timer.h"
#include "microbit_v2.h"


#define NUMBUTTONS 2
#define DEBOUNCE_MS 50 

#define btna_pin 14
#define btnb_pin 23

//state holders
uint16_t debounce_timer[NUMBUTTONS] = {0,0};
uint16_t button_state[NUMBUTTONS] = {0,0};

APP_TIMER_DEF(deb_timer);
#define TPS 32768

void _debounce_incrementer(void * __unused){
    // printf("debounce state: %d, %d", debounce_timer[0], debounce_timer[1]);
    for(int i = 0;i < NUMBUTTONS; i++){
        debounce_timer[i] += 20;
    }
}


void _debounce_timer_init(){
    app_timer_create(&deb_timer, APP_TIMER_MODE_REPEATED, _debounce_incrementer);
    //get close to 20ms slices
    app_timer_start(deb_timer, TPS/50, NULL);
}

void button_a_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    // if (debounce_timer[0] >= DEBOUNCE_MS) {
    //     debounce_timer[0] = 0;
    //     // printf("action")
    //     // if(!button_state[0]) { // going from button not pressed to pressed
    //     //     button_state[0] = 1;
    //     //     // printf("Button A Pressed (Debounced)\n");
    //     // } else{
    //     //     // going from button pressed to not pressed
    //     //     button_state[0] = 0;
    //     //     // printf("Button A Released (Debounced)\n");
    //     // }
    //     if(nrf_gpio_pin_read(14)) button_state[0] = 1;
    //     else button_state[0] = 0;
    // }
    debounce_timer[0] = 0;
    button_state[0] = !nrf_gpio_pin_read(btna_pin);
}

void button_b_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    // if (debounce_timer[1] >= DEBOUNCE_MS) {
    //     debounce_timer[1] = 0;
    //     if(!button_state[1]) { // going from button not pressed to pressed
    //       button_state[1] = 1;
    //     //   printf("Button B Pressed (Debounced)\n");
    //   } else{
    //       // going from button pressed to not pressed
    //       button_state[1] = 0;
    //     //   printf("Button B Released (Debounced)\n");
    //   }
    // }
    debounce_timer[1] = 0;
    button_state[1] = !nrf_gpio_pin_read(btnb_pin);
}

void button_init() {
    nrfx_gpiote_init();
    // button a init
    nrfx_gpiote_in_config_t button_a_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_TOGGLE(true);
    nrfx_gpiote_in_init(btna_pin, &button_a_config, button_a_handler);
    nrfx_gpiote_in_event_enable(btna_pin, true);
    // button b init
    nrfx_gpiote_in_config_t button_b_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_TOGGLE(true);
    nrfx_gpiote_in_init(btnb_pin, &button_b_config, button_b_handler);
    nrfx_gpiote_in_event_enable(btnb_pin, true);
    _debounce_timer_init();
}