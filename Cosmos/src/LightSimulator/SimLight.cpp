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
 * file SimLight.cpp created by Benoit Barbot on 5/02/2015.                    *
 *******************************************************************************
 */

#include "SimLight.hpp"

/**
 * Constructor for the Simulator initialize the event queue
 * but don't fill it.
 */
SimulatorLight::SimulatorLight():verbose(0),curr_time(0.0),EQ(N){
    Result=true;
    simStatus = false;
#ifdef CLIENT_SIM
    initRandomGenerator(0);/////////////  Change seed     //////////////////////////

#endif

}

/**
 * Fill the event queue with the initially enabled transition
 */
void SimulatorLight::InitialEventsQueue() {
    //Check each transition. If a transition is enabled then his fire
    //time is simulated and added to the structure.
    
    Event E;
    for(TR_PL_ID t=0; t<N.tr ; t++)
        if (N.IsEnabled(t)){
            GenerateEvent(E, t);
            EQ.insert(E);
        }
    
#ifndef NO_STRING_SIM
    //Print marking and location of the automata
    //Usefull to track a simulation
    if (verbose>1) {
        print("Initial Event Queue");
        print("Time\t");
        N.Marking.printHeader();
        print("\n");
        print(curr_time);
        print("\t");
        N.Marking.print();
        print("\n");
        if(verbose>2)EQ.view();
        print("\n");
    }
    
#endif
}

/**
 * Reset the SPN, The LHA and the Event Queue to the initial state.
 */
void SimulatorLight::reset() {
    curr_time=0.0;
    N.reset();
    EQ.reset();
}

void SimulatorLight::StartSimulation(void) {
    simStatus = true;
}

void SimulatorLight::StopSimulation(void) {
    simStatus = false;
}

/**
 * Update the enabling transition of the SPN, and update the event queue.
 * @param E1_transitionNum the number of the transition which last
 * occured in the SPN.
 * @param b is the binding of the last transition.
 */
void SimulatorLight::updateSPN(TR_PL_ID E1_transitionNum){
    //This function update the Petri net according to a transition.
    //In particular it update the set of enabled transition.
    
    //if (E1_transitionNum != UNSET_TRANS ){
        //check if the current transition is still enabled
        bool Nenabled = N.IsEnabled(E1_transitionNum);
        bool NScheduled = EQ.isScheduled(E1_transitionNum);

        if (Nenabled && NScheduled) {
            GenerateEvent(F, E1_transitionNum);
            EQ.replace(F); //replace the transition with the new generated time
        } else if (Nenabled && !NScheduled) {
            GenerateEvent(F, E1_transitionNum);
            EQ.insert(F);
        } else if (!Nenabled && NScheduled) {
            EQ.remove(E1_transitionNum);
        }
    //}

#ifdef FAST_SIM
    // Possibly adding Events corresponding to newly enabled-transitions
    //const auto &net = N.PossiblyEn();
    for (TR_PL_ID t=0; N.PossiblyEnabled[N.lastTransition][t] != UNSET_TRANS ;t++) {
        const TR_PL_ID it = N.PossiblyEnabled[N.lastTransition][t];
#ifndef NO_STRING_SIM
        if(verbose > 4){
            print("enabled ?: ");
            print(it);
            print("\n");
        }
#endif
        //for(vector<abstractBinding>::const_iterator bindex = N.Transition[*it].bindingList.begin() ;
        //	bindex != N.Transition[*it].bindingList.end() ; ++bindex){
        if (N.IsEnabled(it)) {
#ifndef NO_STRING_SIM
            if(verbose > 4){
                print("-> enabled -> is schedule? \n");
            }
#endif
            if (!EQ.isScheduled(it)) {
#ifndef NO_STRING_SIM
                if(verbose > 4){
                    print("-> yes \n");
                }
#endif
                //if(!EQ.restart(curr_time,it)){
                GenerateEvent(F, (it));
                EQ.insert(F);
                //}

            }
        }

    }

    // Possibly removing Events corresponding to newly disabled-transitions
    //const auto &ndt = N.PossiblyDis();
    //for (const auto &it : ndt) {
    for (TR_PL_ID t=0; N.PossiblyDisabled[N.lastTransition][t] != UNSET_TRANS;t++) {
        const TR_PL_ID it = N.PossiblyDisabled[N.lastTransition][t];
#ifndef NO_STRING_SIM
        if(verbose > 4){
            print("disabled ?: ");
            print(it);
            print("\n");
        }
#endif
        //for(vector<abstractBinding>::const_iterator bindex = N.Transition[*it].bindingList.begin() ;
        //	bindex != N.Transition[*it].bindingList.end() ; ++bindex){
        if (EQ.isScheduled(it)) {
            if (!N.IsEnabled(it )){
#ifndef NO_STRING_SIM
                if(verbose > 4){
                    print("-> yes \n ");
                }
#endif
                EQ.remove(it);
            }
        }
    }
#endif

    /*
     // Update transition which have no precondition on the Marking
     for (size_t t=0; N.FreeMarkDepT[N.lastTransition][t]!= -1;t++) {
     const auto &it = N.FreeMarkDepT[N.lastTransition][t];
     //const auto &fmd = N.FreeMarkingDependant();
     //for (const auto &it : fmd) {
     //if (N.IsEnabled(it,bindex)) {
     if (EQ.isScheduled(it)) {
     GenerateEvent(F, it);
     (*EQ).replace(F);
     }

     }
     //assert(cerr<< "assert!"<< endl);
     */

    //In Debug mode check that transition are scheduled iff they are enabled

#ifndef FAST_SIM
    //if(E1_transitionNum== UNSET_TRANS)
    for(TR_PL_ID t=0; t<N.tr ; t++) {
        if (N.IsEnabled(t)){
            if (!EQ.isScheduled(t)) {
                GenerateEvent(F, (t));
                EQ.insert(F);
            }
        } else {
            if (EQ.isScheduled(t)) {
                EQ.remove(t);
            }
        }
    }
#endif
    
}

/**
 * Simulate a whole trajectory in the system. Result is store in SimOutput
 */
void SimulatorLight::SimulateSinglePath() {
    reset();
    InitialEventsQueue();
    
    while (simStatus) {
        
        dW(4, 1);
        dW(7, 0);
        
        // Just added now
        //ClearMarkers();
        
        if (EQ.isEmpty())break; //No event should not happen
        
        //Take the first event in the queue
        const Event &E1 = EQ.InPosition(0);
        
        //need to wait
        if(E1.time > curr_time){
            
            wait(E1.time - cRealTime());

            curr_time = cRealTime();
            unsigned char data = InDataAvailable();
            if (data == 2) {
                break;
            }else{
                if(data == 1 ){
                    TR_PL_ID trs = N.getIncomingTrans();
                    if (trs != UNSET_TRANS) {
                        N.fire(trs, curr_time); // Fire the special transition for incoming message                        
                        updateSPN(trs);
                    }
                }
            }
        } else {
            
            if (InDataAvailable() == 2) break;

#ifndef NO_STRING_SIM
            if(verbose > 2){
                print("Firing:");
                print(E1.transition);
                print("\n");
            }
#endif

            curr_time = E1.time;

            //Fire the transition in the SPN
            N.fire(E1.transition, curr_time);
            
            updateSPN(E1.transition);
        }

#ifndef NO_STRING_SIM
        //Print marking and location of the automata
        //Usefull to track a simulation
        if (verbose>1) {
            print("Time\t");
            N.Marking.printHeader();
            print("\n");
            print(curr_time);
            print("\t");
            N.Marking.print();
            print("\n");
            if(verbose>2)EQ.view();
            print("\n");
        }

#endif
        //if(curr_time >= 10000)break;
    }
}


#ifdef CLIENT_SIM
void SimulatorLight::initRandomGenerator(unsigned int seed){
    RandomNumber.seed(seed);
}


/**
 
 * Call the random generator to generate fire time.
 * @param distribution is the type of distribution
 * @param param is a vector of parameters of the distribution.
 */
double SimulatorLight::GenerateTime(REAL_TYPE a, REAL_TYPE b) {
    if(a == b) return a;
    boost::uniform_real<> UNIF(a, b);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > gen(RandomNumber, UNIF);
    return gen();
}

#endif

/**
 * Generate an event based on the type of his distribution
 * @param E the event to update
 * @param Id the number of the transition to of the SPN
 * @param b is the binding of the variable of the SPN for the transition.
 */
void SimulatorLight::GenerateEvent(Event& E,TR_PL_ID Id) {
    double t = curr_time;
    N.GetDistParameters(Id);

#ifdef CLIENT_SIM
    if(isProb(Id)){
        t += GenerateTime(N.ParamDistr[0]-UNIF_WIDTH,N.ParamDistr[0]+UNIF_WIDTH);
    } else {
        t += N.ParamDistr[0];
    }
#else
    t += N.ParamDistr[0];
#endif

#ifndef NO_STRING_SIM
    if(verbose > 3){
        print("Sample:");
        print(Id);
        print(" -> ");
        print((REAL_TYPE)N.ParamDistr[0]);
        print("\n");
    }
#endif
    E.transition = Id;
    E.time = (REAL_TYPE)t;
}
