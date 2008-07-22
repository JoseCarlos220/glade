#include <Audio.h>

#define PERIOD 100

void setup() {
  Audio.start();
}

void loop() {
  for (int i=0; i<PERIOD; i++)
    Audio.write( 255 );
  for (int i=0; i<PERIOD; i++)
    Audio.write( 0 );
}