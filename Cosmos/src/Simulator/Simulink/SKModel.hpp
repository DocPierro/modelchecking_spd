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
 * file SKModel.hpp created by Yann Duplouy 2017                               *
 *******************************************************************************
 */

#ifndef SKModel_h
#define SKModel_h

#include <string>
#include <array>
#include <vector>

#include "DistributionDef.hpp"
#include "SKTime.hpp"


class stateImpl;

class abstractState {
public:
    /**
     * Pointer to to the actual marking inmplementation
     * which is generated.
     */
    stateImpl* P;
    
    abstractState();
    abstractState(const std::vector<int>& m);
    abstractState(const abstractState& m);
    abstractState& operator = (const abstractState& m);
    ~abstractState();
    
    //! Swap marking in constant time
    void swap(abstractState& m);
    void printHeader(std::ostream &)const;
    void print(std::ostream &, double eTime)const;
    void printSedCmd(std::ostream &)const;
    void resetToInitMarking();
    int getNbOfTokens(int)const;
    std::vector<int> getVector()const;
    void setVector(const std::vector<int>&);
    
    void Symmetrize();
    
    
    
    template <typename T, typename C>
    std::vector<size_t> getPerm(const std::vector<T> v, C c)const{
        std::vector<size_t> index( static_cast<int>(c), 0);
        for (size_t i = 0 ; i != index.size() ; i++)index[i] = i;
        sort(index.begin(), index.end(),
             [&](const int& a, const int& b) {
                 return greaterIndex(v, c, a, b);
             });
        return index;
    }
    
    
    template <typename T, typename C>
    void applyPerm(const std::vector<T *> v, C c,const std::vector<size_t> id){
        for( const auto &d : v)d->apply_perm(c,id);
    }
    
    
    /*
     template <typename T, typename C>
     std::vector<size_t> getPerm(const std::vector<T> v, C c) const;
     template <typename T, typename C>
     void applyPerm(const std::vector<T *> v, C c,const std::vector<size_t>);*/
};


class SKTransition {
public:
    const size_t Id;
    const std::string label;
    const DistributionType DistTypeIndex;
    
    std::array<abstractState, 1> bindingList;
    
    SKTransition(const std::string& l):Id(0),label(l),DistTypeIndex(DETERMINISTIC){};
};

#include "timeGen.hpp"

template <class EQT>
class SKModel {
public:
    const size_t shift;
    using stateType = abstractState;

    stateType Marking;
    std::vector<SKTransition> Transition;
    
    size_t lastTransition;
    size_t lastTransitionTime;
    
    SKModel(size_t shift);
    SKModel();
    
    void reset();
    const stateType& getState()const {return Marking;};
    void printHeader(std::ostream &os)const{ Marking.printHeader(os); }
    void printSedCmd(std::ostream &os)const{ Marking.printSedCmd(os); }
    void printState(std::ostream &os, double eTime)const{ Marking.print(os,eTime); }
    const std::vector<size_t> getNbTransition()const;
    const std::vector<std::string> getTransitionLabels()const;
    const std::string getTransitionLabel(size_t i)const;
    
    void setInput(size_t,double);
    void setInput(std::string,double);
    void initialEventsQueue(EQT&, timeGen&);
    void generateEvent(double ctime,Event& E,size_t Id,timeGen& TG);
    void fire(size_t, const abstractBinding&, double);
    void update(double, size_t, const abstractBinding&, EQT&, timeGen&);
    void updateInput(double, size_t, const abstractBinding&, EQT&, timeGen&);
    

private:
    Event E;
    void initialiseIntegrators(int);
    SKTime estimateIntegrators(int,int,SKTime);
    void executeIntegrators(int);
    int findLatencyIndex(double);
    double linearInterpolation(SKTime, SKTime, double, double, SKTime);
    void executeBlocks(size_t,int);
    void checkStates(size_t,int);
    SKTime findStateChanges(size_t,int,SKTime);
    SKTime linAppCrossings(size_t,SKTime,SKTime,double,double);
    void computeBkwds(size_t,int);
};


#endif /* SKModel_h */
