// #include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrfx_rtc.h"
#include "microbit_v2.h"
#include <stdint.h>


#define NUMBUTTONS 2
#define RTCDEBID 0
#define DEBOUNCE_MS 50
extern uint16_t debounce_timer[NUMBUTTONS];
extern uint16_t button_state[NUMBUTTONS];

void rtc_handler(nrfx_rtc_int_type_t int_type);
void rtc_init();
void button_init();