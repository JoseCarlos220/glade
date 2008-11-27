#include <Glade.h>
//#include <GladeOsc.h>
#include <GladeSquare.h>
#include <Audio.h>

//GladeOsc source(220, 1.0f);
GladeSquare source(880, 1.0f);
int ms;

long startTime;
void setup() {
  dac.connect(0, &source);
  Audio.start();
  Glade.build();
  ms = (int) (1000000L / Audio.getSampleRate() * BUFFER_SIZE) / 1000;
  startTime = millis();
  Serial.begin(9600);
  Serial.println(ms, DEC);
}

long ct;
void loop() {
  long ct;
  while ((ct = millis()) - startTime < ms);
  Serial.print(".");
  Glade.step();
  startTime = ct;
}
