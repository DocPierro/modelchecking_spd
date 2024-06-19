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
 * file SKTime.cpp created by Yann Duplouy.                                    *
 *******************************************************************************
 */


#include <stdio.h>
#include "SKTime.hpp"
#include <math.h>

SKTime::SKTime() {
	power=0; precision=0; time=0;
	updatePrecision(5);
	set_to(0.0);
}
SKTime::SKTime(double initTime) {
	power=0; precision=0; time=0;
	updatePrecision(5); set_to(initTime);
}
SKTime::SKTime(size_t prec, int64_t bareTime) {
	power=0; precision=0; time=0;
	updatePrecision(prec);
	time = bareTime;
}
void SKTime::updatePrecision(int prec) {
	if (prec > precision) {
		precision = prec;
		power = pow(10., prec);
		time = pow(10,(prec-precision)) * time;
	}
}
void SKTime::set_to(double newTime) {
	time = round(newTime * power);
}
int64_t SKTime::getBareTime() { return time; }
void SKTime::setBareTime(int64_t bareTime) {
	time=bareTime;
}
int SKTime::getPrecision() { return precision; }
double SKTime::getDouble() {
	double approxTime = time / power;
	return approxTime;
}

SKTime& SKTime::operator= (double newTime) { updatePrecision(5); set_to(newTime);return *this; }

bool SKTime::operator== (SKTime otherTime) {
	int precB = otherTime.getPrecision();
	if (precB > precision) { updatePrecision(precB); }
	else if (precB < precision) { otherTime.updatePrecision(precision); }
	return (time == otherTime.time);
}
bool SKTime::operator!= (SKTime otherTime) {
	int precB = otherTime.getPrecision();
	if (precB > precision) { updatePrecision(precB); }
	else if (precB < precision) { otherTime.updatePrecision(precision); }
	return (time != otherTime.time);
}
SKTime SKTime::operator+ (SKTime otherTime) {
	int precB = otherTime.getPrecision();
	if (precB > precision) { updatePrecision(precB); }
	else if (precB < precision) { otherTime.updatePrecision(precision); }
	return SKTime(precision,time+otherTime.getBareTime());
}
SKTime SKTime::operator- (SKTime otherTime) {
	int precB = otherTime.getPrecision();
	if (precB > precision) { updatePrecision(precB); }
	else if (precB < precision) { otherTime.updatePrecision(precision); }
	return SKTime(precision,time-otherTime.getBareTime());
}
SKTime SKTime::operator- (double otherTime) {
	int64_t oTime = round(otherTime * power);
	return SKTime(precision,time-oTime);
}
SKTime SKTime::operator+ (double otherTime) {
	int64_t oTime = round(otherTime * power);
	return SKTime(precision,time+oTime);
}
bool SKTime::operator< (SKTime otherTime) {
	int prec = getPrecision();
	int precB = otherTime.getPrecision();
	if (precB > prec) { updatePrecision(precB); }
	else if (precB < prec) { otherTime.updatePrecision(prec); }
	return getBareTime() < otherTime.getBareTime();
}
bool SKTime::operator> (SKTime otherTime) {
	int prec = getPrecision();
	int precB = otherTime.getPrecision();
	if (precB > prec) { updatePrecision(precB); }
	else if (precB < prec) { otherTime.updatePrecision(prec); }
	return getBareTime() > otherTime.getBareTime();
}
SKTime SKTime::operator* (int number) {
	return SKTime(precision,time*number);
}
// SKTime::operator double() { return time / power; }
