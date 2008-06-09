/*
 * This file is part of Glade.
 * 
 * (c) 2008 Sofian Audry (info@sofianaudry.com)
 *          Jean-Sébastien Senécal (js@drone.ws)
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
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef AUDIO_H
#define AUDIO_H

#define AUDIO_SPEAKER_PIN 11
#define AUDIO_BUFFER_SIZE 64

// XXX should be a  variable from PWMAudio
#define SAMPLE_RATE 8000 // The number of Hz 

class PWMAudio {

  byte _dacBuffer[AUDIO_BUFFER_SIZE];
  long _dacBufferWriteHead;
  volatile long _dacBufferPlayHead;
  
public:
  
  void start();
  void stop();
  void dacWrite(byte value);  
  void dacPlay();
};

extern PWMAudio Audio;

#endif
