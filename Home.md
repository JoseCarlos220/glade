# Introduction #

**Glade** is a simple, easy-to-use, 8-bit audio library for the [Arduino](http://www.arduino.cc/). It allows you to play sound on the pin 11 of the Arduino board using PWM. The library is separated in two:
  1. the **Audio** library provides a simple interface for 8-bit audio playback, inspired from [this code](http://www.arduino.cc/playground/Code/PCMAudio)
  1. the **Glade** library provides a framework for audio plugins that can be linked to one another in a dataflow-like way, inspired from [ChucK](http://chuck.cs.princeton.edu/) and [PureData](http://puredata.info/)

# Features #

## Supported ##
  * mono and stereo audio playback
  * dataflow connections
  * feedback
  * generators: square wave, audio clip
  * transformers/filters: blend, delay
  * sinks: output (dac)

## Targeted ##
  * generators: triangle/razor wave, sine wave (osc)
  * basic operators: + - **/
  * source: analog-digital-converter (adc)
  * [others...](http://chuck.cs.princeton.edu/doc/program/ugen.html)**

# Example #
```
#include <Audio.h>
#include <Glade.h>
#include <GladeSquare.h>

// Performs a Audio.play() at every audio clock tick.
DEFAULT_AUDIO_TICK();

// build a 440Hz frequency square wave generator with an amplitude of 60%
GladeSquare source(440, 0.6f);

void setup() {
  dac.connect(0, &source); // connect the source to the dac (output)
  Glade.build(); // build the play line (needs to be called whenever connections are changed)
  Audio.start(8000); // start audio playback at 8000 Hz
}

void loop()
{
  source.frequency += 0.001; // will slowly increase frequency
  Glade.step(); // step audio
}
```