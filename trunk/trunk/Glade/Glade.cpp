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

#include "Glade.h"

// GladeNode
///////////////////////////////////////////

GladeNode::GladeNode(uint8_t nInputs) {
  this->nInputs = nInputs;
  if (nInputs > 0)
    inputs = (GladeNode**)malloc(nInputs*sizeof(GladeNode*));
  else
    inputs = 0;
}

GladeNode::~GladeNode() {
  destroy();
}

void GladeNode::connect(uint8_t input, GladeNode *node) {
  inputs[(int)input] = node;
}

void GladeNode::destroy() {
  free(inputs);
}

void GladeNode::fire() {
  memset(this->output, 0, BUFFER_SIZE);
}

// GladeOutput
///////////////////////////////////////////

GladeOutput::GladeOutput() : GladeNode(2) {
}

GladeOutput::~GladeOutput() { GladeNode::destroy(); }

void GladeOutput::fire() {
  int i;
  // Can play stereo if supported.
  for (int c=0; c<nInputs; c++) {
    signal *in = this->inputs[c]->output;
    if (in) {
      //    unsigned long period = 1000000 / SAMPLE_RATE - 7; 
      // subtract 7 us to make up for analogWrite overhead - determined empirically
      for (i=0; i<BUFFER_SIZE; i++) {
        Audio.write( (uint8_t) (((int)*in++) + 128), c);
      }
    }
  }
}

// GladeEngine
///////////////////////////////////////////

void GladeEngine::_build(GladeNode *node) {
  if (node) {
    for (int i=0; i<_nNodes; i++) {
      if (_line[i] == node) // node already in line (feedback)
        return;
    }
    _line[_nNodes++] = node;
    for (int i=0; i<node->nInputs; i++) {
      _build(node->inputs[i]);
    }
  }
}

void GladeEngine::build() {
  _nNodes = 0;
  _build(&dac);
}

void GladeEngine::step() {
  for (int i=_nNodes-1; i>=0; i--)
    _line[i]->fire();
}

void GladeEngine::run(unsigned long time) {
  time += millis(); // reuse the variable to set max time
  while (millis() < time) {
    step();
  }
}

GladeOutput dac = GladeOutput();
GladeEngine Glade = GladeEngine();
