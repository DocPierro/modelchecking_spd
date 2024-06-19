/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini, Benoit Barbot & Hilal Djafri                      *
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

#include <chrono>
#include <float.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <ratio>
#include <set>
#include <time.h>
#include <unistd.h>

#include "Simulator.hpp"
#include "marking.hpp"

using namespace std;

/**
 * Constructor for the Simulator initialize the event queue
 * but don't fill it.
 */
template <class S, class EQT, class DEDS>
SimulatorBase<S, EQT, DEDS>::SimulatorBase(
    DEDS &spn, LHA_orig<typename DEDS::stateType> &automate)
    : N(spn), A(automate) {
  EQ = new EQT(N.getNbTransition()); // initialization of the event queue
  logResult = false;
  sampleTrace = 0.0;
  Result.quantR.resize(A.FormulaVal.size());
  Result.qualR.resize(A.FormulaValQual.size());
  BatchSize = 1000;
  minInteractiveTime = 0.0;
  waitForTransition = -1;
}

/*Simulator::Simulator():verbose(0)){
  EQ = new EventsQueue(N); //initialization of the event queue
  logResult=false;
  sampleTrace = 0.0;
  Result.second.resize(A.FormulaVal.size());
  BatchSize = 1000;
  minInteractiveTime = 0.0;
  }*/

template <class S, class EQT, class DEDS>
SimulatorBase<S, EQT, DEDS>::~SimulatorBase() {
  delete EQ;
}

template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::logValue(const string path) {
  logvalue.open(path, fstream::out);
  logvalue.precision(15);
  logResult = true;
}

template <class S, class EQT, class DEDS>

void SimulatorBase<S, EQT, DEDS>::logTrace(const string path, double sample) {
  if (!logtrace.is_open()) {
    sampleTrace = sample;
    logtrace.open(path, fstream::out);
    // logtrace << "# sampling at:" << sample << endl;
    logtrace.precision(15);
    logtrace << "Time      ";
    N.printHeader(logtrace);
    A.printHeader(logtrace);
    logtrace << endl;
  }
}

template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::printLog(double eTime, size_t t) {
  if (logtrace.is_open())
    if ((A.CurrentTime - lastSampled) >= sampleTrace) {
      lastSampled = A.CurrentTime;
      logtrace << setw(9) << left << setprecision(8) << A.CurrentTime << "  ";
      logtrace << right;
      N.printState(logtrace, eTime);
      A.printState(logtrace);
      if (t != string::npos)
        logtrace << " ->" << N.getTransitionLabel(t);
      logtrace << endl;
    }
}

template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::printLog(double eTime) {
  printLog(eTime, string::npos);
}

template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::SetBatchSize(const size_t RI) {
  BatchSize = RI;
}

/**
 * Reset the SPN, The LHA and the Event Queue to the initial state.
 */
template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::reset() {
  timeGen::reset();
  N.reset();
  A.reset(N.getState());
  EQ->reset();
}

/**
 * This function is called when an accepting state is reached in
 * the automaton. It update the automaton variable before updating the
 * Hasl formula.
 */
template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::returnResultTrue() {
  A.getFinalValues(N.getState(), Result.quantR, Result.qualR);
  Result.accept = true;
}

/**
 * This function update the likelihood in the rare event context.
 * Do nothing when not in rare event context.
 * @param i Number of the transition of the SPN
 */
template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::updateLikelihood(size_t) {
  return;
}

/**
 * This function return true if the transition is the sink transition.
 * always return true when not in rare event context.
 * @param i Number of the transition of the SPN
 */
template <class S, class EQT, class DEDS>
bool SimulatorBase<S, EQT, DEDS>::transitionSink(size_t) {
  return false;
}

/**
 * Simulate one step of simulation
 * @return true if the simulation did not reach an accepting are refusing state.
 */
template <class S, class EQT, class DEDS>
bool SimulatorBase<S, EQT, DEDS>::SimulateOneStep() {

  AutEdge AE = A.GetEnabled_A_Edges(N.getState());

  // If there is no enabled transition in the Petri net
  // try to reach an accepting state by using autonomous edge of
  // the automata refuse the simulation otherwise.
  if (EQ->isEmpty()) {
    while (AE.Index > -1) {
      if (P.verbose > 3) {
        cerr << "Autonomous transition with Empty Queue:";
        cerr << AE.Index << endl;
        A.printState(cerr);
        cerr << endl;
      }
      A.updateLHA(AE.FiringTime - A.CurrentTime, N.getState());
      A.fireAutonomous(AE.Index, N.getState());
      if (A.isFinal()) {
        static_cast<S *>(this)->returnResultTrue();
        return false;
      } else
        AE = A.GetEnabled_A_Edges(N.getState());
    }
    Result.accept = false;
    return false;
  } else {
    // Take the first event in the queue
    const Event &E1 = EQ->InPosition(0);

    // If this transition is the sink transition refuse the simulation
    // Only usefull for Rare Event handling.
    if (static_cast<S *>(this)->transitionSink(E1.transition)) {
      if (P.verbose > 3)
        cerr << "\033[1;33mFiring:\033[0m"
             << "Transition Sink\n";
      Result.accept = false;
      return false;
    }

    // Hook for rare event simulation
    static_cast<S *>(this)->updateLikelihood(E1.transition);

    // Take all autonomous edge in the automata before the fire time
    // of the transition of the Petri net.
    while (E1.time >= AE.FiringTime) {
      // cerr << "looping on autonomous edge";
      double eTime = AE.FiringTime - A.CurrentTime;
      A.updateLHA(eTime, N.getState());
      static_cast<S *>(this)->printLog(eTime);
      A.fireAutonomous(AE.Index, N.getState());
      if (P.verbose > 3) {
        cerr << "Autonomous transition:" << AE.Index << " " << E1.time
             << ">=" << AE.FiringTime << endl;
        A.printState(cerr);
        cerr << endl;
      }
      static_cast<S *>(this)->printLog(eTime);
      if (A.isFinal()) {
        static_cast<S *>(this)->returnResultTrue();
        return false;
      } else
        AE = A.GetEnabled_A_Edges(N.getState());
    }
    if (P.verbose > 3) {
      cerr << "\033[1;33mFiring:\033[0m" << N.getTransitionLabel(E1.transition);
      E1.binding.print();
    }

    // Make time elapse in the LHA
    double eTime = E1.time - A.CurrentTime;
    A.updateLHA(eTime, N.getState());
    if (eTime > 0.0) {
      current_weight = 0.0;
    } else {
      current_weight = E1.weight;
    }
    // Print the state of the system after the time elapse and the transition
    // name
    static_cast<S *>(this)->printLog(eTime, E1.transition);

    // Fire the transition in the SPN
    N.fire(E1.transition, E1.binding, A.CurrentTime);

    if (E1.transition >= A.NbTrans) {
      if (P.verbose > 3)
        cerr << " Transition filtered " << endl;
      N.update(A.CurrentTime, E1.transition, E1.binding, *EQ, *this);
      return true;
    }

    // Check if there exist a valid Synchronisation in the automata and fire it.
    int SE = A.synchroniseWith(E1.transition, N.getState(), E1.binding);

    // If no synchronisation is possible the trajectory is rejected
    if (SE < 0) {
      if (P.verbose > 3)
        cerr << " No synchronisation" << endl;
      Result.accept = false;
      return false;
    } else {
      if (P.verbose > 3)
        cerr << " Synchronisation with " << SE << endl;
      // If synchronisation is possible check if the
      //  reached state is final. Then update the SPN.
      if (A.isFinal()) {
        static_cast<S *>(this)->returnResultTrue();
        return false;
      } else {
        N.update(A.CurrentTime, E1.transition, E1.binding, *EQ, *this);
      }
    }
  }
  return true;
}

/**
 * Interactive mode stop the simulation until the user choose a transition.
 */
template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::interactiveSimulation() {
  string input_line;
  if ((waitForTransition > 0 && (size_t)waitForTransition != N.lastTransition))
    return;
  if (A.CurrentTime < minInteractiveTime &&
      (waitForTransition < 0 || (size_t)waitForTransition != N.lastTransition))
    return;
  bool continueLoop = true;
  waitForTransition = -1;
  while (continueLoop) {
    cerr << "\033[1;31mCosmosSimulator>\033[0m";
    auto trLabels = N.getTransitionLabels();
    if (cin.good()) {
      getline(cin, input_line);
      if (input_line.substr(0, 5).compare("fire ") == 0) {
        string trans = input_line.substr(5, input_line.size() - 5);
        size_t traid;
        for (traid = 0; traid < trLabels.size() && trLabels[traid] != trans;
             traid++)
          ;
        if (traid == trLabels.size())
          cerr << "Unknown transition: " << trans << endl;
        else {
          if (EQ->isScheduled(traid, 0)) {
            Event &F = EQ->getEvent(traid, 0);
            F.setTime(A.CurrentTime);
            F.setPriority((size_t)-1);
            F.setWeight(0.0);
            EQ->replace(F);
            continueLoop = false;
          } else
            cerr << "Transition: " << trans << " is not enabled" << endl;
        }

      } else if (input_line.compare("step") == 0)
        continueLoop = false;
      else if (input_line.compare("stop") == 0)
        exit(EXIT_SUCCESS);
      else if (input_line == "display" || input_line == "d") {
        if (!dotFile.empty()) {
          stringstream ss;
          ss << "sed ";
          N.printSedCmd(ss);
          EQ->printSedCmd(N.getTransitionLabels(), ss);
          ss << P.tmpPath << "/templatePetriNet.dot > " << P.tmpPath
             << "/PetriNet.dot; ";
          ss << "dot " << P.tmpPath << "/PetriNet.dot -Tpdf -o " << dotFile
             << endl;
          const auto retval = system(ss.str().c_str());
          if (retval > 0)
            cerr << "Fail to lauch graphviz with error code" << retval;
        } else
          cerr << "No dot output specified!" << endl;
      } else if (input_line.substr(0, 5) == "wait ") {
        const auto arg = input_line.substr(5, input_line.length() - 5);
        const auto transitionsLabels = N.getTransitionLabels();
        const auto trid =
            find_if(transitionsLabels.begin(), transitionsLabels.end(),
                    [&](string tr) { return (tr == arg); });
        if (trid != transitionsLabels.end()) {
          waitForTransition = trid - transitionsLabels.begin();
          continueLoop = false;
        } else
          try {
            minInteractiveTime = A.CurrentTime + stod(arg);
            continueLoop = false;
          } catch (const invalid_argument &ia) {
            cerr << "Fail to parse time!" << endl;
          } catch (const out_of_range &ia) {
            cerr << "Fail to parse time!" << endl;
          }

      } else if (input_line.compare("exit") == 0) {
        exit(EXIT_SUCCESS);
      } else if (input_line.compare("help") == 0 ||
                 input_line.compare("h") == 0) {
        cerr << "Available command:\n\thelp:\tdisplay this message" << endl;
        cerr << "\ts, step:\tmake one step of simulation" << endl;
        cerr << "\td, draw:\tdraw the GSPN with dot" << endl;
        cerr << "\tfire tr:\tfire transition tr" << endl;
        cerr << "\twait tr:\twait until transition tr occurs" << endl;
        cerr << "\twait t:\twait t times unit" << endl;
        cerr << "\texit t:\texit simulator" << endl;

      } else if (input_line.compare("s") == 0 || input_line.compare("s") == 0)
        continueLoop = false;
      else if (input_line.compare("") == 0)
        ;
      else {
        cerr << "Command not found:" << input_line << endl;
      }

    } else {
      if (cin.eof())
        exit(EXIT_SUCCESS);
      cerr << "error on the input stream\n";
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * Simulate a whole trajectory in the system. Result is store in SimOutput
 */
template <class S, class EQT, class DEDS>
void SimulatorBase<S, EQT, DEDS>::SimulateSinglePath() {

  static_cast<S *>(this)->reset();
  N.initialEventsQueue(*EQ, *this);
  minInteractiveTime = 0.0;
  waitForTransition = -1;

  if (logtrace.is_open())
    logtrace << "New Path" << endl;
  // cerr << "start path"<< endl;

  bool continueb = true;
  lastSampled = -sampleTrace;
  while (continueb) {
    // cerr << "continue path"<< endl;
    static_cast<S *>(this)->printLog(0.0);
    if (P.verbose > 3) {
      // Print marking and location of the automata
      // Usefull to track a simulation
      N.printHeader(cerr);
      A.printHeader(cerr);
      cerr << endl;
      N.printState(cerr, 0.0);
      A.printState(cerr);
      cerr << endl;
      if (P.verbose > 4)
        EQ->view(N.getTransitionLabels());
      if (P.verbose == 6)
        static_cast<S *>(this)->interactiveSimulation();
    }

    continueb = static_cast<S *>(this)->SimulateOneStep();
  }
  if (P.verbose > 3) {
    // Print marking and location of the automata
    // Usefull to track a simulation
    N.printHeader(cerr);
    A.printHeader(cerr);
    cerr << endl;
    N.printState(cerr, 0.0);
    A.printState(cerr);
    cerr << endl;
    if (P.verbose > 4)
      EQ->view(N.getTransitionLabels());
  }
  // cerr << "finish path"<< endl;
}

template <class S, class EQT, class DEDS>
BatchR SimulatorBase<S, EQT, DEDS>::RunBatch() {
  auto starttime = chrono::steady_clock::now();
  auto currenttime = chrono::steady_clock::now();
  chrono::duration<double> timesize(0.03);
  BatchR batchResult(A.FormulaVal.size(), A.FormulaValQual.size());
  while ((batchResult.I < BatchSize && BatchSize != 0) ||
         (currenttime - starttime < timesize && BatchSize == 0)) {
    static_cast<S *>(this)->SimulateSinglePath();
    batchResult.addSim(Result);
    if (P.verbose > 3)
      cerr << batchResult;

    if (Result.accept && logResult) {
      for (size_t i = 0; i < Result.quantR.size(); i++) {
        if (i > 0)
          logvalue << "\t";
        logvalue << Result.quantR[i];
      }
      logvalue << endl;
    }
    currenttime = chrono::steady_clock::now();
  }
  batchResult.simTime = (currenttime - starttime).count();
  return batchResult;
}
