%{
  open Type

%}

%token <int> INT
%token <float> FLOAT
%token <string> NAME
%token TRUE FALSE
%token LPAR RPAR
%token PLUS MINUS MULT DIV
%token LSQBRAK RSQBRAK
%token EOL
%token SEMICOLON
%token UNTIL
%token EVENTUALLY
%token NEXT
%token AND OR
%token NOT
%token EQ SG SL GE LE

%left OR
%left AND
%left NOT
%left EQ SG SL GE LE
%left PLUS MINUS
%left MULT
%left LPAR RPAR

%start main
%type <Type.formula> main

%%

main:
  expr EOL {$1}
;

expr:
  stateCondition UNTIL stateCondition {Until($1,$3) }
 | stateCondition UNTIL timeinterval stateCondition {BoundedUntil($1,$3,$4)}
 | EVENTUALLY stateCondition {Until(True,$2) }
 | EVENTUALLY timeinterval stateCondition {BoundedUntil(True,$2,$3) }
 | NEXT stateCondition {Next($2)}
 | NEXT timeinterval stateCondition {BoundedNext($2,$3)}
 | LPAR expr RPAR {$2}
 | expr AND expr {AndF($1,$3)}
 | expr OR expr {OrF($1,$3)}
;


timeinterval:
   LE floatexpr {(Float(0.0),$2)}
 | SL floatexpr {(Float(0.0),$2)}
 | EQ floatexpr {($2,$2)}
 | SG floatexpr {($2,Infty)}
 | GE floatexpr {($2,Infty)}
 | LSQBRAK floatexpr SEMICOLON floatexpr RSQBRAK {($2,$4)}

stateCondition:
  TRUE    {True}
 | FALSE   {False}
 | stateCondition AND stateCondition {And($1,$3)}
 | stateCondition OR stateCondition {Or($1,$3)}
 | NOT stateCondition {Not($2)}
 | LPAR stateCondition RPAR {$2}
 | intexpr cmp intexpr  { IntAtom($1,$2,$3) }
;

cmp:
  EQ {EQ}
 | SG {SG}
 | SL {SL}
 | GE {GE}
 | LE {LE}
;
  
intexpr:
  INT {Int($1)}
 | NAME {IntName($1)}
 | LPAR intexpr RPAR {$2}
 | intexpr PLUS intexpr {Plus($1,$3)}
 | intexpr MULT intexpr {Mult($1,$3)}
 | intexpr MINUS intexpr {Minus($1,$3)}
;

floatexpr:
  FLOAT  {Float($1)}
 | INT {Float(float_of_int $1)}
;
