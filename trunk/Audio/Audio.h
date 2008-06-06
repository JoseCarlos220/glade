#include "WConstants.h"

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef AUDIO_H
#define AUDIO_H

#define AUDIO_SPEAKER_PIN 11
#define AUDIO_BUFFER_SIZE 256

// XXX should be a static variable from Audio
#define SAMPLE_RATE 8000 // The number of Hz 

class Audio {

  static byte _dacBuffer[AUDIO_BUFFER_SIZE];
  static long _dacBufferWriteHead;
  static long _dacBufferPlayHead;
  
public:
  
  static void start();
  static void stop();
  static void dacWrite(byte value);  
  static void dacPlay();
};

#endif
