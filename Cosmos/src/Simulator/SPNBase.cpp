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
 * file spn_orig.cpp created by Benoit Barbot on 03/09/15.                     *
 *******************************************************************************
 */

#include "SPNBase.hpp"
#include "timeGenBis.hpp"

using namespace std;

template <class S, class EQT>
const vector<size_t> SPNBase<S, EQT>::getNbTransition() const {
  auto nbtrans = vector<size_t>();
  for (let t : Transition)
    nbtrans.push_back(t.bindingList.size());
  return nbtrans;
}

template <class S, class EQT>
const vector<string> SPNBase<S, EQT>::getTransitionLabels() const {
  auto nbtrans = vector<string>();
  for (let t : Transition)
    nbtrans.push_back(t.label);
  return nbtrans;
}

template <class S, class EQT>
const string SPNBase<S, EQT>::getTransitionLabel(size_t i) const {
  return Transition[i].label;
}

/**
 * Fill the event queue with the initially enabled transition
 */
template <class S, class EQT>
void SPNBase<S, EQT>::initialEventsQueue(EventsQueue &EQ, timeGen &TG) {
  // Check each transition. If a transition is enabled then his fire
  // time is simulated and added to the structure.

  Event E;

  for (const auto &t : this->Transition) {
    if (P.verbose > 5) {
      std::cerr << "Initial Event for transition " << t.Id << "("
                << shift + t.Id << "," << t.label << ") : \n";
    }
    for (auto &bindex : t.bindingList) {
      if (P.verbose > 5) {
        std::cerr << "Transition " << t.label << ": ";
        bindex.print();
        std::cerr << "\n";
      }
      if (IsEnabled(t.Id, bindex)) {
        generateEvent(0.0, E, t.Id, bindex, TG, *(static_cast<S *>(this)));
        EQ.insert(E);
      }
    }
  }
}

template <class S, class EQT>
void SPNBase<S, EQT>::initialEventsQueue(EventsQueueSet &EQ, timeGen &TG) {
  // Check each transition. If a transition is enabled then his fire
  // time is simulated and added to the structure.

  Event E;
  abstractBindingIterator absMkIt(Marking);

  for (const auto &t : Transition) {
    if (P.verbose > 5) {
      std::cerr << "Initial Event for transition " << t.Id << "("
                << shift + t.Id << "," << t.label << ") : \n";
    }
    size_t it = t.Id;

    /* const auto &it = (const size_t) t.Id; */
    for (absMkIt.reset(it, Marking); absMkIt.isValid();
         absMkIt.next(it, Marking)) {
      const auto &bindex = absMkIt.getBinding();
      if (false && P.verbose > 5) {
        std::cerr << "Transition " << t.label << ": ";
        bindex.print();
        std::cerr << "\n";
      }
      if (IsEnabled(t.Id, bindex)) {
        generateEvent(0.0, E, t.Id, bindex, TG, *(static_cast<S *>(this)));
        EQ.insert(E);
      }
    }
  }
}

/**
 * Update the enabling transition of the SPN, and update the event queue.
 * @param E1_transitionNum the number of the transition which last
 * occured in the SPN.
 * @param b is the binding of the last transition.
 */
template <class S, class EQT>
void SPNBase<S, EQT>::update(double ctime, size_t tr, const abstractBinding &lb,
                             EventsQueue &EQ, timeGen &TG) {
  // This function update the Petri net according to a transition.
  // In particular it update the set of enabled transition.

  // check if the current transition is still enabled
  if (Transition[tr].DistTypeIndex != SYNC) {
    for (const auto &bindex : Transition[tr].bindingList) {
      bool Nenabled = IsEnabled(tr, bindex);
      bool NScheduled = EQ.isScheduled(shift + tr, bindex.idcount);

      if (Nenabled && NScheduled && lb.idcount == bindex.idcount) {
        generateEvent(ctime, F, tr, bindex, TG, *(static_cast<S *>(this)));
        EQ.replace(F); // replace the transition with the new generated time
      } else if (Nenabled && !NScheduled) {
        generateEvent(ctime, F, tr, bindex, TG, *(static_cast<S *>(this)));
        EQ.insert(F);
      } else if (!Nenabled && NScheduled) {
        EQ.remove(shift + tr, bindex.idcount);
      }
    }
  } else {
    // Do not remove it it is not in the queue anyway
    EQ.remove(shift + tr, lb.idcount);
  }

  // Possibly adding Events corresponding to newly enabled-transitions
  // const auto &net = PossiblyEn();
  for (size_t t = 0; PossiblyEnabled[lastTransition][t] != -1; t++) {
    const auto &it = PossiblyEnabled[lastTransition][t];
    size_t bindnum = 0;
    const abstractBinding *bindex =
        nextPossiblyEnabledBinding(it, lb, &bindnum);
    while (bindex != NULL) {
      if (false && P.verbose > 4) {
        std::cerr << "consider for enabling: " << Transition[it].label << ",";
        bindex->print();
        cerr << endl;
      }

      // for(vector<abstractBinding>::const_iterator bindex =
      // Transition[*it].bindingList.begin() ; 	bindex !=
      //Transition[*it].bindingList.end() ; ++bindex){
      if (IsEnabled(it, *bindex)) {
        if (!EQ.isScheduled(shift + it, bindex->idcount)) {
          if (P.verbose > 4) {
            cerr << "->New transition enabled: " << Transition[it].label << ",";
            bindex->print();
            cerr << endl;
          }
          if (!EQ.restart(ctime, shift + it, bindex->idcount)) {
            generateEvent(ctime, F, it, *bindex, TG, *(static_cast<S *>(this)));
            EQ.insert(F);
          }

        } else {
          if (Transition[it].MarkingDependent) {
            generateEvent(ctime, F, it, *bindex, TG, *(static_cast<S *>(this)));
            EQ.replace(F);
          }
        }
      }
      bindex = nextPossiblyEnabledBinding(it, lb, &bindnum);
    }
  }

  // Possibly removing Events corresponding to newly disabled-transitions
  // const auto &ndt = PossiblyDis();
  // for (const auto &it : ndt) {
  for (size_t t = 0; PossiblyDisabled[lastTransition][t] != -1; t++) {
    const auto &it = PossiblyDisabled[lastTransition][t];
    size_t bindnum = 0;
    const abstractBinding *bindex =
        nextPossiblyDisabledBinding(it, lb, &bindnum);
    while (bindex != NULL) {
      if (false && P.verbose > 4) {
        cerr << "consider for disabling: " << Transition[it].label << ",";
        bindex->print();
        cerr << endl;
      }

      if (Transition[it].DistTypeIndex == SYNC) {
        if (P.verbose > 4) {
          cerr << "Transition is a SYNC transition. Consider for enabling "
                  "instead."
               << endl;
        }
        if (IsEnabled(it, *bindex)) {
          if (!EQ.isScheduled(shift + it, bindex->idcount)) {
            if (P.verbose > 4) {
              cerr << "->New transition enabled: " << Transition[it].label
                   << ",";
              bindex->print();
              cerr << endl;
            }
          }
          if (!EQ.restart(ctime, shift + it, bindex->idcount)) {
            generateEvent(ctime, F, it, *bindex, TG, *(static_cast<S *>(this)));
            EQ.insert(F);
          }
        }
      } else {

        // for(vector<abstractBinding>::const_iterator bindex =
        // Transition[*it].bindingList.begin() ; 	bindex !=
        //Transition[*it].bindingList.end() ; ++bindex){
        if (EQ.isScheduled(shift + it, bindex->idcount)) {
          if (!IsEnabled(it, *bindex)) {
            if (P.verbose > 4) {
              cerr << "<-New transition disabled: " << Transition[it].label
                   << ",";
              bindex->print();
              cerr << endl;
            }
            if (Transition[it].AgeMemory) {
              EQ.pause(ctime, shift + it, bindex->idcount);
            } else
              EQ.remove(shift + it, bindex->idcount);
          } else {
            if (Transition[it].MarkingDependent) {
              generateEvent(ctime, F, shift + it, *bindex, TG,
                            *(static_cast<S *>(this)));
              EQ.replace(F);
            }
          }
        }
      }
      bindex = nextPossiblyDisabledBinding(it, lb, &bindnum);
    }
  }

  // Update transition which have no precondition on the Marking
  for (size_t t = 0; FreeMarkDepT[lastTransition][t] != -1; t++) {
    const auto &it = FreeMarkDepT[lastTransition][t];
    // const auto &fmd = FreeMarkingDependant();
    // for (const auto &it : fmd) {
    for (const auto &bindex : Transition[it].bindingList) {
      // if (IsEnabled(it,bindex)) {
      if (EQ.isScheduled(shift + it, bindex.idcount)) {
        generateEvent(ctime, F, shift + it, bindex, TG,
                      *(static_cast<S *>(this)));
        EQ.replace(F);
      }
      //}
    }
  }
  // assert(cerr<< "assert!"<< endl);

  /*
   //In Debug mode check that transition are scheduled iff they are enabled
   for (const auto &t : Transition){
   for(const auto &bindex : t.bindingList){
   if (IsEnabled(t.Id, bindex) !=
   EQ.isScheduled(t.Id, bindex.idcount)){
   cerr << "IsEnabled(" << t.label << ",";
   bindex.print();
   cerr <<")" << endl;
   if(EQ.isScheduled(t.Id, bindex.idcount)){
   cerr << "Scheduled and not enabled!"<< endl;
   }else{
   cerr << "Enabled and not scheduled!" << endl;
   }
   assert(IsEnabled(t.Id, bindex) ==
   EQ.isScheduled(t.Id, bindex.idcount));
   }
   }
   }
   */
}

template <class S, class EQT>
void SPNBase<S, EQT>::cleanNewlyDisabled(size_t tr, EventsQueueSet &EQ) {
  const auto &bset = EQ.getScheduledBinding(tr);
  for (auto it = bset.begin(); it != bset.end();) {
    const auto &binding = *it;
    it++;
    Event &e = EQ.getEvent(tr, binding.first);
    bool stillEnabled = IsEnabled(tr, e.binding);
    if (!stillEnabled) {
      if (false && P.verbose > 4) {
        std::cerr << "Removing :" << Transition[tr].label << " : ";
        e.binding.print();
        std::cerr << endl;
      }
      EQ.remove(shift + tr, binding.first);
    }
  }
}

template <class S, class EQT>
void SPNBase<S, EQT>::update(double ctime, size_t tr, const abstractBinding &lb,
                             EventsQueueSet &EQ, timeGen &TG) {
  // This function update the Petri net according to a transition.
  // In particular it update the set of enabled transition.

  // check if the current transition is still enabled
  abstractBindingIterator absMkIt(Marking);

  cleanNewlyDisabled(tr, EQ);

  // for(const auto &bindex : Transition[E1_transitionNum].bindingList){
  if (Transition[tr].DistTypeIndex != SYNC) {
    absMkIt.reset(tr, Marking);
    for (; absMkIt.isValid(); absMkIt.next(tr, Marking)) {
      const auto &bindex = absMkIt.getBinding();
      // bindex = Transition[E1_transitionNum].bindingList[tmpbindex];
      // bindex type abstractBinding

      if (P.verbose > 4) {
        std::cerr << "Transition :" << Transition[tr].label << "\tbinding:";
        bindex.print();
        std::cerr << endl;
      }

      bool Nenabled = IsEnabled(tr, bindex);
      bool NScheduled = EQ.isScheduled(shift + tr, bindex.idcount);
      if (P.verbose > 4) {
        std::cerr << "Current status: IsEnabled ? " << Nenabled
                  << ". Is Scheduled ? " << NScheduled << ".\n";
      }

      if (Nenabled && NScheduled && lb.idcount == bindex.idcount) {
        generateEvent(ctime, F, tr, bindex, TG, *(static_cast<S *>(this)));
        EQ.replace(F); // replace the transition with the new generated time
      } else if (Nenabled && !NScheduled) {
        generateEvent(ctime, F, tr, bindex, TG, *(static_cast<S *>(this)));
        EQ.insert(F);
      } else if (!Nenabled && NScheduled) {
        EQ.remove(shift + tr, bindex.idcount);
      }
    }
  } else {
    // Do not remove it, it is not in the queue anyway
    EQ.remove(shift + tr, lb.idcount);
  }
  // absMkIt.reset();

  // Possibly adding Events corresponding to newly enabled-transitions
  // const auto &net = PossiblyEn();
  for (size_t t = 0; PossiblyEnabled[lastTransition][t] != -1; t++) {
    size_t it = (size_t)PossiblyEnabled[lastTransition][t];

    if (P.verbose > 4)
      std::cerr << "consider for enabling: " << Transition[it].label << ",";
    for (absMkIt.reset(it, Marking); absMkIt.isValid();
         absMkIt.next(it, Marking)) {
      const auto &bindex = absMkIt.getBinding();

      if (P.verbose > 4) {
        // std::cerr << "consider for enabling: " << Transition[it].label <<
        // ",";
        bindex.print();
        cerr << endl;
      }

      // for(vector<abstractBinding>::const_iterator bindex =
      // Transition[*it].bindingList.begin() ; 	bindex !=
      //Transition[*it].bindingList.end() ; ++bindex){
      if (IsEnabled(it, bindex)) {
        if (!EQ.isScheduled(shift + it, bindex.idcount)) {
          if (P.verbose > 4) {
            cerr << "->New transition enabled: " << Transition[it].label << ",";
            bindex.print();
            cerr << endl;
          }
          if (!EQ.restart(ctime, shift + it, bindex.idcount)) {
            generateEvent(ctime, F, it, bindex, TG, *(static_cast<S *>(this)));
            EQ.insert(F);
          }

        } else {
          if (Transition[it].MarkingDependent) {
            generateEvent(ctime, F, it, bindex, TG, *(static_cast<S *>(this)));
            EQ.replace(F);
          }
        }
      }
    }
  }

  // Possibly removing Events corresponding to newly disabled-transitions
  // const auto &ndt = PossiblyDis();
  // for (const auto &it : ndt) {
  for (size_t t = 0; PossiblyDisabled[lastTransition][t] != -1; t++) {
    // const auto &it = PossiblyDisabled[lastTransition][t];
    size_t it = (size_t)PossiblyDisabled[lastTransition][t];
    cleanNewlyDisabled(it, EQ);
  }

  // Update transition which have no precondition on the Marking
  for (size_t t = 0; FreeMarkDepT[lastTransition][t] != -1; t++) {
    // const auto &it = FreeMarkDepT[lastTransition][t];
    size_t it = (size_t)FreeMarkDepT[lastTransition][t];
    // const auto &fmd = FreeMarkingDependant();
    // for (const auto &it : fmd) {

    for (absMkIt.reset(it, Marking); absMkIt.isValid();
         absMkIt.next(it, Marking)) {
      const auto &bindex = absMkIt.getBinding();
      // if (IsEnabled(it,bindex)) {
      if (EQ.isScheduled(it, bindex.idcount)) {
        generateEvent(ctime, F, it, bindex, TG, *(static_cast<S *>(this)));
        EQ.replace(F);
      }
      //}
    }
  }
  // assert(cerr<< "assert!"<< endl);

  /*
   //In Debug mode check that transition are scheduled iff they are enabled
   for (const auto &t : Transition){
   for(const auto &bindex : t.bindingList){
   if (IsEnabled(t.Id, bindex) !=
   EQ.isScheduled(t.Id, bindex.idcount)){
   cerr << "IsEnabled(" << t.label << ",";
   bindex.print();
   cerr <<")" << endl;
   if(EQ.isScheduled(t.Id, bindex.idcount)){
   cerr << "Scheduled and not enabled!"<< endl;
   }else{
   cerr << "Enabled and not scheduled!" << endl;
   }
   assert(IsEnabled(t.Id, bindex) ==
   EQ.isScheduled(t.Id, bindex.idcount));
   }
   }
   }
   */
}
