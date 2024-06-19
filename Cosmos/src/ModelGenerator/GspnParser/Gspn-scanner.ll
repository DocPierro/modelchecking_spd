%{                                            /* -*- C++ -*- */
# include <cstdlib>
# include <errno.h>
# include <limits.h>
# include <string>
# include "Gspn-Reader.hpp"
//# include "Gspn-parser.tab.hh"


# undef yywrap
# define yywrap() 1

# define YY_DECL GSPN_DECL

/* By default gspnlex returns int, we use token_type.
   Unfortunately gspnterminate by default returns 0, which is
   not of token_type.  */
#define yyterminate() return token::END
#ifndef __clang_analyzer__
%}

%option noyywrap nounput batch debug

STR [a-zA-Z][a-zA-Z_0-9]*
INT [0-9]+|[0-9]*
REAL ([0-9]+|[0-9]*\.[0-9]+)([eE][-+]?[0-9]+)? 
blank [ \t]


%{
# define GSPN_USER_ACTION  gspnlloc->columns (gspnleng);
%}
%%
%{
  gspnlloc->step ();
%}
{blank}+   gspnlloc->step ();
[\n]+      gspnlloc->lines (gspnleng); gspnlloc->step ();
[\%][^\n]*[\n]

%{
  typedef gspn::Gspn_parser::token token;
%}
           /* Convert ints to the actual type of tokens.  */
[{}]     return gspn::Gspn_parser::token_type (gspntext[0]);

";" {gspnlval->name = new std::string (gspntext); return token::SEMICOLON;
            Reader.error (*gspnlloc, "; expected");} 

"," {gspnlval->name = new std::string (gspntext); return token::COMMA;
            Reader.error (*gspnlloc, ", expected");}

"(" {gspnlval->name = new std::string (gspntext); return token::LB;
            Reader.error (*gspnlloc, "( expected");}
")" {gspnlval->name = new std::string (gspntext); return token::RB;
            Reader.error (*gspnlloc, ") expected");}

"=" {gspnlval->name = new std::string (gspntext); return token::EQ;
            Reader.error (*gspnlloc, "= expected");}

"/" {gspnlval->name = new std::string (gspntext); return token::DIV;
            Reader.error (*gspnlloc, "/ expected");}
"*" {gspnlval->name = new std::string (gspntext); return token::MUL;
            Reader.error (*gspnlloc, "* expected");}
"+" {gspnlval->name = new std::string (gspntext); return token::PLUS;
            Reader.error (*gspnlloc, "+ expected");}
"-" {gspnlval->name = new std::string (gspntext); return token::MINUS;
            Reader.error (*gspnlloc, "- expected");}
"^" {gspnlval->name = new std::string (gspntext); return token::POWER;
            Reader.error (*gspnlloc, "^ expected");}

"GspnName" {gspnlval->name = new std::string (gspntext); return token::GspnName;
            Reader.error (*gspnlloc, "GspnName expected");}



"NbPlaces" {gspnlval->name = new std::string (gspntext); return token::NbPl;
            Reader.error (*gspnlloc, "NbPlces expected");}

"NbTransitions" {gspnlval->name = new std::string (gspntext); return token::NbTr;
            Reader.error (*gspnlloc, "NbTransitions expected");}

"PlacesList" {gspnlval->name = new std::string (gspntext); return token::PList;
            Reader.error (*gspnlloc, "PlacesList expected");}

"TransitionsList" {gspnlval->name = new std::string (gspntext); return token::TList;
            Reader.error (*gspnlloc, "TransitionsList expected");}

"Marking" {gspnlval->name = new std::string (gspntext); return token::marking;
            Reader.error (*gspnlloc, "marking expected");}

"Transitions" {gspnlval->name = new std::string (gspntext); return token::transitions;
            Reader.error (*gspnlloc, "Transitions expected");}

"InArcs" {gspnlval->name = new std::string (gspntext); return token::in;
            Reader.error (*gspnlloc, "InArcs expected");}

"OutArcs" {gspnlval->name = new std::string (gspntext); return token::out;
            Reader.error (*gspnlloc, "OutArcs expected");}

"InhibitorArcs" {gspnlval->name = new std::string (gspntext); return token::inhibitor;
            Reader.error (*gspnlloc, "InhibitorArcs expected");}





"min" {gspnlval->name = new std::string (gspntext); return token::MIN;
            Reader.error (*gspnlloc, "min expected");}

"max" {gspnlval->name = new std::string (gspntext); return token::MAX;
            Reader.error (*gspnlloc, "max expected");}

"const" {gspnlval->name = new std::string (gspntext); return token::Const;
            Reader.error (*gspnlloc, "Const expected");}

"int" {gspnlval->name = new std::string (gspntext); return token::INT;
            Reader.error (*gspnlloc, "int expected");}

"double" {gspnlval->name = new std::string (gspntext); return token::DOUBLE;
            Reader.error (*gspnlloc, "double expected");}

"floor" {gspnlval->name = new std::string (gspntext); return token::FLOOR;
            Reader.error (*gspnlloc, "floor expected");}

"SINGLE" {gspnlval->name = new std::string (gspntext); return token::SINGLE;
            Reader.error (*gspnlloc, "SINGLE expected");}

"MULTIPLE" {gspnlval->name = new std::string (gspntext); return token::MULTIPLE;
            Reader.error (*gspnlloc, "MULTIPLE expected");}

"INFINITE" {gspnlval->name = new std::string (gspntext); return token::INFINITE;
            Reader.error (*gspnlloc, "INFINITE expected");}

"ENABLINGMEMORY" {gspnlval->name = new std::string (gspntext); return token::ENABLINGMEMORY;
            Reader.error (*gspnlloc, "ENABLINGMEMORY expected");}

"AGEMEMORY" {gspnlval->name = new std::string (gspntext); return token::AGEMEMORY;
            Reader.error (*gspnlloc, "AGEMEMORY expected");}

"IMMEDIATE" {gspnlval->name = new std::string (gspntext); return token::IMDT;
            Reader.error (*gspnlloc, "IMMEDIATE expected");}
"EXPONENTIAL" {gspnlval->name = new std::string (gspntext); return token::EXPO;
            Reader.error (*gspnlloc, "EXPONENTIAL expected");}




{INT}      {
  errno = 0; 
  int x=atoi(gspntext);
  gspnlval->IntVal = x;
  return token::ival;
}
 
{REAL}      {
  errno = 0; 
  double x=atof(gspntext);
  gspnlval->RealVal = x;
  return token::rval;
}


{STR}       {gspnlval->name = new std::string (gspntext); return token::str;
            Reader.error (*gspnlloc, "invalid character");}

%%




void
Gspn_Reader::scan_end ()
{  
  gspnlex_destroy();
}


void Gspn_Reader::scan_expression (const std::string& f){
  const char* ch=f.c_str();
gspnset_debug(trace_scanning);

// GSPN_BUFFER_STATE b=gspn_scan_string(ch);
// gspn_switch_to_buffer(b);
 gspn_scan_string(ch);
 
}
#endif // not __clang_analyzer__

