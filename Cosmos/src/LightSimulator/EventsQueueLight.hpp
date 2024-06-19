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
 * file EventsQueueLight.hpp created by Benoit Barbot on 4/02/2015.            *
 *******************************************************************************
 */

#include "Event.hpp"
#include "spnLight.hpp"

#ifndef __Cosmos__EventsQueueLight__
#define __Cosmos__EventsQueueLight__

class EventsQueue {
public:
    //EventsQueue(size_t);
    EventsQueue(const SPN& N);

    void insert(const Event &);
    void replace(const Event &);
    void remove(TR_PL_ID);
    bool isScheduled(TR_PL_ID)const;

    bool isEmpty()const;
    void reset();
    TR_PL_ID getSize()const ;


    const Event& InPosition(TR_PL_ID)const ;

#ifndef NO_STRING_SIM 
    void view()const;
#endif

private:
    TR_PL_ID heap_size;

    /**
     * The Events Heap Index vector allow to retrive the index
     * of an event in the Events Heap (evtHeap) given its transition
     * and binding index in constant time.
     * If the return value is -1 the corresponding event is not in the event heap.
     */
    TR_PL_ID evtHeapIndex[NB_EVENT];

    /**
     * This is the vector of events, all the events of every transition
     * and every binding must occurs in this vector.
     * the first index is allong transition and the second along binding index.
     */
    //Event evtTbl[NB_EVENT];


    /**
     * The event heap is a vector of pairs. each pairs are the transition index
     * and binding index of an event in the event table.
     * This vector is a heap for the relation Event::isPriorer.
     * The first element of the heap is the most urgent event.
     */
    Event evtHeap[NB_EVENT];

    TR_PL_ID getLeftChildIndex(TR_PL_ID nodeIndex)const {
        return 2 * nodeIndex + 1;
    }

    TR_PL_ID getRightChildIndex(TR_PL_ID nodeIndex)const {
        return 2 * nodeIndex + 2;
    }

    TR_PL_ID getParentIndex(TR_PL_ID nodeIndex)const {
        return (nodeIndex - 1) / 2;
    }

    void siftUp(TR_PL_ID );
    void siftDown(TR_PL_ID );
    void swapEvt(TR_PL_ID ,TR_PL_ID );


};


#endif /* defined(__Cosmos__EventsQueueLight__) */
