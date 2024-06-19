/* Include files */

#include "HeartModelECG_sfun.h"
#include "HeartModelECG_sfun_debug_macros.h"
#include "c1_HeartModelECG.h"
#include "c2_HeartModelECG.h"
#include "c3_HeartModelECG.h"
#include "c4_HeartModelECG.h"
#include "c5_HeartModelECG.h"
#include "c6_HeartModelECG.h"
#include "c7_HeartModelECG.h"
#include "c8_HeartModelECG.h"
#include "c9_HeartModelECG.h"
#include "c10_HeartModelECG.h"
#include "c11_HeartModelECG.h"
#include "c12_HeartModelECG.h"
#include "c13_HeartModelECG.h"
#include "c14_HeartModelECG.h"
#include "c15_HeartModelECG.h"
#include "c16_HeartModelECG.h"
#include "c17_HeartModelECG.h"
#include "c18_HeartModelECG.h"
#include "c19_HeartModelECG.h"
#include "c20_HeartModelECG.h"
#include "c21_HeartModelECG.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _HeartModelECGMachineNumber_;

/* Function Declarations */

/* Function Definitions */
void HeartModelECG_initializer(void)
{
}

void HeartModelECG_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_HeartModelECG_method_dispatcher(SimStruct *simstructPtr,
  unsigned int chartFileNumber, const char* specsCksum, int_T method, void *data)
{
  if (chartFileNumber==1) {
    c1_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==2) {
    c2_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==3) {
    c3_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==4) {
    c4_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==5) {
    c5_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==6) {
    c6_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==7) {
    c7_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==8) {
    c8_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==9) {
    c9_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==10) {
    c10_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==11) {
    c11_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==12) {
    c12_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==13) {
    c13_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==14) {
    c14_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==15) {
    c15_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==16) {
    c16_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==17) {
    c17_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==18) {
    c18_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==19) {
    c19_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==20) {
    c20_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==21) {
    c21_HeartModelECG_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_HeartModelECG_process_check_sum_call( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[20];
  if (nrhs<1 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the checksum */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"sf_get_check_sum"))
    return 0;
  plhs[0] = mxCreateDoubleMatrix( 1,4,mxREAL);
  if (nrhs>1 && mxIsChar(prhs[1])) {
    mxGetString(prhs[1], commandName,sizeof(commandName)/sizeof(char));
    commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
    if (!strcmp(commandName,"machine")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2924270614U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(3652328058U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3299781183U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1312367627U);
    } else if (!strcmp(commandName,"exportedFcn")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0U);
    } else if (!strcmp(commandName,"makefile")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(554052517U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2880077197U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1464277993U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(928734424U);
    } else if (nrhs==3 && !strcmp(commandName,"chart")) {
      unsigned int chartFileNumber;
      chartFileNumber = (unsigned int)mxGetScalar(prhs[2]);
      switch (chartFileNumber) {
       case 1:
        {
          extern void sf_c1_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c1_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 2:
        {
          extern void sf_c2_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c2_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 3:
        {
          extern void sf_c3_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c3_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 4:
        {
          extern void sf_c4_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c4_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 5:
        {
          extern void sf_c5_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c5_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 6:
        {
          extern void sf_c6_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c6_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 7:
        {
          extern void sf_c7_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c7_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 8:
        {
          extern void sf_c8_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c8_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 9:
        {
          extern void sf_c9_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c9_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 10:
        {
          extern void sf_c10_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c10_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 11:
        {
          extern void sf_c11_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c11_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 12:
        {
          extern void sf_c12_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c12_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 13:
        {
          extern void sf_c13_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c13_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 14:
        {
          extern void sf_c14_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c14_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 15:
        {
          extern void sf_c15_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c15_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 16:
        {
          extern void sf_c16_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c16_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 17:
        {
          extern void sf_c17_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c17_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 18:
        {
          extern void sf_c18_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c18_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 19:
        {
          extern void sf_c19_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c19_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 20:
        {
          extern void sf_c20_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c20_HeartModelECG_get_check_sum(plhs);
          break;
        }

       case 21:
        {
          extern void sf_c21_HeartModelECG_get_check_sum(mxArray *plhs[]);
          sf_c21_HeartModelECG_get_check_sum(plhs);
          break;
        }

       default:
        ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0.0);
      }
    } else if (!strcmp(commandName,"target")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2083502392U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1110276785U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3258378658U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(3926592909U);
    } else {
      return 0;
    }
  } else {
    ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1169975514U);
    ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(406848004U);
    ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3551419599U);
    ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1132384689U);
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_HeartModelECG_autoinheritance_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[32];
  char aiChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the autoinheritance_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_autoinheritance_info"))
    return 0;
  mxGetString(prhs[2], aiChksum,sizeof(aiChksum)/sizeof(char));
  aiChksum[(sizeof(aiChksum)/sizeof(char)-1)] = '\0';

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(aiChksum, "tfAKclESNdNSAp0D0hpzHG") == 0) {
          extern mxArray *sf_c1_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c1_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 2:
      {
        if (strcmp(aiChksum, "LxHgVwa3lEflkd7ZPhE2uH") == 0) {
          extern mxArray *sf_c2_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c2_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 3:
      {
        if (strcmp(aiChksum, "VsByuXYAXj4Xh8KdXQSDoD") == 0) {
          extern mxArray *sf_c3_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c3_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 4:
      {
        if (strcmp(aiChksum, "XWhkO9oPv48GLRrlTVGNYD") == 0) {
          extern mxArray *sf_c4_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c4_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 5:
      {
        if (strcmp(aiChksum, "wCeAQnYuGMeIfBqiUD61KC") == 0) {
          extern mxArray *sf_c5_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c5_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 6:
      {
        if (strcmp(aiChksum, "AqfFAGMspLYyzfQXAJtmEE") == 0) {
          extern mxArray *sf_c6_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c6_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 7:
      {
        if (strcmp(aiChksum, "WlcOzGH4z1Tbv9ty8oohmF") == 0) {
          extern mxArray *sf_c7_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c7_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 8:
      {
        if (strcmp(aiChksum, "81IERSrJ6VCdlcDDw7hY2D") == 0) {
          extern mxArray *sf_c8_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c8_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 9:
      {
        if (strcmp(aiChksum, "1QdW0N71XmVJpiJwzqgDLC") == 0) {
          extern mxArray *sf_c9_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c9_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 10:
      {
        if (strcmp(aiChksum, "67YbHXpVi9sEgBUUy9CHlF") == 0) {
          extern mxArray *sf_c10_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c10_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 11:
      {
        if (strcmp(aiChksum, "FcJErXpObojvov4RzoWcaH") == 0) {
          extern mxArray *sf_c11_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c11_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 12:
      {
        if (strcmp(aiChksum, "zEoqPpb765j1zJOqw48aL") == 0) {
          extern mxArray *sf_c12_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c12_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 13:
      {
        if (strcmp(aiChksum, "AMjA9AbjR9S7PON0Jt2HcE") == 0) {
          extern mxArray *sf_c13_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c13_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 14:
      {
        if (strcmp(aiChksum, "D6kXHjeUbTkp6UFQHfiBY") == 0) {
          extern mxArray *sf_c14_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c14_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 15:
      {
        if (strcmp(aiChksum, "BNj3CE1RKFal2O6Lfb8eHD") == 0) {
          extern mxArray *sf_c15_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c15_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 16:
      {
        if (strcmp(aiChksum, "x6isv6RGHvxJ0w9u9ZIC") == 0) {
          extern mxArray *sf_c16_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c16_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 17:
      {
        if (strcmp(aiChksum, "FG0Nh4V2lxOwldTm7763dG") == 0) {
          extern mxArray *sf_c17_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c17_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 18:
      {
        if (strcmp(aiChksum, "FG0Nh4V2lxOwldTm7763dG") == 0) {
          extern mxArray *sf_c18_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c18_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 19:
      {
        if (strcmp(aiChksum, "FG0Nh4V2lxOwldTm7763dG") == 0) {
          extern mxArray *sf_c19_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c19_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 20:
      {
        if (strcmp(aiChksum, "FG0Nh4V2lxOwldTm7763dG") == 0) {
          extern mxArray *sf_c20_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c20_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 21:
      {
        if (strcmp(aiChksum, "FG0Nh4V2lxOwldTm7763dG") == 0) {
          extern mxArray *sf_c21_HeartModelECG_get_autoinheritance_info(void);
          plhs[0] = sf_c21_HeartModelECG_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_HeartModelECG_get_eml_resolved_functions_info( int nlhs, mxArray
  * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[64];
  if (nrhs<2 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the get_eml_resolved_functions_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_eml_resolved_functions_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        extern const mxArray
          *sf_c1_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c1_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 2:
      {
        extern const mxArray
          *sf_c2_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c2_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 3:
      {
        extern const mxArray
          *sf_c3_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c3_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 4:
      {
        extern const mxArray
          *sf_c4_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c4_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 5:
      {
        extern const mxArray
          *sf_c5_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c5_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 6:
      {
        extern const mxArray
          *sf_c6_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c6_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 7:
      {
        extern const mxArray
          *sf_c7_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c7_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 8:
      {
        extern const mxArray
          *sf_c8_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c8_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 9:
      {
        extern const mxArray
          *sf_c9_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c9_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 10:
      {
        extern const mxArray
          *sf_c10_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c10_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 11:
      {
        extern const mxArray
          *sf_c11_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c11_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 12:
      {
        extern const mxArray
          *sf_c12_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c12_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 13:
      {
        extern const mxArray
          *sf_c13_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c13_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 14:
      {
        extern const mxArray
          *sf_c14_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c14_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 15:
      {
        extern const mxArray
          *sf_c15_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c15_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 16:
      {
        extern const mxArray
          *sf_c16_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c16_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 17:
      {
        extern const mxArray
          *sf_c17_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c17_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 18:
      {
        extern const mxArray
          *sf_c18_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c18_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 19:
      {
        extern const mxArray
          *sf_c19_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c19_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 20:
      {
        extern const mxArray
          *sf_c20_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c20_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 21:
      {
        extern const mxArray
          *sf_c21_HeartModelECG_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c21_HeartModelECG_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_HeartModelECG_third_party_uses_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the third_party_uses_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_third_party_uses_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "NgagU6DvgJECK1wOxq40CD") == 0) {
          extern mxArray *sf_c1_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c1_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "oJecBbo3mfT93mMB83nTGF") == 0) {
          extern mxArray *sf_c2_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c2_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "iGxkkdOdxvxinp4ltbVV0D") == 0) {
          extern mxArray *sf_c3_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c3_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "qiR5ycYnvQWT42lgceA3nB") == 0) {
          extern mxArray *sf_c4_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c4_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "LZRpOoxz3go9NqZi0TMwYH") == 0) {
          extern mxArray *sf_c5_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c5_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "5KERGKQ2WGDsl3dzyd1RSB") == 0) {
          extern mxArray *sf_c6_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c6_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "lpsK3luMh2VL8FJQKbDP9F") == 0) {
          extern mxArray *sf_c7_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c7_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 8:
      {
        if (strcmp(tpChksum, "zK7ecCBNq0wD3Cn41oRbUF") == 0) {
          extern mxArray *sf_c8_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c8_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 9:
      {
        if (strcmp(tpChksum, "RO3E5sQdbTzAzLtlIv02mF") == 0) {
          extern mxArray *sf_c9_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c9_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 10:
      {
        if (strcmp(tpChksum, "Dhw0UYLVJiB18rFKCN8vQG") == 0) {
          extern mxArray *sf_c10_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c10_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 11:
      {
        if (strcmp(tpChksum, "uBOvtAM6vHDCnp9hZ6rMqF") == 0) {
          extern mxArray *sf_c11_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c11_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 12:
      {
        if (strcmp(tpChksum, "tJ8yvZvTd6aXPMx7wkvAi") == 0) {
          extern mxArray *sf_c12_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c12_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 13:
      {
        if (strcmp(tpChksum, "KcOJKPl9jb5T3UBP96q1m") == 0) {
          extern mxArray *sf_c13_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c13_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 14:
      {
        if (strcmp(tpChksum, "VVKv9Ps7rEZwm1pLZVH5AB") == 0) {
          extern mxArray *sf_c14_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c14_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 15:
      {
        if (strcmp(tpChksum, "LSZY6xDMdhHkvKpEbFImuH") == 0) {
          extern mxArray *sf_c15_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c15_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 16:
      {
        if (strcmp(tpChksum, "SV2Hl2oMpTFKUiYO6QKv8D") == 0) {
          extern mxArray *sf_c16_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c16_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 17:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c17_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c17_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 18:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c18_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c18_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 19:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c19_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c19_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 20:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c20_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c20_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     case 21:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c21_HeartModelECG_third_party_uses_info(void);
          plhs[0] = sf_c21_HeartModelECG_third_party_uses_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

unsigned int sf_HeartModelECG_updateBuildInfo_args_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the updateBuildInfo_args_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_updateBuildInfo_args_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "NgagU6DvgJECK1wOxq40CD") == 0) {
          extern mxArray *sf_c1_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c1_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "oJecBbo3mfT93mMB83nTGF") == 0) {
          extern mxArray *sf_c2_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c2_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "iGxkkdOdxvxinp4ltbVV0D") == 0) {
          extern mxArray *sf_c3_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c3_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "qiR5ycYnvQWT42lgceA3nB") == 0) {
          extern mxArray *sf_c4_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c4_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "LZRpOoxz3go9NqZi0TMwYH") == 0) {
          extern mxArray *sf_c5_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c5_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "5KERGKQ2WGDsl3dzyd1RSB") == 0) {
          extern mxArray *sf_c6_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c6_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "lpsK3luMh2VL8FJQKbDP9F") == 0) {
          extern mxArray *sf_c7_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c7_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 8:
      {
        if (strcmp(tpChksum, "zK7ecCBNq0wD3Cn41oRbUF") == 0) {
          extern mxArray *sf_c8_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c8_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 9:
      {
        if (strcmp(tpChksum, "RO3E5sQdbTzAzLtlIv02mF") == 0) {
          extern mxArray *sf_c9_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c9_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 10:
      {
        if (strcmp(tpChksum, "Dhw0UYLVJiB18rFKCN8vQG") == 0) {
          extern mxArray *sf_c10_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c10_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 11:
      {
        if (strcmp(tpChksum, "uBOvtAM6vHDCnp9hZ6rMqF") == 0) {
          extern mxArray *sf_c11_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c11_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 12:
      {
        if (strcmp(tpChksum, "tJ8yvZvTd6aXPMx7wkvAi") == 0) {
          extern mxArray *sf_c12_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c12_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 13:
      {
        if (strcmp(tpChksum, "KcOJKPl9jb5T3UBP96q1m") == 0) {
          extern mxArray *sf_c13_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c13_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 14:
      {
        if (strcmp(tpChksum, "VVKv9Ps7rEZwm1pLZVH5AB") == 0) {
          extern mxArray *sf_c14_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c14_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 15:
      {
        if (strcmp(tpChksum, "LSZY6xDMdhHkvKpEbFImuH") == 0) {
          extern mxArray *sf_c15_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c15_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 16:
      {
        if (strcmp(tpChksum, "SV2Hl2oMpTFKUiYO6QKv8D") == 0) {
          extern mxArray *sf_c16_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c16_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 17:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c17_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c17_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 18:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c18_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c18_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 19:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c19_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c19_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 20:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c20_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c20_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     case 21:
      {
        if (strcmp(tpChksum, "in2JOss7EPhyGWy2qSg79F") == 0) {
          extern mxArray *sf_c21_HeartModelECG_updateBuildInfo_args_info(void);
          plhs[0] = sf_c21_HeartModelECG_updateBuildInfo_args_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

void HeartModelECG_debug_initialize(struct SfDebugInstanceStruct* debugInstance)
{
  _HeartModelECGMachineNumber_ = sf_debug_initialize_machine(debugInstance,
    "HeartModelECG","sfun",0,21,0,0,0);
  sf_debug_set_machine_event_thresholds(debugInstance,
    _HeartModelECGMachineNumber_,0,0);
  sf_debug_set_machine_data_thresholds(debugInstance,
    _HeartModelECGMachineNumber_,0);
}

void HeartModelECG_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_HeartModelECG_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info("HeartModelECG",
      "HeartModelECG");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_HeartModelECG_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
