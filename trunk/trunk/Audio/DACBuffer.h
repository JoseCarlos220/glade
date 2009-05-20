#include <stdint.h>
#include <string.h>

#ifndef DAC_BUFFER_H
#define DAC_BUFFER_H

// Should not be bigger than 127 (otherwise there is a risk of overflow in read())
#define AUDIO_BUFFER_SIZE 32

class DACBuffer {
  uint8_t _buffer[AUDIO_BUFFER_SIZE];
  long _writeHead;
  volatile long _playHeadOffset; // = how far is play head from write head
public:
  DACBuffer();

  void reset();

  inline bool write(uint8_t value) {
    // avoid overflowing the audio buffer (wait until space available)
    if (_playHeadOffset < AUDIO_BUFFER_SIZE) {
      _buffer[ _writeHead++ ] = value;
      _writeHead %= AUDIO_BUFFER_SIZE;
      _playHeadOffset++;
      return true;
    } else {
      // buffer is full
      // XXX could add a "overwrite" argument that would force overwriting
      return false;
    }
  }
  /*
    // avoid overflowing the audio buffer (wait until space available)
    if (_writeHead - _playHead < AUDIO_BUFFER_SIZE) {
      _buffer[ (_writeHead++) % AUDIO_BUFFER_SIZE ] = value;
      return true;
    } else {
      return false;
    }
  }*/
  inline bool read(uint8_t *value) {
    if (_playHeadOffset > 0) {
      // XXX we must add drop frame capability
      *value = _buffer[ (_writeHead - (_playHeadOffset--) + AUDIO_BUFFER_SIZE) % AUDIO_BUFFER_SIZE ];
      return true;
    } else {
      return false;
    }
  }
/*
  inline bool read(uint8_t *value) {
    if (_playHead < _writeHead) {
      // XXX we must add drop frame capability
      *value = _buffer[ (_playHead++) % AUDIO_BUFFER_SIZE ];
      return true;
    } else {
      return false;
    }
  }*/

  //  inline bool write(uint8_t value);
  //  inline bool read(uint8_t *value);
};

#endif
