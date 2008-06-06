/*
 * This file is part of Glade.
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
