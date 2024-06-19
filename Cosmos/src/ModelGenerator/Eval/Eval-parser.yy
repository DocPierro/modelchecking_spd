%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4"
%defines
%define api.parser.class {Eval_parser}

%code requires {
#include <math.h>
#include <limits.h>
# include <string>

#include <fstream>
#include <sstream>
#include <set>
class Eval;

using namespace std;

}

// The parsing context.
%parse-param { Eval& Evaluate }
%lex-param   { Eval& Evaluate }

%locations

%debug
%define parse.error verbose

// Symbols.
%union
{
  double       RealVal;
  int          IntVal;
  std::string *name;

};

%code {
#include "Eval.hpp"
#include <set>
#include <vector>

}

%token        END      0 "end of file"

%token <name>    str
%token <RealVal>  rval
%token <IntVal>  ival

%token <name>     MIN
%token <name>     MAX
%token <name>     FLOOR
%token <name>     PLUS
%token <name>     MINUS
%token <name>     MUL
%token <name>     DIV
%token <name>     POWER
%token <name>     LB
%token <name>     RB
%token <name>     COMMA
%token <name>     DOUBLE

%type<RealVal>	exp



%printer    { debug_stream () << *$$; } str
%destructor { delete $$; } str

%printer    { debug_stream () << $$; } <IntVal>
%printer    { debug_stream () << $$; } <RealVal>

%%

%left PLUS MINUS;
%left NEG;
%left MUL  DIV;
%left POWER;
%left DOUBLE;
%start ArithmethicExp;
ArithmethicExp:exp {Evaluate.IntResult=(int) $1;Evaluate.RealResult=$1;};
exp:
        ival {$$=$1;}
        |rval {$$=$1;}
        |LB exp RB {$$=$2;}
        |MINUS exp %prec NEG {$$=-$2;}
        |exp PLUS exp{$$=$1+$3;}
        |exp MUL exp{$$=$1*$3;}
        |exp MINUS exp{$$=$1-$3;}
        |DOUBLE exp {$$=double($2);}
        |exp DIV exp	{if($3==0)        {cout<<"Division by zero !";YYABORT;}
                                         else $$=$1/double $3;
                                        }
        |exp POWER exp {$$=pow($1,$3);}
        |MIN   LB exp COMMA exp RB	{if($3<=$5) $$=$3; else $$=$5;}
        |MAX   LB exp COMMA exp RB	{if($3>=$5) $$=$3;       else $$=$5;}
        |FLOOR LB exp  RB			{$$=floor($3);}

%%

void
eval::Eval_parser::error (const eval::Eval_parser::location_type& l,
                          const std::string& m)
{
 Evaluate.error (l, m);
}
