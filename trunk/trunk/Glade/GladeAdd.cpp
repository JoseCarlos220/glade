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
#include "GladeAdd.h"

GladeAdd::GladeAdd() : GladeNode(2) {
}
GladeAdd::~GladeAdd() { GladeNode::destroy(); }

void GladeAdd::fire() {
  int i;
  signal* out = this->output;
  signal *input1 = this->inputs[0]->output;
  signal *input2 = this->inputs[1]->output;
  for (i=0; i<BUFFER_SIZE; i++) {
    *out++ = (signal) CLAMP(((int)*input1++) + ((int)*input2++), SIGNAL_MIN, SIGNAL_MAX);
  }
}

