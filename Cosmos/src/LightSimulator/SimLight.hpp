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
 * file SimLight.hpp created by Benoit Barbot on 5/02/2015.                    *
 *******************************************************************************
 */


#ifndef __Cosmos__SimLight__
#define __Cosmos__SimLight__

#include "spnLight.hpp"
#include "EventsQueueLight.hpp"

#ifdef CLIENT_SIM
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"
#endif


class SimulatorLight {
public:
    SimulatorLight();

    //! verbose level of the simulator
    int verbose;
        
    /**
     *  Current simulation time
     */
    REAL_TYPE curr_time;

    SPN N; //!The object representing the SPN

    /**
     * \brief Simulate single path
     * this function loop over SimulateOneStep until a the path terminate.
     */
    void SimulateSinglePath();
    void StartSimulation(void);
    void StopSimulation(void);
    
    void InitialEventsQueue(); //!initialize the event queue
    void reset(); //! reset the simulator
    
protected:
    
    //! a Temporary event
    Event F;

    //! Store result beetween two trajectory simulation.
    bool Result;
    bool simStatus;
    /**
     * \brief The event queue of the simulator.
     * The event queue is a datastructure containing the
     * enabled transitions with
     * the time at wich they will be fire if still enabled
     */
    EventsQueue EQ;

    void GenerateEvent(Event &,TR_PL_ID); //! generate a new event use Generate Time

    //! update value in the SPN after a transition
    void updateSPN(TR_PL_ID);


#ifdef CLIENT_SIM
    void initRandomGenerator(unsigned int seed);
    double GenerateTime(REAL_TYPE a, REAL_TYPE b);

    //!The random Generator Mersenne Twister from the boost library
    boost::mt19937 RandomNumber;
#endif


};


#endif /* defined(__Cosmos__SimLight__) */
