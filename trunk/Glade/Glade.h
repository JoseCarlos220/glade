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

#include "WConstants.h"

#include "Audio.h"

#include <string.h>
#include <avr/pgmspace.h>

#ifndef GLADE_H___
#define GLADE_H___
//#ifndef GLADE_H
//#define GLADE_H

#define BUFFER_SIZE 16

/* Clamps a int32-range int between 0 and 255 inclusive. */
//#ifndef CLAMP0255
//unsigned char CLAMP0255(int a)
//{
//  return (unsigned char)
//    ( (((-a) >> 15) & a)  // 0 if the number was negative
//      | (255 - a) >> 15); // -1 if the number was greater than 255
//}
//#endif

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
  byte output[BUFFER_SIZE];
  GladeNode **inputs;
  byte nInputs;
  
  GladeNode(byte nInputs = 0);  
  virtual ~GladeNode();
  
  void connect(byte input, GladeNode *node);
  void destroy();
  
  virtual void fire();
};


// Sources
///////////////////////////////////////////

class GladeSquare : public GladeNode {
public:
  float frequency;
  int amplitude;
  float _phase;
  
  GladeSquare(float frequency, int amplitude);
  ~GladeSquare();
  
  void fire();
  
};  

class GladeClip : public GladeNode {
  float _phase;
public:
  const prog_uchar *clip;
  int clipLength;
  int clipHead;
  int offset;
  float speed;
  
  GladeClip(const prog_uchar *clip, const int clipLength, int offset = 0);
  ~GladeClip();
  
  void fire();
};


// Transformers
///////////////////////////////////////////

class GladeBlend : public GladeNode {
public:
  byte blend;
  GladeBlend(byte blend);
  ~GladeBlend();
  
  void fire();
};

class GladeDelay : public GladeNode {
  byte *_record;
  int _delayLength;
  int _currentIndex;
  
public:
  GladeDelay(int delayLength);
  ~GladeDelay();
  
  void fire();
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
  void run();  
};

extern GladeEngine Glade;

#endif
