/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_operator0_module0_H_
#define SWAN_operator0_module0_H_

#include "swan_types.h"

typedef struct Ctx_operator0_module0 {
  swan_int32 mem1;
} outC_operator0_module0;

/* module0::operator0 */
extern void operator0_module0(
  /* cont */swan_int32 * restrict cont,
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
