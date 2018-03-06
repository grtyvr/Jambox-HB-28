#include <SPI.h>
#include "LedMatrix.h"

//LED Matrix Pins
#define NUMBER_OF_DEVICES 4 
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //Not Used
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

void matrix_init() {

    ledMatrix.init();
    ledMatrix.clear();
    ledMatrix.commit();
}

void matrix_clear() {

  ledMatrix.clear();
}

void matrix_setPixel(int x, int y) {

  ledMatrix.setPixel(x, y);
}

void matrix_commit() {

  ledMatrix.commit();
}

/*
  for (int i=0; i<28; i++)
  {
    ledMatrix.clear();
    ledMatrix.scrollTextLeft();
    ledMatrix.drawText();
    ledMatrix.commit();
    delay(100);
  }
  
    ledMatrix.clear();
  ledMatrix.setPixel(col,i);
  ledMatrix.commit();
*/
