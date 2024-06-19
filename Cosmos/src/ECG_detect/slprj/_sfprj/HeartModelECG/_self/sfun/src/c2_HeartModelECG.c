/* Include files */

#include <stddef.h>
#include "blas.h"
#include "HeartModelECG_sfun.h"
#include "c2_HeartModelECG.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "HeartModelECG_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define c2_event_Abeat                 (0)
#define c2_event_AtrRetroReached       (1)
#define c2_event_arg                   (2)
#define c2_event_retroAVJ              (3)
#define c2_event_AVJAnteReached        (4)
#define c2_event_NextAtrBeat           (5)
#define c2_event_AVJRetroReached       (6)
#define c2_event_anteAVJ               (7)
#define c2_event_vrg                   (8)
#define c2_event_VtrAnteReached        (9)
#define c2_event_AVJRetroExit          (10)
#define c2_event_AVJAnteExit           (11)
#define c2_event_NextVtrBeat           (12)
#define c2_event_Vbeat                 (13)
#define c2_event_AP                    (14)
#define c2_event_VP                    (15)
#define c2_event_AS                    (16)
#define c2_event_VS                    (17)
#define c2_event_Aintrinsic            (18)
#define c2_event_Anoise                (19)
#define c2_event_Aretro                (20)
#define c2_event_Vintrinsic            (21)
#define c2_event_Vnoise                (22)
#define c2_event_Vante                 (23)
#define c2_event_APElectr              (24)
#define c2_event_VPElectr              (25)
#define c2_event_IncrClk               (26)
#define c2_event_State                 (27)
#define c2_event_Clk0                  (28)
#define c2_event_Clk1                  (29)
#define c2_event_a_dVSetHigh           (30)
#define c2_event_a_dVSetLow            (31)
#define c2_event_tr_AVJ                (32)
#define c2_event_AVJAnteReachedHigh    (33)
#define c2_event_RLRI                  (34)
#define c2_event_TAVI                  (35)
#define c2_event_RTAVI                 (36)
#define c2_event_RURI                  (37)
#define CALL_EVENT                     (-1)
#define c2_IN_NO_ACTIVE_CHILD          ((uint8_T)0U)
#define c2_IN_Heart                    ((uint8_T)1U)
#define c2_IN_a_dVLow                  ((uint8_T)1U)
#define c2_IN_a_dvHigh                 ((uint8_T)2U)
#define c2_IN_q0                       ((uint8_T)1U)
#define c2_IN_Ante                     ((uint8_T)1U)
#define c2_IN_Empty                    ((uint8_T)2U)
#define c2_IN_Retro                    ((uint8_T)3U)
#define c2_IN_Wait4ABeat               ((uint8_T)1U)
#define c2_IN_Wait4ASynch              ((uint8_T)2U)
#define c2_IN_Wait4VBeat               ((uint8_T)1U)
#define c2_IN_Wait4VSynch              ((uint8_T)2U)
#define c2_IN_Recovery                 ((uint8_T)2U)
#define c2_IN_Refractory               ((uint8_T)3U)
#define c2_b_IN_Retro                  ((uint8_T)4U)
#define c2_IN_Excitable                ((uint8_T)1U)
#define c2_IN_GenerateRetroWave        ((uint8_T)2U)
#define c2_IN_VSynch                   ((uint8_T)4U)
#define c2_IN_Vsense                   ((uint8_T)5U)
#define c2_IN_VsenseAnte               ((uint8_T)6U)
#define c2_IN_ASynch                   ((uint8_T)1U)
#define c2_IN_Asense                   ((uint8_T)2U)
#define c2_IN_AsenseRetro              ((uint8_T)3U)
#define c2_b_IN_Excitable              ((uint8_T)4U)
#define c2_IN_GenerateAnteWave         ((uint8_T)5U)
#define c2_b_IN_Refractory             ((uint8_T)6U)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c2_debug_family_names[5] = { "nargin", "nargout", "tRec",
  "minDly", "t" };

static const char * c2_b_debug_family_names[4] = { "nargin", "nargout", "Vm",
  "t" };

static const char * c2_c_debug_family_names[4] = { "nargin", "nargout", "tRec",
  "t" };

static const char * c2_d_debug_family_names[5] = { "nargin", "nargout", "t",
  "t0", "dT" };

static const char * c2_e_debug_family_names[6] = { "nargin", "nargout", "t",
  "t0", "dV", "dT" };

/* Function Declarations */
static void initialize_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void initialize_params_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct *
  chartInstance);
static void enable_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void disable_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void c2_update_debugger_state_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance);
static void set_sim_state_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_st);
static void c2_set_sim_state_side_effects_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance);
static void finalize_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void sf_gateway_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void c2_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void initSimStructsc2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void c2_enter_internal_Heart(SFc2_HeartModelECGInstanceStruct
  *chartInstance);
static void c2_Heart(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_A_dV(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_RAConductor(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_AVJOut(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_RVConductor(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_SANode(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_VRG(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_AVJ(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_Retro(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_Ventricle(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void c2_Atrium(SFc2_HeartModelECGInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData);
static real_T c2_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_t, const char_T *c2_identifier);
static real_T c2_b_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_info_helper(const mxArray **c2_info);
static const mxArray *c2_emlrt_marshallOut(const char * c2_u);
static const mxArray *c2_b_emlrt_marshallOut(const uint32_T c2_u);
static real_T c2_AVJDelay(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_tRec, real_T c2_minDly);
static real_T c2_timeToDep(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_Vm);
static real_T c2_initRefrPeriod(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_tRec);
static real_T c2_modulateRefrRetro(SFc2_HeartModelECGInstanceStruct
  *chartInstance, real_T c2_t, real_T c2_t0);
static real_T c2_modulateRefrAnte(SFc2_HeartModelECGInstanceStruct
  *chartInstance, real_T c2_t, real_T c2_t0, real_T c2_dV);
static void c2_eml_scalar_eg(SFc2_HeartModelECGInstanceStruct *chartInstance);
static real_T c2_power(SFc2_HeartModelECGInstanceStruct *chartInstance, real_T
  c2_a, real_T c2_b);
static void c2_eml_error(SFc2_HeartModelECGInstanceStruct *chartInstance);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static uint32_T c2_c_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_AgetEventCounter, const char_T
  *c2_identifier);
static uint32_T c2_d_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static int32_T c2_e_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static uint8_T c2_f_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_tp_Heart, const char_T *c2_identifier);
static uint8_T c2_g_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static boolean_T c2_h_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_AVReached, const char_T *c2_identifier);
static boolean_T c2_i_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static const mxArray *c2_j_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_setSimStateSideEffectsInfo, const char_T
  *c2_identifier);
static const mxArray *c2_k_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void init_dsm_address_info(SFc2_HeartModelECGInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  boolean_T *c2_Aget;
  boolean_T *c2_AVReached;
  boolean_T *c2_Vget;
  boolean_T *c2_Refract;
  c2_Refract = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c2_Vget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c2_AVReached = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c2_Aget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c2_doSetSimStateSideEffects = 0U;
  chartInstance->c2_setSimStateSideEffectsInfo = NULL;
  chartInstance->c2_tp_Heart = 0U;
  chartInstance->c2_is_active_AVJ = 0U;
  chartInstance->c2_is_AVJ = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_AVJ = 0U;
  chartInstance->c2_d_tp_Ante = 0U;
  chartInstance->c2_temporalCounter_i10 = 0U;
  chartInstance->c2_tp_Recovery = 0U;
  chartInstance->c2_temporalCounter_i10 = 0U;
  chartInstance->c2_tp_Refractory = 0U;
  chartInstance->c2_temporalCounter_i10 = 0U;
  chartInstance->c2_d_tp_Retro = 0U;
  chartInstance->c2_temporalCounter_i10 = 0U;
  chartInstance->c2_is_active_AVJOut = 0U;
  chartInstance->c2_is_AVJOut = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_AVJOut = 0U;
  chartInstance->c2_b_tp_Ante = 0U;
  chartInstance->c2_temporalCounter_i4 = 0U;
  chartInstance->c2_b_tp_Empty = 0U;
  chartInstance->c2_b_tp_Retro = 0U;
  chartInstance->c2_temporalCounter_i4 = 0U;
  chartInstance->c2_is_active_A_dV = 0U;
  chartInstance->c2_is_A_dV = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_A_dV = 0U;
  chartInstance->c2_tp_a_dVLow = 0U;
  chartInstance->c2_tp_a_dvHigh = 0U;
  chartInstance->c2_is_active_AtrNoiseGenerator = 0U;
  chartInstance->c2_is_AtrNoiseGenerator = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_AtrNoiseGenerator = 0U;
  chartInstance->c2_b_tp_q0 = 0U;
  chartInstance->c2_temporalCounter_i2 = 0U;
  chartInstance->c2_is_active_Atrium = 0U;
  chartInstance->c2_is_Atrium = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_Atrium = 0U;
  chartInstance->c2_tp_ASynch = 0U;
  chartInstance->c2_tp_Asense = 0U;
  chartInstance->c2_tp_AsenseRetro = 0U;
  chartInstance->c2_b_tp_Excitable = 0U;
  chartInstance->c2_tp_GenerateAnteWave = 0U;
  chartInstance->c2_c_tp_Refractory = 0U;
  chartInstance->c2_temporalCounter_i12 = 0U;
  chartInstance->c2_is_active_RAConductor = 0U;
  chartInstance->c2_is_RAConductor = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_RAConductor = 0U;
  chartInstance->c2_tp_Ante = 0U;
  chartInstance->c2_temporalCounter_i3 = 0U;
  chartInstance->c2_tp_Empty = 0U;
  chartInstance->c2_tp_Retro = 0U;
  chartInstance->c2_temporalCounter_i3 = 0U;
  chartInstance->c2_is_active_RVConductor = 0U;
  chartInstance->c2_is_RVConductor = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_RVConductor = 0U;
  chartInstance->c2_c_tp_Ante = 0U;
  chartInstance->c2_temporalCounter_i6 = 0U;
  chartInstance->c2_c_tp_Empty = 0U;
  chartInstance->c2_c_tp_Retro = 0U;
  chartInstance->c2_temporalCounter_i6 = 0U;
  chartInstance->c2_is_active_SANode = 0U;
  chartInstance->c2_is_SANode = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_SANode = 0U;
  chartInstance->c2_tp_Wait4ABeat = 0U;
  chartInstance->c2_temporalCounter_i8 = 0U;
  chartInstance->c2_tp_Wait4ASynch = 0U;
  chartInstance->c2_is_active_SANodeEctopic = 0U;
  chartInstance->c2_is_SANodeEctopic = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_SANodeEctopic = 0U;
  chartInstance->c2_tp_q0 = 0U;
  chartInstance->c2_temporalCounter_i1 = 0U;
  chartInstance->c2_is_active_VRG = 0U;
  chartInstance->c2_is_VRG = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_VRG = 0U;
  chartInstance->c2_tp_Wait4VBeat = 0U;
  chartInstance->c2_temporalCounter_i9 = 0U;
  chartInstance->c2_tp_Wait4VSynch = 0U;
  chartInstance->c2_is_active_VRGEctopic = 0U;
  chartInstance->c2_is_VRGEctopic = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_VRGEctopic = 0U;
  chartInstance->c2_c_tp_q0 = 0U;
  chartInstance->c2_temporalCounter_i5 = 0U;
  chartInstance->c2_is_active_Ventricle = 0U;
  chartInstance->c2_is_Ventricle = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_Ventricle = 0U;
  chartInstance->c2_tp_Excitable = 0U;
  chartInstance->c2_tp_GenerateRetroWave = 0U;
  chartInstance->c2_b_tp_Refractory = 0U;
  chartInstance->c2_temporalCounter_i11 = 0U;
  chartInstance->c2_tp_VSynch = 0U;
  chartInstance->c2_tp_Vsense = 0U;
  chartInstance->c2_tp_VsenseAnte = 0U;
  chartInstance->c2_is_active_VtrNoiseGenerator = 0U;
  chartInstance->c2_is_VtrNoiseGenerator = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_tp_VtrNoiseGenerator = 0U;
  chartInstance->c2_d_tp_q0 = 0U;
  chartInstance->c2_temporalCounter_i7 = 0U;
  chartInstance->c2_is_active_c2_HeartModelECG = 0U;
  chartInstance->c2_is_c2_HeartModelECG = c2_IN_NO_ACTIVE_CHILD;
  chartInstance->c2_avj_tRec = 0.0;
  chartInstance->c2_clk = 0.0;
  chartInstance->c2_recoverylast = 0.0;
  chartInstance->c2_avj_t4 = 0.0;
  chartInstance->c2_ctime = 0.0;
  chartInstance->c2_avj_refr = 0.0;
  chartInstance->c2_avj_refr_0 = 0.0;
  chartInstance->c2_refractorylast = 0.0;
  chartInstance->c2_AgetEventCounter = 0U;
  *c2_Aget = false;
  chartInstance->c2_AVReachedEventCounter = 0U;
  *c2_AVReached = false;
  chartInstance->c2_VgetEventCounter = 0U;
  *c2_Vget = false;
  chartInstance->c2_RefractEventCounter = 0U;
  *c2_Refract = false;
}

static void initialize_params_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct *
  chartInstance)
{
  real_T c2_d0;
  real_T c2_d1;
  real_T c2_d2;
  real_T c2_d3;
  real_T c2_d4;
  real_T c2_d5;
  real_T c2_d6;
  real_T c2_d7;
  real_T c2_d8;
  real_T c2_d9;
  real_T c2_d10;
  real_T c2_d11;
  real_T c2_d12;
  real_T c2_d13;
  real_T c2_d14;
  real_T c2_d15;
  real_T c2_d16;
  real_T c2_d17;
  real_T c2_d18;
  real_T c2_d19;
  real_T c2_d20;
  real_T c2_d21;
  real_T c2_d22;
  real_T c2_d23;
  real_T c2_d24;
  real_T c2_d25;
  real_T c2_d26;
  real_T c2_d27;
  real_T c2_d28;
  real_T c2_d29;
  real_T c2_d30;
  sf_mex_import_named("Atr_refrD", sf_mex_get_sfun_param(chartInstance->S, 12, 0),
                      &c2_d0, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_Atr_refrD = c2_d0;
  sf_mex_import_named("AV_alpha", sf_mex_get_sfun_param(chartInstance->S, 2, 0),
                      &c2_d1, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_alpha = c2_d1;
  sf_mex_import_named("AV_anteDMin", sf_mex_get_sfun_param(chartInstance->S, 3,
    0), &c2_d2, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_anteDMin = c2_d2;
  sf_mex_import_named("AV_beta", sf_mex_get_sfun_param(chartInstance->S, 4, 0),
                      &c2_d3, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_beta = c2_d3;
  sf_mex_import_named("AV_delta", sf_mex_get_sfun_param(chartInstance->S, 5, 0),
                      &c2_d4, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_delta = c2_d4;
  sf_mex_import_named("AV_k4", sf_mex_get_sfun_param(chartInstance->S, 6, 0),
                      &c2_d5, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_k4 = c2_d5;
  sf_mex_import_named("AV_refrDMin", sf_mex_get_sfun_param(chartInstance->S, 7,
    0), &c2_d6, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_refrDMin = c2_d6;
  sf_mex_import_named("AV_retroDMin", sf_mex_get_sfun_param(chartInstance->S, 8,
    0), &c2_d7, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_retroDMin = c2_d7;
  sf_mex_import_named("AV_tauC", sf_mex_get_sfun_param(chartInstance->S, 9, 0),
                      &c2_d8, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_tauC = c2_d8;
  sf_mex_import_named("AV_theta", sf_mex_get_sfun_param(chartInstance->S, 10, 0),
                      &c2_d9, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_theta = c2_d9;
  sf_mex_import_named("AV_tr", sf_mex_get_sfun_param(chartInstance->S, 11, 0),
                      &c2_d10, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_tr = c2_d10;
  sf_mex_import_named("AV_Vr", sf_mex_get_sfun_param(chartInstance->S, 0, 0),
                      &c2_d11, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_Vr = c2_d11;
  sf_mex_import_named("AV_Vt", sf_mex_get_sfun_param(chartInstance->S, 1, 0),
                      &c2_d12, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_AV_Vt = c2_d12;
  sf_mex_import_named("RA_anteD", sf_mex_get_sfun_param(chartInstance->S, 19, 0),
                      &c2_d13, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_RA_anteD = c2_d13;
  sf_mex_import_named("RA_retroD", sf_mex_get_sfun_param(chartInstance->S, 20, 0),
                      &c2_d14, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_RA_retroD = c2_d14;
  sf_mex_import_named("RV_anteD", sf_mex_get_sfun_param(chartInstance->S, 21, 0),
                      &c2_d15, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_RV_anteD = c2_d15;
  sf_mex_import_named("RV_retroD", sf_mex_get_sfun_param(chartInstance->S, 22, 0),
                      &c2_d16, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_RV_retroD = c2_d16;
  sf_mex_import_named("SA_dV", sf_mex_get_sfun_param(chartInstance->S, 23, 0),
                      &c2_d17, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_SA_dV = c2_d17;
  sf_mex_import_named("SA_ectopD", sf_mex_get_sfun_param(chartInstance->S, 24, 0),
                      &c2_d18, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_SA_ectopD = c2_d18;
  sf_mex_import_named("SA_ectopdV", sf_mex_get_sfun_param(chartInstance->S, 25,
    0), &c2_d19, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_SA_ectopdV = c2_d19;
  sf_mex_import_named("SA_noiseD", sf_mex_get_sfun_param(chartInstance->S, 26, 0),
                      &c2_d20, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_SA_noiseD = c2_d20;
  sf_mex_import_named("VRG_d", sf_mex_get_sfun_param(chartInstance->S, 27, 0),
                      &c2_d21, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_VRG_d = c2_d21;
  sf_mex_import_named("VRG_ectopD", sf_mex_get_sfun_param(chartInstance->S, 28,
    0), &c2_d22, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_VRG_ectopD = c2_d22;
  sf_mex_import_named("VRG_noiseD", sf_mex_get_sfun_param(chartInstance->S, 29,
    0), &c2_d23, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_VRG_noiseD = c2_d23;
  sf_mex_import_named("Vtr_refrD", sf_mex_get_sfun_param(chartInstance->S, 30, 0),
                      &c2_d24, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_Vtr_refrD = c2_d24;
  sf_mex_import_named("PM_TAVI", sf_mex_get_sfun_param(chartInstance->S, 13, 0),
                      &c2_d25, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TAVI = c2_d25;
  sf_mex_import_named("PM_TLRI", sf_mex_get_sfun_param(chartInstance->S, 14, 0),
                      &c2_d26, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TLRI = c2_d26;
  sf_mex_import_named("PM_TPVAB", sf_mex_get_sfun_param(chartInstance->S, 15, 0),
                      &c2_d27, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TPVAB = c2_d27;
  sf_mex_import_named("PM_TPVARP", sf_mex_get_sfun_param(chartInstance->S, 16, 0),
                      &c2_d28, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TPVARP = c2_d28;
  sf_mex_import_named("PM_TURI", sf_mex_get_sfun_param(chartInstance->S, 17, 0),
                      &c2_d29, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TURI = c2_d29;
  sf_mex_import_named("PM_TVRP", sf_mex_get_sfun_param(chartInstance->S, 18, 0),
                      &c2_d30, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c2_PM_TVRP = c2_d30;
}

static void enable_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c2_update_debugger_state_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  uint32_T c2_prevAniVal;
  c2_prevAniVal = _SFD_GET_ANIMATION();
  _SFD_SET_ANIMATION(0U);
  _SFD_SET_HONOR_BREAKPOINTS(0U);
  if (chartInstance->c2_is_active_c2_HeartModelECG == 1U) {
    _SFD_CC_CALL(CHART_ACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_c2_HeartModelECG == c2_IN_Heart) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 0U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 0U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_A_dV == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 15U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 15U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_A_dV == c2_IN_a_dVLow) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_A_dV == c2_IN_a_dvHigh) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_SANodeEctopic == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 38U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 38U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_SANodeEctopic == c2_IN_q0) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_AtrNoiseGenerator == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 18U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 18U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AtrNoiseGenerator == c2_IN_q0) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_RAConductor == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 27U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 27U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RAConductor == c2_IN_Empty) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RAConductor == c2_IN_Ante) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RAConductor == c2_IN_Retro) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_AVJOut == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJOut == c2_IN_Empty) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJOut == c2_IN_Ante) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJOut == c2_IN_Retro) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 14U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 14U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_VRGEctopic == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 43U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 43U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_VRGEctopic == c2_IN_q0) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_RVConductor == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RVConductor == c2_IN_Empty) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RVConductor == c2_IN_Ante) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_RVConductor == c2_IN_Retro) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_VtrNoiseGenerator == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 52U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 52U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_VtrNoiseGenerator == c2_IN_q0) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_SANode == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_SANode == c2_IN_Wait4ABeat) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_SANode == c2_IN_Wait4ASynch) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_VRG == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 40U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 40U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_VRG == c2_IN_Wait4VBeat) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_VRG == c2_IN_Wait4VSynch) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 42U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 42U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_AVJ == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJ == c2_IN_Recovery) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJ == c2_IN_Ante) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJ == c2_IN_Refractory) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_AVJ == c2_b_IN_Retro) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 5U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 5U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_Ventricle == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 45U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 45U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_Excitable) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_Refractory) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_VsenseAnte) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 51U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 51U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_VSynch) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_Vsense) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 50U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 50U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Ventricle == c2_IN_GenerateRetroWave) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_Atrium == 1U) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_b_IN_Excitable) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_b_IN_Refractory) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_IN_ASynch) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_IN_AsenseRetro) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_IN_Asense) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_Atrium == c2_IN_GenerateAnteWave) {
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
  } else {
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
  }

  _SFD_SET_ANIMATION(c2_prevAniVal);
  _SFD_SET_HONOR_BREAKPOINTS(1U);
  _SFD_ANIMATE();
}

static const mxArray *get_sim_state_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  const mxArray *c2_st;
  const mxArray *c2_y = NULL;
  real_T c2_hoistedGlobal;
  real_T c2_u;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_hoistedGlobal;
  real_T c2_b_u;
  const mxArray *c2_c_y = NULL;
  real_T c2_c_hoistedGlobal;
  real_T c2_c_u;
  const mxArray *c2_d_y = NULL;
  real_T c2_d_hoistedGlobal;
  real_T c2_d_u;
  const mxArray *c2_e_y = NULL;
  real_T c2_e_hoistedGlobal;
  real_T c2_e_u;
  const mxArray *c2_f_y = NULL;
  real_T c2_f_hoistedGlobal;
  real_T c2_f_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_g_hoistedGlobal;
  real_T c2_g_u;
  const mxArray *c2_h_y = NULL;
  real_T c2_h_hoistedGlobal;
  real_T c2_h_u;
  const mxArray *c2_i_y = NULL;
  boolean_T c2_i_hoistedGlobal;
  boolean_T c2_i_u;
  const mxArray *c2_j_y = NULL;
  boolean_T c2_j_hoistedGlobal;
  boolean_T c2_j_u;
  const mxArray *c2_k_y = NULL;
  boolean_T c2_k_hoistedGlobal;
  boolean_T c2_k_u;
  const mxArray *c2_l_y = NULL;
  boolean_T c2_l_hoistedGlobal;
  boolean_T c2_l_u;
  const mxArray *c2_m_y = NULL;
  uint32_T c2_m_hoistedGlobal;
  uint32_T c2_m_u;
  const mxArray *c2_n_y = NULL;
  uint32_T c2_n_hoistedGlobal;
  uint32_T c2_n_u;
  const mxArray *c2_o_y = NULL;
  uint32_T c2_o_hoistedGlobal;
  uint32_T c2_o_u;
  const mxArray *c2_p_y = NULL;
  uint32_T c2_p_hoistedGlobal;
  uint32_T c2_p_u;
  const mxArray *c2_q_y = NULL;
  uint8_T c2_q_hoistedGlobal;
  uint8_T c2_q_u;
  const mxArray *c2_r_y = NULL;
  uint8_T c2_r_hoistedGlobal;
  uint8_T c2_r_u;
  const mxArray *c2_s_y = NULL;
  uint8_T c2_s_hoistedGlobal;
  uint8_T c2_s_u;
  const mxArray *c2_t_y = NULL;
  uint8_T c2_t_hoistedGlobal;
  uint8_T c2_t_u;
  const mxArray *c2_u_y = NULL;
  uint8_T c2_u_hoistedGlobal;
  uint8_T c2_u_u;
  const mxArray *c2_v_y = NULL;
  uint8_T c2_v_hoistedGlobal;
  uint8_T c2_v_u;
  const mxArray *c2_w_y = NULL;
  uint8_T c2_w_hoistedGlobal;
  uint8_T c2_w_u;
  const mxArray *c2_x_y = NULL;
  uint8_T c2_x_hoistedGlobal;
  uint8_T c2_x_u;
  const mxArray *c2_y_y = NULL;
  uint8_T c2_y_hoistedGlobal;
  uint8_T c2_y_u;
  const mxArray *c2_ab_y = NULL;
  uint8_T c2_ab_hoistedGlobal;
  uint8_T c2_ab_u;
  const mxArray *c2_bb_y = NULL;
  uint8_T c2_bb_hoistedGlobal;
  uint8_T c2_bb_u;
  const mxArray *c2_cb_y = NULL;
  uint8_T c2_cb_hoistedGlobal;
  uint8_T c2_cb_u;
  const mxArray *c2_db_y = NULL;
  uint8_T c2_db_hoistedGlobal;
  uint8_T c2_db_u;
  const mxArray *c2_eb_y = NULL;
  uint8_T c2_eb_hoistedGlobal;
  uint8_T c2_eb_u;
  const mxArray *c2_fb_y = NULL;
  uint8_T c2_fb_hoistedGlobal;
  uint8_T c2_fb_u;
  const mxArray *c2_gb_y = NULL;
  uint8_T c2_gb_hoistedGlobal;
  uint8_T c2_gb_u;
  const mxArray *c2_hb_y = NULL;
  uint8_T c2_hb_hoistedGlobal;
  uint8_T c2_hb_u;
  const mxArray *c2_ib_y = NULL;
  uint8_T c2_ib_hoistedGlobal;
  uint8_T c2_ib_u;
  const mxArray *c2_jb_y = NULL;
  uint8_T c2_jb_hoistedGlobal;
  uint8_T c2_jb_u;
  const mxArray *c2_kb_y = NULL;
  uint8_T c2_kb_hoistedGlobal;
  uint8_T c2_kb_u;
  const mxArray *c2_lb_y = NULL;
  uint8_T c2_lb_hoistedGlobal;
  uint8_T c2_lb_u;
  const mxArray *c2_mb_y = NULL;
  uint8_T c2_mb_hoistedGlobal;
  uint8_T c2_mb_u;
  const mxArray *c2_nb_y = NULL;
  uint8_T c2_nb_hoistedGlobal;
  uint8_T c2_nb_u;
  const mxArray *c2_ob_y = NULL;
  uint8_T c2_ob_hoistedGlobal;
  uint8_T c2_ob_u;
  const mxArray *c2_pb_y = NULL;
  uint8_T c2_pb_hoistedGlobal;
  uint8_T c2_pb_u;
  const mxArray *c2_qb_y = NULL;
  uint8_T c2_qb_hoistedGlobal;
  uint8_T c2_qb_u;
  const mxArray *c2_rb_y = NULL;
  uint8_T c2_rb_hoistedGlobal;
  uint8_T c2_rb_u;
  const mxArray *c2_sb_y = NULL;
  uint8_T c2_sb_hoistedGlobal;
  uint8_T c2_sb_u;
  const mxArray *c2_tb_y = NULL;
  uint32_T c2_tb_hoistedGlobal;
  uint32_T c2_tb_u;
  const mxArray *c2_ub_y = NULL;
  uint32_T c2_ub_hoistedGlobal;
  uint32_T c2_ub_u;
  const mxArray *c2_vb_y = NULL;
  uint32_T c2_vb_hoistedGlobal;
  uint32_T c2_vb_u;
  const mxArray *c2_wb_y = NULL;
  uint32_T c2_wb_hoistedGlobal;
  uint32_T c2_wb_u;
  const mxArray *c2_xb_y = NULL;
  uint32_T c2_xb_hoistedGlobal;
  uint32_T c2_xb_u;
  const mxArray *c2_yb_y = NULL;
  uint32_T c2_yb_hoistedGlobal;
  uint32_T c2_yb_u;
  const mxArray *c2_ac_y = NULL;
  uint32_T c2_ac_hoistedGlobal;
  uint32_T c2_ac_u;
  const mxArray *c2_bc_y = NULL;
  uint32_T c2_bc_hoistedGlobal;
  uint32_T c2_bc_u;
  const mxArray *c2_cc_y = NULL;
  uint32_T c2_cc_hoistedGlobal;
  uint32_T c2_cc_u;
  const mxArray *c2_dc_y = NULL;
  uint32_T c2_dc_hoistedGlobal;
  uint32_T c2_dc_u;
  const mxArray *c2_ec_y = NULL;
  uint32_T c2_ec_hoistedGlobal;
  uint32_T c2_ec_u;
  const mxArray *c2_fc_y = NULL;
  uint32_T c2_fc_hoistedGlobal;
  uint32_T c2_fc_u;
  const mxArray *c2_gc_y = NULL;
  boolean_T *c2_AVReached;
  boolean_T *c2_Aget;
  boolean_T *c2_Refract;
  boolean_T *c2_Vget;
  c2_Refract = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c2_Vget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c2_AVReached = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c2_Aget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c2_st = NULL;
  c2_st = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_createcellmatrix(56, 1), false);
  c2_hoistedGlobal = chartInstance->c2_avj_refr;
  c2_u = c2_hoistedGlobal;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 0, c2_b_y);
  c2_b_hoistedGlobal = chartInstance->c2_avj_refr_0;
  c2_b_u = c2_b_hoistedGlobal;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 1, c2_c_y);
  c2_c_hoistedGlobal = chartInstance->c2_avj_t4;
  c2_c_u = c2_c_hoistedGlobal;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 2, c2_d_y);
  c2_d_hoistedGlobal = chartInstance->c2_avj_tRec;
  c2_d_u = c2_d_hoistedGlobal;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 3, c2_e_y);
  c2_e_hoistedGlobal = chartInstance->c2_clk;
  c2_e_u = c2_e_hoistedGlobal;
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_create("y", &c2_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 4, c2_f_y);
  c2_f_hoistedGlobal = chartInstance->c2_ctime;
  c2_f_u = c2_f_hoistedGlobal;
  c2_g_y = NULL;
  sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_f_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 5, c2_g_y);
  c2_g_hoistedGlobal = chartInstance->c2_recoverylast;
  c2_g_u = c2_g_hoistedGlobal;
  c2_h_y = NULL;
  sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_g_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 6, c2_h_y);
  c2_h_hoistedGlobal = chartInstance->c2_refractorylast;
  c2_h_u = c2_h_hoistedGlobal;
  c2_i_y = NULL;
  sf_mex_assign(&c2_i_y, sf_mex_create("y", &c2_h_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 7, c2_i_y);
  c2_i_hoistedGlobal = *c2_AVReached;
  c2_i_u = c2_i_hoistedGlobal;
  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_i_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 8, c2_j_y);
  c2_j_hoistedGlobal = *c2_Aget;
  c2_j_u = c2_j_hoistedGlobal;
  c2_k_y = NULL;
  sf_mex_assign(&c2_k_y, sf_mex_create("y", &c2_j_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 9, c2_k_y);
  c2_k_hoistedGlobal = *c2_Refract;
  c2_k_u = c2_k_hoistedGlobal;
  c2_l_y = NULL;
  sf_mex_assign(&c2_l_y, sf_mex_create("y", &c2_k_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 10, c2_l_y);
  c2_l_hoistedGlobal = *c2_Vget;
  c2_l_u = c2_l_hoistedGlobal;
  c2_m_y = NULL;
  sf_mex_assign(&c2_m_y, sf_mex_create("y", &c2_l_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 11, c2_m_y);
  c2_m_hoistedGlobal = chartInstance->c2_AVReachedEventCounter;
  c2_m_u = c2_m_hoistedGlobal;
  c2_n_y = NULL;
  sf_mex_assign(&c2_n_y, sf_mex_create("y", &c2_m_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 12, c2_n_y);
  c2_n_hoistedGlobal = chartInstance->c2_AgetEventCounter;
  c2_n_u = c2_n_hoistedGlobal;
  c2_o_y = NULL;
  sf_mex_assign(&c2_o_y, sf_mex_create("y", &c2_n_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 13, c2_o_y);
  c2_o_hoistedGlobal = chartInstance->c2_RefractEventCounter;
  c2_o_u = c2_o_hoistedGlobal;
  c2_p_y = NULL;
  sf_mex_assign(&c2_p_y, sf_mex_create("y", &c2_o_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 14, c2_p_y);
  c2_p_hoistedGlobal = chartInstance->c2_VgetEventCounter;
  c2_p_u = c2_p_hoistedGlobal;
  c2_q_y = NULL;
  sf_mex_assign(&c2_q_y, sf_mex_create("y", &c2_p_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 15, c2_q_y);
  c2_q_hoistedGlobal = chartInstance->c2_is_active_c2_HeartModelECG;
  c2_q_u = c2_q_hoistedGlobal;
  c2_r_y = NULL;
  sf_mex_assign(&c2_r_y, sf_mex_create("y", &c2_q_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 16, c2_r_y);
  c2_r_hoistedGlobal = chartInstance->c2_is_active_AtrNoiseGenerator;
  c2_r_u = c2_r_hoistedGlobal;
  c2_s_y = NULL;
  sf_mex_assign(&c2_s_y, sf_mex_create("y", &c2_r_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 17, c2_s_y);
  c2_s_hoistedGlobal = chartInstance->c2_is_active_SANode;
  c2_s_u = c2_s_hoistedGlobal;
  c2_t_y = NULL;
  sf_mex_assign(&c2_t_y, sf_mex_create("y", &c2_s_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 18, c2_t_y);
  c2_t_hoistedGlobal = chartInstance->c2_is_active_Atrium;
  c2_t_u = c2_t_hoistedGlobal;
  c2_u_y = NULL;
  sf_mex_assign(&c2_u_y, sf_mex_create("y", &c2_t_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 19, c2_u_y);
  c2_u_hoistedGlobal = chartInstance->c2_is_active_RAConductor;
  c2_u_u = c2_u_hoistedGlobal;
  c2_v_y = NULL;
  sf_mex_assign(&c2_v_y, sf_mex_create("y", &c2_u_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 20, c2_v_y);
  c2_v_hoistedGlobal = chartInstance->c2_is_active_SANodeEctopic;
  c2_v_u = c2_v_hoistedGlobal;
  c2_w_y = NULL;
  sf_mex_assign(&c2_w_y, sf_mex_create("y", &c2_v_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 21, c2_w_y);
  c2_w_hoistedGlobal = chartInstance->c2_is_active_RVConductor;
  c2_w_u = c2_w_hoistedGlobal;
  c2_x_y = NULL;
  sf_mex_assign(&c2_x_y, sf_mex_create("y", &c2_w_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 22, c2_x_y);
  c2_x_hoistedGlobal = chartInstance->c2_is_active_AVJOut;
  c2_x_u = c2_x_hoistedGlobal;
  c2_y_y = NULL;
  sf_mex_assign(&c2_y_y, sf_mex_create("y", &c2_x_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 23, c2_y_y);
  c2_y_hoistedGlobal = chartInstance->c2_is_active_Ventricle;
  c2_y_u = c2_y_hoistedGlobal;
  c2_ab_y = NULL;
  sf_mex_assign(&c2_ab_y, sf_mex_create("y", &c2_y_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 24, c2_ab_y);
  c2_ab_hoistedGlobal = chartInstance->c2_is_active_VRG;
  c2_ab_u = c2_ab_hoistedGlobal;
  c2_bb_y = NULL;
  sf_mex_assign(&c2_bb_y, sf_mex_create("y", &c2_ab_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 25, c2_bb_y);
  c2_bb_hoistedGlobal = chartInstance->c2_is_active_VRGEctopic;
  c2_bb_u = c2_bb_hoistedGlobal;
  c2_cb_y = NULL;
  sf_mex_assign(&c2_cb_y, sf_mex_create("y", &c2_bb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 26, c2_cb_y);
  c2_cb_hoistedGlobal = chartInstance->c2_is_active_VtrNoiseGenerator;
  c2_cb_u = c2_cb_hoistedGlobal;
  c2_db_y = NULL;
  sf_mex_assign(&c2_db_y, sf_mex_create("y", &c2_cb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 27, c2_db_y);
  c2_db_hoistedGlobal = chartInstance->c2_is_active_AVJ;
  c2_db_u = c2_db_hoistedGlobal;
  c2_eb_y = NULL;
  sf_mex_assign(&c2_eb_y, sf_mex_create("y", &c2_db_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 28, c2_eb_y);
  c2_eb_hoistedGlobal = chartInstance->c2_is_active_A_dV;
  c2_eb_u = c2_eb_hoistedGlobal;
  c2_fb_y = NULL;
  sf_mex_assign(&c2_fb_y, sf_mex_create("y", &c2_eb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 29, c2_fb_y);
  c2_fb_hoistedGlobal = chartInstance->c2_is_c2_HeartModelECG;
  c2_fb_u = c2_fb_hoistedGlobal;
  c2_gb_y = NULL;
  sf_mex_assign(&c2_gb_y, sf_mex_create("y", &c2_fb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 30, c2_gb_y);
  c2_gb_hoistedGlobal = chartInstance->c2_is_AtrNoiseGenerator;
  c2_gb_u = c2_gb_hoistedGlobal;
  c2_hb_y = NULL;
  sf_mex_assign(&c2_hb_y, sf_mex_create("y", &c2_gb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 31, c2_hb_y);
  c2_hb_hoistedGlobal = chartInstance->c2_is_SANode;
  c2_hb_u = c2_hb_hoistedGlobal;
  c2_ib_y = NULL;
  sf_mex_assign(&c2_ib_y, sf_mex_create("y", &c2_hb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 32, c2_ib_y);
  c2_ib_hoistedGlobal = chartInstance->c2_is_Atrium;
  c2_ib_u = c2_ib_hoistedGlobal;
  c2_jb_y = NULL;
  sf_mex_assign(&c2_jb_y, sf_mex_create("y", &c2_ib_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 33, c2_jb_y);
  c2_jb_hoistedGlobal = chartInstance->c2_is_RAConductor;
  c2_jb_u = c2_jb_hoistedGlobal;
  c2_kb_y = NULL;
  sf_mex_assign(&c2_kb_y, sf_mex_create("y", &c2_jb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 34, c2_kb_y);
  c2_kb_hoistedGlobal = chartInstance->c2_is_SANodeEctopic;
  c2_kb_u = c2_kb_hoistedGlobal;
  c2_lb_y = NULL;
  sf_mex_assign(&c2_lb_y, sf_mex_create("y", &c2_kb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 35, c2_lb_y);
  c2_lb_hoistedGlobal = chartInstance->c2_is_RVConductor;
  c2_lb_u = c2_lb_hoistedGlobal;
  c2_mb_y = NULL;
  sf_mex_assign(&c2_mb_y, sf_mex_create("y", &c2_lb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 36, c2_mb_y);
  c2_mb_hoistedGlobal = chartInstance->c2_is_AVJOut;
  c2_mb_u = c2_mb_hoistedGlobal;
  c2_nb_y = NULL;
  sf_mex_assign(&c2_nb_y, sf_mex_create("y", &c2_mb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 37, c2_nb_y);
  c2_nb_hoistedGlobal = chartInstance->c2_is_Ventricle;
  c2_nb_u = c2_nb_hoistedGlobal;
  c2_ob_y = NULL;
  sf_mex_assign(&c2_ob_y, sf_mex_create("y", &c2_nb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 38, c2_ob_y);
  c2_ob_hoistedGlobal = chartInstance->c2_is_VRG;
  c2_ob_u = c2_ob_hoistedGlobal;
  c2_pb_y = NULL;
  sf_mex_assign(&c2_pb_y, sf_mex_create("y", &c2_ob_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 39, c2_pb_y);
  c2_pb_hoistedGlobal = chartInstance->c2_is_VRGEctopic;
  c2_pb_u = c2_pb_hoistedGlobal;
  c2_qb_y = NULL;
  sf_mex_assign(&c2_qb_y, sf_mex_create("y", &c2_pb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 40, c2_qb_y);
  c2_qb_hoistedGlobal = chartInstance->c2_is_VtrNoiseGenerator;
  c2_qb_u = c2_qb_hoistedGlobal;
  c2_rb_y = NULL;
  sf_mex_assign(&c2_rb_y, sf_mex_create("y", &c2_qb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 41, c2_rb_y);
  c2_rb_hoistedGlobal = chartInstance->c2_is_AVJ;
  c2_rb_u = c2_rb_hoistedGlobal;
  c2_sb_y = NULL;
  sf_mex_assign(&c2_sb_y, sf_mex_create("y", &c2_rb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 42, c2_sb_y);
  c2_sb_hoistedGlobal = chartInstance->c2_is_A_dV;
  c2_sb_u = c2_sb_hoistedGlobal;
  c2_tb_y = NULL;
  sf_mex_assign(&c2_tb_y, sf_mex_create("y", &c2_sb_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 43, c2_tb_y);
  c2_tb_hoistedGlobal = chartInstance->c2_temporalCounter_i2;
  c2_tb_u = c2_tb_hoistedGlobal;
  c2_ub_y = NULL;
  sf_mex_assign(&c2_ub_y, sf_mex_create("y", &c2_tb_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 44, c2_ub_y);
  c2_ub_hoistedGlobal = chartInstance->c2_temporalCounter_i8;
  c2_ub_u = c2_ub_hoistedGlobal;
  c2_vb_y = NULL;
  sf_mex_assign(&c2_vb_y, sf_mex_create("y", &c2_ub_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 45, c2_vb_y);
  c2_vb_hoistedGlobal = chartInstance->c2_temporalCounter_i12;
  c2_vb_u = c2_vb_hoistedGlobal;
  c2_wb_y = NULL;
  sf_mex_assign(&c2_wb_y, sf_mex_create("y", &c2_vb_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 46, c2_wb_y);
  c2_wb_hoistedGlobal = chartInstance->c2_temporalCounter_i1;
  c2_wb_u = c2_wb_hoistedGlobal;
  c2_xb_y = NULL;
  sf_mex_assign(&c2_xb_y, sf_mex_create("y", &c2_wb_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 47, c2_xb_y);
  c2_xb_hoistedGlobal = chartInstance->c2_temporalCounter_i11;
  c2_xb_u = c2_xb_hoistedGlobal;
  c2_yb_y = NULL;
  sf_mex_assign(&c2_yb_y, sf_mex_create("y", &c2_xb_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 48, c2_yb_y);
  c2_yb_hoistedGlobal = chartInstance->c2_temporalCounter_i9;
  c2_yb_u = c2_yb_hoistedGlobal;
  c2_ac_y = NULL;
  sf_mex_assign(&c2_ac_y, sf_mex_create("y", &c2_yb_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 49, c2_ac_y);
  c2_ac_hoistedGlobal = chartInstance->c2_temporalCounter_i7;
  c2_ac_u = c2_ac_hoistedGlobal;
  c2_bc_y = NULL;
  sf_mex_assign(&c2_bc_y, sf_mex_create("y", &c2_ac_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 50, c2_bc_y);
  c2_bc_hoistedGlobal = chartInstance->c2_temporalCounter_i5;
  c2_bc_u = c2_bc_hoistedGlobal;
  c2_cc_y = NULL;
  sf_mex_assign(&c2_cc_y, sf_mex_create("y", &c2_bc_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 51, c2_cc_y);
  c2_cc_hoistedGlobal = chartInstance->c2_temporalCounter_i3;
  c2_cc_u = c2_cc_hoistedGlobal;
  c2_dc_y = NULL;
  sf_mex_assign(&c2_dc_y, sf_mex_create("y", &c2_cc_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 52, c2_dc_y);
  c2_dc_hoistedGlobal = chartInstance->c2_temporalCounter_i6;
  c2_dc_u = c2_dc_hoistedGlobal;
  c2_ec_y = NULL;
  sf_mex_assign(&c2_ec_y, sf_mex_create("y", &c2_dc_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 53, c2_ec_y);
  c2_ec_hoistedGlobal = chartInstance->c2_temporalCounter_i4;
  c2_ec_u = c2_ec_hoistedGlobal;
  c2_fc_y = NULL;
  sf_mex_assign(&c2_fc_y, sf_mex_create("y", &c2_ec_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 54, c2_fc_y);
  c2_fc_hoistedGlobal = chartInstance->c2_temporalCounter_i10;
  c2_fc_u = c2_fc_hoistedGlobal;
  c2_gc_y = NULL;
  sf_mex_assign(&c2_gc_y, sf_mex_create("y", &c2_fc_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c2_y, 55, c2_gc_y);
  sf_mex_assign(&c2_st, c2_y, false);
  return c2_st;
}

static void set_sim_state_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_st)
{
  const mxArray *c2_u;
  boolean_T *c2_AVReached;
  boolean_T *c2_Aget;
  boolean_T *c2_Refract;
  boolean_T *c2_Vget;
  c2_Refract = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c2_Vget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c2_AVReached = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c2_Aget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c2_u = sf_mex_dup(c2_st);
  chartInstance->c2_avj_refr = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 0)), "avj_refr");
  chartInstance->c2_avj_refr_0 = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 1)), "avj_refr_0");
  chartInstance->c2_avj_t4 = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 2)), "avj_t4");
  chartInstance->c2_avj_tRec = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 3)), "avj_tRec");
  chartInstance->c2_clk = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 4)), "clk");
  chartInstance->c2_ctime = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 5)), "ctime");
  chartInstance->c2_recoverylast = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 6)), "recoverylast");
  chartInstance->c2_refractorylast = c2_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 7)), "refractorylast");
  *c2_AVReached = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c2_u, 8)), "AVReached");
  *c2_Aget = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u,
    9)), "Aget");
  *c2_Refract = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c2_u, 10)), "Refract");
  *c2_Vget = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c2_u,
    11)), "Vget");
  chartInstance->c2_AVReachedEventCounter = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 12)), "AVReachedEventCounter");
  chartInstance->c2_AgetEventCounter = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 13)), "AgetEventCounter");
  chartInstance->c2_RefractEventCounter = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 14)), "RefractEventCounter");
  chartInstance->c2_VgetEventCounter = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 15)), "VgetEventCounter");
  chartInstance->c2_is_active_c2_HeartModelECG = c2_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 16)),
     "is_active_c2_HeartModelECG");
  chartInstance->c2_is_active_AtrNoiseGenerator = c2_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 17)),
     "is_active_AtrNoiseGenerator");
  chartInstance->c2_is_active_SANode = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 18)), "is_active_SANode");
  chartInstance->c2_is_active_Atrium = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 19)), "is_active_Atrium");
  chartInstance->c2_is_active_RAConductor = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 20)), "is_active_RAConductor");
  chartInstance->c2_is_active_SANodeEctopic = c2_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 21)),
     "is_active_SANodeEctopic");
  chartInstance->c2_is_active_RVConductor = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 22)), "is_active_RVConductor");
  chartInstance->c2_is_active_AVJOut = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 23)), "is_active_AVJOut");
  chartInstance->c2_is_active_Ventricle = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 24)), "is_active_Ventricle");
  chartInstance->c2_is_active_VRG = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 25)), "is_active_VRG");
  chartInstance->c2_is_active_VRGEctopic = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 26)), "is_active_VRGEctopic");
  chartInstance->c2_is_active_VtrNoiseGenerator = c2_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c2_u, 27)),
     "is_active_VtrNoiseGenerator");
  chartInstance->c2_is_active_AVJ = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 28)), "is_active_AVJ");
  chartInstance->c2_is_active_A_dV = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 29)), "is_active_A_dV");
  chartInstance->c2_is_c2_HeartModelECG = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 30)), "is_c2_HeartModelECG");
  chartInstance->c2_is_AtrNoiseGenerator = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 31)), "is_AtrNoiseGenerator");
  chartInstance->c2_is_SANode = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 32)), "is_SANode");
  chartInstance->c2_is_Atrium = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 33)), "is_Atrium");
  chartInstance->c2_is_RAConductor = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 34)), "is_RAConductor");
  chartInstance->c2_is_SANodeEctopic = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 35)), "is_SANodeEctopic");
  chartInstance->c2_is_RVConductor = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 36)), "is_RVConductor");
  chartInstance->c2_is_AVJOut = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 37)), "is_AVJOut");
  chartInstance->c2_is_Ventricle = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 38)), "is_Ventricle");
  chartInstance->c2_is_VRG = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 39)), "is_VRG");
  chartInstance->c2_is_VRGEctopic = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 40)), "is_VRGEctopic");
  chartInstance->c2_is_VtrNoiseGenerator = c2_f_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 41)), "is_VtrNoiseGenerator");
  chartInstance->c2_is_AVJ = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 42)), "is_AVJ");
  chartInstance->c2_is_A_dV = c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_u, 43)), "is_A_dV");
  chartInstance->c2_temporalCounter_i2 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 44)), "temporalCounter_i2");
  chartInstance->c2_temporalCounter_i8 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 45)), "temporalCounter_i8");
  chartInstance->c2_temporalCounter_i12 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 46)), "temporalCounter_i12");
  chartInstance->c2_temporalCounter_i1 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 47)), "temporalCounter_i1");
  chartInstance->c2_temporalCounter_i11 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 48)), "temporalCounter_i11");
  chartInstance->c2_temporalCounter_i9 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 49)), "temporalCounter_i9");
  chartInstance->c2_temporalCounter_i7 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 50)), "temporalCounter_i7");
  chartInstance->c2_temporalCounter_i5 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 51)), "temporalCounter_i5");
  chartInstance->c2_temporalCounter_i3 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 52)), "temporalCounter_i3");
  chartInstance->c2_temporalCounter_i6 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 53)), "temporalCounter_i6");
  chartInstance->c2_temporalCounter_i4 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 54)), "temporalCounter_i4");
  chartInstance->c2_temporalCounter_i10 = c2_c_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c2_u, 55)), "temporalCounter_i10");
  sf_mex_assign(&chartInstance->c2_setSimStateSideEffectsInfo,
                c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c2_u, 56)), "setSimStateSideEffectsInfo"), true);
  sf_mex_destroy(&c2_u);
  chartInstance->c2_doSetSimStateSideEffects = 1U;
  c2_update_debugger_state_c2_HeartModelECG(chartInstance);
  sf_mex_destroy(&c2_st);
}

static void c2_set_sim_state_side_effects_c2_HeartModelECG
  (SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  if (chartInstance->c2_doSetSimStateSideEffects != 0) {
    if (chartInstance->c2_is_c2_HeartModelECG == c2_IN_Heart) {
      chartInstance->c2_tp_Heart = 1U;
    } else {
      chartInstance->c2_tp_Heart = 0U;
    }

    if (chartInstance->c2_is_active_AtrNoiseGenerator == 1U) {
      chartInstance->c2_tp_AtrNoiseGenerator = 1U;
    } else {
      chartInstance->c2_tp_AtrNoiseGenerator = 0U;
    }

    if (chartInstance->c2_is_AtrNoiseGenerator == c2_IN_q0) {
      chartInstance->c2_b_tp_q0 = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 4) == 0.0) {
        chartInstance->c2_temporalCounter_i2 = 0U;
      }
    } else {
      chartInstance->c2_b_tp_q0 = 0U;
    }

    if (chartInstance->c2_is_active_SANodeEctopic == 1U) {
      chartInstance->c2_tp_SANodeEctopic = 1U;
    } else {
      chartInstance->c2_tp_SANodeEctopic = 0U;
    }

    if (chartInstance->c2_is_SANodeEctopic == c2_IN_q0) {
      chartInstance->c2_tp_q0 = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 6) == 0.0) {
        chartInstance->c2_temporalCounter_i1 = 0U;
      }
    } else {
      chartInstance->c2_tp_q0 = 0U;
    }

    if (chartInstance->c2_is_active_VtrNoiseGenerator == 1U) {
      chartInstance->c2_tp_VtrNoiseGenerator = 1U;
    } else {
      chartInstance->c2_tp_VtrNoiseGenerator = 0U;
    }

    if (chartInstance->c2_is_VtrNoiseGenerator == c2_IN_q0) {
      chartInstance->c2_d_tp_q0 = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 8) == 0.0) {
        chartInstance->c2_temporalCounter_i7 = 0U;
      }
    } else {
      chartInstance->c2_d_tp_q0 = 0U;
    }

    if (chartInstance->c2_is_active_AVJOut == 1U) {
      chartInstance->c2_tp_AVJOut = 1U;
    } else {
      chartInstance->c2_tp_AVJOut = 0U;
    }

    if (chartInstance->c2_is_AVJOut == c2_IN_Ante) {
      chartInstance->c2_b_tp_Ante = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 10) == 0.0) {
        chartInstance->c2_temporalCounter_i4 = 0U;
      }
    } else {
      chartInstance->c2_b_tp_Ante = 0U;
    }

    if (chartInstance->c2_is_AVJOut == c2_IN_Empty) {
      chartInstance->c2_b_tp_Empty = 1U;
    } else {
      chartInstance->c2_b_tp_Empty = 0U;
    }

    if (chartInstance->c2_is_AVJOut == c2_IN_Retro) {
      chartInstance->c2_b_tp_Retro = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 12) == 0.0) {
        chartInstance->c2_temporalCounter_i4 = 0U;
      }
    } else {
      chartInstance->c2_b_tp_Retro = 0U;
    }

    if (chartInstance->c2_is_active_RVConductor == 1U) {
      chartInstance->c2_tp_RVConductor = 1U;
    } else {
      chartInstance->c2_tp_RVConductor = 0U;
    }

    if (chartInstance->c2_is_RVConductor == c2_IN_Ante) {
      chartInstance->c2_c_tp_Ante = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 14) == 0.0) {
        chartInstance->c2_temporalCounter_i6 = 0U;
      }
    } else {
      chartInstance->c2_c_tp_Ante = 0U;
    }

    if (chartInstance->c2_is_RVConductor == c2_IN_Empty) {
      chartInstance->c2_c_tp_Empty = 1U;
    } else {
      chartInstance->c2_c_tp_Empty = 0U;
    }

    if (chartInstance->c2_is_RVConductor == c2_IN_Retro) {
      chartInstance->c2_c_tp_Retro = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 16) == 0.0) {
        chartInstance->c2_temporalCounter_i6 = 0U;
      }
    } else {
      chartInstance->c2_c_tp_Retro = 0U;
    }

    if (chartInstance->c2_is_active_RAConductor == 1U) {
      chartInstance->c2_tp_RAConductor = 1U;
    } else {
      chartInstance->c2_tp_RAConductor = 0U;
    }

    if (chartInstance->c2_is_RAConductor == c2_IN_Ante) {
      chartInstance->c2_tp_Ante = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 18) == 0.0) {
        chartInstance->c2_temporalCounter_i3 = 0U;
      }
    } else {
      chartInstance->c2_tp_Ante = 0U;
    }

    if (chartInstance->c2_is_RAConductor == c2_IN_Empty) {
      chartInstance->c2_tp_Empty = 1U;
    } else {
      chartInstance->c2_tp_Empty = 0U;
    }

    if (chartInstance->c2_is_RAConductor == c2_IN_Retro) {
      chartInstance->c2_tp_Retro = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 20) == 0.0) {
        chartInstance->c2_temporalCounter_i3 = 0U;
      }
    } else {
      chartInstance->c2_tp_Retro = 0U;
    }

    if (chartInstance->c2_is_active_SANode == 1U) {
      chartInstance->c2_tp_SANode = 1U;
    } else {
      chartInstance->c2_tp_SANode = 0U;
    }

    if (chartInstance->c2_is_SANode == c2_IN_Wait4ABeat) {
      chartInstance->c2_tp_Wait4ABeat = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 22) == 0.0) {
        chartInstance->c2_temporalCounter_i8 = 0U;
      }
    } else {
      chartInstance->c2_tp_Wait4ABeat = 0U;
    }

    if (chartInstance->c2_is_SANode == c2_IN_Wait4ASynch) {
      chartInstance->c2_tp_Wait4ASynch = 1U;
    } else {
      chartInstance->c2_tp_Wait4ASynch = 0U;
    }

    if (chartInstance->c2_is_active_Ventricle == 1U) {
      chartInstance->c2_tp_Ventricle = 1U;
    } else {
      chartInstance->c2_tp_Ventricle = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_Excitable) {
      chartInstance->c2_tp_Excitable = 1U;
    } else {
      chartInstance->c2_tp_Excitable = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_GenerateRetroWave) {
      chartInstance->c2_tp_GenerateRetroWave = 1U;
    } else {
      chartInstance->c2_tp_GenerateRetroWave = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_Refractory) {
      chartInstance->c2_b_tp_Refractory = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 27) == 0.0) {
        chartInstance->c2_temporalCounter_i11 = 0U;
      }
    } else {
      chartInstance->c2_b_tp_Refractory = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_VSynch) {
      chartInstance->c2_tp_VSynch = 1U;
    } else {
      chartInstance->c2_tp_VSynch = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_Vsense) {
      chartInstance->c2_tp_Vsense = 1U;
    } else {
      chartInstance->c2_tp_Vsense = 0U;
    }

    if (chartInstance->c2_is_Ventricle == c2_IN_VsenseAnte) {
      chartInstance->c2_tp_VsenseAnte = 1U;
    } else {
      chartInstance->c2_tp_VsenseAnte = 0U;
    }

    if (chartInstance->c2_is_active_Atrium == 1U) {
      chartInstance->c2_tp_Atrium = 1U;
    } else {
      chartInstance->c2_tp_Atrium = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_IN_ASynch) {
      chartInstance->c2_tp_ASynch = 1U;
    } else {
      chartInstance->c2_tp_ASynch = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_IN_Asense) {
      chartInstance->c2_tp_Asense = 1U;
    } else {
      chartInstance->c2_tp_Asense = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_IN_AsenseRetro) {
      chartInstance->c2_tp_AsenseRetro = 1U;
    } else {
      chartInstance->c2_tp_AsenseRetro = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_b_IN_Excitable) {
      chartInstance->c2_b_tp_Excitable = 1U;
    } else {
      chartInstance->c2_b_tp_Excitable = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_IN_GenerateAnteWave) {
      chartInstance->c2_tp_GenerateAnteWave = 1U;
    } else {
      chartInstance->c2_tp_GenerateAnteWave = 0U;
    }

    if (chartInstance->c2_is_Atrium == c2_b_IN_Refractory) {
      chartInstance->c2_c_tp_Refractory = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 37) == 0.0) {
        chartInstance->c2_temporalCounter_i12 = 0U;
      }
    } else {
      chartInstance->c2_c_tp_Refractory = 0U;
    }

    if (chartInstance->c2_is_active_VRGEctopic == 1U) {
      chartInstance->c2_tp_VRGEctopic = 1U;
    } else {
      chartInstance->c2_tp_VRGEctopic = 0U;
    }

    if (chartInstance->c2_is_VRGEctopic == c2_IN_q0) {
      chartInstance->c2_c_tp_q0 = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 39) == 0.0) {
        chartInstance->c2_temporalCounter_i5 = 0U;
      }
    } else {
      chartInstance->c2_c_tp_q0 = 0U;
    }

    if (chartInstance->c2_is_active_VRG == 1U) {
      chartInstance->c2_tp_VRG = 1U;
    } else {
      chartInstance->c2_tp_VRG = 0U;
    }

    if (chartInstance->c2_is_VRG == c2_IN_Wait4VBeat) {
      chartInstance->c2_tp_Wait4VBeat = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 41) == 0.0) {
        chartInstance->c2_temporalCounter_i9 = 0U;
      }
    } else {
      chartInstance->c2_tp_Wait4VBeat = 0U;
    }

    if (chartInstance->c2_is_VRG == c2_IN_Wait4VSynch) {
      chartInstance->c2_tp_Wait4VSynch = 1U;
    } else {
      chartInstance->c2_tp_Wait4VSynch = 0U;
    }

    if (chartInstance->c2_is_active_AVJ == 1U) {
      chartInstance->c2_tp_AVJ = 1U;
    } else {
      chartInstance->c2_tp_AVJ = 0U;
    }

    if (chartInstance->c2_is_AVJ == c2_IN_Ante) {
      chartInstance->c2_d_tp_Ante = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 44) == 0.0) {
        chartInstance->c2_temporalCounter_i10 = 0U;
      }
    } else {
      chartInstance->c2_d_tp_Ante = 0U;
    }

    if (chartInstance->c2_is_AVJ == c2_IN_Recovery) {
      chartInstance->c2_tp_Recovery = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 45) == 0.0) {
        chartInstance->c2_temporalCounter_i10 = 0U;
      }
    } else {
      chartInstance->c2_tp_Recovery = 0U;
    }

    if (chartInstance->c2_is_AVJ == c2_IN_Refractory) {
      chartInstance->c2_tp_Refractory = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 46) == 0.0) {
        chartInstance->c2_temporalCounter_i10 = 0U;
      }
    } else {
      chartInstance->c2_tp_Refractory = 0U;
    }

    if (chartInstance->c2_is_AVJ == c2_b_IN_Retro) {
      chartInstance->c2_d_tp_Retro = 1U;
      if (sf_mex_sub(chartInstance->c2_setSimStateSideEffectsInfo,
                     "setSimStateSideEffectsInfo", 1, 47) == 0.0) {
        chartInstance->c2_temporalCounter_i10 = 0U;
      }
    } else {
      chartInstance->c2_d_tp_Retro = 0U;
    }

    if (chartInstance->c2_is_active_A_dV == 1U) {
      chartInstance->c2_tp_A_dV = 1U;
    } else {
      chartInstance->c2_tp_A_dV = 0U;
    }

    if (chartInstance->c2_is_A_dV == c2_IN_a_dVLow) {
      chartInstance->c2_tp_a_dVLow = 1U;
    } else {
      chartInstance->c2_tp_a_dVLow = 0U;
    }

    if (chartInstance->c2_is_A_dV == c2_IN_a_dvHigh) {
      chartInstance->c2_tp_a_dvHigh = 1U;
    } else {
      chartInstance->c2_tp_a_dvHigh = 0U;
    }

    chartInstance->c2_doSetSimStateSideEffects = 0U;
  }
}

static void finalize_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  sf_mex_destroy(&chartInstance->c2_setSimStateSideEffectsInfo);
}

static void sf_gateway_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  real_T *c2_SA_d;
  real_T *c2_QR_delay;
  real_T *c2_RT_delay;
  real_T *c2_PQ_delay;
  boolean_T *c2_Aget;
  boolean_T *c2_AVReached;
  boolean_T *c2_Vget;
  boolean_T *c2_Refract;
  c2_Refract = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 4);
  c2_Vget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c2_AVReached = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c2_Aget = (boolean_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c2_PQ_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c2_RT_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c2_QR_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c2_SA_d = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  c2_set_sim_state_side_effects_c2_HeartModelECG(chartInstance);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  if (chartInstance->c2_temporalCounter_i1 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i1++;
  }

  if (chartInstance->c2_temporalCounter_i2 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i2++;
  }

  if (chartInstance->c2_temporalCounter_i3 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i3++;
  }

  if (chartInstance->c2_temporalCounter_i4 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i4++;
  }

  if (chartInstance->c2_temporalCounter_i5 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i5++;
  }

  if (chartInstance->c2_temporalCounter_i6 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i6++;
  }

  if (chartInstance->c2_temporalCounter_i7 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i7++;
  }

  if (chartInstance->c2_temporalCounter_i8 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i8++;
  }

  if (chartInstance->c2_temporalCounter_i9 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i9++;
  }

  if (chartInstance->c2_temporalCounter_i10 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i10++;
  }

  if (chartInstance->c2_temporalCounter_i11 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i11++;
  }

  if (chartInstance->c2_temporalCounter_i12 < MAX_uint32_T) {
    chartInstance->c2_temporalCounter_i12++;
  }

  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_Atr_refrD, 0U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_alpha, 1U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_anteDMin, 2U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_beta, 3U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_delta, 4U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_k4, 5U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_refrDMin, 6U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_retroDMin, 7U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_tauC, 8U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_theta, 9U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_tr, 10U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_Vr, 11U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_AV_Vt, 12U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_RA_anteD, 13U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_RA_retroD, 14U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_RV_anteD, 15U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_RV_retroD, 16U);
  _SFD_DATA_RANGE_CHECK(*c2_SA_d, 17U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_SA_dV, 18U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_SA_ectopD, 19U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_SA_ectopdV, 20U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_SA_noiseD, 21U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_VRG_d, 22U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_VRG_ectopD, 23U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_VRG_noiseD, 24U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_Vtr_refrD, 25U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_tRec, 26U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_clk, 27U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TAVI, 28U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TLRI, 29U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TPVAB, 30U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TPVARP, 31U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TURI, 32U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_PM_TVRP, 33U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_recoverylast, 34U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_t4, 35U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr_0, 38U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
  _SFD_DATA_RANGE_CHECK(*c2_QR_delay, 40U);
  _SFD_DATA_RANGE_CHECK(*c2_RT_delay, 41U);
  _SFD_DATA_RANGE_CHECK(*c2_PQ_delay, 42U);
  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_c2_HeartModelECG(chartInstance);
  if (chartInstance->c2_AgetEventCounter > 0U) {
    *c2_Aget = !*c2_Aget;
    chartInstance->c2_AgetEventCounter--;
  }

  if (chartInstance->c2_AVReachedEventCounter > 0U) {
    *c2_AVReached = !*c2_AVReached;
    chartInstance->c2_AVReachedEventCounter--;
  }

  if (chartInstance->c2_VgetEventCounter > 0U) {
    *c2_Vget = !*c2_Vget;
    chartInstance->c2_VgetEventCounter--;
  }

  if (chartInstance->c2_RefractEventCounter > 0U) {
    *c2_Refract = !*c2_Refract;
    chartInstance->c2_RefractEventCounter--;
  }

  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_HeartModelECGMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c2_c2_HeartModelECG(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_active_c2_HeartModelECG == 0U) {
    _SFD_CC_CALL(CHART_ENTER_ENTRY_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_active_c2_HeartModelECG = 1U;
    _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_c2_HeartModelECG = c2_IN_Heart;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 0U, chartInstance->c2_sfEvent);
    chartInstance->c2_tp_Heart = 1U;
    c2_enter_internal_Heart(chartInstance);
  } else if (chartInstance->c2_is_c2_HeartModelECG != c2_IN_Heart) {
  } else {
    c2_Heart(chartInstance);
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
}

static void initSimStructsc2_HeartModelECG(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void c2_enter_internal_Heart(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  chartInstance->c2_is_active_AtrNoiseGenerator = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 18U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_AtrNoiseGenerator = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 14U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_AtrNoiseGenerator == c2_IN_q0) {
  } else {
    chartInstance->c2_is_AtrNoiseGenerator = c2_IN_q0;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i2 = 0U;
    chartInstance->c2_b_tp_q0 = 1U;
  }

  chartInstance->c2_is_active_SANodeEctopic = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 38U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_SANodeEctopic = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 18U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_SANodeEctopic == c2_IN_q0) {
  } else {
    chartInstance->c2_is_SANodeEctopic = c2_IN_q0;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i1 = 0U;
    chartInstance->c2_tp_q0 = 1U;
  }

  chartInstance->c2_is_active_VtrNoiseGenerator = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 52U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_VtrNoiseGenerator = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 42U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_VtrNoiseGenerator == c2_IN_q0) {
  } else {
    chartInstance->c2_is_VtrNoiseGenerator = c2_IN_q0;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i7 = 0U;
    chartInstance->c2_d_tp_q0 = 1U;
  }

  chartInstance->c2_is_active_AVJOut = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_AVJOut = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 27U, chartInstance->c2_sfEvent);
  chartInstance->c2_is_AVJOut = c2_IN_Empty;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
  chartInstance->c2_b_tp_Empty = 1U;
  chartInstance->c2_is_active_RVConductor = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_RVConductor = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
  chartInstance->c2_is_RVConductor = c2_IN_Empty;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
  chartInstance->c2_c_tp_Empty = 1U;
  chartInstance->c2_is_active_RAConductor = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 27U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_RAConductor = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
  chartInstance->c2_is_RAConductor = c2_IN_Empty;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_Empty = 1U;
  chartInstance->c2_is_active_SANode = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_SANode = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_SANode == c2_IN_Wait4ABeat) {
  } else {
    chartInstance->c2_is_SANode = c2_IN_Wait4ABeat;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i8 = 0U;
    chartInstance->c2_tp_Wait4ABeat = 1U;
  }

  chartInstance->c2_is_active_Ventricle = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 45U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_Ventricle = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_Ventricle == c2_IN_Refractory) {
  } else {
    chartInstance->c2_is_Ventricle = c2_IN_Refractory;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i11 = 0U;
    chartInstance->c2_b_tp_Refractory = 1U;
  }

  chartInstance->c2_is_active_Atrium = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_Atrium = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 15U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_Atrium == c2_b_IN_Refractory) {
  } else {
    chartInstance->c2_is_Atrium = c2_b_IN_Refractory;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i12 = 0U;
    chartInstance->c2_c_tp_Refractory = 1U;
  }

  chartInstance->c2_is_active_VRGEctopic = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 43U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_VRGEctopic = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_VRGEctopic == c2_IN_q0) {
  } else {
    chartInstance->c2_is_VRGEctopic = c2_IN_q0;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i5 = 0U;
    chartInstance->c2_c_tp_q0 = 1U;
  }

  chartInstance->c2_is_active_VRG = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 40U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_VRG = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 38U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_VRG == c2_IN_Wait4VBeat) {
  } else {
    chartInstance->c2_is_VRG = c2_IN_Wait4VBeat;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i9 = 0U;
    chartInstance->c2_tp_Wait4VBeat = 1U;
  }

  chartInstance->c2_is_active_AVJ = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_AVJ = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 60U, chartInstance->c2_sfEvent);
  chartInstance->c2_avj_t4 = c2_timeToDep(chartInstance, chartInstance->c2_AV_Vr);
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_t4, 35U);
  chartInstance->c2_ctime = 0.0;
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
  chartInstance->c2_recoverylast = 0.0;
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_recoverylast, 34U);
  chartInstance->c2_refractorylast = 0.0;
  _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
  if (chartInstance->c2_is_AVJ == c2_IN_Recovery) {
  } else {
    chartInstance->c2_is_AVJ = c2_IN_Recovery;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i10 = 0U;
    chartInstance->c2_tp_Recovery = 1U;
  }

  chartInstance->c2_is_active_A_dV = 1U;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 15U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_A_dV = 1U;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 69U, chartInstance->c2_sfEvent);
  chartInstance->c2_is_A_dV = c2_IN_a_dVLow;
  _SFD_CS_CALL(STATE_ACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
  chartInstance->c2_tp_a_dVLow = 1U;
}

static void c2_Heart(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_temp;
  boolean_T c2_out;
  boolean_T c2_b_temp;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  int32_T c2_b_previousEvent;
  boolean_T c2_c_temp;
  boolean_T c2_c_out;
  boolean_T c2_d_temp;
  boolean_T c2_d_out;
  int32_T c2_c_previousEvent;
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_active_AtrNoiseGenerator == 0U) {
  } else {
    _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 18U, chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_AtrNoiseGenerator != c2_IN_q0) {
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 0U,
                   chartInstance->c2_sfEvent);
      c2_temp = (_SFD_CCP_CALL(0U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
      if (c2_temp) {
        c2_temp = (_SFD_CCP_CALL(0U, 1, (real_T)
                    chartInstance->c2_temporalCounter_i2 >=
                    chartInstance->c2_SA_noiseD != 0U, chartInstance->c2_sfEvent)
                   != 0);
      }

      c2_out = (CV_TRANSITION_EVAL(0U, (int32_T)c2_temp) != 0);
      if (c2_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 0U, chartInstance->c2_sfEvent);
        chartInstance->c2_AgetEventCounter++;
        chartInstance->c2_b_tp_q0 = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AtrNoiseGenerator = c2_IN_q0;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i2 = 0U;
        chartInstance->c2_b_tp_q0 = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 19U,
                     chartInstance->c2_sfEvent);
      }

      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 19U, chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 18U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_SANodeEctopic == 0U) {
  } else {
    _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 38U, chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_SANodeEctopic != c2_IN_q0) {
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 17U,
                   chartInstance->c2_sfEvent);
      c2_b_temp = (_SFD_CCP_CALL(17U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_b_temp) {
        c2_b_temp = (_SFD_CCP_CALL(17U, 1, (real_T)
          chartInstance->c2_temporalCounter_i1 >= chartInstance->c2_SA_ectopD !=
          0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_b_out = (CV_TRANSITION_EVAL(17U, (int32_T)c2_b_temp) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_Abeat;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_Abeat,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Atrium == 0U) {
        } else {
          c2_Atrium(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_Abeat,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        c2_b_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_a_dVSetHigh;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_a_dVSetHigh,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_A_dV == 0U) {
        } else {
          c2_A_dV(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_a_dVSetHigh,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_b_previousEvent;
        chartInstance->c2_tp_q0 = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_SANodeEctopic = c2_IN_q0;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i1 = 0U;
        chartInstance->c2_tp_q0 = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 39U,
                     chartInstance->c2_sfEvent);
      }

      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 39U, chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 38U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_VtrNoiseGenerator == 0U) {
  } else {
    _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 52U, chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_VtrNoiseGenerator != c2_IN_q0) {
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 43U,
                   chartInstance->c2_sfEvent);
      c2_c_temp = (_SFD_CCP_CALL(43U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_c_temp) {
        c2_c_temp = (_SFD_CCP_CALL(43U, 1, (real_T)
          chartInstance->c2_temporalCounter_i7 >= chartInstance->c2_VRG_noiseD
          != 0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_c_out = (CV_TRANSITION_EVAL(43U, (int32_T)c2_c_temp) != 0);
      if (c2_c_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 43U, chartInstance->c2_sfEvent);
        chartInstance->c2_VgetEventCounter++;
        chartInstance->c2_d_tp_q0 = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_VtrNoiseGenerator = c2_IN_q0;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i7 = 0U;
        chartInstance->c2_d_tp_q0 = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 53U,
                     chartInstance->c2_sfEvent);
      }

      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 53U, chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 52U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_AVJOut == 0U) {
  } else {
    c2_AVJOut(chartInstance);
  }

  if (chartInstance->c2_is_active_RVConductor == 0U) {
  } else {
    c2_RVConductor(chartInstance);
  }

  if (chartInstance->c2_is_active_RAConductor == 0U) {
  } else {
    c2_RAConductor(chartInstance);
  }

  if (chartInstance->c2_is_active_SANode == 0U) {
  } else {
    c2_SANode(chartInstance);
  }

  if (chartInstance->c2_is_active_Ventricle == 0U) {
  } else {
    c2_Ventricle(chartInstance);
  }

  if (chartInstance->c2_is_active_Atrium == 0U) {
  } else {
    c2_Atrium(chartInstance);
  }

  if (chartInstance->c2_is_active_VRGEctopic == 0U) {
  } else {
    _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 43U, chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_VRGEctopic != c2_IN_q0) {
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 45U,
                   chartInstance->c2_sfEvent);
      c2_d_temp = (_SFD_CCP_CALL(45U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_d_temp) {
        c2_d_temp = (_SFD_CCP_CALL(45U, 1, (real_T)
          chartInstance->c2_temporalCounter_i5 >= chartInstance->c2_VRG_ectopD
          != 0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_d_out = (CV_TRANSITION_EVAL(45U, (int32_T)c2_d_temp) != 0);
      if (c2_d_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 45U, chartInstance->c2_sfEvent);
        c2_c_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_Vbeat;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_Vbeat,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Ventricle == 0U) {
        } else {
          c2_Ventricle(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_Vbeat,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_c_previousEvent;
        chartInstance->c2_c_tp_q0 = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_VRGEctopic = c2_IN_q0;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 44U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i5 = 0U;
        chartInstance->c2_c_tp_q0 = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 44U,
                     chartInstance->c2_sfEvent);
      }

      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 44U, chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 43U, chartInstance->c2_sfEvent);
  }

  if (chartInstance->c2_is_active_VRG == 0U) {
  } else {
    c2_VRG(chartInstance);
  }

  if (chartInstance->c2_is_active_AVJ == 0U) {
  } else {
    c2_AVJ(chartInstance);
  }

  if (chartInstance->c2_is_active_A_dV == 0U) {
  } else {
    c2_A_dV(chartInstance);
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
}

static void c2_A_dV(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  boolean_T c2_c_out;
  boolean_T c2_d_out;
  int32_T c2_b_previousEvent;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 15U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_A_dV) {
   case c2_IN_a_dVLow:
    CV_STATE_EVAL(15, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 63U,
                 chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(63U, (int32_T)_SFD_CCP_CALL(63U, 0,
                chartInstance->c2_sfEvent == c2_event_a_dVSetHigh != 0U,
                chartInstance->c2_sfEvent)) != 0);
    guard2 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 63U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_a_dVLow = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_A_dV = c2_IN_a_dvHigh;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_a_dvHigh = 1U;
      guard2 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 66U,
                   chartInstance->c2_sfEvent);
      c2_b_out = (CV_TRANSITION_EVAL(66U, (int32_T)_SFD_CCP_CALL(66U, 0,
        chartInstance->c2_sfEvent == c2_event_tr_AVJ != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 66U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_AVJAnteReached;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AVJAnteReached,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_AVJ == 0U) {
        } else {
          c2_AVJ(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AVJAnteReached,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        if (chartInstance->c2_is_A_dV != c2_IN_a_dVLow) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 66U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_AVReachedEventCounter++;
        }

        if (chartInstance->c2_is_A_dV != c2_IN_a_dVLow) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 66U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_a_dVLow = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_A_dV = c2_IN_a_dVLow;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_a_dVLow = 1U;
          guard2 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 16U,
                     chartInstance->c2_sfEvent);
        guard2 = true;
      }
    }

    if (guard2 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 16U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_a_dvHigh:
    CV_STATE_EVAL(15, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 64U,
                 chartInstance->c2_sfEvent);
    c2_c_out = (CV_TRANSITION_EVAL(64U, (int32_T)_SFD_CCP_CALL(64U, 0,
      chartInstance->c2_sfEvent == c2_event_a_dVSetLow != 0U,
      chartInstance->c2_sfEvent)) != 0);
    guard1 = false;
    if (c2_c_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 64U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_a_dvHigh = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_A_dV = c2_IN_a_dVLow;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_a_dVLow = 1U;
      guard1 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 65U,
                   chartInstance->c2_sfEvent);
      c2_d_out = (CV_TRANSITION_EVAL(65U, (int32_T)_SFD_CCP_CALL(65U, 0,
        chartInstance->c2_sfEvent == c2_event_tr_AVJ != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_d_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 65U, chartInstance->c2_sfEvent);
        c2_b_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_AVJAnteReachedHigh;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AVJAnteReachedHigh,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_AVJ == 0U) {
        } else {
          c2_AVJ(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AVJAnteReachedHigh,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_b_previousEvent;
        if (chartInstance->c2_is_A_dV != c2_IN_a_dvHigh) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 65U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_AVReachedEventCounter++;
        }

        if (chartInstance->c2_is_A_dV != c2_IN_a_dvHigh) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 65U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_a_dvHigh = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_A_dV = c2_IN_a_dvHigh;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 17U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_a_dvHigh = 1U;
          guard1 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 17U,
                     chartInstance->c2_sfEvent);
        guard1 = true;
      }
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 17U, chartInstance->c2_sfEvent);
    }
    break;

   default:
    CV_STATE_EVAL(15, 0, 0);
    chartInstance->c2_is_A_dV = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 16U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 15U, chartInstance->c2_sfEvent);
}

static void c2_RAConductor(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_temp;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  boolean_T c2_c_out;
  boolean_T c2_d_out;
  boolean_T c2_e_out;
  boolean_T c2_b_temp;
  boolean_T c2_f_out;
  int32_T c2_b_previousEvent;
  real_T *c2_PQ_delay;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  c2_PQ_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 27U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_RAConductor) {
   case c2_IN_Ante:
    CV_STATE_EVAL(27, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 9U, chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(9U, (int32_T)_SFD_CCP_CALL(9U, 0,
                chartInstance->c2_sfEvent == c2_event_retroAVJ != 0U,
                chartInstance->c2_sfEvent)) != 0);
    guard2 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 9U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Ante = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RAConductor = c2_IN_Empty;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Empty = 1U;
      guard2 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 10U,
                   chartInstance->c2_sfEvent);
      c2_temp = (_SFD_CCP_CALL(10U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
      if (c2_temp) {
        c2_temp = (_SFD_CCP_CALL(10U, 1, (real_T)
                    chartInstance->c2_temporalCounter_i3 >= *c2_PQ_delay != 0U,
                    chartInstance->c2_sfEvent) != 0);
      }

      c2_b_out = (CV_TRANSITION_EVAL(10U, (int32_T)c2_temp) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_tr_AVJ;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_tr_AVJ,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_A_dV == 0U) {
        } else {
          c2_A_dV(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_tr_AVJ,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        if (chartInstance->c2_is_RAConductor != c2_IN_Ante) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
        }

        if (chartInstance->c2_is_RAConductor != c2_IN_Ante) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 10U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_Ante = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_RAConductor = c2_IN_Empty;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Empty = 1U;
          guard2 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 28U,
                     chartInstance->c2_sfEvent);
        guard2 = true;
      }
    }

    if (guard2 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 28U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Empty:
    CV_STATE_EVAL(27, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 13U,
                 chartInstance->c2_sfEvent);
    c2_c_out = (CV_TRANSITION_EVAL(13U, (int32_T)_SFD_CCP_CALL(13U, 0,
      chartInstance->c2_sfEvent == c2_event_arg != 0U, chartInstance->c2_sfEvent))
                != 0);
    if (c2_c_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Empty = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RAConductor = c2_IN_Ante;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i3 = 0U;
      chartInstance->c2_tp_Ante = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 8U,
                   chartInstance->c2_sfEvent);
      c2_d_out = (CV_TRANSITION_EVAL(8U, (int32_T)_SFD_CCP_CALL(8U, 0,
        chartInstance->c2_sfEvent == c2_event_retroAVJ != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_d_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 8U, chartInstance->c2_sfEvent);
        chartInstance->c2_tp_Empty = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_RAConductor = c2_IN_Retro;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i3 = 0U;
        chartInstance->c2_tp_Retro = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 29U,
                     chartInstance->c2_sfEvent);
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 29U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_Retro:
    CV_STATE_EVAL(27, 0, 3);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 11U,
                 chartInstance->c2_sfEvent);
    c2_e_out = (CV_TRANSITION_EVAL(11U, (int32_T)_SFD_CCP_CALL(11U, 0,
      chartInstance->c2_sfEvent == c2_event_arg != 0U, chartInstance->c2_sfEvent))
                != 0);
    guard1 = false;
    if (c2_e_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 11U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Retro = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RAConductor = c2_IN_Empty;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Empty = 1U;
      guard1 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 12U,
                   chartInstance->c2_sfEvent);
      c2_b_temp = (_SFD_CCP_CALL(12U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_b_temp) {
        c2_b_temp = (_SFD_CCP_CALL(12U, 1, (real_T)
          chartInstance->c2_temporalCounter_i3 >= chartInstance->c2_RA_retroD !=
          0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_f_out = (CV_TRANSITION_EVAL(12U, (int32_T)c2_b_temp) != 0);
      if (c2_f_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
        c2_b_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_AtrRetroReached;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AtrRetroReached,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Atrium == 0U) {
        } else {
          c2_Atrium(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AtrRetroReached,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_b_previousEvent;
        if (chartInstance->c2_is_RAConductor != c2_IN_Retro) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
        }

        if (chartInstance->c2_is_RAConductor != c2_IN_Retro) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_Retro = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_RAConductor = c2_IN_Empty;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Empty = 1U;
          guard1 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 30U,
                     chartInstance->c2_sfEvent);
        guard1 = true;
      }
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 30U, chartInstance->c2_sfEvent);
    }
    break;

   default:
    CV_STATE_EVAL(27, 0, 0);
    chartInstance->c2_is_RAConductor = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 27U, chartInstance->c2_sfEvent);
}

static void c2_AVJOut(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_temp;
  boolean_T c2_out;
  int32_T c2_previousEvent;
  boolean_T c2_b_out;
  boolean_T c2_c_out;
  boolean_T c2_b_temp;
  boolean_T c2_d_out;
  int32_T c2_b_previousEvent;
  real_T *c2_QR_delay;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  c2_QR_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 10U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_AVJOut) {
   case c2_IN_Ante:
    CV_STATE_EVAL(10, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 29U,
                 chartInstance->c2_sfEvent);
    c2_temp = (_SFD_CCP_CALL(29U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
    if (c2_temp) {
      c2_temp = (_SFD_CCP_CALL(29U, 1, (real_T)
                  chartInstance->c2_temporalCounter_i4 >= c2_AVJDelay
                  (chartInstance, chartInstance->c2_avj_tRec, *c2_QR_delay *
                   0.666) != 0U, chartInstance->c2_sfEvent) != 0);
    }

    c2_out = (CV_TRANSITION_EVAL(29U, (int32_T)c2_temp) != 0);
    guard2 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      c2_previousEvent = chartInstance->c2_sfEvent;
      chartInstance->c2_sfEvent = c2_event_anteAVJ;
      _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_anteAVJ,
                   chartInstance->c2_sfEvent);
      if (chartInstance->c2_is_active_RVConductor == 0U) {
      } else {
        c2_RVConductor(chartInstance);
      }

      _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_anteAVJ,
                   chartInstance->c2_sfEvent);
      chartInstance->c2_sfEvent = c2_previousEvent;
      if (chartInstance->c2_is_AVJOut != c2_IN_Ante) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      }

      if (chartInstance->c2_is_AVJOut != c2_IN_Ante) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 29U, chartInstance->c2_sfEvent);
      } else {
        chartInstance->c2_b_tp_Ante = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJOut = c2_IN_Empty;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
        chartInstance->c2_b_tp_Empty = 1U;
        guard2 = true;
      }
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 12U,
                   chartInstance->c2_sfEvent);
      guard2 = true;
    }

    if (guard2 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 12U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Empty:
    CV_STATE_EVAL(10, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 28U,
                 chartInstance->c2_sfEvent);
    c2_b_out = (CV_TRANSITION_EVAL(28U, (int32_T)_SFD_CCP_CALL(28U, 0,
      chartInstance->c2_sfEvent == c2_event_AVJAnteExit != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_b_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 28U, chartInstance->c2_sfEvent);
      chartInstance->c2_b_tp_Empty = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_AVJOut = c2_IN_Ante;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i4 = 0U;
      chartInstance->c2_b_tp_Ante = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 30U,
                   chartInstance->c2_sfEvent);
      c2_c_out = (CV_TRANSITION_EVAL(30U, (int32_T)_SFD_CCP_CALL(30U, 0,
        chartInstance->c2_sfEvent == c2_event_AVJRetroExit != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_c_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 30U, chartInstance->c2_sfEvent);
        chartInstance->c2_b_tp_Empty = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJOut = c2_IN_Retro;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 14U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i4 = 0U;
        chartInstance->c2_b_tp_Retro = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 13U,
                     chartInstance->c2_sfEvent);
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 13U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_Retro:
    CV_STATE_EVAL(10, 0, 3);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 31U,
                 chartInstance->c2_sfEvent);
    c2_b_temp = (_SFD_CCP_CALL(31U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
    if (c2_b_temp) {
      c2_b_temp = (_SFD_CCP_CALL(31U, 1, (real_T)
        chartInstance->c2_temporalCounter_i4 >= c2_AVJDelay(chartInstance,
        chartInstance->c2_avj_tRec, chartInstance->c2_AV_retroDMin) != 0U,
        chartInstance->c2_sfEvent) != 0);
    }

    c2_d_out = (CV_TRANSITION_EVAL(31U, (int32_T)c2_b_temp) != 0);
    guard1 = false;
    if (c2_d_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
      c2_b_previousEvent = chartInstance->c2_sfEvent;
      chartInstance->c2_sfEvent = c2_event_retroAVJ;
      _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_retroAVJ,
                   chartInstance->c2_sfEvent);
      if (chartInstance->c2_is_active_RAConductor == 0U) {
      } else {
        c2_RAConductor(chartInstance);
      }

      _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_retroAVJ,
                   chartInstance->c2_sfEvent);
      chartInstance->c2_sfEvent = c2_b_previousEvent;
      if (chartInstance->c2_is_AVJOut != c2_IN_Retro) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
      }

      if (chartInstance->c2_is_AVJOut != c2_IN_Retro) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 31U, chartInstance->c2_sfEvent);
      } else {
        chartInstance->c2_b_tp_Retro = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 14U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJOut = c2_IN_Empty;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 13U, chartInstance->c2_sfEvent);
        chartInstance->c2_b_tp_Empty = 1U;
        guard1 = true;
      }
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 14U,
                   chartInstance->c2_sfEvent);
      guard1 = true;
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 14U, chartInstance->c2_sfEvent);
    }
    break;

   default:
    CV_STATE_EVAL(10, 0, 0);
    chartInstance->c2_is_AVJOut = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 12U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 10U, chartInstance->c2_sfEvent);
}

static void c2_RVConductor(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_temp;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  boolean_T c2_c_out;
  boolean_T c2_d_out;
  boolean_T c2_e_out;
  boolean_T c2_b_temp;
  boolean_T c2_f_out;
  int32_T c2_b_previousEvent;
  real_T *c2_QR_delay;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  c2_QR_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 31U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_RVConductor) {
   case c2_IN_Ante:
    CV_STATE_EVAL(31, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 52U,
                 chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(52U, (int32_T)_SFD_CCP_CALL(52U, 0,
                chartInstance->c2_sfEvent == c2_event_vrg != 0U,
                chartInstance->c2_sfEvent)) != 0);
    guard2 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 52U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Ante = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RVConductor = c2_IN_Empty;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Empty = 1U;
      guard2 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 22U,
                   chartInstance->c2_sfEvent);
      c2_temp = (_SFD_CCP_CALL(22U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
      if (c2_temp) {
        c2_temp = (_SFD_CCP_CALL(22U, 1, (real_T)
                    chartInstance->c2_temporalCounter_i6 >= *c2_QR_delay / 3.0
                    != 0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_b_out = (CV_TRANSITION_EVAL(22U, (int32_T)c2_temp) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_VtrAnteReached;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_VtrAnteReached,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Ventricle == 0U) {
        } else {
          c2_Ventricle(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_VtrAnteReached,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        if (chartInstance->c2_is_RVConductor != c2_IN_Ante) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
        }

        if (chartInstance->c2_is_RVConductor != c2_IN_Ante) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_c_tp_Ante = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_RVConductor = c2_IN_Empty;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
          chartInstance->c2_c_tp_Empty = 1U;
          guard2 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 32U,
                     chartInstance->c2_sfEvent);
        guard2 = true;
      }
    }

    if (guard2 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 32U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Empty:
    CV_STATE_EVAL(31, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 21U,
                 chartInstance->c2_sfEvent);
    c2_c_out = (CV_TRANSITION_EVAL(21U, (int32_T)_SFD_CCP_CALL(21U, 0,
      chartInstance->c2_sfEvent == c2_event_anteAVJ != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_c_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Empty = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RVConductor = c2_IN_Ante;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i6 = 0U;
      chartInstance->c2_c_tp_Ante = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 25U,
                   chartInstance->c2_sfEvent);
      c2_d_out = (CV_TRANSITION_EVAL(25U, (int32_T)_SFD_CCP_CALL(25U, 0,
        chartInstance->c2_sfEvent == c2_event_vrg != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_d_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
        chartInstance->c2_c_tp_Empty = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_RVConductor = c2_IN_Retro;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i6 = 0U;
        chartInstance->c2_c_tp_Retro = 1U;
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 33U,
                     chartInstance->c2_sfEvent);
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 33U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_Retro:
    CV_STATE_EVAL(31, 0, 3);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 24U,
                 chartInstance->c2_sfEvent);
    c2_e_out = (CV_TRANSITION_EVAL(24U, (int32_T)_SFD_CCP_CALL(24U, 0,
      chartInstance->c2_sfEvent == c2_event_anteAVJ != 0U,
      chartInstance->c2_sfEvent)) != 0);
    guard1 = false;
    if (c2_e_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Retro = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_RVConductor = c2_IN_Empty;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Empty = 1U;
      guard1 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 23U,
                   chartInstance->c2_sfEvent);
      c2_b_temp = (_SFD_CCP_CALL(23U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_b_temp) {
        c2_b_temp = (_SFD_CCP_CALL(23U, 1, (real_T)
          chartInstance->c2_temporalCounter_i6 >= chartInstance->c2_RV_retroD !=
          0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_f_out = (CV_TRANSITION_EVAL(23U, (int32_T)c2_b_temp) != 0);
      if (c2_f_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
        c2_b_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_AVJRetroReached;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AVJRetroReached,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_AVJ == 0U) {
        } else {
          c2_AVJ(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AVJRetroReached,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_b_previousEvent;
        if (chartInstance->c2_is_RVConductor != c2_IN_Retro) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
        }

        if (chartInstance->c2_is_RVConductor != c2_IN_Retro) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_c_tp_Retro = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_RVConductor = c2_IN_Empty;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
          chartInstance->c2_c_tp_Empty = 1U;
          guard1 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 34U,
                     chartInstance->c2_sfEvent);
        guard1 = true;
      }
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 34U, chartInstance->c2_sfEvent);
    }
    break;

   default:
    CV_STATE_EVAL(31, 0, 0);
    chartInstance->c2_is_RVConductor = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 31U, chartInstance->c2_sfEvent);
}

static void c2_SANode(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_temp;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  int32_T c2_b_previousEvent;
  boolean_T c2_c_out;
  real_T *c2_SA_d;
  boolean_T guard1 = false;
  c2_SA_d = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 35U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_SANode) {
   case c2_IN_Wait4ABeat:
    CV_STATE_EVAL(35, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 51U,
                 chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(51U, (int32_T)_SFD_CCP_CALL(51U, 0,
                chartInstance->c2_sfEvent == c2_event_NextAtrBeat != 0U,
                chartInstance->c2_sfEvent)) != 0);
    guard1 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 51U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Wait4ABeat = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_SANode = c2_IN_Wait4ABeat;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i8 = 0U;
      chartInstance->c2_tp_Wait4ABeat = 1U;
      guard1 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 1U,
                   chartInstance->c2_sfEvent);
      c2_temp = (_SFD_CCP_CALL(1U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
      if (c2_temp) {
        c2_temp = (_SFD_CCP_CALL(1U, 1, (real_T)
                    chartInstance->c2_temporalCounter_i8 >= *c2_SA_d != 0U,
                    chartInstance->c2_sfEvent) != 0);
      }

      c2_b_out = (CV_TRANSITION_EVAL(1U, (int32_T)c2_temp) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_Abeat;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_Abeat,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Atrium == 0U) {
        } else {
          c2_Atrium(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_Abeat,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        if (chartInstance->c2_is_SANode != c2_IN_Wait4ABeat) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
        } else {
          c2_b_previousEvent = chartInstance->c2_sfEvent;
          chartInstance->c2_sfEvent = c2_event_a_dVSetLow;
          _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_a_dVSetLow,
                       chartInstance->c2_sfEvent);
          if (chartInstance->c2_is_active_A_dV == 0U) {
          } else {
            c2_A_dV(chartInstance);
          }

          _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_a_dVSetLow,
                       chartInstance->c2_sfEvent);
          chartInstance->c2_sfEvent = c2_b_previousEvent;
          if (chartInstance->c2_is_SANode != c2_IN_Wait4ABeat) {
            _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
          }
        }

        if (chartInstance->c2_is_SANode != c2_IN_Wait4ABeat) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 1U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_Wait4ABeat = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_SANode = c2_IN_Wait4ASynch;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Wait4ASynch = 1U;
          guard1 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 36U,
                     chartInstance->c2_sfEvent);
        guard1 = true;
      }
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 36U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Wait4ASynch:
    CV_STATE_EVAL(35, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 19U,
                 chartInstance->c2_sfEvent);
    c2_c_out = (CV_TRANSITION_EVAL(19U, (int32_T)_SFD_CCP_CALL(19U, 0,
      chartInstance->c2_sfEvent == c2_event_NextAtrBeat != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_c_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 19U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Wait4ASynch = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_SANode = c2_IN_Wait4ABeat;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i8 = 0U;
      chartInstance->c2_tp_Wait4ABeat = 1U;
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 37U,
                   chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 37U, chartInstance->c2_sfEvent);
    break;

   default:
    CV_STATE_EVAL(35, 0, 0);
    chartInstance->c2_is_SANode = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 35U, chartInstance->c2_sfEvent);
}

static void c2_VRG(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_temp;
  boolean_T c2_b_out;
  int32_T c2_previousEvent;
  boolean_T c2_c_out;
  boolean_T guard1 = false;
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 40U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_VRG) {
   case c2_IN_Wait4VBeat:
    CV_STATE_EVAL(40, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 50U,
                 chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(50U, (int32_T)_SFD_CCP_CALL(50U, 0,
                chartInstance->c2_sfEvent == c2_event_NextVtrBeat != 0U,
                chartInstance->c2_sfEvent)) != 0);
    guard1 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 50U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Wait4VBeat = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_VRG = c2_IN_Wait4VBeat;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i9 = 0U;
      chartInstance->c2_tp_Wait4VBeat = 1U;
      guard1 = true;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 39U,
                   chartInstance->c2_sfEvent);
      c2_temp = (_SFD_CCP_CALL(39U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
      if (c2_temp) {
        c2_temp = (_SFD_CCP_CALL(39U, 1, (real_T)
                    chartInstance->c2_temporalCounter_i9 >=
                    chartInstance->c2_VRG_d != 0U, chartInstance->c2_sfEvent) !=
                   0);
      }

      c2_b_out = (CV_TRANSITION_EVAL(39U, (int32_T)c2_temp) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
        c2_previousEvent = chartInstance->c2_sfEvent;
        chartInstance->c2_sfEvent = c2_event_Vbeat;
        _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_Vbeat,
                     chartInstance->c2_sfEvent);
        if (chartInstance->c2_is_active_Ventricle == 0U) {
        } else {
          c2_Ventricle(chartInstance);
        }

        _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_Vbeat,
                     chartInstance->c2_sfEvent);
        chartInstance->c2_sfEvent = c2_previousEvent;
        if (chartInstance->c2_is_VRG != c2_IN_Wait4VBeat) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
        }

        if (chartInstance->c2_is_VRG != c2_IN_Wait4VBeat) {
          _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 39U, chartInstance->c2_sfEvent);
        } else {
          chartInstance->c2_tp_Wait4VBeat = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_VRG = c2_IN_Wait4VSynch;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 42U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Wait4VSynch = 1U;
          guard1 = true;
        }
      } else {
        _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 41U,
                     chartInstance->c2_sfEvent);
        guard1 = true;
      }
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 41U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Wait4VSynch:
    CV_STATE_EVAL(40, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 40U,
                 chartInstance->c2_sfEvent);
    c2_c_out = (CV_TRANSITION_EVAL(40U, (int32_T)_SFD_CCP_CALL(40U, 0,
      chartInstance->c2_sfEvent == c2_event_NextVtrBeat != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_c_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 40U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Wait4VSynch = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 42U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_VRG = c2_IN_Wait4VBeat;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i9 = 0U;
      chartInstance->c2_tp_Wait4VBeat = 1U;
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 42U,
                   chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 42U, chartInstance->c2_sfEvent);
    break;

   default:
    CV_STATE_EVAL(40, 0, 0);
    chartInstance->c2_is_VRG = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 41U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 40U, chartInstance->c2_sfEvent);
}

static void c2_AVJ(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_temp;
  boolean_T c2_out;
  int32_T c2_previousEvent;
  boolean_T c2_b_out;
  boolean_T c2_b_temp;
  boolean_T c2_c_out;
  boolean_T c2_d_out;
  boolean_T c2_e_out;
  boolean_T c2_f_out;
  boolean_T c2_g_out;
  boolean_T c2_h_out;
  boolean_T c2_c_temp;
  boolean_T c2_i_out;
  boolean_T guard1 = false;
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_AVJ) {
   case c2_IN_Ante:
    CV_STATE_EVAL(1, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 61U,
                 chartInstance->c2_sfEvent);
    c2_temp = (_SFD_CCP_CALL(61U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
    if (c2_temp) {
      c2_temp = (_SFD_CCP_CALL(61U, 1, (real_T)
                  chartInstance->c2_temporalCounter_i10 >=
                  chartInstance->c2_avj_t4 != 0U, chartInstance->c2_sfEvent) !=
                 0);
    }

    c2_out = (CV_TRANSITION_EVAL(61U, (int32_T)c2_temp) != 0);
    guard1 = false;
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 61U, chartInstance->c2_sfEvent);
      chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
      chartInstance->c2_avj_tRec = chartInstance->c2_ctime -
        chartInstance->c2_recoverylast;
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_tRec, 26U);
      c2_previousEvent = chartInstance->c2_sfEvent;
      chartInstance->c2_sfEvent = c2_event_AVJAnteExit;
      _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AVJAnteExit,
                   chartInstance->c2_sfEvent);
      if (chartInstance->c2_is_active_AVJOut == 0U) {
      } else {
        c2_AVJOut(chartInstance);
      }

      _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AVJAnteExit,
                   chartInstance->c2_sfEvent);
      chartInstance->c2_sfEvent = c2_previousEvent;
      if (chartInstance->c2_is_AVJ != c2_IN_Ante) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 61U, chartInstance->c2_sfEvent);
      } else {
        chartInstance->c2_avj_refr_0 = c2_initRefrPeriod(chartInstance,
          chartInstance->c2_avj_tRec);
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr_0, 38U);
        chartInstance->c2_avj_refr = chartInstance->c2_avj_refr_0;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
        chartInstance->c2_refractorylast = chartInstance->c2_ctime;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
      }

      if (chartInstance->c2_is_AVJ != c2_IN_Ante) {
        _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 61U, chartInstance->c2_sfEvent);
      } else {
        chartInstance->c2_d_tp_Ante = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJ = c2_IN_Refractory;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i10 = 0U;
        chartInstance->c2_tp_Refractory = 1U;
        guard1 = true;
      }
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 2U,
                   chartInstance->c2_sfEvent);
      guard1 = true;
    }

    if (guard1 == true) {
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 2U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Recovery:
    CV_STATE_EVAL(1, 0, 2);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 62U,
                 chartInstance->c2_sfEvent);
    c2_b_out = (CV_TRANSITION_EVAL(62U, (int32_T)_SFD_CCP_CALL(62U, 0,
      chartInstance->c2_sfEvent == c2_event_AVJAnteReached != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_b_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 62U, chartInstance->c2_sfEvent);
      chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
      chartInstance->c2_avj_t4 = c2_timeToDep(chartInstance,
        (chartInstance->c2_AV_Vr + (chartInstance->c2_ctime -
        chartInstance->c2_recoverylast) * chartInstance->c2_AV_k4) +
        chartInstance->c2_SA_dV);
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_t4, 35U);
      chartInstance->c2_tp_Recovery = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_AVJ = c2_IN_Ante;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i10 = 0U;
      chartInstance->c2_d_tp_Ante = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 56U,
                   chartInstance->c2_sfEvent);
      c2_b_temp = (_SFD_CCP_CALL(56U, 0, 1 != 0U, chartInstance->c2_sfEvent) !=
                   0);
      if (c2_b_temp) {
        c2_b_temp = (_SFD_CCP_CALL(56U, 1, (real_T)
          chartInstance->c2_temporalCounter_i10 >= chartInstance->c2_avj_t4 !=
          0U, chartInstance->c2_sfEvent) != 0);
      }

      c2_c_out = (CV_TRANSITION_EVAL(56U, (int32_T)c2_b_temp) != 0);
      if (c2_c_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 56U, chartInstance->c2_sfEvent);
        chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
        chartInstance->c2_avj_tRec = chartInstance->c2_avj_t4;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_tRec, 26U);
        chartInstance->c2_avj_refr_0 = c2_initRefrPeriod(chartInstance,
          chartInstance->c2_avj_tRec);
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr_0, 38U);
        chartInstance->c2_avj_refr = chartInstance->c2_avj_refr_0;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
        chartInstance->c2_refractorylast = chartInstance->c2_ctime;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
        chartInstance->c2_tp_Recovery = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJ = c2_IN_Refractory;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i10 = 0U;
        chartInstance->c2_tp_Refractory = 1U;
      } else {
        _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 67U,
                     chartInstance->c2_sfEvent);
        c2_d_out = (CV_TRANSITION_EVAL(67U, (int32_T)_SFD_CCP_CALL(67U, 0,
          chartInstance->c2_sfEvent == c2_event_AVJAnteReachedHigh != 0U,
          chartInstance->c2_sfEvent)) != 0);
        if (c2_d_out) {
          _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 67U, chartInstance->c2_sfEvent);
          chartInstance->c2_ctime += (real_T)
            chartInstance->c2_temporalCounter_i10;
          _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
          chartInstance->c2_avj_t4 = c2_timeToDep(chartInstance,
            (chartInstance->c2_AV_Vr + (chartInstance->c2_ctime -
            chartInstance->c2_recoverylast) * chartInstance->c2_AV_k4) +
            chartInstance->c2_SA_ectopdV);
          _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_t4, 35U);
          chartInstance->c2_tp_Recovery = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_AVJ = c2_IN_Ante;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
          chartInstance->c2_temporalCounter_i10 = 0U;
          chartInstance->c2_d_tp_Ante = 1U;
        } else {
          _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 55U,
                       chartInstance->c2_sfEvent);
          c2_e_out = (CV_TRANSITION_EVAL(55U, (int32_T)_SFD_CCP_CALL(55U, 0,
            chartInstance->c2_sfEvent == c2_event_AVJRetroReached != 0U,
            chartInstance->c2_sfEvent)) != 0);
          if (c2_e_out) {
            _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 55U, chartInstance->c2_sfEvent);
            chartInstance->c2_ctime += (real_T)
              chartInstance->c2_temporalCounter_i10;
            _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
            chartInstance->c2_tp_Recovery = 0U;
            _SFD_CS_CALL(STATE_INACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
            chartInstance->c2_is_AVJ = c2_b_IN_Retro;
            _SFD_CS_CALL(STATE_ACTIVE_TAG, 5U, chartInstance->c2_sfEvent);
            chartInstance->c2_temporalCounter_i10 = 0U;
            chartInstance->c2_d_tp_Retro = 1U;
          } else {
            _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 3U,
                         chartInstance->c2_sfEvent);
          }
        }
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 3U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_Refractory:
    CV_STATE_EVAL(1, 0, 3);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 57U,
                 chartInstance->c2_sfEvent);
    c2_f_out = (CV_TRANSITION_EVAL(57U, (int32_T)_SFD_CCP_CALL(57U, 0,
      chartInstance->c2_sfEvent == c2_event_AVJAnteReached != 0U,
      chartInstance->c2_sfEvent)) != 0);
    if (c2_f_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 57U, chartInstance->c2_sfEvent);
      chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
      chartInstance->c2_avj_refr = (chartInstance->c2_avj_refr -
        (chartInstance->c2_ctime - chartInstance->c2_refractorylast)) +
        c2_modulateRefrAnte(chartInstance, chartInstance->c2_ctime -
                            chartInstance->c2_refractorylast,
                            chartInstance->c2_avj_refr_0,
                            chartInstance->c2_SA_dV);
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
      chartInstance->c2_refractorylast = chartInstance->c2_ctime;
      _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
      chartInstance->c2_tp_Refractory = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_AVJ = c2_IN_Refractory;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i10 = 0U;
      chartInstance->c2_tp_Refractory = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 58U,
                   chartInstance->c2_sfEvent);
      c2_g_out = (CV_TRANSITION_EVAL(58U, (int32_T)_SFD_CCP_CALL(58U, 0,
        chartInstance->c2_sfEvent == c2_event_AVJRetroReached != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_g_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 58U, chartInstance->c2_sfEvent);
        chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
        chartInstance->c2_avj_refr = (chartInstance->c2_avj_refr -
          (chartInstance->c2_ctime - chartInstance->c2_refractorylast)) +
          c2_modulateRefrRetro(chartInstance, chartInstance->c2_ctime -
          chartInstance->c2_refractorylast, chartInstance->c2_avj_refr_0);
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
        chartInstance->c2_refractorylast = chartInstance->c2_ctime;
        _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
        chartInstance->c2_tp_Refractory = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_AVJ = c2_IN_Refractory;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
        chartInstance->c2_temporalCounter_i10 = 0U;
        chartInstance->c2_tp_Refractory = 1U;
      } else {
        _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 68U,
                     chartInstance->c2_sfEvent);
        c2_h_out = (CV_TRANSITION_EVAL(68U, (int32_T)_SFD_CCP_CALL(68U, 0,
          chartInstance->c2_sfEvent == c2_event_AVJAnteReachedHigh != 0U,
          chartInstance->c2_sfEvent)) != 0);
        if (c2_h_out) {
          _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 68U, chartInstance->c2_sfEvent);
          chartInstance->c2_ctime += (real_T)
            chartInstance->c2_temporalCounter_i10;
          _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
          chartInstance->c2_avj_refr = (chartInstance->c2_avj_refr -
            (chartInstance->c2_ctime - chartInstance->c2_refractorylast)) +
            c2_modulateRefrAnte(chartInstance, chartInstance->c2_ctime -
                                chartInstance->c2_refractorylast,
                                chartInstance->c2_avj_refr_0,
                                chartInstance->c2_SA_ectopdV);
          _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
          chartInstance->c2_refractorylast = chartInstance->c2_ctime;
          _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
          chartInstance->c2_tp_Refractory = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_AVJ = c2_IN_Refractory;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
          chartInstance->c2_temporalCounter_i10 = 0U;
          chartInstance->c2_tp_Refractory = 1U;
        } else {
          _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 54U,
                       chartInstance->c2_sfEvent);
          c2_c_temp = (_SFD_CCP_CALL(54U, 0, 1 != 0U, chartInstance->c2_sfEvent)
                       != 0);
          if (c2_c_temp) {
            c2_c_temp = (_SFD_CCP_CALL(54U, 1, (real_T)
              chartInstance->c2_temporalCounter_i10 >=
              chartInstance->c2_avj_refr != 0U, chartInstance->c2_sfEvent) != 0);
          }

          c2_i_out = (CV_TRANSITION_EVAL(54U, (int32_T)c2_c_temp) != 0);
          if (c2_i_out) {
            _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 54U, chartInstance->c2_sfEvent);
            chartInstance->c2_ctime += (real_T)
              chartInstance->c2_temporalCounter_i10;
            _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
            chartInstance->c2_avj_t4 = c2_timeToDep(chartInstance,
              chartInstance->c2_AV_Vr);
            _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_t4, 35U);
            chartInstance->c2_recoverylast = chartInstance->c2_ctime;
            _SFD_DATA_RANGE_CHECK(chartInstance->c2_recoverylast, 34U);
            chartInstance->c2_tp_Refractory = 0U;
            _SFD_CS_CALL(STATE_INACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
            chartInstance->c2_is_AVJ = c2_IN_Recovery;
            _SFD_CS_CALL(STATE_ACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
            chartInstance->c2_temporalCounter_i10 = 0U;
            chartInstance->c2_tp_Recovery = 1U;
          } else {
            _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 4U,
                         chartInstance->c2_sfEvent);
          }
        }
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 4U, chartInstance->c2_sfEvent);
    break;

   case c2_b_IN_Retro:
    CV_STATE_EVAL(1, 0, 4);
    c2_Retro(chartInstance);
    break;

   default:
    CV_STATE_EVAL(1, 0, 0);
    chartInstance->c2_is_AVJ = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 2U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 1U, chartInstance->c2_sfEvent);
}

static void c2_Retro(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  int32_T c2_previousEvent;
  _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 59U, chartInstance->c2_sfEvent);
  c2_previousEvent = chartInstance->c2_sfEvent;
  chartInstance->c2_sfEvent = c2_event_AVJRetroExit;
  _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_AVJRetroExit,
               chartInstance->c2_sfEvent);
  if (chartInstance->c2_is_active_AVJOut == 0U) {
  } else {
    c2_AVJOut(chartInstance);
  }

  _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_AVJRetroExit,
               chartInstance->c2_sfEvent);
  chartInstance->c2_sfEvent = c2_previousEvent;
  if (chartInstance->c2_is_AVJ != c2_b_IN_Retro) {
    _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 59U, chartInstance->c2_sfEvent);
  } else {
    chartInstance->c2_ctime += (real_T)chartInstance->c2_temporalCounter_i10;
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_ctime, 36U);
    chartInstance->c2_avj_tRec = chartInstance->c2_ctime -
      chartInstance->c2_recoverylast;
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_tRec, 26U);
    chartInstance->c2_avj_refr_0 = c2_initRefrPeriod(chartInstance,
      chartInstance->c2_avj_tRec);
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr_0, 38U);
    chartInstance->c2_avj_refr = chartInstance->c2_avj_refr_0;
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_avj_refr, 37U);
    chartInstance->c2_refractorylast = chartInstance->c2_ctime;
    _SFD_DATA_RANGE_CHECK(chartInstance->c2_refractorylast, 39U);
  }

  if (chartInstance->c2_is_AVJ != c2_b_IN_Retro) {
    _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 59U, chartInstance->c2_sfEvent);
  } else {
    chartInstance->c2_d_tp_Retro = 0U;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 5U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_AVJ = c2_IN_Refractory;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
    chartInstance->c2_temporalCounter_i10 = 0U;
    chartInstance->c2_tp_Refractory = 1U;
    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 5U, chartInstance->c2_sfEvent);
  }
}

static void c2_Ventricle(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  boolean_T c2_out;
  boolean_T c2_b_out;
  boolean_T c2_c_out;
  int32_T c2_previousEvent;
  boolean_T c2_temp;
  boolean_T c2_d_out;
  int32_T c2_b_previousEvent;
  real_T *c2_RT_delay;
  c2_RT_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 45U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_Ventricle) {
   case c2_IN_Excitable:
    CV_STATE_EVAL(45, 0, 1);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 34U,
                 chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(34U, (int32_T)_SFD_CCP_CALL(34U, 0,
                chartInstance->c2_sfEvent == c2_event_VtrAnteReached != 0U,
                chartInstance->c2_sfEvent)) != 0);
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 34U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Excitable = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Ventricle = c2_IN_VsenseAnte;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 51U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_VsenseAnte = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 36U,
                   chartInstance->c2_sfEvent);
      c2_b_out = (CV_TRANSITION_EVAL(36U, (int32_T)_SFD_CCP_CALL(36U, 0,
        chartInstance->c2_sfEvent == c2_event_VP != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 36U, chartInstance->c2_sfEvent);
        chartInstance->c2_tp_Excitable = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_Ventricle = c2_IN_GenerateRetroWave;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
        chartInstance->c2_tp_GenerateRetroWave = 1U;
      } else {
        _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 33U,
                     chartInstance->c2_sfEvent);
        c2_c_out = (CV_TRANSITION_EVAL(33U, (int32_T)_SFD_CCP_CALL(33U, 0,
          chartInstance->c2_sfEvent == c2_event_Vbeat != 0U,
          chartInstance->c2_sfEvent)) != 0);
        if (c2_c_out) {
          _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 33U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Excitable = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_Ventricle = c2_IN_Vsense;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 50U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_Vsense = 1U;
        } else {
          _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 46U,
                       chartInstance->c2_sfEvent);
        }
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 46U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_GenerateRetroWave:
    CV_STATE_EVAL(45, 0, 2);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
    c2_previousEvent = chartInstance->c2_sfEvent;
    chartInstance->c2_sfEvent = c2_event_vrg;
    _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_vrg,
                 chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_active_RVConductor == 0U) {
    } else {
      c2_RVConductor(chartInstance);
    }

    _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_vrg,
                 chartInstance->c2_sfEvent);
    chartInstance->c2_sfEvent = c2_previousEvent;
    if (chartInstance->c2_is_Ventricle != c2_IN_GenerateRetroWave) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
    }

    if (chartInstance->c2_is_Ventricle != c2_IN_GenerateRetroWave) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 37U, chartInstance->c2_sfEvent);
    } else {
      chartInstance->c2_tp_GenerateRetroWave = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Ventricle = c2_IN_VSynch;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_VSynch = 1U;
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 47U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Refractory:
    CV_STATE_EVAL(45, 0, 3);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 32U,
                 chartInstance->c2_sfEvent);
    c2_temp = (_SFD_CCP_CALL(32U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
    if (c2_temp) {
      c2_temp = (_SFD_CCP_CALL(32U, 1, (real_T)
                  chartInstance->c2_temporalCounter_i11 >= *c2_RT_delay != 0U,
                  chartInstance->c2_sfEvent) != 0);
    }

    c2_d_out = (CV_TRANSITION_EVAL(32U, (int32_T)c2_temp) != 0);
    if (c2_d_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 32U, chartInstance->c2_sfEvent);
      chartInstance->c2_RefractEventCounter++;
      chartInstance->c2_b_tp_Refractory = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Ventricle = c2_IN_Excitable;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Excitable = 1U;
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 48U,
                   chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 48U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_VSynch:
    CV_STATE_EVAL(45, 0, 4);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
    c2_b_previousEvent = chartInstance->c2_sfEvent;
    chartInstance->c2_sfEvent = c2_event_NextVtrBeat;
    _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_NextVtrBeat,
                 chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_active_VRG == 0U) {
    } else {
      c2_VRG(chartInstance);
    }

    _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_NextVtrBeat,
                 chartInstance->c2_sfEvent);
    chartInstance->c2_sfEvent = c2_b_previousEvent;
    if (chartInstance->c2_is_Ventricle != c2_IN_VSynch) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
    }

    if (chartInstance->c2_is_Ventricle != c2_IN_VSynch) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 35U, chartInstance->c2_sfEvent);
    } else {
      chartInstance->c2_tp_VSynch = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Ventricle = c2_IN_Refractory;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i11 = 0U;
      chartInstance->c2_b_tp_Refractory = 1U;
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 49U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Vsense:
    CV_STATE_EVAL(45, 0, 5);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
    chartInstance->c2_VgetEventCounter++;
    chartInstance->c2_tp_Vsense = 0U;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 50U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_Ventricle = c2_IN_GenerateRetroWave;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 47U, chartInstance->c2_sfEvent);
    chartInstance->c2_tp_GenerateRetroWave = 1U;
    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 50U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_VsenseAnte:
    CV_STATE_EVAL(45, 0, 6);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 48U, chartInstance->c2_sfEvent);
    chartInstance->c2_VgetEventCounter++;
    chartInstance->c2_tp_VsenseAnte = 0U;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 51U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_Ventricle = c2_IN_VSynch;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 49U, chartInstance->c2_sfEvent);
    chartInstance->c2_tp_VSynch = 1U;
    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 51U, chartInstance->c2_sfEvent);
    break;

   default:
    CV_STATE_EVAL(45, 0, 0);
    chartInstance->c2_is_Ventricle = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 45U, chartInstance->c2_sfEvent);
}

static void c2_Atrium(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  int32_T c2_previousEvent;
  boolean_T c2_out;
  boolean_T c2_b_out;
  boolean_T c2_c_out;
  int32_T c2_b_previousEvent;
  boolean_T c2_temp;
  boolean_T c2_d_out;
  _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 20U, chartInstance->c2_sfEvent);
  switch (chartInstance->c2_is_Atrium) {
   case c2_IN_ASynch:
    CV_STATE_EVAL(20, 0, 1);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
    c2_previousEvent = chartInstance->c2_sfEvent;
    chartInstance->c2_sfEvent = c2_event_NextAtrBeat;
    _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_NextAtrBeat,
                 chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_active_SANode == 0U) {
    } else {
      c2_SANode(chartInstance);
    }

    _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_NextAtrBeat,
                 chartInstance->c2_sfEvent);
    chartInstance->c2_sfEvent = c2_previousEvent;
    if (chartInstance->c2_is_Atrium != c2_IN_ASynch) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
    }

    if (chartInstance->c2_is_Atrium != c2_IN_ASynch) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 20U, chartInstance->c2_sfEvent);
    } else {
      chartInstance->c2_tp_ASynch = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Atrium = c2_b_IN_Refractory;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
      chartInstance->c2_temporalCounter_i12 = 0U;
      chartInstance->c2_c_tp_Refractory = 1U;
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 21U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_IN_Asense:
    CV_STATE_EVAL(20, 0, 2);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 53U, chartInstance->c2_sfEvent);
    chartInstance->c2_AgetEventCounter++;
    chartInstance->c2_tp_Asense = 0U;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_Atrium = c2_IN_GenerateAnteWave;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
    chartInstance->c2_tp_GenerateAnteWave = 1U;
    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 22U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_AsenseRetro:
    CV_STATE_EVAL(20, 0, 3);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 46U, chartInstance->c2_sfEvent);
    chartInstance->c2_AgetEventCounter++;
    chartInstance->c2_tp_AsenseRetro = 0U;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
    chartInstance->c2_is_Atrium = c2_IN_ASynch;
    _SFD_CS_CALL(STATE_ACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
    chartInstance->c2_tp_ASynch = 1U;
    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 23U, chartInstance->c2_sfEvent);
    break;

   case c2_b_IN_Excitable:
    CV_STATE_EVAL(20, 0, 4);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 3U, chartInstance->c2_sfEvent);
    c2_out = (CV_TRANSITION_EVAL(3U, (int32_T)_SFD_CCP_CALL(3U, 0,
                chartInstance->c2_sfEvent == c2_event_Abeat != 0U,
                chartInstance->c2_sfEvent)) != 0);
    if (c2_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 3U, chartInstance->c2_sfEvent);
      chartInstance->c2_b_tp_Excitable = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Atrium = c2_IN_Asense;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 22U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_Asense = 1U;
    } else {
      _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 5U,
                   chartInstance->c2_sfEvent);
      c2_b_out = (CV_TRANSITION_EVAL(5U, (int32_T)_SFD_CCP_CALL(5U, 0,
        chartInstance->c2_sfEvent == c2_event_AP != 0U,
        chartInstance->c2_sfEvent)) != 0);
      if (c2_b_out) {
        _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 5U, chartInstance->c2_sfEvent);
        chartInstance->c2_b_tp_Excitable = 0U;
        _SFD_CS_CALL(STATE_INACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
        chartInstance->c2_is_Atrium = c2_IN_GenerateAnteWave;
        _SFD_CS_CALL(STATE_ACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
        chartInstance->c2_tp_GenerateAnteWave = 1U;
      } else {
        _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 4U,
                     chartInstance->c2_sfEvent);
        c2_c_out = (CV_TRANSITION_EVAL(4U, (int32_T)_SFD_CCP_CALL(4U, 0,
          chartInstance->c2_sfEvent == c2_event_AtrRetroReached != 0U,
          chartInstance->c2_sfEvent)) != 0);
        if (c2_c_out) {
          _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 4U, chartInstance->c2_sfEvent);
          chartInstance->c2_b_tp_Excitable = 0U;
          _SFD_CS_CALL(STATE_INACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
          chartInstance->c2_is_Atrium = c2_IN_AsenseRetro;
          _SFD_CS_CALL(STATE_ACTIVE_TAG, 23U, chartInstance->c2_sfEvent);
          chartInstance->c2_tp_AsenseRetro = 1U;
        } else {
          _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 24U,
                       chartInstance->c2_sfEvent);
        }
      }
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 24U, chartInstance->c2_sfEvent);
    break;

   case c2_IN_GenerateAnteWave:
    CV_STATE_EVAL(20, 0, 5);
    _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 7U, chartInstance->c2_sfEvent);
    c2_b_previousEvent = chartInstance->c2_sfEvent;
    chartInstance->c2_sfEvent = c2_event_arg;
    _SFD_CE_CALL(EVENT_BEFORE_BROADCAST_TAG, c2_event_arg,
                 chartInstance->c2_sfEvent);
    if (chartInstance->c2_is_active_RAConductor == 0U) {
    } else {
      c2_RAConductor(chartInstance);
    }

    _SFD_CE_CALL(EVENT_AFTER_BROADCAST_TAG, c2_event_arg,
                 chartInstance->c2_sfEvent);
    chartInstance->c2_sfEvent = c2_b_previousEvent;
    if (chartInstance->c2_is_Atrium != c2_IN_GenerateAnteWave) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 7U, chartInstance->c2_sfEvent);
    }

    if (chartInstance->c2_is_Atrium != c2_IN_GenerateAnteWave) {
      _SFD_CT_CALL(TRANSITION_INACTIVE_TAG, 7U, chartInstance->c2_sfEvent);
    } else {
      chartInstance->c2_tp_GenerateAnteWave = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 25U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Atrium = c2_IN_ASynch;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
      chartInstance->c2_tp_ASynch = 1U;
      _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 25U, chartInstance->c2_sfEvent);
    }
    break;

   case c2_b_IN_Refractory:
    CV_STATE_EVAL(20, 0, 6);
    _SFD_CT_CALL(TRANSITION_BEFORE_PROCESSING_TAG, 6U, chartInstance->c2_sfEvent);
    c2_temp = (_SFD_CCP_CALL(6U, 0, 1 != 0U, chartInstance->c2_sfEvent) != 0);
    if (c2_temp) {
      c2_temp = (_SFD_CCP_CALL(6U, 1, (real_T)
                  chartInstance->c2_temporalCounter_i12 >=
                  chartInstance->c2_Atr_refrD != 0U, chartInstance->c2_sfEvent)
                 != 0);
    }

    c2_d_out = (CV_TRANSITION_EVAL(6U, (int32_T)c2_temp) != 0);
    if (c2_d_out) {
      _SFD_CT_CALL(TRANSITION_ACTIVE_TAG, 6U, chartInstance->c2_sfEvent);
      chartInstance->c2_c_tp_Refractory = 0U;
      _SFD_CS_CALL(STATE_INACTIVE_TAG, 26U, chartInstance->c2_sfEvent);
      chartInstance->c2_is_Atrium = c2_b_IN_Excitable;
      _SFD_CS_CALL(STATE_ACTIVE_TAG, 24U, chartInstance->c2_sfEvent);
      chartInstance->c2_b_tp_Excitable = 1U;
    } else {
      _SFD_CS_CALL(STATE_ENTER_DURING_FUNCTION_TAG, 26U,
                   chartInstance->c2_sfEvent);
    }

    _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 26U, chartInstance->c2_sfEvent);
    break;

   default:
    CV_STATE_EVAL(20, 0, 0);
    chartInstance->c2_is_Atrium = c2_IN_NO_ACTIVE_CHILD;
    _SFD_CS_CALL(STATE_INACTIVE_TAG, 21U, chartInstance->c2_sfEvent);
    break;
  }

  _SFD_CS_CALL(EXIT_OUT_OF_FUNCTION_TAG, 20U, chartInstance->c2_sfEvent);
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber)
{
  (void)c2_machineNumber;
  (void)c2_chartNumber;
  (void)c2_instanceNumber;
}

static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  real_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, false);
  return c2_mxArrayOutData;
}

static real_T c2_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_t, const char_T *c2_identifier)
{
  real_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_t), &c2_thisId);
  sf_mex_destroy(&c2_t);
  return c2_y;
}

static real_T c2_b_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_y;
  real_T c2_d31;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d31, 1, 0, 0U, 0, 0U, 0);
  c2_y = c2_d31;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_t;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_t = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_t), &c2_thisId);
  sf_mex_destroy(&c2_t);
  *(real_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

const mxArray *sf_c2_HeartModelECG_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_nameCaptureInfo = NULL;
  sf_mex_assign(&c2_nameCaptureInfo, sf_mex_createstruct("structure", 2, 29, 1),
                false);
  c2_info_helper(&c2_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c2_nameCaptureInfo);
  return c2_nameCaptureInfo;
}

static void c2_info_helper(const mxArray **c2_info)
{
  const mxArray *c2_rhs0 = NULL;
  const mxArray *c2_lhs0 = NULL;
  const mxArray *c2_rhs1 = NULL;
  const mxArray *c2_lhs1 = NULL;
  const mxArray *c2_rhs2 = NULL;
  const mxArray *c2_lhs2 = NULL;
  const mxArray *c2_rhs3 = NULL;
  const mxArray *c2_lhs3 = NULL;
  const mxArray *c2_rhs4 = NULL;
  const mxArray *c2_lhs4 = NULL;
  const mxArray *c2_rhs5 = NULL;
  const mxArray *c2_lhs5 = NULL;
  const mxArray *c2_rhs6 = NULL;
  const mxArray *c2_lhs6 = NULL;
  const mxArray *c2_rhs7 = NULL;
  const mxArray *c2_lhs7 = NULL;
  const mxArray *c2_rhs8 = NULL;
  const mxArray *c2_lhs8 = NULL;
  const mxArray *c2_rhs9 = NULL;
  const mxArray *c2_lhs9 = NULL;
  const mxArray *c2_rhs10 = NULL;
  const mxArray *c2_lhs10 = NULL;
  const mxArray *c2_rhs11 = NULL;
  const mxArray *c2_lhs11 = NULL;
  const mxArray *c2_rhs12 = NULL;
  const mxArray *c2_lhs12 = NULL;
  const mxArray *c2_rhs13 = NULL;
  const mxArray *c2_lhs13 = NULL;
  const mxArray *c2_rhs14 = NULL;
  const mxArray *c2_lhs14 = NULL;
  const mxArray *c2_rhs15 = NULL;
  const mxArray *c2_lhs15 = NULL;
  const mxArray *c2_rhs16 = NULL;
  const mxArray *c2_lhs16 = NULL;
  const mxArray *c2_rhs17 = NULL;
  const mxArray *c2_lhs17 = NULL;
  const mxArray *c2_rhs18 = NULL;
  const mxArray *c2_lhs18 = NULL;
  const mxArray *c2_rhs19 = NULL;
  const mxArray *c2_lhs19 = NULL;
  const mxArray *c2_rhs20 = NULL;
  const mxArray *c2_lhs20 = NULL;
  const mxArray *c2_rhs21 = NULL;
  const mxArray *c2_lhs21 = NULL;
  const mxArray *c2_rhs22 = NULL;
  const mxArray *c2_lhs22 = NULL;
  const mxArray *c2_rhs23 = NULL;
  const mxArray *c2_lhs23 = NULL;
  const mxArray *c2_rhs24 = NULL;
  const mxArray *c2_lhs24 = NULL;
  const mxArray *c2_rhs25 = NULL;
  const mxArray *c2_lhs25 = NULL;
  const mxArray *c2_rhs26 = NULL;
  const mxArray *c2_lhs26 = NULL;
  const mxArray *c2_rhs27 = NULL;
  const mxArray *c2_lhs27 = NULL;
  const mxArray *c2_rhs28 = NULL;
  const mxArray *c2_lhs28 = NULL;
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("mrdivide"), "name", "name", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1388463696U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1370013486U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c2_rhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.assert"),
                  "name", "name", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/assert.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363718156U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c2_rhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("rdivide"), "name", "name", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363717480U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c2_rhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363718156U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c2_rhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1286822396U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c2_rhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_div"), "name", "name", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c2_rhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.div"), "name",
                  "name", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/div.p"), "resolved",
                  "resolved", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1389311520U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c2_rhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("exp"), "name", "name", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/exp.m"), "resolved",
                  "resolved", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1343833980U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c2_rhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/exp.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_exp"), "name",
                  "name", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_exp.m"),
                  "resolved", "resolved", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1301332064U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c2_rhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("max"), "name", "name", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/max.m"), "resolved",
                  "resolved", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1311258916U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c2_rhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/max.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_min_or_max"), "name",
                  "name", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1378299584U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c2_rhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum"),
                  "context", "context", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c2_rhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.scalarEg"),
                  "name", "name", 12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/scalarEg.p"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1389311520U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c2_rhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum"),
                  "context", "context", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c2_rhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "context", "context", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("coder.internal.scalexpAlloc"),
                  "name", "name", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/scalexpAlloc.p"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1389311520U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c2_rhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum"),
                  "context", "context", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1323174178U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c2_rhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_scalar_bin_extremum"),
                  "context", "context", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c2_rhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_scalar_bin_extremum"),
                  "context", "context", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363718156U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c2_rhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("power"), "name", "name", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "resolved",
                  "resolved", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363717480U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c2_rhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "context",
                  "context", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363718156U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c2_rhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c2_rhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c2_rhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("floor"), "name", "name", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "resolved",
                  "resolved", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363717454U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c2_rhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1363718156U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c2_rhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1286822326U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c2_rhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_error"), "name", "name",
                  25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_error.m"), "resolved",
                  "resolved", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1343833958U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c2_rhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1375984288U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c2_rhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(""), "context", "context", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("min"), "name", "name", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/min.m"), "resolved",
                  "resolved", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1311258918U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c2_rhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/min.m"), "context",
                  "context", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("eml_min_or_max"), "name",
                  "name", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c2_info, c2_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(1378299584U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c2_info, c2_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c2_rhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c2_lhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c2_info, sf_mex_duplicatearraysafe(&c2_lhs28), "lhs", "lhs",
                  28);
  sf_mex_destroy(&c2_rhs0);
  sf_mex_destroy(&c2_lhs0);
  sf_mex_destroy(&c2_rhs1);
  sf_mex_destroy(&c2_lhs1);
  sf_mex_destroy(&c2_rhs2);
  sf_mex_destroy(&c2_lhs2);
  sf_mex_destroy(&c2_rhs3);
  sf_mex_destroy(&c2_lhs3);
  sf_mex_destroy(&c2_rhs4);
  sf_mex_destroy(&c2_lhs4);
  sf_mex_destroy(&c2_rhs5);
  sf_mex_destroy(&c2_lhs5);
  sf_mex_destroy(&c2_rhs6);
  sf_mex_destroy(&c2_lhs6);
  sf_mex_destroy(&c2_rhs7);
  sf_mex_destroy(&c2_lhs7);
  sf_mex_destroy(&c2_rhs8);
  sf_mex_destroy(&c2_lhs8);
  sf_mex_destroy(&c2_rhs9);
  sf_mex_destroy(&c2_lhs9);
  sf_mex_destroy(&c2_rhs10);
  sf_mex_destroy(&c2_lhs10);
  sf_mex_destroy(&c2_rhs11);
  sf_mex_destroy(&c2_lhs11);
  sf_mex_destroy(&c2_rhs12);
  sf_mex_destroy(&c2_lhs12);
  sf_mex_destroy(&c2_rhs13);
  sf_mex_destroy(&c2_lhs13);
  sf_mex_destroy(&c2_rhs14);
  sf_mex_destroy(&c2_lhs14);
  sf_mex_destroy(&c2_rhs15);
  sf_mex_destroy(&c2_lhs15);
  sf_mex_destroy(&c2_rhs16);
  sf_mex_destroy(&c2_lhs16);
  sf_mex_destroy(&c2_rhs17);
  sf_mex_destroy(&c2_lhs17);
  sf_mex_destroy(&c2_rhs18);
  sf_mex_destroy(&c2_lhs18);
  sf_mex_destroy(&c2_rhs19);
  sf_mex_destroy(&c2_lhs19);
  sf_mex_destroy(&c2_rhs20);
  sf_mex_destroy(&c2_lhs20);
  sf_mex_destroy(&c2_rhs21);
  sf_mex_destroy(&c2_lhs21);
  sf_mex_destroy(&c2_rhs22);
  sf_mex_destroy(&c2_lhs22);
  sf_mex_destroy(&c2_rhs23);
  sf_mex_destroy(&c2_lhs23);
  sf_mex_destroy(&c2_rhs24);
  sf_mex_destroy(&c2_lhs24);
  sf_mex_destroy(&c2_rhs25);
  sf_mex_destroy(&c2_lhs25);
  sf_mex_destroy(&c2_rhs26);
  sf_mex_destroy(&c2_lhs26);
  sf_mex_destroy(&c2_rhs27);
  sf_mex_destroy(&c2_lhs27);
  sf_mex_destroy(&c2_rhs28);
  sf_mex_destroy(&c2_lhs28);
}

static const mxArray *c2_emlrt_marshallOut(const char * c2_u)
{
  const mxArray *c2_y = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c2_u)), false);
  return c2_y;
}

static const mxArray *c2_b_emlrt_marshallOut(const uint32_T c2_u)
{
  const mxArray *c2_y = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 7, 0U, 0U, 0U, 0), false);
  return c2_y;
}

static real_T c2_AVJDelay(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_tRec, real_T c2_minDly)
{
  real_T c2_t;
  uint32_T c2_debug_family_var_map[5];
  real_T c2_nargin = 2.0;
  real_T c2_nargout = 1.0;
  real_T c2_hoistedGlobal;
  real_T c2_b_hoistedGlobal;
  real_T c2_A;
  real_T c2_B;
  real_T c2_x;
  real_T c2_y;
  real_T c2_b_x;
  real_T c2_b_y;
  real_T c2_c_x;
  real_T c2_c_y;
  real_T c2_d_y;
  real_T c2_d_x;
  real_T c2_e_x;
  _SFD_SET_DATA_VALUE_PTR(43U, &c2_t);
  _SFD_SET_DATA_VALUE_PTR(45U, &c2_minDly);
  _SFD_SET_DATA_VALUE_PTR(44U, &c2_tRec);
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 5U, 5U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tRec, 2U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_minDly, 3U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t, 4U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(11, 0);
  _SFD_EML_CALL(11U, chartInstance->c2_sfEvent, 2);
  c2_hoistedGlobal = chartInstance->c2_AV_alpha;
  c2_b_hoistedGlobal = chartInstance->c2_AV_tauC;
  c2_A = -c2_tRec;
  c2_B = c2_b_hoistedGlobal;
  c2_x = c2_A;
  c2_y = c2_B;
  c2_b_x = c2_x;
  c2_b_y = c2_y;
  c2_c_x = c2_b_x;
  c2_c_y = c2_b_y;
  c2_d_y = c2_c_x / c2_c_y;
  c2_d_x = c2_d_y;
  c2_e_x = c2_d_x;
  c2_e_x = muDoubleScalarExp(c2_e_x);
  c2_t = c2_minDly + c2_hoistedGlobal * c2_e_x;
  _SFD_EML_CALL(11U, chartInstance->c2_sfEvent, -2);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_UNSET_DATA_VALUE_PTR(43U);
  _SFD_UNSET_DATA_VALUE_PTR(45U);
  _SFD_UNSET_DATA_VALUE_PTR(44U);
  return c2_t;
}

static real_T c2_timeToDep(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_Vm)
{
  real_T c2_t;
  uint32_T c2_debug_family_var_map[4];
  real_T c2_nargin = 1.0;
  real_T c2_nargout = 1.0;
  real_T c2_hoistedGlobal;
  real_T c2_b_hoistedGlobal;
  real_T c2_A;
  real_T c2_B;
  real_T c2_x;
  real_T c2_y;
  real_T c2_b_x;
  real_T c2_b_y;
  real_T c2_c_x;
  real_T c2_c_y;
  real_T c2_d_y;
  real_T c2_varargin_2;
  real_T c2_varargin_3;
  real_T c2_e_y;
  real_T c2_f_y;
  real_T c2_yk;
  real_T c2_g_y;
  _SFD_SET_DATA_VALUE_PTR(46U, &c2_t);
  _SFD_SET_DATA_VALUE_PTR(47U, &c2_Vm);
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 4U, 4U, c2_b_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Vm, 2U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t, 3U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(9, 0);
  _SFD_EML_CALL(9U, chartInstance->c2_sfEvent, 2);
  c2_hoistedGlobal = chartInstance->c2_AV_Vt;
  c2_b_hoistedGlobal = chartInstance->c2_AV_k4;
  c2_A = c2_hoistedGlobal - c2_Vm;
  c2_B = c2_b_hoistedGlobal;
  c2_x = c2_A;
  c2_y = c2_B;
  c2_b_x = c2_x;
  c2_b_y = c2_y;
  c2_c_x = c2_b_x;
  c2_c_y = c2_b_y;
  c2_d_y = c2_c_x / c2_c_y;
  c2_varargin_2 = c2_d_y;
  c2_varargin_3 = c2_varargin_2;
  c2_e_y = c2_varargin_3;
  c2_f_y = c2_e_y;
  c2_eml_scalar_eg(chartInstance);
  c2_yk = c2_f_y;
  c2_g_y = c2_yk;
  c2_eml_scalar_eg(chartInstance);
  c2_t = muDoubleScalarMax(0.0, c2_g_y);
  _SFD_EML_CALL(9U, chartInstance->c2_sfEvent, -2);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_UNSET_DATA_VALUE_PTR(46U);
  _SFD_UNSET_DATA_VALUE_PTR(47U);
  return c2_t;
}

static real_T c2_initRefrPeriod(SFc2_HeartModelECGInstanceStruct *chartInstance,
  real_T c2_tRec)
{
  real_T c2_t;
  uint32_T c2_debug_family_var_map[4];
  real_T c2_nargin = 1.0;
  real_T c2_nargout = 1.0;
  real_T c2_hoistedGlobal;
  real_T c2_b_hoistedGlobal;
  real_T c2_c_hoistedGlobal;
  real_T c2_A;
  real_T c2_B;
  real_T c2_x;
  real_T c2_y;
  real_T c2_b_x;
  real_T c2_b_y;
  real_T c2_c_x;
  real_T c2_c_y;
  real_T c2_d_y;
  real_T c2_d_x;
  real_T c2_e_x;
  _SFD_SET_DATA_VALUE_PTR(48U, &c2_t);
  _SFD_SET_DATA_VALUE_PTR(49U, &c2_tRec);
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 4U, 4U, c2_c_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tRec, 2U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t, 3U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(6, 0);
  _SFD_EML_CALL(6U, chartInstance->c2_sfEvent, 2);
  c2_hoistedGlobal = chartInstance->c2_AV_refrDMin;
  c2_b_hoistedGlobal = chartInstance->c2_AV_beta;
  c2_c_hoistedGlobal = chartInstance->c2_AV_tr;
  c2_A = -c2_tRec;
  c2_B = c2_c_hoistedGlobal;
  c2_x = c2_A;
  c2_y = c2_B;
  c2_b_x = c2_x;
  c2_b_y = c2_y;
  c2_c_x = c2_b_x;
  c2_c_y = c2_b_y;
  c2_d_y = c2_c_x / c2_c_y;
  c2_d_x = c2_d_y;
  c2_e_x = c2_d_x;
  c2_e_x = muDoubleScalarExp(c2_e_x);
  c2_t = c2_hoistedGlobal + c2_b_hoistedGlobal * (1.0 - c2_e_x);
  _SFD_EML_CALL(6U, chartInstance->c2_sfEvent, -2);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_UNSET_DATA_VALUE_PTR(48U);
  _SFD_UNSET_DATA_VALUE_PTR(49U);
  return c2_t;
}

static real_T c2_modulateRefrRetro(SFc2_HeartModelECGInstanceStruct
  *chartInstance, real_T c2_t, real_T c2_t0)
{
  real_T c2_dT;
  uint32_T c2_debug_family_var_map[5];
  real_T c2_nargin = 2.0;
  real_T c2_nargout = 1.0;
  real_T c2_hoistedGlobal;
  real_T c2_A;
  real_T c2_B;
  real_T c2_x;
  real_T c2_y;
  real_T c2_b_x;
  real_T c2_b_y;
  real_T c2_c_x;
  real_T c2_c_y;
  real_T c2_d_y;
  _SFD_SET_DATA_VALUE_PTR(51U, &c2_dT);
  _SFD_SET_DATA_VALUE_PTR(52U, &c2_t0);
  _SFD_SET_DATA_VALUE_PTR(50U, &c2_t);
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 5U, 5U, c2_d_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t, 2U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t0, 3U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_dT, 4U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(8, 0);
  _SFD_EML_CALL(8U, chartInstance->c2_sfEvent, 3);
  c2_hoistedGlobal = chartInstance->c2_AV_refrDMin;
  c2_A = c2_t;
  c2_B = c2_t0;
  c2_x = c2_A;
  c2_y = c2_B;
  c2_b_x = c2_x;
  c2_b_y = c2_y;
  c2_c_x = c2_b_x;
  c2_c_y = c2_b_y;
  c2_d_y = c2_c_x / c2_c_y;
  c2_dT = c2_hoistedGlobal * c2_power(chartInstance, c2_d_y,
    chartInstance->c2_AV_theta);
  _SFD_EML_CALL(8U, chartInstance->c2_sfEvent, -3);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_UNSET_DATA_VALUE_PTR(51U);
  _SFD_UNSET_DATA_VALUE_PTR(52U);
  _SFD_UNSET_DATA_VALUE_PTR(50U);
  return c2_dT;
}

static real_T c2_modulateRefrAnte(SFc2_HeartModelECGInstanceStruct
  *chartInstance, real_T c2_t, real_T c2_t0, real_T c2_dV)
{
  real_T c2_dT;
  uint32_T c2_debug_family_var_map[6];
  real_T c2_nargin = 3.0;
  real_T c2_nargout = 1.0;
  real_T c2_hoistedGlobal;
  real_T c2_A;
  real_T c2_B;
  real_T c2_x;
  real_T c2_y;
  real_T c2_b_x;
  real_T c2_b_y;
  real_T c2_c_x;
  real_T c2_c_y;
  real_T c2_d_y;
  real_T c2_b_hoistedGlobal;
  real_T c2_c_hoistedGlobal;
  real_T c2_d_hoistedGlobal;
  real_T c2_b_A;
  real_T c2_b_B;
  real_T c2_d_x;
  real_T c2_e_y;
  real_T c2_e_x;
  real_T c2_f_y;
  real_T c2_f_x;
  real_T c2_g_y;
  real_T c2_h_y;
  real_T c2_varargin_2;
  real_T c2_varargin_3;
  real_T c2_i_y;
  real_T c2_j_y;
  real_T c2_yk;
  real_T c2_k_y;
  real_T c2_minval;
  _SFD_SET_DATA_VALUE_PTR(55U, &c2_dT);
  _SFD_SET_DATA_VALUE_PTR(56U, &c2_dV);
  _SFD_SET_DATA_VALUE_PTR(54U, &c2_t0);
  _SFD_SET_DATA_VALUE_PTR(53U, &c2_t);
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 6U, 6U, c2_e_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 0U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 1U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t, 2U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_t0, 3U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_dV, 4U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_dT, 5U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(7, 0);
  _SFD_EML_CALL(7U, chartInstance->c2_sfEvent, 4);
  c2_hoistedGlobal = chartInstance->c2_AV_refrDMin;
  c2_A = c2_t;
  c2_B = c2_t0;
  c2_x = c2_A;
  c2_y = c2_B;
  c2_b_x = c2_x;
  c2_b_y = c2_y;
  c2_c_x = c2_b_x;
  c2_c_y = c2_b_y;
  c2_d_y = c2_c_x / c2_c_y;
  c2_b_hoistedGlobal = chartInstance->c2_AV_theta;
  c2_c_hoistedGlobal = chartInstance->c2_AV_Vt;
  c2_d_hoistedGlobal = chartInstance->c2_AV_Vr;
  c2_b_A = c2_dV;
  c2_b_B = c2_c_hoistedGlobal - c2_d_hoistedGlobal;
  c2_d_x = c2_b_A;
  c2_e_y = c2_b_B;
  c2_e_x = c2_d_x;
  c2_f_y = c2_e_y;
  c2_f_x = c2_e_x;
  c2_g_y = c2_f_y;
  c2_h_y = c2_f_x / c2_g_y;
  c2_varargin_2 = c2_h_y;
  c2_varargin_3 = c2_varargin_2;
  c2_i_y = c2_varargin_3;
  c2_j_y = c2_i_y;
  c2_eml_scalar_eg(chartInstance);
  c2_yk = c2_j_y;
  c2_k_y = c2_yk;
  c2_eml_scalar_eg(chartInstance);
  c2_minval = muDoubleScalarMin(1.0, c2_k_y);
  c2_dT = c2_hoistedGlobal * c2_power(chartInstance, c2_d_y, c2_b_hoistedGlobal)
    * c2_power(chartInstance, c2_minval, chartInstance->c2_AV_delta);
  _SFD_EML_CALL(7U, chartInstance->c2_sfEvent, -4);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_UNSET_DATA_VALUE_PTR(55U);
  _SFD_UNSET_DATA_VALUE_PTR(56U);
  _SFD_UNSET_DATA_VALUE_PTR(54U);
  _SFD_UNSET_DATA_VALUE_PTR(53U);
  return c2_dT;
}

static void c2_eml_scalar_eg(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static real_T c2_power(SFc2_HeartModelECGInstanceStruct *chartInstance, real_T
  c2_a, real_T c2_b)
{
  real_T c2_b_a;
  real_T c2_b_b;
  real_T c2_ak;
  real_T c2_bk;
  real_T c2_x;
  real_T c2_b_x;
  real_T c2_c_a;
  real_T c2_c_b;
  real_T c2_ar;
  real_T c2_br;
  c2_b_a = c2_a;
  c2_b_b = c2_b;
  c2_eml_scalar_eg(chartInstance);
  c2_ak = c2_b_a;
  c2_bk = c2_b_b;
  if (c2_ak < 0.0) {
    c2_x = c2_bk;
    c2_b_x = c2_x;
    c2_b_x = muDoubleScalarFloor(c2_b_x);
    if (c2_b_x != c2_bk) {
      c2_eml_error(chartInstance);
    }
  }

  c2_c_a = c2_ak;
  c2_c_b = c2_bk;
  c2_eml_scalar_eg(chartInstance);
  c2_ar = c2_c_a;
  c2_br = c2_c_b;
  return muDoubleScalarPower(c2_ar, c2_br);
}

static void c2_eml_error(SFc2_HeartModelECGInstanceStruct *chartInstance)
{
  int32_T c2_i0;
  static char_T c2_cv0[31] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'p', 'o', 'w', 'e', 'r', '_', 'd', 'o', 'm', 'a', 'i',
    'n', 'E', 'r', 'r', 'o', 'r' };

  char_T c2_u[31];
  const mxArray *c2_y = NULL;
  (void)chartInstance;
  for (c2_i0 = 0; c2_i0 < 31; c2_i0++) {
    c2_u[c2_i0] = c2_cv0[c2_i0];
  }

  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", c2_u, 10, 0U, 1U, 0U, 2, 1, 31), false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    1U, 14, c2_y));
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  uint32_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(uint32_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 7, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, false);
  return c2_mxArrayOutData;
}

static uint32_T c2_c_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_AgetEventCounter, const char_T
  *c2_identifier)
{
  uint32_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_AgetEventCounter),
    &c2_thisId);
  sf_mex_destroy(&c2_b_AgetEventCounter);
  return c2_y;
}

static uint32_T c2_d_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint32_T c2_y;
  uint32_T c2_u0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_u0, 1, 7, 0U, 0, 0U, 0);
  c2_y = c2_u0;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_AgetEventCounter;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  uint32_T c2_y;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_b_AgetEventCounter = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_AgetEventCounter),
    &c2_thisId);
  sf_mex_destroy(&c2_b_AgetEventCounter);
  *(uint32_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  int32_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(int32_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, false);
  return c2_mxArrayOutData;
}

static int32_T c2_e_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_y;
  int32_T c2_i1;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_i1, 1, 6, 0U, 0, 0U, 0);
  c2_y = c2_i1;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_sfEvent;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_y;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_b_sfEvent = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData = NULL;
  uint8_T c2_u;
  const mxArray *c2_y = NULL;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_u = *(uint8_T *)c2_inData;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_y, false);
  return c2_mxArrayOutData;
}

static uint8_T c2_f_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_tp_Heart, const char_T *c2_identifier)
{
  uint8_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_tp_Heart),
    &c2_thisId);
  sf_mex_destroy(&c2_b_tp_Heart);
  return c2_y;
}

static uint8_T c2_g_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_y;
  uint8_T c2_u1;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_u1, 1, 3, 0U, 0, 0U, 0);
  c2_y = c2_u1;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_tp_Heart;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  uint8_T c2_y;
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)chartInstanceVoid;
  c2_b_tp_Heart = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_tp_Heart),
    &c2_thisId);
  sf_mex_destroy(&c2_b_tp_Heart);
  *(uint8_T *)c2_outData = c2_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static boolean_T c2_h_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_AVReached, const char_T *c2_identifier)
{
  boolean_T c2_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_y = c2_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_AVReached),
    &c2_thisId);
  sf_mex_destroy(&c2_AVReached);
  return c2_y;
}

static boolean_T c2_i_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  boolean_T c2_y;
  boolean_T c2_b0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_b0, 1, 11, 0U, 0, 0U, 0);
  c2_y = c2_b0;
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static const mxArray *c2_j_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_b_setSimStateSideEffectsInfo, const char_T
  *c2_identifier)
{
  const mxArray *c2_y = NULL;
  emlrtMsgIdentifier c2_thisId;
  c2_y = NULL;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  sf_mex_assign(&c2_y, c2_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_setSimStateSideEffectsInfo), &c2_thisId), false);
  sf_mex_destroy(&c2_b_setSimStateSideEffectsInfo);
  return c2_y;
}

static const mxArray *c2_k_emlrt_marshallIn(SFc2_HeartModelECGInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  const mxArray *c2_y = NULL;
  (void)chartInstance;
  (void)c2_parentId;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_duplicatearraysafe(&c2_u), false);
  sf_mex_destroy(&c2_u);
  return c2_y;
}

static void init_dsm_address_info(SFc2_HeartModelECGInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c2_HeartModelECG_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2296937912U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(800573474U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(656876107U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1707787456U);
}

mxArray *sf_c2_HeartModelECG_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("LxHgVwa3lEflkd7ZPhE2uH");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,31,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,10,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,10,"type",mxType);
    }

    mxSetField(mxData,10,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,11,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,11,"type",mxType);
    }

    mxSetField(mxData,11,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,12,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,12,"type",mxType);
    }

    mxSetField(mxData,12,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,13,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,13,"type",mxType);
    }

    mxSetField(mxData,13,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,14,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,14,"type",mxType);
    }

    mxSetField(mxData,14,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,15,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,15,"type",mxType);
    }

    mxSetField(mxData,15,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,16,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,16,"type",mxType);
    }

    mxSetField(mxData,16,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,17,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,17,"type",mxType);
    }

    mxSetField(mxData,17,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,18,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,18,"type",mxType);
    }

    mxSetField(mxData,18,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,19,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,19,"type",mxType);
    }

    mxSetField(mxData,19,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,20,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,20,"type",mxType);
    }

    mxSetField(mxData,20,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,21,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,21,"type",mxType);
    }

    mxSetField(mxData,21,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,22,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,22,"type",mxType);
    }

    mxSetField(mxData,22,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,23,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,23,"type",mxType);
    }

    mxSetField(mxData,23,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,24,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,24,"type",mxType);
    }

    mxSetField(mxData,24,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,25,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,25,"type",mxType);
    }

    mxSetField(mxData,25,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,26,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,26,"type",mxType);
    }

    mxSetField(mxData,26,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,27,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,27,"type",mxType);
    }

    mxSetField(mxData,27,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,28,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,28,"type",mxType);
    }

    mxSetField(mxData,28,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,29,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,29,"type",mxType);
    }

    mxSetField(mxData,29,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,30,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,30,"type",mxType);
    }

    mxSetField(mxData,30,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c2_HeartModelECG_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c2_HeartModelECG_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c2_HeartModelECG(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x10'type','srcId','name','auxInfo'{{M[3],M[674],T\"avj_refr\",},{M[3],M[675],T\"avj_refr_0\",},{M[3],M[672],T\"avj_t4\",},{M[3],M[384],T\"avj_tRec\",},{M[3],M[558],T\"clk\",},{M[3],M[673],T\"ctime\",},{M[3],M[670],T\"recoverylast\",},{M[3],M[676],T\"refractorylast\",},{M[6],M[802],T\"AVReached\",},{M[6],M[353],T\"Aget\",}}",
    "100 S1x10'type','srcId','name','auxInfo'{{M[6],M[803],T\"Refract\",},{M[6],M[377],T\"Vget\",},{M[7],M[802],T\"AVReachedEventCounter\",},{M[7],M[353],T\"AgetEventCounter\",},{M[7],M[803],T\"RefractEventCounter\",},{M[7],M[377],T\"VgetEventCounter\",},{M[8],M[0],T\"is_active_c2_HeartModelECG\",},{M[8],M[5],T\"is_active_AtrNoiseGenerator\",},{M[8],M[13],T\"is_active_SANode\",},{M[8],M[33],T\"is_active_Atrium\",}}",
    "100 S1x10'type','srcId','name','auxInfo'{{M[8],M[50],T\"is_active_RAConductor\",},{M[8],M[84],T\"is_active_SANodeEctopic\",},{M[8],M[129],T\"is_active_RVConductor\",},{M[8],M[171],T\"is_active_AVJOut\",},{M[8],M[243],T\"is_active_Ventricle\",},{M[8],M[264],T\"is_active_VRG\",},{M[8],M[271],T\"is_active_VRGEctopic\",},{M[8],M[272],T\"is_active_VtrNoiseGenerator\",},{M[8],M[624],T\"is_active_AVJ\",},{M[8],M[653],T\"is_active_A_dV\",}}",
    "100 S1x10'type','srcId','name','auxInfo'{{M[9],M[0],T\"is_c2_HeartModelECG\",},{M[9],M[5],T\"is_AtrNoiseGenerator\",},{M[9],M[13],T\"is_SANode\",},{M[9],M[33],T\"is_Atrium\",},{M[9],M[50],T\"is_RAConductor\",},{M[9],M[84],T\"is_SANodeEctopic\",},{M[9],M[129],T\"is_RVConductor\",},{M[9],M[171],T\"is_AVJOut\",},{M[9],M[243],T\"is_Ventricle\",},{M[9],M[264],T\"is_VRG\",}}",
    "100 S1x10'type','srcId','name','auxInfo'{{M[9],M[271],T\"is_VRGEctopic\",},{M[9],M[272],T\"is_VtrNoiseGenerator\",},{M[9],M[624],T\"is_AVJ\",},{M[9],M[653],T\"is_A_dV\",},{M[11],M[0],T\"temporalCounter_i2\",S'et','os','ct'{{T\"at\",M[7],M[1]}}},{M[11],M[0],T\"temporalCounter_i8\",S'et','os','ct'{{T\"at\",M[14],M[1]}}},{M[11],M[0],T\"temporalCounter_i12\",S'et','os','ct'{{T\"at\",M[34],M[1]}}},{M[11],M[0],T\"temporalCounter_i1\",S'et','os','ct'{{T\"at\",M[85],M[1]}}},{M[11],M[0],T\"temporalCounter_i11\",S'et','os','ct'{{T\"at\",M[245],M[1]}}},{M[11],M[0],T\"temporalCounter_i9\",S'et','os','ct'{{T\"at\",M[268],M[1]}}}}",
    "100 S1x6'type','srcId','name','auxInfo'{{M[11],M[0],T\"temporalCounter_i7\",S'et','os','ct'{{T\"at\",M[274],M[1]}}},{M[11],M[0],T\"temporalCounter_i5\",S'et','os','ct'{{T\"at\",M[277],M[1]}}},{M[11],M[0],T\"temporalCounter_i3\",S'et','os','ct'{{T\"at\",M1x2[52 51],M[1]}}},{M[11],M[0],T\"temporalCounter_i6\",S'et','os','ct'{{T\"at\",M1x2[156 157],M[1]}}},{M[11],M[0],T\"temporalCounter_i4\",S'et','os','ct'{{T\"at\",M1x2[175 172],M[1]}}},{M[11],M[0],T\"temporalCounter_i10\",S'et','os','ct'{{T\"at\",M1x4[632 640 633 625],M[1]}}}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 56, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_HeartModelECG_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_HeartModelECGInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc2_HeartModelECGInstanceStruct *)
      chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _HeartModelECGMachineNumber_,
           2,
           54,
           70,
           0,
           57,
           42,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize ist own list of scripts */
        init_script_number_translation(_HeartModelECGMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_HeartModelECGMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _HeartModelECGMachineNumber_,
            chartInstance->chartNumber,
            42,
            42,
            42);
          _SFD_SET_DATA_PROPS(0,10,0,0,"Atr_refrD");
          _SFD_SET_DATA_PROPS(1,10,0,0,"AV_alpha");
          _SFD_SET_DATA_PROPS(2,10,0,0,"AV_anteDMin");
          _SFD_SET_DATA_PROPS(3,10,0,0,"AV_beta");
          _SFD_SET_DATA_PROPS(4,10,0,0,"AV_delta");
          _SFD_SET_DATA_PROPS(5,10,0,0,"AV_k4");
          _SFD_SET_DATA_PROPS(6,10,0,0,"AV_refrDMin");
          _SFD_SET_DATA_PROPS(7,10,0,0,"AV_retroDMin");
          _SFD_SET_DATA_PROPS(8,10,0,0,"AV_tauC");
          _SFD_SET_DATA_PROPS(9,10,0,0,"AV_theta");
          _SFD_SET_DATA_PROPS(10,10,0,0,"AV_tr");
          _SFD_SET_DATA_PROPS(11,10,0,0,"AV_Vr");
          _SFD_SET_DATA_PROPS(12,10,0,0,"AV_Vt");
          _SFD_SET_DATA_PROPS(13,10,0,0,"RA_anteD");
          _SFD_SET_DATA_PROPS(14,10,0,0,"RA_retroD");
          _SFD_SET_DATA_PROPS(15,10,0,0,"RV_anteD");
          _SFD_SET_DATA_PROPS(16,10,0,0,"RV_retroD");
          _SFD_SET_DATA_PROPS(17,1,1,0,"SA_d");
          _SFD_SET_DATA_PROPS(18,10,0,0,"SA_dV");
          _SFD_SET_DATA_PROPS(19,10,0,0,"SA_ectopD");
          _SFD_SET_DATA_PROPS(20,10,0,0,"SA_ectopdV");
          _SFD_SET_DATA_PROPS(21,10,0,0,"SA_noiseD");
          _SFD_SET_DATA_PROPS(22,10,0,0,"VRG_d");
          _SFD_SET_DATA_PROPS(23,10,0,0,"VRG_ectopD");
          _SFD_SET_DATA_PROPS(24,10,0,0,"VRG_noiseD");
          _SFD_SET_DATA_PROPS(25,10,0,0,"Vtr_refrD");
          _SFD_SET_DATA_PROPS(26,0,0,0,"avj_tRec");
          _SFD_SET_DATA_PROPS(27,0,0,0,"clk");
          _SFD_SET_DATA_PROPS(28,10,0,0,"PM_TAVI");
          _SFD_SET_DATA_PROPS(29,10,0,0,"PM_TLRI");
          _SFD_SET_DATA_PROPS(30,10,0,0,"PM_TPVAB");
          _SFD_SET_DATA_PROPS(31,10,0,0,"PM_TPVARP");
          _SFD_SET_DATA_PROPS(32,10,0,0,"PM_TURI");
          _SFD_SET_DATA_PROPS(33,10,0,0,"PM_TVRP");
          _SFD_SET_DATA_PROPS(34,0,0,0,"recoverylast");
          _SFD_SET_DATA_PROPS(35,0,0,0,"avj_t4");
          _SFD_SET_DATA_PROPS(36,0,0,0,"ctime");
          _SFD_SET_DATA_PROPS(37,0,0,0,"avj_refr");
          _SFD_SET_DATA_PROPS(38,0,0,0,"avj_refr_0");
          _SFD_SET_DATA_PROPS(39,0,0,0,"refractorylast");
          _SFD_SET_DATA_PROPS(40,1,1,0,"QR_delay");
          _SFD_SET_DATA_PROPS(41,1,1,0,"RT_delay");
          _SFD_SET_DATA_PROPS(42,1,1,0,"PQ_delay");
          _SFD_SET_DATA_PROPS(43,9,0,0,"");
          _SFD_SET_DATA_PROPS(44,8,0,0,"");
          _SFD_SET_DATA_PROPS(45,8,0,0,"");
          _SFD_SET_DATA_PROPS(46,9,0,0,"");
          _SFD_SET_DATA_PROPS(47,8,0,0,"");
          _SFD_SET_DATA_PROPS(48,9,0,0,"");
          _SFD_SET_DATA_PROPS(49,8,0,0,"");
          _SFD_SET_DATA_PROPS(50,8,0,0,"");
          _SFD_SET_DATA_PROPS(51,9,0,0,"");
          _SFD_SET_DATA_PROPS(52,8,0,0,"");
          _SFD_SET_DATA_PROPS(53,8,0,0,"");
          _SFD_SET_DATA_PROPS(54,8,0,0,"");
          _SFD_SET_DATA_PROPS(55,9,0,0,"");
          _SFD_SET_DATA_PROPS(56,8,0,0,"");
          _SFD_EVENT_SCOPE(0,0);
          _SFD_EVENT_SCOPE(1,0);
          _SFD_EVENT_SCOPE(2,0);
          _SFD_EVENT_SCOPE(3,0);
          _SFD_EVENT_SCOPE(4,0);
          _SFD_EVENT_SCOPE(5,0);
          _SFD_EVENT_SCOPE(6,0);
          _SFD_EVENT_SCOPE(7,0);
          _SFD_EVENT_SCOPE(8,0);
          _SFD_EVENT_SCOPE(9,0);
          _SFD_EVENT_SCOPE(10,0);
          _SFD_EVENT_SCOPE(11,0);
          _SFD_EVENT_SCOPE(12,0);
          _SFD_EVENT_SCOPE(13,0);
          _SFD_EVENT_SCOPE(14,0);
          _SFD_EVENT_SCOPE(15,0);
          _SFD_EVENT_SCOPE(16,0);
          _SFD_EVENT_SCOPE(17,0);
          _SFD_EVENT_SCOPE(18,0);
          _SFD_EVENT_SCOPE(19,0);
          _SFD_EVENT_SCOPE(20,0);
          _SFD_EVENT_SCOPE(21,0);
          _SFD_EVENT_SCOPE(22,0);
          _SFD_EVENT_SCOPE(23,0);
          _SFD_EVENT_SCOPE(24,0);
          _SFD_EVENT_SCOPE(25,0);
          _SFD_EVENT_SCOPE(26,0);
          _SFD_EVENT_SCOPE(27,0);
          _SFD_EVENT_SCOPE(28,0);
          _SFD_EVENT_SCOPE(29,0);
          _SFD_EVENT_SCOPE(30,0);
          _SFD_EVENT_SCOPE(31,0);
          _SFD_EVENT_SCOPE(32,0);
          _SFD_EVENT_SCOPE(33,0);
          _SFD_EVENT_SCOPE(34,0);
          _SFD_EVENT_SCOPE(35,0);
          _SFD_EVENT_SCOPE(36,0);
          _SFD_EVENT_SCOPE(37,0);
          _SFD_EVENT_SCOPE(38,2);
          _SFD_EVENT_SCOPE(39,2);
          _SFD_EVENT_SCOPE(40,2);
          _SFD_EVENT_SCOPE(41,2);
          _SFD_STATE_INFO(0,1,0);
          _SFD_STATE_INFO(1,0,1);
          _SFD_STATE_INFO(2,0,0);
          _SFD_STATE_INFO(3,0,0);
          _SFD_STATE_INFO(4,0,0);
          _SFD_STATE_INFO(5,0,0);
          _SFD_STATE_INFO(10,0,1);
          _SFD_STATE_INFO(12,0,0);
          _SFD_STATE_INFO(13,0,0);
          _SFD_STATE_INFO(14,0,0);
          _SFD_STATE_INFO(15,0,1);
          _SFD_STATE_INFO(16,0,0);
          _SFD_STATE_INFO(17,0,0);
          _SFD_STATE_INFO(18,0,1);
          _SFD_STATE_INFO(19,0,0);
          _SFD_STATE_INFO(20,0,1);
          _SFD_STATE_INFO(21,0,0);
          _SFD_STATE_INFO(22,0,0);
          _SFD_STATE_INFO(23,0,0);
          _SFD_STATE_INFO(24,0,0);
          _SFD_STATE_INFO(25,0,0);
          _SFD_STATE_INFO(26,0,0);
          _SFD_STATE_INFO(27,0,1);
          _SFD_STATE_INFO(28,0,0);
          _SFD_STATE_INFO(29,0,0);
          _SFD_STATE_INFO(30,0,0);
          _SFD_STATE_INFO(31,0,1);
          _SFD_STATE_INFO(32,0,0);
          _SFD_STATE_INFO(33,0,0);
          _SFD_STATE_INFO(34,0,0);
          _SFD_STATE_INFO(35,0,1);
          _SFD_STATE_INFO(36,0,0);
          _SFD_STATE_INFO(37,0,0);
          _SFD_STATE_INFO(38,0,1);
          _SFD_STATE_INFO(39,0,0);
          _SFD_STATE_INFO(40,0,1);
          _SFD_STATE_INFO(41,0,0);
          _SFD_STATE_INFO(42,0,0);
          _SFD_STATE_INFO(43,0,1);
          _SFD_STATE_INFO(44,0,0);
          _SFD_STATE_INFO(45,0,1);
          _SFD_STATE_INFO(46,0,0);
          _SFD_STATE_INFO(47,0,0);
          _SFD_STATE_INFO(48,0,0);
          _SFD_STATE_INFO(49,0,0);
          _SFD_STATE_INFO(50,0,0);
          _SFD_STATE_INFO(51,0,0);
          _SFD_STATE_INFO(52,0,1);
          _SFD_STATE_INFO(53,0,0);
          _SFD_STATE_INFO(6,0,2);
          _SFD_STATE_INFO(7,0,2);
          _SFD_STATE_INFO(8,0,2);
          _SFD_STATE_INFO(9,0,2);
          _SFD_STATE_INFO(11,0,2);
          _SFD_CH_SUBSTATE_COUNT(1);
          _SFD_CH_SUBSTATE_DECOMP(0);
          _SFD_CH_SUBSTATE_INDEX(0,0);
          _SFD_ST_SUBSTATE_COUNT(0,13);
          _SFD_ST_SUBSTATE_INDEX(0,0,18);
          _SFD_ST_SUBSTATE_INDEX(0,1,38);
          _SFD_ST_SUBSTATE_INDEX(0,2,52);
          _SFD_ST_SUBSTATE_INDEX(0,3,10);
          _SFD_ST_SUBSTATE_INDEX(0,4,31);
          _SFD_ST_SUBSTATE_INDEX(0,5,27);
          _SFD_ST_SUBSTATE_INDEX(0,6,35);
          _SFD_ST_SUBSTATE_INDEX(0,7,45);
          _SFD_ST_SUBSTATE_INDEX(0,8,20);
          _SFD_ST_SUBSTATE_INDEX(0,9,43);
          _SFD_ST_SUBSTATE_INDEX(0,10,40);
          _SFD_ST_SUBSTATE_INDEX(0,11,1);
          _SFD_ST_SUBSTATE_INDEX(0,12,15);
          _SFD_ST_SUBSTATE_COUNT(18,1);
          _SFD_ST_SUBSTATE_INDEX(18,0,19);
          _SFD_ST_SUBSTATE_COUNT(19,0);
          _SFD_ST_SUBSTATE_COUNT(38,1);
          _SFD_ST_SUBSTATE_INDEX(38,0,39);
          _SFD_ST_SUBSTATE_COUNT(39,0);
          _SFD_ST_SUBSTATE_COUNT(52,1);
          _SFD_ST_SUBSTATE_INDEX(52,0,53);
          _SFD_ST_SUBSTATE_COUNT(53,0);
          _SFD_ST_SUBSTATE_COUNT(10,3);
          _SFD_ST_SUBSTATE_INDEX(10,0,12);
          _SFD_ST_SUBSTATE_INDEX(10,1,13);
          _SFD_ST_SUBSTATE_INDEX(10,2,14);
          _SFD_ST_SUBSTATE_COUNT(12,0);
          _SFD_ST_SUBSTATE_COUNT(13,0);
          _SFD_ST_SUBSTATE_COUNT(14,0);
          _SFD_ST_SUBSTATE_COUNT(31,3);
          _SFD_ST_SUBSTATE_INDEX(31,0,32);
          _SFD_ST_SUBSTATE_INDEX(31,1,33);
          _SFD_ST_SUBSTATE_INDEX(31,2,34);
          _SFD_ST_SUBSTATE_COUNT(32,0);
          _SFD_ST_SUBSTATE_COUNT(33,0);
          _SFD_ST_SUBSTATE_COUNT(34,0);
          _SFD_ST_SUBSTATE_COUNT(27,3);
          _SFD_ST_SUBSTATE_INDEX(27,0,28);
          _SFD_ST_SUBSTATE_INDEX(27,1,29);
          _SFD_ST_SUBSTATE_INDEX(27,2,30);
          _SFD_ST_SUBSTATE_COUNT(28,0);
          _SFD_ST_SUBSTATE_COUNT(29,0);
          _SFD_ST_SUBSTATE_COUNT(30,0);
          _SFD_ST_SUBSTATE_COUNT(35,2);
          _SFD_ST_SUBSTATE_INDEX(35,0,36);
          _SFD_ST_SUBSTATE_INDEX(35,1,37);
          _SFD_ST_SUBSTATE_COUNT(36,0);
          _SFD_ST_SUBSTATE_COUNT(37,0);
          _SFD_ST_SUBSTATE_COUNT(45,6);
          _SFD_ST_SUBSTATE_INDEX(45,0,46);
          _SFD_ST_SUBSTATE_INDEX(45,1,47);
          _SFD_ST_SUBSTATE_INDEX(45,2,48);
          _SFD_ST_SUBSTATE_INDEX(45,3,49);
          _SFD_ST_SUBSTATE_INDEX(45,4,50);
          _SFD_ST_SUBSTATE_INDEX(45,5,51);
          _SFD_ST_SUBSTATE_COUNT(46,0);
          _SFD_ST_SUBSTATE_COUNT(47,0);
          _SFD_ST_SUBSTATE_COUNT(48,0);
          _SFD_ST_SUBSTATE_COUNT(49,0);
          _SFD_ST_SUBSTATE_COUNT(50,0);
          _SFD_ST_SUBSTATE_COUNT(51,0);
          _SFD_ST_SUBSTATE_COUNT(20,6);
          _SFD_ST_SUBSTATE_INDEX(20,0,21);
          _SFD_ST_SUBSTATE_INDEX(20,1,22);
          _SFD_ST_SUBSTATE_INDEX(20,2,23);
          _SFD_ST_SUBSTATE_INDEX(20,3,24);
          _SFD_ST_SUBSTATE_INDEX(20,4,25);
          _SFD_ST_SUBSTATE_INDEX(20,5,26);
          _SFD_ST_SUBSTATE_COUNT(21,0);
          _SFD_ST_SUBSTATE_COUNT(22,0);
          _SFD_ST_SUBSTATE_COUNT(23,0);
          _SFD_ST_SUBSTATE_COUNT(24,0);
          _SFD_ST_SUBSTATE_COUNT(25,0);
          _SFD_ST_SUBSTATE_COUNT(26,0);
          _SFD_ST_SUBSTATE_COUNT(43,1);
          _SFD_ST_SUBSTATE_INDEX(43,0,44);
          _SFD_ST_SUBSTATE_COUNT(44,0);
          _SFD_ST_SUBSTATE_COUNT(40,2);
          _SFD_ST_SUBSTATE_INDEX(40,0,41);
          _SFD_ST_SUBSTATE_INDEX(40,1,42);
          _SFD_ST_SUBSTATE_COUNT(41,0);
          _SFD_ST_SUBSTATE_COUNT(42,0);
          _SFD_ST_SUBSTATE_COUNT(1,4);
          _SFD_ST_SUBSTATE_INDEX(1,0,2);
          _SFD_ST_SUBSTATE_INDEX(1,1,3);
          _SFD_ST_SUBSTATE_INDEX(1,2,4);
          _SFD_ST_SUBSTATE_INDEX(1,3,5);
          _SFD_ST_SUBSTATE_COUNT(2,0);
          _SFD_ST_SUBSTATE_COUNT(3,0);
          _SFD_ST_SUBSTATE_COUNT(4,0);
          _SFD_ST_SUBSTATE_COUNT(5,0);
          _SFD_ST_SUBSTATE_COUNT(15,2);
          _SFD_ST_SUBSTATE_INDEX(15,0,16);
          _SFD_ST_SUBSTATE_INDEX(15,1,17);
          _SFD_ST_SUBSTATE_COUNT(16,0);
          _SFD_ST_SUBSTATE_COUNT(17,0);
        }

        _SFD_CV_INIT_CHART(1,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,13,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(1,4,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(2,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(3,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(4,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(5,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(10,3,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(12,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(13,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(14,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(15,2,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(16,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(17,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(18,1,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(19,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(20,6,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(21,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(22,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(23,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(24,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(25,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(26,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(27,3,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(28,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(29,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(30,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(31,3,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(32,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(33,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(34,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(35,2,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(36,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(37,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(38,1,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(39,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(40,2,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(41,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(42,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(43,1,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(44,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(45,6,1,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(46,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(47,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(48,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(49,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(50,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(51,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(52,1,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(53,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(6,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(7,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(8,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(9,0,0,0,0,0,NULL,NULL);
        }

        {
          _SFD_CV_INIT_STATE(11,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(69,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(63,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 6 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(66,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 6 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(65,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 10 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(64,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(18,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 17, 0 };

          static unsigned int sEndGuardMap[] = { 21, 22 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(17,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(14,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 17, 0 };

          static unsigned int sEndGuardMap[] = { 21, 22 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(0,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(16,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 3 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(13,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 8 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(9,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 8 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(8,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 3 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(11,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 16, 0 };

          static unsigned int sEndGuardMap[] = { 20, 21 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(12,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 15, 0 };

          static unsigned int sEndGuardMap[] = { 19, 20 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(10,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(27,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 44, 0 };

          static unsigned int sEndGuardMap[] = { 48, 49 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(29,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(28,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 42, 0 };

          static unsigned int sEndGuardMap[] = { 46, 47 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(31,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 12 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(30,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(44,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 18, 0 };

          static unsigned int sEndGuardMap[] = { 22, 23 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(45,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(26,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 7 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(21,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 3 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(25,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 3 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(52,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 17, 0 };

          static unsigned int sEndGuardMap[] = { 21, 22 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(22,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 7 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(24,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 16, 0 };

          static unsigned int sEndGuardMap[] = { 20, 21 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(23,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 18, 0 };

          static unsigned int sEndGuardMap[] = { 22, 23 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(43,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(42,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(2,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 12, 0 };

          static unsigned int sEndGuardMap[] = { 16, 17 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(1,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(51,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(19,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(38,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 13, 0 };

          static unsigned int sEndGuardMap[] = { 17, 18 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(39,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(40,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 11 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(50,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(60,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 13, 0 };

          static unsigned int sEndGuardMap[] = { 17, 18 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(61,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 14 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(62,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 18 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(67,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 15, 0 };

          static unsigned int sEndGuardMap[] = { 19, 20 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(54,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 15 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(55,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 13, 0 };

          static unsigned int sEndGuardMap[] = { 17, 18 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(56,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 18 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(68,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 14 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(57,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 15 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(58,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(59,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(41,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 15, 0 };

          static unsigned int sEndGuardMap[] = { 19, 20 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(32,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 5 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(33,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 14 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(34,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(35,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 2 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(36,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(48,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(37,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(47,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(15,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 5 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(3,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 16, 0 };

          static unsigned int sEndGuardMap[] = { 20, 21 };

          static int sPostFixPredicateTree[] = { 0, 1, -3 };

          _SFD_CV_INIT_TRANS(6,2,&(sStartGuardMap[0]),&(sEndGuardMap[0]),3,
                             &(sPostFixPredicateTree[0]));
        }

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 15 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(4,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(20,0,NULL,NULL,0,NULL);

        {
          static unsigned int sStartGuardMap[] = { 0 };

          static unsigned int sEndGuardMap[] = { 2 };

          static int sPostFixPredicateTree[] = { 0 };

          _SFD_CV_INIT_TRANS(5,1,&(sStartGuardMap[0]),&(sEndGuardMap[0]),1,
                             &(sPostFixPredicateTree[0]));
        }

        _SFD_CV_INIT_TRANS(46,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(7,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(53,0,NULL,NULL,0,NULL);
        _SFD_CV_INIT_TRANS(49,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(11,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(11,0,"AVJDelay",0,-1,83);
        _SFD_CV_INIT_EML(9,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(9,0,"timeToDep",0,-1,64);
        _SFD_CV_INIT_EML(6,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(6,0,"initRefrPeriod",0,-1,92);
        _SFD_CV_INIT_EML(8,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(8,0,"modulateRefrRetro",1,-1,86);
        _SFD_CV_INIT_EML(7,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(7,0,"modulateRefrAnte",0,-1,219);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(12,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(13,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(14,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(15,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(16,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(17,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(18,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(19,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(20,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(21,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(22,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(23,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(24,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(25,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(26,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(27,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(28,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(29,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(30,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(31,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(32,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(33,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(34,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(35,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(36,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(37,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(38,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(39,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(40,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(41,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(42,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(43,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(44,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(45,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(46,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(47,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(48,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(49,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(50,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(51,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(52,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(53,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(54,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(55,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(56,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)c2_sf_marshallIn);
        _SFD_SET_DATA_VALUE_PTR(43,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(44,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(45,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(46,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(47,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(48,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(49,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(50,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(51,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(52,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(53,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(54,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(55,(void *)(NULL));
        _SFD_SET_DATA_VALUE_PTR(56,(void *)(NULL));

        {
          real_T *c2_SA_d;
          real_T *c2_QR_delay;
          real_T *c2_RT_delay;
          real_T *c2_PQ_delay;
          c2_PQ_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c2_RT_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c2_QR_delay = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c2_SA_d = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, &chartInstance->c2_Atr_refrD);
          _SFD_SET_DATA_VALUE_PTR(1U, &chartInstance->c2_AV_alpha);
          _SFD_SET_DATA_VALUE_PTR(2U, &chartInstance->c2_AV_anteDMin);
          _SFD_SET_DATA_VALUE_PTR(3U, &chartInstance->c2_AV_beta);
          _SFD_SET_DATA_VALUE_PTR(4U, &chartInstance->c2_AV_delta);
          _SFD_SET_DATA_VALUE_PTR(5U, &chartInstance->c2_AV_k4);
          _SFD_SET_DATA_VALUE_PTR(6U, &chartInstance->c2_AV_refrDMin);
          _SFD_SET_DATA_VALUE_PTR(7U, &chartInstance->c2_AV_retroDMin);
          _SFD_SET_DATA_VALUE_PTR(8U, &chartInstance->c2_AV_tauC);
          _SFD_SET_DATA_VALUE_PTR(9U, &chartInstance->c2_AV_theta);
          _SFD_SET_DATA_VALUE_PTR(10U, &chartInstance->c2_AV_tr);
          _SFD_SET_DATA_VALUE_PTR(11U, &chartInstance->c2_AV_Vr);
          _SFD_SET_DATA_VALUE_PTR(12U, &chartInstance->c2_AV_Vt);
          _SFD_SET_DATA_VALUE_PTR(13U, &chartInstance->c2_RA_anteD);
          _SFD_SET_DATA_VALUE_PTR(14U, &chartInstance->c2_RA_retroD);
          _SFD_SET_DATA_VALUE_PTR(15U, &chartInstance->c2_RV_anteD);
          _SFD_SET_DATA_VALUE_PTR(16U, &chartInstance->c2_RV_retroD);
          _SFD_SET_DATA_VALUE_PTR(17U, c2_SA_d);
          _SFD_SET_DATA_VALUE_PTR(18U, &chartInstance->c2_SA_dV);
          _SFD_SET_DATA_VALUE_PTR(19U, &chartInstance->c2_SA_ectopD);
          _SFD_SET_DATA_VALUE_PTR(20U, &chartInstance->c2_SA_ectopdV);
          _SFD_SET_DATA_VALUE_PTR(21U, &chartInstance->c2_SA_noiseD);
          _SFD_SET_DATA_VALUE_PTR(22U, &chartInstance->c2_VRG_d);
          _SFD_SET_DATA_VALUE_PTR(23U, &chartInstance->c2_VRG_ectopD);
          _SFD_SET_DATA_VALUE_PTR(24U, &chartInstance->c2_VRG_noiseD);
          _SFD_SET_DATA_VALUE_PTR(25U, &chartInstance->c2_Vtr_refrD);
          _SFD_SET_DATA_VALUE_PTR(26U, &chartInstance->c2_avj_tRec);
          _SFD_SET_DATA_VALUE_PTR(27U, &chartInstance->c2_clk);
          _SFD_SET_DATA_VALUE_PTR(28U, &chartInstance->c2_PM_TAVI);
          _SFD_SET_DATA_VALUE_PTR(29U, &chartInstance->c2_PM_TLRI);
          _SFD_SET_DATA_VALUE_PTR(30U, &chartInstance->c2_PM_TPVAB);
          _SFD_SET_DATA_VALUE_PTR(31U, &chartInstance->c2_PM_TPVARP);
          _SFD_SET_DATA_VALUE_PTR(32U, &chartInstance->c2_PM_TURI);
          _SFD_SET_DATA_VALUE_PTR(33U, &chartInstance->c2_PM_TVRP);
          _SFD_SET_DATA_VALUE_PTR(34U, &chartInstance->c2_recoverylast);
          _SFD_SET_DATA_VALUE_PTR(35U, &chartInstance->c2_avj_t4);
          _SFD_SET_DATA_VALUE_PTR(36U, &chartInstance->c2_ctime);
          _SFD_SET_DATA_VALUE_PTR(37U, &chartInstance->c2_avj_refr);
          _SFD_SET_DATA_VALUE_PTR(38U, &chartInstance->c2_avj_refr_0);
          _SFD_SET_DATA_VALUE_PTR(39U, &chartInstance->c2_refractorylast);
          _SFD_SET_DATA_VALUE_PTR(40U, c2_QR_delay);
          _SFD_SET_DATA_VALUE_PTR(41U, c2_RT_delay);
          _SFD_SET_DATA_VALUE_PTR(42U, c2_PQ_delay);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _HeartModelECGMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "oJecBbo3mfT93mMB83nTGF";
}

static void sf_opaque_initialize_c2_HeartModelECG(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc2_HeartModelECGInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
    chartInstanceVar);
  initialize_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c2_HeartModelECG(void *chartInstanceVar)
{
  enable_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c2_HeartModelECG(void *chartInstanceVar)
{
  disable_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c2_HeartModelECG(void *chartInstanceVar)
{
  sf_gateway_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c2_HeartModelECG(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c2_HeartModelECG
    ((SFc2_HeartModelECGInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c2_HeartModelECG();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_raw2high'.\n");
  }

  return plhs[0];
}

extern void sf_internal_set_sim_state_c2_HeartModelECG(SimStruct* S, const
  mxArray *st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[3];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxDuplicateArray(st);      /* high level simctx */
  prhs[2] = (mxArray*) sf_get_sim_state_info_c2_HeartModelECG();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 3, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c2_HeartModelECG(SimStruct* S)
{
  return sf_internal_get_sim_state_c2_HeartModelECG(S);
}

static void sf_opaque_set_sim_state_c2_HeartModelECG(SimStruct* S, const mxArray
  *st)
{
  sf_internal_set_sim_state_c2_HeartModelECG(S, st);
}

static void sf_opaque_terminate_c2_HeartModelECG(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_HeartModelECGInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_HeartModelECG_optimization_info();
    }

    finalize_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    if (crtInfo != NULL) {
      utFree((void *)crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_HeartModelECG(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c2_HeartModelECG((SFc2_HeartModelECGInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c2_HeartModelECG(SimStruct *S)
{
  /* Actual parameters from chart:
     AV_Vr AV_Vt AV_alpha AV_anteDMin AV_beta AV_delta AV_k4 AV_refrDMin AV_retroDMin AV_tauC AV_theta AV_tr Atr_refrD PM_TAVI PM_TLRI PM_TPVAB PM_TPVARP PM_TURI PM_TVRP RA_anteD RA_retroD RV_anteD RV_retroD SA_dV SA_ectopD SA_ectopdV SA_noiseD VRG_d VRG_ectopD VRG_noiseD Vtr_refrD
   */
  const char_T *rtParamNames[] = { "AV_Vr", "AV_Vt", "AV_alpha", "AV_anteDMin",
    "AV_beta", "AV_delta", "AV_k4", "AV_refrDMin", "AV_retroDMin", "AV_tauC",
    "AV_theta", "AV_tr", "Atr_refrD", "PM_TAVI", "PM_TLRI", "PM_TPVAB",
    "PM_TPVARP", "PM_TURI", "PM_TVRP", "RA_anteD", "RA_retroD", "RV_anteD",
    "RV_retroD", "SA_dV", "SA_ectopD", "SA_ectopdV", "SA_noiseD", "VRG_d",
    "VRG_ectopD", "VRG_noiseD", "Vtr_refrD" };

  ssSetNumRunTimeParams(S,ssGetSFcnParamsCount(S));

  /* registration for AV_Vr*/
  ssRegDlgParamAsRunTimeParam(S, 0, 0, rtParamNames[0], SS_DOUBLE);

  /* registration for AV_Vt*/
  ssRegDlgParamAsRunTimeParam(S, 1, 1, rtParamNames[1], SS_DOUBLE);

  /* registration for AV_alpha*/
  ssRegDlgParamAsRunTimeParam(S, 2, 2, rtParamNames[2], SS_DOUBLE);

  /* registration for AV_anteDMin*/
  ssRegDlgParamAsRunTimeParam(S, 3, 3, rtParamNames[3], SS_DOUBLE);

  /* registration for AV_beta*/
  ssRegDlgParamAsRunTimeParam(S, 4, 4, rtParamNames[4], SS_DOUBLE);

  /* registration for AV_delta*/
  ssRegDlgParamAsRunTimeParam(S, 5, 5, rtParamNames[5], SS_DOUBLE);

  /* registration for AV_k4*/
  ssRegDlgParamAsRunTimeParam(S, 6, 6, rtParamNames[6], SS_DOUBLE);

  /* registration for AV_refrDMin*/
  ssRegDlgParamAsRunTimeParam(S, 7, 7, rtParamNames[7], SS_DOUBLE);

  /* registration for AV_retroDMin*/
  ssRegDlgParamAsRunTimeParam(S, 8, 8, rtParamNames[8], SS_DOUBLE);

  /* registration for AV_tauC*/
  ssRegDlgParamAsRunTimeParam(S, 9, 9, rtParamNames[9], SS_DOUBLE);

  /* registration for AV_theta*/
  ssRegDlgParamAsRunTimeParam(S, 10, 10, rtParamNames[10], SS_DOUBLE);

  /* registration for AV_tr*/
  ssRegDlgParamAsRunTimeParam(S, 11, 11, rtParamNames[11], SS_DOUBLE);

  /* registration for Atr_refrD*/
  ssRegDlgParamAsRunTimeParam(S, 12, 12, rtParamNames[12], SS_DOUBLE);

  /* registration for PM_TAVI*/
  ssRegDlgParamAsRunTimeParam(S, 13, 13, rtParamNames[13], SS_DOUBLE);

  /* registration for PM_TLRI*/
  ssRegDlgParamAsRunTimeParam(S, 14, 14, rtParamNames[14], SS_DOUBLE);

  /* registration for PM_TPVAB*/
  ssRegDlgParamAsRunTimeParam(S, 15, 15, rtParamNames[15], SS_DOUBLE);

  /* registration for PM_TPVARP*/
  ssRegDlgParamAsRunTimeParam(S, 16, 16, rtParamNames[16], SS_DOUBLE);

  /* registration for PM_TURI*/
  ssRegDlgParamAsRunTimeParam(S, 17, 17, rtParamNames[17], SS_DOUBLE);

  /* registration for PM_TVRP*/
  ssRegDlgParamAsRunTimeParam(S, 18, 18, rtParamNames[18], SS_DOUBLE);

  /* registration for RA_anteD*/
  ssRegDlgParamAsRunTimeParam(S, 19, 19, rtParamNames[19], SS_DOUBLE);

  /* registration for RA_retroD*/
  ssRegDlgParamAsRunTimeParam(S, 20, 20, rtParamNames[20], SS_DOUBLE);

  /* registration for RV_anteD*/
  ssRegDlgParamAsRunTimeParam(S, 21, 21, rtParamNames[21], SS_DOUBLE);

  /* registration for RV_retroD*/
  ssRegDlgParamAsRunTimeParam(S, 22, 22, rtParamNames[22], SS_DOUBLE);

  /* registration for SA_dV*/
  ssRegDlgParamAsRunTimeParam(S, 23, 23, rtParamNames[23], SS_DOUBLE);

  /* registration for SA_ectopD*/
  ssRegDlgParamAsRunTimeParam(S, 24, 24, rtParamNames[24], SS_DOUBLE);

  /* registration for SA_ectopdV*/
  ssRegDlgParamAsRunTimeParam(S, 25, 25, rtParamNames[25], SS_DOUBLE);

  /* registration for SA_noiseD*/
  ssRegDlgParamAsRunTimeParam(S, 26, 26, rtParamNames[26], SS_DOUBLE);

  /* registration for VRG_d*/
  ssRegDlgParamAsRunTimeParam(S, 27, 27, rtParamNames[27], SS_DOUBLE);

  /* registration for VRG_ectopD*/
  ssRegDlgParamAsRunTimeParam(S, 28, 28, rtParamNames[28], SS_DOUBLE);

  /* registration for VRG_noiseD*/
  ssRegDlgParamAsRunTimeParam(S, 29, 29, rtParamNames[29], SS_DOUBLE);

  /* registration for Vtr_refrD*/
  ssRegDlgParamAsRunTimeParam(S, 30, 30, rtParamNames[30], SS_DOUBLE);
  ssSetModelReferenceSampleTimeDisallowInheritance(S);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_HeartModelECG_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(sf_get_instance_specialization(),
                infoStruct,2,"RTWCG"));
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,2,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,2);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,4);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=4; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 4; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(1920026790U));
  ssSetChecksum1(S,(651573269U));
  ssSetChecksum2(S,(1891328245U));
  ssSetChecksum3(S,(1990617368U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c2_HeartModelECG(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Stateflow");
  }
}

static void mdlStart_c2_HeartModelECG(SimStruct *S)
{
  SFc2_HeartModelECGInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc2_HeartModelECGInstanceStruct *)utMalloc(sizeof
    (SFc2_HeartModelECGInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc2_HeartModelECGInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 0;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c2_HeartModelECG;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c2_HeartModelECG;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c2_HeartModelECG;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c2_HeartModelECG;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c2_HeartModelECG;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c2_HeartModelECG;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c2_HeartModelECG;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c2_HeartModelECG;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_HeartModelECG;
  chartInstance->chartInfo.mdlStart = mdlStart_c2_HeartModelECG;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c2_HeartModelECG;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c2_HeartModelECG_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_HeartModelECG(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_HeartModelECG(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_HeartModelECG(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_HeartModelECG_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
