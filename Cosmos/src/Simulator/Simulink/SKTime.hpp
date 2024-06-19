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
 * file SKTime.hpp created by Yann Duplouy.                                    *
 *******************************************************************************
 */



#ifndef SKModel_Commons
#define SKModel_Commons
#include <sys/types.h>

class SKTime {
public:
	SKTime();
	SKTime(double);
	SKTime(size_t,int64_t);

	void updatePrecision(int);
	void set_to(double);
	void increment(double);
	int getPrecision();
	int64_t getBareTime();
	double getDouble();
	void setBareTime(int64_t);

	SKTime& operator = (double);
	SKTime operator + (SKTime);
	SKTime operator - (SKTime);
	SKTime operator + (double);
	SKTime operator - (double);
	bool operator == (SKTime);
	bool operator != (SKTime);
	bool operator < (SKTime);
	bool operator > (SKTime);
	SKTime operator * (int);
//	operator double();

private:
	int precision;
	int64_t time;
	double power;
};

#endif
