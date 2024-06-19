#ifndef _MarkingImpl_HPP
#define    _MarkingImpl_HPP
#include <string.h>
class abstractBindingImpl {
public:
};
class abstractMarkingImpl {
public:
	int _PL_SIG_Abeat;
	int _PL_SIG_NextAtrBeat;
	int _PL_SIG_NextVtrBeat;
	int _PL_SIG_SREC;
	int _PL_SIG_Vbeat;
	int _PL_SIG_a_dVSetHigh;
	int _PL_SIG_a_dVSetLow;
	int _PL_Ventricle;
	int _PL_Atrium;
	int _PL_VtrNoiseGenerator;
	int _PL_AtrNoiseGenerator;
	int _PL_Parameter_Serial;
	int _PL_RAConductor;
	int _PL_AVJOut;
	int _PL_AVJ;
	int _PL_A_dV;
	int _PL_RVConductor;
	int _PL_SANode;
	int _PL_VRG;
	int _PL_SANodeEctopic;
	int _PL_VRGEctopic;
};
#endif
