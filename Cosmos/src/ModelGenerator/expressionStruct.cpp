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
 * file expressionStruct.cpp                                                   *
 * Created by Benoit Barbot on 17/04/14.                                       *
 *******************************************************************************
 */


#include <cmath>
#include <sstream>

#include "expressionStruct.hpp"

using namespace std;

std::ostream& text_output::operator<<(std::ostream& os, const expr& e)
{
  switch (e.t) {
  case Empty:
    break;
  case UnParsed: os << e.stringVal << " ";
    break;
  case ColouredPlaceHolder: os << "COLOURED ";
    break;
  case Bool: os  << e.boolVal << " ";
    break;
  case Int: os << e.intVal << " ";
    break;
  case Real: os << e.realVal << " ";
    break;
  case PlaceName: os << " Marking.P->_PL_" << e.stringVal << " ";
    break;
  case ColorVarName: os << " bl.P->" << e.stringVal << ".c0 ";
    break;
  case Constant: os  << e.stringVal << " ";
    break;
  case Ceil: os << " ceil(" << *(e.lhs) << ") ";
    break;
  case Floor: os << " floor(" << *(e.lhs) << ") ";
    break;
  case Exp: os << " exp(" << *(e.lhs) << ") ";
    break;
  case Plus: os << " (" << *(e.lhs) << " + " << *(e.rhs) << ") ";
    break;
  case Times: os << " (" << *(e.lhs) << " * " << *(e.rhs) << ") ";
    break;
  case Minus: os << " (" << *(e.lhs) << " - " << *(e.rhs) << ") ";
    break;
  case Div: os << " (" << *(e.lhs) << " / (double) " << *(e.rhs) << ") ";
    break;
  case Min: os << " min(" << *(e.lhs) << ", " << *(e.rhs) << ") ";
    break;
  case Max: os << " max(" << *(e.lhs) << ", " << *(e.rhs) << ") ";
    break;
  case Pow: os << " pow(" << *(e.lhs) << ", " << *(e.rhs) << ") ";
    break;
  case Neg: os << " !(" << *(e.lhs) << ") ";
    break;
  case And: os << " (" << *(e.lhs) << " && " << *(e.rhs) << ") ";
    break;
  case Or: os << " (" << *(e.lhs) << " || " << *(e.rhs) << ") ";
    break;
  case Eq: os << " (" << *(e.lhs) << " == " << *(e.rhs) << ") ";
    break;
  case Neq: os << " (" << *(e.lhs) << " != " << *(e.rhs) << ") ";
    break;
  case Leq: os << " (" << *(e.lhs) << " <= " << *(e.rhs) << ") ";
    break;
  case Sl: os << " (" << *(e.lhs) << " < " << *(e.rhs) << ") ";
    break;
  case Geq: os << " (" << *(e.lhs) << " >= " << *(e.rhs) << ") ";
    break;
  case SG: os << " (" << *(e.lhs) << " > " << *(e.rhs) << ") ";
    break;
  case In: os << " contains(" <<*(e.lhs) <<" , " << *(e.rhs) << ") ";
    break;
  case NotIn: os << " !contains((" << *(e.lhs) <<" , " << *(e.rhs) << ") ";
    break;
  case ListContinuation: os << *(e.lhs) << " , " << *(e.rhs);
    break;
  case Var: os << e.lhs->stringVal; break;
  case Lambda: os << "([] (auto "<< e.lhs->stringVal << "){" << *(e.rhs) << "})"; break;
  case App: os << "("<<  e.lhs->stringVal << "(" << *(e.rhs) << "))"; break;

  }
  return os;
}

bool operator< (const expr &lhs, const expr &rhs){
    if(lhs.is_concrete() && rhs.is_concrete()){
        return lhs.get_real() < rhs.get_real();
    }
    if( lhs.t == Bool && rhs.t == Bool)return lhs.boolVal < rhs.boolVal;
    
    return true;
}

bool operator== (const expr &lhs, const expr &rhs){
    return (lhs.t == rhs.t
            && (lhs.t != Bool || lhs.boolVal == rhs.boolVal)
            && (lhs.t != Int || lhs.intVal == rhs.intVal)
            && (lhs.t != Real || lhs.realVal == rhs.realVal)
            && ((lhs.t == Bool || lhs.t == Int || lhs.t == Real)
                && lhs.stringVal == rhs.stringVal
                && lhs.lhs == rhs.lhs
                && lhs.rhs == rhs.rhs)
            );
}
bool operator!= (const expr &lhs, const expr &rhs){
    return !(lhs == rhs);
}

expr::expr(const string& str){
    try {
        intVal = std::stoi(str);
        t = Int;
    }catch (...) {
        try {
            realVal = std::stod(str);
            t = Real;
        }catch (...){
            stringVal = str;
            t = UnParsed;
        }
    }
}

string expr::to_string()const {
  using namespace text_output;
  stringstream ss;
  ss << *this;
  return ss.str();
}

std::ostream& xml_output::operator<<(std::ostream& os, const expr& e){
  switch (e.t) {
  case Empty:
    break;
  case UnParsed: os << "<attribute name=\"expr\"><attribute name=\"unParsed\">"<< e.stringVal << "</attribute></attribute>";
    break;
  case ColouredPlaceHolder: os << "<attribute name=\"expr\"><attribute name=\"unParsed\">COLOURED</attribute></attribute>";
    break;
  case Bool: os << "<attribute name=\"expr\"><attribute name=\"boolValue\">"<< boolalpha << e.boolVal << "</attribute></attribute>";
    break;
  case Int:  os << "<attribute name=\"expr\"><attribute name=\"intValue\">"<< e.intVal << "</attribute></attribute>";
    break;
  case Real: os << "<attribute name=\"expr\"><attribute name=\"numValue\">"<< e.realVal << "</attribute></attribute>";
    break;
  case PlaceName: os << "<attribute name=\"expr\"><attribute name=\"name\">"<< e.stringVal << "</attribute></attribute>";
    break;
  case Constant: os << "<attribute name=\"expr\"><attribute name=\"name\">"<< e.stringVal << "</attribute></attribute>";
    break;
  case Plus:
    os << "<attribute name=\"expr\"><attribute name=\"function\"><attribute name=\"+\">" << e.lhs << e.rhs;
    os << "</attribute></attribute></attribute>";
    break;
  case Times:
    os << "<attribute name=\"expr\"><attribute name=\"function\"><attribute name=\"*\">" << e.lhs << e.rhs;
    os << "</attribute></attribute></attribute>";
    break;
  case Minus:
    os << "<attribute name=\"expr\"><attribute name=\"function\"><attribute name=\"-\">"<< e.lhs << e.rhs;
    os << "</attribute></attribute></attribute>";
    break;

  case Div:
    os << "<attribute name=\"expr\"><attribute name=\"function\"><attribute name=\"/\">"<< e.lhs << e.rhs;
    os << "</attribute></attribute></attribute>";
    break;
  default:
    cerr << "PrintXML Not yet implemented";
    exit(EXIT_FAILURE);
  }
  return os;
}

double expr::get_real()const{
  if(t==Int) return (double)intVal;
  else if(t==Real) return realVal;
  else return NAN;

}
expType expr::mix(const expr &le,const expr &re)const{
  const auto l=le.t;
  const auto r=re.t;
  if (l == Int && r ==Int)return Int;
  else if ((l == Real && r ==Int)|| (l == Int && r == Real)|| (l == Real && r ==Real))
    return Real;
  else if(le.is_concrete() && re.is_concrete())
    return Bool;
  else return Plus;
}

bool expr::is_concrete()const{
  return (t== Int || t == Real || t==boolVal);
}

bool expr::empty()const{
  return (t==Empty || (t==UnParsed && stringVal.empty()));
}

bool expr::is_zero()const{
  return empty() || (t== Int && intVal == 0) || ( t == Real && realVal == 0.0);
}

void expr::rewrite( function<void(expr&)> f){
  switch (t) {
  case Constant:
  case Empty:
  case UnParsed:
  case PlaceName:
  case Bool:
  case Int:
  case Real:
  case Var:
  case Lambda:
  case ColouredPlaceHolder:
  case ColorVarName:
    // 0-ary
    f(*this);
    break;

  case App:
    //unary right
    rhs->rewrite(f);
    f(*this);
    break;
    
  case Ceil:
  case Floor:
  case Neg:
  case Exp:
    //unary left
    lhs->rewrite(f);
    f(*this);
    break;
    
  case In:
  case NotIn:
  case Plus:
  case Times:
  case Minus:
  case Div:
  case Min:
  case Max:
  case Pow:
  case And:
  case Or:
  case Eq:
  case Neq:
  case Leq:
  case Sl:
  case Geq:
  case SG:
  case ListContinuation:
    //binary
    lhs->rewrite(f);
    rhs->rewrite(f);
    f(*this);
    break;
  }
}
  

void expr::eval(const map<string,int> &intconst,const map<string,double> &realconst){
  switch (t) {
  case Constant:
    {
      const auto it = intconst.find(stringVal);
      if(it != intconst.end()){
        t=Int;
        intVal = it->second;
      }else{
        const auto it = realconst.find(stringVal);
        if(it != realconst.end()){
          t=Real;
          realVal = it->second;
        }
      }
    }
    break;
  case Empty:
  case UnParsed:
  case PlaceName:
  case Bool:
  case Int:
  case Real:
  case Var:
  case Lambda:
  case ColouredPlaceHolder:
  case In:
  case NotIn:
  case ColorVarName:
    break;
  case App:
    rhs->eval(intconst,realconst);
    break;
  case ListContinuation:
    lhs->eval(intconst,realconst);
    rhs->eval(intconst,realconst);
    break;
  case Ceil:
  case Floor:
  case Neg:
  case Exp:
    lhs->eval(intconst,realconst);
    if (lhs->t==Int){
      expr tmp = *lhs;
      *this= tmp;
    } else if(lhs->t==Real){
      if(t==Floor)intVal = (int)floor(lhs->realVal);
      else intVal=(int)ceil(lhs->realVal);
      t= Int;
      lhs.reset();
    } else if(lhs->t==Bool){
      t=Bool;
      boolVal = !lhs->boolVal;
      lhs.reset();
    }
    break;
  case Plus:
  case Times:
  case Minus:
  case Div:
  case Min:
  case Max:
  case Pow:
  case And:
  case Or:
  case Eq:
  case Neq:
  case Leq:
  case Sl:
  case Geq:
  case SG:
    lhs->eval(intconst,realconst);
    rhs->eval(intconst,realconst);
    expType t2=mix(*rhs,*lhs);
    if(t2==Int) {
      switch (t) {
      case Plus: intVal = lhs->intVal + rhs->intVal;
        break;
      case Times: intVal = lhs->intVal * rhs->intVal;
        break;
      case Minus: intVal = lhs->intVal - rhs->intVal;
        break;
      case Div: intVal = lhs->intVal / rhs->intVal;
        break;
      case Min: intVal = min(lhs->intVal,rhs->intVal);
        break;
      case Max: intVal = max(lhs->intVal,rhs->intVal);
        break;
      case Pow: intVal = (lhs->intVal)^(rhs->intVal);
        break;
      default:
        break;
      }
      t = Int;
      lhs.reset();
      rhs.reset();
    } else if (t2==Real ){
      switch (t) {
      case Plus: realVal = lhs->get_real() + rhs->get_real();
        break;
      case Times: realVal = lhs->get_real() * rhs->get_real();
        break;
      case Minus: realVal = lhs->get_real() - rhs->get_real();
        break;
      case Div: realVal = lhs->get_real() / rhs->get_real();
        break;
      case Min: realVal = min(lhs->get_real(),rhs->get_real());
        break;
      case Max: realVal = max(lhs->get_real(),rhs->get_real());
        break;
      case Pow: realVal = pow(lhs->get_real(),rhs->get_real());
        break;
      default:
        break;
      }
      t = Real;
      lhs.reset();
      rhs.reset();
    } else if (t2 == Bool){
      switch (t) {
      case And: boolVal = lhs->boolVal && rhs->boolVal;
        break;
      case Or: boolVal = lhs->boolVal || rhs->boolVal;
        break;
      case Eq: boolVal = lhs->get_real() == rhs->get_real();
        break;
      case Leq: boolVal = lhs->get_real() <= rhs->get_real();
        break;
      case Sl: boolVal = lhs->get_real() < rhs->get_real();
        break;
      case Geq: boolVal = lhs->get_real() >= rhs->get_real();
        break;
      case SG: boolVal = lhs->get_real() > rhs->get_real();
        break;
      default:
        break;
      }
      t = Bool;
      lhs.reset();
      rhs.reset();
    }
    break;
  }
}



void expr::eval(){
  eval(map<string,int>(), map<string,double>());
}

void expr::get_places(set<string> & acset)const{
  if(t==PlaceName)acset.insert(stringVal);
  if(lhs.use_count()>0)lhs->get_places(acset);
  if(rhs.use_count()>0)rhs->get_places(acset);
}

bool expr::is_markDep()const{
  set<string> accset;
  get_places(accset);
  return !accset.empty();
}
