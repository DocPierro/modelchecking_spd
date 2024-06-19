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
 * file LHA_orig.hpp created by Benoit Barbot on 026/08/15. *
 *******************************************************************************
 */

#ifndef Cosmos_LHA_orig_h
#define Cosmos_LHA_orig_h

#include "LHA.hpp"

/**
 * Class implementing the Linear Hybrid Automaton.
 * Part of the implementation is generated at runtime for efficiency.
 */
template <class DEDState> class LHA_orig : public LHA<DEDState> {
public:
  /*
   * Curent likelihood.
   * Use only for importance sampling
   */
  double Likelihood;

  /**
   *  Copy the state of an other LHA.
   *  It only copy pointer thus is in constant time.
   */
  void copyState(LHA_orig *);

  //! fire an autonomous transition of an LHA
  void fireAutonomous(int, const DEDState &);

  /**
   * \brief Synchronized the execution of the LHA with a transition of the SPN.
   */
  virtual int synchroniseWith(size_t, const DEDState &,
                              const abstractBinding &);

  /**
   * \brief Return an autonomous edge for a given marking.
   */
  virtual AutEdge GetEnabled_A_Edges(const DEDState &);

  //! update value in the LHA by elapsing time
  virtual void updateLHA(double DeltaT, const DEDState &);

  //! test if the automaton is in a final state
  virtual bool isFinal() const {
    return (this->FinalLoc[this->CurrentLocation]);
  };

  /**
   * reset the automata to its initial state according to the
   * marking of the SPN.
   */
  virtual void reset(const DEDState &);

  virtual void getFinalValues(const DEDState &, vector<double> &,
                              vector<bool> &);

protected:
  //! fire the transition of an LHA
  void fireLHA(int, const DEDState &, const abstractBinding &);

  /**
   * \brief Set the initial location of the LHA for a marking
   * Loop over the set of initial location to find one enabled.
   */
  virtual void setInitLocation(const DEDState &);

  /**
   * \brief Return a synchronized edge for a given transition of the SPN.
   */
  int GetEnabled_S_Edges(size_t, const DEDState &, const abstractBinding &);

  void resetLinForms();
};

#endif
