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

#include "Lha-Reader.hpp"
#include "Lha_gmlparser.hpp"
#include "../casesWriter.hpp"

#include "string.h"

#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <list>




using namespace std;

LhaType::LhaType(GspnType& Mspn) : MyGspn(&Mspn),NbLoc(0),isDeterministic(true) {
	for (auto it= MyGspn->IntConstant.begin(); it!= MyGspn->IntConstant.end() ; ++it){
		LhaRealConstant[it->first]=it->second;
		LhaIntConstant[it->first]=it->second;
	}
	for (auto it= MyGspn->RealConstant.begin(); it!= MyGspn->RealConstant.end() ; ++it){
		LhaRealConstant[it->first]=it->second;
	}
}


Lha_Reader::Lha_Reader(GspnType& mspn,parameters &Q) : MyLha(mspn),P(Q){
    trace_scanning = false;
    trace_parsing = false;

}

Lha_Reader::~Lha_Reader() {
}

string Lha_Reader::funDecl(const string& funtype) const{
    return "template<class DEDState>\n"+funtype+ " LHA<DEDState>::";
}

int Lha_Reader::parse(string& expr) {
    scan_expression(expr);

    lha::Lha_parser parser(*this);

    parser.set_debug_level(trace_parsing);

    int res = parser.parse();
    scan_end();
    return res;
}

int Lha_Reader::parse_file(parameters& P) {
    string str;
	MyLha.ConfidenceLevel=P.Level;

    ifstream file(P.PathLha.c_str(), ios::in);
    if (file) {


        while (!file.eof()) {

            string str2;
            getline(file, str2);
            str = str + "\n" + str2;
        }

        int x = parse(str);
        if (x) cout << "Parsing LHA description file failed" << endl;


        return x;
    } else {
        cout << "Can't open : " << P.PathLha << endl;
        return 1;
    }
}
int Lha_Reader::parse_gml_file(parameters& P) {
	MyLha.ConfidenceLevel=P.Level;
    ifstream ifile(P.PathLha.c_str());
    if(ifile){
        //cout << "parse GML:" << filename << endl;
        ModelHandlerPtr handlerPtr(new MyLhaModelHandler(&MyLha,P));
        ExpatModelParser parser = ExpatModelParser(handlerPtr);
        parser.parse_file(P.PathLha);
        //cout << "end parse GML"<< endl;
        return 0;
    }else{
        cout << "File " << P.PathLha << " does not exist!" << endl;
        exit(EXIT_FAILURE);
    }


}

bool is_simple(const string &s){
    return (s.find(" ") == string::npos
            && s.find("+") == string::npos
            && s.find("*") == string::npos
            && s.find("/") == string::npos
            && s.find("-") == string::npos
            && s.find(",") == string::npos
            && s.find("(") == string::npos
            && s.find(")") == string::npos);
}

string Lha_Reader::InvRelOp(const string& str)const {
    if (str == "<=") return ">=";
    if (str == ">=") return "<=";
	cerr << "Fail to inverse RelOp"<< endl;
	exit(EXIT_FAILURE);
}

void
Lha_Reader::error(const lha::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void
Lha_Reader::error(const std::string& m) {
    std::cerr << m << std::endl;
}

void Lha_Reader::view() {


}

void Lha_Reader::WriteFile(parameters& P)const {
	string Pref = P.tmpPath;

    string loc;

    //loc = Pref + "../SOURCES/Cosmos/LHA.cpp";
    loc = Pref + "/LHA.cpp";

	ofstream LhaCppFile(loc.c_str(), ios::out | ios::trunc);

	LhaCppFile << "#include <iostream>" << endl;
	LhaCppFile << "#include <algorithm>" << endl;
	LhaCppFile << "#include <vector>" << endl;

	LhaCppFile << "using namespace std;" << endl;

    LhaCppFile << "#include \"markingImpl.hpp\"" << endl;
    if(P.modelType == External || P.modelType == GSPN_Simulink){
         LhaCppFile << "#include \"stateImpl.hpp\"" << endl;
    }
	LhaCppFile << "#include <math.h>" << endl;
	LhaCppFile << "#include <float.h>" << endl;
	LhaCppFile << "#include \"LHA.hpp\"" << endl;

    for (let it : MyLha.LhaRealConstant) {
		LhaCppFile << "    const double " << it.first << "=" << it.second << ";" << endl;
	}

    LhaCppFile << "namespace hybridVar {\n";
    for (let it : MyLha.LhaRealHybrid) {
      if (MyLha.LhaIntHybrid.find(it.first)!=MyLha.LhaIntHybrid.end()) {
        LhaCppFile << "\textern int " << it.first << ";\n";
      }
      else {
        LhaCppFile << "\textern double " << it.first << ";\n";
      }
    }
    LhaCppFile << "};\n";

	LhaCppFile << "struct Variables {\n";
	for(size_t v =0 ; v< MyLha.Vars.type.size(); v++){
		if(MyLha.Vars.type[v] == COLOR_VARIABLE){
			LhaCppFile << "\t" << MyLha.MyGspn->colClasses[MyLha.Vars.colorDomain[v]].cname() << " " << MyLha.Vars.label[v] << ";\n";
        }else if(MyLha.Vars.type[v] == INT_INDEXED_DISC_ARRAY){
            LhaCppFile << "\tdouble "<< MyLha.Vars.label[v];
            LhaCppFile << "[ " << MyLha.Vars.colorDomain[v] << " ];" << endl;
		}else if(MyLha.Vars.colorDomain[v] != UNCOLORED_DOMAIN){
			LhaCppFile << "\tdouble "<< MyLha.Vars.label[v];
			colorDomain coldom = MyLha.MyGspn->colDoms[MyLha.Vars.colorDomain[v]];

			for (vector<size_t>::const_iterator itcol = coldom.colorClassIndex.begin();
				 itcol != coldom.colorClassIndex.end(); ++itcol ) {
				LhaCppFile << "[ Color_" << MyLha.MyGspn->colClasses[*itcol].name << "_Total ]";
			}
			LhaCppFile << ";\n";

		} else LhaCppFile << "\tdouble "<< MyLha.Vars.label[v] << ";\n";
	}
	LhaCppFile << "};\n";

    LhaCppFile << "bool varOrder(const Variables &v1,const Variables &v2){\n";
    for(size_t v =0 ; v< MyLha.Vars.type.size(); v++){
        if(MyLha.Vars.type[v] == COLOR_VARIABLE){
            LhaCppFile << "\tcerr << \"Not yet supported\";\n";
        }else if(MyLha.Vars.type[v] == INT_INDEXED_DISC_ARRAY){
            LhaCppFile << "\tfor(unsigned int i =0; i<" << MyLha.Vars.colorDomain[v] << ";i++)";
            LhaCppFile << "if(v1."<<MyLha.Vars.label[v]<<"[i]<v2."<<MyLha.Vars.label[v]<<"[i])return true;"<< endl;
        } else LhaCppFile << "\tif(v1."<<MyLha.Vars.label[v]<<"<v2."<<MyLha.Vars.label[v]<<")return true;"<< endl;
    }
    LhaCppFile << "\treturn false;\n};\n";


	LhaCppFile << funDecl("void") << "resetVariables(){\n";
	for(size_t v= 0 ; v < MyLha.Vars.type.size(); v++){
		if(MyLha.Vars.type[v] == COLOR_VARIABLE){
			LhaCppFile << "\tVars->" << MyLha.Vars.label[v] << "= Color_" << MyLha.MyGspn->colClasses[MyLha.Vars.colorDomain[v]].name << "_Total ;\n";
		}else if(MyLha.Vars.colorDomain[v]==UNCOLORED_DOMAIN)
			LhaCppFile << "\tVars->"<< MyLha.Vars.label[v] << "= "<< MyLha.Vars.initialValue[v]<<";\n";
		else LhaCppFile << "\tmemset(Vars->"<< MyLha.Vars.label[v] << ",0 , sizeof(Vars->"<< MyLha.Vars.label[v] << "));\n";
	}
	LhaCppFile << "};\n";

	LhaCppFile << funDecl("void") << "printHeader(ostream &s)const{\n";
	LhaCppFile << "\ts << \"\tLocation\\t";
    if(P.StringInSpnLHA){
        for(size_t v= 0 ; v < MyLha.Vars.type.size(); v++)
            if(MyLha.Vars.isTraced[v])LhaCppFile << MyLha.Vars.label[v] << "\\t";
	}
	LhaCppFile << "\";\n";
	LhaCppFile << "};\n";

	LhaCppFile << funDecl("void") << "printState(ostream &s){\n";
	LhaCppFile << "\ts << \"\\t\" << LocLabel[CurrentLocation] << \"\\t\";\n";
    if(P.StringInSpnLHA){
        for(size_t v= 0 ; v < MyLha.Vars.type.size(); v++)
            if(MyLha.Vars.isTraced[v]){
                if(MyLha.Vars.type[v] == INT_INDEXED_DISC_ARRAY){
                    LhaCppFile << "\ts << \"[\";" <<endl;
                    LhaCppFile << "\tfor(unsigned int i=0; i<"<< MyLha.Vars.colorDomain[v] <<"; i++)";
                    LhaCppFile << "s << Vars->"<< MyLha.Vars.label[v] << "[i] << \", \";" << endl;
                    LhaCppFile << "s <<\"] \\t\";\n";
                }else{
                    LhaCppFile << "\ts << Vars->"<< MyLha.Vars.label[v] << " << \"\\t\";\n";
                }
            }
    }
	LhaCppFile << "};\n";


    vector<vector<list<int > > > accedge(MyLha.NbLoc,vector<list<int> >(MyLha.TransitionIndex.size()));
    for (size_t es =0 ; es < MyLha.NbLoc; es++)
        for (auto it : MyLha.TransitionIndex)
            for (size_t e = 0; e < MyLha.Edge.size(); e++)
                if(MyLha.Edge[e].Source == es
                   && MyLha.EdgeActions[e].count(it.first)>0)
                    accedge[es][it.second].push_back(e);

    stringstream accstr;
    size_t maxset =0;
    accstr << "{"<< endl<< "\t";
    for (auto &it1 : accedge){
        for(auto &it2 : it1){
            accstr << it2.size() << " ,";
            maxset = max(maxset,it2.size());
        }
    }
    while (maxset>0){
        accstr << endl << "\t";
        maxset=0;
        for (auto &it1 : accedge){
            for(auto &it2 : it1){
                if(it2.size()>0){
                    accstr << it2.front() << " ,";
                    it2.pop_front();
                }else accstr << "-1,";
                maxset = max(maxset,it2.size());
            }
        }
    }
    accstr << "}";



    //LhaCppFile << "/*" << accstr.str() << "*/" << endl;


    LhaCppFile << funDecl("const int") << "ActionEdgesAr[] = " << accstr.str() << ";"<< endl;
    /*LhaCppFile << "const int* LHAActionEdgesAr2["<< MyLha.NbLoc
     <<"]["<< MyLha.TransitionIndex.size() <<"] = " << accset3.str() << ";"<< endl;
     LhaCppFile << "const int* LHAActionEdges = (const int***)LHAActionEdgesAr;"<< endl;
     */
    LhaCppFile << funDecl("") << "LHA():NbLoc(" << MyLha.NbLoc << "),NbTrans(" << MyLha.TransitionIndex.size()<<"),NbVar(" << MyLha.NbVar << "),FinalLoc( " << MyLha.NbLoc << ",false){" << endl;

    for (let it : MyLha.InitLoc)
        LhaCppFile << "    InitLoc.insert(" << it << ");" << endl;
    for (let it : MyLha.FinalLoc)
        LhaCppFile << "    FinalLoc[" << it << "]=true;" << endl;

	if(P.CountTrans)
		LhaCppFile << "    EdgeCounter = vector<int>("<<MyLha.Edge.size()<<",0);"<< endl;
    LhaCppFile << "    Edge= vector<LhaEdge>(" << MyLha.Edge.size() << ");" << endl;

	if(P.StringInSpnLHA){
		LhaCppFile << "\n    vector<string> vlstr(NbLoc);" << endl;
		LhaCppFile << "    LocLabel= vlstr;" << endl;

		for (size_t i = 0; i < MyLha.LocLabel.size(); i++)
			LhaCppFile << "    LocLabel[" << i << "]=\"" << MyLha.LocLabel[i] << "\";" << endl;

        LhaCppFile << "    VarLabel= vector<string>(NbVar);" << endl;
        for (size_t v = 0; v < MyLha.Vars.label.size(); v++) {
			LhaCppFile << "    VarLabel[" << v << "]=\"" << MyLha.Vars.label[v] << "\";" << endl;
        }
	}

    for (size_t i = 0; i < MyLha.Edge.size(); i++) {
        LhaCppFile << "    Edge[" << i << "] = LhaEdge(" << MyLha.Edge[i].Index;
        LhaCppFile << ", " << MyLha.Edge[i].Source;
        LhaCppFile << ", " << MyLha.Edge[i].Target;
        if (MyLha.EdgeActions[i].size() < 1)
            LhaCppFile << ",Auto);" << endl;
        else
            LhaCppFile << ",Synch);" << endl;

    }

    LhaCppFile << "\tVars = new Variables;" << endl;
	LhaCppFile << "\ttempVars = new Variables;" << endl;
	LhaCppFile << "\tresetVariables();" << endl;

    LhaCppFile << "    Out_A_Edges =vector< set < int > >(NbLoc);" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        if (MyLha.EdgeActions[e].size() < 1)
            LhaCppFile << "    Out_A_Edges[" << MyLha.Edge[e].Source << "].insert(" << e << ");" << endl;
    }

    LhaCppFile << "    LinForm= vector<double>("<< MyLha.LinearForm.size() <<",0.0);" << endl;
    LhaCppFile << "    OldLinForm=vector<double>("<< MyLha.LinearForm.size() <<",0.0);" << endl;
    LhaCppFile << "    LhaFunc=vector<double>("<< MyLha.LhaFuncArg.size() <<",0.0);" << endl;
    LhaCppFile << "    LhaFuncDefaults=vector<double>("<< MyLha.LhaFuncArg.size() <<",0.0);" << endl;

    for (size_t i = 0;i < MyLha.LhaFuncArg.size(); i++) {
        if (MyLha.LhaFuncType[i] == "Min") {
            LhaCppFile << "    LhaFunc[" << i << "] = DBL_MAX;" << endl;
            LhaCppFile << "    LhaFuncDefaults[" << i << "] = DBL_MAX;" << endl;
        }
    }

	if(P.CountTrans){
		LhaCppFile << "    FormulaVal = vector<double>(" << MyLha.Algebraic.size() + MyLha.Edge.size() << ",0.0);" << endl;
	} else {
		LhaCppFile << "    FormulaVal = vector<double>(" << MyLha.Algebraic.size() << ",0.0);" << endl;
    }
    LhaCppFile << "    FormulaValQual = vector<bool>(" << MyLha.FinalStateCond.size() << ",false);" << endl;

    LhaCppFile << "}\n" << endl;


	LhaCppFile << funDecl("void") << "DoElapsedTimeUpdate(double DeltaT,const DEDState& Marking) {\n";
    for (size_t v = 0; v < MyLha.Vars.label.size() ; v++) {
		if(MyLha.Vars.type[v] == CONTINIOUS_VARIABLE )
			LhaCppFile <<  "\tVars->"<< MyLha.Vars.label[v] << " += GetFlow("<<v<<", Marking) * DeltaT;\n";
    }
	LhaCppFile << "}\n";


    LhaCppFile << funDecl("double") << "GetFlow(int v, const DEDState& Marking)const{" << endl;
    casesHandler flowcases("v");
    for (size_t x = 0; x < MyLha.NbVar; x++) {
		stringstream newcase;
		if(MyLha.Vars.type[x] == CONTINIOUS_VARIABLE ){
			casesHandler flowcases2("CurrentLocation");
			for (size_t l = 0; l < MyLha.NbLoc; l++) {
				stringstream newcase2;
                if (MyLha.FuncFlow[l][x] != ""){
					newcase2 << "\t\t\treturn " << MyLha.FuncFlow[l][x] << ";" << endl;
				}else newcase2 << "\t\treturn 0.0;" << endl;
				flowcases2.addCase(l, newcase2.str(),MyLha.LocLabel[l]);
			}
			flowcases2.writeCases(newcase);
		}
		flowcases.addCase(x, newcase.str(),MyLha.Vars.label[x]);
    }
    flowcases.writeCases(LhaCppFile);
    LhaCppFile << "}\n" << endl;

    LhaCppFile << funDecl("bool") << "CheckLocation(int loc,const DEDState& Marking)const{" << endl;
	casesHandler checklock("loc");
    for (size_t l = 0; l < MyLha.NbLoc; l++) {
		stringstream newcase;
		newcase << "         return " << MyLha.FuncLocProperty[l] << ";" << endl;
		checklock.addCase(l, newcase.str(),MyLha.LocLabel[l]);
    }
	checklock.writeCases(LhaCppFile);

    LhaCppFile << "}\n" << endl;

    LhaCppFile << funDecl("bool") << "CheckEdgeContraints(int ed,size_t ptt,const abstractBinding& b,const DEDState& Marking)const{" << endl;
    casesHandler checkConstrain("ed");
    for (size_t e = 0; e < MyLha.Edge.size(); e++){

		stringstream newcase;
		if((MyLha.ConstraintsRelOp[e].size()>0 && MyLha.EdgeActions[e].size() > 0) || MyLha.unTimeEdgeConstraints[e]!="true" ){
			newcase << "{" << endl;
			if(MyLha.ConstraintsRelOp[e].size()>0 && MyLha.EdgeActions[e].size() > 0){
				for (size_t c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++) {
                    size_t k = 0;
                    for (size_t v = 0; v < MyLha.NbVar; v++)
                        if (MyLha.ConstraintsCoeffs[e][c][v] != "" /*&& MyLha.Vars.type[v] != CONTINIOUS_VARIABLE*/)k++;
                    if(k>0){
                        newcase << "         if(!( ";
                        for (size_t v = 0; v < MyLha.Vars.label.size(); v++) {
                            if (MyLha.ConstraintsCoeffs[e][c][v] != "" /*&& MyLha.Vars.type[v] != CONTINIOUS_VARIABLE*/)
                                newcase << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*Vars->" << MyLha.Vars.label[v];

                        }
                        newcase << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << ")) return false;" << endl;
                    }
				}
			}
            newcase << "\t\treturn (" << MyLha.unTimeEdgeConstraints[e] << ");" << endl;
			newcase << "     }" << endl;
		}else{
			newcase << "\treturn true;" << endl;
		}
		checkConstrain.addCase(e, newcase.str());
	}
	checkConstrain.writeCases(LhaCppFile);

    LhaCppFile << "}\n" << endl;


    LhaCppFile << funDecl("t_interval") << "GetEdgeEnablingTime(int ed,const DEDState& Marking)const{" << endl;
	casesHandler enablingtime("ed");
	//    LhaCppFile << "    switch(ed){" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++){
		stringstream newcase;
		if(MyLha.ConstraintsRelOp[e].size()>0  && MyLha.EdgeActions[e].size() < 1){
			//LhaCppFile << "     case " << e << ": //";
			//LhaCppFile << MyLha.LocLabel[MyLha.Edge[e].Source] << " -> " << MyLha.LocLabel[MyLha.Edge[e].Target] << endl;

			//LhaCppFile << "         return GetEdgeEnablingTime_" << e << "( Marking);" << endl;
			newcase << "         {" << endl;

			newcase << "             t_interval EnablingT;\n" << endl;
			newcase << "             EnablingT.first=CurrentTime;" << endl;
			newcase << "             EnablingT.second=DBL_MAX;\n" << endl;
			newcase << "             t_interval EmptyInterval;\n" << endl;
			newcase << "             EmptyInterval.first=0;" << endl;
			newcase << "             EmptyInterval.second=-1;\n" << endl;

			newcase << "             double SumAF;" << endl;
			newcase << "             double SumAX;" << endl;

			newcase << "\n" << endl;

			for (size_t c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++){
                size_t k = 0;
                for (size_t v = 0; v < MyLha.NbVar; v++)
					if (MyLha.ConstraintsCoeffs[e][c][v] != "" && MyLha.Vars.type[v] == CONTINIOUS_VARIABLE)k++;
                if(k>0){

                    newcase << "             SumAF=";
                    for (size_t v = 0; v < MyLha.NbVar; v++)
                        if (MyLha.ConstraintsCoeffs[e][c][v] != "" && MyLha.Vars.type[v] == CONTINIOUS_VARIABLE)
                            newcase << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*GetFlow(" << v << ", Marking)";
                    newcase << ";\n             SumAX=";
                    for (size_t v = 0; v < MyLha.Vars.label.size(); v++)
                        if (MyLha.ConstraintsCoeffs[e][c][v] != "" && MyLha.Vars.type[v] == CONTINIOUS_VARIABLE)
                            newcase << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*Vars->" << MyLha.Vars.label[v];
                    newcase << ";\n" << endl;

                    string RelOp = MyLha.ConstraintsRelOp[e][c];

                    newcase << "             if(SumAF==0){" << endl;
                    newcase << "                  if(!(SumAX" << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << "))" << endl;
                    newcase << "                      return EmptyInterval;" << endl;
                    newcase << "             }" << endl;
                    newcase << "             else{" << endl;
                    newcase << "                  double t=CurrentTime+(" << MyLha.ConstraintsConstants[e][c] << "-SumAX)/(double)SumAF;" << endl;
                    if (RelOp == "==") {

                        newcase << "                  if(t>=EnablingT.first && t<=EnablingT.second){" << endl;
                        newcase << "                      EnablingT.first=t; EnablingT.second=t;" << endl;
                        newcase << "                  }" << endl;
                        newcase << "                  else return EmptyInterval;" << endl;

                    } else {
                        newcase << "                  if(SumAF>0){" << endl;
                        if (RelOp == "<=") {
                            newcase << "                     if(EnablingT.second>t) EnablingT.second=t;" << endl;
                            newcase << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;

                        } else {
                            newcase << "                     if(EnablingT.first<t) EnablingT.first=t;" << endl;
                            newcase << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                        }

                        newcase << "                      }" << endl;

                        newcase << "                  else{" << endl;
                        const auto RelOp2 = InvRelOp(RelOp);
                        if (RelOp2 == "<=") {
                            newcase << "                     if(EnablingT.second>t) EnablingT.second=t;" << endl;
                            newcase << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;

                        } else {
                            newcase << "                     if(EnablingT.first<t) EnablingT.first=t;" << endl;
                            newcase << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                        }

                        newcase << "                      }" << endl;
                    }
                    newcase << "             }" << endl;

                }
            }

			newcase << "             return EnablingT;" << endl;
			newcase << "         }"<< endl;


			//LhaCppFile << "         }" << endl;
			//LhaCppFile << "         break;" << endl;

		}else{
			newcase << "         {" << endl;

			newcase << "             t_interval EnablingT;\n" << endl;
			newcase << "             EnablingT.first=CurrentTime;" << endl;
			newcase << "             EnablingT.second=DBL_MAX;\n" << endl;
			newcase << "             return EnablingT;" << endl;
			newcase << "         }"<< endl;

		}
		enablingtime.addCase(e, newcase.str());
	}
	enablingtime.writeCases(LhaCppFile);
    LhaCppFile << "}\n" << endl;



    LhaCppFile << funDecl("void") << "DoEdgeUpdates(int ed,const DEDState& Marking, const abstractBinding& b){" << endl;
	casesHandler edgeUpdateHandler("ed");
    //LhaCppFile << "    switch(ed){" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
		stringstream newcase;
        int k = 0;
		for (size_t v = 0; v < MyLha.NbVar; v++)if (MyLha.FuncEdgeUpdates[e][v] != "")k++;
		if(k>0 || P.CountTrans){
			//LhaCppFile << "     case " << e << ": // ";
			//LhaCppFile << MyLha.LocLabel[MyLha.Edge[e].Source] << " -> " << MyLha.LocLabel[MyLha.Edge[e].Target] << endl;
			newcase << "         {"<< endl;
			if(P.CountTrans)
				newcase << "         EdgeCounter[" << e << "]++ ;"<< endl;

            if (k==1) {
                for (size_t v = 0; v < MyLha.NbVar; v++)
					if (MyLha.FuncEdgeUpdates[e][v] != ""){
						newcase << "\t\tVars->" << MyLha.Vars.label[v];
                        if(MyLha.Vars.type[v] ==INT_INDEXED_DISC_ARRAY)
                            newcase << "[ (int) " << MyLha.FuncEdgeUpdatesIndex[e][v] << "]";
                        newcase << "=" << MyLha.FuncEdgeUpdates[e][v] << ";" << endl;
					}
            } else if (k > 1) {
                //newcase << "         *tempVars = *Vars;" << endl;
				for (size_t v = 0; v < MyLha.NbVar; v++)
					if (MyLha.FuncEdgeUpdates[e][v] != ""){
                        if(MyLha.Vars.type[v] ==INT_INDEXED_DISC_ARRAY){
                            newcase << "\t\tconst int tmp_INDEX_" << MyLha.Vars.label[v];
                            newcase << "= " << MyLha.FuncEdgeUpdatesIndex[e][v] << ";"<< endl;
                        }
						newcase << "\t\ttempVars->" << MyLha.Vars.label[v];
                        if(MyLha.Vars.type[v] ==INT_INDEXED_DISC_ARRAY)
                            newcase << "[ tmp_INDEX_" << MyLha.Vars.label[v] << "]";
                        newcase << "=" << MyLha.FuncEdgeUpdates[e][v] << ";" << endl;
					}/*else{
                      newcase << "         tempVars->" << MyLha.Vars.label[v] << "=Vars->" << MyLha.Vars.label[v] << ";" << endl;
                      }*/
				//newcase << "\t\tstd::swap(Vars,tempVars);\n";
				for (size_t v = 0; v < MyLha.NbVar; v++)
					if (MyLha.FuncEdgeUpdates[e][v] != ""){
                        newcase << "\t\tVars->" << MyLha.Vars.label[v];
                        if(MyLha.Vars.type[v] ==INT_INDEXED_DISC_ARRAY)
                            newcase << "[ tmp_INDEX_" << MyLha.Vars.label[v] << "]";
                        newcase << " = tempVars->"<< MyLha.Vars.label[v];
                        if(MyLha.Vars.type[v] ==INT_INDEXED_DISC_ARRAY)
                            newcase << "[ tmp_INDEX_" << MyLha.Vars.label[v] << "]";
                        newcase << ";"<<endl;
                    }
			}
			//LhaCppFile << "         DoEdgeUpdates_" << e << "( Marking);" << endl;
			newcase << "         }"<< endl;
			//LhaCppFile << "         break;" << endl;
		}
		edgeUpdateHandler.addCase(e, newcase.str());
	}
	edgeUpdateHandler.writeCases(LhaCppFile);
	//LhaCppFile << "    }" << endl;
    for (let it : MyLha.LinearForm)
        if(!MyLha.SimplyUsedLinearForm[it.second]){
            LhaCppFile << "    OldLinForm[" << it.second << "]=LinForm[" << it.second << "];" << endl;

        }

	LhaCppFile << "}\n" << endl;



	/*for (int e = 0; e < MyLha.Edge.size(); e++) {
	 LhaCppFile << "void LHA::DoEdgeUpdates_" << e << "(abstractMarking& Marking){" << endl;

	 LhaCppFile << "    }\n" << endl;
	 }*/

	LhaCppFile << funDecl("void") << "UpdateLinForm(const DEDState& Marking){" << endl;
    for (let it : MyLha.LinearForm)
        if(!MyLha.SimplyUsedLinearForm[it.second]){
            LhaCppFile << "    LinForm[" << it.second << "]=" << it.first << ";" << endl;

        }
	LhaCppFile << "    }\n" << endl;

	LhaCppFile << funDecl("void") << "UpdateLhaFunc(double& Delta ){" << endl;
	for (size_t i = 0; i < MyLha.LhaFuncArg.size(); i++) {
		/*if (MyLha.LhaFuncType[i] == "Last")
		 LhaCppFile << "    LhaFunc[" << i << "]=LinForm[" << MyLha.LhaFuncArg[i] << "];" << endl;
		 else {*/
		if (MyLha.LhaFuncType[i] == "Min")
			LhaCppFile << "    LhaFunc[" << i << "]=min(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
		else if (MyLha.LhaFuncType[i] == "Max")
			LhaCppFile << "    LhaFunc[" << i << "]=Max(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "],OldLinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
		else if (MyLha.LhaFuncType[i] == "Integral")
			LhaCppFile << "    LhaFunc[" << i << "]=Integral(LhaFunc[" << i << "], CurrentTime, Delta, OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
		else if (MyLha.LhaFuncType[i] == "Mean")
			LhaCppFile << "    LhaFunc[" << i << "]=Integral(LhaFunc[" << i << "], CurrentTime, Delta, OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
		//}
		
	}
	LhaCppFile << "\n    }\n" << endl;
	
	LhaCppFile << funDecl("void") << "UpdateFormulaVal(const DEDState& Marking){\n" << endl;
    for (size_t i = 0; i < MyLha.LhaFuncArg.size(); i++) {
        if (MyLha.LhaFuncType[i] == "Last" && MyLha.SimplyUsedLinearForm[MyLha.LhaFuncArg[i]]){
            string str;
            for( auto it : MyLha.LinearForm) if(it.second== MyLha.LhaFuncArg[i])str=it.first;
            LhaCppFile << "    LhaFunc[" << i << "]= " << str << ";" << endl;
        }else if (MyLha.LhaFuncType[i] == "Last"){
            LhaCppFile << "    LhaFunc[" << i << "]=LinForm[" << MyLha.LhaFuncArg[i] << "];" << endl;
        } else if(MyLha.LhaFuncType[i] == "Mean")
            LhaCppFile << "    LhaFunc[" << i << "]=LhaFunc[" << i << "] / CurrentTime ;" << endl;
    }

    for(size_t i=0;i<MyLha.Algebraic.size();i++){
		//haCppFile << "    OldFormulaVal=FormulaVal["<<i<<"];" << endl;
		LhaCppFile << "    FormulaVal["<<i<<"]=" << MyLha.Algebraic[i] << ";" << endl;
	}
	if (P.CountTrans) {
		for(size_t i=0;i<MyLha.Edge.size();i++){
			size_t j = i+ MyLha.Algebraic.size();
			LhaCppFile << "    FormulaVal["<<j<<"]= EdgeCounter[" << i << "];\n";
		}
	}
    for(size_t i = 0; i < MyLha.FinalStateCond.size(); i++){
        LhaCppFile << "    FormulaValQual[" << i << "] = CurrentLocation == " << MyLha.LocIndex.find(MyLha.FinalStateCond[i])->second << ";"<< endl;
    }

    LhaCppFile << "}\n" << endl;

    LhaCppFile << "bool IsLHADeterministic = "<< MyLha.isDeterministic<< ";" << endl;

    LhaCppFile << "fullState::fullState():loc(0){\n\tvar= new Variables;\n}\n" << endl;
    LhaCppFile << "fullState::fullState(int l,const Variables &v):loc(l){\n\tvar= new Variables(v);\n}\n" << endl;
    LhaCppFile << "fullState::fullState(const fullState &fs):loc(fs.loc){\n\tvar= new Variables(*(fs.var));\n}\n" << endl;
    LhaCppFile << "fullState::~fullState(){delete var;}\n" << endl;

    LhaCppFile << "template class LHA<abstractMarking>;" << endl;
    if(P.modelType == External /*|| P.modelType == GSPN_Simulink*/){
        LhaCppFile << "#include \"SKModel.hpp\"" << endl;
        LhaCppFile << "template class LHA<abstractState>;" << endl;
    }
    //LhaCppFile << "#include \"MarkovChainState.hpp\"" << endl;
    
    /*LhaCppFile << "class State{" << endl;
    LhaCppFile << "    int state;" << endl;
    LhaCppFile << "    void printSedCmd(std::ostream &of)const{ of << state << \"\t\"; }" << endl;
    LhaCppFile << "    void printHeader(std::ostream &of)const{ of << state << \"\t\"; }" << endl;
    LhaCppFile << "    void print(std::ostream &of, double et)const{ of << state << \"\t\"; }" << endl;
    LhaCppFile << "};" << endl;
    LhaCppFile << "template class LHA<State>;" << endl;*/
    

    //LhaCppFile << "template class LHA<State>;" << endl;
    
	LhaCppFile.close();
}


void Lha_Reader::writeDotFile(const string &file)const{
    ofstream df(file.c_str(), ios::out | ios::trunc);
    df << "digraph G {" << endl;

    for (let l : MyLha.LocLabel ) {
        df << "\t" << l;
        df << " [shape=circle];" << endl;
    }

    for (let e : MyLha.Edge ){
        df << "\t" << MyLha.LocLabel[e.Source] << "->" << MyLha.LocLabel[e.Target];
        df << " [label=\"{";
        if(MyLha.EdgeActions[e.Index].size()>5){df<< "##";}else for(let a :MyLha.EdgeActions[e.Index])df << a << ", ";
        df<< "}, {";

        if((MyLha.ConstraintsRelOp[e.Index].size()>0 && MyLha.EdgeActions[e.Index].size() > 0) || MyLha.unTimeEdgeConstraints[e.Index]!="true" ){
            if(MyLha.ConstraintsRelOp[e.Index].size()>0 && MyLha.EdgeActions[e.Index].size() > 0){
                for (size_t c = 0; c < MyLha.ConstraintsRelOp[e.Index].size(); c++) {
                    size_t k = 0;
                    for (size_t v = 0; v < MyLha.NbVar; v++)
                        if (MyLha.ConstraintsCoeffs[e.Index][c][v] != "")k++;
                    if(k>0){
                        for (size_t v = 0; v < MyLha.Vars.label.size(); v++) {
                            if (MyLha.ConstraintsCoeffs[e.Index][c][v] != "" )
                                df << "(" << MyLha.ConstraintsCoeffs[e.Index][c][v] << ")*" << MyLha.Vars.label[v];

                        }
                        df << MyLha.ConstraintsRelOp[e.Index][c] << MyLha.ConstraintsConstants[e.Index][c] << ", ";
                    }
                }
            }
        }

        df << "}, {";
        for (size_t v = 0; v < MyLha.NbVar; v++)
            if (MyLha.FuncEdgeUpdates[e.Index][v] != ""){
                df << MyLha.Vars.label[v] << ":=" << MyLha.FuncEdgeUpdates[e.Index][v] << ",";
            }
        df<<"} \"];" << endl;
    }
    df << "}" << endl;
}










