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
#define AUDIO_LEFT_PIN 3
#define AUDIO_RIGHT_PIN 11
#else
// no stereo support for now on ATmega8
#define AUDIO_LEFT_PIN 11
#define AUDIO_RIGHT_PIN 11
#endif

#define MONO 1
#define STEREO 2

#define LEFT 0
#define RIGHT 1

class PWMAudio {
  DACBuffer _buffers[2];
  long _sampleRate;
  uint8_t _nChannels;

public:
  PWMAudio();

  long getSampleRate() const { return _sampleRate; }
  uint8_t nChannels() const { return _nChannels; }

  void start(uint8_t nChannels = MONO, long sampleRate = 8000);
  void stop();

  inline bool write(uint8_t value, uint8_t channel = LEFT) {
    return _buffers[channel].write(value);
  }

  inline void play() {
    uint8_t value;
    switch (_nChannels) {
    case STEREO:
      if (_buffers[RIGHT].read(&value))
        analogWrite(AUDIO_RIGHT_PIN, value);
    case MONO:
      if (_buffers[LEFT].read(&value))
        analogWrite(AUDIO_LEFT_PIN, value);
    }
  }
};

extern PWMAudio Audio;

#endif
