// MAJOR PROBLEM: NOT THE SAME SOUND IF I PUT THIS IN A .cpp FILE!!!

#include "WConstants.h"

#ifndef AUDIO_LINE_H
#define AUDIO_LINE_H

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
class AudioNode {
public:
  byte output[BUFFER_SIZE];
  AudioNode **inputs;
  byte nInputs;
  
  AudioNode(byte nInputs = 0) {
    this->nInputs = nInputs;
    if (nInputs > 0) {
      inputs = (AudioNode**)malloc(nInputs*sizeof(AudioNode*));
      memset(inputs, 0, nInputs);
    }
    else
      inputs = 0;
  }
  
  ~AudioNode() {
    destroy();
  }
  
  void connect(byte input, AudioNode *node) {
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

class AudioSquare : public AudioNode {
public:
  float frequency;
  int amplitude;
  float _phase;
  
  AudioSquare(float frequency, int amplitude) : AudioNode(0) {
    this->frequency = frequency;
    this->amplitude = amplitude;
    this->_phase = 0;
  }
  
  ~AudioSquare() { AudioNode::destroy(); }
  
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

class AudioClip : public AudioNode {
  float _phase;
public:
  const prog_uchar *clip;
  int clipLength;
  int clipHead;
  float speed;
  
  AudioClip(const prog_uchar *clip, const int clipLength, int clipHead = 0) : AudioNode(0) {
    this->clip = clip;
    this->clipLength = clipLength;
    this->clipHead = clipHead;
    speed = 1.0f;
    _phase = 0;
  }
  
  ~AudioClip() { 
    AudioNode::destroy(); 
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

class AudioBlend : public AudioNode {
public:
  byte blend;
  AudioBlend(byte blend) : AudioNode(2) {
    this->blend = blend;
  }
  ~AudioBlend() { AudioNode::destroy(); }
  
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

class AudioDelay : public AudioNode {
  byte *_record;
  int _delayLength;
  int _currentIndex;
  
public:
  AudioDelay(int delayLength) : AudioNode(1) {
    _record = (byte*) malloc(delayLength * sizeof(byte));
    _currentIndex = 0;
    _delayLength = delayLength;
  }
  
  ~AudioDelay() { 
    free(_record);
    AudioNode::destroy(); 
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

class AudioOutput : public AudioNode {
public:
  AudioOutput() : AudioNode(1) {
  }
  
  ~AudioOutput() { AudioNode::destroy(); }
  
  void fire() {
    int i;
    byte *in = this->inputs[0]->output;
//    unsigned long period = 1000000 / SAMPLE_RATE - 7; // subtract 7 us to make up for analogWrite overhead - determined empirically
    for (i=0; i<BUFFER_SIZE; i++) {
      Audio::dacWrite(*in++);
    }
  }
  
};

AudioOutput dac;

#define AUDIO_LINE_MAX_NODES 32
class AudioLine {
  static AudioNode *_line[AUDIO_LINE_MAX_NODES];
  static int _nNodes;
  
public:
  static void build() {
    _nNodes = 0;
    _build(&dac);
  }
  
  static void _build(AudioNode *node) {
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
  
  static void run() {
    for (int i=_nNodes-1; i>=0; i--)
      _line[i]->fire();
  }
};

AudioNode *AudioLine::_line[AUDIO_LINE_MAX_NODES];
int AudioLine::_nNodes = 0;  


#endif
