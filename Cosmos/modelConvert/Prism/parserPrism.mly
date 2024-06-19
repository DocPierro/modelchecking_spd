%{
  open Type
  open PrismType
%}

%token <int> INT
%token <float> FLOAT
%token <string> NAME INTNAME BOOLNAME DOUBLENAME FUNNAME CLOCKNAME
%token <string> STRING
%token <string> XMLTOK
%token LPAR RPAR
%token PLUS MINUS MULT DIV
%token LSQBRAK RSQBRAK
%token EOL
%token SEMICOLON COLON PRIME COMMA QMARK
%token AND OR
%token NOT
%token BOOL TRUE FALSE CLOCK
%token CONST
%token EQ SG SL GE LE
%token RANGE 
%token CTMC PTA MDP DTMC
%token MODULE ENDMODULE INIT ENDINIT REWARDS ENDREWARDS FORMULA INVARIANT ENDINVARIANT
%token PARSEINT PARSEFLOAT PARSEBOOL PARSEDISTR
%token ARROW
%token EOF
%token INTKW DOUBLEKW
%token LABEL
%token FLOOR CEIL


%left OR
%left AND
%left NOT
%left EQ SG SL GE LE
%left PLUS MINUS
%left MULT DIV
%left LPAR RPAR

%start main floatexpr intexpr stateCondition parseCmd
%type <PrismType.prism_file> main
%type <int Type.expr'> intexpr
%type <float Type.expr'> floatexpr
%type <bool Type.expr'> stateCondition
%type <Type.cmdAttr> parseCmd
%%

parseCmd:
| EOF {Close}
| PARSEINT COLON intexpr SEMICOLON {ParseInt(eval $3)}
| PARSEFLOAT COLON floatexpr SEMICOLON {ParseFloat(eval $3)}
| PARSEBOOL COLON stateCondition SEMICOLON {ParseBool(eval $3)}
| PARSEDISTR COLON INTNAME LPAR floatexprlist RPAR SEMICOLON {ParseDistr ($3,$5)};
| PARSEINT COLON XMLTOK SEMICOLON {XMLInt $3}
| PARSEFLOAT COLON XMLTOK SEMICOLON {XMLFloat $3}
| PARSEBOOL COLON XMLTOK SEMICOLON {XMLBool $3};
| PARSEDISTR COLON XMLTOK SEMICOLON {XMLDistr $3};
  
main:
  | CTMC defmod initrew EOF { let consts,modlist,labels,formulas = $2 in
    {consts ; modlist; kind=PrismType.CTMC;labels;formulas }}
  | PTA defmod initrew EOF { let consts,modlist,labels,formulas = $2 in
    {consts ; modlist;  kind=PrismType.PTA;labels;formulas }}
  | MDP defmod initrew EOF { let consts,modlist,labels,formulas = $2 in
    {consts ; modlist;  kind=PrismType.MDP;labels;formulas }}
  | DTMC defmod initrew EOF { let consts,modlist,labels,formulas = $2 in
    {consts ; modlist;  kind=PrismType.DTMC;labels;formulas }}

defmod:
  definition defmod { let (defi1,defd1,defl1,form1) = $1 and ((defi2,defd2),modl,defl2,form2) = $2 in 
		      ((defi1@defi2,defd1@defd2)  ,modl,defl1@defl2,form1@form2) }
  | modulelist defmod { let (defl,modl,lab,form) = $2 in (defl,$1::modl,lab,form) }
  | { (([],[]),[],[],[]) }
;

anyname:
  NAME {$1}
| BOOLNAME {$1}
| INTNAME {$1}
| DOUBLENAME {$1}
| CLOCKNAME {$1}
;

definition:
  CONST INTKW INTNAME EQ intexpr SEMICOLON { add_int $3; [($3,Some $5)] , [],[],[] }
| CONST INTNAME EQ intexpr SEMICOLON     { add_int $2; [($2,Some $4)] , [],[],[] }
| CONST DOUBLEKW DOUBLENAME EQ floatOrInt SEMICOLON { add_double $3; [], [($3,Some $5)],[],[] }
| CONST INTKW INTNAME SEMICOLON  { add_int $3; [($3,None)]    , [],[],[]  }
| CONST DOUBLEKW DOUBLENAME SEMICOLON {       add_int $3; [], [($3,None)],[],[] }
| FORMULA NAME EQ intexpr SEMICOLON { [],[],[],[$2,IntExpr $4]  }
| FORMULA NAME EQ stateCondition SEMICOLON { [],[],[],[$2,BoolExpr $4] }
| FORMULA NAME EQ floatexpr SEMICOLON { [],[],[],[$2,FloatExpr $4] }
| LABEL STRING EQ stateCondition SEMICOLON { [],[],[$2,$4],[] }
;

modulelist:
  MODULE anyname varlist invariant actionlist ENDMODULE {
    Full { name=$2;
      varlist=$3;
      actionlist=$5;
      actionset = find_action $5
    }
  }
  | MODULE anyname EQ anyname LSQBRAK renamelist RSQBRAK ENDMODULE {
    Renaming ($2,$4,$6)
  }
;

invariant:
  INVARIANT stateCondition ENDINVARIANT {};
  | {}


renamelist:
  anyname EQ anyname {[($1,$3)]}
  | anyname EQ anyname COMMA renamelist {($1,$3)::$5}
;

varlist:
  anyname COLON rangevar INIT TRUE SEMICOLON varlist 
  { IntK($1,$3,Int 1)::$7 }  
  | anyname COLON rangevar INIT FALSE SEMICOLON varlist 
      { IntK($1,$3,Int 0)::$7 }  
  | anyname COLON rangevar INIT intexpr SEMICOLON varlist 
      { IntK($1,$3,(eval $5))::$7 }
  | anyname COLON rangevar SEMICOLON varlist 
      { IntK($1,$3,(fst $3))::$5 }
  | anyname COLON CLOCK SEMICOLON varlist { ClockK($1)::$5 }
| {[]}
;

rangevar:
  LSQBRAK intexpr RANGE intexpr RSQBRAK { ($2,$4) }
| BOOL { (Int 0,Int 1) }
;

actionlist:
  headaction stateCondition ARROW tailaction SEMICOLON actionlist
  { ($1,Guard.flatten $2,$4):: $6 }
| {[]}
;

headaction:
  LSQBRAK RSQBRAK {None}
| LSQBRAK anyname RSQBRAK {Some $2}
;

tailaction:
 update {[Float(1.0),$1]}
| floatOrInt COLON update {[$1,$3]}
| floatOrInt COLON update PLUS tailaction {($1,$3)::$5}
; 

floatexpr:
//  INT {Float (float $1)}
| FLOAT {Float($1)}
//| INTNAME {CastInt(IntName $1)}
| LPAR floatexpr RPAR {$2 }
| floatexpr MULT floatexpr {Mult($1,$3)}
| floatexpr MULT intexpr   {Mult($1,(CastInt $3))}
| intexpr   MULT floatexpr {Mult(CastInt $1,$3)}

| floatexpr PLUS floatexpr {Plus($1,$3)}
| floatexpr PLUS intexpr   {Plus($1,(CastInt $3))}
| intexpr   PLUS floatexpr {Plus(CastInt $1,$3)}

| floatexpr MINUS floatexpr {Minus($1,$3)}
| floatexpr MINUS intexpr   {Minus($1,CastInt $3)}
| intexpr   MINUS floatexpr {Minus(CastInt $1,$3)}

| floatexpr DIV floatexpr {Div($1,$3)}
| floatexpr DIV intexpr   {Div($1,CastInt $3)}
| intexpr   DIV floatexpr {Div(CastInt $1,$3)}
| intexpr   DIV intexpr   {Div(CastInt $1,CastInt $3)}

| DOUBLENAME {FloatName($1)}
| CLOCKNAME {FloatName($1)}
| FUNNAME LPAR floatexprlist RPAR {FunCall($1,$3) }
| LPAR stateCondition QMARK floatexpr COLON floatexpr RPAR { If($2,$4,$6) }
| LPAR stateCondition QMARK intexpr   COLON floatexpr RPAR { If($2,CastInt $4,$6) }
| LPAR stateCondition QMARK floatexpr COLON intexpr   RPAR { If($2,$4,CastInt $6) }
;

floatOrInt:
  floatexpr {eval $1};
| intexpr {eval (CastInt $1)}

floatexprlist:
  floatexpr COMMA floatexprlist {$1::$3}
| intexpr COMMA floatexprlist {(CastInt $1)::$3}
| floatexpr { [$1] }
| intexpr { [CastInt $1] }
| { [] }
;
cmp:
  EQ {EQ}
 | SG {SG}
 | SL {SL}
 | GE {GE}
 | LE {LE}
 | NOT EQ {NEQ}
;

update:
  TRUE { [] } 
| LPAR upatom RPAR { [$2] }
| LPAR upatom RPAR AND update {$2::$5}
;

upatom:
  anyname PRIME EQ intexpr {$1,IntUp(eval $4)}
| anyname PRIME EQ stateCondition {$1,BoolUp(eval $4)}
;

initrew: 
  INIT TRUE ENDINIT initrew {()}
| REWARDS STRING actionrewardlist ENDREWARDS initrew {()}
| REWARDS actionrewardlist ENDREWARDS initrew {()}
| {()}
;


actionrewardlist:
LSQBRAK NAME RSQBRAK stateCondition COLON floatOrInt SEMICOLON actionrewardlist
  {()}
| LSQBRAK RSQBRAK stateCondition COLON floatOrInt SEMICOLON actionrewardlist
  {()}
|  stateCondition COLON floatOrInt SEMICOLON actionrewardlist
  {()}
| {()}
;

stateCondition:
TRUE {Bool true}
| FALSE {Bool false}
| BOOLNAME { BoolName($1) }
| stateCondition AND stateCondition {And($1,$3)}
| stateCondition OR stateCondition {Or($1,$3)}
| stateCondition EQ SG stateCondition {Or(Not $1,$4)}
| NOT stateCondition {Not($2)}
| LPAR stateCondition RPAR {$2}
| stateCondition EQ stateCondition {eval @@ BoolAtom($1,EQ, $3) }
| stateCondition NOT EQ stateCondition {eval @@ BoolAtom($1,NEQ,$4) }
| intexpr cmp intexpr  { IntAtom(eval $1,$2,eval $3) }
| floatexpr cmp floatexpr  { FloatAtom(eval $1,$2,eval $3) }
| floatexpr cmp intexpr  { FloatAtom(eval $1,$2,CastInt (eval $3)) }
| intexpr cmp floatexpr  { FloatAtom(CastInt(eval $1),$2,eval $3) }
| LPAR stateCondition QMARK stateCondition COLON stateCondition RPAR { If($2,$4,$6) }
;

intexpr:
  INT {Int($1)}
 | INTNAME {IntName($1)}
 | LPAR intexpr RPAR {$2}
 | intexpr PLUS intexpr {Plus($1,$3)}
 | intexpr MULT intexpr {Mult($1,$3)}
 | intexpr MINUS intexpr {Minus($1,$3)}
 | NOT intexpr {Minus(Int 1,$2)}
 | FLOOR LPAR floatexpr RPAR {Floor($3)}
 | CEIL LPAR floatexpr RPAR {Ceil($3)}
 | LPAR stateCondition QMARK intexpr COLON intexpr RPAR { If($2,$4,$6) }
// | FUNNAME LPAR intexprlist RPAR { FunCall($1,$3) }
;
/*
intexprlist:
| intexpr COMMA intexprlist { $1::$3 }
| intexpr { [$1] }
| { [] }
;

*/
