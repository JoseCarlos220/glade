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
#include "GladeBlend.h"

GladeBlend::GladeBlend(float blend) : GladeNode(2) {
  this->blend = (uint8_t) (255 * blend);
}

GladeBlend::~GladeBlend() { GladeNode::destroy(); }

void GladeBlend::fire() {
  int i;
  int tmp;
  signal* out = this->output;
  signal *input1 = this->inputs[0]->output;
  signal *input2 = this->inputs[1]->output;
  for (i=0; i<BUFFER_SIZE; i++) {
    *out++ = INT_BLEND(  ((int)(*input1)),  ((int)(*input2)), blend, tmp );
    input1++; input2++;
  }
}
