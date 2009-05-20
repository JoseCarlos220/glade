#include "DACBuffer.h"

DACBuffer::DACBuffer() {
  reset();
}

void DACBuffer::reset() {
  _writeHead = _playHeadOffset = 0;
  memset(_buffer, 0, AUDIO_BUFFER_SIZE);
}
