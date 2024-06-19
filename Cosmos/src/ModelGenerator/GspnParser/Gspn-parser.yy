%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4"
%defines
%define api.parser.class {Gspn_parser}

%code requires {
#include <math.h>
#include <limits.h>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

#include "../expressionStruct.hpp"
#include "../../Simulator/DistributionDef.hpp"

class Gspn_Reader;
using namespace std;

#define BUFF_SIZE 5000

 }


%parse-param { Gspn_Reader& Reader }
%lex-param   { Gspn_Reader& Reader }

%locations

%debug
%define parse.error verbose

 // Symbols.
%union
 {
   double       RealVal;
   int         IntVal;
   std::string *name;
   char CharVal[100];
   char expression[BUFF_SIZE];
   expr* retexpr;
};

%code {
#include "Gspn-Reader.hpp"
#include <set>
#include <vector>

  vector<expr> Par;
  vector<expr> Par2;
  std::string distrib;
  

  int NbServers;
  bool SingleService;
  bool MarkingDependent;
  bool AgeMemory;

 }

%token        END      0 "end of file"

%token <name>        str
%token <CharVal>   chr
%token <RealVal>     rval
%token <IntVal>      ival

%token <name>     SEMICOLON
%token <name>     COMMA
%token <name>     LB
%token <name>     RB
%token <name>     EQ
%token <name>     DIV
%token <name>     MUL
%token <name>     PLUS
%token <name>     MINUS
%token <name>     POWER
%token <name>     GspnName
%token <name>     NbPl
%token <name>     NbTr
%token <name>     PList
%token <name>     TList

%token <name>     in
%token <name>     out
%token <name>     marking
%token <name>     transitions
%token <name>     Const
%token <name>     INT
%token <name>     DOUBLE
%token <name>     inhibitor
%token <name>     MIN
%token <name>     MAX
%token <name>     FLOOR

%token <name>     SINGLE
%token <name>     INFINITE
%token <name>     MULTIPLE

%token <name>     ENABLINGMEMORY
%token <name>     AGEMEMORY

%token <name>     IMDT
%token <name>     EXPO

%type<retexpr> IntStringFormula
%type<retexpr> RealStringFormula
%type<retexpr> WEIGHT
%type<retexpr> PRIORITY




%printer    { debug_stream () << *$$; } str
%destructor { delete $$; } str

%printer    { debug_stream () << $$; } <IntVal>
%printer    { debug_stream () << $$; } <RealVal>

%%


%left PLUS MINUS;
%left MUL  DIV;
%left POWER;


%start GSPN;

IntStringFormula: ival { $$ = new expr($1);}
| str {
  if(Reader.spn->PlacesId.find(*$1)!=Reader.spn->PlacesId.end())
    {
        $$ = new expr( PlaceName ,*$1);
    }
  else if(Reader.spn->IntConstant.find(*$1)!=Reader.spn->IntConstant.end())
    {
        $$ = new expr( Constant , *$1);
    }
  else{cout<<"'"<<*$1<<"' has not been declared"<<endl;YYABORT;}}
| LB IntStringFormula RB{ $$ = $2;       }
| IntStringFormula PLUS  IntStringFormula   { $$ = new expr(Plus,*$1,*$3); }
| IntStringFormula MINUS IntStringFormula   { $$ = new expr(Minus,*$1,*$3);}
| IntStringFormula MUL   IntStringFormula   { $$ = new expr(Times,*$1,*$3); }
| IntStringFormula POWER IntStringFormula   { $$ = new expr(Pow,*$1,*$3);  }
| FLOOR LB IntStringFormula RB { $$ = new expr(Floor,*$3, *$3 ); }
| FLOOR LB IntStringFormula DIV IntStringFormula RB {
    expr* e = new expr(Div, *$3, *$5);
    $$ = new expr(Floor, *e , *e );}
| MIN LB IntStringFormula COMMA IntStringFormula RB { $$ = new expr(Min,*$3,*$5);  }
| MAX LB IntStringFormula COMMA IntStringFormula RB { $$ = new expr(Max,*$3,*$5);  };


RealStringFormula:  rval { $$ = new expr($1);}
| ival { $$ = new expr($1);}
| str {
  if(Reader.spn->PlacesId.find(*$1)!=Reader.spn->PlacesId.end())
  {
      $$ = new expr( PlaceName ,*$1);
    }
  else if(Reader.spn->RealConstant.find(*$1)!=Reader.spn->RealConstant.end())
  {
      $$ = new expr( Constant , *$1);
    }
  else{cout<<"'"<<*$1<<"' has not been declared"<<endl;YYABORT;}}
| LB RealStringFormula RB              { $$ = $2;  }
| RealStringFormula DIV  RealStringFormula   {  $$ = new expr(Div, *$1, *$3);  }
| RealStringFormula PLUS  RealStringFormula   { $$ = new expr(Plus,*$1,*$3);  }
| RealStringFormula MINUS RealStringFormula   { $$ = new expr(Minus,*$1,*$3);  }
| RealStringFormula MUL   RealStringFormula   { $$ = new expr(Times,*$1,*$3);  }
| RealStringFormula POWER RealStringFormula   { $$ = new expr(Pow,*$1,*$3);  }
| FLOOR LB RealStringFormula RB { $$ = new expr(Floor,*$3, *$3 );   }
| MIN LB RealStringFormula COMMA RealStringFormula RB {$$ = new expr(Min,*$3,*$5); }
| MAX LB RealStringFormula COMMA RealStringFormula RB {$$ = new expr(Max,*$3,*$5);   };



GSPN: declarations definitions
|declarations definitions redifinitions;

declarations:  Constants Sizes  Lists
| Sizes  Lists;



Constants: Constant
|Constant Constants;

Sizes: NbPlaces NbTransitions
|NbTransitions NbPlaces;

Constant: Const INT str EQ IntStringFormula SEMICOLON
{if(Reader.spn->RealConstant.find(*$3)!=Reader.spn->RealConstant.end())
    {cout<<"Constant "<<*$3<<" already defined."<<endl; YYABORT;}
  else {auto st= $5;
    if (Reader.P.constants.count(*$3)>0)st = new expr(Reader.P.constants[*$3]);

    if (st->is_concrete()){
        Reader.spn->IntConstant[*$3]= (int)st->get_real();
        Reader.spn->RealConstant[*$3]=st->get_real();
    
    }else{
        cout<<"Constant "<<*$3<<":"<< st->to_string() << " must be a concrete value."<<endl;
        YYABORT;
    }
  }
}
| Const DOUBLE str EQ RealStringFormula SEMICOLON
{if(Reader.spn->RealConstant.find(*$3)!=Reader.spn->RealConstant.end())
    {cout<<"Constant "<<*$3<<" already defined."<<endl; YYABORT;}
  else {auto st=$5;
        if (Reader.P.constants.count(*$3)>0)st = new expr (Reader.P.constants[*$3]);
        if(st->is_concrete()){
      Reader.spn->RealConstant[*$3]=st->get_real();
        }else{
            cout<<"Constant "<<*$3<<":"<< st->to_string() <<" must be a concrete value."<<endl;
            YYABORT;
        }
    
  }
};

Lists: PlacesList TransitionsList
|TransitionsList PlacesList;

NbPlaces: NbPl EQ ival SEMICOLON {Reader.spn->pl=$3;

 }
|NbPl EQ str SEMICOLON {if(Reader.spn->IntConstant.find(*$3)==Reader.spn->IntConstant.end())
     {
       std::cout<<*$3<<" was not declared"<<std::endl;
       YYABORT;
     }
   Reader.spn->pl=Reader.spn->IntConstant[*$3];

 };

NbTransitions: NbTr EQ ival SEMICOLON {Reader.spn->tr=$3;

 }
|NbTr EQ str SEMICOLON {if(Reader.spn->IntConstant.find(*$3)==Reader.spn->IntConstant.end())
     {
       std::cout<<*$3<<" was not declared"<<std::endl;
       YYABORT;
     }
   Reader.spn->tr=Reader.spn->IntConstant[*$3];

 };

PlacesList: PList EQ '{' PLabels '}' SEMICOLON {
  if(Reader.spn->PlacesId.size()!=Reader.spn->pl){
      std::cout<<"Place label missing or redeclared, expected:"<< Reader.spn->pl <<"; found: "<< Reader.spn->PlacesId.size() <<std::endl;
    YYABORT;
  }

  //Reader.spn->Marking= vector<string>(Reader.spn->pl, " ");

  MarkingDependent=false;
  AgeMemory=false;


 };

PLabels : str {
    //cout << "New place name:" << *$1 << endl;M
        place p;
        p.name = *$1;
        p.id = Reader.spn->placeStruct.size();
        Reader.spn->placeStruct.push_back(p);
    int sz=Reader.spn->PlacesId.size();
    Reader.spn->PlacesId[*$1]=sz;
 }
|PLabels COMMA str {
    //cout << "New place name:" << *$3 << endl;

        place p;
        p.name = *$3;
        p.id = Reader.spn->placeStruct.size();
        Reader.spn->placeStruct.push_back(p);
        int sz=Reader.spn->PlacesId.size();
    Reader.spn->PlacesId[*$3]=sz;
 };

TransitionsList: TList EQ '{' TLabels '}' SEMICOLON {
  if(Reader.spn->TransList.size()!=Reader.spn->tr){
      std::cout<<"Transition label missing or redeclared, expected:"<< Reader.spn->tr <<"; found: "<< Reader.spn->TransList.size() << endl;
     YYABORT;
  }

 };

TLabels : str {Reader.spn->TransList.insert(*$1);
   /*int sz=Reader.spn->TransId.size();
   Reader.spn->TransId[*$1]=sz;*/

 }
|TLabels COMMA str {Reader.spn->TransList.insert(*$3);
   /*int sz=Reader.spn->TransId.size();
   Reader.spn->TransId[*$3]=sz;*/

 };


definitions: PlacesDef TransitionsDef InArcs OutArcs
|PlacesDef TransitionsDef InArcs OutArcs Inhibitors;

PlacesDef: marking EQ '{' PLACES '}' SEMICOLON {};

PLACES: PLACE
|PLACES  PLACE;

PLACE: LB str COMMA IntStringFormula RB SEMICOLON
{ if(Reader.spn->PlacesId.find(*$2)==Reader.spn->PlacesId.end())
    {cout<<"'"<<*$2<<"' has not been declared"<<endl;
      YYABORT;
    }
  expr st= *$4;
  /*if(Evaluate.parse(st)){
    std::cout<<"Initial Marking is not marking dependent"<<std::endl;
    YYABORT;
  }*/
  //cerr << "marking" << *$2 << st.to_string() << endl;
  
  Reader.spn->placeStruct[Reader.spn->PlacesId[*$2]].Marking=st;
  Reader.spn->placeStruct[Reader.spn->PlacesId[*$2]].initMarking=vector<coloredToken>(1, coloredToken(st));
};


TransitionsDef: transitions EQ '{' TRANSITIONS '}' SEMICOLON;

TRANSITIONS: TRANSITION {}
|TRANSITIONS TRANSITION {};

TRANSITION: LB str COMMA dist COMMA PRIORITY COMMA WEIGHT RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  transition *trans = new transition();
  trans->name = $2->c_str();
  trans->type = Timed;
  trans->dist.name = distrib;
  for(auto &s:Par2)trans->dist.Param.push_back(s);
  trans->priority = *$6;
  trans->weight = *$8;
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = false;
  trans->nbServers = 1;
  trans->id = Reader.spn->transitionStruct.size();
  int sz=Reader.spn->TransId.size();
  Reader.spn->TransId[*$2]=sz;
  Reader.spn->transitionStruct.push_back(*trans);

  Par.clear();
  Par2.clear();
 }
| LB str COMMA dist COMMA PRIORITY COMMA WEIGHT COMMA MEMORY RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  transition *trans = new transition();
  trans->name = $2->c_str();
  trans->type = Timed;
  trans->dist.name = distrib;
  for(auto &s:Par2)trans->dist.Param.push_back(s);
  trans->priority = *$6;
  trans->weight = *$8;
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = AgeMemory;
  trans->nbServers = 1;
  trans->id = Reader.spn->transitionStruct.size();
        int sz=Reader.spn->TransId.size();
        Reader.spn->TransId[*$2]=sz;
  Reader.spn->transitionStruct.push_back(*trans);

  Par.clear();
  Par2.clear();
  AgeMemory=false;
 }
| LB str COMMA EXPO LB RealStringFormula RB COMMA PRIORITY COMMA WEIGHT COMMA SERVICE RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(!SingleService) MarkingDependent=true;
  vector<expr> v(1);
  MarkingDependent=$6->is_markDep();
  v[0]= *$6;
  Par.clear();
  Par2.clear();

  transition *trans = new transition();
  trans->name = $2->c_str();
  trans->type = Timed;
  trans->dist.name = "EXPONENTIAL";
  for(auto &s:v)trans->dist.Param.push_back(s);
  trans->priority = *$9;
  trans->weight = *$11;
  trans->singleService = SingleService;
  trans->markingDependant = MarkingDependent;
  trans->ageMemory = false;
  trans->nbServers = NbServers;
  trans->id = Reader.spn->transitionStruct.size();
        int sz=Reader.spn->TransId.size();
        Reader.spn->TransId[*$2]=sz;
  Reader.spn->transitionStruct.push_back(*trans);

    Par.clear();
    Par2.clear();
  MarkingDependent=false;

 }

| LB str COMMA EXPO LB RealStringFormula RB COMMA PRIORITY COMMA WEIGHT COMMA SERVICE COMMA MEMORY RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }

  if(!SingleService) MarkingDependent=true;
  vector<expr> v(1);
  v[0]= *$6;
  MarkingDependent=$6->is_markDep();

  transition *trans = new transition();
  trans->name= $2->c_str();
  trans->type = Timed;
  trans->dist.name = "EXPONENTIAL";
  for(auto &s:v)trans->dist.Param.push_back(s);
  trans->priority = *$9;
  trans->weight = *$11;
  trans->singleService = SingleService;
  trans->markingDependant = MarkingDependent;
  trans->ageMemory = AgeMemory;
  trans->nbServers = NbServers;
  trans->id = Reader.spn->transitionStruct.size();
  Reader.spn->transitionStruct.push_back(*trans);
        int sz=Reader.spn->TransId.size();
        Reader.spn->TransId[*$2]=sz;
  MarkingDependent=false;
  AgeMemory=false;
  Par.clear();
  Par2.clear();
 }

|LB str COMMA IMDT COMMA PRIORITY COMMA WEIGHT  RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }

  transition *trans = new transition();
  trans->name = $2->c_str();
  trans->type = unTimed;
  trans->dist.name = "IMMEDIATE";
  trans->priority = *$6;
  trans->weight = *$8;
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = false;
  trans->nbServers = 1;
  trans->id = Reader.spn->transitionStruct.size();
        int sz=Reader.spn->TransId.size();
        Reader.spn->TransId[*$2]=sz;
  Reader.spn->transitionStruct.push_back(*trans);
  Par.clear();
 };





WEIGHT: RealStringFormula
{expr st=*$1;
    st.eval();
  if( ! st.is_concrete())
    {cout<<"Weight is not marking dependent: '"<<st.to_string() <<"'"<<endl;YYABORT; }
  else{ if(st.get_real()<0)
      {cout<<"Weight is a positive value: '"<<st.to_string() <<"'"<<endl; YYABORT;}
    else{
        $$ = $1;
    }
  }
};
PRIORITY: RealStringFormula
{expr st=*$1;
    st.eval();
  if( ! st.is_concrete())
    {cout<<"Priority is not marking dependent: '"<<st.to_string() <<"'"<<endl;YYABORT; }
  else{ if(st.get_real()<0)
      {cout<<"Priority is a positive value: '"<<st.to_string()<<"'"<<endl; YYABORT;}
    else{
        $$ = $1;
    }
  }
};

SERVICE: SINGLE {SingleService=true; NbServers=1;}
| INFINITE {SingleService=false;NbServers=INT_MAX;}
| MULTIPLE LB ival RB {NbServers= $3;
   if(NbServers<1)
     { cout<<"Number of servers should be at least one"<<endl;
       YYABORT;
     }
   if(NbServers==1) SingleService=true;
   else {SingleService=false;}


 }
| MULTIPLE LB str RB {
  if(Reader.spn->IntConstant.find(*$3)!=Reader.spn->IntConstant.end()){
    NbServers=Reader.spn->IntConstant[*$3];
    if(NbServers<1)
      { cout<<"Number of servers should be at least one"<<endl;
        YYABORT;
      }
    if(NbServers==1) SingleService=true;
    else {SingleService=false;}
  }
  else{cout<<*$3<<" not defined or not defined like an integer constant "<<endl;}

 };

MEMORY: AGEMEMORY{AgeMemory=true;}
| ENABLINGMEMORY{AgeMemory=false;};


dist:str LB params RB {
  distrib=*$1;
  int np=Par.size();
  switch (Reader.IndexDist[distrib]) {
    case UNIFORM:
      {
        if (np != 2){
          cout << "Uniform distribution has two parameters: min and max where 0<=min<max" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        /*
        double p1,p2;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        Par2.push_back(expr(p1));
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        Par2.push_back(expr(p2));
        if ((p1>=p2) || (p1<0)) {
          cout << "In uniform distribution 0 <= min< max " << endl;
          YYABORT;
        }*/
        break;
      }

    case EXPONENTIAL:
      {
        if (np != 1) {
          cout << "exponential distribution has one parameter: Lambda > 0, got "<< np << " parameters "<< distrib <<"[";
          for(const auto &x : Par) cout << x.to_string() << ", ";
          cout << "]" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        
        /*double p1;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        Par2.push_back( expr(p1));
        if (p1<= 0 ) {
          cout << "In exponential distribution Lambda > 0" << endl;
          YYABORT;
        }*/
        break;
      }

    case DETERMINISTIC:
      {
        if (np != 1) {
          cout << "Deterministic distribution has one parameter:  T >= 0" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        /*double p1;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        Par2.push_back(expr(p1));
        if (p1<0 ) {
          cout << "In Deterministic distribution Lambda > 0" << endl;
          YYABORT;
        }*/
        break;
      }

    case LOGNORMAL:
      {
        if (np != 2) {
          cout << "Lognormal distribution has two parameters: mu and sigma^2, where mu > 0 sigma^2 > 0" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        break;
        
        /*double p1,p2;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        Par2.push_back(expr(p1));
        Par2.push_back( expr(p2));
        if ((p1<=0) || (p2<=0)) {
          cout << "In Lognormal distribution mu > 0 sigma^2 > 0" << endl;
          YYABORT;
        }
        break;*/
      }

    case TRIANGLE:
      {
        if (np != 3) {
          cout << "Triangle distribution has three parameters: 0 <= b <= c <= a" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        Par2.push_back(Par[2]);
        break;
        
        /*
        double p1,p2,p3;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        st=Par[2];
        Evaluate.parse(st);p3=Evaluate.RealResult;
        Par2.push_back(expr(p1));
        Par2.push_back(expr(p2));
        Par2.push_back(expr(p3));
        if ((p1<0) || (p2<p1) || (p3<p2)) {
          cout << "In Triangle distribution: 0 <= b <= c<= a" << endl;
          YYABORT;
        }
        break;*/
      }

    case GEOMETRIC:
      {
        if (np != 2) {
          cout << "Geometric distribution has two parameters: 0 <= p <= 1 (probability of success) and T > 0 (duration of one step)" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        break;
        
        /*
        double p1,p2,p3;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        st=Par[2];
        Evaluate.parse(st);p3=Evaluate.RealResult;
        Par2.push_back(expr(p1));
        Par2.push_back(expr(p2));
        Par2.push_back(expr(p3));
        if ((p1<0) || (p1>1) || p2<=0) {
          cout << "In Geometric distribution 0 <= p <= 1 (probability of success) and T > 0 (duration of one step)" << endl;
          YYABORT;
        }
        break;*/
      }
    case ERLANG:
      {
        if (np != 2) {
          cout << "Erlang distribution has two parameters: Shape and Rate, where Shape is a positive integer and Rate > 0" << endl;
          YYABORT;
        }
        
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        break;
        
        /*
        string st=Par[0];
        Evaluate.parse(st);
        double p1 = Evaluate.RealResult;
        if(Evaluate.RealResult != Evaluate.IntResult || Evaluate.IntResult < 1){
          cout << "In Erlang distribution Shape is a positive integer" << endl;
          YYABORT;
        }

        st=Par[1];
        Evaluate.parse(st);
        double p2 = Evaluate.RealResult;
        if (Evaluate.RealResult<=0) {
          cout << "In Erlang distribution Rate > 0" << endl;
          YYABORT;
        }
        Par2.push_back(expr(p1));
        Par2.push_back(expr(p2));
        break;*/
      }
    case GAMMA:
      {
        if (np != 2) {
          cout << "Gamma distribution has two parameters: Shape>0 and Scale>0" << endl;
          YYABORT;
        }
        Par2.push_back(Par[0]);
        Par2.push_back(Par[1]);
        break;
        
        /*
        string st=Par[0];
        Evaluate.parse(st);
        Par2.push_back(expr(Evaluate.RealResult));
        if(Evaluate.RealResult<=0){
          cout << "In Gamma distribution Shape > 0" << endl;
          YYABORT;
        }

        st=Par[1];
        Evaluate.parse(st);
        Par2.push_back(expr(Evaluate.RealResult));
        if (Evaluate.RealResult<=0) {
          cout << "In Gamma distribution Scale > 0" << endl;
          YYABORT;
        }
        break;*/
      }


    default: cout << "\nUnknown distribution !" << endl;
      YYABORT;

  }
 };



params:param
|params COMMA param;


param:RealStringFormula {
    expr st= *$1;
    if(st.is_markDep())
     {cout<<"The exponential distribution is the only marking dependent distribution: '"<<st.to_string()<<"'"<<endl;YYABORT; }
   else{
       Par.push_back(st);
   }
 };


InArcs: in EQ '{' incells '}' SEMICOLON {};

incells: incell {}
|incells incell {};

incell: LB str COMMA str COMMA IntStringFormula RB SEMICOLON {
  if(Reader.spn->PlacesId.find(*$2)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->TransList.find(*$4)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
    expr st = *$6;
    Reader.spn->inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]), arc(st)));
  /*string st=$6;
  if(Evaluate.parse(st)){
    Reader.spn->inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]), arc(st)));
  }else Reader.spn->inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(Evaluate.IntResult)));*/

 }
|LB str COMMA str   RB SEMICOLON {
  if(Reader.spn->PlacesId.find(*$2)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->TransList.find(*$4)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.spn->inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(1)));

 };

OutArcs: out EQ '{' outcells '}' SEMICOLON {};

outcells: outcell {}
|outcells outcell {};

outcell: LB str COMMA str COMMA IntStringFormula RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->PlacesId.find(*$4)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
    
    expr st = *$6;
    Reader.spn->outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$2],Reader.spn->PlacesId[*$4]),arc(st)));
  /*string st=$6;
  if(Evaluate.parse(st)){
          Reader.spn->outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$2],Reader.spn->PlacesId[*$4]),arc(st)));
  }
  else Reader.spn->outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$2],Reader.spn->PlacesId[*$4]),arc(Evaluate.IntResult)));*/

 }
|LB str COMMA str   RB SEMICOLON {
  if(Reader.spn->TransList.find(*$2)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->PlacesId.find(*$4)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.spn->outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$2],Reader.spn->PlacesId[*$4]),arc(1)));
 };



Inhibitors: inhibitor EQ '{' inhibcells '}' SEMICOLON {};

inhibcells: inhibcell {}
|inhibcells inhibcell {};

inhibcell: LB str COMMA str COMMA IntStringFormula RB SEMICOLON {
  if(Reader.spn->PlacesId.find(*$2)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->TransList.find(*$4)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
  /*string st=$6;
  if(Evaluate.parse(st)){
    Reader.spn->inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(st)));
  }
  else Reader.spn->inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(Evaluate.IntResult)));*/
  expr st = *$6;
  Reader.spn->inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(st)));
  
   
   
 }

|LB str COMMA str   RB SEMICOLON {
  if(Reader.spn->PlacesId.find(*$2)==Reader.spn->PlacesId.end())
    {
      std::cout<<"Place: "<<*$2<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.spn->TransList.find(*$4)==Reader.spn->TransList.end())
    {
      std::cout<<"Transition: "<<*$4<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.spn->inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.spn->arckey(Reader.spn->TransId[*$4],Reader.spn->PlacesId[*$2]),arc(1)));

 };



redifinitions: TransitionsDef;

%%

void
gspn::Gspn_parser::error (const gspn::Gspn_parser::location_type& l,
                          const std::string& m)
{
  Reader.error (l, m);
}
