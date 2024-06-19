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
 * file Gspn-Writer-Color.cpp                                                  *
 * Created by Benoit Barbot on 17/04/15.                                       *
 *******************************************************************************
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "Gspn-Writer-Color.hpp"
#include "../casesWriter.hpp"

using namespace std;

Gspn_Writer_Color::Gspn_Writer_Color(GspnType& mgspn,parameters& Q):Gspn_Writer(mgspn,Q){}

void Gspn_Writer_Color::writeEnabledDisabledBinding(ofstream &SpnF)const{
    SpnF << "const abstractBinding* "<<objName<<"nextPossiblyEnabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {" << endl;
    SpnF << "\tswitch(lastTransition*(tr+1) + targettr){"<< endl;
    for(size_t trit = 0; trit != MyGspn.tr;++trit){
        if(!MyGspn.transitionStruct[trit].varDomain.empty())
            for (size_t trit2= 0; trit2 != MyGspn.tr; ++trit2) {
                set<size_t> varList = MyGspn.transitionStruct[trit2].varDomain;
                if(!varList.empty()){

                    size_t nbp = 0;
                    size_t pivotplace = MyGspn.pl;
                    bool fallback = false;
                    for(size_t itp = 0; itp!=MyGspn.pl; ++itp){
                        //Check that that there is at least one variable on the two arcs
                        if(MyGspn.access(MyGspn.outArcsStruct,trit,itp).isColored
                           && MyGspn.access(MyGspn.inArcsStruct,trit2,itp).isColored
                           ){
                            //Check that there is only one token on each arcs
                            if(MyGspn.access(MyGspn.outArcsStruct,trit,itp).coloredVal.size()==1
                               && MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal.size()==1){
                                //Check that the token is not the ALL token.
                                if(!MyGspn.access(MyGspn.outArcsStruct,trit,itp).coloredVal[0].hasAll &&
                                   !MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].hasAll){
                                    nbp++;
                                    pivotplace = itp;
                                    for(size_t varpt = 0;varpt < MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].field.size(); ++varpt){
                                        if(MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].Flags[varpt] == CT_VARIABLE){
                                            varList.erase(MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].field[varpt].intVal);
                                        }
                                    }
                                }else fallback = true;//Handling of ALL token not yet implemented
                            }else
                                fallback= true; // Handling of several token not yet implemented
                        } else {
                            //Sychronization over an uncolored place
                            nbp++;
                            pivotplace= itp;
                        }
                    }
                    if ( nbp==1 && varList.empty() && !fallback) {
                        // Here we implement only the case with one place one token on the arc
                        // For all other casess fall back to enumeration.
                        SpnF << "\tcase " << trit*(MyGspn.tr+1) + trit2 <<
                        ":\t//" << MyGspn.transitionStruct[trit].name << "->"
                        << MyGspn.placeStruct[pivotplace].name <<
                        "->" << MyGspn.transitionStruct[trit2].name << endl;
                        SpnF << "\t{"<< endl;
                        SpnF << "\t\tif(*bindingNum==1)return NULL;" << endl; //return NULL if it is the second call
                        SpnF << "\t\tsize_t btotal = b.idTotal();" << endl;
                        assert(pivotplace < MyGspn.pl);
                        const auto tok = MyGspn.access(MyGspn.outArcsStruct,trit,pivotplace);
                        assert(tok.coloredVal.size()>0 && tok.coloredVal[0].field.size());
                        const auto cc1 = tok.coloredVal[0];
                        SpnF << "\t\tbtotal += " << ((cc1.varIncrement[0]
                                                      + MyGspn.colClasses[cc1.field[0].intVal].colors.size()) % MyGspn.colClasses[cc1.field[0].intVal].colors.size() ) *
                        varMultiplier(cc1.field[0].intVal) <<  ";"<< endl;
                        //Compute the number of the new binding in the global numerotation.

                        SpnF << "\t\tsize_t bloc = Transition[targettr].bindingLinkTable[btotal];" << endl;
                        SpnF << "\t\tif(bloc==string::npos)return NULL;" << endl;

                        SpnF << "\t\t*bindingNum=1;" << endl;
                        SpnF << "\t\treturn &(Transition[targettr].bindingList[bloc]);" << endl;
                        SpnF << "\t}"<< endl;
                    }else if(fallback){
                        SpnF << "\t\t//Fallback:" << MyGspn.transitionStruct[trit].name << "->" << MyGspn.transitionStruct[trit2].name << endl;
                    } else if(varList.size() < MyGspn.transitionStruct[trit2].varDomain.size()){
                        SpnF << "\t\t//Partial synch over variable: " << MyGspn.transitionStruct[trit].name << "->" << MyGspn.transitionStruct[trit2].name << " var ";
                        for (set<size_t>::const_iterator itset = varList.begin(); itset != varList.end(); ++itset) {
                            SpnF << MyGspn.colVars[*itset].name << ", ";
                        }
                        SpnF << "Not set" <<endl;
                    }
                }
            }
    }
    SpnF << "\tdefault:"<< endl;
    SpnF << "\t\tif(*bindingNum==Transition[targettr].bindingList.size())return NULL;"<<endl;
    SpnF << "\t\t*bindingNum = *bindingNum +1;"<< endl;
    SpnF << "\t\treturn &(Transition[targettr].bindingList[*bindingNum-1]);"<< endl;
    SpnF << "}}"<< endl;

    SpnF << "const abstractBinding* "<<objName<<"nextPossiblyDisabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {" << endl;
    SpnF << "\tswitch(lastTransition*(tr+1) + targettr){"<< endl;

    for(size_t trit = 0; trit != MyGspn.tr;++trit){
        if(!MyGspn.transitionStruct[trit].varDomain.empty())
            for (size_t trit2= 0; trit2 != MyGspn.tr; ++trit2) {
                set<size_t> varList = MyGspn.transitionStruct[trit2].varDomain;
                if(!varList.empty()){

                    size_t nbp = 0;
                    size_t pivotplace= MyGspn.pl;
                    bool fallback = false;
                    for(size_t itp = 0; itp!=MyGspn.pl; ++itp){
                        //Check that that there is at least one variable on the two arcs
                        if(MyGspn.access(MyGspn.inArcsStruct,trit,itp).isColored
                           && MyGspn.access(MyGspn.inArcsStruct,trit2,itp).isColored){
                            //Check that there is only one token on each arcs
                            if(MyGspn.access(MyGspn.inArcsStruct,trit,itp).coloredVal.size()==1
                               && MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal.size()==1){
                                //Check that the token is not the ALL token.
                                if(!MyGspn.access(MyGspn.inArcsStruct,trit,itp).coloredVal[0].hasAll &&
                                   !MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].hasAll){
                                    nbp++;
                                    pivotplace = itp;
                                    for(size_t varpt = 0;varpt < MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].field.size(); ++varpt){
                                        if(MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].Flags[varpt] == CT_VARIABLE){
                                            varList.erase(MyGspn.access(MyGspn.inArcsStruct,trit2,itp).coloredVal[0].field[varpt].intVal);
                                        }
                                    }
                                }else fallback = true;//Handling of ALL token not yet implemented
                            }else {
                                fallback= true; // Handling of several token not yet implemented
                            }
                        }else {
                            //Sychronization over an uncolored place
                            nbp++;
                            pivotplace= itp;
                        }
                    }
                    if ( nbp==1 && varList.empty() && !fallback) {
                        // Here we implement only the case with one place one token on the arc
                        // For all other casess fall back to enumeration.
                        SpnF << "\tcase " << trit*(MyGspn.tr+1) + trit2 <<
                        ":\t//" << MyGspn.transitionStruct[trit].name << "->"
                        << MyGspn.placeStruct[pivotplace].name <<
                        "->" << MyGspn.transitionStruct[trit2].name << endl;
                        SpnF << "\t{"<< endl;
                        SpnF << "\t\tif(*bindingNum==1)return NULL;" << endl; //return NULL if it is the second call
                        SpnF << "\t\tsize_t btotal = b.idTotal();" << endl;
                        assert(pivotplace < MyGspn.pl);
                        const auto tok = MyGspn.access(MyGspn.inArcsStruct,trit,pivotplace);
                        assert(tok.coloredVal.size()>0 && tok.coloredVal[0].field.size());
                        const auto cc1 = tok.coloredVal[0];
                        SpnF << "\t\tbtotal += " << ((cc1.varIncrement[0]
                                                      + MyGspn.colClasses[cc1.field[0].intVal].colors.size()) % MyGspn.colClasses[cc1.field[0].intVal].colors.size() ) *
                        varMultiplier(cc1.field[0].intVal) <<  ";"<< endl;
                        //Compute the number of the new binding in the global numerotation.

                        SpnF << "\t\tsize_t bloc = Transition[targettr].bindingLinkTable[btotal];" << endl;
                        SpnF << "\t\tif(bloc==string::npos)return NULL;" << endl;

                        SpnF << "\t\t*bindingNum=1;" << endl;
                        SpnF << "\t\treturn &(Transition[targettr].bindingList[bloc]);" << endl;
                        SpnF << "\t}"<< endl;
                    }else if(fallback){
                        SpnF << "\t\t//Fallback:" << MyGspn.transitionStruct[trit].name << "->" << MyGspn.transitionStruct[trit2].name << endl;
                    } else if(varList.size() < MyGspn.transitionStruct[trit2].varDomain.size()){
                        SpnF << "\t\t//Partial synch over variable: " << MyGspn.transitionStruct[trit].name << "->" << MyGspn.transitionStruct[trit2].name << " var ";
                        for (set<size_t>::const_iterator itset = varList.begin(); itset != varList.end(); ++itset) {
                            SpnF << MyGspn.colVars[*itset].name << ", ";
                        }
                        SpnF << "Not set" <<endl;
                    }
                }
            }
    }


    SpnF << "\tdefault:"<< endl;
    SpnF << "\t\tif(*bindingNum==Transition[targettr].bindingList.size())return NULL;"<<endl;
    SpnF << "\t\t*bindingNum = *bindingNum +1;"<< endl;
    SpnF << "\t\treturn &(Transition[targettr].bindingList[*bindingNum-1]);"<< endl;
    SpnF << "}}"<< endl;


    /*SpnF << "\tconst abstractBinding* result = &(Transition[targettr].bindingList[*bindingNum]);"<< endl;
     SpnF << "\tif(*bindingNum==Transition[targettr].bindingList.size()-1){*bindingNum= string::npos;}"<<endl;
     SpnF << "\telse{*bindingNum = *bindingNum +1;};"<< endl;
     SpnF << "\treturn result;"<< endl;
     SpnF << "}"<< endl;*/

}

void Gspn_Writer_Color::writeEnabledDisabledBindingSet(ofstream &SpnF)const{
    //SpnF << "const abstMarkIter = new abstractMarkingIterator();";
    SpnF << "const abstractBinding* "<<objName<<"nextPossiblyEnabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {" << endl;
    
    // Switch sur les transitions :
    
    // 1. récupérer les variables d'entrées
    // 2. itérer sur les places d'entrées
    SpnF << "\t\tassert(false);" << endl;
            /*
    SpnF << "\t\tif(*bindingNum==Transition[targettr].bindingList.size())return NULL;"<<endl;
    SpnF << "\t\t*bindingNum = *bindingNum +1;"<< endl;
    SpnF << "\t\treturn &(Transition[targettr].bindingList[*bindingNum-1]);"<< endl;
             */
    SpnF << "}"<< endl;

    SpnF << "const abstractBinding* "<<objName<<"nextPossiblyDisabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {" << endl;
    SpnF << "\t\tassert(false);" << endl;
    /*
    SpnF << "\t\tif(*bindingNum==Transition[targettr].bindingList.size())return NULL;"<<endl;
    SpnF << "\t\t*bindingNum = *bindingNum +1;"<< endl;
    SpnF << "\t\treturn &(Transition[targettr].bindingList[*bindingNum-1]);"<< endl;
    */
    SpnF << "}"<< endl;

}

void Gspn_Writer_Color::printloot(ofstream& fs, const colorDomain& dom, size_t nesting )const{
    //const colorDomain& dom = MyGspn.colDoms[domain];
    if(nesting == dom.colorClassIndex.size()){
        stringstream mult;
        mult << "x.mult";
        if(P.is_domain_impl_set){
            mult << "(";
            for (size_t count = 0 ; count < dom.colorClassIndex.size(); count++ ) {
                mult << (count ==0?"":",") << "c" << count ;
            }
            mult << ")";
        }else for (size_t count = 0 ; count < dom.colorClassIndex.size(); count++ ) {
                mult << "[c" << count << "]";
            }
        
            
        fs << "\t\tif(" << mult.str() << ")\n\t\t";
        fs << "\t\t\toutprintloot << "<< mult.str();
        fs << "<< \"<\"";
        for (size_t count = 0 ; count < dom.colorClassIndex.size(); count++ ) {
            if( count > 0)fs << " << \",\"";
            fs << " << Color_"<< MyGspn.colClasses[dom.colorClassIndex[count]].name << "_names[c" << count << "]";
        }
        fs << "<< \">,\";\n";
        return;
    }
    fs << "\tfor(size_t c" << nesting << " = 0 ; c"<<nesting<<"< Color_";
    fs << MyGspn.colClasses[dom.colorClassIndex[nesting]].name << "_Total; c";
    fs << nesting << "++ )\n";
    printloot(fs, dom, nesting+1);
}


void Gspn_Writer_Color::writeDomainToken(std::ofstream &header, const colorDomain & it)const{
    //token class def
    header << "\nstruct " << it.tokname() << "{\n";
    vector<size_t>::const_iterator itcol;
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        header << "\t" << MyGspn.colClasses[*itcol].cname() << " c" << itcol - it.colorClassIndex.begin() << ";\n";
    }
    header << "\tint mult;\n";
    
    header << "\t" << it.tokname() << "( ";
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        header << " "<<MyGspn.colClasses[*itcol].cname() << " cv" << itcol - it.colorClassIndex.begin()<< " = ("<< MyGspn.colClasses[*itcol].cname() << ")0, " ;
    }
    header << "int v =1) {\n";
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        size_t pos = itcol - it.colorClassIndex.begin();
        header << "\t\tc" << pos << "= cv"<<pos <<";\n";
    }
    header << "\t\tmult = v;\n\t}\n";
    
    
    header << "\t" << it.tokname() << "( ";
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        header << "const "<<MyGspn.colClasses[*itcol].name << "_Token& cv" << itcol - it.colorClassIndex.begin()<< ", " ;
    }
    header << "int v =1) {\n";
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        size_t pos = itcol - it.colorClassIndex.begin();
        header << "\t\tc" << pos << "= cv";
        header << pos << ".c0" << ";\n";
    }
    header << "\t\tmult = v;\n\t}\n";
    
    header << "\t" << it.tokname() << " operator * (size_t v){\n";
    header << "\t\tmult *= v;\n\t\treturn *this;\n\t}\n";
    
    header << "\tvoid print(std::ostream& out) const {\n\t\tout << mult << \"<\" <<";
    for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
        if(itcol != it.colorClassIndex.begin())header << "<< \" , \" << ";
        if( MyGspn.colClasses[*itcol].type == ColorClassType::Continuous ){
            header << "c" << itcol - it.colorClassIndex.begin();
        } else {
            header << " Color_"<< MyGspn.colClasses[*itcol].name << "_names[c" << itcol - it.colorClassIndex.begin() << "]";
        }
    }
    header << " << \">\";\n";
    header << "\t}\n";
    
    if(it.isBounded){
        header << "\tvoid iter() {\n";
        for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
            size_t pos = itcol - it.colorClassIndex.begin();
            header << "\t\tif( c" << pos << "< ("<< MyGspn.colClasses[*itcol].cname() << ")(Color_";
            header << MyGspn.colClasses[*itcol].name << "_Total - 1) )";
            header << "{ c"<< pos << " = ("<< MyGspn.colClasses[*itcol].cname() << ")(c"<< pos <<"+ 1); return;};\n";
            header << "c"<< pos << " = ("<< MyGspn.colClasses[*itcol].cname() << ")(0);\n";
        }
        header << "\t}\n";
        
        if (it.colorClassIndex.size()==1 && MyGspn.colClasses[it.colorClassIndex[0]].type == ColorClassType::Cyclic) {
            header << "\t" << it.tokname() << " next(int i)const {\n";
            header << "\t\t" << it.tokname() << " x(("<< MyGspn.colClasses[it.colorClassIndex[0]].cname() << ")((c0 +i) % Color_";
            header << MyGspn.colClasses[it.colorClassIndex[0]].name << "_Total), ";
            header << " mult);\n\t\treturn x;}\n";
        }
        
        header << "\tbool islast()const {\n\t\treturn (";
        for (itcol = it.colorClassIndex.begin(); itcol != it.colorClassIndex.end() ; ++itcol ) {
            size_t pos = itcol - it.colorClassIndex.begin();
            if (pos > 0)header << " && ";
            header << " c" << pos << "== ("<< MyGspn.colClasses[*itcol].cname() << ")(Color_";
            header << MyGspn.colClasses[*itcol].name << "_Total -1) ";
        }
        header << " );\n\t}\n";
    }
        
    /* header << "\tbool operator > (const " << it.tokname() << " x){\n";
    header << "\t\treturn mult > x.mult ;\n\t}\n"; // Solution temporaire, il faudrait peut-être vérifier que c'est bien des tokens du même type ?
    header << "\tbool operator < (const " << it.tokname() << " x){\n";
    header << "\t\treturn mult < x.mult ;\n\t}\n"; */
    
    header << "\tbool operator > (const int x){\n";
    header << "\t\treturn mult > x ;\n\t}\n";
    header << "\tbool operator < (const int x){\n";
    header << "\t\treturn mult < x ;\n\t}\n";
    
    header << "\tbool operator < (const " << it.tokname() << " &tok) const {\n"; // Comparaison demandée par la structure Map
    for (size_t colIndex=0; colIndex< it.colorClassIndex.size(); colIndex++ ) {
        // Comparaison en ordre lexico
        header << "\t\tif (c" << colIndex << " < tok.c" << colIndex << ") { return true; }\n";
        header << "\t\tif (c" << colIndex << " > tok.c" << colIndex << ") { return false; }\n";
    }
    header << "\t\treturn false;\n";
    header << "\t}\n";
    
    header << "\tbool same_color (const " << it.tokname() << " &tok) const {\n";
    for (size_t colIndex=0; colIndex< it.colorClassIndex.size(); colIndex++ ) {
        header << "\t\tif (c" << colIndex << " != tok.c" << colIndex << ") { return false; }\n";
    }
    header << "\t\treturn true;\n";
    header << "\t}\n";
    

    
    header << "};\n";
    
}

void Gspn_Writer_Color::writeDomainTable(std::ofstream &SpnCppFile , std::ofstream &header, const colorDomain & it)const{
    
    stringstream domaindecl;
    stringstream colorArgsName;
    stringstream colorArrayAccess;
    stringstream colorArrayAccess2;
    stringstream colorArrayAccess3;
    stringstream allCondition;
    stringstream forLoop;
    stringstream colorTypeFor;
    
    for (vector<size_t>::const_iterator it2 = it.colorClassIndex.begin();
         it2 != it.colorClassIndex.end(); ++it2 ) {
        let cc = MyGspn.colClasses[*it2];
        domaindecl << "[ Color_" << cc.name << "_Total ]";
        size_t countCol = it2 - it.colorClassIndex.begin();
        if(countCol > 0){
            colorArgsName << ",";
            allCondition << " && ";
            colorArrayAccess3 << " * ";
        }
	//, Color_"<< cc.name <<"_Total
        colorArgsName << cc.cname() << " c" << countCol;
        colorArrayAccess << "[c" << countCol << "]";
        colorArrayAccess2 << "[i" << countCol << "]";
        if( cc.staticSubclasses.empty() ){
            colorArrayAccess3 << "1";
        } else {
            colorArrayAccess3 << " (x.c" << countCol << " < Color_"<< cc.name << "_All ? 1 : ( "<<cc.name<< "_SubClass_Table[ x.c"<< countCol << " - Color_"<< cc.name << "_All -1 ] ? 1 : 0) ) ";
        }
            
        allCondition << "c" << countCol << " != Color_" << cc.name << "_All";
        
        forLoop << "\t\t\tfor( int i" <<
        countCol <<"= ( c" << countCol << " == Color_" << cc.name << "_All ? 0 : c" << countCol << ");";
        forLoop << "i" << countCol <<"< ( c" << countCol << " == Color_" << cc.name << "_All ? Color_"<< cc.name <<"_Total : c" << countCol << "+1);";
        forLoop << "i" << countCol << "++)\n";
        
        colorTypeFor <<  "\t\tfor( int i" << countCol <<"= ( x.c" << countCol << "< Color_" << cc.name << "_All ? x.c" << countCol <<   " :0 );";
        colorTypeFor <<               "i" << countCol <<"< ( x.c" << countCol << "< Color_" << cc.name << "_All ? x.c" << countCol << "+1 : Color_"<< cc.name <<"_Total);";
        colorTypeFor << "i" << countCol << "++)\n";
    }
    
    header << "struct " << it.cname() << ":";
    
    vector<size_t> domcontains = vector<size_t>();
    for(let it2 : it.colorClassIndex)if( count(domcontains.begin(),domcontains.end(),it2) ==0)domcontains.push_back(it2);
    for (auto it2 = domcontains.begin(); it2 != domcontains.end(); ++it2) header << (it2 == domcontains.begin()?" ":", ") << "contains_" << MyGspn.colClasses[*it2].cname();
    header << " {\n\tint mult" << domaindecl.str() << ";\n";
    header << "\t" << it.cname() << "(size_t v =0) { fill( (int*)mult ,((int*)mult) + sizeof(mult)/sizeof(int), v );}"<< endl;
    header << "\t" << it.cname() << "(" << colorArgsName.str() << ") {\n";
    //header << "\t\t" << "memset(&mult,0 , sizeof(mult));\n";
    header << "\t\t" << "fill( (int*)mult ,((int*)mult) + sizeof(mult)/sizeof(int), 0 );"<< endl;
    header << "\t\t" << "if(" << allCondition.str() << ")\n";
    header << "\t\t\t" << "mult" << colorArrayAccess.str() << " = 1 ;\n";
    header << "\t\telse{\n";
    header << forLoop.str() << "\t\t\t\tmult" << colorArrayAccess2.str() << " = 1 ;\n";
    header << "\t\t}\n";
    header << "\t}\n";
    
    /*header << "\t" << it.cname() << "(" << colorTypeConstructor.str() << "){" << endl;
    header << "\t\tfill( (int*)mult ,((int*)mult) + sizeof(mult)/sizeof(int), 0 );"<<endl;
    header << colorTypeFor.str();
    header << "\t\tmult" << colorArrayAccess2.str() << " = "<< colorArrayAccess3.str() << " ;" << endl;
    header << "\t}" << endl;*/
    
    
//    header << "\tint mult("<<
    header << "\tsize_t copyVector(vector<int> &v ,size_t s)const{\n";
    header << "\t\tcopy((int*)mult,(int*)mult + sizeof(mult)/sizeof(int), v.begin() + s );\n\t\treturn s+sizeof(mult)/sizeof(int);\n\t}\n";
    header << "\tsize_t setVector(const vector<int> &v ,size_t s){\n";
    header << "\t\tcopy(v.begin() + s, v.begin() + s + sizeof(mult)/sizeof(int), (int*)mult );\n\t\treturn s+sizeof(mult)/sizeof(int);\n\t}\n";
    
    
    
    header << "\t" << it.cname() << "& operator = (const " << it.cname() << "& x){\n";
    header << "\t\tcopy((int*)x.mult,(int*)x.mult + sizeof(mult)/sizeof(int),(int*)mult);\n\t\treturn *this;\n\t}\n";
    
    header << "\t" << it.cname() << "& operator = (const " << it.tokname() << "& x){\n";
    header << "\t\t" << "fill( (int*)mult ,((int*)mult) + sizeof(mult)/sizeof(int), 0 );"<< endl;
    header << "\t\t*this += x;\n\t\treturn *this;\n\t}\n";
    
    header << "\tbool operator == (const " << it.cname() << "& x){\n";
    header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult);\n\t}\n";
    
    header << "\tbool operator < (const " << it.cname() << "& x){\n";
    header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult,std::less<int>());\n\t}\n";
    
    header << "\tbool operator > (const " << it.cname() << "& x){\n";
    header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult,std::greater<int>());\n\t}\n";
    
    header << "\t" << it.cname() << " operator * (int v){\n";
    header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++) ((int*)mult)[count]*= v;\n\t\treturn *this;\n\t}\n";
    
    header << "\t" << it.cname() << "& operator += (const " << it.cname() << "& x){\n";
    header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
    header << "\n\t\t\t((int*)mult)[count]+= ((int*)x.mult)[count] ;\n";
    header << "\t\treturn *this;\n\t}\n";
    
    header << "\t" << it.cname() << "& operator += (const " << it.tokname() << "& x){\n";
    header << colorTypeFor.str();
    header << "\t\tmult";
    for (vector<size_t>::const_iterator it2 = it.colorClassIndex.begin();
         it2 != it.colorClassIndex.end(); ++it2 ) {
        header << "[ i" << it2- it.colorClassIndex.begin() << " ]" ;
    }
    header << " += "<< colorArrayAccess3.str() << "* x.mult;\n\t\treturn *this;\n\t}\n";
    
    header << "\t" << it.cname() << " operator + (const " << it.tokname() << "& x) && {\n";
    header << "\t\t"<< it.cname()<< " d(*this);\n\t\td+=x;\n ";
    header << "\t\treturn d;\n\t}\n";
    
    header << "\t" << it.cname() << "& operator -= (const " << it.tokname() << "& x){\n";
    header << colorTypeFor.str();
    header << "\t\tmult";
    for (vector<size_t>::const_iterator it2 = it.colorClassIndex.begin();
         it2 != it.colorClassIndex.end(); ++it2 ) {
        header << "[ i" << it2- it.colorClassIndex.begin() << " ]" ;
    }
    header << " -= "<< colorArrayAccess3.str() << "* x.mult;\n\t\treturn *this;\n\t}\n";
    
    header << "\tbool operator < (const " << it.tokname() << "& x)const{\n";
    header << "\t\treturn mult";
    for (vector<size_t>::const_iterator it2 = it.colorClassIndex.begin();
         it2 != it.colorClassIndex.end(); ++it2 ) {
        header << "[ x.c" << it2- it.colorClassIndex.begin() << " ]" ;
    }
    header << " < x.mult;\n\t}\n";
    
    header << "\tbool operator >= (const " << it.tokname() << "& x)const{\n";
    header << "\t\treturn mult";
    for (vector<size_t>::const_iterator it2 = it.colorClassIndex.begin();
         it2 != it.colorClassIndex.end(); ++it2 ) {
        header << "[ x.c" << it2- it.colorClassIndex.begin() << " ]" ;
    }
    header << " >= x.mult;\n\t}\n";
    
    header << "\t" << it.cname() << " operator + (const " << it.cname() << "& x) &&{\n";
    header << "\t\t"<< it.cname() << " returnval = *this; returnval+= x;\n";
    header << "\t\treturn returnval;\n\t}\n";
    
    header << "\t" << it.cname() << "& operator -= (const " << it.cname() << "& x){\n";
    header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
    header << "\n\t\t\t((int*)mult)[count]-= ((int*)x.mult)[count] ;\n";
    header << "\t\treturn *this;\n\t}\n";
    
    header << "\t" << it.cname() << " operator - (const " << it.cname() << "& x)const{\n";
    header << "\t\t"<< it.cname() << " returnval = *this; returnval-= x;\n";
    header << "\t\treturn returnval;\n\t}\n";
    
    header << "\tint card (void){\n";
    header << "\tint acc=0;\n";
    header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
    header << "\n\t\t\tacc += ((int*)mult)[count] ;\n";
    header << "\t\treturn acc;\n\t}\n";
    
    for( let cci : domcontains){
        let cc = MyGspn.colClasses[cci];
        header << "\tvirtual void apply_perm("<< cc.cname() <<",const std::vector<size_t> &index){\n";
        header << "\t\t"<< it.cname() <<" temp = *this ;\n";
        //header << "\t\tstd::copy( mult, mult + sizeof(mult)/sizeof(int), temp);\n";
        
        stringstream forloop2;
        stringstream accessperm;
        for (auto it2 = it.colorClassIndex.begin(); it2 != it.colorClassIndex.end(); ++it2 ) {
            size_t countCol = it2 - it.colorClassIndex.begin();
            forloop2 << "\t\tfor( int i" << countCol <<"= 0 ; i" << countCol <<"< Color_"<< MyGspn.colClasses[*it2].name <<"_Total ;";
            forloop2 << "i" << countCol << "++)\n";
            if(*it2 == cci){
                accessperm << "[ index[i"<< countCol <<"] ]";
            }else{
                accessperm << "[ i"<< countCol <<" ]";
            }
        }
 
        header << forloop2.str();
        header << "\t\t\tmult" << colorArrayAccess2.str() << " = temp.mult" << accessperm.str();
        header << ";" << endl;
        header << "\t}\n";
    }
    
    for( let cci : domcontains){
        let cc = MyGspn.colClasses[cci];
        header << "\tvirtual int compare("<< cc.cname() <<",int cci,int ccj)const{\n";
        
        stringstream forloop2;
        stringstream accesspermi;
        stringstream accesspermj;
        for (auto it2 = it.colorClassIndex.begin(); it2 != it.colorClassIndex.end(); ++it2 ) {
            size_t countCol = it2 - it.colorClassIndex.begin();
            if(*it2 != cci){
                forloop2 << "\t\tfor( int i" << countCol <<"= 0 ; i" << countCol <<"< Color_"<< MyGspn.colClasses[*it2].name <<"_Total ;";
                forloop2 << "i" << countCol << "++)\n";
            }
            if(*it2 == cci){
                accesspermi << "[ cci ]";
                accesspermj << "[ ccj ]";
            }else{
                accesspermi << "[ i"<< countCol <<" ]";
                accesspermj << "[ i"<< countCol <<" ]";
            }
        }
        
        header << forloop2.str() << "\t\t{"<<endl;;
        header << "\t\t\tif(mult" << accesspermi.str() << " > mult" << accesspermj.str() <<")return 1;" << endl;
        header << "\t\t\tif(mult" << accesspermi.str() << " < mult" << accesspermj.str() <<")return -1;" << endl;
        header << "\t\t}"<< endl;
        
        header << "\t\treturn 0;" << endl;
        header << "\t}\n";
    }
    
    
    header << "};\n";
    //end of domain class definition
    
    
    header << it.cname() << " operator + (const " << it.tokname() << "& t1 ,const " << it.tokname() << "& t2 )\n\n;";
    header << "std::ostream& operator << (std::ostream& out, const " << it.cname() << "& x);" << endl;
    
    SpnCppFile << "" << it.cname() << " operator + (const " << it.tokname() << "& t1 ,const " << it.tokname() << "& t2 ){\n";
    SpnCppFile << "\t"<< it.cname() << " d; d += t1; d+=t2 ;\n";
    SpnCppFile << "\treturn d;\n}\n";
    
    
    SpnCppFile << "std::ostream& operator << (std::ostream& out, const " << it.cname();
    SpnCppFile << "& x) {\n";
    SpnCppFile << "\tstringstream outprintloot;" << endl;
    printloot(SpnCppFile, it, 0);
    SpnCppFile << "\tout << \"(\" << outprintloot.str() << \")\";" << endl;
    SpnCppFile << "\treturn out;\n}\n";
    
    SpnCppFile << "inline bool contains(const "<<it.cname() << "& d1, const " << it.cname() << "& d2){";
    SpnCppFile << "\treturn (d1-d2) > -1;\n";
    SpnCppFile << "}\n";
    
    SpnCppFile << "inline bool contains(const "<<it.cname() << "& d1, const " << it.tokname() << "& tok){";
    SpnCppFile << "\treturn d1 >= tok;\n";
    SpnCppFile << "}\n";
}

void Gspn_Writer_Color::writeDomainSet(std::ofstream &SpnCppFile , std::ofstream &header, const colorDomain & it)const{
    if(it.isBounded){
        header << "typedef IterableDomainGen< " << it.tokname();
    } else header << "typedef DomainGen< " << it.tokname();
    /* for (let it2 : it.colorClassIndex) header << (it2==it.colorClassIndex[0]?" ":", ") << "contains_" << MyGspn.colClasses[it2].cname(); */
    header << "> " << it.cname() << ";" << endl;
    
    SpnCppFile << "inline bool contains(const "<<it.cname() << "& d1, const " << it.cname() << "& d2){";
    //SpnCppFile << "\treturn (d1-d2) > -1;\n";
    SpnCppFile << "\treturn d1 >= d2;\n";
    SpnCppFile << "}\n";
    
    SpnCppFile << "inline bool contains(const "<<it.cname() << "& d1, const " << it.tokname() << "& tok){";
    SpnCppFile << "\treturn d1 >= tok;\n";
    SpnCppFile << "}\n";
    
}

void Gspn_Writer_Color::writeNextBindingSet(std::ofstream &, std::ofstream &header)const{
    using namespace text_output;
    header << "#include <boost/functional/hash.hpp>" <<endl;
    header << "class abstractBindingIteratorImpl {\n";
    header << "public:\n";
    header << "\tbool isValid = false;"<< endl;
    const auto macroVar = MyGspn.computeMacroVar();
    
    for (let var : macroVar) {
        const auto &domain = MyGspn.colDoms[var.second.second];
        header << "//iterator on: "<< var.first << " over domain "<< domain.name << endl;
        header << "\tmap<"<< domain.tokname() << ", unsigned int>::const_iterator itMacroVar_"<< var.second.first << ";\n";
    }
        
    for (let var : MyGspn.colVars) {
        let classname = MyGspn.colDoms[var.type];
        let colclass = MyGspn.colClasses[classname.colorClassIndex[0]];
        header << "\t"<< colclass.cname() <<" itVar_" << var.name << " =";
        if(colclass.type != ColorClassType::Continuous){
            header << "Color_" << colclass.name << "_" << colclass.colors[0].name << " ;"<< endl;
        } else {
            header << "0; " << endl;
        }
    }
    
    header << "\nprivate:";
    
    casesHandler bindingcases("t");
    casesHandler isCoherentCases("t");
    casesHandler resetCases("t");
    
    for (size_t t = 0; t < MyGspn.tr ; t++){
        stringstream nextcase;
        stringstream coherentcase;
        stringstream resetcase;
        
        let trans = MyGspn.transitionStruct[t];
        
        //varlist is the set of all variable used by the transition.
        auto varlist = set<size_t>(trans.varDomain);
        
        //for each marcro variable i.e. a token shape :
        for (let tokmap : macroVar) {
            let tokid= tokmap.second.first;
            //let vardomain = MyGspn.colDoms[varmap.second.second];
            
            //currtok is a coloured tocken of the net.
            let currtok = tokmap.first;
            
            //varSet is the set of variable used in the tocken
            const auto varSet = currtok.varSet();
            
            //We compute the intersection of this macro var with the set of all variables
            vector<size_t> inter(varSet.size());
            vector<size_t>::iterator it;
            it=set_intersection(varSet.begin(),varSet.end(),varlist.begin(),varlist.end(),inter.begin());
            inter.resize(it - inter.begin());
            
            //if the intersection is empty skip this macro var
            if(  !MyGspn.isInVar(currtok, t) ||  inter.empty() )continue;
            //remove variable from the list of variable.
            for(let v:inter)varlist.erase(v);
                
            nextcase << "//chosing Macro Var " << tokid << " -> " << currtok << endl;
            
            auto leading_place = MyGspn.placeStruct.size();
            //On choisit une place sur laquelle on iter.
            for (let place : MyGspn.placeStruct){
                //arcmult is a set of token labelling arc place.id -> t
                let arcmult = MyGspn.access(MyGspn.inArcsStruct, t, place.id);
                
                //The labelling of the arc contain the macrovar currtok
                if ( any_of(arcmult.coloredVal.begin(), arcmult.coloredVal.end(), [&](const coloredToken& tok){return tok==currtok;})) {
                    leading_place = place.id;
                    nextcase << "// place " << place.name << " is leading for " << currtok << endl;
                    
                    resetcase << "\t\t\titMacroVar_" << tokid << " = m._PL_" << place.name << ".tokens.begin();" << endl;
                    resetcase << "\t\t\tisValid &= itMacroVar_" << tokid << " != m._PL_" << place.name << ".tokens.end();" << endl;
                    
                    //nextcase << "\t\t\tif (itMacroVar_" << tokid << " == m._PL_" << place.name << ".tokens.end())return false;" << endl;
                    nextcase << "\t\t\titMacroVar_" << tokid << "++;" << endl;
                    nextcase << "\t\t\tif (itMacroVar_" << tokid << " == m._PL_" << place.name << ".tokens.end()) {" << endl;
                    nextcase << "\t\t\t\titMacroVar_" << tokid << " = m._PL_" << place.name << ".tokens.begin();" << endl;
                    nextcase << "\t\t\t} else {" << endl;
                    stringstream updatevar;
                    for(size_t i=0; i< currtok.field.size();i++ )if(currtok.Flags[i]==CT_VARIABLE){
                        let cvar = MyGspn.colVars[currtok.field[i].intVal];
                        let classname = MyGspn.colDoms[cvar.type];
                        let colclass = MyGspn.colClasses[classname.colorClassIndex[0]];
                        updatevar << "\t\t\titVar_" << cvar.name << "= (" << colclass.cname() ;
                        updatevar << ") (itMacroVar_" << tokid << "->first.c" << i << " - ";
                        updatevar << currtok.varIncrement[i] << ");\n";
                    }
                    nextcase << "\t" << updatevar.str();
                    resetcase << updatevar.str();
                    nextcase << "\t\t\t\treturn true;"<< endl;
                    nextcase << "\t\t\t}" << endl;

                    nextcase << updatevar.str();
                    
                    
                    break;
                }
            }
            
            for (let place : MyGspn.placeStruct)if(place.id != leading_place){
                let vartemp = MyGspn.access(MyGspn.inArcsStruct, t, place.id);
                if (vartemp.containsVar(tokid)) {
                    for(let tok : vartemp.coloredVal){
                        coherentcase << "// check coherence for "<< currtok << " on "<< place.name << " with token ";
                        text_output::operator<<(coherentcase, tok) << endl;
                    }
                }
            }
        }
        
        //If no leading place iterate over the color class
        for( let varid : varlist ){
            auto& currvar = MyGspn.colVars[varid];
            const auto& vardom = MyGspn.colDoms[currvar.type];
	    if(vardom.isBounded){
	      nextcase << "\n\t\t\tif (itVar_" << currvar.name << " != (((size_t) Color_" << vardom.name << "_Total) - 1) ) { ";
	      nextcase << "\n\t\t\t\titVar_" << currvar.name << "= ("+vardom.name +"_Color_Classe)(itVar_" << currvar.name << "+1); return true;";
	      nextcase << "\n\t\t\t}";
	      nextcase << "\n\t\t\titVar_" << currvar.name << " = ("+vardom.name +"_Color_Classe)0;";
	    }
        }
        
        resetcase << "if( !isCoherent(t,m)){ next(t,m); }; " << endl;
        
        nextcase << "\n\t\t\treturn false;";
        bindingcases.addCase(t, nextcase.str(),MyGspn.transitionStruct[t].name);
        isCoherentCases.addCase(t, coherentcase.str(),MyGspn.transitionStruct[t].name);
        resetCases.addCase(t, resetcase.str(),MyGspn.transitionStruct[t].name);
    }
    header << "\n\tbool nextInterieur(size_t& t,const abstractMarkingImpl& m) {\n";
    header << bindingcases;
    header << "\nreturn false;";
    header << "\n\t}"<<endl;
    
    header << "public:"<< endl;
    header << "\tbool operator==(const abstractBindingIteratorImpl &other) const {"<< endl;
    header << "\t\tif(isValid != other.isValid)return false;" << endl;
    //all invalid iterator are equal
    header << "\t\tif(!isValid && !other.isValid)return true;" << endl;
    for (let var : macroVar) {
        header << "\t\tif(itMacroVar_"<< var.second.first << " != other.itMacroVar_"<< var.second.first << ")return false;"<< endl;
    }
    header << "\t\treturn true;\n\t}"<< endl;
    header << "\tbool operator!=(const abstractBindingIteratorImpl &other)const {"<<endl;
    header << "\t\treturn !(*this == other);"<< endl;
    header << "\t}"<< endl;
    
    header << "\tvoid reset(size_t& t,abstractMarkingImpl& m){"<< endl;
    header << "\t\tisValid = true;"<< endl;
    if (P.verbose > 4) {
        header << "\n\t\tcerr << \"abstractBindingIteratorImpl has been reset for transition.\\n\";"<<endl;
    }
    for (let var : MyGspn.colVars) {
        let classname = MyGspn.colDoms[var.type];
        let colclass = MyGspn.colClasses[classname.colorClassIndex[0]];
	if(colclass.type != Continuous)
	  header << "\t\titVar_"
	       << var.name
	       << " = Color_"
	       << colclass.name
	       << "_"
	       << colclass.colors[0].name
	       << " ;"
	       << endl;
        
    }
    
    header << resetCases;
    header << "\t}"<< endl;
    
    //header << "\nprivate:\n";
    
    //    header << "\tsize_t _ITVAR_" << var.name << ";\n";
    //    header << "\tbool _ISDEFITVAR_" << var.name << ";\n";
    header << "private:"<< endl;
    header << "\tstatic const abstractBindingIteratorImpl endIt();"<< endl;
    header << "\n\tbool isCoherent(size_t& t,const abstractMarkingImpl& m) {" << endl;
    /*for (let var : MyGspn.colVars) {
        header << "\t\tIsDefItVar_" << var.name << " = false;" << endl;
	}*/
    casesHandler bindingcasesB("t");
    for (size_t t = 0; t < MyGspn.tr ; t++){
        stringstream newcase;	
        // liste des variables utilisé par la transition.
        const auto& trans = MyGspn.transitionStruct[t];
        auto varlist = trans.varDomain;
        
        vector<bool> isVisited(MyGspn.placeStruct.size(),false);
        /* for (let var : varlist) {
         auto& currvar = MyGspn.colVars[var];
         bool isUsed = false;
         for (let place : MyGspn.placeStruct) {
         if (isUsed) { continue; }
         isUsed = true;
         }
         //if (not isUsed) { newcase << "\t _ISDEFITVAR_" << currvar.name << " = true;\n"; }
         } */
        
        // TODO : sortir le _ISDEFITVAR_ du code généré vers le code générateur
	/* TO CHECK if NEEDED
        for (let place : MyGspn.placeStruct) {
            bool placeVue = false;
            let vartempB = MyGspn.access(MyGspn.inArcsStruct, t, place.id).coloredVal;
            for (const auto& vartemp : vartempB) {
                if (not placeVue) {
		  if(place.colorDom != UNCOLORED_DOMAIN){
                    newcase << "\n\t\t\tif (m._PL_" << place.name << ".tokens.empty()) { return false; }";
		  } else {
		    newcase << "\n\t\t\tif (m._PL_" << place.name << "==0) { return false; }";
		  }
		  placeVue = true;
                }
                for (size_t varnum = 0;varnum < vartemp.field.size();varnum++) {
                    //for (let var : vartemp) {
                    if (not (vartemp.Flags[varnum] == CT_VARIABLE)) {
                        cerr << "Un élément de l'arc n'est pas une variable" << endl;
                        exit(EXIT_FAILURE);
                    }
                    auto& currvar = MyGspn.colVars[vartemp.field[varnum].intVal];
                    newcase << "\n\t\t\tif (IsDefItVar_" << currvar.name << ") {";
                    newcase << "\n\t\t\t\tif (not (ItVar_" << currvar.name << " == (*ItPl_" << place.name << ").first.c" << varnum << ")) { return false; }";
                    // Cas où la variable est déjà définie
                    newcase << "\n\t\t\t}";
                    newcase << "\n\t\t\telse {";
                    // _IT_place : token ds un Domain
                    newcase << "\n\t\t\t\tItVar_" << currvar.name << " = (*ItPl_" << place.name << ").first.c" << varnum << ";";
                    newcase << "\n\t\t\t\tIsDefItVar_" << currvar.name << " = true;";
                    newcase << "\n\t\t\t\t}";
                }
            }
	    }*/
	if(trans.guard.is_concrete() && trans.guard.boolVal){
	  newcase << "\n\t\t\treturn true;";
	} else {
	 
	  newcase << "\n\t\t\treturn " << trans.guard << ";";
	}
        bindingcasesB.addCase(t, newcase.str(),MyGspn.transitionStruct[t].name);
    }
    // Lister les variables, mettre des valeurs "tentatives" en parcourant les valeurs actuelles
    // des différents itérateurs. Si échec/Collision => forcer le next
    //bindingcasesB.writeCases(header);
    header << bindingcasesB << endl;
    header << "\n\t}";
    
    header << "\npublic:"<<endl;
    
    header << "\tvoid next(size_t& t,const abstractMarkingImpl& m){"<<endl;;
    //SpnCppFile << "\nbool abstractBindingIteratorImpl::next(size_t& t, abstractMarkingImpl& m) {";
    header << "\t\tbool is_coherent = false;"<<endl;
    if (P.verbose > 4) {
        header << "\n\t\tcerr << \"abstractBindingIteratorImpl::next has been called.\\n\";";
        header << "\n\t\tcerr << \"[Call Values :] Found New ?\" << isValid << \" Is Coherent?\" << is_coherent << \"\\n\";"<< endl;
    }
    header << "\t\twhile (( !is_coherent) && isValid) {"<<endl;
    header << "\t\t\tisValid = nextInterieur(t,m);"<<endl;
    header << "\t\t\tis_coherent = isCoherent(t,m);"<<endl;
    if (P.verbose > 4) {
        header << "\t\t\tcerr << \"Found New ?\" << isValid << \" Is Coherent?\" << is_coherent << \"\\n\";"<<endl;
    }
    header << "\t\t}"<<endl;
    header << "\t\tisValid &= is_coherent;\n"<<endl;
    // Tant que c'est pas cohérent, je pop le next itérateur
    header << "\t}"<<endl;
    
    // Créer une fonction qui génère concreteBinding ?
    header << "\n\tsize_t getIndex(){"<<endl;
    //SpnCppFile << "size_t abstractBindingIteratorImpl::getIndex() {";
    header << "\t\tsize_t accum = 0;"<<endl;
    for (let var : MyGspn.colVars) {
        size_t varclass = var.type;
        let vardom = MyGspn.colDoms[varclass];
        let varcc = MyGspn.colClasses[vardom.colorClassIndex[0]];
        if(varcc.type != ColorClassType::Continuous ){
            header << "\t\tboost::hash_combine(accum, itVar_" << var.name << " );"<<endl;
        }else {
            header << "\t\tboost::hash_combine(accum, itVar_" << var.name << ");"<<endl;
        }
    }
    header << "\t\treturn accum;"<< endl;;
    header << "\t}"<<endl << endl;
    
    header << "\tabstractBinding getBinding(){"<< endl;
    //header  << "abstractBinding abstractBindingIteratorImpl::getBinding() {";
    header  << "\t\tabstractBinding newBind;" << endl;
    for (let var : MyGspn.colVars) {
        size_t varclass = var.type;
        const auto& classname = MyGspn.colDoms[varclass].name;
        header  << "\t\tnewBind.P->" << var.name << ".c0 = (" << classname << "_Color_Classe) itVar_" << var.name << ";"<< endl;
    }
    header  << "\t\tnewBind.idcount = getIndex();"<<endl;;
    header  << "\t\treturn newBind;"<< endl;
    header  << "\t}"<<endl;
    
    
    header << "};\n";
}

void Gspn_Writer_Color::writeMarkingClasse(ofstream &SpnCppFile,ofstream &header)const{
    for(let it : MyGspn.colClasses){
        switch (it.type) {
            case ColorClassType::Cyclic:
            case ColorClassType::Asymmetric:
                header << "enum " << it.cname() << ": int {\n";
                for (let it2: it.colors ) {
                    header << "\tColor_" << it.name << "_" << it2.name << ",\n";
                }
                header << "\tColor_" << it.name << "_Total,\n";
                header << "\tColor_"<< it.name << "_All\n";
                for( let css : it.staticSubclasses){
                    header << ",\n\tColor_" << it.name << "_SubClass_" << css.name;
                }
                header << "\n};\n";

                header << "extern const char *Color_"<< it.name << "_names[];\n";
                break;
                
            case ColorClassType::Continuous:
                header << "typedef double "<< it.cname() << ";" << endl;
                break;
        }
        header << "struct contains_"<< it.cname() << "{"<< endl;
        header << "\tvirtual void apply_perm("<< it.cname() << ",const std::vector<size_t> &index)=0;"<<endl;
        header << "\tvirtual int compare("<< it.cname() << ",int,int) const =0;"<<endl;
        header << "};"<< endl;
        
        for( const auto &css : it.staticSubclasses){
            header << "static const bool " << it.name << "_SubClass_" << css.name;
            header << " [Color_" << it.name << "_Total]= {";
            for( auto it2 = it.colors.begin(); it2 != it.colors.end(); ++it2){
                if(it2 != it.colors.begin()) header << ",";
                
                header << (count_if(css.colors.begin(),css.colors.end(),[&](const color & c){return (it2->id == c.id);  } )>0);
            }
            header << " };" << endl;
        }
        header << "static const bool*" << it.name << "_SubClass_Table[ " << it.staticSubclasses.size()<<"] = {" ;
        for( auto css =it.staticSubclasses.begin() ; css != it.staticSubclasses.end(); ++css){
            if(css != it.staticSubclasses.begin())header << ",";
            header << it.name << "_SubClass_" << css->name;
        }
        header << "};" <<endl;
        
        
        header << "inline bool contains(" << it.cname() << " c , const bool sub[]){ return sub[(int)c]; }" << endl;
        
    }
    
    header << "#include \"marking.hpp\"" << endl;
    header << "#include \"markingTemplate.hpp\"" << endl;
    
    for (vector<colorDomain>::const_iterator it = MyGspn.colDoms.begin()+1;
         it != MyGspn.colDoms.end(); ++it ) {
        writeDomainToken(header, *it);
        if (P.is_domain_impl_set) { writeDomainSet(SpnCppFile, header, *it); }
        else { writeDomainTable(SpnCppFile, header, *it ); }
    }
    
    Gspn_Writer::writeMarkingClasse(SpnCppFile, header);
    
    if (P.is_domain_impl_set) {
        writeNextBindingSet(SpnCppFile, header);
    }else {
        header << "\nclass abstractBindingIteratorImpl {";
        header << "\npublic:";
        header << "\n\tconst static bool isValid =false;";
        header << "\n\tvoid reset(size_t,const abstractMarkingImpl& m){};";
        header << "\n\tvoid next(size_t& t,const abstractMarkingImpl& m){};";
        header << "\n\tsize_t getIndex(){return 0;};";
        header << "\n\tabstractBinding getBinding(){return abstractBinding();};";
        header << "\n};\n";
    }
    
    SpnCppFile << "\nvoid abstractBindingIterator::reset(size_t t,const abstractMarking& m) {";
    SpnCppFile << "\n\tP->reset(t,*(m.P));";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nbool abstractBindingIterator::isValid()const {";
    SpnCppFile << "\n\treturn P->isValid;";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nabstractBindingIterator::abstractBindingIterator(const abstractMarking& m) {";
    SpnCppFile << "\n\tP = new abstractBindingIteratorImpl;";
    SpnCppFile << "\n\t//reset(m);";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nvoid abstractBindingIterator::next(size_t& t,const abstractMarking& m) {";
    SpnCppFile << "\n\tP->next(t,*(m.P));";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nabstractBindingIterator::~abstractBindingIterator() {";
    SpnCppFile << "\n\tdelete(P);";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nsize_t abstractBindingIterator::getIndex() {";
    SpnCppFile << "\n\treturn P->getIndex();";
    SpnCppFile << "\n};";
    
    SpnCppFile << "\nabstractBinding abstractBindingIterator::getBinding() {";
    SpnCppFile << "\n\treturn P->getBinding();";
    SpnCppFile << "\n};\n";
}

