/*
 * This file is part of Glade.
 * 
 * (c) 2008-2009 Sofian Audry | info((@))sofianaudry((.))com
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

#include "DACBuffer.h"

DACBuffer::DACBuffer() {
  reset();
}

void DACBuffer::reset() {
  _writeHead = _playHeadOffset = 0;
  memset(_buffer, 0, AUDIO_BUFFER_SIZE);
}
