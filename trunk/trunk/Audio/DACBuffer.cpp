#include "DACBuffer.h"

DACBuffer::DACBuffer() {
  reset();
}

void DACBuffer::reset() {
  _writeHead = _playHeadOffset = 0;
  memset(_buffer, 0, AUDIO_BUFFER_SIZE);
}

//inline bool DACBuffer::write(uint8_t value) {
//  // avoid overflowing the audio buffer (wait until space available)
//  if (_writeHead - _playHead < AUDIO_BUFFER_SIZE) {
//    _buffer[ (_writeHead++) % AUDIO_BUFFER_SIZE ] = value;
//    return true;
//  } else {
//    return false;
//  }
//}
//
//inline bool DACBuffer::read(uint8_t *value) {
//  if (_playHead < _writeHead) {
//    // XXX we must add drop frame capability
//    *value = _buffer[ (_playHead++) % AUDIO_BUFFER_SIZE ];
//    return true;
//  } else {
//    return false;
//  }
//}