%{
  open Type
  open StateflowType
%}

%token <int> INT
%token <float> FLOAT
%token <string> NAME INTNAME BOOLNAME DOUBLENAME
%token <string> STRING
%token LPAR RPAR
%token PLUS MINUS MULT DIV
%token LSQBRAK RSQBRAK
%token LBRAK RBRAK
%token EOL
%token SEMICOLON COLON PRIME COMMA
%token AND OR
%token NOT
%token BOOL TRUE FALSE
%token CONST
%token EQ SG SL GE LE
%token CTMC MODULE ENDMODULE INIT ENDINIT REWARDS ENDREWARDS FORMULA 
%token ARROW
%token EOF
%token INTKW DOUBLEKW
%token LABEL
%token SEND AFTER
%token EXP
%token FUNCTION ENDFUN

%left OR
%left AND
%left NOT
%left EQ SG SL GE LE
%left PLUS MINUS
%left MULT DIV
%left LPAR RPAR

%start main
%type <StateflowType.simulink_trans_label> main
//%type <string*string> matlabfun

%%

main:
  action EOF { $1 }
   | LSQBRAK boolexp RSQBRAK action EOF {
     begin
       match $4.trigger with  RAction _ -> failwith "Guard on read action not permitted";
       | _ ->{$4 with guard = Some $2}
     end }
   | delay action EOF 
       { if $2.trigger <> Imm then failwith "two trigger for this transition";
	 {$2 with trigger = $1 }}; 
   | delay LSQBRAK boolexp RSQBRAK action EOF 
     { if $5.trigger <> Imm then failwith "two trigger for this transition";
       {$5 with trigger = $1; guard = Some $3 }}; 

delay:
  AFTER LPAR floatexpr COMMA NAME RPAR { Delay($3) };

action:
  pre { {empty_trans_label with trigger = $1 } }
 | pre LBRAK post RBRAK { {$3 with trigger = $1} };

pre:
  NAME { RAction($1)} 
| { Imm }
| LSQBRAK boolexp RSQBRAK { ImmWC($2) };

boolexp:
 NAME cmp NAME { (Printf.sprintf "%s%s%s" $1 $2 $3) }
| NAME cmp FLOAT { (Printf.sprintf "%s%s%f" $1 $2 $3) }
| FLOAT cmp NAME { (Printf.sprintf "%f%s%s" $1 $2 $3) }
| boolexp OR OR boolexp { (Printf.sprintf "(%s || %s)" $1 $4) }
| boolexp AND AND boolexp { (Printf.sprintf "(%s &amp;&amp; %s)" $1 $4) };
| LPAR boolexp RPAR { (Printf.sprintf "(%s)" $2) }

cmp:
EQ EQ {"=="}
| SG {">"}
| SL {"<"}
| GE {">="}
| LE {"<="}
| NOT EQ {"!="}
;


post:
  NAME SEMICOLON post { {$3 with write = ($1)::($3.write) }; }
 | NAME EQ expr SEMICOLON post { {$5 with update = (Printf.sprintf "%s=%s" $1 $3)::($5.update) } }
 | SEND LPAR NAME COMMA NAME RPAR SEMICOLON post {  {$8 with write = ($3::($8.write)) } }
 | funcall SEMICOLON post { {$3 with update =  ($1)::($3.update) } } 
 | {empty_trans_label};
  

expr:
  NAME {$1}
 | INT {Printf.sprintf "%i" $1}
 | FLOAT {Printf.sprintf "%f" $1}
 | expr PLUS expr {Printf.sprintf "%s+%s" $1 $3}
 | expr MULT expr {Printf.sprintf "%s*%s" $1 $3}
 | expr MINUS expr {Printf.sprintf "%s-%s" $1 $3}
 | LPAR expr RPAR { Printf.sprintf "(%s)" $2}
 | expr SL SL expr { Printf.sprintf "%s &lt;&lt; %s" $1 $4}
 | expr OR expr { Printf.sprintf "%s | %s" $1 $3}
 | funcall { $1 }
 

funcall:
 | NAME LPAR RPAR { Printf.sprintf "%s()" $1 }
 | NAME LPAR expr RPAR { Printf.sprintf "%s(%s)" $1 $3}
 | NAME LPAR expr COMMA expr RPAR { Printf.sprintf "%s(%s,%s)" $1 $3 $5};
 | NAME LPAR expr COMMA expr COMMA expr RPAR { Printf.sprintf "%s(%s,%s,%s)" $1 $3 $5 $7};

floatexpr:
  INT {Float (float $1)}
| FLOAT {Float($1)}
| LPAR floatexpr RPAR {$2 }
| floatexpr MULT floatexpr {Mult($1,$3)}
| floatexpr PLUS floatexpr {Plus($1,$3)}
| MINUS floatexpr {Minus(Float 0.0,$2)}
| floatexpr MINUS floatexpr {Minus($1,$3)}
| floatexpr DIV floatexpr {Div($1,$3)}
| NAME {FloatName($1)}
| EXP LPAR floatexpr RPAR {Exp($3)};
| NAME LPAR floatexprlist RPAR {FunCall($1,$3) }

floatexprlist:
  floatexpr COMMA floatexprlist { $1::$3 }
| floatexpr { [$1] }
| {[]}

/*
matlabfun:
  FUNCTION NAME EQ NAME body ENDFUN
  {($4,Printf.sprintf "double %s(){\n\tdouble %s;\n%s\n\treturn %s;\n}" $4 $2 $5 $2)}
 | FUNCTION NAME EQ NAME LPAR arglist RPAR body ENDFUN 
  {($4,Printf.sprintf "double %s(%s){\n\tdouble %s;\n%s\n\treturn %s;\n}" $4 $2 $6 $8 $2)};

arglist:
  NAME COMMA arglist { Printf.sprintf "double $1,$3"}
| NAME { Printf.sprintf "double $1"} 
| {""};

body: {""}
*/
