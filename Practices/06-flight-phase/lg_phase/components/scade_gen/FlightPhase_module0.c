/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "FlightPhase_module0.h"

/* module0::FlightPhase */
void FlightPhase_module0(
  /* next */swan_bool next,
  /* phase */swan_int32 * restrict phase,
  outC_FlightPhase_module0 * restrict outC)
{
  switch (outC->mem1) {
    case SSM_st_LANDING_SSM1 :
      *phase = swan_lit_int32(3);
      if (next) {
        outC->mem1 = SSM_st_GROUND_SSM1;
      }
      else {
        outC->mem1 = SSM_st_LANDING_SSM1;
      }
      break;
    case SSM_st_CRUISE_SSM1 :
      *phase = swan_lit_int32(2);
      if (next) {
        outC->mem1 = SSM_st_LANDING_SSM1;
      }
      else {
        outC->mem1 = SSM_st_CRUISE_SSM1;
      }
      break;
    case SSM_st_TAKEOFF_SSM1 :
      *phase = swan_lit_int32(1);
      if (next) {
        outC->mem1 = SSM_st_CRUISE_SSM1;
      }
      else {
        outC->mem1 = SSM_st_TAKEOFF_SSM1;
      }
      break;
    case SSM_st_GROUND_SSM1 :
      *phase = swan_lit_int32(0);
      if (next) {
        outC->mem1 = SSM_st_TAKEOFF_SSM1;
      }
      else {
        outC->mem1 = SSM_st_GROUND_SSM1;
      }
      break;
    default :
      /* this default branch is unreachable */
      break;
  }
}

#ifndef SWAN_USER_DEFINED_INIT
void FlightPhase_init_module0(outC_FlightPhase_module0 * restrict outC)
{
  outC->mem1 = SSM_st_GROUND_SSM1;
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void FlightPhase_reset_module0(outC_FlightPhase_module0 * restrict outC)
{
  outC->mem1 = SSM_st_GROUND_SSM1;
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** FlightPhase_module0.c
*************************************************************$ */
