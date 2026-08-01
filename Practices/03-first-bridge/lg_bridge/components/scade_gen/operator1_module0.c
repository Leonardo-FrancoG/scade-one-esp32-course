/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "operator1_module0.h"

/* module0::operator1 */
void operator1_module0(
  /* cont */swan_int32 * restrict cont,
  /* green */swan_bool * restrict green,
  /* yellow */swan_bool * restrict yellow,
  /* red */swan_bool * restrict red,
  outC_operator1_module0 * restrict outC)
{
  *cont = outC->mem1 + swan_lit_int32(1);
  if (*cont > swan_lit_int32(120)) {
    *cont = swan_lit_int32(0);
  }
  outC->mem1 = *cont;
  *red = *cont >= swan_lit_int32(70);
  *green = *cont <= swan_lit_int32(50);
  *yellow = !*green && !*red;
}

#ifndef SWAN_USER_DEFINED_INIT
void operator1_init_module0(outC_operator1_module0 * restrict outC)
{
  outC->mem1 = - swan_lit_int32(1);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void operator1_reset_module0(outC_operator1_module0 * restrict outC)
{
  outC->mem1 = - swan_lit_int32(1);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** operator1_module0.c
*************************************************************$ */
