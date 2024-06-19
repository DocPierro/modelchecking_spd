/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
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
 * file marking.cpp created by Benoit Barbot on 28/01/13.                      *
 *******************************************************************************
 */

#ifndef Cosmos_marking_h
#define Cosmos_marking_h

#define let const auto &

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

class abstractMarkingImpl;

class abstractMarking {
public:
  /**
   * Pointer to to the actual marking inmplementation
   * which is generated.
   */
  abstractMarkingImpl *P;

  abstractMarking();
  abstractMarking(const std::vector<int> &m);
  abstractMarking(const abstractMarking &m);
  abstractMarking &operator=(const abstractMarking &m);
  ~abstractMarking();

  //! Swap marking in constant time
  void swap(abstractMarking &m);
  void printHeader(std::ostream &) const;
  void print(std::ostream &, double eTime) const;
  void printSedCmd(std::ostream &) const;
  void resetToInitMarking();
  int getNbOfTokens(int) const;
  std::vector<int> getVector() const;
  void setVector(const std::vector<int> &);

  void Symmetrize();

  template <typename T, typename C>
  std::vector<size_t> getPerm(const std::vector<T> v, C c) const {
    std::vector<size_t> index(static_cast<int>(c), 0);
    for (size_t i = 0; i != index.size(); i++)
      index[i] = i;
    sort(index.begin(), index.end(),
         [&](const int &a, const int &b) { return greaterIndex(v, c, a, b); });
    return index;
  }

  template <typename T, typename C>
  void applyPerm(const std::vector<T *> v, C c, const std::vector<size_t> id) {
    for (let d : v)
      d->apply_perm(c, id);
  }

  /*
      template <typename T, typename C>
      std::vector<size_t> getPerm(const std::vector<T> v, C c) const;
      template <typename T, typename C>
      void applyPerm(const std::vector<T *> v, C c,const std::vector<size_t>);*/
};

struct markingEqState {
  bool operator()(const abstractMarking &t1, const abstractMarking &t2) const;
};

struct markingHashState {
  int operator()(const abstractMarking &t1) const;
};

class abstractBindingImpl;

class abstractBinding {
public:
  abstractBindingImpl *P;
  abstractBinding();
  ~abstractBinding();
  abstractBinding(const abstractBinding &b);
  abstractBinding &operator=(const abstractBinding &m);

  //! Next binding in the binding list of the transition.
  bool next();
  //! Print in human readable format.
  void print() const;
  //! Transition to which the binding belong.
  int transition() const;
  //! Total number of binding in binding list.
  size_t idTotal() const;
  //! Identifier for this binding
  size_t id() const;
  size_t idcount;

  friend class abstractBindingIterator;
};

inline bool contains(int i, int j) { return i >= j; }

class abstractBindingIteratorImpl;

class abstractBindingIterator {
public:
  abstractBindingIteratorImpl *P;

  abstractBindingIterator(const abstractMarking &m);
  ~abstractBindingIterator();
  void reset(size_t t, const abstractMarking &m);
  void next(size_t &t, const abstractMarking &m);
  bool isValid() const;
  size_t getIndex();
  abstractBinding getBinding();

  abstractBinding operator*() { return getBinding(); }
};

template <typename C> struct ColorType {
  const bool isSSC;
  const union {
    C color;
    const bool *subclass;
  };

  ColorType(C c) : isSSC(false), color(c){};
  ColorType(const bool *s) : isSSC(true), subclass(s){};
  long member(int c) {
    if (isSSC) {
      return subclass[c];
    } else {
      return c == color;
    }
  }
};

#endif
