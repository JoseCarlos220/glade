// MAJOR PROBLEM: NOT THE SAME SOUND IF I PUT THIS IN A .cpp FILE!!!

#include "WConstants.h"

#ifndef GLADE_LINE_H
#define GLADE_LINE_H

// WHEN IN A .cpp FILE I NEED TO ADD THESE LINES (WHY??? I DON'T UNDERSTAND)
/*#include <stdlib.h>
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
#endif*/

#define BUFFER_SIZE 16

// Basic class
///////////////////////////////////////////
class GladeNode {
public:
  byte output[BUFFER_SIZE];
  GladeNode **inputs;
  byte nInputs;
  
  GladeNode(byte nInputs = 0) {
    this->nInputs = nInputs;
    if (nInputs > 0) {
      inputs = (GladeNode**)malloc(nInputs*sizeof(GladeNode*));
      memset(inputs, 0, nInputs);
    }
    else
      inputs = 0;
  }
  
  ~GladeNode() {
    destroy();
  }
  
  void connect(byte input, GladeNode *node) {
    inputs[(int)input] = node;
  }
  
  virtual void fire() {
    memset(this->output, 0, BUFFER_SIZE);
  }
  
  void destroy() {
    free(inputs);
  }
  
};


// Sources
///////////////////////////////////////////

class GladeSquare : public GladeNode {
public:
  float frequency;
  int amplitude;
  float _phase;
  
  GladeSquare(float frequency, int amplitude) : GladeNode(0) {
    this->frequency = frequency;
    this->amplitude = amplitude;
    this->_phase = 0;
  }
  
  ~GladeSquare() { GladeNode::destroy(); }
  
  void fire() {
    int i;
    float num = (float)this->frequency / (float)SAMPLE_RATE;
    byte* out = this->output;
    for (i=0; i<BUFFER_SIZE; i++) {
      *out++ = (this->_phase < 0.5 ? 127 - this->amplitude : 127 + this->amplitude);
      this->_phase += num;
      if (this->_phase > 1) this->_phase = 0;
    }
  }
  
};  

class GladeClip : public GladeNode {
  float _phase;
public:
  const prog_uchar *clip;
  int clipLength;
  int clipHead;
  float speed;
  
  GladeClip(const prog_uchar *clip, const int clipLength, int clipHead = 0) : GladeNode(0) {
    this->clip = clip;
    this->clipLength = clipLength;
    this->clipHead = clipHead;
    speed = 1.0f;
    _phase = 0;
  }
  
  ~GladeClip() { 
    GladeNode::destroy(); 
  }
  
  void fire() {
    int i;
    byte *out = output;
    for (i=0; i<BUFFER_SIZE; i++) {
      *out++ = pgm_read_byte(&clip[clipHead]);
      _phase += speed;
      if (_phase >= 1) {
        clipHead = (clipHead + (int)_phase) % clipLength;
        _phase -= (int)_phase;
      }
    }
  }
};


// Transformers
///////////////////////////////////////////

class GladeBlend : public GladeNode {
public:
  byte blend;
  GladeBlend(byte blend) : GladeNode(2) {
    this->blend = blend;
  }
  ~GladeBlend() { GladeNode::destroy(); }
  
  void fire() {
    int i;
    byte oneMinusBlend = 255 - blend;
    byte* out = this->output;
    byte *input1 = inputs[0]->output;
    byte *input2 = inputs[1]->output;
    for (i=0; i<BUFFER_SIZE; i++) {
      *out++ = (byte) ( (((unsigned int)*input1++) * blend + ((unsigned int)*input2++) * oneMinusBlend) / 255 );
    }
  }
  
};

class GladeDelay : public GladeNode {
  byte *_record;
  int _delayLength;
  int _currentIndex;
  
public:
  GladeDelay(int delayLength) : GladeNode(1) {
    _record = (byte*) malloc(delayLength * sizeof(byte));
    _currentIndex = 0;
    _delayLength = delayLength;
  }
  
  ~GladeDelay() { 
    free(_record);
    GladeNode::destroy(); 
  }
  
  void fire() {
    int i;
    byte *in = inputs[0]->output;
    byte *out = output;
    for (i=0; i<BUFFER_SIZE; i++) {
      _record[_currentIndex] = *in++;
      // boaf...
      *out++ = _record[ (_currentIndex + _delayLength) % _delayLength ];
      _currentIndex = (++_currentIndex) % _delayLength;
    }
  }
};

// Sinks
///////////////////////////////////////////

class GladeOutput : public GladeNode {
public:
  GladeOutput() : GladeNode(1) {
  }
  
  ~GladeOutput() { GladeNode::destroy(); }
  
  void fire() {
    int i;
    byte *in = this->inputs[0]->output;
//    unsigned long period = 1000000 / SAMPLE_RATE - 7; // subtract 7 us to make up for analogWrite overhead - determined empirically
    for (i=0; i<BUFFER_SIZE; i++) {
      Audio.dacWrite(*in++);
    }
  }
  
};

GladeOutput dac;

#define Glade_LINE_MAX_NODES 32
class GladeEngine {
  GladeNode *_line[Glade_LINE_MAX_NODES];
  int _nNodes;
  
public:
  void build() {
    _nNodes = 0;
    _build(&dac);
  }
  
  void _build(GladeNode *node) {
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
  
  void run() {
    for (int i=_nNodes-1; i>=0; i--)
      _line[i]->fire();
  }
};

GladeEngine Glade;


#endif
