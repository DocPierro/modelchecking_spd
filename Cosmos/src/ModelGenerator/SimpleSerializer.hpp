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
 * file SimpleSerializer.hpp                                                   *
 * Created by Benoit Barbot on 04/005/2018.                                    *
 *******************************************************************************
 */

#ifndef SimpleSerializer_hpp
#define SimpleSerializer_hpp

#include <stdio.h>
#include <iostream>
#include <map>

#include "expressionStruct.hpp"

class Serializer{
private:
    std::ostream &of;
public:
    Serializer(std::ostream &f):of(f){};
    Serializer operator&(bool d);
    Serializer operator&(double d);
    Serializer operator&(expr &d);
    Serializer operator&(int d);
    Serializer operator&(unsigned int d);
    Serializer operator&(long d);
    Serializer operator&(unsigned long d);
    Serializer operator&(const std::string &d);
    template <class T, class Q>
    Serializer operator&(std::map<T,Q> &m){
        of << m.size() << " ";
        for( auto & key : m ){
            *this & key.first;
            *this & key.second;
        }
        return *this;
    };
};

class HumanReadable{
private:
    std::ostream &of;
public:
    HumanReadable(std::ostream &f):of(f){};
    HumanReadable operator&(bool d);
    HumanReadable operator&(double d);
    HumanReadable operator&(expr &d);
    HumanReadable operator&(int d);
    HumanReadable operator&(unsigned int d);
    HumanReadable operator&(long d);
    HumanReadable operator&(unsigned long d);
    HumanReadable operator&(const std::string &d);
    template <class T, class Q>
    HumanReadable operator&(std::map<T,Q> &m){
        of << "dictionary:{";
        for( const auto & key : m ){
            *this & key.first;
            of << " : ";
            *this & key.second;
            of << "; ";
        }
        of << "}";
        return *this;
    };
};

class Unserializer{
private:
    std::istream &inf;
public:
    Unserializer(std::istream &f):inf(f){};
    Unserializer operator&(bool &d);
    Unserializer operator&(double &d);
    Unserializer operator&(int &d);
    Unserializer operator&(expr &d);
    Unserializer operator&(unsigned int &d);
    Unserializer operator&(long &d);
    Unserializer operator&(unsigned long &d);
    Unserializer operator&(std::string &d);
    template <class T, class Q>
    Unserializer operator&(std::map<T,Q> &m){
        size_t length;
        inf >> length;
        for( size_t i=0; i< length; i++){
            T key;
            Q value;
            *this & key;
            *this & value;
            m.insert(std::pair<T,Q>(key,value));
        }
        return *this;
    };
};


#endif /* SimpleSerializer_hpp */
