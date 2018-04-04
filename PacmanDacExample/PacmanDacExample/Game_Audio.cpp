/*
	Game_Audio Library
	
	This library is based heavily on the XT_DAC_Audio libray
	http://www.xtronical.com/basics/audio/digitised-speech-sound-esp32-playing-wavs/
	
	This library is designed for 8-bit quality audio that an old school video 
	game might use. This leaves plenty of cpu resources for the video game logic.

	The interrupt is sync'd with the sample rate. This removes the need to do any 
	complex math in the interrupt. Using the FPU in the interrupt appears to have been
	causing some instability and crashes when I loaded up the CPU with game logic.
	
	(c) B. Dring 2018, Licensed under GNU GPL 3.0 and later, under this license absolutely no warranty given
	
*/

#include "esp32-hal-timer.h";
#include "Game_Audio.h";
#include "driver/i2s.h"

// because of the difficulty in passing parameters to interrupt handlers we have a global
// object of this type that points to the object the user creates.
Game_Audio_Class *GameAudioGlobalObject;       

hw_timer_t * timer = NULL;

// this interrupt is called at the SampleRate of the audio file.
void IRAM_ATTR onTimer() 
{    
  
  if(GameAudioGlobalObject->CurrentWav!=0)
  {
    if(GameAudioGlobalObject->CurrentWav->Completed==false)
    {                    
      
        // old dacwrite
        // dacWrite(GameAudioGlobalObject->DacPin,GameAudioGlobalObject->CurrentWav->Data[GameAudioGlobalObject->CurrentWav->DataIdx]);             												// write out the data
        
        // new 'dacwrite'
        i2s_push_sample(
          (i2s_port_t)I2S_PORT_NUMBER, 
          (const char*)&GameAudioGlobalObject->CurrentWav->Data[GameAudioGlobalObject->CurrentWav->DataIdx], 
          0);
        
        GameAudioGlobalObject->CurrentWav->DataIdx++;
        if(GameAudioGlobalObject->CurrentWav->DataIdx>=GameAudioGlobalObject->CurrentWav->DataSize)  // if that was the last data byte...
          GameAudioGlobalObject->CurrentWav->Completed=true; // mark as completed
       
    }
  }  
}
                                                        
Game_Audio_Class::Game_Audio_Class(uint8_t DacPin, uint8_t TimerNo)
{  
  // setup i2s
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),  // transmitting
      .sample_rate = DEFAULT_SAMPLE_RATE,                   // sample rate
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,         // 16 bits per sample
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,         // stereo
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,             // higher priority
      .dma_buf_count = 8,                                   // honestly, these are just whatever I found initially.
      .dma_buf_len = 64,                                    // i'm not sure we're even using the DMA here
      .use_apll = true                                      // this seems to be for a more stable clock output
    };
    
  i2s_pin_config_t pin_config = {
      .bck_io_num = 26,                                     // this is BCK pin, data clock
      .ws_io_num = 25,                                      // this is LRCK pin, 'word strobe' / lrck / 'word clock'. This is for switching between left / right channels
      .data_out_num = 22,                                   // this is DATA output pin, actual audio serial audio data out
      .data_in_num = -1                                     // Not used
  };

  // instantiate one of the I2S drivers
  i2s_driver_install((i2s_port_t)I2S_PORT_NUMBER, &i2s_config, 0, NULL);

  // give it some pins to write to
  i2s_set_pin((i2s_port_t)I2S_PORT_NUMBER, &pin_config);
  
  timer = timerBegin(TimerNo, AUDIO_INTERRUPT_PRESCALER, true);
  timerAttachInterrupt(timer, &onTimer, true);          // P3= edge trggered
  timerAlarmWrite(timer, ESP32_F_CPU/AUDIO_INTERRUPT_PRESCALER/MIN_SAMPLERATE, true); // use min rate until a wav file is played 
  timerAlarmEnable(timer);                              // enable
  GameAudioGlobalObject=this;						// set global variable to use in ISR
  this->DacPin=DacPin;
  
  // old onboard dac write call
  // dacWrite(DacPin,0x7f);								// Set speaker to mid point to stop any clicks during sample playback

  // new, external 'dacwrite' call
  i2s_push_sample(
    (i2s_port_t)I2S_PORT_NUMBER, 
    (const char*)0x7f, 
    0);
}



int Game_Audio_Class::PlayWav(Game_Audio_Wav_Class *Wav, bool interruptCurrent, float sampleRateMultiplier){
  int newSampleRate;

  if (interruptCurrent == false && GameAudioGlobalObject->CurrentWav != NULL) // if it is NULL a wav has not been played yet and we cannot check values
  {
	  if (GameAudioGlobalObject->CurrentWav->Completed == false)
		  return 0;
  }
  
  
  	
  if (sampleRateMultiplier != 1.0) {
	uint16_t newSampleRate;
	newSampleRate = (uint16_t)(sampleRateMultiplier * Wav->SampleRate);
	if (newSampleRate > MAX_SAMPLERATE || newSampleRate < MIN_SAMPLERATE)
	{
		timerAlarmWrite(timer, ESP32_F_CPU/AUDIO_INTERRUPT_PRESCALER/Wav->SampleRate, true);  // set the interrupt rate to the sample rate
	}
	else
	{
		timerAlarmWrite(timer, ESP32_F_CPU/AUDIO_INTERRUPT_PRESCALER/newSampleRate, true);  // set the interrupt rate to the sample rate
	}
		
  }
  else {
	timerAlarmWrite(timer, ESP32_F_CPU/AUDIO_INTERRUPT_PRESCALER/Wav->SampleRate, true);  // set the interrupt rate to the sample rate
  }
  
  //GameAudioGlobalObject->CurrentWav->Completed = true; // if we are interrupting a different one, it needs to be marked as complete

  // update I2S output sample rate
  i2s_set_sample_rates((i2s_port_t)I2S_PORT_NUMBER, Wav->SampleRate);
  
  //Wav->Count=0;
  Wav->DataIdx=AUDIO_DATA_START;
  Wav->Completed=false; 
  CurrentWav=Wav;
  
  return 1;
  
}

int Game_Audio_Class::IsPlaying()
{
	if (GameAudioGlobalObject->CurrentWav == NULL)
		return false;
	
	return (GameAudioGlobalObject->CurrentWav->Completed == false);
}

void Game_Audio_Class::StopPlaying()
{
	if (GameAudioGlobalObject->CurrentWav != NULL) // if it is NULL we don't need to do anythin
		GameAudioGlobalObject->CurrentWav->Completed=true;
}



Game_Audio_Wav_Class::Game_Audio_Wav_Class(unsigned char *WavData)
{
  // create a new wav class object
  
  SampleRate=(WavData[WAV_SAMPLERATE_H]*256)+WavData[WAV_SAMPLERATE_L]; 
  
  if (SampleRate > MAX_SAMPLERATE || SampleRate < MIN_SAMPLERATE)
	  SampleRate = MIN_SAMPLERATE; // This will prevent problems and the error will be obvious in the playback
  
  DataSize=(WavData[WAV_FILESIZE_H]*65536)+(WavData[WAV_FILESIZE_M]*256)+WavData[WAV_FILESIZE_L]+AUDIO_DATA_START;
  Data=WavData;
  //Count=0;
  DataIdx=AUDIO_DATA_START;
  Completed=true;
}

float Game_Audio_Wav_Class::getDuration() {
	return ((float)(DataSize - AUDIO_DATA_START) / (float)SampleRate);
}

uint16_t Game_Audio_Wav_Class::getSampleRate() {
	return SampleRate;
}


 


