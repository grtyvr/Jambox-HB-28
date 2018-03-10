#include "Samples.h"

void setup() {  

  Serial.begin(115200);
  matrix_init();
  input_init();
  synth_init();
}

void loop() {
  
  matrix_clear();
  matrix_setPixel(10, input_pot1() / 128);
  matrix_setPixel(12, input_pot2() / 128);
  matrix_setPixel(14, input_pot3() / 128);
  matrix_setPixel(16, input_pot4() / 128);
  matrix_setPixel(18, input_pot5() / 128);
  
  matrix_update();
  synth_update();
}
