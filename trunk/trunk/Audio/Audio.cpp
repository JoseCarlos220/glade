/*
 * This file is part of Glade.
 *
 * (c) 2008 Sofian Audry
 *
 * This code is an wrapper around the speaker_pcm program by
 * Michael Smith (michael@hurts.ca), available at
 * http://www.arduino.cc/playground/Code/PCMAudio
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

#include "Audio.h"
#include "WConstants.h"

unsigned long PWMAudio::_sampleRate = DEFAULT_AUDIO_SAMPLE_RATE;

PWMAudio::PWMAudio(uint8_t channel) {
  setChannel(channel);
}

void PWMAudio::setChannel(uint8_t channel) {
  pinMode(channel, OUTPUT);
  
  // Do non-inverting PWM the pin (p.155)
#if defined(__AVR_ATmega168__)
  if (channel == RIGHT) {
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
    
    _port = &OCR2A;
  } else {
    TCCR2A = (TCCR2A | _BV(COM2B1)) & ~_BV(COM2B0);
    TCCR2A &= ~(_BV(COM2A1) | _BV(COM2A0));
    
    _port = &OCR2B;
  }
  
  (*_port) = 0;
#else
  // XXX ***Not supported for now***.
  //TCCR2 = (TCCR2 | _BV(COM21)) & ~_BV(COM20);
#endif

}

void PWMAudio::start(unsigned long sampleRate) {
  
  // Set sample rate
  _sampleRate = sampleRate;

  // Set up Timer 2 to do pulse width modulation on the speaker
  // pin.

#if defined(__AVR_ATmega168__)
  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
#else
  // XXX ***Not supported for now***.
  // ASSR &= ~_BV(AS2);
#endif

  // Set fast PWM mode  (p.157)
#if defined(__AVR_ATmega168__)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);
#else
  // XXX ***Not supported for now***.
  // TCCR2 |= _BV(WGM21) | _BV(WGM20);
#endif

  // No prescaler (p.158)
#if defined(__AVR_ATmega168__)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
#else
  // XXX ***Not supported for now***.
  // TCCR2 = (TCCR2 & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
#endif

  // Set initial pulse width to the first sample.
//#if defined(__AVR_ATmega168__)
//  OCR2A = 0;
//#else
//  OCR2 = 0;
//#endif

  // Set up Timer 1 to send a sample every interrupt.

  cli();

  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

  // No prescaler (p.134)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  OCR1A = F_CPU / _sampleRate;    // 16e6 / 8000 = 2000

  // Enable interrupt when TCNT1 == OCR1A (p.136)
#if defined(__AVR_ATmega168__)
  TIMSK1 |= _BV(OCIE1A);
#else
  // XXX ***Not supported for now***.
  // TIMSK |= _BV(OCIE1A);
#endif

  sei();
}

void PWMAudio::stop() {
  // Disable playback per-sample interrupt.
#if defined(__AVR_ATmega168__)
  TIMSK1 &= ~_BV(OCIE1A);
#else
  // XXX ***Not supported for now***.
  // TIMSK &= ~_BV(OCIE1A);
#endif

  // Disable the per-sample timer completely.
  TCCR1B &= ~_BV(CS10);

  // Disable the PWM timer.
#if defined(__AVR_ATmega168__)
  TCCR2B &= ~_BV(CS10);
#else
  // XXX ***Not supported for now***.
  // TCCR2 &= ~_BV(CS10);
#endif
}

PWMAudio Audio = PWMAudio(CHANNEL_DEFAULT);