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
 * file Lha_gmlparser.cpp                                                      *
 * Created by Benoit Barbot on 03/11/11.                                       *
 *******************************************************************************
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <cstdlib>
#include <regex>

#include "Lha_gmlparser.hpp"
#include "expatmodelparser.hh"
#include "modelhandler.hh"

#include "Lha-Reader.hpp"

//#include "tree.hh"
#include "tree/tree_util.hh"


using namespace std;


/*
 Function sometime usefull for debuging
void print_tree(const tree<string>& tr, tree<string>::pre_order_iterator it, tree<string>::pre_order_iterator end)
{
	if(!tr.is_valid(it)) return;
	int rootdepth=tr.depth(it);
	while(it!=end) {
		cout << "    ";
		for(int i=0; i<tr.depth(it)-rootdepth; ++i)
			cout << "  ";
		cout << (*it) << endl << flush;
		++it;
	}
}
 */

string MyLhaModelHandler::simplifyString(string str)
{
	size_t n1 = str.find_first_not_of(" \n");
	size_t n2 = str.find_last_not_of(" \n");
	if(n1 ==std::string::npos )return "";
	return str.substr(n1, n2-n1+1);
}

bool MyLhaModelHandler::is_void(string str)
{
	//int n1 = str.find_first_not_of(" \n");
	//int n2 = str.find_last_not_of(" \n");
	return str.compare("\n");
}

void MyLhaModelHandler::appendSimplify(string *st, string str)
{
	size_t n1 = str.find_first_not_of(" \n");
	size_t n2 = str.find_last_not_of(" \n");
	if(n1 !=std::string::npos )st->append(str.substr(n1, n2-n1+1));
}

/*void evalinfix(string *st, tree<string>::pre_order_iterator it , string str){
 st->append("(");
 for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
 if(it2!= it.begin()) st->append(str);
 eval_expr( st, it2);
 }
 st->append(")");
 }*/

void MyLhaModelHandler::eval_expr(bool *is_mark_dep, string *st, tree<string>::pre_order_iterator it )
{
    
	if((P.verbose-3)>2)cout << (*it) << endl;
	if((*it) == "expr"){
		eval_expr(is_mark_dep, st, it.begin());
	}else if((*it) == "boolExpr"){
        eval_expr(is_mark_dep, st, it.begin());
    }else if((*it) == "markingExpr"){
		string stmark;
		size_t colDomIndex = eval_marking_expr(stmark, it.begin());
		if (colDomIndex == UNCOLORED_DOMAIN){
			st->append(stmark);
		}else {
			st->append(stmark);
			st->append(".card()");
			//cerr << "Not uncolored type: " << stmark << endl;
		}
		*is_mark_dep = true;
    }else if((*it) == "function"){
        eval_expr(is_mark_dep, st, it.begin());
    }else if((*it) == "numValue"){
		if((P.verbose-3)>2)cout << "\t" << it.node->first_child->data<<endl;
		appendSimplify(st,it.node->first_child->data);
	}else if ((*it) == "boolValue") {
		appendSimplify(st,it.node->first_child->data);
	}else if ((*it) == "name") {
        string var = simplifyString(it.node->first_child->data);
        if(MyLHA->LhaIntConstant.count(var)>0 ||
           MyLHA->LhaRealConstant.count(var)>0){st->append(var);
        }else {
			vector<string>::const_iterator vn = find(MyLHA->Vars.label.begin(), MyLHA->Vars.label.end(), var);
			if(vn != MyLHA->Vars.label.end()){
				std::ostringstream s; s<<"Vars->"<< var;
				st->append(s.str());
				
			} else if(MyLHA->MyGspn->PlacesId.count(var)>0) {
				*is_mark_dep =true;
				st->append("Marking.P->_PL_");
				st->append(var.c_str());
				if(MyLHA->MyGspn->placeStruct[MyLHA->MyGspn->PlacesId[var]].colorDom != UNCOLORED_DOMAIN )
					st->append(".card()");
				st->append(" ");
			} else {
				vector<colorVariable>::const_iterator varit=  MyLHA->MyGspn->colVars.begin();
				for ( ; varit != MyLHA->MyGspn->colVars.end() && varit->name.compare(var)!=0 ; ++varit) ;
				if (varit != MyLHA->MyGspn->colVars.end()) {
					st->append("b.P->");
					st->append(varit->name);
					st->append(".c0");
				}else {
					throw GrmlInputException("Unknown name:" + var );
				}
				
			}
		}
    }else if (	(*it) == "+"  || (*it) == "*"
			  || (*it) == "min"   || (*it) == "max"
			  || (*it) == "floor" || (*it) == "-"
			  || (*it) == "/"   || (*it) == "power"
			  || (*it) == "or"   || (*it) == "and"
			  || (*it) == "not"   || (*it) == "equal"
			  || (*it) == "notEqual"   || (*it) == "greater"
			  || (*it) == "greaterEqual"   || (*it) == "less"
			  || (*it) == "lessEqual" ){
		
		
		if ((*it) == "min") st->append("min");
		if ((*it) == "max") st->append("max");
		if ((*it) == "floor" ) st->append("floor");
		
		st->append("(");
		for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
			if(it2!= it.begin()) {
				if ((*it) == "+" ) { st->append("+"); }
				else if ((*it) == "*" ) { st->append("*"); }
				else if ((*it) == "-" ) { st->append("-"); }
				else if ((*it) == "/") { st->append("/"); }
				else if ((*it) == "power") { st->append("^"); }
				else if ((*it) == "or") { st->append(" || "); }
				else if ((*it) == "and") { st->append(" && "); }
				else if ((*it) == "not") { st->append(" !"); }
				else if ((*it) == "notEqual") { st->append(" !="); }
				else if ((*it) == "equal") { st->append("=="); }
				else if ((*it) == "greater") { st->append(">"); }
				else if ((*it) == "greaterEqual") { st->append(">="); }
				else if ((*it) == "less") { st->append("<"); }
				else if ((*it) == "lessEqual") { st->append("<="); }
				else st->append(",");
			}
			eval_expr(is_mark_dep, st, it2);
		}
		st->append(")");
		;
    } else throw GrmlInputException("Unexpected:" + *it);
}

size_t MyLhaModelHandler::eval_marking_expr(string &st, tree<string>::pre_order_iterator it){
	if((P.verbose-3)>2)cout << "eval_marking_expr:"<< (*it) << endl;
	if(*it == "markingExpr")
		return eval_marking_expr(st, it.begin());
	else if(*it == "name"){
		string stpl = simplifyString(*(it.begin()));
		if(MyLHA->MyGspn->PlacesId.count(stpl)>0){
			st.append("Marking.P->_PL_");
			st.append(stpl);
			return MyLHA->MyGspn->placeStruct[MyLHA->MyGspn->PlacesId[stpl]].colorDom;
		} else {
			st.append(stpl);
			return UNCOLORED_DOMAIN;
		}
	} else if(*it == "selectColor"){
		st.append(" ( ");
		size_t colClassCounter =0;
		size_t colDomIndex = (size_t)-1;
		for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
			if((P.verbose-3)>2)cout << (*it2) << endl;
			if(*it2 == "markingExpr") colDomIndex = eval_marking_expr(st, it2);
			else if(*it2 == "name"){
				string cname = simplifyString(*(it2.begin()));
				vector<color>::const_iterator itcc;
				colorClass cc = MyLHA->MyGspn->colClasses[MyLHA->MyGspn->colDoms[colDomIndex].colorClassIndex[colClassCounter]];
				for( itcc= cc.colors.begin(); itcc != cc.colors.end() && cname != itcc->name; ++itcc) ;
				if (itcc == cc.colors.end()) {
					cerr << "unknwon color:" << cname << "for color classe" << cc.name<< endl;
				}else{
					st.append("[ Color_" + cc.name + "_" + itcc->name +" ]");
				}
				colClassCounter++;
			}
		}
		st.append(" ) ");
		return colDomIndex;
	}
    throw GrmlInputException("Unexpected: "+ *it);
}


void MyLhaModelHandler::eval_term(vector<string> &CoeffsVector, tree<string>::pre_order_iterator it){
	if((P.verbose-3)>2)cout << (*it) << endl;
	if((*it) == "expr"){
		eval_term(CoeffsVector, it.begin());
	}else if((*it) == "name"){
		string var = simplifyString(*(it.begin()));
		if((P.verbose-3)>2)cout << "\t" << var << endl;
		vector<string>::const_iterator vi=find(MyLHA->Vars.label.begin(), MyLHA->Vars.label.end(), var);
		if(vi!= MyLHA->Vars.label.end())CoeffsVector[vi-MyLHA->Vars.label.begin()]= "1";
		else cout << "Unkown Variable " << var <<endl;
	}else cout << "fail eval tree : linexp" << endl;
	
}

void MyLhaModelHandler::eval_linexpr(vector<string> &CoeffsVector, tree<string>::pre_order_iterator it){
	if((P.verbose-3)>2)cout << (*it) << endl;
	if((*it) == "expr"){
		eval_linexpr(CoeffsVector, it.begin());
	}else if((*it) == "function" && (((*it.begin()) == "+") || ((*it.begin()) == "-"))){
		for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
			eval_linexpr(CoeffsVector, it2);
		}
	}else eval_term(CoeffsVector, it);
	
}

void MyLhaModelHandler::eval_guard(vector<vector<string> > &CoeffsMatrix,vector<string> &CST, vector<string> &comp,tree<string>::pre_order_iterator it)
{
	if((P.verbose-3)>2)cout << (*it) << endl;
	if((*it) == "boolExpr"){
        eval_guard(CoeffsMatrix,CST,comp,it.begin());
    }else if ((*it) == "boolValue") {
		//cout << "\tguard = true" << it.node->first_child->data << endl;
	}else if ((*it) == "and"){
		//cout << "(";
		for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
			eval_guard(CoeffsMatrix,CST,comp,it2);
		}
		//cout << ")";
	}else if((*it) == "equal"
			 || (*it) == "greaterEqual"
			 || (*it) == "lessEqual" ){
		if((*it) == "equal")comp.push_back("==");
		if((*it) == "lessEqual")comp.push_back("<=");
		if((*it) == "greaterEqual")comp.push_back(">=");
		
		vector<string> CoeffsVector(MyLHA->NbVar,"");
		eval_linexpr(CoeffsVector,it.begin());
		//cout << ";";
		CoeffsMatrix.push_back(CoeffsVector);
		string* st= new string("");
		bool markdep = false;
		eval_expr(&markdep, st, it.begin().node->next_sibling);
		if((P.verbose-3)>1)cout<< "CST: " << *st << endl;
		CST.push_back(*st);
		
	} else cout << "failevaltree: guard" <<endl;
}


int MyLhaModelHandler::eval_str (string s){
	string val = simplifyString(s);
	int intval = atoi( val.c_str() );
	return intval;
}

int MyLhaModelHandler::eval_intFormula( map<std::string,int> intconst, tree<string>::pre_order_iterator it )
{
    if((P.verbose-3)>2)cout << "intformula: " <<(*it) << endl;
	if((*it) == "expr"){
		return eval_intFormula(intconst,it.begin());
	}else if((*it) == "numValue"){
		return eval_str(it.node->first_child->data);
	}else if ((*it) == "name") {
		string val = simplifyString(it.node->first_child->data);
		int intval = intconst[(val).c_str()];
		return intval;
	}else if ((*it) == "+" || (*it) == "*"
			  || (*it) == "min"  || (*it) == "max"
			  || (*it) == "ipower"|| (*it) == "-")  {
		
		int v1=0;
		int v2=0;
		for (treeSI it2 = (it.begin()) ; it2 != (it.end()) ; ++it2 ) {
			if(it2!= it.begin()) { v1 = eval_intFormula(intconst, it2);
			} else {
				v2 = eval_intFormula(intconst, it2);
			}
			
		}
		
		if ((*it) == "+") { return v1+v2; }
		else if ((*it) == "*") { return v1*v2; }
		else if ((*it) == "-") { return v1-v2; }
		else if ((*it) == "min") { return min(v1,v2); }
		else if ((*it) == "max") {  return max(v1,v2); }
		else if ((*it) == "power") {  return v1^v2; }
		else throw GrmlInputException("Unexpected: "+ *it);
		
	}else throw GrmlInputException("Unexpected: "+ *it);
}



MyLhaModelHandler::MyLhaModelHandler(LhaType *MyLHA2,parameters &Q):MyLHA(MyLHA2), P(Q) {
	//Initialisation
	MyLHA->NbVar = 0;
	countLoc=0;
	ParseLoc=true;
	//ParseDecl=true;
}
//~MyModelHandler() { }


void MyLhaModelHandler::on_read_model(const XmlString& formalismUrl) {
	// output used formalism
	ParseLoc=true;
	//ParseDecl=true;
	if((P.verbose-3)>0)cout << "read model : formalism = " << formalismUrl << endl;
}

HaslFormulasTop* MyLhaModelHandler::exportHASLTop(tree<string>::pre_order_iterator it){
	
	if((P.verbose-3)>2)cout << *it << ":" << endl;
	if((*it) == "AVG"){
		string lhafunc = exportHASL(it.begin());
        if((P.verbose-3)>1)cout << lhafunc << endl;
		stringstream ss;
		ss << "LhaFunc[" << MyLHA->LhaFunction[lhafunc] << "]";
		MyLHA->Algebraic.push_back( ss.str() );
		return (new HaslFormulasTop(MyLHA->Algebraic.size()-1));
	} else if((*it) == "PROB"){
		return (new HaslFormulasTop(PROBABILITY));
	} else if(*it == "PDF" || *it == "CDF"){
		string lhafunc;
		double deltab = 1;
		double minb =0;
		double maxb =0;
		for(treeSI it2 = it.begin(); it2 != it.end(); it2++){
			if (*it2 == "min")minb = atof(exportHASL(it2).c_str());
			else if (*it2 == "max")maxb = atof(exportHASL(it2).c_str());
			else if (*it2 == "delta")deltab = atof(exportHASL(it2).c_str());
			else lhafunc = exportHASL(it2);
		}
		if(lhafunc.empty()){
			throw GrmlInputException("Ill formed HASL formula");
		}
		
        if((P.verbose-3)>1)cout << lhafunc << endl;
		stringstream ss;
		ss << "LhaFunc[" << MyLHA->LhaFunction[lhafunc] << "]";
		
		
		for(double bucket = minb ; bucket < maxb ; bucket+= deltab){
			std::ostringstream algPDF;
			if(*it == "PDF")
				algPDF << "(("<< ss.str() <<" >= "<<bucket<<"&& "<<ss.str() <<"<"<<bucket+deltab<<") ? 1:0)";
			else
				algPDF << "(("<< ss.str() <<" <= "<<bucket<<") ? 1:0)";
			
			MyLHA->Algebraic.push_back(algPDF.str());
			MyLHA->HASLtop.push_back(new HaslFormulasTop((size_t)MyLHA->Algebraic.size()-1));
			MyLHA->HASLtop.back()->TypeOp = PDF_PART;
			std::ostringstream s; s<<"$_$: Value in ["<< bucket<< " , "<<bucket+deltab<<"]";
			MyLHA->HASLname.push_back(s.str());
		}
		
		return (NULL);
	} else throw GrmlInputException("Unexpected: "+ *it);
}


string MyLhaModelHandler::exportHASL(tree<string>::pre_order_iterator it){
	if((P.verbose-3)>2)cout << *it << ":" << endl;
	if(*it == "YHF"){
		return exportHASL(it.begin());
	}else if(*it == "delta"){
		return simplifyString(*(it.begin()));
	}else if(*it == "min"){
		return simplifyString(*(it.begin()));
	}else if(*it == "max"){
		return simplifyString(*(it.begin()));
	}else if(*it == "last"){
		//cout << *(it.begin()) << endl;
		string linForm = exportHASL(it.begin());
		const char* linformc = linForm.c_str();
		if(MyLHA->LinearForm.find(linformc)==MyLHA->LinearForm.end()){
			size_t i=MyLHA->LinearForm.size();
			MyLHA->LinearForm[linformc]=i;
		}
		
		MyLHA->LhaFuncArg.push_back(MyLHA->LinearForm[linformc]);
		MyLHA->LhaFuncType.push_back("Last");
		string ss="Last("; ss.append(linformc); ss.append(")");
		if(MyLHA->LhaFunction.find(ss)==MyLHA->LhaFunction.end()){
			size_t i=MyLHA->LhaFunction.size();
			MyLHA->LhaFunction[ss]=i;
		}
		return ss;
		
	}else if((*it) == "expr"){
		return exportHASL(it.begin());
	}else if((*it) == "name"){
		string var = simplifyString(it.node->first_child->data);
		stringstream ss;
		ss << "Vars->" << var;
		
		return ss.str();
		
	} else throw GrmlInputException("Unexpected: "+ *it);
	
}

treeSI MyLhaModelHandler::findbranchlha(treeSI t, string branch){
    if( branch == "")return t;
    size_t nextnode = branch.find_first_of("/");
    for (treeSI it = (t.begin()) ; it != (t.end()) ; ++it) {
        if((*it).compare(branch.substr(0,nextnode))==0){
            return findbranchlha(it, branch.substr(nextnode+1,branch.length()-nextnode-1));
        }
    }
    cerr << "Fail to find branch:" << branch << "in the file\n";
	return t.end();
}

void MyLhaModelHandler::on_read_model_attribute(const Attribute& attribute) {
	// read model attribute
	for(treeSI it = attribute.begin(); it != attribute.end(); ++it) {
		if((P.verbose-3)>1)cout << *it << ":" << endl;
		if((*it) == "declaration"){
            //cout<< "start int const" << endl;
            treeSI t1 = findbranchlha(it, "constants/intConsts/");
            //cout << "find branch" << endl;
			if (t1 != it.end())
				for (treeSI it2 = (t1.begin()) ; it2 != (t1.end()) ; ++it2 ) {
					if ((*it2) == "intConst") {
						//cout << "start intConst" << endl;
						string constname = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
						//cout << "finish name:"<< *constname << endl;
						//treeSI test = find(it2.begin(),it2.end(),"expr");
						//cout << "find expr" << endl;
						int constvalue = eval_intFormula(MyLHA->LhaIntConstant,find(it2.begin(),it2.end(),"expr"));
						//cout << "finish expr" << endl;
						//Evaluate_gml.parse(constvalue);
						if((P.verbose-3)>1)cout << "\tconst int " << constname << "=" << constvalue << endl;
						if (P.constants.count(constname)>0) {
                            constvalue = (int) P.constants[constname].get_real();
							if((P.verbose-3)>0)cout << "const int " << constname << " overwrite to " << constvalue << endl;;
						}
						MyLHA->LhaIntConstant[constname]=constvalue; //Evaluate_gml.IntResult;
						MyLHA->LhaRealConstant[constname]= constvalue; //Evaluate_gml.RealResult;
						
					}
				}
            //cout << "finished intconst" << endl;
            t1 = findbranchlha(it, "constants/realConsts/");
			if (t1 != it.end())
				for (treeSI it2 = (t1.begin()) ; it2 != (t1.end()) ; ++it2 ) {
					//cout << "test:" << *it2 << ":"<< endl;
					if ((*it2) == "realConst") {
						string constname = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
						
						bool ismarkdep=false;
						string constvalue;
						eval_expr( &ismarkdep, &constvalue, find(it2.begin(),it2.end(),"expr").begin());
						if(ismarkdep){
							throw GrmlInputException("Constant should not be marking dependant ");
						}
						if (P.constants.count(constname)>0) {
							constvalue = P.constants[constname].to_string();
							if((P.verbose-3)>0)cout << "const double " << constname << " overwrite to " << constvalue << endl;;
						}
						Evaluate_gml.parse(constvalue);
						if((P.verbose-3)>1)cout << "\tconst double " << constname << "=" << Evaluate_gml.RealResult << endl;
						
						MyLHA->LhaRealConstant[constname]=Evaluate_gml.RealResult;
						
					}
				}
            //cout << "finished realconst" << endl;
            t1 = findbranchlha(it, "variables/reals/");
			if (t1 != it.end())
				for (treeSI it2 = (t1.begin()) ; it2 != (t1.end()) ; ++it2 ) {
					if(*it2 == "real") {
						string constname = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
						MyLHA->Vars.label.push_back(constname);
						MyLHA->Vars.initialValue.push_back(0.0);
						treeSI dom = find(it2.begin(),it2.end(),"domain");
						if (dom != it2.end()) {
							string domname = simplifyString(*(dom.begin()));
							vector<colorDomain>::const_iterator domit;
							for(domit = MyLHA->MyGspn->colDoms.begin(); domit != MyLHA->MyGspn->colDoms.end() && domit->name != domname; ++domit) ;
							if(domit != MyLHA->MyGspn->colDoms.end())MyLHA->Vars.colorDomain.push_back(domit - MyLHA->MyGspn->colDoms.begin());
							else cerr << "Unknown color Domain " << domname << endl;
						}else MyLHA->Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
						MyLHA->Vars.type.push_back(CONTINIOUS_VARIABLE);
                        MyLHA->Vars.isTraced.push_back(true);
						MyLHA->NbVar++;
						
						if((P.verbose-3)>1)cout << "\tcontinuous var " << constname << " index: " << MyLHA->NbVar-1 << " domain: " << MyLHA->Vars.colorDomain[MyLHA->NbVar-1] <<endl;
					} else if(*it2 != "")cout << "Unknown variable Type" << *it2 << endl;
				}
            //cout << "finished realvar" << endl;
            t1 = findbranchlha(it, "variables/discretes/");
			if (t1 != it.end())
				for (treeSI it2 = (t1.begin()) ; it2 != (t1.end()) ; ++it2 ) {
					if ((*it2) == "discrete") {
						string constname = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
						MyLHA->Vars.label.push_back(constname);
						MyLHA->Vars.initialValue.push_back(0.0);
						treeSI dom = find(it2.begin(),it2.end(),"domain");
						if (dom != it2.end()) {
							string domname = simplifyString(*(dom.begin()));
							vector<colorDomain>::const_iterator domit;
							for(domit = MyLHA->MyGspn->colDoms.begin(); domit != MyLHA->MyGspn->colDoms.end() && domit->name != domname; ++domit) ;
							if(domit != MyLHA->MyGspn->colDoms.end())MyLHA->Vars.colorDomain.push_back(domit - MyLHA->MyGspn->colDoms.begin());
							else cerr << "Unknown color Domain " << domname << endl;
						}else MyLHA->Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
						
						MyLHA->Vars.type.push_back(DISCRETE_VARIABLE);
                        MyLHA->Vars.isTraced.push_back(true);
                        MyLHA->NbVar++;
						if((P.verbose-3)>1)cout << "\tdiscrete var " << constname << " index: " << MyLHA->NbVar-1 << " domain: " << MyLHA->Vars.colorDomain[MyLHA->NbVar-1] <<endl;
					}
				}
			t1 = findbranchlha(it, "variables/colors/");
			if (t1 != it.end())
				for (treeSI it2 = (t1.begin()) ; it2 != (t1.end()) ; ++it2 ) {
					if ((*it2) == "color") {
						string constname = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
						MyLHA->Vars.label.push_back(constname);
						MyLHA->Vars.initialValue.push_back(0.0);
						treeSI dom = find(it2.begin(),it2.end(),"domain");
						if (dom != it2.end()) {
							string domname = simplifyString(*(dom.begin()));
							vector<colorClass>::const_iterator domit;
							for(domit = MyLHA->MyGspn->colClasses.begin(); domit != MyLHA->MyGspn->colClasses.end() && domit->name != domname; ++domit) ;
							if(domit != MyLHA->MyGspn->colClasses.end())MyLHA->Vars.colorDomain.push_back(domit - MyLHA->MyGspn->colClasses.begin());
							else cerr << "Unknown color Domain " << domname << endl;
						}else cerr << "No color class specify for color variable " << constname << endl;
						
						MyLHA->Vars.type.push_back(COLOR_VARIABLE);
						MyLHA->NbVar++;
                        MyLHA->Vars.isTraced.push_back(true);
						if((P.verbose-3)>1)cout << "\tcolor var " << constname << " index: " << MyLHA->NbVar-1<< " domain: " << MyLHA->Vars.colorDomain[MyLHA->NbVar-1] << endl;
					}
				}
			//cout << "finished discrete var" << endl;
		} else if((*it) == "HASLFormula"){
			//cout << "export hasl formula" << endl;
			HaslFormulasTop* haslform =  exportHASLTop(it.begin());
			if(haslform != NULL){
				MyLHA->HASLtop.push_back(haslform);
				MyLHA->HASLname.push_back("HASLFormula");
			}
		} else throw GrmlInputException("Unexpected: "+ *it);
	}
	//print_tree(attribute, attribute.begin(), attribute.end());
}

void MyLhaModelHandler::on_read_node(const XmlString& id,
									 const XmlString& nodeType,
									 const AttributeMap& attributes,
									 const XmlStringList&) {
	
	if((P.verbose-3)>1)cout << "read node : " << id << ", " << nodeType << endl;
	if(nodeType == "state"){
		//cout << "location:"<<endl;
		flush(cout);
		int id2 = atoi(id.c_str());
		Gml2Loc[id2]=countLoc;
		
		bool markdep=false;
		
        string Plname = simplifyString(*(attributes.find("name")->second.begin().begin()));
        if((P.verbose-3)>1)cout << "name: " << Plname << endl;
		MyLHA->LocLabel.push_back(Plname);
		MyLHA->LocIndex[Plname]=countLoc;
		
		string* inv = new string("");
		eval_expr(&markdep, inv, attributes.find("invariant")->second.begin().begin());
        if((P.verbose-3)>1)cout << "invariant: " << *inv << endl;
		MyLHA->FuncLocProperty.push_back(*inv);
		
		treeSI itflow = attributes.find("flows")->second.begin();
		vector<string> v1(MyLHA->NbVar,"");
		for(treeSI it2 = itflow.begin(); it2!=itflow.end();++it2){
			if((*it2) == "flow"){
				
				string var = simplifyString((find(it2.begin(),it2.end(),"name")).node->first_child->data);
				string varflow;
				
				eval_expr( &markdep, &varflow, find(it2.begin(),it2.end(),"expr").begin());
				
				size_t vi = find(MyLHA->Vars.label.begin(), MyLHA->Vars.label.end(), var) - MyLHA->Vars.label.begin();
				if((P.verbose-3)>1)cout << "\tvar: " << var << " index: " << vi << " flow: " << varflow << endl;
				if(MyLHA->Vars.type[vi] == CONTINIOUS_VARIABLE){
					v1[vi]= varflow;
				} else cout << "Variable "<< var << " is continious, it as no flow"<< endl;
			}
		}
		MyLHA->StrFlow.push_back(v1);
		MyLHA->FuncFlow.push_back(v1);
		
		string type = simplifyString(*(attributes.find("type")->second.begin().begin()));
		if((P.verbose-3)>1)cout << "\ttype:" << type << endl;
		if ((type) == "Initial") {
			MyLHA->InitLoc.insert(countLoc);
		} else if ((type) == "Final") {
			MyLHA->FinalLoc.insert(countLoc);
		}
		
		countLoc++ ;
		
	}
	
	
	/*for(AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
	 print_tree(it->second, it->second.begin(), it->second.end());
	 }
	 for(XmlStringList::const_iterator it = references.begin(); it != references.end(); ++it) {
	 cout << "    ref => " << *it << endl;
	 }*/
}

void MyLhaModelHandler::on_read_arc(const XmlString& id,
									const XmlString& arcType,
									const XmlString& source,
									const XmlString& target,
									const AttributeMap& attributes,
									const XmlStringList&) {
	if(ParseLoc){
		ParseLoc=false;
		MyLHA->NbLoc= countLoc;
		
		vector<string> v1(MyLHA->NbVar,"");
		
		vector< vector<string> > vv(MyLHA->NbLoc,v1);
		
		CoeffsVector=v1;
		vector<string> v2(MyLHA->NbLoc,"");
		//int sz=MyLHA->TransitionIndex.size();
		set <string> Pt;
		PetriTransitions=Pt;
		for(map<string, int>::iterator it=MyLHA->TransitionIndex.begin();it!=MyLHA->TransitionIndex.end();it++)
			PetriTransitions.insert((*it).first);

		MyLHA->Out_S_Edges=vector < set<int> >(MyLHA->NbLoc);
		MyLHA->Out_A_Edges=vector < set<int> >(MyLHA->NbLoc);
		
		
	}
	
	if((P.verbose-3)>1)cout << "read arc : " << id << ", " << arcType << ", " << source << " -> " << target << endl;
    
	int sourceGML = atoi(source.c_str());
	int targetGML = atoi(target.c_str());
	
	LhaEdge edge;
	edge.Index=MyLHA->Edge.size();
	edge.Source= Gml2Loc[sourceGML];
	edge.Target= Gml2Loc[targetGML];
    MyLHA->Edge.push_back(edge);

    set <string> SubSet;
    AttributeMap::const_iterator attrf = attributes.find("label");
    if (attrf != attributes.end()) {
        tree<string> itaction = attrf->second.begin().begin();
        string actionstr = simplifyString(*(itaction.begin()));
        if((actionstr) == "ALL"){
            SubSet= PetriTransitions;
        }else{
            for(treeSI it2 = itaction.begin(); it2!=itaction.end();++it2){
                if ((*it2) == "actionName") {
                    string actionstr2 = simplifyString(*(it2.begin()));
                    if((P.verbose-3)>0)cout << "Action: " << actionstr2 << endl;
                    regex actionreg(".*"+actionstr2+".*");
                    size_t countmatch = 0;
                    for(const auto &acs : PetriTransitions)
                        if(regex_match(acs, actionreg)){
                            countmatch++;
                            SubSet.insert(acs);
                            if((P.verbose-3)>0)cout << "action:" << acs << " matches " << actionstr2 << endl;
                        }
                    if(countmatch==0){
                        cerr << "No match for action name: "<< actionstr2 <<endl;
                    }
                }
            }
        }
    }
    attrf = attributes.find("action");
    if (attrf != attributes.end()) {
        treeSI itaction = attrf->second.begin();
        for(treeSI it2 = itaction.begin(); it2!=itaction.end();++it2){
            if ((*it2) == "actionName") {
                string actionstr2 = simplifyString(*(it2.begin()));
                if((P.verbose-3)>0)cout << "Action: " << actionstr2 << endl;
                regex actionreg(".*"+actionstr2+".*");
                size_t countmatch = 0;
                for(const auto &acs : PetriTransitions)
                    if(regex_match(acs, actionreg)){
                        countmatch++;
                        SubSet.insert(acs);
                        if((P.verbose-3)>0)cout << "action " << acs << " matches " << actionstr2 << endl;
                    }
                if(countmatch==0){
                    cerr << "No match for action name: "<< actionstr2 <<endl;
                }
            }
        }
    }
    attrf = attributes.find("allExcept");
    if (attrf != attributes.end()) {
        treeSI itaction = attrf->second.begin();
        SubSet = MyLHA->MyGspn->TransList;
        for(treeSI it2 = itaction.begin(); it2!=itaction.end();++it2){
            if (*it2 == "actionName") {
                string actionstr2 = simplifyString(*(it2.begin()));
                if((P.verbose-3)>0)cout << "All except: " << actionstr2 << endl;
                regex actionreg(".*"+actionstr2+".*");
                size_t countmatch = 0;
                for(const auto &acs : PetriTransitions)
                    if(regex_match(acs, actionreg)){
                        countmatch++;
                        SubSet.erase(acs);
                        if((P.verbose-3)>0)cout << "action " << acs << " matches " << actionstr2 << endl;
                    }
                if(countmatch==0){
                    cerr << "No match for action name: "<< actionstr2 <<endl;
                }
            }
        }
    }
    
    
    
    MyLHA->EdgeActions.push_back(SubSet);
    if(SubSet.size()>0) MyLHA->Out_S_Edges[edge.Source].insert(edge.Index);
    else MyLHA->Out_A_Edges[edge.Source].insert(edge.Index);
    
    bool markdep=false;
	
	treeSI itflow = attributes.find("updates")->second.begin();
	vector<string> v1(MyLHA->NbVar,"");
	if (*(itflow.begin()) == "update") {
		for(treeSI it2 = itflow.begin(); it2!=itflow.end();++it2){
			string var;
			string varflow;
			//cout << "var update:" << endl;
			for(treeSI it3 = it2.begin(); it3!=it2.end();++it3){
				if((*it3) == "name")var = simplifyString(*(it3.begin()));
				if((*it3) == "expr")eval_expr(&markdep, &varflow, it3.begin() );
			}
			size_t vi = find(MyLHA->Vars.label.begin(), MyLHA->Vars.label.end(), var) - MyLHA->Vars.label.begin();
			v1[vi]= varflow;
			if((P.verbose-3)>1 && !var.empty())
				cout << "\tvar: " << var << " index: " << vi << " update: " << varflow << endl;
			
		}
	}
	MyLHA->FuncEdgeUpdates.push_back(v1);
	
	treeSI itguard = attributes.find("guard")->second.begin();
	if((P.verbose-3)>2)cout << "guard:" << endl;
	vector<vector<string> > CoeffsMatrix;
	vector<string> CST;
	vector<string> comp;
	if(SubSet.size()==0){
		eval_guard(CoeffsMatrix,CST,comp,itguard.begin().begin());
        stringstream untime;
        untime << "true";
        for (size_t i=0; i< CoeffsMatrix.size(); i++) {
            size_t k=0;
            for (size_t j =0 ; j<CoeffsMatrix[0].size(); j++)
                if(CoeffsMatrix[i][j] !="" && MyLHA->Vars.type[j] != CONTINIOUS_VARIABLE)k++;
            if(k>0){
                untime << "&& (";
                for (size_t j =0 ; j<CoeffsMatrix[0].size(); j++) {
                    if(CoeffsMatrix[i][j]!= "" && MyLHA->Vars.type[j] != CONTINIOUS_VARIABLE)
                        untime << "+" << CoeffsMatrix[i][j]<< "* Vars->"<<MyLHA->Vars.label[j] <<" " ;
                }
                untime << comp[i] << CST[i]<<")";
            }
        }
        if((P.verbose-3)>1){
			cout << "guard:" << untime.str() << endl;
		}
		MyLHA->unTimeEdgeConstraints.push_back(untime.str());
	}else{
		string guard;
		bool markdep;
		eval_expr(&markdep, &guard, itguard.begin().begin());
		MyLHA->unTimeEdgeConstraints.push_back(guard);
	}
	MyLHA->ConstraintsCoeffs.push_back(CoeffsMatrix);
	MyLHA->ConstraintsConstants.push_back(CST);
	MyLHA->ConstraintsRelOp.push_back(comp);
	
	
}
