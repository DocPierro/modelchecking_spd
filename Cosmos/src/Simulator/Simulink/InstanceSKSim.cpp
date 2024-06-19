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
 * file InstanceSPNSim.cpp created by Benoît Barbot on 17/05/2017.             *
 *******************************************************************************
 */


//   Classes in SKModel
#include "SKModel.hpp"
#include "../Simulator.cpp"
#include "../SPNBase.cpp"

template class SPNBase<SPN_orig<EventsQueue>, EventsQueue>;
template class SPNBase<SPN_orig<EventsQueueSet>, EventsQueueSet>;

//template class SimulatorBase<Simulator<EventsQueue,SKModel<EventsQueue>>, EventsQueue,SKModel<EventsQueue>>;
//template class SimulatorBase<Simulator<EventsQueueSet,SKModel<EventsQueueSet>>, EventsQueueSet,SKModel<EventsQueueSet>>;

#include "../MultiModel.hpp"
template class SimulatorBase<Simulator<EventsQueue, MultiModel<EventsQueue, SPN_orig<EventsQueue>, SKModel<EventsQueue> > >, EventsQueue, MultiModel<EventsQueue, SPN_orig<EventsQueue>, SKModel<EventsQueue> > >;

template class MultiModel<EventsQueue, SPN_orig<EventsQueue>, SKModel<EventsQueue>>;

template class SimulatorBase<Simulator<EventsQueueSet, MultiModel<EventsQueueSet, SPN_orig<EventsQueueSet>, SKModel<EventsQueueSet> > >, EventsQueueSet, MultiModel<EventsQueueSet, SPN_orig<EventsQueueSet>, SKModel<EventsQueueSet> > >;

template class MultiModel<EventsQueueSet, SPN_orig<EventsQueueSet>, SKModel<EventsQueueSet>>;


/*#include "../LHA_orig.cpp"
template class LHA<abstractState>;
template class LHA_orig<abstractState>;*/

#include "SKModelBase.cpp"
template class SKModel<EventsQueue>;
template class SKModel<EventsQueueSet>;
