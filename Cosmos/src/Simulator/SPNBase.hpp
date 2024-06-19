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
 * file spn_orig.hpp created by Benoit Barbot on 03/09/15.                     *
 *******************************************************************************
 */

#ifndef __Cosmos__spn_orig__
#define __Cosmos__spn_orig__

#include "Event.hpp"
#include "spn.hpp"
#include "timeGen.hpp"
#include "timeGenBis.hpp"

#include "EventsQueue.hpp"
#include "EventsQueueSet.hpp"

template <class S, class EQT> class SPNBase : public SPN {
public:
  SPNBase(size_t s) : shift(s){};
  const size_t shift;

  void update(double ctime, size_t tr, const abstractBinding &b,
              EventsQueue &EQ, timeGen &TG);
  void update(double ctime, size_t tr, const abstractBinding &b,
              EventsQueueSet &EQ, timeGen &TG);

  void initialEventsQueue(EventsQueue &EQ, timeGen &TG);
  void initialEventsQueue(EventsQueueSet &EQ, timeGen &TG);

  void printHeader(std::ostream &os) const { Marking.printHeader(os); }
  void printSedCmd(std::ostream &os) const { Marking.printSedCmd(os); }
  void printState(std::ostream &os, double eTime) const {
    Marking.print(os, eTime);
  }

  const std::vector<size_t> getNbTransition() const;
  const std::vector<std::string> getTransitionLabels() const;
  const std::string getTransitionLabel(size_t i) const;

protected:
  //! a Temporary event
  Event F;

private:
  void cleanNewlyDisabled(size_t tr, EventsQueueSet &EQ);
};

template <class EQT> class SPN_orig : public SPNBase<SPN_orig<EQT>, EQT> {
public:
  SPN_orig(size_t shift) : SPNBase<SPN_orig, EQT>(shift){};
};

#endif /* defined(__Cosmos__spn_orig__) */
