#ifndef __c2_idhp_model_whole_h__
#define __c2_idhp_model_whole_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_idhp_model_wholeInstanceStruct
#define typedef_SFc2_idhp_model_wholeInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  uint32_T c2_APOutEventCounter;
  uint32_T c2_VPOutEventCounter;
  uint32_T c2_AgetOutEventCounter;
  uint32_T c2_VgetOutEventCounter;
  int32_T c2_sfEvent;
  uint8_T c2_tp_Heart;
  uint8_T c2_tp_AtrNoiseGenerator;
  uint8_T c2_tp_q0;
  uint8_T c2_tp_SANodeEctopic;
  uint8_T c2_b_tp_q0;
  uint8_T c2_tp_VtrNoiseGenerator;
  uint8_T c2_c_tp_q0;
  uint8_T c2_tp_VRGEctopic;
  uint8_T c2_d_tp_q0;
  uint8_T c2_tp_AVJOut;
  uint8_T c2_tp_Empty;
  uint8_T c2_tp_Ante;
  uint8_T c2_tp_Retro;
  uint8_T c2_tp_RVConductor;
  uint8_T c2_b_tp_Empty;
  uint8_T c2_b_tp_Ante;
  uint8_T c2_b_tp_Retro;
  uint8_T c2_tp_RAConductor;
  uint8_T c2_c_tp_Empty;
  uint8_T c2_c_tp_Ante;
  uint8_T c2_c_tp_Retro;
  uint8_T c2_tp_SANode;
  uint8_T c2_tp_Wait4ABeat;
  uint8_T c2_tp_Wait4ASynch;
  uint8_T c2_tp_VRG;
  uint8_T c2_tp_Wait4VBeat;
  uint8_T c2_tp_Wait4VSynch;
  uint8_T c2_tp_Ventricle;
  uint8_T c2_tp_Excitable;
  uint8_T c2_tp_Refractory;
  uint8_T c2_tp_VsenseAnte;
  uint8_T c2_tp_VSynch;
  uint8_T c2_tp_Vsense;
  uint8_T c2_tp_GenerateRetroWave;
  uint8_T c2_tp_Atrium;
  uint8_T c2_b_tp_Excitable;
  uint8_T c2_b_tp_Refractory;
  uint8_T c2_tp_ASynch;
  uint8_T c2_tp_AsenseRetro;
  uint8_T c2_tp_Asense;
  uint8_T c2_tp_GenerateAnteWave;
  uint8_T c2_tp_AVJ;
  uint8_T c2_tp_Recovery;
  uint8_T c2_d_tp_Ante;
  uint8_T c2_c_tp_Refractory;
  uint8_T c2_d_tp_Retro;
  uint8_T c2_tp_VRP;
  uint8_T c2_tp_Idle;
  uint8_T c2_b_tp_VRP;
  uint8_T c2_tp_Vsig;
  uint8_T c2_tp_LRI;
  uint8_T c2_b_tp_LRI;
  uint8_T c2_tp_ASensed;
  uint8_T c2_tp_AVI;
  uint8_T c2_tp_WaitURI;
  uint8_T c2_b_tp_Idle;
  uint8_T c2_b_tp_AVI;
  uint8_T c2_tp_PVARP;
  uint8_T c2_c_tp_Idle;
  uint8_T c2_tp_PVAB;
  uint8_T c2_tp_Asig;
  uint8_T c2_b_tp_PVARP;
  uint8_T c2_tp_URI;
  uint8_T c2_b_tp_URI;
  boolean_T c2_isStable;
  uint8_T c2_is_active_c2_idhp_model_whole;
  uint8_T c2_is_c2_idhp_model_whole;
  uint8_T c2_is_AtrNoiseGenerator;
  uint8_T c2_is_active_AtrNoiseGenerator;
  uint8_T c2_is_SANodeEctopic;
  uint8_T c2_is_active_SANodeEctopic;
  uint8_T c2_is_VtrNoiseGenerator;
  uint8_T c2_is_active_VtrNoiseGenerator;
  uint8_T c2_is_VRGEctopic;
  uint8_T c2_is_active_VRGEctopic;
  uint8_T c2_is_AVJOut;
  uint8_T c2_is_active_AVJOut;
  uint8_T c2_is_RVConductor;
  uint8_T c2_is_active_RVConductor;
  uint8_T c2_is_RAConductor;
  uint8_T c2_is_active_RAConductor;
  uint8_T c2_is_SANode;
  uint8_T c2_is_active_SANode;
  uint8_T c2_is_VRG;
  uint8_T c2_is_active_VRG;
  uint8_T c2_is_Ventricle;
  uint8_T c2_is_active_Ventricle;
  uint8_T c2_is_Atrium;
  uint8_T c2_is_active_Atrium;
  uint8_T c2_is_AVJ;
  uint8_T c2_is_active_AVJ;
  uint8_T c2_is_VRP;
  uint8_T c2_is_active_VRP;
  uint8_T c2_is_LRI;
  uint8_T c2_is_active_LRI;
  uint8_T c2_is_AVI;
  uint8_T c2_is_active_AVI;
  uint8_T c2_is_PVARP;
  uint8_T c2_is_active_PVARP;
  uint8_T c2_is_URI;
  uint8_T c2_is_active_URI;
  real_T c2_Atr_refrD;
  real_T c2_AV_alpha;
  real_T c2_AV_anteDMin;
  real_T c2_AV_beta;
  real_T c2_AV_delta;
  real_T c2_AV_k4;
  real_T c2_AV_refrDMin;
  real_T c2_AV_retroDMin;
  real_T c2_AV_tauC;
  real_T c2_AV_theta;
  real_T c2_AV_tr;
  real_T c2_AV_Vr;
  real_T c2_AV_Vt;
  real_T c2_RA_anteD;
  real_T c2_RA_retroD;
  real_T c2_RV_anteD;
  real_T c2_RV_retroD;
  real_T c2_SA_d;
  real_T c2_SA_dV;
  real_T c2_SA_ectopD;
  real_T c2_SA_ectopdV;
  real_T c2_SA_noiseD;
  real_T c2_VRG_d;
  real_T c2_VRG_ectopD;
  real_T c2_VRG_noiseD;
  real_T c2_Vtr_refrD;
  real_T c2_a_dV;
  real_T c2_avj_tRec;
  real_T c2_clk;
  real_T c2_PM_TAVI;
  real_T c2_PM_TLRI;
  real_T c2_PM_TPVAB;
  real_T c2_PM_TPVARP;
  real_T c2_PM_TURI;
  real_T c2_PM_TVRP;
  real_T c2_avj_refr;
  real_T c2_avj_t4;
  real_T c2_avj_refr_0;
  uint32_T c2_temporalCounter_i1;
  uint32_T c2_temporalCounter_i2;
  uint32_T c2_temporalCounter_i3;
  uint32_T c2_temporalCounter_i4;
  uint32_T c2_temporalCounter_i5;
  uint32_T c2_temporalCounter_i6;
  uint32_T c2_temporalCounter_i7;
  uint32_T c2_temporalCounter_i8;
  uint32_T c2_temporalCounter_i9;
  uint32_T c2_temporalCounter_i10;
  uint32_T c2_temporalCounter_i11;
  uint32_T c2_temporalCounter_i12;
  uint32_T c2_temporalCounter_i13;
  uint32_T c2_temporalCounter_i14;
  uint32_T c2_temporalCounter_i15;
  uint32_T c2_temporalCounter_i16;
  uint32_T c2_temporalCounter_i17;
  uint8_T c2_doSetSimStateSideEffects;
  const mxArray *c2_setSimStateSideEffectsInfo;
} SFc2_idhp_model_wholeInstanceStruct;

#endif                                 /*typedef_SFc2_idhp_model_wholeInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_idhp_model_whole_get_eml_resolved_functions_info
  (void);

/* Function Definitions */
extern void sf_c2_idhp_model_whole_get_check_sum(mxArray *plhs[]);
extern void c2_idhp_model_whole_method_dispatcher(SimStruct *S, int_T method,
  void *data);

#endif
