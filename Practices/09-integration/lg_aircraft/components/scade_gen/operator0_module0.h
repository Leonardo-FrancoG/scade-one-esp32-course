/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_operator0_module0_H_
#define SWAN_operator0_module0_H_

#include "swan_types.h"

typedef struct Ctx_operator0_module0 {
  swan_int32 mem1;
  swan_int32 mem2;
  swan_int32 mem3;
  swan_int32 mem4;
  SSM_ST_SSM3 mem5;
  SSM_ST_SSM2 mem6;
  SSM_ST_SSM1 mem7;
} outC_operator0_module0;

/* module0::operator0 */
extern void operator0_module0(
  /* mode */swan_int32 mode,
  /* dir */swan_int32 dir,
  /* despegue */swan_bool * restrict despegue,
  /* tierra */swan_bool * restrict tierra,
  /* cruise */swan_bool * restrict cruise,
  /* aterrizaje */swan_bool * restrict aterrizaje,
  /* aleizq */swan_int32 * restrict aleizq,
  /* aleder */swan_int32 * restrict aleder,
  /* tdir */swan_int32 * restrict tdir,
  /* elev */swan_int32 * restrict elev,
  /* red */swan_bool * restrict red,
  /* green */swan_bool * restrict green,
  /* blanca */swan_bool * restrict blanca,
  /* dirf */swan_int32 * restrict dirf,
  /* emergency */swan_bool * restrict emergency,
  /* cont */swan_int32 * restrict cont,
  /* emergency_off */swan_int32 * restrict emergency_off,
  /* safe */swan_bool * restrict safe,
  /* val */swan_int32 * restrict val,
  /* lgr */swan_int32 * restrict lgr,
  outC_operator0_module0 * restrict outC);

#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
extern void operator0_reset_module0(outC_operator0_module0 * restrict outC);
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */

#ifndef SWAN_USER_DEFINED_INIT
extern void operator0_init_module0(outC_operator0_module0 * restrict outC);
#endif /* SWAN_USER_DEFINED_INIT */



#endif /* SWAN_operator0_module0_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** operator0_module0.h
*************************************************************$ */
