#include <string>
using namespace std;
#define temporalCount(msec) 0
#include "markingImplLight.hpp"
const void print_magic(int v){
	switch(v){
		case 1144: print("PostRead_NextVtrBeat");break;
		case 1142: print("PostRead_NextVtrBeat");break;
		case 1140: print("PostRead_NextAtrBeat");break;
		case 1138: print("PostRead_NextAtrBeat");break;
		case 1136: print("PostRead_a_dVSetHigh");break;
		case 1134: print("PostRead_a_dVSetLow");break;
		case 1132: print("PostRead_SREC");break;
		case 1130: print("PostRead_Abeat");break;
		case 1128: print("PostRead_Vbeat");break;
		case 910: print("273_Pre_Refractory");break;
		case 247: print("GenerateRetroWave");break;
		case 308: print("Vsense");break;
		case 246: print("VSynch");break;
		case 310: print("VsenseAnte");break;
		case 245: print("Refractory");break;
		case 244: print("Excitable");break;
		case 908: print("80_Pre_Refractory");break;
		case 42: print("GenerateAnteWave");break;
		case 304: print("Asense");break;
		case 218: print("ASynch");break;
		case 306: print("AsenseRetro");break;
		case 34: print("Refractory");break;
		case 35: print("Excitable");break;
		case 952: print("275_Pre_q0");break;
		case 274: print("q0");break;
		case 934: print("79_Pre_q0");break;
		case 7: print("q0");break;
		case 906: print("852_Pre_Init");break;
		case 872: print("DataReceived");break;
		case 870: print("Init");break;
		case 944: print("81_Pre_Empty");break;
		case 942: print("58_Pre_Empty");break;
		case 940: print("56_Pre_Empty");break;
		case 52: print("Retro");break;
		case 51: print("Ante");break;
		case 60: print("Empty");break;
		case 962: print("174_Pre_Empty");break;
		case 960: print("177_Pre_Empty");break;
		case 958: print("179_Pre_Empty");break;
		case 172: print("Retro");break;
		case 175: print("Ante");break;
		case 173: print("Empty");break;
		case 920: print("651_Pre_Refractory");break;
		case 918: print("650_Pre_Recovery");break;
		case 916: print("668_Pre_Refractory");break;
		case 914: print("637_Pre_Refractory");break;
		case 912: print("638_Pre_Refractory");break;
		case 625: print("Retro");break;
		case 633: print("Refractory");break;
		case 632: print("Ante");break;
		case 640: print("Recovery");break;
		case 655: print("a_dvHigh");break;
		case 654: print("a_dVLow");break;
		case 950: print("167_Pre_Empty");break;
		case 948: print("160_Pre_Empty");break;
		case 946: print("161_Pre_Empty");break;
		case 157: print("Retro");break;
		case 156: print("Ante");break;
		case 155: print("Empty");break;
		case 926: print("19_Pre_Wait4ABeat");break;
		case 924: print("16_Pre_Wait4ASynch");break;
		case 922: print("492_Pre_Wait4ABeat");break;
		case 88: print("Wait4ASynch");break;
		case 14: print("Wait4ABeat");break;
		case 932: print("265_Pre_Wait4VBeat");break;
		case 930: print("266_Pre_Wait4VSynch");break;
		case 928: print("491_Pre_Wait4VBeat");break;
		case 269: print("Wait4VSynch");break;
		case 268: print("Wait4VBeat");break;
		case 938: print("87_Pre_q0");break;
		case 936: print("86_Pre_q0");break;
		case 85: print("q0");break;
		case 956: print("278_Pre_q0");break;
		case 954: print("279_Pre_q0");break;
		case 277: print("q0");break;
		default: print((TR_PL_ID)v);break;;
	}
}
uint32 tmp32=0;
uint16 tmp16=0;
double parval=0;
uint32 value=0;
uint16 parid=0;
uint8 bufs5=0;
uint8 bufs4=0;
uint8 bufs3=0;
uint8 bufs2=0;
uint8 bufs1=0;
uint32 BAUDRATE=0;
double refractorylast=0;
double avj_refr_0=0;
double avj_refr=0;
double avj_t4=0;
double recoverylast=0;
double clk=0;
double avj_tRec=0;
double Vtr_refrD=0;
double VRG_noiseD=0;
double VRG_ectopD=0;
double VRG_d=0;
double SA_noiseD=0;
double SA_ectopdV=0;
double SA_ectopD=0;
double SA_dV=0;
double SA_d=0;
double RV_retroD=0;
double RV_anteD=0;
double RA_retroD=0;
double RA_anteD=0;
double AV_Vt=0;
double AV_Vr=0;
double AV_tr=0;
double AV_theta=0;
double AV_tauC=0;
double AV_retroDMin=0;
double AV_refrDMin=0;
double AV_k4=0;
double AV_delta=0;
double AV_beta=0;
double AV_anteDMin=0;
double AV_alpha=0;
double Atr_refrD=0;
double power(double x,double y){return pow(x,y);}

double min(double x,double y){return fmin(x,y);}

double max(double x,double y){return fmax(x,y);}

void magicReset(){}

void SWrite3(unsigned char h,unsigned  char d,unsigned  char e);

double ConvertIntToDouble(uint32 value)
{
   double parval;
   parval = *(double*)&value;
   return parval;
}

char SetParameter(int parid, float parval)
{
    if (parid==1) { 
        SA_d = parval;
        return 1;
    }
    if (parid==2) { 
        SA_noiseD = parval;
        return 1;
    }
    if (parid==3) {
        SA_ectopD = parval;
        return 1;
    }
    return 0;
}

char SReceive(void);

void SWrite(unsigned char h);
double AVJDelay(double tRec, double minDly){
//
	double t;
    t = minDly+AV_alpha*exp(-tRec/AV_tauC);
	return t;
}
double initRefrPeriod(double tRec){
//
	double t;
    t = AV_refrDMin + AV_beta*(1 - exp(-tRec/AV_tr));
	return t;
}
double modulateRefrRetro(double t, double t0){
//
	double dT;
    dT = AV_refrDMin*power(t/t0,AV_theta);
	return dT;
}
double modulateRefrAnte(double t, double t0, double dV){
//
	double dT;
    //val1 = t/t0;
    //val2 = power(t/t0,avj_theta);
    dT = AV_refrDMin*power(t/t0,AV_theta)*power(min(1,dV/(AV_Vt-AV_Vr)),AV_delta);
    //fprintf(1,'//f //f\n', val1, val2);
	return dT;
}
double timeToDep(double Vm){
//
	double t;
    t = max(0,(AV_Vt-Vm)/AV_k4);
	return t;
}
void magicUpdate(int t,double ctime){
  switch(t){
	case TR_I957_init_RT:
		VRG_ectopD=30450;
	break;
	case TR_I939_init_RT:
		SA_ectopD=10400;
	break;
	case TR_I933_init_RT:
		VRG_d=3000;
	break;
	case TR_I927_init_RT:
		SA_d=1010;
	break;
	case TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT:
		ctime=ctime+temporalCount(msec);
		avj_t4=timeToDep(AV_Vr+(ctime-recoverylast)*AV_k4+SA_dV);
	break;
	case TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT:
		ctime=ctime+temporalCount(msec);
		avj_t4=timeToDep(AV_Vr+(ctime-recoverylast)*AV_k4+SA_ectopdV);
	break;
	case TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT:
		ctime=ctime+temporalCount(msec);
	break;
	case TR_I951_init_RT:
		RV_anteD=50;
		RV_retroD=50;
	break;
	case TR_I1084_S_AVJRetroExit_L_Retro_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=ctime-recoverylast;
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I1083_S_AVJAnteExit_L_Ante_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=(ctime-recoverylast);
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I1082_S_AVJRetroExit_L_Retro_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=ctime-recoverylast;
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I1081_S_AVJAnteExit_L_Ante_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=(ctime-recoverylast);
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=(ctime-recoverylast);
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=ctime-recoverylast;
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_D634_L_Refractory_RT:
		ctime=ctime+temporalCount(msec);
		avj_t4=timeToDep(AV_Vr);
		recoverylast=ctime;
	break;
	case TR_I919_init_RT:
		AV_Vr=-90;
		AV_Vt=-40;
		AV_beta=250;
		AV_delta=10;
		AV_k4=0.030000;
		AV_refrDMin=50;
		AV_theta=10;
		AV_tr=500;
		SA_dV=50;
		SA_ectopdV=50;
		avj_t4=timeToDep(AV_Vr);
		ctime=0;
		recoverylast=0;
		refractorylast=0;
	break;
	case TR_D636_L_Recovery_RT:
		ctime=ctime+temporalCount(msec);
		avj_tRec=avj_t4;
		avj_refr_0=initRefrPeriod(avj_tRec);
		avj_refr=avj_refr_0;
		refractorylast=ctime;
	break;
	case TR_I917_L_Refractory_RT:
		ctime=ctime+temporalCount(msec);
		avj_refr=avj_refr-(ctime-refractorylast)+modulateRefrAnte((ctime-refractorylast),avj_refr_0,SA_ectopdV);
		refractorylast=ctime;
	break;
	case TR_I915_L_Refractory_RT:
		ctime=ctime+temporalCount(msec);
		avj_refr=avj_refr-(ctime-refractorylast)+modulateRefrAnte((ctime-refractorylast),avj_refr_0,SA_dV);
		refractorylast=ctime;
	break;
	case TR_I913_L_Refractory_RT:
		ctime=ctime+temporalCount(msec);
		avj_refr=avj_refr-(ctime-refractorylast)+modulateRefrRetro((ctime-refractorylast),avj_refr_0);
		refractorylast=ctime;
	break;
	case TR_I963_init_RT:
		AV_alpha=150;
		AV_anteDMin=50;
		AV_retroDMin=100;
		AV_tauC=100;
	break;
	case TR_I945_init_RT:
		RA_anteD=30;
		RA_retroD=30;
	break;
	case TR_I1133_L_Init_RT:
		bufs1=SReceive();
	break;
	case TR_I907_init_RT:
		BAUDRATE=115200;
	break;
	case TR_I935_init_RT:
		SA_noiseD=1000000;
	break;
	case TR_I953_init_RT:
		VRG_noiseD=1000000;
	break;
	case TR_I909_init_RT:
		Atr_refrD=50;
	break;
	case TR_I307_L_AsenseRetro_RT:
		SWrite(0x31);
	break;
	case TR_I616_L_Asense_RT:
		SWrite(0x31);
	break;
	case TR_I911_init_RT:
		Vtr_refrD=200;
	break;
	case TR_I311_L_VsenseAnte_RT:
		SWrite(0x32);
	break;
	case TR_I309_L_Vsense_RT:
		SWrite(0x32);
	break;
	default: break; 
	}
}
bool magicConditional(int t){
  switch(t){
	case TR_C1107_S_VP_L_DataReceived_RT:
		return bufs1==0x34;
	case TR_C1106_S_VP_L_Excitable_L_DataReceived_RT:
		return bufs1==0x34;
	case TR_C1077_S_AP_L_DataReceived_RT:
		return bufs1==0x33;
	case TR_C1076_S_AP_L_Excitable_L_DataReceived_RT:
		return bufs1==0x33;
	case TR_C844_L_DataReceived_RT:
		return ((bufs1!=0x33 && bufs1!=0x34));
	default: return true; 
	}
}
