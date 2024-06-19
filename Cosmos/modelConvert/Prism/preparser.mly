%{
  open Type
  open PrismType
%}

%token <int> INT
%token <float> FLOAT
%token <string> NAME INTNAME BOOLNAME DOUBLENAME
%token <string> STRING
%token LPAR RPAR
%token PLUS MINUS MULT DIV
%token LSQBRAK RSQBRAK
%token EOL
%token SEMICOLON COLON PRIME COMMA QMARK
%token AND OR
%token NOT
%token CLOCK
%token BOOL TRUE FALSE
%token CONST
%token NEQ EQ SG SL GE LE
%token RANGE 
%token CTMC MODULE ENDMODULE INIT ENDINIT REWARDS ENDREWARDS FORMULA INVARIANT ENDINVARIANT
%token ARROW
%token EOF
%token INTKW DOUBLEKW
%token LABEL
%token FLOOR CEIL

%left OR
%left AND
%left NOT
%left PLUS MINUS
%left MULT DIV
%left LPAR RPAR
%left EQ SG SL GE LE

%start main
%type <unit> main
%%

main:
  CTMC defmod {($2)};

defmod:
  definition defmod { }
  | modulelist defmod { }
  | INIT TRUE ENDINIT defmod {}
  | REWARDS actionrewardlist ENDREWARDS defmod {}      
  | REWARDS STRING actionrewardlist ENDREWARDS defmod {}      
  | LABEL STRING EQ expr SEMICOLON defmod {}
  | EOF {}
;

definition:
  CONST INTKW NAME EQ expr SEMICOLON { add_int $3 }
  | CONST NAME EQ expr SEMICOLON     { add_int $2  }
  | CONST DOUBLEKW NAME EQ expr SEMICOLON { add_double $3 }
  | CONST INTKW NAME SEMICOLON  {         add_int $3 }
  | CONST DOUBLEKW NAME SEMICOLON {       add_int $3 }
  | FORMULA NAME EQ expr SEMICOLON {  }
;

modulelist:
  MODULE NAME varlist actionlist ENDMODULE { }
  |  MODULE NAME varlist invariant actionlist ENDMODULE { }
  | MODULE NAME EQ NAME LSQBRAK renamelist RSQBRAK ENDMODULE { }
;

invariant:
  INVARIANT expr ENDINVARIANT {};

renamelist:
  NAME EQ NAME { add_copy $1 $3 }
  | NAME EQ NAME COMMA renamelist { add_copy $1 $3 }
;

varlist:
  | NAME COLON rangevar INIT expr SEMICOLON varlist { add_var $1 $3 }
  | NAME COLON rangevar SEMICOLON varlist { add_var $1 $3 }
  | NAME COLON CLOCK SEMICOLON varlist { add_var $1 Clock }
  | {}
;

rangevar:
  LSQBRAK expr RANGE expr RSQBRAK { IntVar }
| BOOL { BoolVar }
;


actionlist:
  headaction expr ARROW tailaction SEMICOLON actionlist {}
| {}
;

headaction:
  LSQBRAK RSQBRAK {}
| LSQBRAK NAME RSQBRAK {}
;

tailaction:
 update {}
| expr COLON update {}
| expr COLON update PLUS tailaction {}
; 

cmp:
  EQ {EQ}
 | SG {SG}
 | SL {SL}
 | GE {GE}
 | LE {LE}
 | NEQ {NEQ}
;

update:
  TRUE { } 
| LPAR upatom RPAR { }
| LPAR upatom RPAR AND update {}
;

upatom:
  NAME PRIME EQ expr {}
;

actionrewardlist:
LSQBRAK NAME RSQBRAK expr COLON expr SEMICOLON actionrewardlist
  {()}
| LSQBRAK RSQBRAK expr COLON expr SEMICOLON actionrewardlist
  {()}
| expr COLON expr SEMICOLON actionrewardlist
  {()}
| {()}
;

expr:
  INT expr {}
| FLOAT expr {}
| NAME expr {}
| LPAR expr RPAR expr {}
| LPAR expr QMARK expr COLON expr RPAR expr {}
| MULT expr {}
| PLUS expr {}
| MINUS expr {}
| DIV expr {}
| NOT expr {}
| FLOOR expr {}
| CEIL expr {}
| TRUE expr {}
| FALSE expr {}
| AND expr {}
| OR expr {}
| cmp expr {}
| COMMA expr {}
| {}
;
