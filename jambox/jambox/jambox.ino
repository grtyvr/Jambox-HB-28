// NOTE! sequencer.ino is not done or ready for use, please ignore it for now ( or just delete it. )
// NOTE PT.2 the remix: You will have to get the matrix display driver from here http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-display-spi-led-matrix.html

volatile uint16_t synth_SyncPhaseAcc = 0;
volatile uint16_t synth_SyncPhaseInc = 0;
volatile uint16_t synth_GrainPhaseAcc = 0;
volatile uint16_t synth_GrainPhaseInc = 0;
volatile uint16_t synth_GrainAmp = 0;
volatile uint8_t synth_GrainDecay = 0;
volatile uint16_t synth_Grain2PhaseAcc = 0;
volatile uint16_t synth_Grain2PhaseInc = 0;
volatile uint16_t synth_Grain2Amp = 0;
volatile uint8_t synth_Grain2Decay = 0;

void setup() {  

  Serial.begin(115200);

  matrix_init();
  input_init();
  synth_init();
  //sequencer_init();
  Serial.println("All systems initialized.");
}

void loop() {
  
  matrix_clear();    
  //sequencer_update();
  synth_update();
  matrix_update();

  // there is intentionally no delay here, delays are done inside the sequencer to allow for proper bpm timing when playing
}
