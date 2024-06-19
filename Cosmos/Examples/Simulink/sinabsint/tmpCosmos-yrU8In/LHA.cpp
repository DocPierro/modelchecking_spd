#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include "markingImpl.hpp"
#include <math.h>
#include <float.h>
#include "LHA.hpp"
    const double T=10;
    const double Ttrans=0;
    const double invT=0.1;
struct Variables {
	double time;
	double countT;
	double SimulinkTransition;
	double PLVAR_B13O1;
	double PLVAR_B8O1;
	double PLVAR_B7O1;
	double PLVAR_B9O1;
	double PLVAR_B11O1;
	double PLVAR_B10O1;
};
bool varOrder(const Variables &v1,const Variables &v2){
	if(v1.time<v2.time)return true;
	if(v1.countT<v2.countT)return true;
	if(v1.SimulinkTransition<v2.SimulinkTransition)return true;
	if(v1.PLVAR_B13O1<v2.PLVAR_B13O1)return true;
	if(v1.PLVAR_B8O1<v2.PLVAR_B8O1)return true;
	if(v1.PLVAR_B7O1<v2.PLVAR_B7O1)return true;
	if(v1.PLVAR_B9O1<v2.PLVAR_B9O1)return true;
	if(v1.PLVAR_B11O1<v2.PLVAR_B11O1)return true;
	if(v1.PLVAR_B10O1<v2.PLVAR_B10O1)return true;
	return false;
};
template<class DEDState>
void LHA<DEDState>::resetVariables(){
	Vars->time= 0;
	Vars->countT= 0;
	Vars->SimulinkTransition= 0;
	Vars->PLVAR_B13O1= 0;
	Vars->PLVAR_B8O1= 0;
	Vars->PLVAR_B7O1= 0;
	Vars->PLVAR_B9O1= 0;
	Vars->PLVAR_B11O1= 0;
	Vars->PLVAR_B10O1= 0;
};
template<class DEDState>
void LHA<DEDState>::printHeader(ostream &s)const{
	s << "	Location\ttime\tcountT\tSimulinkTransition\tPLVAR_B13O1\tPLVAR_B8O1\tPLVAR_B7O1\tPLVAR_B9O1\tPLVAR_B11O1\tPLVAR_B10O1\t";
};
template<class DEDState>
void LHA<DEDState>::printState(ostream &s){
	s << "\t" << LocLabel[CurrentLocation] << "\t";
	s << Vars->time << "\t";
	s << Vars->countT << "\t";
	s << Vars->SimulinkTransition << "\t";
	s << Vars->PLVAR_B13O1 << "\t";
	s << Vars->PLVAR_B8O1 << "\t";
	s << Vars->PLVAR_B7O1 << "\t";
	s << Vars->PLVAR_B9O1 << "\t";
	s << Vars->PLVAR_B11O1 << "\t";
	s << Vars->PLVAR_B10O1 << "\t";
};
template<class DEDState>
const int LHA<DEDState>::ActionEdgesAr[] = {
	1 ,1 ,0 ,
	0 ,2 ,-1,};
template<class DEDState>
 LHA<DEDState>::LHA():NbLoc(3),NbTrans(1),NbVar(9),FinalLoc( 3,false){
    InitLoc.insert(0);
    FinalLoc[2]=true;
    Edge= vector<LhaEdge>(4);

    vector<string> vlstr(NbLoc);
    LocLabel= vlstr;
    LocLabel[0]="l0";
    LocLabel[1]="l1";
    LocLabel[2]="l2";
    VarLabel= vector<string>(NbVar);
    VarLabel[0]="time";
    VarLabel[1]="countT";
    VarLabel[2]="SimulinkTransition";
    VarLabel[3]="PLVAR_B13O1";
    VarLabel[4]="PLVAR_B8O1";
    VarLabel[5]="PLVAR_B7O1";
    VarLabel[6]="PLVAR_B9O1";
    VarLabel[7]="PLVAR_B11O1";
    VarLabel[8]="PLVAR_B10O1";
    Edge[0] = LhaEdge(0, 0, 0,Synch);
    Edge[1] = LhaEdge(1, 0, 1,Auto);
    Edge[2] = LhaEdge(2, 1, 1,Synch);
    Edge[3] = LhaEdge(3, 1, 2,Auto);
	Vars = new Variables;
	tempVars = new Variables;
	resetVariables();
    Out_A_Edges =vector< set < int > >(NbLoc);
    Out_A_Edges[0].insert(1);
    Out_A_Edges[1].insert(3);
    LinForm= vector<double>(7,0.0);
    OldLinForm=vector<double>(7,0.0);
    LhaFunc=vector<double>(7,0.0);
    FormulaVal = vector<double>(7,0.0);
    FormulaValQual = vector<bool>(0,false);
}

template<class DEDState>
void LHA<DEDState>::DoElapsedTimeUpdate(double DeltaT,const DEDState& Marking) {
	Vars->time += GetFlow(0, Marking) * DeltaT;
	Vars->SimulinkTransition += GetFlow(2, Marking) * DeltaT;
	Vars->PLVAR_B13O1 += GetFlow(3, Marking) * DeltaT;
	Vars->PLVAR_B8O1 += GetFlow(4, Marking) * DeltaT;
	Vars->PLVAR_B7O1 += GetFlow(5, Marking) * DeltaT;
	Vars->PLVAR_B9O1 += GetFlow(6, Marking) * DeltaT;
	Vars->PLVAR_B11O1 += GetFlow(7, Marking) * DeltaT;
	Vars->PLVAR_B10O1 += GetFlow(8, Marking) * DeltaT;
}
template<class DEDState>
double LHA<DEDState>::GetFlow(int v, const DEDState& Marking)const{
	switch (v){
		case 1:	//countT

		break;
		case 2:	//SimulinkTransition
		return 0.0;


		break;
		case 8:	//PLVAR_B10O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B10O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 7:	//PLVAR_B11O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B11O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 3:	//PLVAR_B13O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B13O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 5:	//PLVAR_B7O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B7O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 4:	//PLVAR_B8O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B8O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 6:	//PLVAR_B9O1
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_B9O1  * 0.1;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 0:	//time
	switch (CurrentLocation){
		case 2:	//l2
		return 0.0;

		break;
		default:	//l0,l1,
			return 1;

		break;
	}

		break;
	}
}

template<class DEDState>
bool LHA<DEDState>::CheckLocation(int loc,const DEDState& Marking)const{
         return true;

}

template<class DEDState>
bool LHA<DEDState>::CheckEdgeContraints(int ed,size_t ptt,const abstractBinding& b,const DEDState& Marking)const{
	switch (ed){
		case 0:	//
{
         if(!( +(1)*Vars->time<=0)) return false;
		return (true);
     }

		break;
		case 2:	//
{
         if(!( +(1)*Vars->time<=10)) return false;
		return (true);
     }

		break;
		default:	//,,
	return true;

		break;
	}
}

template<class DEDState>
t_interval LHA<DEDState>::GetEdgeEnablingTime(int ed,const DEDState& Marking)const{
	switch (ed){
		case 1:	//
         {
             t_interval EnablingT;

             EnablingT.first=CurrentTime;
             EnablingT.second=DBL_MAX;

             t_interval EmptyInterval;

             EmptyInterval.first=0;
             EmptyInterval.second=-1;

             double SumAF;
             double SumAX;


             SumAF=+(1)*GetFlow(0, Marking);
             SumAX=+(1)*Vars->time;

             if(SumAF==0){
                  if(!(SumAX==0))
                      return EmptyInterval;
             }
             else{
                  double t=CurrentTime+(0-SumAX)/(double)SumAF;
                  if(t>=EnablingT.first && t<=EnablingT.second){
                      EnablingT.first=t; EnablingT.second=t;
                  }
                  else return EmptyInterval;
             }
             return EnablingT;
         }

		break;
		case 3:	//
         {
             t_interval EnablingT;

             EnablingT.first=CurrentTime;
             EnablingT.second=DBL_MAX;

             t_interval EmptyInterval;

             EmptyInterval.first=0;
             EmptyInterval.second=-1;

             double SumAF;
             double SumAX;


             SumAF=+(1)*GetFlow(0, Marking);
             SumAX=+(1)*Vars->time;

             if(SumAF==0){
                  if(!(SumAX==10))
                      return EmptyInterval;
             }
             else{
                  double t=CurrentTime+(10-SumAX)/(double)SumAF;
                  if(t>=EnablingT.first && t<=EnablingT.second){
                      EnablingT.first=t; EnablingT.second=t;
                  }
                  else return EmptyInterval;
             }
             return EnablingT;
         }

		break;
		default:	//,,
         {
             t_interval EnablingT;

             EnablingT.first=CurrentTime;
             EnablingT.second=DBL_MAX;

             return EnablingT;
         }

		break;
	}
}

template<class DEDState>
void LHA<DEDState>::DoEdgeUpdates(int ed,const DEDState& Marking, const abstractBinding& b){
	switch (ed){
		case 1:	//
         {
		Vars->time=0;
         }

		break;
		case 2:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->SimulinkTransition=Vars->SimulinkTransition + 0.1;
		Vars->countT = tempVars->countT;
		Vars->SimulinkTransition = tempVars->SimulinkTransition;
         }

		break;
	}
}

template<class DEDState>
void LHA<DEDState>::UpdateLinForm(const DEDState& Marking){
    }

template<class DEDState>
void LHA<DEDState>::UpdateLhaFunc(double& Delta ){

    }

template<class DEDState>
void LHA<DEDState>::UpdateFormulaVal(){

    LhaFunc[0]= Vars->SimulinkTransition;
    LhaFunc[1]= Vars->PLVAR_B13O1;
    LhaFunc[2]= Vars->PLVAR_B8O1;
    LhaFunc[3]= Vars->PLVAR_B7O1;
    LhaFunc[4]= Vars->PLVAR_B9O1;
    LhaFunc[5]= Vars->PLVAR_B11O1;
    LhaFunc[6]= Vars->PLVAR_B10O1;
    FormulaVal[0]=LhaFunc[0];
    FormulaVal[1]=LhaFunc[1];
    FormulaVal[2]=LhaFunc[2];
    FormulaVal[3]=LhaFunc[3];
    FormulaVal[4]=LhaFunc[4];
    FormulaVal[5]=LhaFunc[5];
    FormulaVal[6]=LhaFunc[6];
}

bool IsLHADeterministic = 1;
fullState::fullState():loc(0){
	var= new Variables;
}

fullState::fullState(int l,const Variables &v):loc(l){
	var= new Variables(v);
}

fullState::fullState(const fullState &fs):loc(fs.loc){
	var= new Variables(*(fs.var));
}

fullState::~fullState(){delete var;}

template class LHA<abstractMarking>;
