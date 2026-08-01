/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_FlightPhase_module0_H_
#define SWAN_FlightPhase_module0_H_

#include "swan_types.h"

typedef struct Ctx_FlightPhase_module0 {
  SSM_ST_SSM1 mem1;
} outC_FlightPhase_module0;

/* module0::FlightPhase */
extern void FlightPhase_module0(
  /* next */swan_bool next,
  /* phase */swan_int32 * restrict phase,
  outC_FlightPhase_module0 * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void FlightPhase_reset_module0(outC_FlightPhase_module0 * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void FlightPhase_init_module0(outC_FlightPhase_module0 * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_FlightPhase_module0_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** FlightPhase_module0.h
*************************************************************$ */
