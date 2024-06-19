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
 * file Gspn_gmlparser.hpp                                                     *
 * Created by Benoit Barbot on 05/10/12.                                       *
 *******************************************************************************
 */

#ifndef _SPN_GML_HPP
#define	_SPN_GML_HPP


#include "expatmodelparser.hh"
#include "modelhandler.hh"
#include "Gspn-model.hpp"
#include <map>

#include <exception>


class MyModelHandler: public ModelHandler
{
public:
	bool ParsePl;
    std::map<int,bool> IsPlace;
	std::map<int,int> Gml2Place;
	std::map<int,int> Gml2Trans;
	GspnType *MyGspn;
	
	MyModelHandler(GspnType&) ;
	MyModelHandler(GspnType&,std::map<int,bool>&,std::map<int,int>&,std::map<int,int>&);
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
	expr eval_expr(tree<std::string>::pre_order_iterator it );
    int eval_intFormula( tree<std::string>::pre_order_iterator it );
    double eval_realFormula(tree<std::string>::pre_order_iterator it );
	void eval_tokenProfileArc(coloredToken& ,bool &, std::set<size_t>& , tree<std::string>::pre_order_iterator,bool isInitialMarking);
	expr eval_guard(tree<std::string>::pre_order_iterator);

	std::string simplifyString(std::string str);
	treeSI findbranch(treeSI t, std::string branch);
};








#endif


