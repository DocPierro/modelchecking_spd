%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4"
%defines
%define api.parser.class {Lha_parser}

%code requires {
        #include <math.h>
        #include <limits.h>
        # include <string>

        #include <fstream>
        #include <sstream>
        #include <set>
        #include "../Eval/Eval.hpp"
        #include "../HaslFormula.hpp"

        class Lha_Reader;


        #define BUFF_SIZE 5000
}

// The parsing context.
%parse-param { Lha_Reader& Reader }
%lex-param   { Lha_Reader& Reader }

%locations

%debug
%define parse.error verbose

// Symbols.
%union
{
        double       RealVal;
        int            IntVal;
        std::string *name;
        char expression[BUFF_SIZE];
        HaslFormulasTop *TOPHASL;
};

%code {
        #include "../HaslFormula.hpp"
        #include "Lha-Reader.hpp"
        #include <set>
        #include <vector>

        Eval LhaEvaluate;



        vector<string> FuncFlowVector;

        vector<string> FuncUpdateVector;
    vector<string> FuncUpdateVectorIndex;

        set <string> PetriTransitions;
        set <string> SubSet;



        vector < string > CoeffsVector;
        vector < vector <string> > CoeffsMatrix;
        vector <string> CST;
        vector <string> comp;






}

%token        END      0 "end of file"

%token <name>     str
%token <RealVal>  rval
%token <IntVal>  ival

%token <name>     SEMICOLON
%token <name>     COLON
%token <name>     COMMA
%token <name>     LB
%token <name>     RB
%token <name>     LSB
%token <name>     RSB

%token <name>     SHARP
%token <name>     ALL
%token <name>     BackSlash

%token <name>     EQ
%token <name>     GEQ
%token <name>     LEQ
%token <name>     LL
%token <name>     GG

%token <name>     AND
%token <name>     OR
%token <name>     NOT

%token <name>     TRUE
%token <name>     FALSE

%token <name>     DOUBLE
%token <name>     INT

%token <name>     DIV
%token <name>     MUL
%token <name>     PLUS
%token <name>     MINUS
%token <name>     POWER
%token <name>     FLOOR

%token <name>     MIN
%token <name>     MAX
%token <name>     MOD

%token <name>     LhaMIN
%token <name>     LhaMAX
%token <name>     AVG
%token <name>     VAR
%token <name>     PROB
%token <name>     SPRT
%token <name>     LAST
%token <name>     INTEGRAL
%token <name>     MEAN
%token <name>     DISC

%token <name>     NOTDET

%token <name>     LhaName

%token <name>     Const
%token <name>     Hybrid

%token <name>     NbLoc
%token <name>     NbVar

%token <name>     VList
%token <name>     LList

%token <name>     Iloc
%token <name>     Floc

%token <name>     locations
%token <name>     edges

%token <name>     PDF
%token <namd>     CDF

%token <name>     EXIST_TOK
%token <name>     NOTALL_TOK

%type<expression> IntMarkingFormula
%type<expression> RealMarkingFormula
%type<expression> RealVarMarkingFormula
%type<expression> ColorClassList
%type<expression> LogExpr
%type<expression> CompExpr

%type<expression> AlgExpr
%type<expression> LhaFunc
%type<expression> LinForm
%type<expression> VarTerm

%type<TOPHASL> TopHaslExp
%type<RealVal> rorival

%printer    { debug_stream () << *$$; } str
%destructor { delete $$; } str

%printer    { debug_stream () << $$; } <IntVal>
%printer    { debug_stream () << $$; } <RealVal>

%%
%left OR;
%left AND;
%left NOT;
%left LEQ GEQ LL GG EQ;
%left PLUS MINUS;
%left NEG;
%left MUL  DIV;
%left POWER;
%left LB RB;


%start LHA;

LHA:
| NOTDET declarations InitFinal definitions {Reader.MyLha.isDeterministic=false;}
    | declarations InitFinal definitions
        | HaslExps;

declarations:
| Sizes Constants HybridVars Lists HaslExps
| Sizes HybridVars Lists HaslExps
| Sizes Constants Lists HaslExps
| Sizes  Lists HaslExps;




IntMarkingFormula:
ival {snprintf($$,BUFF_SIZE,"%i",$1);}
| str {
        if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<< $1->c_str()<<" ";
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        }
        else if(Reader.MyLha.LhaIntConstant.find(*$1)!=Reader.MyLha.LhaIntConstant.end())
        {std::ostringstream s; s<<Reader.MyLha.LhaIntConstant[*$1];
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        } else {
        size_t vararray = Reader.MyLha.Vars.find(*$1);
        if(vararray!= Reader.MyLha.NbVar && Reader.MyLha.Vars.type[vararray]==DISCRETE_VARIABLE){
            snprintf($$,BUFF_SIZE, " floor(Vars->%s)", Reader.MyLha.Vars.label[vararray].c_str() );
        } else {cout<<"'"<<*$1<<"' is not a place label or a defined constant"<<endl;YYABORT;}
    }
}
| str LSB ColorClassList RSB {
    auto pi = Reader.MyLha.PlaceIndex.find(*$1);
        if( pi !=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s;
         s<<" Marking.P->_PL_"<< $1->c_str()<< ".mult";
     if(P.is_domain_impl_set){
         size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
         s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << $3 << "))";
     } else s << $3;
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*$1<<"' is not a place label "<<endl;YYABORT;}}
| str LSB IntMarkingFormula RSB {
    size_t vararray = Reader.MyLha.Vars.find(*$1);
        if(vararray!= Reader.MyLha.NbVar && Reader.MyLha.Vars.type[vararray]==INT_INDEXED_DISC_ARRAY)
        {std::ostringstream s;
        s<<" Vars.P->" << Reader.MyLha.Vars.label[vararray] << "["<< *$3 << "]";
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*$1<<"' is not a discrete array name "<<endl;YYABORT;}}
| LB IntMarkingFormula RB{snprintf($$,BUFF_SIZE,"( %s )", $2);       }
| MINUS IntMarkingFormula %prec NEG {snprintf($$,BUFF_SIZE,"-%s",$2);}
| IntMarkingFormula PLUS  IntMarkingFormula   {snprintf($$,BUFF_SIZE,"%s + %s", $1, $3);  }
| IntMarkingFormula MINUS IntMarkingFormula   {snprintf($$,BUFF_SIZE,"%s - %s", $1, $3);  }
| IntMarkingFormula MUL   IntMarkingFormula   {snprintf($$,BUFF_SIZE,"%s * %s", $1, $3);  }
| IntMarkingFormula POWER IntMarkingFormula   {snprintf($$,BUFF_SIZE,"pow(%s , %s)", $1, $3);  }
| FLOOR LB IntMarkingFormula RB {snprintf($$,BUFF_SIZE,"floor( %s )", $3);  }
| FLOOR LB IntMarkingFormula DIV IntMarkingFormula RB {snprintf($$,BUFF_SIZE,"floor( %s /double(%s))", $3,$5);  }
| MIN LB IntMarkingFormula COMMA IntMarkingFormula RB {snprintf($$,BUFF_SIZE,"min(%s , %s)", $3, $5);  }
| MAX LB IntMarkingFormula COMMA IntMarkingFormula RB {snprintf($$,BUFF_SIZE,"max(%s , %s)", $3, $5);  };
| MOD LB IntMarkingFormula COMMA IntMarkingFormula RB {snprintf($$,BUFF_SIZE,"((int)%s %% (int)%s)", $3, $5);  };


RealMarkingFormula:  rval {snprintf($$,BUFF_SIZE, "%f",$1);}
| ival {snprintf($$,BUFF_SIZE,"%d",$1);}
| str SHARP {
    if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<$1->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".value()";
        snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*$1<<"' is not a place label"<<endl;YYABORT;}}
| str {
        if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<<$1->c_str();
                if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".card()";
                snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
        }
        else if(Reader.MyLha.LhaRealConstant.find(*$1)!=Reader.MyLha.LhaRealConstant.end())
        {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*$1];
                snprintf($$, BUFF_SIZE,"%s",(s.str()).c_str());
        }

        else{cout<<"'"<<*$1<<"' is not a place label or a defined constant"<<endl;YYABORT;}}
| str LSB ColorClassList RSB {
    auto pi = Reader.MyLha.PlaceIndex.find(*$1);
    if( pi !=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s;
        s<<" Marking.P->_PL_"<< $1->c_str()<< ".mult";
        if(P.is_domain_impl_set){
            size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
            s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << $3 << "))";
     } else s << $3;
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*$1<<"' is not a place label "<<endl;YYABORT;}}
| LB RealMarkingFormula RB              {snprintf($$,BUFF_SIZE,"( %s )", $2);       }
| MINUS RealMarkingFormula %prec NEG {snprintf($$,BUFF_SIZE,"-%s",$2);}
| RealMarkingFormula DIV  RealMarkingFormula   {snprintf($$,BUFF_SIZE,"%s /double(%s)", $1, $3);  }
| RealMarkingFormula PLUS  RealMarkingFormula   {snprintf($$,BUFF_SIZE,"%s + %s", $1, $3);  }
| RealMarkingFormula MINUS RealMarkingFormula   {snprintf($$,BUFF_SIZE,"%s - %s", $1, $3);  }
| RealMarkingFormula MUL   RealMarkingFormula   {snprintf($$,BUFF_SIZE,"%s * %s", $1, $3);  }
| RealMarkingFormula POWER RealMarkingFormula   {snprintf($$,BUFF_SIZE,"pow(%s , %s)", $1, $3);  }
| FLOOR LB RealMarkingFormula RB {snprintf($$,BUFF_SIZE,"floor( %s )", $3);  }
| MIN LB RealMarkingFormula COMMA RealMarkingFormula RB {snprintf($$,BUFF_SIZE,"min(%s , %s)", $3, $5);  }
| MAX LB RealMarkingFormula COMMA RealMarkingFormula RB {snprintf($$,BUFF_SIZE,"max(%s , %s)", $3, $5);  };

RealVarMarkingFormula:  rval {snprintf($$,BUFF_SIZE, "%f",$1);}
| ival {snprintf($$,BUFF_SIZE,"%d",$1);}
| str SHARP {
    if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<$1->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".value()";
        snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*$1<<"' is not a place label"<<endl;YYABORT;}}
| str {
        if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<<$1->c_str();
                if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".card()";
                snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
        }
        else{ if(Reader.MyLha.LhaRealConstant.find(*$1)!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*$1];
                        snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
                }
                else {if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size())
                        {
                                std::ostringstream s; s<<"Vars->"<< $1->c_str();
                                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
                        }
                        else {cout<<"'"<<*$1<<"' is not a place label, a lha variable or a defined constant"<<endl;YYABORT;}
                }
        }
}
| str LSB IntMarkingFormula RSB {
    size_t varin = Reader.MyLha.Vars.find(*$1);
    if(varin != Reader.MyLha.Vars.label.size()){
        snprintf($$,BUFF_SIZE, "Vars->%s[%s]", $1->c_str(),$3);
    }
        else {cout<<"'"<<*$1<<"' is not an lha array variable"<<endl;YYABORT;}
    }
| str LSB ColorClassList RSB {
    auto pi = Reader.MyLha.PlaceIndex.find(*$1);
    if( pi !=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s;
        s<<" Marking.P->_PL_"<< $1->c_str()<< ".mult";
        if(P.is_domain_impl_set){
            size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
            s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << $3 << "))";
     } else s << $3;
                snprintf($$,BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*$1<<"' is not a place label "<<endl;YYABORT;}}
| LB RealVarMarkingFormula RB              {snprintf($$,BUFF_SIZE,"( %s )", $2);       }
| MINUS RealVarMarkingFormula %prec NEG {snprintf($$,BUFF_SIZE,"-%s",$2);}
| RealVarMarkingFormula DIV  RealVarMarkingFormula    {snprintf($$,BUFF_SIZE,"%s /double(%s)", $1, $3);  }
| RealVarMarkingFormula PLUS  RealVarMarkingFormula   {snprintf($$,BUFF_SIZE,"%s + %s", $1, $3);  }
| RealVarMarkingFormula MINUS RealVarMarkingFormula   {snprintf($$,BUFF_SIZE,"%s - %s", $1, $3);  }
| RealVarMarkingFormula MUL   RealVarMarkingFormula   {snprintf($$,BUFF_SIZE,"%s * %s", $1, $3);  }
| RealVarMarkingFormula POWER RealVarMarkingFormula   {snprintf($$,BUFF_SIZE,"pow(%s , %s)", $1, $3);  }
| FLOOR LB RealVarMarkingFormula RB {snprintf($$,BUFF_SIZE,"floor( %s )", $3);  }
| MIN LB RealVarMarkingFormula COMMA RealVarMarkingFormula RB {snprintf($$,BUFF_SIZE,"min(%s , %s)", $3, $5);  }
| MAX LB RealVarMarkingFormula COMMA RealVarMarkingFormula RB {snprintf($$,BUFF_SIZE,"max(%s , %s)", $3, $5);  };

ColorClassList: str COMMA {if(P.is_domain_impl_set){
                                snprintf($$,BUFF_SIZE,"%s", $1->c_str()); }
                     else snprintf($$,BUFF_SIZE,"[%s]", $1->c_str()); }
| str COMMA ColorClassList {if(P.is_domain_impl_set){
    snprintf($$,BUFF_SIZE,"%s,%s", $1->c_str(), $3); }
else
    snprintf($$,BUFF_SIZE,"[%s]%s", $1->c_str(), $3); };

Sizes: NbLocations NbVariables
|NbVariables NbLocations
|NbLocations {Reader.MyLha.NbVar=0;}
|NbVariables {Reader.MyLha.NbLoc=0;}
| {Reader.MyLha.NbVar=0;Reader.MyLha.NbLoc=0;}

Constants: Constant
|Constant Constants;

HybridVars: HybridVar
|HybridVar HybridVars;

Constant: Const INT str EQ IntMarkingFormula SEMICOLON
{if(Reader.MyLha.LhaRealConstant.find(*$3)!=Reader.MyLha.LhaRealConstant.end())
        {cout<<"Constant "<<*$3<<" already defined."<<endl; YYABORT;}
        else {string st=$5;
                if (Reader.P.constants.count(*$3)>0)st = Reader.P.constants[*$3].to_string();

                LhaEvaluate.parse(st);
                Reader.MyLha.LhaIntConstant[*$3]=LhaEvaluate.IntResult;Reader.MyLha.LhaRealConstant[*$3]=LhaEvaluate.RealResult;}
}
| Const DOUBLE str EQ RealMarkingFormula SEMICOLON
{if(Reader.MyLha.LhaRealConstant.find(*$3)!=Reader.MyLha.LhaRealConstant.end())
        {cout<<"Constant "<<*$3<<" already defined."<<endl; YYABORT;}
        else {string st=$5;
                if (Reader.P.constants.count(*$3)>0)st = Reader.P.constants[*$3].to_string();
                LhaEvaluate.parse(st);
                Reader.MyLha.LhaRealConstant[*$3]=LhaEvaluate.RealResult;}
};

HybridVar: Hybrid INT str SEMICOLON
{if(Reader.MyLha.LhaRealHybrid.find(*$3)!=Reader.MyLha.LhaRealHybrid.end())
        {cout<<"Hybrid Variable "<<*$3<<" defined twice in the LHA."<<endl; YYABORT;}
        else {
                Reader.MyLha.LhaIntHybrid[*$3]=1;
                Reader.MyLha.LhaRealHybrid[*$3]=1.0;
        }
}
| Hybrid DOUBLE str SEMICOLON
{if(Reader.MyLha.LhaRealHybrid.find(*$3)!=Reader.MyLha.LhaRealHybrid.end())
    {cout<<"Hybrid Variable "<<*$3<<" defined twice in the LHA."<<endl; YYABORT;}
    else {
        Reader.MyLha.LhaRealHybrid[*$3]=1.0;
    }
}
;


NbVariables: NbVar EQ ival SEMICOLON {Reader.MyLha.NbVar=$3;};

NbLocations: NbLoc EQ ival SEMICOLON {Reader.MyLha.NbLoc=$3;};

Lists: VariablesList LocationsList
| LocationsList VariablesList;

VariablesList: VList EQ '{' VLabels '}' SEMICOLON {
        if(Reader.MyLha.NbVar==0)Reader.MyLha.NbVar = Reader.MyLha.Vars.label.size();
        if(Reader.MyLha.Vars.label.size()!=Reader.MyLha.NbVar){
                std::cout<<"Variable label missing or redeclared, please check your variables list"<<std::endl;
                YYABORT;
        }

        FuncFlowVector.resize(Reader.MyLha.NbVar);
        FuncUpdateVector.resize(Reader.MyLha.NbVar);
    FuncUpdateVectorIndex.resize(Reader.MyLha.NbVar);
        CoeffsVector.resize(Reader.MyLha.NbVar);

        for(const auto &it : Reader.MyLha.TransitionIndex)
                PetriTransitions.insert(it.first);
};

VLabels : str {
        Reader.MyLha.Vars.label.push_back(*$1);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(CONTINIOUS_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;
}
| DISC str {

        Reader.MyLha.Vars.label.push_back(*$2);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(DISCRETE_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
    //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;

}
| DISC str LSB ival RSB {

        Reader.MyLha.Vars.label.push_back(*$2);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(INT_INDEXED_DISC_ARRAY);
        Reader.MyLha.Vars.colorDomain.push_back($4);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;

}

|VLabels COMMA str {
        Reader.MyLha.Vars.label.push_back(*$3);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(CONTINIOUS_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$3]=Reader.MyLha.VarLabel.size()-1;
};
|VLabels COMMA DISC str {
        Reader.MyLha.Vars.label.push_back(*$4);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(DISCRETE_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$3]=Reader.MyLha.VarLabel.size()-1;
}
|VLabels COMMA DISC str LSB ival RSB {
    Reader.MyLha.Vars.label.push_back(*$4);
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(INT_INDEXED_DISC_ARRAY);
        Reader.MyLha.Vars.colorDomain.push_back($6);
    Reader.MyLha.Vars.isTraced.push_back(true);

};

LocationsList: LList EQ '{' LLabels '}' SEMICOLON {
        if(Reader.MyLha.NbLoc ==0)Reader.MyLha.NbLoc = Reader.MyLha.LocIndex.size();
        if(Reader.MyLha.LocIndex.size()!=Reader.MyLha.NbLoc){
        std::cout<<"Location label missing or redeclared, please check your locations list"<<std::endl;
        YYABORT;
        }
    Reader.MyLha.FuncLocProperty=vector<string>(Reader.MyLha.NbLoc,"");
    Reader.MyLha.FuncFlow=vector<vector<string> >(Reader.MyLha.NbLoc,vector<string>(Reader.MyLha.NbVar,""));
};

LLabels : str {
        Reader.MyLha.LocLabel.push_back(*$1);
        Reader.MyLha.LocIndex[*$1]=Reader.MyLha.LocLabel.size()-1;
}
|LLabels COMMA str {Reader.MyLha.LocLabel.push_back(*$3);
        Reader.MyLha.LocIndex[*$3]=Reader.MyLha.LocLabel.size()-1;
};


InitFinal: init final
|final init;

init: Iloc EQ '{' iLLabels '}' SEMICOLON;

iLLabels : str {

        if(Reader.MyLha.LocIndex.find(*$1)!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.InitLoc.insert(Reader.MyLha.LocIndex[*$1]);
        else cout<<"Unknown location:" << *$1 <<endl;


}
|iLLabels COMMA str {if(Reader.MyLha.LocIndex.find(*$3)!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.InitLoc.insert(Reader.MyLha.LocIndex[*$3]);
        else cout<<"Unknown location:"<< *$3 << endl;
};

final: Floc EQ '{' fLLabels '}' SEMICOLON;

fLLabels : str {

        if(Reader.MyLha.LocIndex.find(*$1)!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.FinalLoc.insert(Reader.MyLha.LocIndex[*$1]);
        else cout<<"Unknown location: "<< *$1 <<endl;


}
|fLLabels COMMA str {if(Reader.MyLha.LocIndex.find(*$3)!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.FinalLoc.insert(Reader.MyLha.LocIndex[*$3]);
        else {cout<<"Unknown location: "<< *$3 <<endl;YYABORT;}
};





definitions: LocationsDef EdgesDef
|EdgesDef LocationsDef;



LocationsDef: locations EQ '{' LOCATIONS '}' SEMICOLON {
        for(size_t l=0; l<Reader.MyLha.FuncLocProperty.size();l++)
        if(Reader.MyLha.FuncLocProperty[l]=="")
        {cout<<"Some locations are not definded. Please define all the declared locations."<<endl;
                YYABORT;}

        Reader.MyLha.Out_S_Edges.resize(Reader.MyLha.NbLoc);
        Reader.MyLha.Out_A_Edges.resize(Reader.MyLha.NbLoc);
};

LOCATIONS: LOCATION
|LOCATIONS  LOCATION;

LOCATION: LB str COMMA LogExpr COMMA LB FLOWS RB RB SEMICOLON
{
    auto loc = Reader.MyLha.LocIndex.find(*$2);
        if(loc !=Reader.MyLha.LocIndex.end()){
                Reader.MyLha.FuncLocProperty[loc->second]= $4;
                Reader.MyLha.FuncFlow[loc->second] = FuncFlowVector;
        FuncFlowVector=vector<string>(Reader.MyLha.NbVar,"");
        }
        else {cout<<"Unknown location: "<< *$2 <<endl;YYABORT;}

}
|LB str COMMA LogExpr RB SEMICOLON


{
    auto loc = Reader.MyLha.LocIndex.find(*$2);
        if(loc != Reader.MyLha.LocIndex.end()){
                Reader.MyLha.FuncLocProperty[loc->second]= $4;
                Reader.MyLha.FuncFlow[loc->second] = FuncFlowVector;
        }
        else {cout<<"Unknown location: "<< *$2 <<endl;YYABORT;}


};

FLOWS: FLOW
|FLOWS COMMA FLOW;
FLOW: str COLON RealMarkingFormula {
    if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size())
        FuncFlowVector[Reader.MyLha.Vars.find(*$1)]=$3;
        else{ cout<<"'"<<*$1<<"' is not an Lha variable"<<endl;
                YYABORT;}
};

LogExpr: TRUE                             {snprintf($$,BUFF_SIZE,"true");}
|FALSE                            {snprintf($$,BUFF_SIZE,"false");}
|CompExpr                         {snprintf($$,BUFF_SIZE,"%s", $1);}
|LogExpr AND LogExpr {snprintf($$,BUFF_SIZE,"%s && %s", $1, $3);}
|LogExpr OR LogExpr  {snprintf($$,BUFF_SIZE,"%s || %s", $1, $3);}
|LB LogExpr RB       {snprintf($$,BUFF_SIZE,"( %s )", $2);}
|NOT LogExpr         {snprintf($$,BUFF_SIZE,"! %s", $2);};

CompExpr: RealMarkingFormula EQ RealMarkingFormula  {snprintf($$,BUFF_SIZE,"%s == %s", $1, $3);}
|RealMarkingFormula LEQ RealMarkingFormula {snprintf($$,BUFF_SIZE,"%s <= %s", $1, $3);}
|RealMarkingFormula GEQ RealMarkingFormula {snprintf($$,BUFF_SIZE,"%s >= %s", $1, $3);}
|RealMarkingFormula LL RealMarkingFormula  {snprintf($$,BUFF_SIZE,"%s < %s", $1, $3);}
|RealMarkingFormula GG RealMarkingFormula  {snprintf($$,BUFF_SIZE,"%s > %s", $1, $3);};




EdgesDef: edges EQ '{' EDGES '}' SEMICOLON{

} ;

EDGES: EDGE
|EDGE EDGES;

EDGE: LB LB str COMMA str RB COMMA ACTIONS COMMA CONSTRAINTS COMMA UPDATES RB SEMICOLON{

        if(Reader.MyLha.LocIndex.find(*$3)!=Reader.MyLha.LocIndex.end())
        if(Reader.MyLha.LocIndex.find(*$5)!=Reader.MyLha.LocIndex.end())
        { int ne=Reader.MyLha.Edge.size();
                Reader.MyLha.AnEdge.Index=ne;
                Reader.MyLha.AnEdge.Source=Reader.MyLha.LocIndex[*$3];
                Reader.MyLha.AnEdge.Target=Reader.MyLha.LocIndex[*$5];
                Reader.MyLha.Edge.push_back(Reader.MyLha.AnEdge);
                Reader.MyLha.EdgeActions.push_back(SubSet);
                if(SubSet.size()>0) Reader.MyLha.Out_S_Edges[Reader.MyLha.AnEdge.Source].insert(ne);
                else Reader.MyLha.Out_A_Edges[Reader.MyLha.AnEdge.Source].insert(ne);
                SubSet.erase(SubSet.begin(),SubSet.end());
                Reader.MyLha.ConstraintsCoeffs.push_back(CoeffsMatrix);
        Reader.MyLha.ConstraintsConstants.push_back(CST);
                Reader.MyLha.ConstraintsRelOp.push_back(comp);
                Reader.MyLha.unTimeEdgeConstraints.push_back("true");
                vector<string> vs;comp=vs;CST=vs;
                vector <vector <string> > ms;CoeffsMatrix=ms;

        }
        else {cout<<*$5<<" is not a location label"<<endl;YYABORT;}
        else  {cout<<*$3<<" is not a location label"<<endl;YYABORT;}
};

ACTIONS: SHARP
| ALL {SubSet=PetriTransitions;}
| ALL BackSlash SetOfActions {set<string> temp=PetriTransitions;
        for(set<string>::iterator it=SubSet.begin();it!=SubSet.end();it++)
        temp.erase((*it));
        SubSet=temp;}
| SetOfActions;

SetOfActions: '{' Actions '}';

Actions: str {if(Reader.MyLha.TransitionIndex.find(*$1)!=Reader.MyLha.TransitionIndex.end())
        SubSet.insert(*$1);
        else {cout<<*$1<<" is not a Petri-net transition "<<endl;YYABORT;}
}
|Actions COMMA str {if(Reader.MyLha.TransitionIndex.find(*$3)!=Reader.MyLha.TransitionIndex.end())
        SubSet.insert(*$3);
        else {cout<<*$3<<" is not a Petri-net transition "<<endl;YYABORT;}};


CONSTRAINTS: CONSTRAINT {}
|CONSTRAINT AND CONSTRAINTS {}
|SHARP;

CONSTRAINT:  LinearExp EQ RealMarkingFormula
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back("==");
        CST.push_back($3);
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;}
|LinearExp LEQ RealMarkingFormula
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back("<=");
        CST.push_back($3);
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;}
|LinearExp GEQ RealMarkingFormula
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back(">=");
        CST.push_back($3);
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;};

LinearExp: term
|LinearExp MINUS term
|LinearExp PLUS  term;

term:   str
{ if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size())
        {CoeffsVector[Reader.MyLha.Vars.find(*$1)]="1";}
        else {cout<<*$1<<" is not a Lha variable"<<endl;YYABORT;}}
| LB RealMarkingFormula RB MUL str
{ if(Reader.MyLha.Vars.find(*$5)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"("<<$2<<")";
                CoeffsVector[Reader.MyLha.Vars.find(*$5)]=s.str();}
        else {cout<<*$5<<" is not a Lha variable"<<endl;YYABORT;}}
| ival MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<$1;
                CoeffsVector[Reader.MyLha.Vars.find(*$3)]=s.str();}
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}}
| rval MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<$1;
                CoeffsVector[Reader.MyLha.Vars.find(*$3)]=s.str();}
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}}
| str  MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*$1)!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*$1];
                        CoeffsVector[Reader.MyLha.Vars.find(*$3)]=s.str();
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
                        {std::ostringstream s; s<<"Marking.P->_PL_"<<$1->c_str()<<" ";
                                CoeffsVector[Reader.MyLha.Vars.find(*$3)]=s.str();
                        }
                        else
                        {
                                cout<<*$1<<" is not Petri-net Place or a defined constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}
}
| MINUS str %prec NEG
{ if(Reader.MyLha.Vars.find(*$2)!=Reader.MyLha.Vars.label.size())
        {CoeffsVector[Reader.MyLha.Vars.find(*$2)]="-1";}
        else {cout<<*$2<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS LB RealMarkingFormula RB MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$6)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-("<<$3<<")";
                CoeffsVector[Reader.MyLha.Vars.find(*$6)]=s.str();}
        else {cout<<*$6<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS ival MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-"<<$2;
                CoeffsVector[Reader.MyLha.Vars.find(*$4)]=s.str();}
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS rval MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-"<<$2;
                CoeffsVector[Reader.MyLha.Vars.find(*$4)]=s.str();}
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS str MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*$2)!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<"-"<<Reader.MyLha.LhaRealConstant[*$2];
                        CoeffsVector[Reader.MyLha.Vars.find(*$4)]=s.str();
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*$2)!=Reader.MyLha.PlaceIndex.end())
                        {std::ostringstream s; s<<"-Marking.P->_PL_"<<$2->c_str()<<" ";
                                CoeffsVector[Reader.MyLha.Vars.find(*$4)]=s.str();
                        }
                        else
                        {
                                cout<<*$2<<" is not Petri-net Place or a defined constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}
}	;


UPDATES: '{' Updates '}' {
    Reader.MyLha.FuncEdgeUpdates.push_back(FuncUpdateVector);
    Reader.MyLha.FuncEdgeUpdatesIndex.push_back(FuncUpdateVectorIndex);
    FuncUpdateVector=vector<string>(Reader.MyLha.NbVar,"");
    FuncUpdateVectorIndex=vector<string>(Reader.MyLha.NbVar,"");
}
| SHARP {
    Reader.MyLha.FuncEdgeUpdates.push_back(FuncUpdateVector);
    Reader.MyLha.FuncEdgeUpdatesIndex.push_back(FuncUpdateVectorIndex);
};

Updates: Update
|Updates COMMA Update ;

Update: str EQ RealVarMarkingFormula {
        if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size())
    {FuncUpdateVector[Reader.MyLha.Vars.find(*$1)]= $3;}
        else{cout<<*$1<<" is not  variable label"<<endl;YYABORT;}
}
| str LSB IntMarkingFormula RSB EQ RealVarMarkingFormula {
    size_t varin = Reader.MyLha.Vars.find(*$1);
        if(varin != Reader.MyLha.Vars.label.size()){
        FuncUpdateVector[varin]= $6;
        FuncUpdateVectorIndex[varin]= $3;
        //cout << "var:" << *$1 << "\tindex:" << $3 << "\tupdate:" << $6<< endl;
    }else{cout<<*$1<<" is not  variable label"<<endl;YYABORT;}
}




HaslExps: HaslExp | HaslExp HaslExps;

HaslExp: str EQ TopHaslExp SEMICOLON {
        if($3 != NULL){
                Reader.MyLha.HASLname.push_back(*$1);
                Reader.MyLha.HASLtop.push_back($3);
        }else{
                for(vector<string>::iterator it = Reader.MyLha.HASLname.begin(); it <Reader.MyLha.HASLname.end() ; it++){
                        if( it->find("$_$") == 0)
                                it->replace(0,3,*$1);
                }
        }
}
| TopHaslExp SEMICOLON {
        if($1 != NULL){
                Reader.MyLha.HASLname.push_back("");
                Reader.MyLha.HASLtop.push_back($1);
        }else{
                for(vector<string>::iterator it = Reader.MyLha.HASLname.begin(); it <Reader.MyLha.HASLname.end() ; it++){
                        if( it->find("$_$") == 0)
                                it->replace(0,3,"");
                }
        }
}

rorival:
rval {$$=$1;}
| ival {$$=(double)$1;}
| str { if(Reader.MyLha.LhaRealConstant.find(*$1)!=Reader.MyLha.LhaRealConstant.end())
                        $$ = Reader.MyLha.LhaRealConstant[*$1];
                else { if(Reader.MyLha.LhaIntConstant.find(*$1)!=Reader.MyLha.LhaIntConstant.end())
                        $$ = (double)Reader.MyLha.LhaIntConstant[*$1];
                else {
                        cout<<*$1<<" is not a definded constant "<<endl;
                        YYABORT;
                }}}

TopHaslExp:
AVG LB AlgExpr RB {
        Reader.MyLha.Algebraic.push_back($3);
        $$ = new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1);
}
| PROB {
        $$ = new HaslFormulasTop(PROBABILITY);
}
| PROB LB RB {
    $$ = new HaslFormulasTop(PROBABILITY);
}
| PROB LB str RB {
    Reader.MyLha.FinalStateCond.push_back(*$3);
    $$ = new HaslFormulasTop(PROBCOND,(size_t)Reader.MyLha.FinalStateCond.size()-1);
}
| EXIST_TOK {
    $$ = new HaslFormulasTop(EXISTS);
}
| NOTALL_TOK {
    $$ = new HaslFormulasTop(NOTALLS);
}
| SPRT LB rval COMMA rval RB {
        $$ = new HaslFormulasTop($3,$5);
}
| PDF LB AlgExpr COMMA rorival COMMA rorival COMMA rorival RB {

        for(double bucket = $7 ; bucket < $9 ; bucket+= $5){
                std::ostringstream algPDF;
                algPDF << "(("<<$3<<" >= "<<bucket<<"&& "<<$3<<"<"<<bucket+$5<<") ? 1:0)";

                Reader.MyLha.Algebraic.push_back(algPDF.str());
                Reader.MyLha.HASLtop.push_back(
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1));
                Reader.MyLha.HASLtop.back()->TypeOp = PDF_PART;
                std::ostringstream s; s<<"$_$: Value in ["<< bucket<< " , "<<bucket+$5<<"]";
                Reader.MyLha.HASLname.push_back(s.str());
        }
        $$ = NULL;
}
| CDF LB AlgExpr COMMA rorival COMMA rorival COMMA rorival RB {

        for(double bucket = $7 ; bucket < $9 ; bucket+= $5){
                std::ostringstream algCDF;
                algCDF << "(("<<$3<<" <= "<<bucket<<") ? 1:0)";

                Reader.MyLha.Algebraic.push_back(algCDF.str());
                Reader.MyLha.HASLtop.push_back(
                new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1));
                Reader.MyLha.HASLtop.back()->TypeOp = CDF_PART;
                std::ostringstream s; s<<"$_$: Value in [-infinity , "<< bucket<<"]";
                Reader.MyLha.HASLname.push_back(s.str());
        }
        $$ = NULL;
}
| VAR LB AlgExpr RB {
        Reader.MyLha.Algebraic.push_back($3);
        char tmp[5000];
        snprintf(tmp,BUFF_SIZE,"(%s * %s)", $3,$3);
        Reader.MyLha.Algebraic.push_back(tmp);
        $$ = new HaslFormulasTop(HASL_MINUS,
                new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1),
                new HaslFormulasTop(HASL_TIME,
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-2),
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-2)));
}
| LB TopHaslExp RB {
        $$ = $2;
}
| TopHaslExp PLUS TopHaslExp {
        $$ = new HaslFormulasTop(HASL_PLUS, $1,$3);
}
| TopHaslExp MINUS TopHaslExp {
        $$ = new HaslFormulasTop(HASL_MINUS, $1,$3);
}
| TopHaslExp MUL TopHaslExp {
        $$ = new HaslFormulasTop(HASL_TIME, $1, $3);
}
| TopHaslExp DIV TopHaslExp {
        $$ = new HaslFormulasTop(HASL_DIV, $1,$3);
}
| ival {
        $$ = new HaslFormulasTop((double)$1,0.0,1.0);
}
| rval {
        $$ = new HaslFormulasTop((double)$1,0.0,1.0);
}

AlgExpr:LhaFunc {string ss=$1;
        snprintf($$,BUFF_SIZE,"LhaFunc[%d]", Reader.MyLha.LhaFunction[ss]);
}
|MIN LB AlgExpr COMMA AlgExpr  RB {snprintf($$,BUFF_SIZE,"min(%s,%s)", $3,$5);}
|MAX LB AlgExpr COMMA AlgExpr  RB {snprintf($$,BUFF_SIZE,"max(%s,%s)", $3,$5);}
|MINUS AlgExpr %prec NEG {snprintf($$,BUFF_SIZE,"-%s", $2);}
|FLOOR LB AlgExpr RB {snprintf($$,BUFF_SIZE,"floor(%s)", $3);}
|LB AlgExpr RB {snprintf($$,BUFF_SIZE,"(%s)", $2);}
|AlgExpr POWER AlgExpr {snprintf($$,BUFF_SIZE,"pow(%s , %s)", $1,$3);}
|AlgExpr PLUS AlgExpr {snprintf($$,BUFF_SIZE,"(%s + %s)", $1,$3);}
|AlgExpr MINUS AlgExpr {snprintf($$,BUFF_SIZE,"(%s - %s)", $1,$3);}
|AlgExpr MUL AlgExpr {snprintf($$,BUFF_SIZE,"(%s * %s)", $1,$3);}
|AlgExpr DIV AlgExpr {snprintf($$,BUFF_SIZE,"(%s / %s)", $1,$3);};

LhaFunc:  LAST     LB LinForm RB {std::ostringstream s; s<<$3;
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Last");
        string ss="Last("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf($$,BUFF_SIZE,"%s", ss.c_str());
}
|LhaMIN   LB LinForm RB {std::ostringstream s; s<<$3;
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Min");
        string ss="Min("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf($$,BUFF_SIZE,"%s", ss.c_str());
}
|LhaMAX   LB LinForm RB{std::ostringstream s; s<<$3;
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Max");
        string ss="Max("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf($$,BUFF_SIZE,"%s", ss.c_str());
}
|INTEGRAL LB LinForm RB{std::ostringstream s; s<<$3;
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Integral");
        string ss="Integral("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf($$,BUFF_SIZE,"%s", ss.c_str());
}
| MEAN LB LinForm RB{std::ostringstream s; s<<$3;
                if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
                {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
                Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
                Reader.MyLha.LhaFuncType.push_back("Mean");
                string ss="Mean("; ss.append(s.str()); ss.append(")");
                if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
                {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
                snprintf($$,BUFF_SIZE,"%s", ss.c_str());
};

LinForm: VarTerm {snprintf($$,BUFF_SIZE,"%s", $1);  }
|LinForm MINUS VarTerm {snprintf($$,BUFF_SIZE,"%s - %s", $1, $3);  }
|LinForm PLUS  VarTerm {snprintf($$,BUFF_SIZE,"%s + %s", $1, $3);  };

VarTerm:
 str
{ if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"Vars->%s", $1->c_str());}
    else if(Reader.MyLha.LhaRealHybrid.find(*$1)!=Reader.MyLha.LhaRealHybrid.end())
    {snprintf($$,BUFF_SIZE," hybridVar::%s",$1->c_str());}
        else if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<< $1->c_str();
                        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".card()";
                snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
        } else {cout<<*$1<<" is not a Hybrid variable, a Lha variable or a place name"<<endl;YYABORT;}
}
| str SHARP {
    if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<$1->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*$1]].colorDom !=0 )s <<".value()";
        snprintf($$,BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*$1<<"' is not a place label"<<endl;YYABORT;}}
| str LSB IntMarkingFormula RSB {
    if(Reader.MyLha.Vars.find(*$1)!=Reader.MyLha.Vars.label.size()){
        snprintf($$,BUFF_SIZE,"Vars->%s[%s]", $1->c_str(), $3);
        } else {cout<<*$1<<" is not a Lha array variable "<<endl;YYABORT;}
}

//| RealMarkingFormula { snprintf($$,BUFF_SIZE,"(%s)", $1); }
| LB RealMarkingFormula RB MUL str
{ if(Reader.MyLha.Vars.find(*$5)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"(%s) * Vars->%s", $2, $5->c_str());
        }
        else {cout<<*$5<<" is not a Lha variable"<<endl;YYABORT;}}
| ival MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"%d * Vars->%s", $1, $3->c_str());
        }
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}}
| rval MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"%f * Vars->%s", $1, $3->c_str());
        }
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}}

| ival
{ snprintf($$,BUFF_SIZE,"%i ", $1);
}
| rval
{ snprintf($$,BUFF_SIZE,"%f ", $1);}

| str  MUL str
{ if(Reader.MyLha.Vars.find(*$3)!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*$1)!=Reader.MyLha.LhaRealConstant.end())
                {snprintf($$,BUFF_SIZE,"%f * Vars->%s", Reader.MyLha.LhaRealConstant[*$1],$3->c_str());
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*$1)!=Reader.MyLha.PlaceIndex.end())
                        {snprintf($$,BUFF_SIZE,"Marking.P->_PL_%s * Vars->%s", $1->c_str() ,$3->c_str());
                        }
                        else
                        {
                                cout<<*$1<<" is not Petri-net Place or a definded constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*$3<<" is not a Lha variable"<<endl;YYABORT;}
}
|MINUS str %prec NEG
{ if(Reader.MyLha.Vars.find(*$2)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"-Vars->%s", $2->c_str());}
        else {cout<<*$2<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS LB RealMarkingFormula RB MUL str
{ if(Reader.MyLha.Vars.find(*$6)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"-(%s) * Vars->%s", $3, $6->c_str());
        }
        else {cout<<*$5<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS ival MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"-%d * Vars->%s", $2, $4->c_str());
        }
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS rval MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {snprintf($$,BUFF_SIZE,"-%f * Vars->%s", $2, $4->c_str());
        }
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}}
| MINUS str  MUL str %prec NEG
{ if(Reader.MyLha.Vars.find(*$4)!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*$2)!=Reader.MyLha.LhaRealConstant.end())
                {snprintf($$,BUFF_SIZE,"-%f * Vars->%s", Reader.MyLha.LhaRealConstant[*$2],$4->c_str());
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*$2)!=Reader.MyLha.PlaceIndex.end())
                        {snprintf($$,BUFF_SIZE,"-Marking.P->_PL_%s * Vars->%s", $2->c_str() ,$4->c_str());
                        }
                        else
                        {
                                cout<<*$2<<" is not Petri-net Place or a definded constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*$4<<" is not a Lha variable"<<endl;YYABORT;}
};





%%

void
lha::Lha_parser::error (const lha::Lha_parser::location_type& l,
const std::string& m)
{
        Reader.error (l, m);
}
