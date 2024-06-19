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

#include "Gspn-Reader.hpp"
#include "Gspn_gmlparser.hpp"

#include "string.h"

#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;


Gspn_Reader::Gspn_Reader(parameters &Q):spn(new GspnType()),P(Q) {
  trace_scanning = false;
  trace_parsing = false;
}

Gspn_Reader::~Gspn_Reader() {
}

int Gspn_Reader::parse(string& expr) {

  scan_expression(expr);

  IndexDist["UNIFORM"] = UNIFORM;
  IndexDist["EXPONENTIAL"] = EXPONENTIAL;
  IndexDist["DETERMINISTIC"] = DETERMINISTIC;
  IndexDist["LOGNORMAL"] = LOGNORMAL;
  IndexDist["TRIANGLE"] = TRIANGLE;
  IndexDist["GEOMETRIC"] = GEOMETRIC;
  IndexDist["ERLANG"] = ERLANG;
  IndexDist["GAMMA"] =GAMMA;
  IndexDist["IMMEDIATE"] = IMMEDIATE;
  IndexDist["USERDEFINED"] = USERDEFINE;


  gspn::Gspn_parser parser(*this);

  parser.set_debug_level(trace_parsing);

  int res = parser.parse();
  scan_end();
  return res;
}

int Gspn_Reader::parse_file(string &filename) {
  string str;

  ifstream file(filename.c_str(), ios::in);
  if (file) {

    while (!file.eof()) {

      string str2;
      getline(file, str2);
      str = str + "\n" + str2;
    }
    file.close();

    int x = parse(str);

    if (x) cout << "Parsing GSPN Description file failed" << endl;

    return x;
  } else {
    cout << "Can't open : " << filename << endl;
    return 1;
  }
}

//#include "Gspn_gmlparser.cpp";
int Gspn_Reader::parse_gml_file(parameters &P) {
  ifstream ifile(P.PathGspn.c_str());
  if(ifile){
    //cout << "parse GML:" << filename << endl;
    spn->nbpass=0;
    //first pass declaration and place.
    MyModelHandler* handler = new MyModelHandler(*spn);
    ModelHandlerPtr handlerPtr(handler);
    ExpatModelParser parser = ExpatModelParser(handlerPtr);
    parser.parse_file(P.PathGspn);

    spn->nbpass=1;
    //second pass transitions and arcs.
    ModelHandlerPtr handlerPtr2(new MyModelHandler(*spn,handler->IsPlace,handler->Gml2Place,handler->Gml2Trans));
    ExpatModelParser parser2 = ExpatModelParser(handlerPtr2);

    parser2.parse_file(P.PathGspn);

    //cout << "end parse GML:"<< spn->pl << endl;
    if (P.RareEvent)addSinkTrans();
    return 0;
  }else{
    cout << "File " << P.PathGspn << " does not exist!" << endl;
    exit(EXIT_FAILURE);
  }

}

void
Gspn_Reader::error(const gspn::location& l, const std::string& m) {
  std::cerr << l << ": " << m << std::endl;
}

void
Gspn_Reader::error(const std::string& m) {
  std::cerr << m << std::endl;
}


void Gspn_Reader::addSinkTrans(){
  spn->outArcsStruct.insert(make_pair(spn->arckey(spn->tr-1, spn->pl-1), arc(1)));
}
