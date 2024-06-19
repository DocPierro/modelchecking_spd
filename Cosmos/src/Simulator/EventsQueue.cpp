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

#include "EventsQueue.hpp"
#include "Event.hpp"
#include <algorithm>
#include <assert.h>
#include <iomanip>
#include <iostream>

using namespace std;

/**
 *	Build an Event queue for the Petri net given as parameter.
 */
EventsQueue::EventsQueue(const std::vector<size_t> &t)
    : evtTbl(t.size(), vector<Event>()) {
  auto comp = 0;
  for (size_t it = 0; it < t.size(); ++it) {
    // evtTbl.push_back(vector<Event>());
    evtHeapIndex.push_back(vector<long int>(t[it], -1));
    for (size_t it2 = 0; it2 < t[it]; ++it2) {
      evtTbl[it].push_back(Event());
      comp++;
    }
  }
  evtHeap.reserve(comp);
}

/**
 * Clear the events queue. The resulting events queues does not contain any
 * event.
 */
void EventsQueue::reset() {
  for (size_t it = 0; it < evtHeapIndex.size(); ++it)
    for (size_t it2 = 0; it2 < evtHeapIndex[it].size(); ++it2) {
      evtHeapIndex[it][it2] = -1;
      evtTbl[it][it2].time = -1.0;
    }
  evtHeap.clear();
  // Qsize = 0;
}

/**
 * Copy of events queue
 */
EventsQueue::EventsQueue(const EventsQueue &orig) {

  // Qsize = orig.Qsize;

  evtTbl = orig.evtTbl;
  evtHeap = orig.evtHeap;
  evtHeapIndex = orig.evtHeapIndex;

  /*eq = orig.eq;
   TransTableSize = orig.TransTableSize;
   TransTable = orig.TransTable;*/
}

EventsQueue::~EventsQueue() {}

/*
 *	Insert a new event in the events queue and update the tree.
 *  @param e an event of the Petri net.
 */
void EventsQueue::insert(const Event &e) {
  // assert(!isScheduled(e.transition, e.binding.id()));
  evtTbl[e.transition][e.binding.id()] = e;
  evtHeapIndex[e.transition][e.binding.id()] = evtHeap.size();
  evtHeap.push_back(sizeSq(e.transition, e.binding.id()));

  siftUp(evtHeap.size() - 1);
}

/*
 *	Replace the time, priority,service of an event and update the tree.
 *  @param e an event of the Petri net.
 */
void EventsQueue::replace(const Event &e) {
  // assert(isScheduled(e.transition, e.binding.id()));
  long int k = evtHeapIndex[e.transition][e.binding.id()];
  evtTbl[e.transition][e.binding.id()] = e;

  siftUp(k);
  siftDown(k);
}

/*
 *	Remove an event and update the tree.
 *  @param tr a transition of the Petri net.
 *  @param b a binding of the Petri net.
 */
void EventsQueue::remove(size_t tr, size_t b) {
  long int i = evtHeapIndex[tr][b];
  evtTbl[tr][b].time = -1.0;
  // assert(i>=0);
  if (i >= 0) {
    if ((size_t)i == evtHeap.size() - 1) {
      evtHeapIndex[tr][b] = -1;
      evtHeap.pop_back();
    } else {
      evtHeapIndex[evtHeap.back().tr][evtHeap.back().bid] = i;
      evtHeapIndex[tr][b] = -1;
      evtHeap[i] = evtHeap.back();
      evtHeap.pop_back();

      siftDown(i);
      siftUp(i);
    }
  }
}

/**
 *  Pause an event, remove it from the tree
 *  @param t is the current time
 *  @param tr a transition of the Petri net.
 *  @param b a binding of the Petri net.
 */
void EventsQueue::pause(double t, size_t tr, size_t b) {
  evtTbl[tr][b].time -= t;
  long int i = evtHeapIndex[tr][b];
  // assert(i>=0);
  if (i >= 0) {
    if ((size_t)i == evtHeap.size() - 1) {
      evtHeapIndex[tr][b] = -1;
      evtHeap.pop_back();
    } else {
      evtHeapIndex[evtHeap.back().tr][evtHeap.back().bid] = i;
      evtHeapIndex[tr][b] = -1;
      evtHeap[i] = evtHeap.back();
      evtHeap.pop_back();

      siftDown(i);
      siftUp(i);
    }
  }
}

/**
 * Check if an event can be restarted. If it is possible
 * restart it otherwise return false.
 *  @param t is the current time
 *  @param tr a transition of the Petri net.
 *  @param b a binding of the Petri net.
 */
bool EventsQueue::restart(double t, size_t tr, size_t b) {
  if (evtTbl[tr][b].time < 0.0)
    return false;
  evtTbl[tr][b].time += t;
  evtHeapIndex[tr][b] = evtHeap.size();
  evtHeap.push_back(sizeSq(tr, b));

  siftUp(evtHeap.size() - 1);
  return true;
}

const Event &EventsQueue::InPosition(size_t i) const {
  // assert(i < evtHeap.size());
  return evtTbl[evtHeap[i].tr][evtHeap[i].bid];
}

bool EventsQueue::isScheduled(size_t tr, size_t b) const {
  return (evtHeapIndex[tr][b] >= 0);
}

Event &EventsQueue::getEvent(size_t tr, size_t b) {
  Event &e = evtTbl[tr][b];
  return e;
}

void EventsQueue::swapEvt(size_t i, size_t j) {
  // assert((size_t)evtHeapIndex[evtHeap[j].first][evtHeap[j].second] ==j
  //	   && (size_t)evtHeapIndex[evtHeap[i].first][evtHeap[i].second] == i);
  evtHeapIndex[evtHeap[j].tr][evtHeap[j].bid] = i;
  evtHeapIndex[evtHeap[i].tr][evtHeap[i].bid] = j;

  sizeSq swappair = evtHeap[j];
  evtHeap[j] = evtHeap[i];
  evtHeap[i] = swappair;
}

void EventsQueue::siftUp(size_t i) {
  size_t parentIndex;

  if (i != 0) {
    parentIndex = getParentIndex(i);

    if (InPosition(i).isPriorer(InPosition(parentIndex))) {
      swapEvt(i, parentIndex);
      siftUp(parentIndex);
    }
  }
}

void EventsQueue::siftDown(size_t i) {
  size_t leftChildIndex, rightChildIndex, minIndex;
  leftChildIndex = getLeftChildIndex(i);
  rightChildIndex = getRightChildIndex(i);
  if (rightChildIndex >= evtHeap.size()) {
    if (leftChildIndex >= evtHeap.size())
      return;
    else
      minIndex = leftChildIndex;
  } else {

    if (InPosition(leftChildIndex).isPriorer(InPosition(rightChildIndex)))
      minIndex = leftChildIndex;
    else
      minIndex = rightChildIndex;
  }

  if (InPosition(minIndex).isPriorer(InPosition(i))) {
    swapEvt(minIndex, i);
    siftDown(minIndex);
  }
}

bool EventsQueue::isEmpty() const { return (evtHeap.size() == 0); }

void EventsQueue::printSedCmd(const std::vector<std::string> &trlabl,
                              ostream &df) const {
  if (evtHeap.size() > 0) {
    df << "-e 's/\\$CF_" << trlabl[InPosition(0).transition] << "\\$/Red/g' ";
    for (unsigned int i = 1; i < evtHeap.size(); i++) {
      df << "-e 's/\\$CF_" << trlabl[InPosition(i).transition]
         << "\\$/Lightblue/g' ";
    }
  }
  df << "-e 's/\\$CF_[^\\$]*\\$/Black/g' ";
  for (size_t i = 0; i < trlabl.size(); i++) {
    df << "-e 's/\\$ET_" << trlabl[i] << "\\$/";
    if (isScheduled(i, 0)) {
      let ev = evtTbl[i][0];
      df << "(" << ev.time << ", " << ev.priority << ", " << ev.weight << ")";
    }
    df << "/g' ";
  }
}

/**
 *	Print the content of the queues in a human readable format.
 */
void EventsQueue::view(const std::vector<std::string> &trlabl) const {
  cerr << "********** EVENTS-QUEUE VIEW **********" << endl;

  // cerr << "Qsize:" << evtHeap.size() << endl;

  if (evtHeap.size() == 0)
    cerr << "EVENTS-QUEUE is empty!" << endl;
  else
    for (unsigned int i = 0; i < evtHeap.size(); i++) {
      Event e = InPosition(i);
      // cerr << "Equeue[" << i << "]:" << "( ";
      auto tr = trlabl[e.transition];
      // if(tr.DistTypeIndex == IMMEDIATE)cerr << "\033[1;33m";
      cerr << setw(15) << left << tr << "(" << e.transition << ") :";
      // cerr << "tr ID:" << setw(4)<< e.transition << " ";
      e.binding.print();
      cerr << ",\tt=" << e.time << ",\tp=" << e.priority << ",\tw=" << e.weight;
      // if(tr.DistTypeIndex == IMMEDIATE)cerr << "\033[0m";
      cerr << endl;
    }
}
