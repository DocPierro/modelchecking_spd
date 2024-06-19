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
 * file markingTemplate.hpp created by Benoit Barbot on 18/05/2016. *
 *******************************************************************************
 */
#ifndef markingTemplate_h
#define markingTemplate_h
#include <assert.h>

/**
 * Type of token when using set for place marking implementation
 */

template <typename T> class DomainGen {
public:
  std::map<T, unsigned int> tokens;

  DomainGen(size_t v = 0) { assert(v == 0); }

  DomainGen(const T &t) { tokens.insert(std::pair<T, int>(t, 1)); }

  void clear() { tokens.clear(); }

  size_t copyVector(std::vector<int> &, size_t) const { return 0; }
  size_t setVector(const std::vector<int> &, size_t) { return 0; }
  DomainGen &operator=(const DomainGen &x) {
    tokens = x.tokens;
    return *this;
  }
  DomainGen &operator=(const T &x) {
    tokens.clear();
    *this += x;
    return *this;
  }
  bool operator==(const DomainGen &x) { return tokens == x.tokens; }

  bool operator<(const T &x) const {
    if (tokens.empty())
      return true;
    if (tokens.size() > 1)
      return false;

    auto xsing = x;
    xsing.mult = 1;
    auto tokDom = tokens.find(xsing);
    if (tokDom != tokens.end())
      return tokDom->second < x.mult;
    return false;
  }

  bool operator>=(const T &x) const {
    auto xsing = x;
    xsing.mult = 1;
    auto tokDom = tokens.find(xsing);
    if (tokDom != tokens.end())
      return tokDom->second >= x.mult;
    return false;
  }

  bool operator<(const DomainGen &x) const {
    bool strictEq = false;
    for (const auto &tok : tokens) {
      auto tokDom2 = x.tokens.find(tok.first);
      if (tokDom2 != x.tokens.end()) {
        if (tokDom2->second < tok.second)
          return false;
        if (tokDom2->second > tok.second)
          strictEq = true;
      } else {
        return false;
      }
    }
    return strictEq;
  }

  bool operator>=(const DomainGen &x) const {
    // Pour tous les tokens de [x], ces tokens ont au moins la même multiplicité
    // dans [tok]
    // bool strictEq = false;
    for (const auto &tok : x.tokens) {
      auto tokDom2 = tokens.find(tok.first);
      if (tokDom2 != x.tokens.end()) {
        if (tokDom2->second < tok.second)
          return false;
      } else {
        return false;
      }
    }
    return true;
  }

  unsigned int card() const {
    unsigned int acc = 0;
    for (const auto &tok : tokens)
      acc += tok.second;
    return acc;
  }

  auto value() const -> decltype(T::c0) {
    if (tokens.begin() == tokens.end())
      return (decltype(T::c0))0;
    return tokens.begin()->first.c0;
  }

  int mult(const T &c) {
    for (const auto &tok : tokens)
      if (c.same_color(tok.first))
        return tok.second;
    return 0;
  }

  DomainGen &operator+=(const DomainGen &x) {
    for (const auto &tok : x.tokens) {
      auto tokDom1 = tokens.find(tok.first);
      if (tokDom1 != tokens.end()) {
        tokDom1->second += tok.second;
      } else {
        tokens.insert(tok);
      }
    }
    return *this;
  }

  DomainGen &operator+=(const T &x) {
    auto xsing = x;
    xsing.mult = 1;
    auto tokDom1 = tokens.find(xsing);
    if (tokDom1 != tokens.end()) {
      tokDom1->second += x.mult;
    } else {
      tokens.insert(std::pair<T, unsigned int>(xsing, x.mult));
    }
    return *this;
  }

  DomainGen &operator+(const T &x) {
    DomainGen &d(*this);
    d += x;
    return d;
  }

  DomainGen &operator-=(const T &x) {
    auto xsing = x;
    xsing.mult = 1;
    auto tokDom1 = tokens.find(xsing);
    if (tokDom1 != tokens.end()) {
      if (x.mult > tokDom1->second) {
        assert(false);
      }
      tokDom1->second -= x.mult;
      if (tokDom1->second == 0) {
        tokens.erase(tokDom1);
      }
    } else {
      std::cerr << "\nTrying to remove inexistant token : ";
      x.print(std::cerr);
      std::cerr << ".\n";
      assert(false);
    }
    return *this;
  }

  DomainGen &operator-=(const DomainGen &x) {
    for (const auto &tokmap : x.tokens) {
      *this -= tokmap.first;
    }
    return *this;
  }

  DomainGen &operator-(const DomainGen &x) {
    DomainGen &d(*this);
    d -= x;
    return d;
  }

  DomainGen &operator-(const T &x) {
    DomainGen &d(*this);
    d -= x;
    return d;
  }

  DomainGen &operator*(int v) {
    for (const auto tok : tokens) {
      tok.second *= v;
    }
    return *this;
  }

  /*
   bool operator > (const patient_Domain& x){
   return  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int),
   (int*)x.mult,std::greater<int>());
   }
   */

  virtual void apply_perm(T, const std::vector<size_t> &) {
    std::cerr << "non implémenté";
    exit(1);
  }
  virtual int compare(T, int, int) const {
    std::cerr << "non implémenté";
    exit(1);
    return 0;
  }
};

template <typename T> struct DomainGen<T> operator+(const T &t1, const T &t2) {
  DomainGen<T> d;
  d += t1;
  d += t2;
  return d;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const DomainGen<T> &x) {
  if (x.tokens.size() == 1) {
    const auto &tok = x.tokens.begin()->first;
    if (tok.mult != 1) {
      tok.print(out);
    } else {
      out << tok.c0;
    }
  } else {
    out << "(";
    for (const auto &t : x.tokens) {
      out << t.second << "*";
      t.first.print(out);
      out << ",";
    }
    out << ")";
  }
  return out;
}

template <typename T> class IterableDomainGen : public DomainGen<T> {
public:
  IterableDomainGen(size_t v = 0) : DomainGen<T>() {
    if (v > 0) {
      T tok = T();
      DomainGen<T>::tokens[tok] = 1;
      while (!tok.islast()) {
        tok.iter();
        DomainGen<T>::tokens[tok] = 1;
      }
    }
  }
};

#endif /* markingTemplate_h */
