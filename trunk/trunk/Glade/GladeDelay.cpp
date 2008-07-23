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
#include "GladeDelay.h"

GladeDelay::GladeDelay(int delayLength) : GladeNode(1) {
  _record = (signal*) malloc(delayLength * sizeof(byte));
  _currentIndex = 0;
  _delayLength = delayLength;
}

GladeDelay::~GladeDelay() { 
  free(_record);
  GladeNode::destroy(); 
}

void GladeDelay::fire() {
  int i;
  signal *in = inputs[0]->output;
  signal *out = output;
  for (i=0; i<BUFFER_SIZE; i++) {
    _record[_currentIndex] = *in++;
    // boaf...
    *out++ = _record[ (_currentIndex + _delayLength) % _delayLength ];
    _currentIndex = (++_currentIndex) % _delayLength;
  }
}
