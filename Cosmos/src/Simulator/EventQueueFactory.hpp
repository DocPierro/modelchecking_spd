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
 * file EventsQueueFactory.hpp created by Benoit Barbot on 06/10/2016          *
 *******************************************************************************
 */
#ifndef EventQueueFactory_hpp
#define EventQueueFactory_hpp

#include "EventsQueue.hpp"
#include "EventsQueueSet.hpp"

#endif /* EventQueueFactory_hpp */
/*
class EventQueueFactory {
public:
    EventQueueFactory(bool isSet){
        this->isSet = isSet;
    }

    EventsQueue* newEventQueue(const SPN& N)const{
        if(isSet){
            EventsQueue* eq = new EventsQueueSet(N);
            return eq;
        }else {
            return new EventsQueue(N);
        }
    }

private:
    bool isSet;

}
*/
