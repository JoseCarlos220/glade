/*
 * This file is part of Glade.
 * 
 * (c) 2008-2009 Sofian Audry | info((@))sofianaudry((.))com
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

#ifndef GLADE_DELAY_H
#define GLADE_DELAY_H

class GladeDelay : public GladeNode {
  signal *_record;
  int _delayLength;
  int _currentIndex;
  
public:
  GladeDelay(int delayLength);
  ~GladeDelay();
  
  void fire();
};

#endif
