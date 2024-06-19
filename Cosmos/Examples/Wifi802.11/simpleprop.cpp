#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include "markingImpl.hpp"
#include <math.h>
#include <float.h>
#include "LHA.hpp"
    const double AIFS1=2;
    const double AIFS2=3;
    const double AIFS3=6;
    const double AIFS4=9;
    const double Ack=11;
    const double CTS=11;
    const double Data=200;
    const double N=2;
    const double RTS=16;
    const double SIFS=1;
    const double T=10000;
    const double Timeout=5;
    const double Traffic=1;
    const double Ttrans=0;
    const double Vuln=2;
    const double aSlot=1;
    const double f=2;
    const double invT=0.0001;
namespace hybridVar {
};
struct Variables {
	double time;
	double countT;
	double CorrectPacketAckp1;
	double CorrectPacketAckp2;
	double CorrectPacketAckp3;
	double CorrectPacketAckp4;
	double PLVAR_Idle;
	double PLVAR_Medium;
	double PLVAR_Garbled;
};
bool varOrder(const Variables &v1,const Variables &v2){
	if(v1.time<v2.time)return true;
	if(v1.countT<v2.countT)return true;
	if(v1.CorrectPacketAckp1<v2.CorrectPacketAckp1)return true;
	if(v1.CorrectPacketAckp2<v2.CorrectPacketAckp2)return true;
	if(v1.CorrectPacketAckp3<v2.CorrectPacketAckp3)return true;
	if(v1.CorrectPacketAckp4<v2.CorrectPacketAckp4)return true;
	if(v1.PLVAR_Idle<v2.PLVAR_Idle)return true;
	if(v1.PLVAR_Medium<v2.PLVAR_Medium)return true;
	if(v1.PLVAR_Garbled<v2.PLVAR_Garbled)return true;
	return false;
};
template<class DEDState>
void LHA<DEDState>::resetVariables(){
	Vars->time= 0;
	Vars->countT= 0;
	Vars->CorrectPacketAckp1= 0;
	Vars->CorrectPacketAckp2= 0;
	Vars->CorrectPacketAckp3= 0;
	Vars->CorrectPacketAckp4= 0;
	Vars->PLVAR_Idle= 0;
	Vars->PLVAR_Medium= 0;
	Vars->PLVAR_Garbled= 0;
};
template<class DEDState>
void LHA<DEDState>::printHeader(ostream &s)const{
	s << "	Location\t";
};
template<class DEDState>
void LHA<DEDState>::printState(ostream &s){
	s << "\t" << LocLabel[CurrentLocation] << "\t";
};
template<class DEDState>
const int LHA<DEDState>::ActionEdgesAr[] = {
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,4 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,2 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};
template<class DEDState>
 LHA<DEDState>::LHA():NbLoc(3),NbTrans(39),NbVar(9),FinalLoc( 3,false){
    InitLoc.insert(0);
    FinalLoc[2]=true;
    Edge= vector<LhaEdge>(8);
    Edge[0] = LhaEdge(0, 0, 0,Synch);
    Edge[1] = LhaEdge(1, 0, 1,Auto);
    Edge[2] = LhaEdge(2, 1, 1,Synch);
    Edge[3] = LhaEdge(3, 1, 1,Synch);
    Edge[4] = LhaEdge(4, 1, 1,Synch);
    Edge[5] = LhaEdge(5, 1, 1,Synch);
    Edge[6] = LhaEdge(6, 1, 1,Synch);
    Edge[7] = LhaEdge(7, 1, 2,Auto);
	Vars = new Variables;
	tempVars = new Variables;
	resetVariables();
    Out_A_Edges =vector< set < int > >(NbLoc);
    Out_A_Edges[0].insert(1);
    Out_A_Edges[1].insert(7);
    LinForm= vector<double>(7,0.0);
    OldLinForm=vector<double>(7,0.0);
    LhaFunc=vector<double>(7,0.0);
    LhaFuncDefaults=vector<double>(7,0.0);
    FormulaVal = vector<double>(7,0.0);
    FormulaValQual = vector<bool>(0,false);
}

template<class DEDState>
void LHA<DEDState>::DoElapsedTimeUpdate(double DeltaT,const DEDState& Marking) {
	Vars->time += GetFlow(0, Marking) * DeltaT;
	Vars->CorrectPacketAckp1 += GetFlow(2, Marking) * DeltaT;
	Vars->CorrectPacketAckp2 += GetFlow(3, Marking) * DeltaT;
	Vars->CorrectPacketAckp3 += GetFlow(4, Marking) * DeltaT;
	Vars->CorrectPacketAckp4 += GetFlow(5, Marking) * DeltaT;
	Vars->PLVAR_Idle += GetFlow(6, Marking) * DeltaT;
	Vars->PLVAR_Medium += GetFlow(7, Marking) * DeltaT;
	Vars->PLVAR_Garbled += GetFlow(8, Marking) * DeltaT;
}
template<class DEDState>
double LHA<DEDState>::GetFlow(int v, const DEDState& Marking)const{
	switch (v){
		case 1:	//countT

		break;
		case 8:	//PLVAR_Garbled
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Garbled  * 0.0001;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 6:	//PLVAR_Idle
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Idle.card()  * 0.0001;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 7:	//PLVAR_Medium
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Medium  * 0.0001;

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
		default:	//CorrectPacketAckp1,CorrectPacketAckp2,CorrectPacketAckp3,CorrectPacketAckp4,
		return 0.0;


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
		case 6:	//
{
         if(!( +(1)*Vars->time<=10000)) return false;
		return (true);
     }

		break;
		case 2:	//
{
    if( b.P->p.c0 != Color_Pr_pr1 )return false;
         if(!( +(1)*Vars->time<=10000)) return false;
		return (true);
     }

		break;
		case 3:	//
{
        if( b.P->p.c0 != Color_Pr_pr2 )return false;
         if(!( +(1)*Vars->time<=10000)) return false;
		return (true);
     }

		break;
		case 4:	//
{
        if( b.P->p.c0 != Color_Pr_pr3 )return false;
         if(!( +(1)*Vars->time<=10000)) return false;
		return (true);
     }

		break;
		case 5:	//
{
        if( b.P->p.c0 != Color_Pr_pr4 )return false;
         if(!( +(1)*Vars->time<=10000)) return false;
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
		case 7:	//
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
                  if(!(SumAX==10000))
                      return EmptyInterval;
             }
             else{
                  double t=CurrentTime+(10000-SumAX)/(double)SumAF;
                  if(t>=EnablingT.first && t<=EnablingT.second){
                      EnablingT.first=t; EnablingT.second=t;
                  }
                  else return EmptyInterval;
             }
             return EnablingT;
         }

		break;
		default:	//,,,,,,
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
		case 6:	//
         {
		Vars->countT=Vars->countT + 1;
         }

		break;
		case 1:	//
         {
		Vars->time=0;
         }

		break;
		case 2:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->CorrectPacketAckp1=Vars->CorrectPacketAckp1 + 1;
		Vars->countT = tempVars->countT;
		Vars->CorrectPacketAckp1 = tempVars->CorrectPacketAckp1;
         }

		break;
		case 3:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->CorrectPacketAckp2=Vars->CorrectPacketAckp2 + 1;
		Vars->countT = tempVars->countT;
		Vars->CorrectPacketAckp2 = tempVars->CorrectPacketAckp2;
         }

		break;
		case 4:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->CorrectPacketAckp3=Vars->CorrectPacketAckp3 + 1;
		Vars->countT = tempVars->countT;
		Vars->CorrectPacketAckp3 = tempVars->CorrectPacketAckp3;
         }

		break;
		case 5:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->CorrectPacketAckp4=Vars->CorrectPacketAckp4 + 1;
		Vars->countT = tempVars->countT;
		Vars->CorrectPacketAckp4 = tempVars->CorrectPacketAckp4;
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
void LHA<DEDState>::UpdateFormulaVal(const DEDState& Marking){

    LhaFunc[0]= Vars->CorrectPacketAckp1;
    LhaFunc[1]= Vars->CorrectPacketAckp2;
    LhaFunc[2]= Vars->CorrectPacketAckp3;
    LhaFunc[3]= Vars->CorrectPacketAckp4;
    LhaFunc[4]= Vars->PLVAR_Idle;
    LhaFunc[5]= Vars->PLVAR_Medium;
    LhaFunc[6]= Vars->PLVAR_Garbled;
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
