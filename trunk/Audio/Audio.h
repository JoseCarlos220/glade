#include "WConstants.h"

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef PWMAudio_H
#define PWMAudio_H

#define AUDIO_SPEAKER_PIN 11
#define AUDIO_BUFFER_SIZE 256

// XXX should be a  variable from PWMAudio
#define SAMPLE_RATE 8000 // The number of Hz 

class PWMAudio {

  byte _dacBuffer[AUDIO_BUFFER_SIZE];
  long _dacBufferWriteHead;
  long _dacBufferPlayHead;
  
public:
  
  void start();
  void stop();
  void dacWrite(byte value);  
  void dacPlay();
};

PWMAudio Audio;

#endif
