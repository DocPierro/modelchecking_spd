#ifndef __c2_HeartModelECG_h__
#define __c2_HeartModelECG_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_HeartModelECGInstanceStruct
#define typedef_SFc2_HeartModelECGInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  uint32_T c2_AgetEventCounter;
  uint32_T c2_AVReachedEventCounter;
  uint32_T c2_VgetEventCounter;
  uint32_T c2_RefractEventCounter;
  int32_T c2_sfEvent;
  uint8_T c2_tp_Heart;
  uint8_T c2_tp_A_dV;
  uint8_T c2_tp_a_dVLow;
  uint8_T c2_tp_a_dvHigh;
  uint8_T c2_tp_SANodeEctopic;
  uint8_T c2_tp_q0;
  uint8_T c2_tp_AtrNoiseGenerator;
  uint8_T c2_b_tp_q0;
  uint8_T c2_tp_RAConductor;
  uint8_T c2_tp_Empty;
  uint8_T c2_tp_Ante;
  uint8_T c2_tp_Retro;
  uint8_T c2_tp_AVJOut;
  uint8_T c2_b_tp_Empty;
  uint8_T c2_b_tp_Ante;
  uint8_T c2_b_tp_Retro;
  uint8_T c2_tp_VRGEctopic;
  uint8_T c2_c_tp_q0;
  uint8_T c2_tp_RVConductor;
  uint8_T c2_c_tp_Empty;
  uint8_T c2_c_tp_Ante;
  uint8_T c2_c_tp_Retro;
  uint8_T c2_tp_VtrNoiseGenerator;
  uint8_T c2_d_tp_q0;
  uint8_T c2_tp_SANode;
  uint8_T c2_tp_Wait4ABeat;
  uint8_T c2_tp_Wait4ASynch;
  uint8_T c2_tp_VRG;
  uint8_T c2_tp_Wait4VBeat;
  uint8_T c2_tp_Wait4VSynch;
  uint8_T c2_tp_AVJ;
  uint8_T c2_tp_Recovery;
  uint8_T c2_d_tp_Ante;
  uint8_T c2_tp_Refractory;
  uint8_T c2_d_tp_Retro;
  uint8_T c2_tp_Ventricle;
  uint8_T c2_tp_Excitable;
  uint8_T c2_b_tp_Refractory;
  uint8_T c2_tp_VsenseAnte;
  uint8_T c2_tp_VSynch;
  uint8_T c2_tp_Vsense;
  uint8_T c2_tp_GenerateRetroWave;
  uint8_T c2_tp_Atrium;
  uint8_T c2_b_tp_Excitable;
  uint8_T c2_c_tp_Refractory;
  uint8_T c2_tp_ASynch;
  uint8_T c2_tp_AsenseRetro;
  uint8_T c2_tp_Asense;
  uint8_T c2_tp_GenerateAnteWave;
  boolean_T c2_isStable;
  uint8_T c2_is_active_c2_HeartModelECG;
  uint8_T c2_is_c2_HeartModelECG;
  uint8_T c2_is_A_dV;
  uint8_T c2_is_active_A_dV;
  uint8_T c2_is_SANodeEctopic;
  uint8_T c2_is_active_SANodeEctopic;
  uint8_T c2_is_AtrNoiseGenerator;
  uint8_T c2_is_active_AtrNoiseGenerator;
  uint8_T c2_is_RAConductor;
  uint8_T c2_is_active_RAConductor;
  uint8_T c2_is_AVJOut;
  uint8_T c2_is_active_AVJOut;
  uint8_T c2_is_VRGEctopic;
  uint8_T c2_is_active_VRGEctopic;
  uint8_T c2_is_RVConductor;
  uint8_T c2_is_active_RVConductor;
  uint8_T c2_is_VtrNoiseGenerator;
  uint8_T c2_is_active_VtrNoiseGenerator;
  uint8_T c2_is_SANode;
  uint8_T c2_is_active_SANode;
  uint8_T c2_is_VRG;
  uint8_T c2_is_active_VRG;
  uint8_T c2_is_AVJ;
  uint8_T c2_is_active_AVJ;
  uint8_T c2_is_Ventricle;
  uint8_T c2_is_active_Ventricle;
  uint8_T c2_is_Atrium;
  uint8_T c2_is_active_Atrium;
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
  real_T c2_SA_dV;
  real_T c2_SA_ectopD;
  real_T c2_SA_ectopdV;
  real_T c2_SA_noiseD;
  real_T c2_VRG_d;
  real_T c2_VRG_ectopD;
  real_T c2_VRG_noiseD;
  real_T c2_Vtr_refrD;
  real_T c2_avj_tRec;
  real_T c2_clk;
  real_T c2_PM_TAVI;
  real_T c2_PM_TLRI;
  real_T c2_PM_TPVAB;
  real_T c2_PM_TPVARP;
  real_T c2_PM_TURI;
  real_T c2_PM_TVRP;
  real_T c2_recoverylast;
  real_T c2_avj_t4;
  real_T c2_ctime;
  real_T c2_avj_refr;
  real_T c2_avj_refr_0;
  real_T c2_refractorylast;
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
  uint8_T c2_doSetSimStateSideEffects;
  const mxArray *c2_setSimStateSideEffectsInfo;
} SFc2_HeartModelECGInstanceStruct;

#endif                                 /*typedef_SFc2_HeartModelECGInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_HeartModelECG_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_HeartModelECG_get_check_sum(mxArray *plhs[]);
extern void c2_HeartModelECG_method_dispatcher(SimStruct *S, int_T method, void *
  data);

#endif
