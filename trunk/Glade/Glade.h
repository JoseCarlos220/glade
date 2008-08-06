/*
 * This file is part of Glade.
 * 
 * (c) 2008 Sofian Audry (info@sofianaudry.com)
 *          Jean-Sébastien Senécal (js@drone.ws)
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

// MAJOR PROBLEM: NOT THE SAME SOUND IF I PUT THIS IN A .cpp FILE!!!


extern "C" {
  #include "WConstants.h"
  #include <string.h>
  #include <avr/pgmspace.h>
}

#include "Audio.h"

#include "cppfix.h"

#ifndef GLADE_H
#define GLADE_H

#include <stdlib.h>

// WHEN IN A .cpp FILE I NEED TO ADD THESE LINES (WHY??? I DON'T UNDERSTAND)
/*#ifndef new
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

typedef signed char signal;
#define SIGNAL_MAX 127
#define SIGNAL_MIN -128

/* Clamps a int32-range int between 0 and 255 inclusive. */
//#ifndef CLAMP0255
//unsigned char CLAMP0255(int a)
//{
//  return (unsigned char)
//    ( (((-a) >> 15) & a)  // 0 if the number was negative
//      | (255 - a) >> 15); // -1 if the number was greater than 255
//}
//#endif

#define CLAMP(x,l,h) ((x) < (l) ? (l) : ( (x) > (h) ? (h) : (x) ))

/* Provided temporary int t, returns a * b / 255 */
#define INT_MULT(a,b,t)  ((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))

/* This version of INT_MULT3 is very fast, but suffers from some
   slight roundoff errors.  It returns the correct result 99.987
   percent of the time */
#define INT_MULT3(a,b,c,t)  ((t) = (a) * (b) * (c) + 0x7F5B, \
                            ((((t) >> 7) + (t)) >> 16))

#define INT_BLEND(a,b,alpha,tmp)  (INT_MULT((a) - (b), alpha, tmp) + (b))

// Basic class
///////////////////////////////////////////
class GladeNode {
public:
  signal output[BUFFER_SIZE];
  GladeNode **inputs;
  uint8_t nInputs;
  
  GladeNode(uint8_t nInputs = 0);  
  virtual ~GladeNode();
  
  void connect(uint8_t input, GladeNode *node);
  void destroy();
  
  virtual void fire();
};

// Sinks
///////////////////////////////////////////

class GladeOutput : public GladeNode {
public:
  GladeOutput();
  ~GladeOutput();
  
  void fire();
  
};

extern GladeOutput dac;

// GladeEngine
///////////////////////////////////////////

#define GLADE_LINE_MAX_NODES 32
class GladeEngine {
  GladeNode *_line[GLADE_LINE_MAX_NODES];
  int _nNodes;
  
protected:
  void _build(GladeNode *node);

public:
  void build();
  void step();
  void run(unsigned long time);
};

extern GladeEngine Glade;

#endif
