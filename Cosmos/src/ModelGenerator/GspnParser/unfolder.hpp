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
 * file unfolder.hpp created by Benoit Barbot on 10/01/14.                     *
 *******************************************************************************
 */

#ifndef __Cosmos__unfolder__
#define __Cosmos__unfolder__

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


#include "Gspn-Reader.hpp"

class unfolder: public GspnType {
public:
	unfolder(GspnType& g):GspnType(g),nbPlace(0),nbTrans(0),nbArc(0){};
	
    void export_grml(std::ofstream &fout);
private:
	//void export_init_grml(ofstream &fout);

    void export_place_grml(std::ofstream &fout,const place &p);
	void export_transition_grml(std::ofstream &fout,const transition&t);
	void export_arc_grml(std::ofstream &fout,const transition &t);
	void export_coltoken(std::ofstream &fout,const std::vector<color> &vec,const coloredToken &coltoken,const transition &t,const place &p, bool );
    void export_multcoltok(std::ofstream &fout,const std::vector<color> &vec,const transition &t,const place &p, bool ,const arc);
	//size_t next_uid;
    void print_arc(std::ofstream&,size_t,size_t,size_t, bool, const expr&);

    std::string cleanstr(const std::string &) const;
	std::string str_of_vect(const std::vector<color> &v,const std::string &smid) const;
	
	size_t nbPlace,nbTrans,nbArc;
};
#endif /* defined(__Cosmos__unfolder__) */
