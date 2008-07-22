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

#include "WProgram.h"
#include "WConstants.h"

#include "Glade.h"

// WHEN IN A .cpp FILE I NEED TO ADD THESE LINES (WHY??? I DON'T UNDERSTAND)
#ifndef new
void * operator new(size_t size)
{
  return malloc(size);
}
#endif
#ifndef delete
void operator delete(void * ptr)
{
  free(ptr);
}
#endif

// GladeNode
///////////////////////////////////////////

GladeNode::GladeNode(byte nInputs) {
  this->nInputs = nInputs;
  if (nInputs > 0)
    inputs = (GladeNode**)malloc(nInputs*sizeof(GladeNode*));
  else
    inputs = 0;
}

GladeNode::~GladeNode() {
  destroy();
}

void GladeNode::connect(byte input, GladeNode *node) {
  inputs[(int)input] = node;
}

void GladeNode::destroy() {
  free(inputs);
}

void GladeNode::fire() {
  memset(this->output, 0, BUFFER_SIZE);
}

// GladeSquare
///////////////////////////////////////////

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

// GladeClip
///////////////////////////////////////////
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

// GladeAdd
///////////////////////////////////////////
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

// GladeBlend
///////////////////////////////////////////
GladeBlend::GladeBlend(byte blend) : GladeNode(2) {
  this->blend = blend;
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

// GladeDelay
///////////////////////////////////////////

GladeDelay::GladeDelay(int delayLength) : GladeNode(1) {
  _record = (byte*) malloc(delayLength * sizeof(byte));
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
        Audio.write( (byte) (((int)*in++) + 128), c);
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

void GladeEngine::run(long time) {
  time += millis(); // reuse the variable to set max time
  while (millis() < time) {
    step();
  }
}

GladeOutput dac = GladeOutput();
GladeEngine Glade = GladeEngine();
