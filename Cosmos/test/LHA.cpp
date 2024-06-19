#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include "markingImpl.hpp"
#include <math.h>
#include <float.h>
#include "LHA.hpp"
    const double T=100;
    const double Ttrans=0;
    const double invT=0.01;
namespace hybridVar {
};
struct Variables {
	double time;
	double countT;
	double EXRayBlood;
	double FallIll;
	double HospitalArrival;
	double EToThreat;
	double EToSurgery;
	double HighPrio;
	double MediumPrio;
	double DischargeL;
	double LowPrio;
	double BSurgery;
	double ToSurgery;
	double ToDoctor;
	double ToDoctorL;
	double BToStabilize;
	double EToStabilize;
	double DischargeRec;
	double BBlood;
	double BXRay;
	double DischargeM;
	double EBloodEx;
	double EXRay;
	double PLVAR_ReadyT;
	double PLVAR_Healthy;
	double PLVAR_Ill;
	double PLVAR_Arrival;
	double PLVAR_ThreatedByDocH;
	double PLVAR_ThreatedByDocL;
	double PLVAR_WaitingRoom;
	double PLVAR_OperatingRoom;
	double PLVAR_ArrivalUrgence;
	double PLVAR_USurgery;
	double PLVAR_TraumaTeam;
	double PLVAR_Ustab;
	double PLVAR_WSurgery;
	double PLVAR_WBloodEx;
	double PLVAR_WXRayEx;
	double PLVAR_CountDoctor;
	double PLVAR_MonitoredRoom;
	double PLVAR_Doctor;
	double PLVAR_PatientRecovered;
	double PLVAR_UBloodEx;
	double PLVAR_ResB;
	double PLVAR_Waiting;
	double PLVAR_ResX;
	double PLVAR_UXRayEx;
	double PLVAR_FXRay;
	double PLVAR_FBloodEx;
};
bool varOrder(const Variables &v1,const Variables &v2){
	if(v1.time<v2.time)return true;
	if(v1.countT<v2.countT)return true;
	if(v1.EXRayBlood<v2.EXRayBlood)return true;
	if(v1.FallIll<v2.FallIll)return true;
	if(v1.HospitalArrival<v2.HospitalArrival)return true;
	if(v1.EToThreat<v2.EToThreat)return true;
	if(v1.EToSurgery<v2.EToSurgery)return true;
	if(v1.HighPrio<v2.HighPrio)return true;
	if(v1.MediumPrio<v2.MediumPrio)return true;
	if(v1.DischargeL<v2.DischargeL)return true;
	if(v1.LowPrio<v2.LowPrio)return true;
	if(v1.BSurgery<v2.BSurgery)return true;
	if(v1.ToSurgery<v2.ToSurgery)return true;
	if(v1.ToDoctor<v2.ToDoctor)return true;
	if(v1.ToDoctorL<v2.ToDoctorL)return true;
	if(v1.BToStabilize<v2.BToStabilize)return true;
	if(v1.EToStabilize<v2.EToStabilize)return true;
	if(v1.DischargeRec<v2.DischargeRec)return true;
	if(v1.BBlood<v2.BBlood)return true;
	if(v1.BXRay<v2.BXRay)return true;
	if(v1.DischargeM<v2.DischargeM)return true;
	if(v1.EBloodEx<v2.EBloodEx)return true;
	if(v1.EXRay<v2.EXRay)return true;
	if(v1.PLVAR_ReadyT<v2.PLVAR_ReadyT)return true;
	if(v1.PLVAR_Healthy<v2.PLVAR_Healthy)return true;
	if(v1.PLVAR_Ill<v2.PLVAR_Ill)return true;
	if(v1.PLVAR_Arrival<v2.PLVAR_Arrival)return true;
	if(v1.PLVAR_ThreatedByDocH<v2.PLVAR_ThreatedByDocH)return true;
	if(v1.PLVAR_ThreatedByDocL<v2.PLVAR_ThreatedByDocL)return true;
	if(v1.PLVAR_WaitingRoom<v2.PLVAR_WaitingRoom)return true;
	if(v1.PLVAR_OperatingRoom<v2.PLVAR_OperatingRoom)return true;
	if(v1.PLVAR_ArrivalUrgence<v2.PLVAR_ArrivalUrgence)return true;
	if(v1.PLVAR_USurgery<v2.PLVAR_USurgery)return true;
	if(v1.PLVAR_TraumaTeam<v2.PLVAR_TraumaTeam)return true;
	if(v1.PLVAR_Ustab<v2.PLVAR_Ustab)return true;
	if(v1.PLVAR_WSurgery<v2.PLVAR_WSurgery)return true;
	if(v1.PLVAR_WBloodEx<v2.PLVAR_WBloodEx)return true;
	if(v1.PLVAR_WXRayEx<v2.PLVAR_WXRayEx)return true;
	if(v1.PLVAR_CountDoctor<v2.PLVAR_CountDoctor)return true;
	if(v1.PLVAR_MonitoredRoom<v2.PLVAR_MonitoredRoom)return true;
	if(v1.PLVAR_Doctor<v2.PLVAR_Doctor)return true;
	if(v1.PLVAR_PatientRecovered<v2.PLVAR_PatientRecovered)return true;
	if(v1.PLVAR_UBloodEx<v2.PLVAR_UBloodEx)return true;
	if(v1.PLVAR_ResB<v2.PLVAR_ResB)return true;
	if(v1.PLVAR_Waiting<v2.PLVAR_Waiting)return true;
	if(v1.PLVAR_ResX<v2.PLVAR_ResX)return true;
	if(v1.PLVAR_UXRayEx<v2.PLVAR_UXRayEx)return true;
	if(v1.PLVAR_FXRay<v2.PLVAR_FXRay)return true;
	if(v1.PLVAR_FBloodEx<v2.PLVAR_FBloodEx)return true;
	return false;
};
template<class DEDState>
void LHA<DEDState>::resetVariables(){
	Vars->time= 0;
	Vars->countT= 0;
	Vars->EXRayBlood= 0;
	Vars->FallIll= 0;
	Vars->HospitalArrival= 0;
	Vars->EToThreat= 0;
	Vars->EToSurgery= 0;
	Vars->HighPrio= 0;
	Vars->MediumPrio= 0;
	Vars->DischargeL= 0;
	Vars->LowPrio= 0;
	Vars->BSurgery= 0;
	Vars->ToSurgery= 0;
	Vars->ToDoctor= 0;
	Vars->ToDoctorL= 0;
	Vars->BToStabilize= 0;
	Vars->EToStabilize= 0;
	Vars->DischargeRec= 0;
	Vars->BBlood= 0;
	Vars->BXRay= 0;
	Vars->DischargeM= 0;
	Vars->EBloodEx= 0;
	Vars->EXRay= 0;
	Vars->PLVAR_ReadyT= 0;
	Vars->PLVAR_Healthy= 0;
	Vars->PLVAR_Ill= 0;
	Vars->PLVAR_Arrival= 0;
	Vars->PLVAR_ThreatedByDocH= 0;
	Vars->PLVAR_ThreatedByDocL= 0;
	Vars->PLVAR_WaitingRoom= 0;
	Vars->PLVAR_OperatingRoom= 0;
	Vars->PLVAR_ArrivalUrgence= 0;
	Vars->PLVAR_USurgery= 0;
	Vars->PLVAR_TraumaTeam= 0;
	Vars->PLVAR_Ustab= 0;
	Vars->PLVAR_WSurgery= 0;
	Vars->PLVAR_WBloodEx= 0;
	Vars->PLVAR_WXRayEx= 0;
	Vars->PLVAR_CountDoctor= 0;
	Vars->PLVAR_MonitoredRoom= 0;
	Vars->PLVAR_Doctor= 0;
	Vars->PLVAR_PatientRecovered= 0;
	Vars->PLVAR_UBloodEx= 0;
	Vars->PLVAR_ResB= 0;
	Vars->PLVAR_Waiting= 0;
	Vars->PLVAR_ResX= 0;
	Vars->PLVAR_UXRayEx= 0;
	Vars->PLVAR_FXRay= 0;
	Vars->PLVAR_FBloodEx= 0;
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
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};
template<class DEDState>
 LHA<DEDState>::LHA():NbLoc(3),NbTrans(21),NbVar(49),FinalLoc( 3,false){
    InitLoc.insert(0);
    FinalLoc[2]=true;
    Edge= vector<LhaEdge>(24);
    Edge[0] = LhaEdge(0, 0, 0,Synch);
    Edge[1] = LhaEdge(1, 0, 1,Auto);
    Edge[2] = LhaEdge(2, 1, 1,Synch);
    Edge[3] = LhaEdge(3, 1, 1,Synch);
    Edge[4] = LhaEdge(4, 1, 1,Synch);
    Edge[5] = LhaEdge(5, 1, 1,Synch);
    Edge[6] = LhaEdge(6, 1, 1,Synch);
    Edge[7] = LhaEdge(7, 1, 1,Synch);
    Edge[8] = LhaEdge(8, 1, 1,Synch);
    Edge[9] = LhaEdge(9, 1, 1,Synch);
    Edge[10] = LhaEdge(10, 1, 1,Synch);
    Edge[11] = LhaEdge(11, 1, 1,Synch);
    Edge[12] = LhaEdge(12, 1, 1,Synch);
    Edge[13] = LhaEdge(13, 1, 1,Synch);
    Edge[14] = LhaEdge(14, 1, 1,Synch);
    Edge[15] = LhaEdge(15, 1, 1,Synch);
    Edge[16] = LhaEdge(16, 1, 1,Synch);
    Edge[17] = LhaEdge(17, 1, 1,Synch);
    Edge[18] = LhaEdge(18, 1, 1,Synch);
    Edge[19] = LhaEdge(19, 1, 1,Synch);
    Edge[20] = LhaEdge(20, 1, 1,Synch);
    Edge[21] = LhaEdge(21, 1, 1,Synch);
    Edge[22] = LhaEdge(22, 1, 1,Synch);
    Edge[23] = LhaEdge(23, 1, 2,Auto);
	Vars = new Variables;
	tempVars = new Variables;
	resetVariables();
    Out_A_Edges =vector< set < int > >(NbLoc);
    Out_A_Edges[0].insert(1);
    Out_A_Edges[1].insert(23);
    LinForm= vector<double>(47,0.0);
    OldLinForm=vector<double>(47,0.0);
    LhaFunc=vector<double>(47,0.0);
    LhaFuncDefaults=vector<double>(47,0.0);
    FormulaVal = vector<double>(47,0.0);
    FormulaValQual = vector<bool>(0,false);
}

template<class DEDState>
void LHA<DEDState>::DoElapsedTimeUpdate(double DeltaT,const DEDState& Marking) {
	Vars->time += GetFlow(0, Marking) * DeltaT;
	Vars->EXRayBlood += GetFlow(2, Marking) * DeltaT;
	Vars->FallIll += GetFlow(3, Marking) * DeltaT;
	Vars->HospitalArrival += GetFlow(4, Marking) * DeltaT;
	Vars->EToThreat += GetFlow(5, Marking) * DeltaT;
	Vars->EToSurgery += GetFlow(6, Marking) * DeltaT;
	Vars->HighPrio += GetFlow(7, Marking) * DeltaT;
	Vars->MediumPrio += GetFlow(8, Marking) * DeltaT;
	Vars->DischargeL += GetFlow(9, Marking) * DeltaT;
	Vars->LowPrio += GetFlow(10, Marking) * DeltaT;
	Vars->BSurgery += GetFlow(11, Marking) * DeltaT;
	Vars->ToSurgery += GetFlow(12, Marking) * DeltaT;
	Vars->ToDoctor += GetFlow(13, Marking) * DeltaT;
	Vars->ToDoctorL += GetFlow(14, Marking) * DeltaT;
	Vars->BToStabilize += GetFlow(15, Marking) * DeltaT;
	Vars->EToStabilize += GetFlow(16, Marking) * DeltaT;
	Vars->DischargeRec += GetFlow(17, Marking) * DeltaT;
	Vars->BBlood += GetFlow(18, Marking) * DeltaT;
	Vars->BXRay += GetFlow(19, Marking) * DeltaT;
	Vars->DischargeM += GetFlow(20, Marking) * DeltaT;
	Vars->EBloodEx += GetFlow(21, Marking) * DeltaT;
	Vars->EXRay += GetFlow(22, Marking) * DeltaT;
	Vars->PLVAR_ReadyT += GetFlow(23, Marking) * DeltaT;
	Vars->PLVAR_Healthy += GetFlow(24, Marking) * DeltaT;
	Vars->PLVAR_Ill += GetFlow(25, Marking) * DeltaT;
	Vars->PLVAR_Arrival += GetFlow(26, Marking) * DeltaT;
	Vars->PLVAR_ThreatedByDocH += GetFlow(27, Marking) * DeltaT;
	Vars->PLVAR_ThreatedByDocL += GetFlow(28, Marking) * DeltaT;
	Vars->PLVAR_WaitingRoom += GetFlow(29, Marking) * DeltaT;
	Vars->PLVAR_OperatingRoom += GetFlow(30, Marking) * DeltaT;
	Vars->PLVAR_ArrivalUrgence += GetFlow(31, Marking) * DeltaT;
	Vars->PLVAR_USurgery += GetFlow(32, Marking) * DeltaT;
	Vars->PLVAR_TraumaTeam += GetFlow(33, Marking) * DeltaT;
	Vars->PLVAR_Ustab += GetFlow(34, Marking) * DeltaT;
	Vars->PLVAR_WSurgery += GetFlow(35, Marking) * DeltaT;
	Vars->PLVAR_WBloodEx += GetFlow(36, Marking) * DeltaT;
	Vars->PLVAR_WXRayEx += GetFlow(37, Marking) * DeltaT;
	Vars->PLVAR_CountDoctor += GetFlow(38, Marking) * DeltaT;
	Vars->PLVAR_MonitoredRoom += GetFlow(39, Marking) * DeltaT;
	Vars->PLVAR_Doctor += GetFlow(40, Marking) * DeltaT;
	Vars->PLVAR_PatientRecovered += GetFlow(41, Marking) * DeltaT;
	Vars->PLVAR_UBloodEx += GetFlow(42, Marking) * DeltaT;
	Vars->PLVAR_ResB += GetFlow(43, Marking) * DeltaT;
	Vars->PLVAR_Waiting += GetFlow(44, Marking) * DeltaT;
	Vars->PLVAR_ResX += GetFlow(45, Marking) * DeltaT;
	Vars->PLVAR_UXRayEx += GetFlow(46, Marking) * DeltaT;
	Vars->PLVAR_FXRay += GetFlow(47, Marking) * DeltaT;
	Vars->PLVAR_FBloodEx += GetFlow(48, Marking) * DeltaT;
}
template<class DEDState>
double LHA<DEDState>::GetFlow(int v, const DEDState& Marking)const{
	switch (v){
		case 1:	//countT

		break;
		case 26:	//PLVAR_Arrival
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Arrival.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 31:	//PLVAR_ArrivalUrgence
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ArrivalUrgence.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 38:	//PLVAR_CountDoctor
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_CountDoctor  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 40:	//PLVAR_Doctor
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Doctor  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 48:	//PLVAR_FBloodEx
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_FBloodEx.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 47:	//PLVAR_FXRay
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_FXRay.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 24:	//PLVAR_Healthy
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Healthy.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 25:	//PLVAR_Ill
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Ill.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 39:	//PLVAR_MonitoredRoom
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_MonitoredRoom.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 30:	//PLVAR_OperatingRoom
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_OperatingRoom  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 41:	//PLVAR_PatientRecovered
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_PatientRecovered.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 23:	//PLVAR_ReadyT
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ReadyT.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 43:	//PLVAR_ResB
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ResB  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 45:	//PLVAR_ResX
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ResX  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 27:	//PLVAR_ThreatedByDocH
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ThreatedByDocH.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 28:	//PLVAR_ThreatedByDocL
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_ThreatedByDocL.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 33:	//PLVAR_TraumaTeam
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_TraumaTeam  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 42:	//PLVAR_UBloodEx
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_UBloodEx.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 32:	//PLVAR_USurgery
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_USurgery.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 46:	//PLVAR_UXRayEx
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_UXRayEx.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 34:	//PLVAR_Ustab
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Ustab.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 36:	//PLVAR_WBloodEx
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_WBloodEx.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 35:	//PLVAR_WSurgery
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_WSurgery.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 37:	//PLVAR_WXRayEx
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_WXRayEx.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 44:	//PLVAR_Waiting
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_Waiting.card()  * 0.01;

		break;
		default:	//l0,l2,
		return 0.0;

		break;
	}

		break;
		case 29:	//PLVAR_WaitingRoom
	switch (CurrentLocation){
		case 1:	//l1
			return  Marking.P->_PL_WaitingRoom.card()  * 0.01;

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
		default:	//EXRayBlood,FallIll,HospitalArrival,EToThreat,EToSurgery,HighPrio,MediumPrio,DischargeL,LowPrio,BSurgery,ToSurgery,ToDoctor,ToDoctorL,BToStabilize,EToStabilize,DischargeRec,BBlood,BXRay,DischargeM,EBloodEx,EXRay,
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
		case 1:	//
		case 23:	//
	return true;

		break;
		case 0:	//
{
         if(!( +(1)*Vars->time<=0)) return false;
		return (true);
     }

		break;
		default:	//,,,,,,,,,,,,,,,,,,,,,
{
         if(!( +(1)*Vars->time<=100)) return false;
		return (true);
     }

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
		case 23:	//
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
                  if(!(SumAX==100))
                      return EmptyInterval;
             }
             else{
                  double t=CurrentTime+(100-SumAX)/(double)SumAF;
                  if(t>=EnablingT.first && t<=EnablingT.second){
                      EnablingT.first=t; EnablingT.second=t;
                  }
                  else return EmptyInterval;
             }
             return EnablingT;
         }

		break;
		default:	//,,,,,,,,,,,,,,,,,,,,,,
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
		case 18:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->BBlood=Vars->BBlood + 0.01;
		Vars->countT = tempVars->countT;
		Vars->BBlood = tempVars->BBlood;
         }

		break;
		case 11:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->BSurgery=Vars->BSurgery + 0.01;
		Vars->countT = tempVars->countT;
		Vars->BSurgery = tempVars->BSurgery;
         }

		break;
		case 15:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->BToStabilize=Vars->BToStabilize + 0.01;
		Vars->countT = tempVars->countT;
		Vars->BToStabilize = tempVars->BToStabilize;
         }

		break;
		case 19:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->BXRay=Vars->BXRay + 0.01;
		Vars->countT = tempVars->countT;
		Vars->BXRay = tempVars->BXRay;
         }

		break;
		case 9:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->DischargeL=Vars->DischargeL + 0.01;
		Vars->countT = tempVars->countT;
		Vars->DischargeL = tempVars->DischargeL;
         }

		break;
		case 20:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->DischargeM=Vars->DischargeM + 0.01;
		Vars->countT = tempVars->countT;
		Vars->DischargeM = tempVars->DischargeM;
         }

		break;
		case 17:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->DischargeRec=Vars->DischargeRec + 0.01;
		Vars->countT = tempVars->countT;
		Vars->DischargeRec = tempVars->DischargeRec;
         }

		break;
		case 21:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EBloodEx=Vars->EBloodEx + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EBloodEx = tempVars->EBloodEx;
         }

		break;
		case 16:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EToStabilize=Vars->EToStabilize + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EToStabilize = tempVars->EToStabilize;
         }

		break;
		case 6:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EToSurgery=Vars->EToSurgery + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EToSurgery = tempVars->EToSurgery;
         }

		break;
		case 5:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EToThreat=Vars->EToThreat + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EToThreat = tempVars->EToThreat;
         }

		break;
		case 22:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EXRay=Vars->EXRay + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EXRay = tempVars->EXRay;
         }

		break;
		case 2:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->EXRayBlood=Vars->EXRayBlood + 0.01;
		Vars->countT = tempVars->countT;
		Vars->EXRayBlood = tempVars->EXRayBlood;
         }

		break;
		case 3:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->FallIll=Vars->FallIll + 0.01;
		Vars->countT = tempVars->countT;
		Vars->FallIll = tempVars->FallIll;
         }

		break;
		case 7:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->HighPrio=Vars->HighPrio + 0.01;
		Vars->countT = tempVars->countT;
		Vars->HighPrio = tempVars->HighPrio;
         }

		break;
		case 4:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->HospitalArrival=Vars->HospitalArrival + 0.01;
		Vars->countT = tempVars->countT;
		Vars->HospitalArrival = tempVars->HospitalArrival;
         }

		break;
		case 10:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->LowPrio=Vars->LowPrio + 0.01;
		Vars->countT = tempVars->countT;
		Vars->LowPrio = tempVars->LowPrio;
         }

		break;
		case 8:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->MediumPrio=Vars->MediumPrio + 0.01;
		Vars->countT = tempVars->countT;
		Vars->MediumPrio = tempVars->MediumPrio;
         }

		break;
		case 13:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->ToDoctor=Vars->ToDoctor + 0.01;
		Vars->countT = tempVars->countT;
		Vars->ToDoctor = tempVars->ToDoctor;
         }

		break;
		case 14:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->ToDoctorL=Vars->ToDoctorL + 0.01;
		Vars->countT = tempVars->countT;
		Vars->ToDoctorL = tempVars->ToDoctorL;
         }

		break;
		case 12:	//
         {
		tempVars->countT=Vars->countT + 1;
		tempVars->ToSurgery=Vars->ToSurgery + 0.01;
		Vars->countT = tempVars->countT;
		Vars->ToSurgery = tempVars->ToSurgery;
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

    LhaFunc[0]= Vars->EXRayBlood;
    LhaFunc[1]= Vars->FallIll;
    LhaFunc[2]= Vars->HospitalArrival;
    LhaFunc[3]= Vars->EToThreat;
    LhaFunc[4]= Vars->EToSurgery;
    LhaFunc[5]= Vars->HighPrio;
    LhaFunc[6]= Vars->MediumPrio;
    LhaFunc[7]= Vars->DischargeL;
    LhaFunc[8]= Vars->LowPrio;
    LhaFunc[9]= Vars->BSurgery;
    LhaFunc[10]= Vars->ToSurgery;
    LhaFunc[11]= Vars->ToDoctor;
    LhaFunc[12]= Vars->ToDoctorL;
    LhaFunc[13]= Vars->BToStabilize;
    LhaFunc[14]= Vars->EToStabilize;
    LhaFunc[15]= Vars->DischargeRec;
    LhaFunc[16]= Vars->BBlood;
    LhaFunc[17]= Vars->BXRay;
    LhaFunc[18]= Vars->DischargeM;
    LhaFunc[19]= Vars->EBloodEx;
    LhaFunc[20]= Vars->EXRay;
    LhaFunc[21]= Vars->PLVAR_ReadyT;
    LhaFunc[22]= Vars->PLVAR_Healthy;
    LhaFunc[23]= Vars->PLVAR_Ill;
    LhaFunc[24]= Vars->PLVAR_Arrival;
    LhaFunc[25]= Vars->PLVAR_ThreatedByDocH;
    LhaFunc[26]= Vars->PLVAR_ThreatedByDocL;
    LhaFunc[27]= Vars->PLVAR_WaitingRoom;
    LhaFunc[28]= Vars->PLVAR_OperatingRoom;
    LhaFunc[29]= Vars->PLVAR_ArrivalUrgence;
    LhaFunc[30]= Vars->PLVAR_USurgery;
    LhaFunc[31]= Vars->PLVAR_TraumaTeam;
    LhaFunc[32]= Vars->PLVAR_Ustab;
    LhaFunc[33]= Vars->PLVAR_WSurgery;
    LhaFunc[34]= Vars->PLVAR_WBloodEx;
    LhaFunc[35]= Vars->PLVAR_WXRayEx;
    LhaFunc[36]= Vars->PLVAR_CountDoctor;
    LhaFunc[37]= Vars->PLVAR_MonitoredRoom;
    LhaFunc[38]= Vars->PLVAR_Doctor;
    LhaFunc[39]= Vars->PLVAR_PatientRecovered;
    LhaFunc[40]= Vars->PLVAR_UBloodEx;
    LhaFunc[41]= Vars->PLVAR_ResB;
    LhaFunc[42]= Vars->PLVAR_Waiting;
    LhaFunc[43]= Vars->PLVAR_ResX;
    LhaFunc[44]= Vars->PLVAR_UXRayEx;
    LhaFunc[45]= Vars->PLVAR_FXRay;
    LhaFunc[46]= Vars->PLVAR_FBloodEx;
    FormulaVal[0]=LhaFunc[0];
    FormulaVal[1]=LhaFunc[1];
    FormulaVal[2]=LhaFunc[2];
    FormulaVal[3]=LhaFunc[3];
    FormulaVal[4]=LhaFunc[4];
    FormulaVal[5]=LhaFunc[5];
    FormulaVal[6]=LhaFunc[6];
    FormulaVal[7]=LhaFunc[7];
    FormulaVal[8]=LhaFunc[8];
    FormulaVal[9]=LhaFunc[9];
    FormulaVal[10]=LhaFunc[10];
    FormulaVal[11]=LhaFunc[11];
    FormulaVal[12]=LhaFunc[12];
    FormulaVal[13]=LhaFunc[13];
    FormulaVal[14]=LhaFunc[14];
    FormulaVal[15]=LhaFunc[15];
    FormulaVal[16]=LhaFunc[16];
    FormulaVal[17]=LhaFunc[17];
    FormulaVal[18]=LhaFunc[18];
    FormulaVal[19]=LhaFunc[19];
    FormulaVal[20]=LhaFunc[20];
    FormulaVal[21]=LhaFunc[21];
    FormulaVal[22]=LhaFunc[22];
    FormulaVal[23]=LhaFunc[23];
    FormulaVal[24]=LhaFunc[24];
    FormulaVal[25]=LhaFunc[25];
    FormulaVal[26]=LhaFunc[26];
    FormulaVal[27]=LhaFunc[27];
    FormulaVal[28]=LhaFunc[28];
    FormulaVal[29]=LhaFunc[29];
    FormulaVal[30]=LhaFunc[30];
    FormulaVal[31]=LhaFunc[31];
    FormulaVal[32]=LhaFunc[32];
    FormulaVal[33]=LhaFunc[33];
    FormulaVal[34]=LhaFunc[34];
    FormulaVal[35]=LhaFunc[35];
    FormulaVal[36]=LhaFunc[36];
    FormulaVal[37]=LhaFunc[37];
    FormulaVal[38]=LhaFunc[38];
    FormulaVal[39]=LhaFunc[39];
    FormulaVal[40]=LhaFunc[40];
    FormulaVal[41]=LhaFunc[41];
    FormulaVal[42]=LhaFunc[42];
    FormulaVal[43]=LhaFunc[43];
    FormulaVal[44]=LhaFunc[44];
    FormulaVal[45]=LhaFunc[45];
    FormulaVal[46]=LhaFunc[46];
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
