/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini & Hilal Djafri                                     *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 *******************************************************************************
 */

/*
 *  This file implement an object for Event of the system
 */

#ifndef _EVENT_HPP
#define _EVENT_HPP

#include "marking.hpp"
#include "string"
#include <iostream>

struct sizeSq {
  sizeSq(size_t t, size_t b) : tr(t), bid(b){};
  size_t tr;
  size_t bid;
};

class Event {
public:
  Event();
  Event(const Event &orig);
  const Event &operator=(const Event &);
  virtual ~Event();
  void setTime(double);
  void setPriority(size_t);
  void setWeight(double);

  size_t transition;
  double time;
  double priority;
  double weight;
  abstractBinding binding;

  bool isPriorer(const Event &e) const;

private:
  Event(unsigned int, double);
  Event(unsigned int, double, unsigned int, double, const abstractBinding &);

  friend std::ostream &operator<<(std::ostream &os, const Event &dt);
};

#endif /* _EVENT_HPP */
