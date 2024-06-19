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
 * file Lha_gmlparser.hpp                                                      *
 * Created by Benoit Barbot on 03/11/11.                                       *
 *******************************************************************************
 */


#ifndef _LHA_GML_HPP
#define	_LHA_GML_HPP

#include "expatmodelparser.hh"
#include "modelhandler.hh"
#include "Lha-Reader.hpp"

#include <map>

class MyLhaModelHandler: public ModelHandler
{
public:
	
    MyLhaModelHandler(LhaType* ,parameters &P) ;
	//~MyModelHandler() { }
	
	
    void on_read_model(const XmlString& formalismUrl) ;
	
    void on_read_model_attribute(const Attribute& attribute)	;
	
    void on_read_node(const XmlString& id,
					  const XmlString& nodeType,
					  const AttributeMap& attributes,
					  const XmlStringList& references) ;
	
    void on_read_arc(const XmlString& id,
					 const XmlString& arcType,
					 const XmlString& source,
					 const XmlString& target,
					 const AttributeMap& attributes,
					 const XmlStringList& references);
	
private:
	string simplifyString(string str);
	bool is_void(string str);
	void appendSimplify(string *st, string str);
	void eval_expr(bool *is_mark_dep, string *st, tree<string>::pre_order_iterator);
	size_t eval_marking_expr(string&, tree<string>::pre_order_iterator);
	
	void eval_term(   vector<string>& , tree<string>::pre_order_iterator );
	void eval_linexpr(vector<string>& , tree<string>::pre_order_iterator );
	void eval_guard(vector<vector<string> >&,vector<string>& , vector<string>& , tree<string>::pre_order_iterator );
	int eval_str (string);
	int eval_intFormula( map<std::string,int> intconst, tree<string>::pre_order_iterator);
	treeSI findbranchlha(treeSI t, string branch);
	Eval Evaluate_gml;
	
	HaslFormulasTop* exportHASLTop(tree<string>::pre_order_iterator);
	string exportHASL(tree<string>::pre_order_iterator);
	
	vector<string> FuncUpdateVector;
	
	set <string> PetriTransitions;
	//set <string> SubSet;
	
	
	
	vector < string > CoeffsVector;
	vector < vector <string> > CoeffsMatrix;
	vector <string> CST;
	vector <string> comp;
	
	int countLoc;
	bool ParseLoc;
	//bool ParseDecl;
	map<int,int> Gml2Loc;
	map<int
	,bool> IsPlace;
	LhaType* MyLHA;
	parameters P;

};








#endif


