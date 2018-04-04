// NOTE! sequencer.ino is not done or ready for use, please ignore it for now ( or just delete it. )
// NOTE PT.2 the remix: You will have to get the matrix display driver from here http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-display-spi-led-matrix.html

void setup() {  

  matrix_init();
  input_init();
  synth_init();
}

void loop() {
  
  // jank knob output for now
  matrix_clear();
  matrix_setPixel(10, input_pot1() / 128);
  matrix_setPixel(12, input_pot2() / 128);
  matrix_setPixel(14, input_pot3() / 128);
  matrix_setPixel(16, input_pot4() / 128);
  matrix_setPixel(18, input_pot5() / 128);
  
  // only call matrix_update once per loop
  matrix_update();
  synth_update();
}
