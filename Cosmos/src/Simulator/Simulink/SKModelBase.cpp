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
 * file SKModelBase.cpp created by Yann Duplouy 2017                               *
 *******************************************************************************
 */

#include <stdio.h>

#include "SKModel.hpp"

template<class EQT>
double SKModel<EQT>::linearInterpolation(SKTime sktA, SKTime sktB, double vA, double vB, SKTime time) {
    double tA = sktA.getDouble(); double tB = sktB.getDouble(); double t = time.getDouble();
    return (vB - vA)/(tB - tA) * (t - tA) + vA;
};

template<class EQT>
SKModel<EQT>::SKModel(size_t s):shift(s) {
    Transition.push_back(SKTransition("SimulinkTransition"));
}

template<class EQT>
SKModel<EQT>::SKModel():shift(0) {
    Transition.push_back(SKTransition("SimulinkTransition"));
}

template<class EQT>
void SKModel<EQT>::initialEventsQueue(EQT& EQ, timeGen& tg) {
    Event E;
    generateEvent(0.0, E, 0, tg);
    EQ.insert(E);
}

template<class EQT>
void SKModel<EQT>::generateEvent(double ctime, Event& E,size_t Id, timeGen& tg) {
    double t = ctime;
    
    E.transition = shift+Id;
    E.time = t;
    E.priority = 1;
    E.weight = 1;
};

template<class EQT>
void SKModel<EQT>::reset() {
    Marking.resetToInitMarking();
};

template<class EQT>
const std::vector<size_t> SKModel<EQT>::getNbTransition()const {
    auto nbtrans = std::vector<size_t>();
    for( let t : Transition)nbtrans.push_back(t.bindingList.size());
    return nbtrans;
}

template<class EQT>
const std::vector<std::string> SKModel<EQT>::getTransitionLabels()const {
    auto nbtrans = std::vector<std::string>();
    for( let t : Transition)nbtrans.push_back(t.label);
    return nbtrans;
}

template<class EQT>
const std::string SKModel<EQT>::getTransitionLabel(size_t i)const {
    return Transition[i].label;
}

