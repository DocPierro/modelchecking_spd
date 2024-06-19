#include "spnLight.hpp"
#define PL_SIG_Abeat_LP 0
#define PL_SIG_NextAtrBeat_LP 1
#define PL_SIG_NextVtrBeat_LP 2
#define PL_SIG_SREC_LP 3
#define PL_SIG_Vbeat_LP 4
#define PL_SIG_a_dVSetHigh_LP 5
#define PL_SIG_a_dVSetLow_LP 6
#define PL_Ventricle_LP 7
#define PL_Atrium_LP 8
#define PL_VtrNoiseGenerator_LP 9
#define PL_AtrNoiseGenerator_LP 10
#define PL_Parameter_Serial_LP 11
#define PL_RAConductor_LP 12
#define PL_AVJOut_LP 13
#define PL_AVJ_LP 14
#define PL_A_dV_LP 15
#define PL_RVConductor_LP 16
#define PL_SANode_LP 17
#define PL_VRG_LP 18
#define PL_SANodeEctopic_LP 19
#define PL_VRGEctopic_LP 20
#define TR_EMPTY_Abeat_RT 0
#define TR_EMPTY_NextAtrBeat_RT 1
#define TR_EMPTY_NextVtrBeat_RT 2
#define TR_EMPTY_SREC_RT 3
#define TR_EMPTY_Vbeat_RT 4
#define TR_EMPTY_a_dVSetHigh_RT 5
#define TR_EMPTY_a_dVSetLow_RT 6
#define TR_INCOMING_SREC_RT 7
#define TR_I957_init_RT 8
#define TR_D279_q0_RT 9
#define TR_I955_L_q0_RT 10
#define TR_I939_init_RT 11
#define TR_D86_q0_RT 12
#define TR_I937_L_q0_RT 13
#define TR_I933_init_RT 14
#define TR_D266_Wait4VBeat_RT 15
#define TR_I931_L_Wait4VBeat_RT 16
#define TR_RNextVtrBeat267__RT 17
#define TR_I1145_L_Wait4VSynch_RT 18
#define TR_RNextVtrBeat491__RT 19
#define TR_I1143_NextVtrBeat_RT 20
#define TR_I929_L_Wait4VBeat_RT 21
#define TR_I927_init_RT 22
#define TR_D16_Wait4ABeat_RT 23
#define TR_I925_L_Wait4ABeat_RT 24
#define TR_RNextAtrBeat492__RT 25
#define TR_I1141_NextAtrBeat_RT 26
#define TR_I923_L_Wait4ABeat_RT 27
#define TR_RNextAtrBeat89__RT 28
#define TR_I1139_L_Wait4ASynch_RT 29
#define TR_I1122_S_retroAVJ_L_Retro_RT 30
#define TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT 31
#define TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT 32
#define TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT 33
#define TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT 34
#define TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT 35
#define TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT 36
#define TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT 37
#define TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT 38
#define TR_I1113_S_VtrAnteReached_L_Ante_RT 39
#define TR_I1112_S_VtrAnteReached_L_Excitable_L_Ante_RT 40
#define TR_I1111_S_vrg_L_GenerateRetroWave_RT 41
#define TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT 42
#define TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT 43
#define TR_I1098_S_AVJRetroReached_L_Retro_RT 44
#define TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT 45
#define TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT 46
#define TR_I1095_S_anteAVJ_L_Ante_RT 47
#define TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT 48
#define TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT 49
#define TR_I951_init_RT 50
#define TR_D160_Ante_RT 51
#define TR_D161_Retro_RT 52
#define TR_Ra_dVSetHigh656__RT 53
#define TR_I1137_L_a_dVLow_RT 54
#define TR_Ra_dVSetLow657__RT 55
#define TR_I1135_L_a_dvHigh_RT 56
#define TR_I1084_S_AVJRetroExit_L_Retro_RT 57
#define TR_I1083_S_AVJAnteExit_L_Ante_RT 58
#define TR_I1082_S_AVJRetroExit_L_Retro_RT 59
#define TR_I1081_S_AVJAnteExit_L_Ante_RT 60
#define TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT 61
#define TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT 62
#define TR_D651_Ante_RT 63
#define TR_D634_L_Refractory_RT 64
#define TR_I919_init_RT 65
#define TR_D636_L_Recovery_RT 66
#define TR_I917_L_Refractory_RT 67
#define TR_I915_L_Refractory_RT 68
#define TR_I913_L_Refractory_RT 69
#define TR_I963_init_RT 70
#define TR_D177_Ante_RT 71
#define TR_D179_Retro_RT 72
#define TR_C1107_S_VP_L_DataReceived_RT 73
#define TR_C1106_S_VP_L_Excitable_L_DataReceived_RT 74
#define TR_I1104_S_AtrRetroReached_L_Retro_RT 75
#define TR_I1103_S_AtrRetroReached_L_Excitable_L_Retro_RT 76
#define TR_I1102_S_arg_L_GenerateAnteWave_RT 77
#define TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT 78
#define TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT 79
#define TR_I945_init_RT 80
#define TR_D58_Retro_RT 81
#define TR_D56_Ante_RT 82
#define TR_C1077_S_AP_L_DataReceived_RT 83
#define TR_C1076_S_AP_L_Excitable_L_DataReceived_RT 84
#define TR_RSREC866__RT 85
#define TR_I1133_L_Init_RT 86
#define TR_I907_init_RT 87
#define TR_C844_L_DataReceived_RT 88
#define TR_D10_L_q0_RT 89
#define TR_I935_init_RT 90
#define TR_I953_init_RT 91
#define TR_D276_L_q0_RT 92
#define TR_I909_init_RT 93
#define TR_RAbeat36__RT 94
#define TR_I1131_L_Excitable_RT 95
#define TR_D40_L_Refractory_RT 96
#define TR_I92_L_ASynch_RT 97
#define TR_I307_L_AsenseRetro_RT 98
#define TR_I616_L_Asense_RT 99
#define TR_I911_init_RT 100
#define TR_D248_L_Refractory_RT 101
#define TR_RVbeat249__RT 102
#define TR_I1129_L_Excitable_RT 103
#define TR_I251_L_VSynch_RT 104
#define TR_I311_L_VsenseAnte_RT 105
#define TR_I309_L_Vsense_RT 106

	const int _nb_Place_SIG_Abeat=0;
	const int _nb_Place_SIG_NextAtrBeat=1;
	const int _nb_Place_SIG_NextVtrBeat=2;
	const int _nb_Place_SIG_SREC=3;
	const int _nb_Place_SIG_Vbeat=4;
	const int _nb_Place_SIG_a_dVSetHigh=5;
	const int _nb_Place_SIG_a_dVSetLow=6;
	const int _nb_Place_Ventricle=7;
	const int _nb_Place_Atrium=8;
	const int _nb_Place_VtrNoiseGenerator=9;
	const int _nb_Place_AtrNoiseGenerator=10;
	const int _nb_Place_Parameter_Serial=11;
	const int _nb_Place_RAConductor=12;
	const int _nb_Place_AVJOut=13;
	const int _nb_Place_AVJ=14;
	const int _nb_Place_A_dV=15;
	const int _nb_Place_RVConductor=16;
	const int _nb_Place_SANode=17;
	const int _nb_Place_VRG=18;
	const int _nb_Place_SANodeEctopic=19;
	const int _nb_Place_VRGEctopic=20;
#include "magicLight.hpp"
#include "markingImplLight.hpp"

void abstractMarking::resetToInitMarking(){
	magicReset();
	P->_PL_SIG_Abeat =0;
	P->_PL_SIG_NextAtrBeat =0;
	P->_PL_SIG_NextVtrBeat =0;
	P->_PL_SIG_SREC =0;
	P->_PL_SIG_Vbeat =0;
	P->_PL_SIG_a_dVSetHigh =0;
	P->_PL_SIG_a_dVSetLow =0;
	P->_PL_Ventricle =910;
	P->_PL_Atrium =908;
	P->_PL_VtrNoiseGenerator =952;
	P->_PL_AtrNoiseGenerator =934;
	P->_PL_Parameter_Serial =906;
	P->_PL_RAConductor =944;
	P->_PL_AVJOut =962;
	P->_PL_AVJ =918;
	P->_PL_A_dV =654;
	P->_PL_RVConductor =950;
	P->_PL_SANode =926;
	P->_PL_VRG =932;
	P->_PL_SANodeEctopic =938;
	P->_PL_VRGEctopic =956;
}


abstractMarking::abstractMarking() {
	P= new abstractMarkingImpl;
	resetToInitMarking();
}

abstractMarking::~abstractMarking() {
	delete(P);
}


void abstractMarking::printHeader()const{
::print("SIG_Abeat	");
::print("SIG_NextAtrBeat	");
::print("SIG_NextVtrBeat	");
::print("SIG_SREC	");
::print("SIG_Vbeat	");
::print("SIG_a_dVSetHigh	");
::print("SIG_a_dVSetLow	");
::print("Ventricle	");
::print("Atrium	");
::print("VtrNoiseGenerator	");
::print("AtrNoiseGenerator	");
::print("Parameter_Serial	");
::print("RAConductor	");
::print("AVJOut	");
::print("AVJ	");
::print("A_dV	");
::print("RVConductor	");
::print("SANode	");
::print("VRG	");
::print("SANodeEctopic	");
::print("VRGEctopic	");
}

void abstractMarking::print()const{
print_magic(P->_PL_SIG_Abeat);
::print("	");
print_magic(P->_PL_SIG_NextAtrBeat);
::print("	");
print_magic(P->_PL_SIG_NextVtrBeat);
::print("	");
print_magic(P->_PL_SIG_SREC);
::print("	");
print_magic(P->_PL_SIG_Vbeat);
::print("	");
print_magic(P->_PL_SIG_a_dVSetHigh);
::print("	");
print_magic(P->_PL_SIG_a_dVSetLow);
::print("	");
print_magic(P->_PL_Ventricle);
::print("	");
print_magic(P->_PL_Atrium);
::print("	");
print_magic(P->_PL_VtrNoiseGenerator);
::print("	");
print_magic(P->_PL_AtrNoiseGenerator);
::print("	");
print_magic(P->_PL_Parameter_Serial);
::print("	");
print_magic(P->_PL_RAConductor);
::print("	");
print_magic(P->_PL_AVJOut);
::print("	");
print_magic(P->_PL_AVJ);
::print("	");
print_magic(P->_PL_A_dV);
::print("	");
print_magic(P->_PL_RVConductor);
::print("	");
print_magic(P->_PL_SANode);
::print("	");
print_magic(P->_PL_VRG);
::print("	");
print_magic(P->_PL_SANodeEctopic);
::print("	");
print_magic(P->_PL_VRGEctopic);
::print("	");
}
static const TR_PL_ID EMPTY_array[1]={-1};
static const TR_PL_ID PE_PossiblyEnabled_0[3]= {TR_I937_L_q0_RT, TR_I925_L_Wait4ABeat_RT, -1 }; /* EMPTY_Abeat*/
static const TR_PL_ID PE_PossiblyEnabled_1[2]= {TR_I92_L_ASynch_RT, -1 }; /* EMPTY_NextAtrBeat*/
static const TR_PL_ID PE_PossiblyEnabled_2[2]= {TR_I251_L_VSynch_RT, -1 }; /* EMPTY_NextVtrBeat*/
static const TR_PL_ID PE_PossiblyEnabled_3[2]= {TR_INCOMING_SREC_RT, -1 }; /* EMPTY_SREC*/
static const TR_PL_ID PE_PossiblyEnabled_4[3]= {TR_I955_L_q0_RT, TR_I931_L_Wait4VBeat_RT, -1 }; /* EMPTY_Vbeat*/
static const TR_PL_ID PE_PossiblyEnabled_5[2]= {TR_I937_L_q0_RT, -1 }; /* EMPTY_a_dVSetHigh*/
static const TR_PL_ID PE_PossiblyEnabled_6[2]= {TR_I925_L_Wait4ABeat_RT, -1 }; /* EMPTY_a_dVSetLow*/
static const TR_PL_ID PE_PossiblyEnabled_7[3]= {TR_EMPTY_SREC_RT, TR_RSREC866__RT, -1 }; /* INCOMING_SREC*/
static const TR_PL_ID PE_PossiblyEnabled_8[2]= {TR_D279_q0_RT, -1 }; /* I957_init*/
static const TR_PL_ID PE_PossiblyEnabled_9[2]= {TR_I955_L_q0_RT, -1 }; /* D279_q0*/
static const TR_PL_ID PE_PossiblyEnabled_10[4]= {TR_EMPTY_Vbeat_RT, TR_D279_q0_RT, TR_RVbeat249__RT, -1 }; /* I955_L_q0*/
static const TR_PL_ID PE_PossiblyEnabled_11[2]= {TR_D86_q0_RT, -1 }; /* I939_init*/
static const TR_PL_ID PE_PossiblyEnabled_12[2]= {TR_I937_L_q0_RT, -1 }; /* D86_q0*/
static const TR_PL_ID PE_PossiblyEnabled_13[6]= {TR_EMPTY_Abeat_RT, TR_EMPTY_a_dVSetHigh_RT, TR_D86_q0_RT, TR_Ra_dVSetHigh656__RT, TR_RAbeat36__RT, -1 }; /* I937_L_q0*/
static const TR_PL_ID PE_PossiblyEnabled_14[3]= {TR_D266_Wait4VBeat_RT, TR_RNextVtrBeat491__RT, -1 }; /* I933_init*/
static const TR_PL_ID PE_PossiblyEnabled_15[2]= {TR_I931_L_Wait4VBeat_RT, -1 }; /* D266_Wait4VBeat*/
static const TR_PL_ID PE_PossiblyEnabled_16[4]= {TR_EMPTY_Vbeat_RT, TR_RNextVtrBeat267__RT, TR_RVbeat249__RT, -1 }; /* I931_L_Wait4VBeat*/
static const TR_PL_ID PE_PossiblyEnabled_17[5]= {TR_EMPTY_NextVtrBeat_RT, TR_I1145_L_Wait4VSynch_RT, TR_RNextVtrBeat491__RT, TR_I251_L_VSynch_RT, -1 }; /* RNextVtrBeat267_*/
static const TR_PL_ID PE_PossiblyEnabled_18[3]= {TR_D266_Wait4VBeat_RT, TR_RNextVtrBeat491__RT, -1 }; /* I1145_L_Wait4VSynch*/
static const TR_PL_ID PE_PossiblyEnabled_19[5]= {TR_EMPTY_NextVtrBeat_RT, TR_RNextVtrBeat267__RT, TR_I1143_NextVtrBeat_RT, TR_I251_L_VSynch_RT, -1 }; /* RNextVtrBeat491_*/
static const TR_PL_ID PE_PossiblyEnabled_20[2]= {TR_I929_L_Wait4VBeat_RT, -1 }; /* I1143_NextVtrBeat*/
static const TR_PL_ID PE_PossiblyEnabled_21[3]= {TR_D266_Wait4VBeat_RT, TR_RNextVtrBeat491__RT, -1 }; /* I929_L_Wait4VBeat*/
static const TR_PL_ID PE_PossiblyEnabled_22[3]= {TR_D16_Wait4ABeat_RT, TR_RNextAtrBeat492__RT, -1 }; /* I927_init*/
static const TR_PL_ID PE_PossiblyEnabled_23[2]= {TR_I925_L_Wait4ABeat_RT, -1 }; /* D16_Wait4ABeat*/
static const TR_PL_ID PE_PossiblyEnabled_24[6]= {TR_EMPTY_Abeat_RT, TR_EMPTY_a_dVSetLow_RT, TR_RNextAtrBeat89__RT, TR_Ra_dVSetLow657__RT, TR_RAbeat36__RT, -1 }; /* I925_L_Wait4ABeat*/
static const TR_PL_ID PE_PossiblyEnabled_25[5]= {TR_EMPTY_NextAtrBeat_RT, TR_I1141_NextAtrBeat_RT, TR_RNextAtrBeat89__RT, TR_I92_L_ASynch_RT, -1 }; /* RNextAtrBeat492_*/
static const TR_PL_ID PE_PossiblyEnabled_26[2]= {TR_I923_L_Wait4ABeat_RT, -1 }; /* I1141_NextAtrBeat*/
static const TR_PL_ID PE_PossiblyEnabled_27[3]= {TR_D16_Wait4ABeat_RT, TR_RNextAtrBeat492__RT, -1 }; /* I923_L_Wait4ABeat*/
static const TR_PL_ID PE_PossiblyEnabled_28[5]= {TR_EMPTY_NextAtrBeat_RT, TR_RNextAtrBeat492__RT, TR_I1139_L_Wait4ASynch_RT, TR_I92_L_ASynch_RT, -1 }; /* RNextAtrBeat89_*/
static const TR_PL_ID PE_PossiblyEnabled_29[3]= {TR_D16_Wait4ABeat_RT, TR_RNextAtrBeat492__RT, -1 }; /* I1139_L_Wait4ASynch*/
static const TR_PL_ID PE_PossiblyEnabled_30[5]= {TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, TR_D58_Retro_RT, -1 }; /* I1122_S_retroAVJ_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_31[4]= {TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1121_S_retroAVJ_L_Ante_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_32[5]= {TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, TR_D58_Retro_RT, -1 }; /* I1120_S_retroAVJ_L_Empty_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_33[6]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_D651_Ante_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyEnabled_34[6]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_D651_Ante_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyEnabled_35[6]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_D651_Ante_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyEnabled_36[6]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_D651_Ante_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyEnabled_37[5]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_I917_L_Refractory_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyEnabled_38[5]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_I915_L_Refractory_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyEnabled_39[4]= {TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_C1107_S_VP_L_DataReceived_RT, TR_D248_L_Refractory_RT, -1 }; /* I1113_S_VtrAnteReached_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_40[3]= {TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_I311_L_VsenseAnte_RT, -1 }; /* I1112_S_VtrAnteReached_L_Excitable_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_41[4]= {TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT, TR_D161_Retro_RT, TR_I251_L_VSynch_RT, -1 }; /* I1111_S_vrg_L_GenerateRetroWave*/
static const TR_PL_ID PE_PossiblyEnabled_42[4]= {TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT, TR_D161_Retro_RT, TR_I251_L_VSynch_RT, -1 }; /* I1110_S_vrg_L_GenerateRetroWave_L_Empty*/
static const TR_PL_ID PE_PossiblyEnabled_43[3]= {TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_I251_L_VSynch_RT, -1 }; /* I1109_S_vrg_L_GenerateRetroWave_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_44[6]= {TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_D651_Ante_RT, -1 }; /* I1098_S_AVJRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_45[6]= {TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_I1084_S_AVJRetroExit_L_Retro_RT, TR_I1082_S_AVJRetroExit_L_Retro_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, -1 }; /* I1097_S_AVJRetroReached_L_Recovery_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_46[4]= {TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_I913_L_Refractory_RT, -1 }; /* I1096_S_AVJRetroReached_AVJRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_47[5]= {TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, TR_D160_Ante_RT, TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, -1 }; /* I1095_S_anteAVJ_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_48[5]= {TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, TR_D160_Ante_RT, TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, -1 }; /* I1094_S_anteAVJ_L_Ante_L_Empty*/
static const TR_PL_ID PE_PossiblyEnabled_49[4]= {TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, -1 }; /* I1093_S_anteAVJ_L_Ante_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_50[3]= {TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, -1 }; /* I951_init*/
static const TR_PL_ID PE_PossiblyEnabled_51[3]= {TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_I1112_S_VtrAnteReached_L_Excitable_L_Ante_RT, -1 }; /* D160_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_52[4]= {TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, -1 }; /* D161_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_53[4]= {TR_EMPTY_a_dVSetHigh_RT, TR_I937_L_q0_RT, TR_I1137_L_a_dVLow_RT, -1 }; /* Ra_dVSetHigh656_*/
static const TR_PL_ID PE_PossiblyEnabled_54[5]= {TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_Ra_dVSetLow657__RT, -1 }; /* I1137_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyEnabled_55[4]= {TR_EMPTY_a_dVSetLow_RT, TR_I925_L_Wait4ABeat_RT, TR_I1135_L_a_dvHigh_RT, -1 }; /* Ra_dVSetLow657_*/
static const TR_PL_ID PE_PossiblyEnabled_56[5]= {TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_Ra_dVSetHigh656__RT, -1 }; /* I1135_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyEnabled_57[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D177_Ante_RT, -1 }; /* I1084_S_AVJRetroExit_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_58[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D177_Ante_RT, -1 }; /* I1083_S_AVJAnteExit_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_59[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D179_Retro_RT, -1 }; /* I1082_S_AVJRetroExit_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_60[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D179_Retro_RT, -1 }; /* I1081_S_AVJAnteExit_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_61[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D177_Ante_RT, -1 }; /* I1080_S_AVJAnteExit_L_Empty_L_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_62[6]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, TR_D179_Retro_RT, -1 }; /* I1079_S_AVJRetroExit_L_Empty_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_63[4]= {TR_I1083_S_AVJAnteExit_L_Ante_RT, TR_I1081_S_AVJAnteExit_L_Ante_RT, TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, -1 }; /* D651_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_64[5]= {TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, TR_D636_L_Recovery_RT, -1 }; /* D634_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_65[5]= {TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, TR_D636_L_Recovery_RT, -1 }; /* I919_init*/
static const TR_PL_ID PE_PossiblyEnabled_66[5]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, -1 }; /* D636_L_Recovery*/
static const TR_PL_ID PE_PossiblyEnabled_67[5]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, -1 }; /* I917_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_68[5]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, -1 }; /* I915_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_69[5]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_D634_L_Refractory_RT, -1 }; /* I913_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_70[3]= {TR_I1080_S_AVJAnteExit_L_Empty_L_Ante_RT, TR_I1079_S_AVJRetroExit_L_Empty_L_Retro_RT, -1 }; /* I963_init*/
static const TR_PL_ID PE_PossiblyEnabled_71[4]= {TR_I1095_S_anteAVJ_L_Ante_RT, TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT, -1 }; /* D177_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_72[4]= {TR_I1122_S_retroAVJ_L_Retro_RT, TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT, TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, -1 }; /* D179_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_73[4]= {TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_RSREC866__RT, TR_D248_L_Refractory_RT, -1 }; /* C1107_S_VP_L_DataReceived*/
static const TR_PL_ID PE_PossiblyEnabled_74[5]= {TR_I1111_S_vrg_L_GenerateRetroWave_RT, TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, TR_RSREC866__RT, -1 }; /* C1106_S_VP_L_Excitable_L_DataReceived*/
static const TR_PL_ID PE_PossiblyEnabled_75[4]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_D40_L_Refractory_RT, -1 }; /* I1104_S_AtrRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_76[3]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I307_L_AsenseRetro_RT, -1 }; /* I1103_S_AtrRetroReached_L_Excitable_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_77[4]= {TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT, TR_D56_Ante_RT, TR_I92_L_ASynch_RT, -1 }; /* I1102_S_arg_L_GenerateAnteWave*/
static const TR_PL_ID PE_PossiblyEnabled_78[4]= {TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT, TR_D56_Ante_RT, TR_I92_L_ASynch_RT, -1 }; /* I1101_S_arg_L_GenerateAnteWave_L_Empty*/
static const TR_PL_ID PE_PossiblyEnabled_79[3]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I92_L_ASynch_RT, -1 }; /* I1100_S_arg_L_GenerateAnteWave_L_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_80[3]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I945_init*/
static const TR_PL_ID PE_PossiblyEnabled_81[3]= {TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_I1103_S_AtrRetroReached_L_Excitable_L_Retro_RT, -1 }; /* D58_Retro*/
static const TR_PL_ID PE_PossiblyEnabled_82[7]= {TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, -1 }; /* D56_Ante*/
static const TR_PL_ID PE_PossiblyEnabled_83[4]= {TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_RSREC866__RT, TR_D40_L_Refractory_RT, -1 }; /* C1077_S_AP_L_DataReceived*/
static const TR_PL_ID PE_PossiblyEnabled_84[5]= {TR_I1102_S_arg_L_GenerateAnteWave_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, TR_RSREC866__RT, -1 }; /* C1076_S_AP_L_Excitable_L_DataReceived*/
static const TR_PL_ID PE_PossiblyEnabled_85[4]= {TR_EMPTY_SREC_RT, TR_INCOMING_SREC_RT, TR_I1133_L_Init_RT, -1 }; /* RSREC866_*/
static const TR_PL_ID PE_PossiblyEnabled_86[6]= {TR_C1107_S_VP_L_DataReceived_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, TR_C844_L_DataReceived_RT, -1 }; /* I1133_L_Init*/
static const TR_PL_ID PE_PossiblyEnabled_87[2]= {TR_RSREC866__RT, -1 }; /* I907_init*/
static const TR_PL_ID PE_PossiblyEnabled_88[2]= {TR_RSREC866__RT, -1 }; /* C844_L_DataReceived*/
static const TR_PL_ID PE_PossiblyEnabled_90[2]= {TR_D10_L_q0_RT, -1 }; /* I935_init*/
static const TR_PL_ID PE_PossiblyEnabled_91[2]= {TR_D276_L_q0_RT, -1 }; /* I953_init*/
static const TR_PL_ID PE_PossiblyEnabled_93[4]= {TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_D40_L_Refractory_RT, -1 }; /* I909_init*/
static const TR_PL_ID PE_PossiblyEnabled_94[5]= {TR_EMPTY_Abeat_RT, TR_I937_L_q0_RT, TR_I925_L_Wait4ABeat_RT, TR_I1131_L_Excitable_RT, -1 }; /* RAbeat36_*/
static const TR_PL_ID PE_PossiblyEnabled_95[2]= {TR_I616_L_Asense_RT, -1 }; /* I1131_L_Excitable*/
static const TR_PL_ID PE_PossiblyEnabled_96[4]= {TR_I1103_S_AtrRetroReached_L_Excitable_L_Retro_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, TR_RAbeat36__RT, -1 }; /* D40_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_97[7]= {TR_EMPTY_NextAtrBeat_RT, TR_RNextAtrBeat492__RT, TR_RNextAtrBeat89__RT, TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_D40_L_Refractory_RT, -1 }; /* I92_L_ASynch*/
static const TR_PL_ID PE_PossiblyEnabled_98[2]= {TR_I92_L_ASynch_RT, -1 }; /* I307_L_AsenseRetro*/
static const TR_PL_ID PE_PossiblyEnabled_99[4]= {TR_I1102_S_arg_L_GenerateAnteWave_RT, TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, -1 }; /* I616_L_Asense*/
static const TR_PL_ID PE_PossiblyEnabled_100[4]= {TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_C1107_S_VP_L_DataReceived_RT, TR_D248_L_Refractory_RT, -1 }; /* I911_init*/
static const TR_PL_ID PE_PossiblyEnabled_101[4]= {TR_I1112_S_VtrAnteReached_L_Excitable_L_Ante_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_RVbeat249__RT, -1 }; /* D248_L_Refractory*/
static const TR_PL_ID PE_PossiblyEnabled_102[5]= {TR_EMPTY_Vbeat_RT, TR_I955_L_q0_RT, TR_I931_L_Wait4VBeat_RT, TR_I1129_L_Excitable_RT, -1 }; /* RVbeat249_*/
static const TR_PL_ID PE_PossiblyEnabled_103[2]= {TR_I309_L_Vsense_RT, -1 }; /* I1129_L_Excitable*/
static const TR_PL_ID PE_PossiblyEnabled_104[7]= {TR_EMPTY_NextVtrBeat_RT, TR_RNextVtrBeat267__RT, TR_RNextVtrBeat491__RT, TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_C1107_S_VP_L_DataReceived_RT, TR_D248_L_Refractory_RT, -1 }; /* I251_L_VSynch*/
static const TR_PL_ID PE_PossiblyEnabled_105[2]= {TR_I251_L_VSynch_RT, -1 }; /* I311_L_VsenseAnte*/
static const TR_PL_ID PE_PossiblyEnabled_106[4]= {TR_I1111_S_vrg_L_GenerateRetroWave_RT, TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, -1 }; /* I309_L_Vsense*/
const TR_PL_ID* SPN::PossiblyEnabled[] = {PE_PossiblyEnabled_0, PE_PossiblyEnabled_1, PE_PossiblyEnabled_2, PE_PossiblyEnabled_3, PE_PossiblyEnabled_4, PE_PossiblyEnabled_5, PE_PossiblyEnabled_6, PE_PossiblyEnabled_7, PE_PossiblyEnabled_8, PE_PossiblyEnabled_9, PE_PossiblyEnabled_10, PE_PossiblyEnabled_11, PE_PossiblyEnabled_12, PE_PossiblyEnabled_13, PE_PossiblyEnabled_14, PE_PossiblyEnabled_15, PE_PossiblyEnabled_16, PE_PossiblyEnabled_17, PE_PossiblyEnabled_18, PE_PossiblyEnabled_19, PE_PossiblyEnabled_20, PE_PossiblyEnabled_21, PE_PossiblyEnabled_22, PE_PossiblyEnabled_23, PE_PossiblyEnabled_24, PE_PossiblyEnabled_25, PE_PossiblyEnabled_26, PE_PossiblyEnabled_27, PE_PossiblyEnabled_28, PE_PossiblyEnabled_29, PE_PossiblyEnabled_30, PE_PossiblyEnabled_31, PE_PossiblyEnabled_32, PE_PossiblyEnabled_33, PE_PossiblyEnabled_34, PE_PossiblyEnabled_35, PE_PossiblyEnabled_36, PE_PossiblyEnabled_37, PE_PossiblyEnabled_38, PE_PossiblyEnabled_39, PE_PossiblyEnabled_40, PE_PossiblyEnabled_41, PE_PossiblyEnabled_42, PE_PossiblyEnabled_43, PE_PossiblyEnabled_44, PE_PossiblyEnabled_45, PE_PossiblyEnabled_46, PE_PossiblyEnabled_47, PE_PossiblyEnabled_48, PE_PossiblyEnabled_49, PE_PossiblyEnabled_50, PE_PossiblyEnabled_51, PE_PossiblyEnabled_52, PE_PossiblyEnabled_53, PE_PossiblyEnabled_54, PE_PossiblyEnabled_55, PE_PossiblyEnabled_56, PE_PossiblyEnabled_57, PE_PossiblyEnabled_58, PE_PossiblyEnabled_59, PE_PossiblyEnabled_60, PE_PossiblyEnabled_61, PE_PossiblyEnabled_62, PE_PossiblyEnabled_63, PE_PossiblyEnabled_64, PE_PossiblyEnabled_65, PE_PossiblyEnabled_66, PE_PossiblyEnabled_67, PE_PossiblyEnabled_68, PE_PossiblyEnabled_69, PE_PossiblyEnabled_70, PE_PossiblyEnabled_71, PE_PossiblyEnabled_72, PE_PossiblyEnabled_73, PE_PossiblyEnabled_74, PE_PossiblyEnabled_75, PE_PossiblyEnabled_76, PE_PossiblyEnabled_77, PE_PossiblyEnabled_78, PE_PossiblyEnabled_79, PE_PossiblyEnabled_80, PE_PossiblyEnabled_81, PE_PossiblyEnabled_82, PE_PossiblyEnabled_83, PE_PossiblyEnabled_84, PE_PossiblyEnabled_85, PE_PossiblyEnabled_86, PE_PossiblyEnabled_87, PE_PossiblyEnabled_88, EMPTY_array, PE_PossiblyEnabled_90, PE_PossiblyEnabled_91, EMPTY_array, PE_PossiblyEnabled_93, PE_PossiblyEnabled_94, PE_PossiblyEnabled_95, PE_PossiblyEnabled_96, PE_PossiblyEnabled_97, PE_PossiblyEnabled_98, PE_PossiblyEnabled_99, PE_PossiblyEnabled_100, PE_PossiblyEnabled_101, PE_PossiblyEnabled_102, PE_PossiblyEnabled_103, PE_PossiblyEnabled_104, PE_PossiblyEnabled_105, PE_PossiblyEnabled_106};

static const TR_PL_ID PE_PossiblyDisabled_0[2]= {TR_RAbeat36__RT, -1 }; /* EMPTY_Abeat*/
static const TR_PL_ID PE_PossiblyDisabled_1[3]= {TR_RNextAtrBeat492__RT, TR_RNextAtrBeat89__RT, -1 }; /* EMPTY_NextAtrBeat*/
static const TR_PL_ID PE_PossiblyDisabled_2[3]= {TR_RNextVtrBeat267__RT, TR_RNextVtrBeat491__RT, -1 }; /* EMPTY_NextVtrBeat*/
static const TR_PL_ID PE_PossiblyDisabled_3[2]= {TR_RSREC866__RT, -1 }; /* EMPTY_SREC*/
static const TR_PL_ID PE_PossiblyDisabled_4[2]= {TR_RVbeat249__RT, -1 }; /* EMPTY_Vbeat*/
static const TR_PL_ID PE_PossiblyDisabled_5[2]= {TR_Ra_dVSetHigh656__RT, -1 }; /* EMPTY_a_dVSetHigh*/
static const TR_PL_ID PE_PossiblyDisabled_6[2]= {TR_Ra_dVSetLow657__RT, -1 }; /* EMPTY_a_dVSetLow*/
static const TR_PL_ID PE_PossiblyDisabled_10[2]= {TR_I931_L_Wait4VBeat_RT, -1 }; /* I955_L_q0*/
static const TR_PL_ID PE_PossiblyDisabled_13[2]= {TR_I925_L_Wait4ABeat_RT, -1 }; /* I937_L_q0*/
static const TR_PL_ID PE_PossiblyDisabled_15[2]= {TR_RNextVtrBeat491__RT, -1 }; /* D266_Wait4VBeat*/
static const TR_PL_ID PE_PossiblyDisabled_16[2]= {TR_I955_L_q0_RT, -1 }; /* I931_L_Wait4VBeat*/
static const TR_PL_ID PE_PossiblyDisabled_17[4]= {TR_EMPTY_NextVtrBeat_RT, TR_RNextVtrBeat491__RT, TR_I251_L_VSynch_RT, -1 }; /* RNextVtrBeat267_*/
static const TR_PL_ID PE_PossiblyDisabled_19[5]= {TR_EMPTY_NextVtrBeat_RT, TR_D266_Wait4VBeat_RT, TR_RNextVtrBeat267__RT, TR_I251_L_VSynch_RT, -1 }; /* RNextVtrBeat491_*/
static const TR_PL_ID PE_PossiblyDisabled_23[2]= {TR_RNextAtrBeat492__RT, -1 }; /* D16_Wait4ABeat*/
static const TR_PL_ID PE_PossiblyDisabled_24[2]= {TR_I937_L_q0_RT, -1 }; /* I925_L_Wait4ABeat*/
static const TR_PL_ID PE_PossiblyDisabled_25[5]= {TR_EMPTY_NextAtrBeat_RT, TR_D16_Wait4ABeat_RT, TR_RNextAtrBeat89__RT, TR_I92_L_ASynch_RT, -1 }; /* RNextAtrBeat492_*/
static const TR_PL_ID PE_PossiblyDisabled_28[4]= {TR_EMPTY_NextAtrBeat_RT, TR_RNextAtrBeat492__RT, TR_I92_L_ASynch_RT, -1 }; /* RNextAtrBeat89_*/
static const TR_PL_ID PE_PossiblyDisabled_30[3]= {TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, TR_D58_Retro_RT, -1 }; /* I1122_S_retroAVJ_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_31[3]= {TR_I1102_S_arg_L_GenerateAnteWave_RT, TR_D56_Ante_RT, -1 }; /* I1121_S_retroAVJ_L_Ante_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_32[2]= {TR_I1101_S_arg_L_GenerateAnteWave_L_Empty_RT, -1 }; /* I1120_S_retroAVJ_L_Empty_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_33[4]= {TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_D651_Ante_RT, -1 }; /* I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyDisabled_34[4]= {TR_I1098_S_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_D651_Ante_RT, -1 }; /* I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyDisabled_35[4]= {TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_D636_L_Recovery_RT, -1 }; /* I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyDisabled_36[4]= {TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_D636_L_Recovery_RT, -1 }; /* I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyDisabled_37[4]= {TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetLow657__RT, TR_D634_L_Refractory_RT, -1 }; /* I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh*/
static const TR_PL_ID PE_PossiblyDisabled_38[4]= {TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, TR_Ra_dVSetHigh656__RT, TR_D634_L_Refractory_RT, -1 }; /* I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow*/
static const TR_PL_ID PE_PossiblyDisabled_39[3]= {TR_C1107_S_VP_L_DataReceived_RT, TR_D248_L_Refractory_RT, -1 }; /* I1113_S_VtrAnteReached_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_40[3]= {TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_RVbeat249__RT, -1 }; /* I1112_S_VtrAnteReached_L_Excitable_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_41[3]= {TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT, TR_D161_Retro_RT, -1 }; /* I1111_S_vrg_L_GenerateRetroWave*/
static const TR_PL_ID PE_PossiblyDisabled_42[2]= {TR_I1094_S_anteAVJ_L_Ante_L_Empty_RT, -1 }; /* I1110_S_vrg_L_GenerateRetroWave_L_Empty*/
static const TR_PL_ID PE_PossiblyDisabled_43[3]= {TR_I1095_S_anteAVJ_L_Ante_RT, TR_D160_Ante_RT, -1 }; /* I1109_S_vrg_L_GenerateRetroWave_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_44[4]= {TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_D651_Ante_RT, -1 }; /* I1098_S_AVJRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_45[4]= {TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_D636_L_Recovery_RT, -1 }; /* I1097_S_AVJRetroReached_L_Recovery_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_46[4]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_D634_L_Refractory_RT, -1 }; /* I1096_S_AVJRetroReached_AVJRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_47[3]= {TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, TR_D160_Ante_RT, -1 }; /* I1095_S_anteAVJ_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_48[2]= {TR_I1110_S_vrg_L_GenerateRetroWave_L_Empty_RT, -1 }; /* I1094_S_anteAVJ_L_Ante_L_Empty*/
static const TR_PL_ID PE_PossiblyDisabled_49[3]= {TR_I1111_S_vrg_L_GenerateRetroWave_RT, TR_D161_Retro_RT, -1 }; /* I1093_S_anteAVJ_L_Ante_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_51[3]= {TR_I1109_S_vrg_L_GenerateRetroWave_L_Ante_RT, TR_I1095_S_anteAVJ_L_Ante_RT, -1 }; /* D160_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_52[3]= {TR_I1111_S_vrg_L_GenerateRetroWave_RT, TR_I1093_S_anteAVJ_L_Ante_L_Retro_RT, -1 }; /* D161_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_53[6]= {TR_EMPTY_a_dVSetHigh_RT, TR_I937_L_q0_RT, TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, -1 }; /* Ra_dVSetHigh656_*/
static const TR_PL_ID PE_PossiblyDisabled_55[6]= {TR_EMPTY_a_dVSetLow_RT, TR_I925_L_Wait4ABeat_RT, TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, -1 }; /* Ra_dVSetLow657_*/
static const TR_PL_ID PE_PossiblyDisabled_57[2]= {TR_D177_Ante_RT, -1 }; /* I1084_S_AVJRetroExit_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_58[2]= {TR_D177_Ante_RT, -1 }; /* I1083_S_AVJAnteExit_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_59[2]= {TR_D179_Retro_RT, -1 }; /* I1082_S_AVJRetroExit_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_60[2]= {TR_D179_Retro_RT, -1 }; /* I1081_S_AVJAnteExit_L_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_63[4]= {TR_I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow_RT, TR_I1098_S_AVJRetroReached_L_Retro_RT, -1 }; /* D651_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_64[4]= {TR_I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh_RT, TR_I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow_RT, TR_I1096_S_AVJRetroReached_AVJRetroReached_L_Retro_RT, -1 }; /* D634_L_Refractory*/
static const TR_PL_ID PE_PossiblyDisabled_66[4]= {TR_I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow_RT, TR_I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh_RT, TR_I1097_S_AVJRetroReached_L_Recovery_L_Retro_RT, -1 }; /* D636_L_Recovery*/
static const TR_PL_ID PE_PossiblyDisabled_71[3]= {TR_I1084_S_AVJRetroExit_L_Retro_RT, TR_I1083_S_AVJAnteExit_L_Ante_RT, -1 }; /* D177_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_72[3]= {TR_I1082_S_AVJRetroExit_L_Retro_RT, TR_I1081_S_AVJAnteExit_L_Ante_RT, -1 }; /* D179_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_73[6]= {TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, TR_C844_L_DataReceived_RT, TR_D248_L_Refractory_RT, -1 }; /* C1107_S_VP_L_DataReceived*/
static const TR_PL_ID PE_PossiblyDisabled_74[6]= {TR_I1112_S_VtrAnteReached_L_Excitable_L_Ante_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, TR_C844_L_DataReceived_RT, TR_RVbeat249__RT, -1 }; /* C1106_S_VP_L_Excitable_L_DataReceived*/
static const TR_PL_ID PE_PossiblyDisabled_75[3]= {TR_C1077_S_AP_L_DataReceived_RT, TR_D40_L_Refractory_RT, -1 }; /* I1104_S_AtrRetroReached_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_76[3]= {TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, TR_RAbeat36__RT, -1 }; /* I1103_S_AtrRetroReached_L_Excitable_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_77[3]= {TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT, TR_D56_Ante_RT, -1 }; /* I1102_S_arg_L_GenerateAnteWave*/
static const TR_PL_ID PE_PossiblyDisabled_78[2]= {TR_I1120_S_retroAVJ_L_Empty_L_Retro_RT, -1 }; /* I1101_S_arg_L_GenerateAnteWave_L_Empty*/
static const TR_PL_ID PE_PossiblyDisabled_79[3]= {TR_I1122_S_retroAVJ_L_Retro_RT, TR_D58_Retro_RT, -1 }; /* I1100_S_arg_L_GenerateAnteWave_L_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_81[3]= {TR_I1122_S_retroAVJ_L_Retro_RT, TR_I1100_S_arg_L_GenerateAnteWave_L_Retro_RT, -1 }; /* D58_Retro*/
static const TR_PL_ID PE_PossiblyDisabled_82[3]= {TR_I1121_S_retroAVJ_L_Ante_L_Retro_RT, TR_I1102_S_arg_L_GenerateAnteWave_RT, -1 }; /* D56_Ante*/
static const TR_PL_ID PE_PossiblyDisabled_83[6]= {TR_C1107_S_VP_L_DataReceived_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_C844_L_DataReceived_RT, TR_D40_L_Refractory_RT, -1 }; /* C1077_S_AP_L_DataReceived*/
static const TR_PL_ID PE_PossiblyDisabled_84[6]= {TR_C1107_S_VP_L_DataReceived_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_I1103_S_AtrRetroReached_L_Excitable_L_Retro_RT, TR_C844_L_DataReceived_RT, TR_RAbeat36__RT, -1 }; /* C1076_S_AP_L_Excitable_L_DataReceived*/
static const TR_PL_ID PE_PossiblyDisabled_85[3]= {TR_EMPTY_SREC_RT, TR_INCOMING_SREC_RT, -1 }; /* RSREC866_*/
static const TR_PL_ID PE_PossiblyDisabled_88[5]= {TR_C1107_S_VP_L_DataReceived_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, TR_C1077_S_AP_L_DataReceived_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, -1 }; /* C844_L_DataReceived*/
static const TR_PL_ID PE_PossiblyDisabled_94[6]= {TR_EMPTY_Abeat_RT, TR_I937_L_q0_RT, TR_I925_L_Wait4ABeat_RT, TR_I1103_S_AtrRetroReached_L_Excitable_L_Retro_RT, TR_C1076_S_AP_L_Excitable_L_DataReceived_RT, -1 }; /* RAbeat36_*/
static const TR_PL_ID PE_PossiblyDisabled_96[3]= {TR_I1104_S_AtrRetroReached_L_Retro_RT, TR_C1077_S_AP_L_DataReceived_RT, -1 }; /* D40_L_Refractory*/
static const TR_PL_ID PE_PossiblyDisabled_101[3]= {TR_I1113_S_VtrAnteReached_L_Ante_RT, TR_C1107_S_VP_L_DataReceived_RT, -1 }; /* D248_L_Refractory*/
static const TR_PL_ID PE_PossiblyDisabled_102[6]= {TR_EMPTY_Vbeat_RT, TR_I955_L_q0_RT, TR_I931_L_Wait4VBeat_RT, TR_I1112_S_VtrAnteReached_L_Excitable_L_Ante_RT, TR_C1106_S_VP_L_Excitable_L_DataReceived_RT, -1 }; /* RVbeat249_*/
const TR_PL_ID* SPN::PossiblyDisabled[] = {PE_PossiblyDisabled_0, PE_PossiblyDisabled_1, PE_PossiblyDisabled_2, PE_PossiblyDisabled_3, PE_PossiblyDisabled_4, PE_PossiblyDisabled_5, PE_PossiblyDisabled_6, EMPTY_array, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_10, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_13, EMPTY_array, PE_PossiblyDisabled_15, PE_PossiblyDisabled_16, PE_PossiblyDisabled_17, EMPTY_array, PE_PossiblyDisabled_19, EMPTY_array, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_23, PE_PossiblyDisabled_24, PE_PossiblyDisabled_25, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_28, EMPTY_array, PE_PossiblyDisabled_30, PE_PossiblyDisabled_31, PE_PossiblyDisabled_32, PE_PossiblyDisabled_33, PE_PossiblyDisabled_34, PE_PossiblyDisabled_35, PE_PossiblyDisabled_36, PE_PossiblyDisabled_37, PE_PossiblyDisabled_38, PE_PossiblyDisabled_39, PE_PossiblyDisabled_40, PE_PossiblyDisabled_41, PE_PossiblyDisabled_42, PE_PossiblyDisabled_43, PE_PossiblyDisabled_44, PE_PossiblyDisabled_45, PE_PossiblyDisabled_46, PE_PossiblyDisabled_47, PE_PossiblyDisabled_48, PE_PossiblyDisabled_49, EMPTY_array, PE_PossiblyDisabled_51, PE_PossiblyDisabled_52, PE_PossiblyDisabled_53, EMPTY_array, PE_PossiblyDisabled_55, EMPTY_array, PE_PossiblyDisabled_57, PE_PossiblyDisabled_58, PE_PossiblyDisabled_59, PE_PossiblyDisabled_60, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_63, PE_PossiblyDisabled_64, EMPTY_array, PE_PossiblyDisabled_66, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_71, PE_PossiblyDisabled_72, PE_PossiblyDisabled_73, PE_PossiblyDisabled_74, PE_PossiblyDisabled_75, PE_PossiblyDisabled_76, PE_PossiblyDisabled_77, PE_PossiblyDisabled_78, PE_PossiblyDisabled_79, EMPTY_array, PE_PossiblyDisabled_81, PE_PossiblyDisabled_82, PE_PossiblyDisabled_83, PE_PossiblyDisabled_84, PE_PossiblyDisabled_85, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_88, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_94, EMPTY_array, PE_PossiblyDisabled_96, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, PE_PossiblyDisabled_101, PE_PossiblyDisabled_102, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array};

const TR_PL_ID* SPN::FreeMarkDepT[] = {EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array, EMPTY_array};

SPN::SPN():
pl(21), tr(107) {
}

TR_PL_ID SPN::getIncomingTrans(){ return TR_INCOMING_SREC_RT;};

bool SPN::IsEnabled(TR_PL_ID t)const {
	if(!magicConditional(t))return false;
switch (t){
	case 0:	//EMPTY_Abeat
    if (!(contains(Marking.P->_PL_SIG_Abeat , 1))) return false;
    return true;

		break;
	case 25:	//RNextAtrBeat492_
    if (!(contains(Marking.P->_PL_SIG_NextAtrBeat , 1))) return false;
    if( Marking.P->_PL_SANode != 14) return false;
    return true;

		break;
	case 28:	//RNextAtrBeat89_
    if (!(contains(Marking.P->_PL_SIG_NextAtrBeat , 1))) return false;
    if( Marking.P->_PL_SANode != 88) return false;
    return true;

		break;
	case 1:	//EMPTY_NextAtrBeat
    if (!(contains(Marking.P->_PL_SIG_NextAtrBeat , 1))) return false;
    return true;

		break;
	case 19:	//RNextVtrBeat491_
    if (!(contains(Marking.P->_PL_SIG_NextVtrBeat , 1))) return false;
    if( Marking.P->_PL_VRG != 268) return false;
    return true;

		break;
	case 17:	//RNextVtrBeat267_
    if (!(contains(Marking.P->_PL_SIG_NextVtrBeat , 1))) return false;
    if( Marking.P->_PL_VRG != 269) return false;
    return true;

		break;
	case 2:	//EMPTY_NextVtrBeat
    if (!(contains(Marking.P->_PL_SIG_NextVtrBeat , 1))) return false;
    return true;

		break;
	case 85:	//RSREC866_
    if (!(contains(Marking.P->_PL_SIG_SREC , 1))) return false;
    if( Marking.P->_PL_Parameter_Serial != 870) return false;
    return true;

		break;
	case 3:	//EMPTY_SREC
    if (!(contains(Marking.P->_PL_SIG_SREC , 1))) return false;
    return true;

		break;
	case 102:	//RVbeat249_
    if (!(contains(Marking.P->_PL_SIG_Vbeat , 1))) return false;
    if( Marking.P->_PL_Ventricle != 244) return false;
    return true;

		break;
	case 4:	//EMPTY_Vbeat
    if (!(contains(Marking.P->_PL_SIG_Vbeat , 1))) return false;
    return true;

		break;
	case 53:	//Ra_dVSetHigh656_
    if (!(contains(Marking.P->_PL_SIG_a_dVSetHigh , 1))) return false;
    if( Marking.P->_PL_A_dV != 654) return false;
    return true;

		break;
	case 5:	//EMPTY_a_dVSetHigh
    if (!(contains(Marking.P->_PL_SIG_a_dVSetHigh , 1))) return false;
    return true;

		break;
	case 55:	//Ra_dVSetLow657_
    if (!(contains(Marking.P->_PL_SIG_a_dVSetLow , 1))) return false;
    if( Marking.P->_PL_A_dV != 655) return false;
    return true;

		break;
	case 6:	//EMPTY_a_dVSetLow
    if (!(contains(Marking.P->_PL_SIG_a_dVSetLow , 1))) return false;
    return true;

		break;
	case 13:	//I937_L_q0
    if (Marking.P->_PL_SIG_Abeat >= 1) return false;
    if (Marking.P->_PL_SIG_a_dVSetHigh >= 1) return false;
    if( Marking.P->_PL_SANodeEctopic != 936) return false;
    return true;

		break;
	case 24:	//I925_L_Wait4ABeat
    if (Marking.P->_PL_SIG_Abeat >= 1) return false;
    if (Marking.P->_PL_SIG_a_dVSetLow >= 1) return false;
    if( Marking.P->_PL_SANode != 924) return false;
    return true;

		break;
	case 97:	//I92_L_ASynch
    if (Marking.P->_PL_SIG_NextAtrBeat >= 1) return false;
    if( Marking.P->_PL_Atrium != 218) return false;
    return true;

		break;
	case 104:	//I251_L_VSynch
    if (Marking.P->_PL_SIG_NextVtrBeat >= 1) return false;
    if( Marking.P->_PL_Ventricle != 246) return false;
    return true;

		break;
	case 7:	//INCOMING_SREC
    if (Marking.P->_PL_SIG_SREC >= 0) return false;
    return true;

		break;
	case 16:	//I931_L_Wait4VBeat
    if (Marking.P->_PL_SIG_Vbeat >= 1) return false;
    if( Marking.P->_PL_VRG != 930) return false;
    return true;

		break;
	case 10:	//I955_L_q0
    if (Marking.P->_PL_SIG_Vbeat >= 1) return false;
    if( Marking.P->_PL_VRGEctopic != 954) return false;
    return true;

		break;
	case 44:	//I1098_S_AVJRetroReached_L_Retro
    if( Marking.P->_PL_AVJ != 632) return false;
    if( Marking.P->_PL_RVConductor != 946) return false;
    return true;

		break;
	case 63:	//D651_Ante
    if( Marking.P->_PL_AVJ != 632) return false;
    return true;

		break;
	case 46:	//I1096_S_AVJRetroReached_AVJRetroReached_L_Retro
    if( Marking.P->_PL_AVJ != 633) return false;
    if( Marking.P->_PL_RVConductor != 946) return false;
    return true;

		break;
	case 64:	//D634_L_Refractory
    if( Marking.P->_PL_AVJ != 633) return false;
    return true;

		break;
	case 45:	//I1097_S_AVJRetroReached_L_Recovery_L_Retro
    if( Marking.P->_PL_AVJ != 640) return false;
    if( Marking.P->_PL_RVConductor != 946) return false;
    return true;

		break;
	case 66:	//D636_L_Recovery
    if( Marking.P->_PL_AVJ != 640) return false;
    return true;

		break;
	case 69:	//I913_L_Refractory
    if( Marking.P->_PL_AVJ != 912) return false;
    return true;

		break;
	case 68:	//I915_L_Refractory
    if( Marking.P->_PL_AVJ != 914) return false;
    return true;

		break;
	case 67:	//I917_L_Refractory
    if( Marking.P->_PL_AVJ != 916) return false;
    return true;

		break;
	case 65:	//I919_init
    if( Marking.P->_PL_AVJ != 918) return false;
    return true;

		break;
	case 59:	//I1082_S_AVJRetroExit_L_Retro
    if( Marking.P->_PL_AVJOut != 172) return false;
    if( Marking.P->_PL_AVJ != 625) return false;
    return true;

		break;
	case 60:	//I1081_S_AVJAnteExit_L_Ante
    if( Marking.P->_PL_AVJOut != 172) return false;
    if( Marking.P->_PL_AVJ != 920) return false;
    return true;

		break;
	case 72:	//D179_Retro
    if( Marking.P->_PL_AVJOut != 172) return false;
    return true;

		break;
	case 62:	//I1079_S_AVJRetroExit_L_Empty_L_Retro
    if( Marking.P->_PL_AVJOut != 173) return false;
    if( Marking.P->_PL_AVJ != 625) return false;
    return true;

		break;
	case 61:	//I1080_S_AVJAnteExit_L_Empty_L_Ante
    if( Marking.P->_PL_AVJOut != 173) return false;
    if( Marking.P->_PL_AVJ != 920) return false;
    return true;

		break;
	case 57:	//I1084_S_AVJRetroExit_L_Retro
    if( Marking.P->_PL_AVJOut != 175) return false;
    if( Marking.P->_PL_AVJ != 625) return false;
    return true;

		break;
	case 58:	//I1083_S_AVJAnteExit_L_Ante
    if( Marking.P->_PL_AVJOut != 175) return false;
    if( Marking.P->_PL_AVJ != 920) return false;
    return true;

		break;
	case 71:	//D177_Ante
    if( Marking.P->_PL_AVJOut != 175) return false;
    return true;

		break;
	case 48:	//I1094_S_anteAVJ_L_Ante_L_Empty
    if( Marking.P->_PL_AVJOut != 960) return false;
    if( Marking.P->_PL_RVConductor != 155) return false;
    return true;

		break;
	case 47:	//I1095_S_anteAVJ_L_Ante
    if( Marking.P->_PL_AVJOut != 960) return false;
    if( Marking.P->_PL_RVConductor != 156) return false;
    return true;

		break;
	case 49:	//I1093_S_anteAVJ_L_Ante_L_Retro
    if( Marking.P->_PL_AVJOut != 960) return false;
    if( Marking.P->_PL_RVConductor != 157) return false;
    return true;

		break;
	case 70:	//I963_init
    if( Marking.P->_PL_AVJOut != 962) return false;
    return true;

		break;
	case 56:	//I1135_L_a_dvHigh
    if( Marking.P->_PL_A_dV != 1134) return false;
    return true;

		break;
	case 54:	//I1137_L_a_dVLow
    if( Marking.P->_PL_A_dV != 1136) return false;
    return true;

		break;
	case 89:	//D10_L_q0
    if( Marking.P->_PL_AtrNoiseGenerator != 7) return false;
    return true;

		break;
	case 90:	//I935_init
    if( Marking.P->_PL_AtrNoiseGenerator != 934) return false;
    return true;

		break;
	case 95:	//I1131_L_Excitable
    if( Marking.P->_PL_Atrium != 1130) return false;
    return true;

		break;
	case 99:	//I616_L_Asense
    if( Marking.P->_PL_Atrium != 304) return false;
    return true;

		break;
	case 98:	//I307_L_AsenseRetro
    if( Marking.P->_PL_Atrium != 306) return false;
    return true;

		break;
	case 83:	//C1077_S_AP_L_DataReceived
    if( Marking.P->_PL_Atrium != 34) return false;
    if( Marking.P->_PL_Parameter_Serial != 872) return false;
    return true;

		break;
	case 75:	//I1104_S_AtrRetroReached_L_Retro
    if( Marking.P->_PL_Atrium != 34) return false;
    if( Marking.P->_PL_RAConductor != 942) return false;
    return true;

		break;
	case 96:	//D40_L_Refractory
    if( Marking.P->_PL_Atrium != 34) return false;
    return true;

		break;
	case 84:	//C1076_S_AP_L_Excitable_L_DataReceived
    if( Marking.P->_PL_Atrium != 35) return false;
    if( Marking.P->_PL_Parameter_Serial != 872) return false;
    return true;

		break;
	case 76:	//I1103_S_AtrRetroReached_L_Excitable_L_Retro
    if( Marking.P->_PL_Atrium != 35) return false;
    if( Marking.P->_PL_RAConductor != 942) return false;
    return true;

		break;
	case 77:	//I1102_S_arg_L_GenerateAnteWave
    if( Marking.P->_PL_Atrium != 42) return false;
    if( Marking.P->_PL_RAConductor != 51) return false;
    return true;

		break;
	case 79:	//I1100_S_arg_L_GenerateAnteWave_L_Retro
    if( Marking.P->_PL_Atrium != 42) return false;
    if( Marking.P->_PL_RAConductor != 52) return false;
    return true;

		break;
	case 78:	//I1101_S_arg_L_GenerateAnteWave_L_Empty
    if( Marking.P->_PL_Atrium != 42) return false;
    if( Marking.P->_PL_RAConductor != 60) return false;
    return true;

		break;
	case 93:	//I909_init
    if( Marking.P->_PL_Atrium != 908) return false;
    return true;

		break;
	case 86:	//I1133_L_Init
    if( Marking.P->_PL_Parameter_Serial != 1132) return false;
    return true;

		break;
	case 88:	//C844_L_DataReceived
    if( Marking.P->_PL_Parameter_Serial != 872) return false;
    return true;

		break;
	case 87:	//I907_init
    if( Marking.P->_PL_Parameter_Serial != 906) return false;
    return true;

		break;
	case 31:	//I1121_S_retroAVJ_L_Ante_L_Retro
    if( Marking.P->_PL_RAConductor != 51) return false;
    if( Marking.P->_PL_AVJOut != 958) return false;
    return true;

		break;
	case 82:	//D56_Ante
    if( Marking.P->_PL_RAConductor != 51) return false;
    return true;

		break;
	case 30:	//I1122_S_retroAVJ_L_Retro
    if( Marking.P->_PL_RAConductor != 52) return false;
    if( Marking.P->_PL_AVJOut != 958) return false;
    return true;

		break;
	case 81:	//D58_Retro
    if( Marking.P->_PL_RAConductor != 52) return false;
    return true;

		break;
	case 32:	//I1120_S_retroAVJ_L_Empty_L_Retro
    if( Marking.P->_PL_RAConductor != 60) return false;
    if( Marking.P->_PL_AVJOut != 958) return false;
    return true;

		break;
	case 34:	//I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 632) return false;
    if( Marking.P->_PL_A_dV != 654) return false;
    return true;

		break;
	case 33:	//I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 632) return false;
    if( Marking.P->_PL_A_dV != 655) return false;
    return true;

		break;
	case 38:	//I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 633) return false;
    if( Marking.P->_PL_A_dV != 654) return false;
    return true;

		break;
	case 37:	//I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 633) return false;
    if( Marking.P->_PL_A_dV != 655) return false;
    return true;

		break;
	case 35:	//I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 640) return false;
    if( Marking.P->_PL_A_dV != 654) return false;
    return true;

		break;
	case 36:	//I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh
    if( Marking.P->_PL_RAConductor != 940) return false;
    if( Marking.P->_PL_AVJ != 640) return false;
    if( Marking.P->_PL_A_dV != 655) return false;
    return true;

		break;
	case 80:	//I945_init
    if( Marking.P->_PL_RAConductor != 944) return false;
    return true;

		break;
	case 51:	//D160_Ante
    if( Marking.P->_PL_RVConductor != 156) return false;
    return true;

		break;
	case 52:	//D161_Retro
    if( Marking.P->_PL_RVConductor != 157) return false;
    return true;

		break;
	case 50:	//I951_init
    if( Marking.P->_PL_RVConductor != 950) return false;
    return true;

		break;
	case 29:	//I1139_L_Wait4ASynch
    if( Marking.P->_PL_SANode != 1138) return false;
    return true;

		break;
	case 26:	//I1141_NextAtrBeat
    if( Marking.P->_PL_SANode != 1140) return false;
    return true;

		break;
	case 23:	//D16_Wait4ABeat
    if( Marking.P->_PL_SANode != 14) return false;
    return true;

		break;
	case 27:	//I923_L_Wait4ABeat
    if( Marking.P->_PL_SANode != 922) return false;
    return true;

		break;
	case 22:	//I927_init
    if( Marking.P->_PL_SANode != 926) return false;
    return true;

		break;
	case 12:	//D86_q0
    if( Marking.P->_PL_SANodeEctopic != 85) return false;
    return true;

		break;
	case 11:	//I939_init
    if( Marking.P->_PL_SANodeEctopic != 938) return false;
    return true;

		break;
	case 20:	//I1143_NextVtrBeat
    if( Marking.P->_PL_VRG != 1142) return false;
    return true;

		break;
	case 18:	//I1145_L_Wait4VSynch
    if( Marking.P->_PL_VRG != 1144) return false;
    return true;

		break;
	case 15:	//D266_Wait4VBeat
    if( Marking.P->_PL_VRG != 268) return false;
    return true;

		break;
	case 21:	//I929_L_Wait4VBeat
    if( Marking.P->_PL_VRG != 928) return false;
    return true;

		break;
	case 14:	//I933_init
    if( Marking.P->_PL_VRG != 932) return false;
    return true;

		break;
	case 9:	//D279_q0
    if( Marking.P->_PL_VRGEctopic != 277) return false;
    return true;

		break;
	case 8:	//I957_init
    if( Marking.P->_PL_VRGEctopic != 956) return false;
    return true;

		break;
	case 103:	//I1129_L_Excitable
    if( Marking.P->_PL_Ventricle != 1128) return false;
    return true;

		break;
	case 74:	//C1106_S_VP_L_Excitable_L_DataReceived
    if( Marking.P->_PL_Ventricle != 244) return false;
    if( Marking.P->_PL_Parameter_Serial != 872) return false;
    return true;

		break;
	case 40:	//I1112_S_VtrAnteReached_L_Excitable_L_Ante
    if( Marking.P->_PL_Ventricle != 244) return false;
    if( Marking.P->_PL_RVConductor != 948) return false;
    return true;

		break;
	case 73:	//C1107_S_VP_L_DataReceived
    if( Marking.P->_PL_Ventricle != 245) return false;
    if( Marking.P->_PL_Parameter_Serial != 872) return false;
    return true;

		break;
	case 39:	//I1113_S_VtrAnteReached_L_Ante
    if( Marking.P->_PL_Ventricle != 245) return false;
    if( Marking.P->_PL_RVConductor != 948) return false;
    return true;

		break;
	case 101:	//D248_L_Refractory
    if( Marking.P->_PL_Ventricle != 245) return false;
    return true;

		break;
	case 42:	//I1110_S_vrg_L_GenerateRetroWave_L_Empty
    if( Marking.P->_PL_Ventricle != 247) return false;
    if( Marking.P->_PL_RVConductor != 155) return false;
    return true;

		break;
	case 43:	//I1109_S_vrg_L_GenerateRetroWave_L_Ante
    if( Marking.P->_PL_Ventricle != 247) return false;
    if( Marking.P->_PL_RVConductor != 156) return false;
    return true;

		break;
	case 41:	//I1111_S_vrg_L_GenerateRetroWave
    if( Marking.P->_PL_Ventricle != 247) return false;
    if( Marking.P->_PL_RVConductor != 157) return false;
    return true;

		break;
	case 106:	//I309_L_Vsense
    if( Marking.P->_PL_Ventricle != 308) return false;
    return true;

		break;
	case 105:	//I311_L_VsenseAnte
    if( Marking.P->_PL_Ventricle != 310) return false;
    return true;

		break;
	case 100:	//I911_init
    if( Marking.P->_PL_Ventricle != 910) return false;
    return true;

		break;
	case 92:	//D276_L_q0
    if( Marking.P->_PL_VtrNoiseGenerator != 274) return false;
    return true;

		break;
	case 91:	//I953_init
    if( Marking.P->_PL_VtrNoiseGenerator != 952) return false;
    return true;

		break;
	default:	//RAbeat36_,
    if (!(contains(Marking.P->_PL_SIG_Abeat , 1))) return false;
    if( Marking.P->_PL_Atrium != 35) return false;
    return true;

}
}

void SPN::fire(TR_PL_ID t,  REAL_TYPE time){
	lastTransition = t;
	magicUpdate(t,time);
	switch(t){
		case 0: {  //EMPTY_Abeat
			Marking.P->_PL_SIG_Abeat -= 1;
       break;
     } 
		case 1: {  //EMPTY_NextAtrBeat
			Marking.P->_PL_SIG_NextAtrBeat -= 1;
       break;
     } 
		case 2: {  //EMPTY_NextVtrBeat
			Marking.P->_PL_SIG_NextVtrBeat -= 1;
       break;
     } 
		case 3: {  //EMPTY_SREC
			Marking.P->_PL_SIG_SREC -= 1;
       break;
     } 
		case 4: {  //EMPTY_Vbeat
			Marking.P->_PL_SIG_Vbeat -= 1;
       break;
     } 
		case 5: {  //EMPTY_a_dVSetHigh
			Marking.P->_PL_SIG_a_dVSetHigh -= 1;
       break;
     } 
		case 6: {  //EMPTY_a_dVSetLow
			Marking.P->_PL_SIG_a_dVSetLow -= 1;
       break;
     } 
		case 7: {  //INCOMING_SREC
			Marking.P->_PL_SIG_SREC += 1;
       break;
     } 
		case 8: {  //I957_init
			Marking.P->_PL_VRGEctopic -= 956;
			Marking.P->_PL_VRGEctopic += 277;
       break;
     } 
		case 9: {  //D279_q0
			Marking.P->_PL_VRGEctopic -= 277;
			Marking.P->_PL_VRGEctopic += 954;
       break;
     } 
		case 10: {  //I955_L_q0
			Marking.P->_PL_SIG_Vbeat += 1;
			Marking.P->_PL_VRGEctopic -= 954;
			Marking.P->_PL_VRGEctopic += 277;
       break;
     } 
		case 11: {  //I939_init
			Marking.P->_PL_SANodeEctopic -= 938;
			Marking.P->_PL_SANodeEctopic += 85;
       break;
     } 
		case 12: {  //D86_q0
			Marking.P->_PL_SANodeEctopic -= 85;
			Marking.P->_PL_SANodeEctopic += 936;
       break;
     } 
		case 13: {  //I937_L_q0
			Marking.P->_PL_SIG_Abeat += 1;
			Marking.P->_PL_SIG_a_dVSetHigh += 1;
			Marking.P->_PL_SANodeEctopic -= 936;
			Marking.P->_PL_SANodeEctopic += 85;
       break;
     } 
		case 14: {  //I933_init
			Marking.P->_PL_VRG -= 932;
			Marking.P->_PL_VRG += 268;
       break;
     } 
		case 15: {  //D266_Wait4VBeat
			Marking.P->_PL_VRG -= 268;
			Marking.P->_PL_VRG += 930;
       break;
     } 
		case 16: {  //I931_L_Wait4VBeat
			Marking.P->_PL_SIG_Vbeat += 1;
			Marking.P->_PL_VRG -= 930;
			Marking.P->_PL_VRG += 269;
       break;
     } 
		case 17: {  //RNextVtrBeat267_
			Marking.P->_PL_SIG_NextVtrBeat -= 1;
			Marking.P->_PL_SIG_NextVtrBeat += 1;
			Marking.P->_PL_VRG -= 269;
			Marking.P->_PL_VRG += 1144;
       break;
     } 
		case 18: {  //I1145_L_Wait4VSynch
			Marking.P->_PL_VRG -= 1144;
			Marking.P->_PL_VRG += 268;
       break;
     } 
		case 19: {  //RNextVtrBeat491_
			Marking.P->_PL_SIG_NextVtrBeat -= 1;
			Marking.P->_PL_SIG_NextVtrBeat += 1;
			Marking.P->_PL_VRG -= 268;
			Marking.P->_PL_VRG += 1142;
       break;
     } 
		case 20: {  //I1143_NextVtrBeat
			Marking.P->_PL_VRG -= 1142;
			Marking.P->_PL_VRG += 928;
       break;
     } 
		case 21: {  //I929_L_Wait4VBeat
			Marking.P->_PL_VRG -= 928;
			Marking.P->_PL_VRG += 268;
       break;
     } 
		case 22: {  //I927_init
			Marking.P->_PL_SANode -= 926;
			Marking.P->_PL_SANode += 14;
       break;
     } 
		case 23: {  //D16_Wait4ABeat
			Marking.P->_PL_SANode -= 14;
			Marking.P->_PL_SANode += 924;
       break;
     } 
		case 24: {  //I925_L_Wait4ABeat
			Marking.P->_PL_SIG_Abeat += 1;
			Marking.P->_PL_SIG_a_dVSetLow += 1;
			Marking.P->_PL_SANode -= 924;
			Marking.P->_PL_SANode += 88;
       break;
     } 
		case 25: {  //RNextAtrBeat492_
			Marking.P->_PL_SIG_NextAtrBeat -= 1;
			Marking.P->_PL_SIG_NextAtrBeat += 1;
			Marking.P->_PL_SANode -= 14;
			Marking.P->_PL_SANode += 1140;
       break;
     } 
		case 26: {  //I1141_NextAtrBeat
			Marking.P->_PL_SANode -= 1140;
			Marking.P->_PL_SANode += 922;
       break;
     } 
		case 27: {  //I923_L_Wait4ABeat
			Marking.P->_PL_SANode -= 922;
			Marking.P->_PL_SANode += 14;
       break;
     } 
		case 28: {  //RNextAtrBeat89_
			Marking.P->_PL_SIG_NextAtrBeat -= 1;
			Marking.P->_PL_SIG_NextAtrBeat += 1;
			Marking.P->_PL_SANode -= 88;
			Marking.P->_PL_SANode += 1138;
       break;
     } 
		case 29: {  //I1139_L_Wait4ASynch
			Marking.P->_PL_SANode -= 1138;
			Marking.P->_PL_SANode += 14;
       break;
     } 
		case 30: {  //I1122_S_retroAVJ_L_Retro
			Marking.P->_PL_RAConductor -= 52;
			Marking.P->_PL_RAConductor += 52;
			Marking.P->_PL_AVJOut -= 958;
			Marking.P->_PL_AVJOut += 173;
       break;
     } 
		case 31: {  //I1121_S_retroAVJ_L_Ante_L_Retro
			Marking.P->_PL_RAConductor -= 51;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJOut -= 958;
			Marking.P->_PL_AVJOut += 173;
       break;
     } 
		case 32: {  //I1120_S_retroAVJ_L_Empty_L_Retro
			Marking.P->_PL_RAConductor -= 60;
			Marking.P->_PL_RAConductor += 52;
			Marking.P->_PL_AVJOut -= 958;
			Marking.P->_PL_AVJOut += 173;
       break;
     } 
		case 33: {  //I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 632;
			Marking.P->_PL_AVJ += 632;
			Marking.P->_PL_A_dV -= 655;
			Marking.P->_PL_A_dV += 655;
       break;
     } 
		case 34: {  //I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 632;
			Marking.P->_PL_AVJ += 632;
			Marking.P->_PL_A_dV -= 654;
			Marking.P->_PL_A_dV += 654;
       break;
     } 
		case 35: {  //I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 640;
			Marking.P->_PL_AVJ += 632;
			Marking.P->_PL_A_dV -= 654;
			Marking.P->_PL_A_dV += 654;
       break;
     } 
		case 36: {  //I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 640;
			Marking.P->_PL_AVJ += 632;
			Marking.P->_PL_A_dV -= 655;
			Marking.P->_PL_A_dV += 655;
       break;
     } 
		case 37: {  //I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 633;
			Marking.P->_PL_AVJ += 916;
			Marking.P->_PL_A_dV -= 655;
			Marking.P->_PL_A_dV += 655;
       break;
     } 
		case 38: {  //I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow
			Marking.P->_PL_RAConductor -= 940;
			Marking.P->_PL_RAConductor += 60;
			Marking.P->_PL_AVJ -= 633;
			Marking.P->_PL_AVJ += 914;
			Marking.P->_PL_A_dV -= 654;
			Marking.P->_PL_A_dV += 654;
       break;
     } 
		case 39: {  //I1113_S_VtrAnteReached_L_Ante
			Marking.P->_PL_Ventricle -= 245;
			Marking.P->_PL_Ventricle += 245;
			Marking.P->_PL_RVConductor -= 948;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 40: {  //I1112_S_VtrAnteReached_L_Excitable_L_Ante
			Marking.P->_PL_Ventricle -= 244;
			Marking.P->_PL_Ventricle += 310;
			Marking.P->_PL_RVConductor -= 948;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 41: {  //I1111_S_vrg_L_GenerateRetroWave
			Marking.P->_PL_Ventricle -= 247;
			Marking.P->_PL_Ventricle += 246;
			Marking.P->_PL_RVConductor -= 157;
			Marking.P->_PL_RVConductor += 157;
       break;
     } 
		case 42: {  //I1110_S_vrg_L_GenerateRetroWave_L_Empty
			Marking.P->_PL_Ventricle -= 247;
			Marking.P->_PL_Ventricle += 246;
			Marking.P->_PL_RVConductor -= 155;
			Marking.P->_PL_RVConductor += 157;
       break;
     } 
		case 43: {  //I1109_S_vrg_L_GenerateRetroWave_L_Ante
			Marking.P->_PL_Ventricle -= 247;
			Marking.P->_PL_Ventricle += 246;
			Marking.P->_PL_RVConductor -= 156;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 44: {  //I1098_S_AVJRetroReached_L_Retro
			Marking.P->_PL_AVJ -= 632;
			Marking.P->_PL_AVJ += 632;
			Marking.P->_PL_RVConductor -= 946;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 45: {  //I1097_S_AVJRetroReached_L_Recovery_L_Retro
			Marking.P->_PL_AVJ -= 640;
			Marking.P->_PL_AVJ += 625;
			Marking.P->_PL_RVConductor -= 946;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 46: {  //I1096_S_AVJRetroReached_AVJRetroReached_L_Retro
			Marking.P->_PL_AVJ -= 633;
			Marking.P->_PL_AVJ += 912;
			Marking.P->_PL_RVConductor -= 946;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 47: {  //I1095_S_anteAVJ_L_Ante
			Marking.P->_PL_AVJOut -= 960;
			Marking.P->_PL_AVJOut += 173;
			Marking.P->_PL_RVConductor -= 156;
			Marking.P->_PL_RVConductor += 156;
       break;
     } 
		case 48: {  //I1094_S_anteAVJ_L_Ante_L_Empty
			Marking.P->_PL_AVJOut -= 960;
			Marking.P->_PL_AVJOut += 173;
			Marking.P->_PL_RVConductor -= 155;
			Marking.P->_PL_RVConductor += 156;
       break;
     } 
		case 49: {  //I1093_S_anteAVJ_L_Ante_L_Retro
			Marking.P->_PL_AVJOut -= 960;
			Marking.P->_PL_AVJOut += 173;
			Marking.P->_PL_RVConductor -= 157;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 50: {  //I951_init
			Marking.P->_PL_RVConductor -= 950;
			Marking.P->_PL_RVConductor += 155;
       break;
     } 
		case 51: {  //D160_Ante
			Marking.P->_PL_RVConductor -= 156;
			Marking.P->_PL_RVConductor += 948;
       break;
     } 
		case 52: {  //D161_Retro
			Marking.P->_PL_RVConductor -= 157;
			Marking.P->_PL_RVConductor += 946;
       break;
     } 
		case 53: {  //Ra_dVSetHigh656_
			Marking.P->_PL_SIG_a_dVSetHigh -= 1;
			Marking.P->_PL_SIG_a_dVSetHigh += 1;
			Marking.P->_PL_A_dV -= 654;
			Marking.P->_PL_A_dV += 1136;
       break;
     } 
		case 54: {  //I1137_L_a_dVLow
			Marking.P->_PL_A_dV -= 1136;
			Marking.P->_PL_A_dV += 655;
       break;
     } 
		case 55: {  //Ra_dVSetLow657_
			Marking.P->_PL_SIG_a_dVSetLow -= 1;
			Marking.P->_PL_SIG_a_dVSetLow += 1;
			Marking.P->_PL_A_dV -= 655;
			Marking.P->_PL_A_dV += 1134;
       break;
     } 
		case 56: {  //I1135_L_a_dvHigh
			Marking.P->_PL_A_dV -= 1134;
			Marking.P->_PL_A_dV += 654;
       break;
     } 
		case 57: {  //I1084_S_AVJRetroExit_L_Retro
			Marking.P->_PL_AVJOut -= 175;
			Marking.P->_PL_AVJOut += 175;
			Marking.P->_PL_AVJ -= 625;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 58: {  //I1083_S_AVJAnteExit_L_Ante
			Marking.P->_PL_AVJOut -= 175;
			Marking.P->_PL_AVJOut += 175;
			Marking.P->_PL_AVJ -= 920;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 59: {  //I1082_S_AVJRetroExit_L_Retro
			Marking.P->_PL_AVJOut -= 172;
			Marking.P->_PL_AVJOut += 172;
			Marking.P->_PL_AVJ -= 625;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 60: {  //I1081_S_AVJAnteExit_L_Ante
			Marking.P->_PL_AVJOut -= 172;
			Marking.P->_PL_AVJOut += 172;
			Marking.P->_PL_AVJ -= 920;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 61: {  //I1080_S_AVJAnteExit_L_Empty_L_Ante
			Marking.P->_PL_AVJOut -= 173;
			Marking.P->_PL_AVJOut += 175;
			Marking.P->_PL_AVJ -= 920;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 62: {  //I1079_S_AVJRetroExit_L_Empty_L_Retro
			Marking.P->_PL_AVJOut -= 173;
			Marking.P->_PL_AVJOut += 172;
			Marking.P->_PL_AVJ -= 625;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 63: {  //D651_Ante
			Marking.P->_PL_AVJ -= 632;
			Marking.P->_PL_AVJ += 920;
       break;
     } 
		case 64: {  //D634_L_Refractory
			Marking.P->_PL_AVJ -= 633;
			Marking.P->_PL_AVJ += 640;
       break;
     } 
		case 65: {  //I919_init
			Marking.P->_PL_AVJ -= 918;
			Marking.P->_PL_AVJ += 640;
       break;
     } 
		case 66: {  //D636_L_Recovery
			Marking.P->_PL_AVJ -= 640;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 67: {  //I917_L_Refractory
			Marking.P->_PL_AVJ -= 916;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 68: {  //I915_L_Refractory
			Marking.P->_PL_AVJ -= 914;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 69: {  //I913_L_Refractory
			Marking.P->_PL_AVJ -= 912;
			Marking.P->_PL_AVJ += 633;
       break;
     } 
		case 70: {  //I963_init
			Marking.P->_PL_AVJOut -= 962;
			Marking.P->_PL_AVJOut += 173;
       break;
     } 
		case 71: {  //D177_Ante
			Marking.P->_PL_AVJOut -= 175;
			Marking.P->_PL_AVJOut += 960;
       break;
     } 
		case 72: {  //D179_Retro
			Marking.P->_PL_AVJOut -= 172;
			Marking.P->_PL_AVJOut += 958;
       break;
     } 
		case 73: {  //C1107_S_VP_L_DataReceived
			Marking.P->_PL_Ventricle -= 245;
			Marking.P->_PL_Ventricle += 245;
			Marking.P->_PL_Parameter_Serial -= 872;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 74: {  //C1106_S_VP_L_Excitable_L_DataReceived
			Marking.P->_PL_Ventricle -= 244;
			Marking.P->_PL_Ventricle += 247;
			Marking.P->_PL_Parameter_Serial -= 872;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 75: {  //I1104_S_AtrRetroReached_L_Retro
			Marking.P->_PL_Atrium -= 34;
			Marking.P->_PL_Atrium += 34;
			Marking.P->_PL_RAConductor -= 942;
			Marking.P->_PL_RAConductor += 60;
       break;
     } 
		case 76: {  //I1103_S_AtrRetroReached_L_Excitable_L_Retro
			Marking.P->_PL_Atrium -= 35;
			Marking.P->_PL_Atrium += 306;
			Marking.P->_PL_RAConductor -= 942;
			Marking.P->_PL_RAConductor += 60;
       break;
     } 
		case 77: {  //I1102_S_arg_L_GenerateAnteWave
			Marking.P->_PL_Atrium -= 42;
			Marking.P->_PL_Atrium += 218;
			Marking.P->_PL_RAConductor -= 51;
			Marking.P->_PL_RAConductor += 51;
       break;
     } 
		case 78: {  //I1101_S_arg_L_GenerateAnteWave_L_Empty
			Marking.P->_PL_Atrium -= 42;
			Marking.P->_PL_Atrium += 218;
			Marking.P->_PL_RAConductor -= 60;
			Marking.P->_PL_RAConductor += 51;
       break;
     } 
		case 79: {  //I1100_S_arg_L_GenerateAnteWave_L_Retro
			Marking.P->_PL_Atrium -= 42;
			Marking.P->_PL_Atrium += 218;
			Marking.P->_PL_RAConductor -= 52;
			Marking.P->_PL_RAConductor += 60;
       break;
     } 
		case 80: {  //I945_init
			Marking.P->_PL_RAConductor -= 944;
			Marking.P->_PL_RAConductor += 60;
       break;
     } 
		case 81: {  //D58_Retro
			Marking.P->_PL_RAConductor -= 52;
			Marking.P->_PL_RAConductor += 942;
       break;
     } 
		case 82: {  //D56_Ante
			Marking.P->_PL_RAConductor -= 51;
			Marking.P->_PL_RAConductor += 940;
       break;
     } 
		case 83: {  //C1077_S_AP_L_DataReceived
			Marking.P->_PL_Atrium -= 34;
			Marking.P->_PL_Atrium += 34;
			Marking.P->_PL_Parameter_Serial -= 872;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 84: {  //C1076_S_AP_L_Excitable_L_DataReceived
			Marking.P->_PL_Atrium -= 35;
			Marking.P->_PL_Atrium += 42;
			Marking.P->_PL_Parameter_Serial -= 872;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 85: {  //RSREC866_
			Marking.P->_PL_SIG_SREC -= 1;
			Marking.P->_PL_SIG_SREC += 1;
			Marking.P->_PL_Parameter_Serial -= 870;
			Marking.P->_PL_Parameter_Serial += 1132;
       break;
     } 
		case 86: {  //I1133_L_Init
			Marking.P->_PL_Parameter_Serial -= 1132;
			Marking.P->_PL_Parameter_Serial += 872;
       break;
     } 
		case 87: {  //I907_init
			Marking.P->_PL_Parameter_Serial -= 906;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 88: {  //C844_L_DataReceived
			Marking.P->_PL_Parameter_Serial -= 872;
			Marking.P->_PL_Parameter_Serial += 870;
       break;
     } 
		case 89: {  //D10_L_q0
			Marking.P->_PL_AtrNoiseGenerator -= 7;
			Marking.P->_PL_AtrNoiseGenerator += 7;
       break;
     } 
		case 90: {  //I935_init
			Marking.P->_PL_AtrNoiseGenerator -= 934;
			Marking.P->_PL_AtrNoiseGenerator += 7;
       break;
     } 
		case 91: {  //I953_init
			Marking.P->_PL_VtrNoiseGenerator -= 952;
			Marking.P->_PL_VtrNoiseGenerator += 274;
       break;
     } 
		case 92: {  //D276_L_q0
			Marking.P->_PL_VtrNoiseGenerator -= 274;
			Marking.P->_PL_VtrNoiseGenerator += 274;
       break;
     } 
		case 93: {  //I909_init
			Marking.P->_PL_Atrium -= 908;
			Marking.P->_PL_Atrium += 34;
       break;
     } 
		case 94: {  //RAbeat36_
			Marking.P->_PL_SIG_Abeat -= 1;
			Marking.P->_PL_SIG_Abeat += 1;
			Marking.P->_PL_Atrium -= 35;
			Marking.P->_PL_Atrium += 1130;
       break;
     } 
		case 95: {  //I1131_L_Excitable
			Marking.P->_PL_Atrium -= 1130;
			Marking.P->_PL_Atrium += 304;
       break;
     } 
		case 96: {  //D40_L_Refractory
			Marking.P->_PL_Atrium -= 34;
			Marking.P->_PL_Atrium += 35;
       break;
     } 
		case 97: {  //I92_L_ASynch
			Marking.P->_PL_SIG_NextAtrBeat += 1;
			Marking.P->_PL_Atrium -= 218;
			Marking.P->_PL_Atrium += 34;
       break;
     } 
		case 98: {  //I307_L_AsenseRetro
			Marking.P->_PL_Atrium -= 306;
			Marking.P->_PL_Atrium += 218;
       break;
     } 
		case 99: {  //I616_L_Asense
			Marking.P->_PL_Atrium -= 304;
			Marking.P->_PL_Atrium += 42;
       break;
     } 
		case 100: {  //I911_init
			Marking.P->_PL_Ventricle -= 910;
			Marking.P->_PL_Ventricle += 245;
       break;
     } 
		case 101: {  //D248_L_Refractory
			Marking.P->_PL_Ventricle -= 245;
			Marking.P->_PL_Ventricle += 244;
       break;
     } 
		case 102: {  //RVbeat249_
			Marking.P->_PL_SIG_Vbeat -= 1;
			Marking.P->_PL_SIG_Vbeat += 1;
			Marking.P->_PL_Ventricle -= 244;
			Marking.P->_PL_Ventricle += 1128;
       break;
     } 
		case 103: {  //I1129_L_Excitable
			Marking.P->_PL_Ventricle -= 1128;
			Marking.P->_PL_Ventricle += 308;
       break;
     } 
		case 104: {  //I251_L_VSynch
			Marking.P->_PL_SIG_NextVtrBeat += 1;
			Marking.P->_PL_Ventricle -= 246;
			Marking.P->_PL_Ventricle += 245;
       break;
     } 
		case 105: {  //I311_L_VsenseAnte
			Marking.P->_PL_Ventricle -= 310;
			Marking.P->_PL_Ventricle += 246;
       break;
     } 
		case 106: {  //I309_L_Vsense
			Marking.P->_PL_Ventricle -= 308;
			Marking.P->_PL_Ventricle += 247;
       break;
     } 
	}
}
void SPN::setConditionsVector(){
}
void SPN::GetDistParameters(TR_PL_ID t )const {
switch (t){
	case 71:	//D177_Ante
	{
		ParamDistr[0]= ( double ) (AVJDelay (avj_tRec  , AV_anteDMin ));
	}

		break;
	case 72:	//D179_Retro
	{
		ParamDistr[0]= ( double ) (AVJDelay (avj_tRec  , AV_retroDMin ));
	}

		break;
	case 96:	//D40_L_Refractory
	{
		ParamDistr[0]= ( double ) Atr_refrD ;
	}

		break;
	case 82:	//D56_Ante
	{
		ParamDistr[0]= ( double ) RA_anteD ;
	}

		break;
	case 81:	//D58_Retro
	{
		ParamDistr[0]= ( double ) RA_retroD ;
	}

		break;
	case 51:	//D160_Ante
	{
		ParamDistr[0]= ( double ) RV_anteD ;
	}

		break;
	case 52:	//D161_Retro
	{
		ParamDistr[0]= ( double ) RV_retroD ;
	}

		break;
	case 23:	//D16_Wait4ABeat
	{
		ParamDistr[0]= ( double ) SA_d ;
	}

		break;
	case 12:	//D86_q0
	{
		ParamDistr[0]= ( double ) SA_ectopD ;
	}

		break;
	case 89:	//D10_L_q0
	{
		ParamDistr[0]= ( double ) SA_noiseD ;
	}

		break;
	case 15:	//D266_Wait4VBeat
	{
		ParamDistr[0]= ( double ) VRG_d ;
	}

		break;
	case 9:	//D279_q0
	{
		ParamDistr[0]= ( double ) VRG_ectopD ;
	}

		break;
	case 92:	//D276_L_q0
	{
		ParamDistr[0]= ( double ) VRG_noiseD ;
	}

		break;
	case 101:	//D248_L_Refractory
	{
		ParamDistr[0]= ( double ) Vtr_refrD ;
	}

		break;
	case 64:	//D634_L_Refractory
	{
		ParamDistr[0]= ( double ) avj_refr ;
	}

		break;
	case 63:	//D651_Ante
	case 66:	//D636_L_Recovery
	{
		ParamDistr[0]= ( double ) avj_t4 ;
	}

		break;
	default:	//EMPTY_Abeat,EMPTY_NextAtrBeat,EMPTY_NextVtrBeat,EMPTY_SREC,EMPTY_Vbeat,EMPTY_a_dVSetHigh,EMPTY_a_dVSetLow,INCOMING_SREC,I957_init,I955_L_q0,I939_init,I937_L_q0,I933_init,I931_L_Wait4VBeat,RNextVtrBeat267_,I1145_L_Wait4VSynch,RNextVtrBeat491_,I1143_NextVtrBeat,I929_L_Wait4VBeat,I927_init,I925_L_Wait4ABeat,RNextAtrBeat492_,I1141_NextAtrBeat,I923_L_Wait4ABeat,RNextAtrBeat89_,I1139_L_Wait4ASynch,I1122_S_retroAVJ_L_Retro,I1121_S_retroAVJ_L_Ante_L_Retro,I1120_S_retroAVJ_L_Empty_L_Retro,I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh,I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow,I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow,I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh,I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh,I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow,I1113_S_VtrAnteReached_L_Ante,I1112_S_VtrAnteReached_L_Excitable_L_Ante,I1111_S_vrg_L_GenerateRetroWave,I1110_S_vrg_L_GenerateRetroWave_L_Empty,I1109_S_vrg_L_GenerateRetroWave_L_Ante,I1098_S_AVJRetroReached_L_Retro,I1097_S_AVJRetroReached_L_Recovery_L_Retro,I1096_S_AVJRetroReached_AVJRetroReached_L_Retro,I1095_S_anteAVJ_L_Ante,I1094_S_anteAVJ_L_Ante_L_Empty,I1093_S_anteAVJ_L_Ante_L_Retro,I951_init,Ra_dVSetHigh656_,I1137_L_a_dVLow,Ra_dVSetLow657_,I1135_L_a_dvHigh,I1084_S_AVJRetroExit_L_Retro,I1083_S_AVJAnteExit_L_Ante,I1082_S_AVJRetroExit_L_Retro,I1081_S_AVJAnteExit_L_Ante,I1080_S_AVJAnteExit_L_Empty_L_Ante,I1079_S_AVJRetroExit_L_Empty_L_Retro,I919_init,I917_L_Refractory,I915_L_Refractory,I913_L_Refractory,I963_init,C1107_S_VP_L_DataReceived,C1106_S_VP_L_Excitable_L_DataReceived,I1104_S_AtrRetroReached_L_Retro,I1103_S_AtrRetroReached_L_Excitable_L_Retro,I1102_S_arg_L_GenerateAnteWave,I1101_S_arg_L_GenerateAnteWave_L_Empty,I1100_S_arg_L_GenerateAnteWave_L_Retro,I945_init,C1077_S_AP_L_DataReceived,C1076_S_AP_L_Excitable_L_DataReceived,RSREC866_,I1133_L_Init,I907_init,C844_L_DataReceived,I935_init,I953_init,I909_init,RAbeat36_,I1131_L_Excitable,I92_L_ASynch,I307_L_AsenseRetro,I616_L_Asense,I911_init,RVbeat249_,I1129_L_Excitable,I251_L_VSynch,I311_L_VsenseAnte,I309_L_Vsense,
	{
		ParamDistr[0]= ( double ) 0 ;
	}

}
}

REAL_TYPE SPN::GetPriority(TR_PL_ID t)const {
switch (t){
	case 63:	//D651_Ante
	case 64:	//D634_L_Refractory
	case 66:	//D636_L_Recovery
		return (double)0.88 ;

		break;
	case 15:	//D266_Wait4VBeat
		return (double)0.89 ;

		break;
	case 9:	//D279_q0
		return (double)0.9 ;

		break;
	case 96:	//D40_L_Refractory
		return (double)0.91 ;

		break;
	case 101:	//D248_L_Refractory
		return (double)0.92 ;

		break;
	case 23:	//D16_Wait4ABeat
		return (double)0.93 ;

		break;
	case 81:	//D58_Retro
	case 82:	//D56_Ante
		return (double)0.94 ;

		break;
	case 51:	//D160_Ante
	case 52:	//D161_Retro
		return (double)0.95 ;

		break;
	case 71:	//D177_Ante
	case 72:	//D179_Retro
		return (double)0.96 ;

		break;
	case 92:	//D276_L_q0
		return (double)0.97 ;

		break;
	case 12:	//D86_q0
		return (double)0.98 ;

		break;
	case 89:	//D10_L_q0
		return (double)0.99 ;

		break;
	case 73:	//C1107_S_VP_L_DataReceived
	case 74:	//C1106_S_VP_L_Excitable_L_DataReceived
	case 83:	//C1077_S_AP_L_DataReceived
	case 84:	//C1076_S_AP_L_Excitable_L_DataReceived
	case 86:	//I1133_L_Init
	case 87:	//I907_init
	case 88:	//C844_L_DataReceived
		return (double)1.86 ;

		break;
	case 54:	//I1137_L_a_dVLow
	case 56:	//I1135_L_a_dvHigh
		return (double)1.87 ;

		break;
	case 14:	//I933_init
	case 16:	//I931_L_Wait4VBeat
	case 18:	//I1145_L_Wait4VSynch
	case 20:	//I1143_NextVtrBeat
	case 21:	//I929_L_Wait4VBeat
		return (double)1.89 ;

		break;
	case 8:	//I957_init
	case 10:	//I955_L_q0
		return (double)1.9 ;

		break;
	case 77:	//I1102_S_arg_L_GenerateAnteWave
	case 78:	//I1101_S_arg_L_GenerateAnteWave_L_Empty
	case 79:	//I1100_S_arg_L_GenerateAnteWave_L_Retro
	case 93:	//I909_init
	case 95:	//I1131_L_Excitable
	case 97:	//I92_L_ASynch
	case 98:	//I307_L_AsenseRetro
	case 99:	//I616_L_Asense
		return (double)1.91 ;

		break;
	case 41:	//I1111_S_vrg_L_GenerateRetroWave
	case 42:	//I1110_S_vrg_L_GenerateRetroWave_L_Empty
	case 43:	//I1109_S_vrg_L_GenerateRetroWave_L_Ante
	case 100:	//I911_init
	case 103:	//I1129_L_Excitable
	case 104:	//I251_L_VSynch
	case 105:	//I311_L_VsenseAnte
	case 106:	//I309_L_Vsense
		return (double)1.92 ;

		break;
	case 22:	//I927_init
	case 24:	//I925_L_Wait4ABeat
	case 26:	//I1141_NextAtrBeat
	case 27:	//I923_L_Wait4ABeat
	case 29:	//I1139_L_Wait4ASynch
		return (double)1.93 ;

		break;
	case 33:	//I1119_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_a_dvHigh
	case 34:	//I1118_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_a_dVLow
	case 35:	//I1117_S_tr_AVJ_L_Ante_S_AVJAnteReached_L_Recovery_L_a_dVLow
	case 36:	//I1116_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_L_Recovery_L_a_dvHigh
	case 37:	//I1115_S_tr_AVJ_L_Ante_S_AVJAnteReachedHigh_AVJAnteReachedHigh_L_a_dvHigh
	case 38:	//I1114_S_tr_AVJ_L_Ante_S_AVJAnteReached_AVJAnteReached_L_a_dVLow
	case 75:	//I1104_S_AtrRetroReached_L_Retro
	case 76:	//I1103_S_AtrRetroReached_L_Excitable_L_Retro
	case 80:	//I945_init
		return (double)1.94 ;

		break;
	case 39:	//I1113_S_VtrAnteReached_L_Ante
	case 40:	//I1112_S_VtrAnteReached_L_Excitable_L_Ante
	case 44:	//I1098_S_AVJRetroReached_L_Retro
	case 45:	//I1097_S_AVJRetroReached_L_Recovery_L_Retro
	case 46:	//I1096_S_AVJRetroReached_AVJRetroReached_L_Retro
	case 50:	//I951_init
		return (double)1.95 ;

		break;
	case 30:	//I1122_S_retroAVJ_L_Retro
	case 31:	//I1121_S_retroAVJ_L_Ante_L_Retro
	case 32:	//I1120_S_retroAVJ_L_Empty_L_Retro
	case 47:	//I1095_S_anteAVJ_L_Ante
	case 48:	//I1094_S_anteAVJ_L_Ante_L_Empty
	case 49:	//I1093_S_anteAVJ_L_Ante_L_Retro
	case 70:	//I963_init
		return (double)1.96 ;

		break;
	case 91:	//I953_init
		return (double)1.97 ;

		break;
	case 11:	//I939_init
	case 13:	//I937_L_q0
		return (double)1.98 ;

		break;
	case 90:	//I935_init
		return (double)1.99 ;

		break;
	case 0:	//EMPTY_Abeat
	case 1:	//EMPTY_NextAtrBeat
	case 2:	//EMPTY_NextVtrBeat
	case 3:	//EMPTY_SREC
	case 4:	//EMPTY_Vbeat
	case 5:	//EMPTY_a_dVSetHigh
	case 6:	//EMPTY_a_dVSetLow
		return (double)3 ;

		break;
	case 17:	//RNextVtrBeat267_
	case 19:	//RNextVtrBeat491_
	case 25:	//RNextAtrBeat492_
	case 28:	//RNextAtrBeat89_
	case 53:	//Ra_dVSetHigh656_
	case 55:	//Ra_dVSetLow657_
	case 85:	//RSREC866_
	case 94:	//RAbeat36_
	case 102:	//RVbeat249_
		return (double)3.99 ;

		break;
	case 7:	//INCOMING_SREC
		return (double)5 ;

		break;
	default:	//I1084_S_AVJRetroExit_L_Retro,I1083_S_AVJAnteExit_L_Ante,I1082_S_AVJRetroExit_L_Retro,I1081_S_AVJAnteExit_L_Ante,I1080_S_AVJAnteExit_L_Empty_L_Ante,I1079_S_AVJRetroExit_L_Empty_L_Retro,I919_init,I917_L_Refractory,I915_L_Refractory,I913_L_Refractory,
		return (double)1.88 ;

}
}

REAL_TYPE SPN::GetWeight(TR_PL_ID t)const{
		return (double)1 ;

}

void SPN::reset() {
	Marking.resetToInitMarking();
}

