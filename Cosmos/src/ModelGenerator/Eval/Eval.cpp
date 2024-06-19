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

#include "Eval-parser.hh"
#include "Eval.hpp"

#include "string.h"

#include <string>
#include <sstream>
#include <fstream>
#include <set>




using namespace std;

Eval::Eval() {
    trace_scanning = false;
    trace_parsing = false;
	
}

Eval::~Eval() {
}

int Eval::parse(const string& expr) {
	
	
    scan_expression(expr);
	
    eval::Eval_parser parser(*this);
	
    parser.set_debug_level(trace_parsing);
	
    int res = parser.parse();
    scan_end();
    return res;
}

void Eval::error(const eval::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void Eval::error(const std::string& m) {
    std::cerr << m << std::endl;
}

