// NOTE! sequencer.ino is not done or ready for use, please ignore it for now ( or just delete it. )
// NOTE PT.2 the remix: You will have to get the matrix display driver from here http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-display-spi-led-matrix.html

void setup() {  

  matrix_init();
  input_init();
  synth_init();
  sequencer_init();
}

void loop() {

  matrix_clear();
  sequencer_update();
  synth_update();
  matrix_update();

  // there is intentionally no delay here, delays are done inside the sequencer to allow for proper bpm timing when playing
}