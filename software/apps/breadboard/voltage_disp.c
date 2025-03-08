// LED Matrix Driver
// Displays characters on the LED matrix

#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "voltage_disp.h"
#include "microbit_v2.h"

uint32_t _rows[5] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
uint32_t _cols[5] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5};

float voltages[2] = {0.0,0.0};
int voltages_quant[2] = {0,0};

void led_matrix_init(void) {
  // initialize row pins

  for(int i = 0; i <5; i ++){
    nrf_gpio_pin_dir_set(_rows[i], NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(_cols[i], NRF_GPIO_PIN_DIR_OUTPUT);
  }
  // all of the leds should be on
  for(int i = 0; i < 5; i++){
    nrf_gpio_pin_clear(_rows[i]);
  }


  for(int i = 0; i < 5; i++){
    nrf_gpio_pin_set(_cols[i]);
  }
}



// bool leds[5][5]= {0};
bool leds[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

typedef enum{
  r0,
  r1,
  r2,
  r3,
  r4,
  NUM_STATES
} row_iter_t;

row_iter_t cur_row = r0;

void drawState(void* _unused) {
    //clear all comb signals
    for(int i = 0; i < 5; i++){
      nrf_gpio_pin_clear(_rows[i]);
    }

    for(int i = 0; i < 5; i++){
      nrf_gpio_pin_set(_cols[i]);
    }
    for(int i = 0; i < 5; i++){
      nrf_gpio_pin_set(_rows[cur_row]);
      nrf_gpio_pin_write(_cols[i],!leds[cur_row][i]);
    }
    cur_row = (cur_row + 1) % NUM_STATES;
}


void updateVoltage(float voltage_1, float voltage_2){
  //  printf("updating voltages\n");
   voltages[0] = voltage_1;
   voltages[1] = voltage_2;
   voltages_quant[0] = (int)(voltage_1 * 5 / 3.6);
   voltages_quant[1] = (int)(voltage_2 * 5 / 3.6);
  //  printf("updating voltages done\n");
   updateLEDState((void*)(NULL));
}

void updateLEDState(void *_unused){
  // printf("update led state input char: %c\n", c);
  for(int i = 0; i<5; i++){
    leds[4-i][0] = voltages_quant[0] > i ? 1: 0;
  }
  for(int i = 0; i<5; i++){
    leds[4-i][4] = voltages_quant[1] > i ? 1: 0;
  }

  // printf("leds rn:\n");
  // for(int i = 0; i<5; i++){
  //   for(int j =0; j<5; j++){
  //     printf(leds[i][j] ? "1" : "0");
  //   }
  //   printf("\n");
  // }

  // printf("voltages: \n");
  // for (int i = 0; i < 2; i++) {
  //   printf("%d: %f\n", i, voltages[i]); // Print with 2 decimal places
  // }
  // printf("voltages quant: \n");
  // for (int i = 0; i < 2; i++) {
  //   printf("%d: %d\n", i, voltages_quant[i]); // Print with 2 decimal places
  // }   
  
  printf("drink status: %d %d\n", (voltages_quant[0] >= 3),(voltages_quant[1] >= 3));


}