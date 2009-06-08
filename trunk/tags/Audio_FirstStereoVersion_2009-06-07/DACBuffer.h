/*
 * This file is part of Glade.
 *
 * (c) 2008 Sofian Audry
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

#ifndef DAC_BUFFER_H
#define DAC_BUFFER_H

// Should not be bigger than 127 (otherwise there is a risk of overflow in read())
#define AUDIO_BUFFER_SIZE 32

class DACBuffer {
  uint8_t _buffer[AUDIO_BUFFER_SIZE];
  long _writeHead;
  volatile long _playHeadOffset; // = how far is play head from write head
  
public:
  DACBuffer();

  void reset();

  inline bool write(uint8_t value) {
    // avoid overflowing the audio buffer (wait until space available)
    if (_playHeadOffset < AUDIO_BUFFER_SIZE) {
      _buffer[ _writeHead++ ] = value;
      _writeHead %= AUDIO_BUFFER_SIZE;
      _playHeadOffset++;
      return true;
    } else {
      // buffer is full
      // XXX could add a "overwrite" argument that would force overwriting
      return false;
    }
  }

  inline bool read(uint8_t *value) {
    if (_playHeadOffset > 0) {
      // XXX we must add drop frame capability
      *value = _buffer[ (_writeHead - (_playHeadOffset--) + AUDIO_BUFFER_SIZE) % AUDIO_BUFFER_SIZE ];
      return true;
    } else {
      return false;
    }
  }
};

#endif
