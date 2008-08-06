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

// XXX: For some reason stdlib MUST be included BEFORE WConstants.h in 
// the .CPP file. Otherwise syntax errors in stdlib.h happen (strange...)
#include <stdlib.h>

#include "GladeClip.h"

GladeClip::GladeClip(const prog_uchar *clip, const int clipLength, int offset) : GladeNode(0) {
  this->clip = clip;
  this->clipLength = clipLength;
  this->clipHead = clipHead;
  this->offset = offset;
  speed = 1.0f;
  _phase = 0;
}

GladeClip::~GladeClip() { 
  GladeNode::destroy(); 
}

void GladeClip::fire() {
  int i;
  signal *out = output;
  for (i=0; i<BUFFER_SIZE; i++) {
    *out++ = (pgm_read_byte(&clip[(clipHead + offset) % clipLength]) + SIGNAL_MIN);
    _phase += speed;
    if (_phase >= 1) {
      clipHead = (clipHead + (int)_phase) % clipLength;
      _phase -= (int)_phase;
    }
  }
}
