/*
 * This file is part of Glade.
 *
 * (c) 2008 Sofian Audry
 *
 * This code is an wrapper around the speaker_pcm program by
 * Michael Smith (michael@hurts.ca), available at
 * http://www.arduino.cc/playground/Code/PCMAudio
 *
 * Glade is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Glade is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade.  If not, see <http://www.gnu.org/licenses/>.
*/

//#include "WConstants.h"

#include "DACBuffer.h"
#include "WProgram.h"

#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef AUDIO_H
#define AUDIO_H

#if defined(__AVR_ATmega168__)
#define LEFT 3
#define RIGHT 11
#define CHANNEL_DEFAULT LEFT
#else
#error "Only ATmega168 supported for now"
// no stereo support for now on ATmega8
//#define AUDIO_LEFT_PIN 11
//#define AUDIO_RIGHT_PIN 11
#endif

#define AUDIO_BUFFER_SIZE 32
#define DEFAULT_AUDIO_SAMPLE_RATE 8000

#define AUDIO_TICK ISR(TIMER1_COMPA_vect)
#define PWM_AUDIO_PLAY(object) object.play();

#define DEFAULT_AUDIO_TICK() AUDIO_TICK { PWM_AUDIO_PLAY(Audio); }

class PWMAudio {
  static unsigned long _sampleRate;
  volatile uint8_t *_port;
  uint8_t _value; // internal use
  DACBuffer _buffer;
  
public:
  PWMAudio(uint8_t channel);
  
  void setChannel(uint8_t channel);

  static void start(unsigned long sampleRate = DEFAULT_AUDIO_SAMPLE_RATE);
  static void stop();

public:
  PWMAudio();
  
  inline bool write(uint8_t value) {
    return _buffer.write(value);
  }
  
  inline bool write(uint8_t value, uint16_t timeOut) {
    uint32_t startTime = millis();
    while (millis() - startTime < timeOut) {
      if (write(value))
        return true;
    }
    return false;
  }
  
  inline bool play() {
    if (_buffer.read(&_value)) {
      (*_port) = _value;
    } else
      return false;
  }
  
};

extern PWMAudio Audio;

#endif
