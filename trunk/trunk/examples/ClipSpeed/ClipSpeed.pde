#include <Audio.h>
#include <Glade.h>

#include "sounddata.h"

GladeClip source1(sounddata_data, sounddata_length);

void setup() {
  Serial.begin(19200);
  Audio.start();
  dac.connect(0, &source1);
  Glade.build();
}

void loop()
{
  if (Serial.available()) {
    switch (Serial.read()) {
     case '+':
      source1.speed *= 1.2;
      break;
     case '-':
      source1.speed *= 0.8;
      break;
    }
  }
  Glade.run();
}
