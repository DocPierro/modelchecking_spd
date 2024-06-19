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

#ifndef EVAL_HH
#define EVAL_HH

#include "Eval-parser.hh"


#define EVAL_DECL                                            \
eval::Eval_parser::token_type                         \
evallex (eval::Eval_parser::semantic_type* evallval,      \
eval::Eval_parser::location_type* evallloc,      \
Eval& Evaluate)

EVAL_DECL;

class Eval {
public:
    Eval();
    virtual ~Eval();
	
    int IntResult;
    double RealResult;
    void scan_begin();
    void scan_end();
    void scan_expression(const std::string&);
	
    bool trace_scanning;
	
	
    int parse(const std::string&);
	
	
    bool trace_parsing;
	
	
    void error(const eval::location& l, const std::string& m);
    void error(const std::string& m);
	
    void view();
};
#endif
