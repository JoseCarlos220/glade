/*
 * This file is part of Glade.
 * 
 * (c) 2008 Sofian Audry (info@sofianaudry.com)
 *          Jean-Sébastien Senécal (js@drone.ws)
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

#define AUDIO_BUFFER_SIZE 32

#define MONO 1
#define STEREO 2

#define LEFT 0
#define RIGHT 1

class DACBuffer {
  uint8_t _buffer[AUDIO_BUFFER_SIZE];
  long _writeHead;
  volatile long _playHead;
public:
  DACBuffer();
  inline bool write(uint8_t value);
  inline bool read(uint8_t *value);
};

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
  
  void write(uint8_t value, uint8_t channel = LEFT);
  void play();
};

extern PWMAudio Audio;

#endif
