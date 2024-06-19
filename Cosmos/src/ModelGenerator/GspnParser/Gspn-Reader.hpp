/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini & Hilal Djafri                                     *
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

#ifndef GSPN_READER_HH
#define GSPN_READER_HH
#include <string>
#include <map>
#include <set>
#include <functional>
#include <unordered_map>
#include <vector>

#include "../expressionStruct.hpp"
#include "../parameters.hpp"
#include "Gspn-parser.hh"
#include "expatmodelparser.hh"
#include "modelhandler.hh"
#include "Gspn-model.hpp"

#define GSPN_DECL                                            \
gspn::Gspn_parser::token_type                         \
gspnlex (gspn::Gspn_parser::semantic_type* gspnlval,      \
gspn::Gspn_parser::location_type* gspnlloc,      \
Gspn_Reader& Reader)

GSPN_DECL;

class Gspn_Reader {
public:
    Gspn_Reader(parameters &P);
	
    virtual ~Gspn_Reader();
	
    std::shared_ptr<GspnType> spn;
	
	int parse(std::string&);
    int parse_file(std::string&);
	int parse_gml_file(parameters&);
	
	void error(const gspn::location& l, const std::string& m);
	
	//<! Index of distribution type.
	std::map<std::string, int> IndexDist;
	parameters P;
	
private:
	
    void scan_begin();
    void scan_end();
    void scan_expression(const std::string&);
	
    bool trace_scanning;
	
    bool trace_parsing;
	
	
    
    void error(const std::string& m);
    
    //void addSink();
    void addSinkTrans();
    
    void view();
	
};
#endif // ! GSPN_READER_HH
