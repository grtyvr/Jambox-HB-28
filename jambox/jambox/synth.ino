// Synth code from Auduino, the Lo-Fi granular synthesiser
// by Peter Knight, Tinker.it http://tinker.it
// http://code.google.com/p/tinkerit/wiki/Auduino

#include "driver/i2s.h"
#include "freertos/queue.h"

#define i2s_num     0
#define sampleRate  44100

hw_timer_t * timer = NULL;

uint16_t syncPhaseAcc;
uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay;

// Smooth logarithmic mapping
uint16_t antilogTable[] = {
  64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
  54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
  45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
  38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768
};

uint16_t mapPhaseInc(uint16_t input) {
  return (antilogTable[input & 0x3f]) >> (input >> 6);
}

// Stepped chromatic mapping
uint16_t midiTable[] = {
  17,18,19,20,22,23,24,26,27,29,31,32,34,36,38,41,43,46,48,51,54,58,61,65,69,73,
  77,82,86,92,97,103,109,115,122,129,137,145,154,163,173,183,194,206,218,231,
  244,259,274,291,308,326,346,366,388,411,435,461,489,518,549,581,616,652,691,
  732,776,822,871,923,978,1036,1097,1163,1232,1305,1383,1465,1552,1644,1742,
  1845,1955,2071,2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
  4389,4650,4927,5220,5530,5859,6207,6577,6968,7382,7821,8286,8779,9301,9854,
  10440,11060,11718,12415,13153,13935,14764,15642,16572,17557,18601,19708,20879,
  22121,23436,24830,26306
};

uint16_t mapMidi(uint16_t input) {
  return (midiTable[(1023-input) >> 3]);
}

// Stepped Pentatonic mapping
//
uint16_t pentatonicTable[54] = {
  0,19,22,26,29,32,38,43,51,58,65,77,86,103,115,129,154,173,206,231,259,308,346,
  411,461,518,616,691,822,923,1036,1232,1383,1644,1845,2071,2463,2765,3288,
  3691,4143,4927,5530,6577,7382,8286,9854,11060,13153,14764,16572,19708,22121,26306
};

uint16_t mapPentatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (pentatonicTable[value]);
}

void IRAM_ATTR SIGNAL() {
    
    uint8_t value;
    uint16_t output = 5;

    i2s_write_bytes((i2s_port_t)i2s_num, (const char *)&output, sizeof(uint16_t), portMAX_DELAY);
    return;
    
    syncPhaseAcc += syncPhaseInc;

    if (syncPhaseAcc < syncPhaseInc) {

        // Time to start the next grain
        grainPhaseAcc = 0;
        grainAmp = 0x7fff;
        grain2PhaseAcc = 0;
        grain2Amp = 0x7fff;
    }

    // Increment the phase of the grain oscillators
    grainPhaseAcc += grainPhaseInc;
    grain2PhaseAcc += grain2PhaseInc;

    // Convert phase into a triangle wave
    value = (grainPhaseAcc >> 7) & 0xff;

    if (grainPhaseAcc & 0x8000) 
        value = ~value;

    // Multiply by current grain amplitude to get sample
    output = value * (grainAmp >> 8);

    // Repeat for second grain
    value = (grain2PhaseAcc >> 7) & 0xff;

    if (grain2PhaseAcc & 0x8000) value = ~value;
        output += value * (grain2Amp >> 8);

    // Make the grain amplitudes decay by a factor every sample (exponential decay)
    grainAmp -= (grainAmp >> 8) * grainDecay;
    grain2Amp -= (grain2Amp >> 8) * grain2Decay;

    // Scale output to the available range, clipping if necessary
    output >>= 9;

    if (output > 255) 
        output = 255;

    i2s_write_bytes((i2s_port_t)i2s_num, (const char *)&output, sizeof(uint32_t), portMAX_DELAY);
}

void synth_init() {

    //i2s configuration 
    i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = sampleRate,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = 26, //this is BCK pin
        .ws_io_num = 25, // this is LRCK pin
        .data_out_num = 22, // this is DATA output pin
        .data_in_num = -1   //Not used
    };

    //initialize i2s with configurations above
    i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
    i2s_set_pin((i2s_port_t)i2s_num, &pin_config);

    //set sample rates of i2s to sample rate of wav file
    i2s_set_sample_rates((i2s_port_t)i2s_num, (int)(sampleRate / 2)); 
    
    /* Use 1st timer of 4 */
    /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
    timer = timerBegin(0, 80, true);

    /* Attach onTimer function to our timer */
    timerAttachInterrupt(timer, &SIGNAL, true);

    /* Set alarm to call onTimer function every second 1 tick is 1us
    /* Repeat the alarm (third parameter) */
    timerAlarmWrite(timer, 32, true);

    /* Start an alarm */
    timerAlarmEnable(timer);
}

void synth_update() {

  // Smooth frequency mapping
  //syncPhaseInc = mapPhaseInc(analogRead(SYNC_CONTROL)) / 4;
  
  // Stepped mapping to MIDI notes: C, Db, D, Eb, E, F...
  //syncPhaseInc = mapMidi(analogRead(SYNC_CONTROL));
  
  // Stepped pentatonic mapping: D, E, G, A, B
  syncPhaseInc = mapPentatonic(input_pot1());
  grainPhaseInc  = mapPhaseInc(input_pot2()) / 2;
  grainDecay     = input_pot3() / 8;
  grain2PhaseInc = mapPhaseInc(input_pot4()) / 2;
  grain2Decay    = input_pot5() / 4;
}
