/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Benoît Barbot & Paolo Ballarini & Hilal Djafri                     *
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
 * file EventsQueueSet.hpp created by on 31/05/2016                            *
 *******************************************************************************
 */

//
//  EventsQueueSet.hpp
//  Cosmos
//
//  Created by Benoît Barbot on 31/05/2016.
//
//

#ifndef EventsQueueSet_hpp
#define EventsQueueSet_hpp

#include <map>
#include <stdio.h>
#include <vector>

#include "Event.hpp"
#include "spn.hpp"

typedef std::vector<int> Tab;
typedef std::vector<Event> EQueue;
typedef std::vector<Event>::iterator EQit;

class EventsQueueSet {
public:
  // EventsQueue(size_t);
  EventsQueueSet(const std::vector<size_t> &sizes);

  EventsQueueSet(const EventsQueueSet &orig);
  virtual ~EventsQueueSet();

  void insert(const Event &);
  void replace(const Event &);
  void pause(double, size_t, size_t);
  bool restart(double, size_t, size_t);
  void remove(size_t);
  void remove(size_t, size_t);
  bool isScheduled(size_t, size_t) const;
  Event &getEvent(size_t tr, size_t b);

  bool isEmpty() const;
  void printSedCmd(const std::vector<std::string> &labels,
                   std::ostream &) const;
  void view(const std::vector<std::string> &labels) const;

  void reset();
  size_t getSize() const;

  const Event &InPosition(size_t) const;
  const std::map<size_t, long int> &getScheduledBinding(size_t tr) const;

  // private:

  /**
   * The Events Heap Index vector allow to retrive the index
   * of an event in the Events Heap (evtHeap) given its transition
   * and binding index in constant time.
   * If the return value is -1 the corresponding event is not in the event heap.
   * transition -> binding hash-> index in event heap
   */
  std::vector<std::map<size_t, long int>> evtHeapIndex;

  /**
   * This is the vector of events, all the events of every transition
   * and every binding must occurs in this vector.
   * the first index is allong transition and the second along binding index.
   * transition -> binding hash -> Event
   */
  std::vector<std::map<size_t, Event>> evtTbl;

  /**
   * The event heap is a vector of pairs. each pairs are the transition index
   * and binding index of an event in the event table.
   * This vector is a heap for the relation Event::isPriorer.
   * The first element of the heap is the most urgent event.
   * event index -> transition*binding hash
   */
  std::vector<Event *> evtHeap;

private:
  size_t getLeftChildIndex(size_t nodeIndex) const { return 2 * nodeIndex + 1; }

  size_t getRightChildIndex(size_t nodeIndex) const {
    return 2 * nodeIndex + 2;
  }

  size_t getParentIndex(size_t nodeIndex) const { return (nodeIndex - 1) / 2; }

  void siftUp(size_t);
  void siftDown(size_t);
  void swapEvt(size_t, size_t);

  void remove(size_t tr, size_t index, size_t bhash);
};

#endif /* EventsQueueSet_hpp */
