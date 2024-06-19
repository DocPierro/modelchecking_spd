%{                                            /* -*- C++ -*- */
# include <cstdlib>
# include <errno.h>
# include <limits.h>
# include <string>
# include "Lha-Reader.hpp"

# undef yywrap
# define yywrap() 1

# define YY_DECL LHA_DECL

/* By default lhalex returns int, we use token_type.
   Unfortunately lhaterminate by default returns 0, which is
   not of token_type.  */
#define yyterminate() return token::END
#ifndef __clang_analyzer__

%}

%option noyywrap nounput batch debug

STR [a-zA-Z][a-zA-Z_$.0-9]*
GENERALSTR [a-zA-Z][a-zA-Z_$\[,\]0-9]*
INT [0-9]+|[0-9]*
REAL ([0-9]+|[0-9]*\.[0-9]+)([eE][-+]?[0-9]+)? 
blank [ \t]


%{
# define LHA_USER_ACTION  lhalloc->columns (lhaleng);
%}
%%
%{
  lhalloc->step ();
%}
{blank}+   lhalloc->step ();
[\n]+      lhalloc->lines (lhaleng); lhalloc->step ();
[\%][^\n]*[\n] 

%{
  typedef lha::Lha_parser::token token;
%}
           /* Convert ints to the actual type of tokens.  */
[{}]     return lha::Lha_parser::token_type (lhatext[0]);

";" {lhalval->name = new std::string (lhatext); return token::SEMICOLON;
            Reader.error (*lhalloc, "; expected");} 

"," {lhalval->name = new std::string (lhatext); return token::COMMA;
            Reader.error (*lhalloc, ", expected");}

"#" {lhalval->name = new std::string (lhatext); return token::SHARP;
            Reader.error (*lhalloc, "# expected");}

":" {lhalval->name = new std::string (lhatext); return token::COLON;
            Reader.error (*lhalloc, ": expected");}

"\\" {lhalval->name = new std::string (lhatext); return token::BackSlash;
            Reader.error (*lhalloc, ": expected");}

"(" {lhalval->name = new std::string (lhatext); return token::LB;
            Reader.error (*lhalloc, "( expected");}
")" {lhalval->name = new std::string (lhatext); return token::RB;
            Reader.error (*lhalloc, ") expected");}
"[" {lhalval->name = new std::string (lhatext); return token::LSB;
            Reader.error (*lhalloc, "[ expected");}
"]" {lhalval->name = new std::string (lhatext); return token::RSB;
            Reader.error (*lhalloc, "] expected");}
"&" {lhalval->name = new std::string (lhatext); return token::AND;
            Reader.error (*lhalloc, "& expected");}
"|" {lhalval->name = new std::string (lhatext); return token::OR;
            Reader.error (*lhalloc, "& expected");}
"!" {lhalval->name = new std::string (lhatext); return token::NOT;
            Reader.error (*lhalloc, "& expected");}

"<=" {lhalval->name = new std::string (lhatext); return token::LEQ;
            Reader.error (*lhalloc, "<= expected");}
">=" {lhalval->name = new std::string (lhatext); return token::GEQ;
            Reader.error (*lhalloc, ">= expected");}

"=" {lhalval->name = new std::string (lhatext); return token::EQ;
            Reader.error (*lhalloc, "= expected");}
"<" {lhalval->name = new std::string (lhatext); return token::LL;
            Reader.error (*lhalloc, "< expected");}
">" {lhalval->name = new std::string (lhatext); return token::GG;
            Reader.error (*lhalloc, "> expected");}


"/" {lhalval->name = new std::string (lhatext); return token::DIV;
            Reader.error (*lhalloc, "/ expected");}
"*" {lhalval->name = new std::string (lhatext); return token::MUL;
            Reader.error (*lhalloc, "* expected");}
"+" {lhalval->name = new std::string (lhatext); return token::PLUS;
            Reader.error (*lhalloc, "+ expected");}
"-" {lhalval->name = new std::string (lhatext); return token::MINUS;
            Reader.error (*lhalloc, "- expected");}
"^" {lhalval->name = new std::string (lhatext); return token::POWER;
            Reader.error (*lhalloc, "^ expected");}

"LhaName" {lhalval->name = new std::string (lhatext); return token::LhaName;
            Reader.error (*lhalloc, "LhaName expected");}



"const" {lhalval->name = new std::string (lhatext); return token::Const;
            Reader.error (*lhalloc, "const expected");}

"hybrid" {lhalval->name = new std::string (lhatext); return token::Hybrid;
			Reader.error (*lhalloc, "hybrid expected");}

"NbLocations" {lhalval->name = new std::string (lhatext); return token::NbLoc;
            Reader.error (*lhalloc, "NbLocations expected");}

"NbVariables" {lhalval->name = new std::string (lhatext); return token::NbVar;
            Reader.error (*lhalloc, "NbVariables expected");}

"VariablesList" {lhalval->name = new std::string (lhatext); return token::VList;
            Reader.error (*lhalloc, "VariablesList expected");}

"LocationsList" {lhalval->name = new std::string (lhatext); return token::LList;
            Reader.error (*lhalloc, "LocationsList expected");}

"InitialLocations" {lhalval->name = new std::string (lhatext); return token::Iloc;
            Reader.error (*lhalloc, "InitialLocations expected");}

"FinalLocations" {lhalval->name = new std::string (lhatext); return token::Floc;
            Reader.error (*lhalloc, "FinalLocations expected");}

"Locations" {lhalval->name = new std::string (lhatext); return token::locations;
            Reader.error (*lhalloc, "Locations expected");}

"Edges" {lhalval->name = new std::string (lhatext); return token::edges;
            Reader.error (*lhalloc, "Edges expected");}



"int" {lhalval->name = new std::string (lhatext); return token::INT;
            Reader.error (*lhalloc, "int expected");}

"double" {lhalval->name = new std::string (lhatext); return token::DOUBLE;
            Reader.error (*lhalloc, "double expected");}

"floor" {lhalval->name = new std::string (lhatext); return token::FLOOR;
            Reader.error (*lhalloc, "floor expected");}


"min" {lhalval->name = new std::string (lhatext); return token::MIN;
            Reader.error (*lhalloc, "min expected");}

"max" {lhalval->name = new std::string (lhatext); return token::MAX;
            Reader.error (*lhalloc, "max expected");}

"mod" {lhalval->name = new std::string (lhatext); return token::MOD;
            Reader.error (*lhalloc, "mod expected");}

"TRUE" {lhalval->name = new std::string (lhatext); return token::TRUE;
            Reader.error (*lhalloc, "TRUE expected");}

"FALSE" {lhalval->name = new std::string (lhatext); return token::FALSE;
            Reader.error (*lhalloc, "FALSE expected");}

"ALL" {lhalval->name = new std::string (lhatext); return token::ALL;
            Reader.error (*lhalloc, "ALL expected");}

"Last" {lhalval->name = new std::string (lhatext); return token::LAST;
            Reader.error (*lhalloc, "Last expected");}

"Min" {lhalval->name = new std::string (lhatext); return token::LhaMIN;
            Reader.error (*lhalloc, "Min expected");}

"Max" {lhalval->name = new std::string (lhatext); return token::LhaMAX;
            Reader.error (*lhalloc, "Max expected");}

"AVG" {lhalval->name = new std::string (lhatext); return token::AVG;
            Reader.error (*lhalloc, "AVG expected");}

"VAR" {lhalval->name = new std::string (lhatext); return token::VAR;
	Reader.error (*lhalloc, "VAR expected");}

"PROB" {lhalval->name = new std::string (lhatext); return token::PROB;
	Reader.error (*lhalloc, "PROB expected");}

"EXISTS" {lhalval->name = new std::string (lhatext); return token::EXIST_TOK;
Reader.error (*lhalloc, "EXISTS expected");}

"NOTALLS" {lhalval->name = new std::string (lhatext); return token::NOTALL_TOK;
Reader.error (*lhalloc, "NOTALLS expected");}

"HYPOTHESIS" {lhalval->name = new std::string (lhatext); return token::SPRT;
	Reader.error (*lhalloc, "HYPOTHESIS expected");}

"Integral" {lhalval->name = new std::string (lhatext); return token::INTEGRAL;
            Reader.error (*lhalloc, "Integral expected");}

"Mean" {lhalval->name = new std::string (lhatext); return token::MEAN;
	Reader.error (*lhalloc, "Mean expected");}

"PDF" {lhalval->name = new std::string (lhatext); return token::PDF;
            Reader.error (*lhalloc, "PDF expected");}

"CDF" {lhalval->name = new std::string (lhatext); return token::CDF;
	Reader.error (*lhalloc, "CDF expected");}
	
"DISC" {lhalval->name = new std::string (lhatext); return token::DISC;
	Reader.error (*lhalloc, "DISC expected");}

"NOTDETERMINISTIC" {lhalval->name = new std::string (lhatext); return token::NOTDET;
Reader.error (*lhalloc, "NOTDETERMINISTIC expected");}


{INT}      {
  errno = 0; 
  int x=atoi(lhatext);
  lhalval->IntVal = x;
  return token::ival;
}
 
{REAL}      {
  errno = 0; 
  double x=atof(lhatext);
  lhalval->RealVal = x;
  return token::rval;
}


{STR}       {lhalval->name = new std::string (lhatext); return token::str;
            Reader.error (*lhalloc, "invalid character");}

%%




void
Lha_Reader::scan_end ()
{  
  lhalex_destroy();
}


void Lha_Reader::scan_expression (const string& f){
  const char* ch=f.c_str();
lhaset_debug(trace_scanning);

// lha_BUFFER_STATE b=lha_scan_string(ch);
// lha_switch_to_buffer(b);
 lha_scan_string(ch);
 
}
#endif // not __clang_analyzer__
