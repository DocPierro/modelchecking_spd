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
 * file SimpleSerializer.cpp                                                   *
 * Created by Benoit Barbot on 04/005/2018.                                    *
 *******************************************************************************
 */

#include "SimpleSerializer.hpp"

using namespace std;

Serializer Serializer::operator&(bool d) {
    of << d << " ";
    return *this;
}
Serializer Serializer::operator&(double d) {
    of << d << " ";
    return *this;
}

Serializer Serializer::operator&(expr &d) {
    of << d.to_string() << " ";
    return *this;
}

Serializer Serializer::operator&(int d) {
    of << d << " ";
    return *this;
}
Serializer Serializer::operator&(unsigned int d) {
    of << d << " ";
    return *this;
}
Serializer Serializer::operator&(long d) {
    of << d << " ";
    return *this;
}
Serializer Serializer::operator&(unsigned long d) {
    of << d << " ";
    return *this;
}
Serializer Serializer::operator&(const std::string &d){
    of << d.length() << " " << d << " ";
    return *this;
}

HumanReadable HumanReadable::operator&(bool d) {
    of << "bool:" << d << " ";
    return *this;
}
HumanReadable HumanReadable::operator&(double d) {
    of << "double:" << d << " ";
    return *this;
}

HumanReadable HumanReadable::operator&(expr &d) {
    const auto s = d.to_string();
    of << s.length() << " " << s << " ";
    return *this;
}


HumanReadable HumanReadable::operator&(int d) {
    of << "int:"<< d << " ";
    return *this;
}
HumanReadable HumanReadable::operator&(unsigned int d) {
    of << "unsigned int:" << d << " ";
    return *this;
}
HumanReadable HumanReadable::operator&(long d) {
    of << "long:" << d << " ";
    return *this;
}
HumanReadable HumanReadable::operator&(unsigned long d) {
    of << "unsigned long:"<< d << " ";
    return *this;
}
HumanReadable HumanReadable::operator&(const std::string &d){
    of << "string:\"" << d << "\" ";
    return *this;
}

Unserializer Unserializer::operator&(bool &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(double &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(int &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(expr &d) {
    size_t length;
    inf >> length >> ws;
    string s;
    s.resize(length);
    inf.read(&s[0], length);
    d = expr(s);
    return *this;
}


Unserializer Unserializer::operator&(unsigned int &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(long &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(unsigned long &d) {
    inf >> d;
    return *this;
}
Unserializer Unserializer::operator&(std::string &d){
    size_t length;
    inf >> length >> ws;
    d.resize(length);
    inf.read(&d[0], length);
    return *this;
}
