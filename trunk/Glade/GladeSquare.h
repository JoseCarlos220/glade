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

#include "WConstants.h"
#include "Glade.h"

#ifndef GLADE_SQUARE_H
#define GLADE_SQUARE_H

class GladeSquare : public GladeNode {
public:
  float frequency;
  float dutyCycle;
  float _phase;
  uint8_t amplitude;
  
  GladeSquare(float frequency, float amplitude = 1.0f, float dutyCycle = 0.5f);
  ~GladeSquare();
  
  void fire();
};  

#endif
