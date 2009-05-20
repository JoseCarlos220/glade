/*
 * This file is part of Glade.
 * 
 * (c) 2008-2009 Sofian Audry | info((@))sofianaudry((.))com
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

// XXX: For some reason stdlib MUST be included BEFORE WConstants.h in 
// the .CPP file. Otherwise syntax errors in stdlib.h happen (strange...)
#include <stdlib.h>

#include "GladeSquare.h"

GladeSquare::GladeSquare(float frequency, float amplitude, float dutyCycle) : GladeNode(0) {
  this->frequency = frequency;
  this->amplitude = (byte) constrain(amplitude * SIGNAL_MAX, 0, SIGNAL_MAX);
  this->dutyCycle = dutyCycle;
  this->_phase = 0;
}

GladeSquare::~GladeSquare() { GladeNode::destroy(); }

void GladeSquare::fire() {
  int i;
  float num = (float)this->frequency / (float)Audio.getSampleRate();
  signal* out = this->output;
  for (i=0; i<BUFFER_SIZE; i++) {
    *out++ = (this->_phase < this->dutyCycle ? +this->amplitude : -this->amplitude);
    this->_phase += num;
    if (this->_phase > 1) this->_phase = 0;
  }
}
