/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#include "swan_sensors.h"
#include "swan_consts.h"
#include "operator0_module0.h"

/* module0::operator0 */
void operator0_module0(
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
  outC_operator0_module0 * restrict outC)
{
  swan_int32 x1;
  swan_int32 x2;

  *cont = outC->mem2 + swan_lit_int32(1);
  if (*cont > swan_lit_int32(10)) {
    *cont = swan_lit_int32(0);
  }
  outC->mem2 = *cont;
  x1 = mode - outC->mem1;
  outC->mem1 = mode;
  *emergency = mode < swan_lit_int32(20) || mode > swan_lit_int32(4075) ||
    (x1 >= swan_lit_int32(2000) || x1 <= - swan_lit_int32(2000));
  *dirf = dir - swan_lit_int32(2048);
  switch (outC->mem7) {
    case SSM_st_emergencia_SSM1 :
      *val = swan_lit_int32(1);
      *aterrizaje = swan_false;
      *cruise = swan_false;
      *tierra = swan_false;
      *despegue = swan_false;
      break;
    case SSM_st_aterrizaje_SSM1 :
      *val = outC->mem4;
      *aterrizaje = swan_true;
      *cruise = swan_false;
      *tierra = swan_false;
      *despegue = swan_false;
      break;
    case SSM_st_cruise_SSM1 :
      *val = outC->mem4;
      *aterrizaje = swan_false;
      *cruise = swan_true;
      *tierra = swan_false;
      *despegue = swan_false;
      break;
    case SSM_st_despegue_SSM1 :
      *val = outC->mem4;
      *aterrizaje = swan_false;
      *cruise = swan_false;
      *tierra = swan_false;
      *despegue = swan_true;
      break;
    case SSM_st_tierra_SSM1 :
      *val = swan_lit_int32(0);
      *aterrizaje = swan_false;
      *cruise = swan_false;
      *tierra = swan_true;
      *despegue = swan_false;
      break;
    default :
      /* this default branch is unreachable */
      break;
  }
  outC->mem4 = *val;
  if (*emergency) {
    *emergency_off = swan_lit_int32(0);
  }
  else {
    if (*emergency) {
      x2 = swan_lit_int32(0);
    }
    else {
      x2 = *val;
    }
    *emergency_off = x2 + outC->mem3;
  }
  *safe = *emergency_off >= swan_lit_int32(10);
  switch (outC->mem7) {
    case SSM_st_emergencia_SSM1 :
      if (*safe) {
        outC->mem7 = SSM_st_tierra_SSM1;
      }
      else {
        outC->mem7 = SSM_st_emergencia_SSM1;
      }
      break;
    case SSM_st_aterrizaje_SSM1 :
      if (mode <= swan_lit_int32(3071)) {
        outC->mem7 = SSM_st_cruise_SSM1;
      }
      else if (*emergency) {
        outC->mem7 = SSM_st_emergencia_SSM1;
      }
      else {
        outC->mem7 = SSM_st_aterrizaje_SSM1;
      }
      break;
    case SSM_st_cruise_SSM1 :
      if (mode > swan_lit_int32(3071)) {
        outC->mem7 = SSM_st_aterrizaje_SSM1;
      }
      else if (mode <= swan_lit_int32(2047)) {
        outC->mem7 = SSM_st_despegue_SSM1;
      }
      else if (*emergency) {
        outC->mem7 = SSM_st_emergencia_SSM1;
      }
      else {
        outC->mem7 = SSM_st_cruise_SSM1;
      }
      break;
    case SSM_st_despegue_SSM1 :
      if (mode > swan_lit_int32(2047)) {
        outC->mem7 = SSM_st_cruise_SSM1;
      }
      else if (mode <= swan_lit_int32(1023)) {
        outC->mem7 = SSM_st_tierra_SSM1;
      }
      else if (*emergency) {
        outC->mem7 = SSM_st_emergencia_SSM1;
      }
      else {
        outC->mem7 = SSM_st_despegue_SSM1;
      }
      break;
    case SSM_st_tierra_SSM1 :
      if (mode > swan_lit_int32(1023)) {
        outC->mem7 = SSM_st_despegue_SSM1;
      }
      else if (*emergency) {
        outC->mem7 = SSM_st_emergencia_SSM1;
      }
      else {
        outC->mem7 = SSM_st_tierra_SSM1;
      }
      break;
    default :
      /* this default branch is unreachable */
      break;
  }
  if (*safe) {
    *emergency_off = swan_lit_int32(0);
  }
  outC->mem3 = *emergency_off;
  switch (outC->mem5) {
    case SSM_st_EMERGENCIA_OFF_SSM3 :
      *red = swan_false;
      if (*safe) {
        outC->mem5 = SSM_st_Tierra_SSM3;
      }
      else if (*cont <= swan_lit_int32(5)) {
        outC->mem5 = SSM_st_EMERGENCIA_ON_SSM3;
      }
      else {
        outC->mem5 = SSM_st_EMERGENCIA_OFF_SSM3;
      }
      break;
    case SSM_st_EMERGENCIA_ON_SSM3 :
      *red = swan_true;
      if (*safe) {
        outC->mem5 = SSM_st_Tierra_SSM3;
      }
      else if (*cont > swan_lit_int32(5)) {
        outC->mem5 = SSM_st_EMERGENCIA_OFF_SSM3;
      }
      else {
        outC->mem5 = SSM_st_EMERGENCIA_ON_SSM3;
      }
      break;
    case SSM_st_Aire_SSM3 :
      *red = swan_true;
      if (*tierra) {
        outC->mem5 = SSM_st_Tierra_SSM3;
      }
      else if (*emergency) {
        outC->mem5 = SSM_st_EMERGENCIA_ON_SSM3;
      }
      else {
        outC->mem5 = SSM_st_Aire_SSM3;
      }
      break;
    case SSM_st_Tierra_SSM3 :
      *red = swan_false;
      if (!*tierra) {
        outC->mem5 = SSM_st_Aire_SSM3;
      }
      else if (*emergency) {
        outC->mem5 = SSM_st_EMERGENCIA_ON_SSM3;
      }
      else {
        outC->mem5 = SSM_st_Tierra_SSM3;
      }
      break;
    default :
      /* this default branch is unreachable */
      break;
  }
  *blanca = *red;
  *green = *red;
  switch (outC->mem6) {
    case SSM_st_emergency_SSM2 :
      *lgr = swan_lit_int32(0);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*safe) {
        outC->mem6 = SSM_st_tierra_SSM2;
      }
      else {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      break;
    case SSM_st_aterrizaje_der_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(100);
      *tdir = swan_lit_int32(110);
      *aleder = swan_lit_int32(115);
      *aleizq = swan_lit_int32(65);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_der_SSM2;
      }
      else if (*dirf <= swan_lit_int32(300)) {
        outC->mem6 = SSM_st_aterrizaje_SSM2;
      }
      else {
        outC->mem6 = SSM_st_aterrizaje_der_SSM2;
      }
      break;
    case SSM_st_aterrizaje_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(100);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_SSM2;
      }
      else if (*dirf < - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_aterrizaje_izq_SSM2;
      }
      else if (*dirf > swan_lit_int32(300)) {
        outC->mem6 = SSM_st_aterrizaje_der_SSM2;
      }
      else {
        outC->mem6 = SSM_st_aterrizaje_SSM2;
      }
      break;
    case SSM_st_cruise_izq_SSM2 :
      *lgr = swan_lit_int32(0);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(70);
      *aleder = swan_lit_int32(65);
      *aleizq = swan_lit_int32(115);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*aterrizaje) {
        outC->mem6 = SSM_st_aterrizaje_izq_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_izq_SSM2;
      }
      else if (*dirf >= - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_cruise_SSM2;
      }
      else {
        outC->mem6 = SSM_st_cruise_izq_SSM2;
      }
      break;
    case SSM_st_cruise_SSM2 :
      *lgr = swan_lit_int32(0);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*aterrizaje) {
        outC->mem6 = SSM_st_aterrizaje_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_SSM2;
      }
      else if (*dirf < - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_cruise_izq_SSM2;
      }
      else if (*dirf > swan_lit_int32(300)) {
        outC->mem6 = SSM_st_cruise_der_SSM2;
      }
      else {
        outC->mem6 = SSM_st_cruise_SSM2;
      }
      break;
    case SSM_st_cruise_der_SSM2 :
      *lgr = swan_lit_int32(0);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(110);
      *aleder = swan_lit_int32(115);
      *aleizq = swan_lit_int32(65);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*aterrizaje) {
        outC->mem6 = SSM_st_aterrizaje_der_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_der_SSM2;
      }
      else if (*dirf <= swan_lit_int32(300)) {
        outC->mem6 = SSM_st_cruise_SSM2;
      }
      else {
        outC->mem6 = SSM_st_cruise_der_SSM2;
      }
      break;
    case SSM_st_tierra_der_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_der_SSM2;
      }
      else if (*dirf <= swan_lit_int32(300)) {
        outC->mem6 = SSM_st_tierra_SSM2;
      }
      else {
        outC->mem6 = SSM_st_tierra_der_SSM2;
      }
      break;
    case SSM_st_aterrizaje_izq_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(100);
      *tdir = swan_lit_int32(70);
      *aleder = swan_lit_int32(65);
      *aleizq = swan_lit_int32(115);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_izq_SSM2;
      }
      else if (*dirf >= - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_aterrizaje_SSM2;
      }
      else {
        outC->mem6 = SSM_st_aterrizaje_izq_SSM2;
      }
      break;
    case SSM_st_despegue_izq_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(105);
      *tdir = swan_lit_int32(70);
      *aleder = swan_lit_int32(65);
      *aleizq = swan_lit_int32(115);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_izq_SSM2;
      }
      else if (*tierra) {
        outC->mem6 = SSM_st_tierra_izq_SSM2;
      }
      else if (*dirf >= - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_despegue_SSM2;
      }
      else {
        outC->mem6 = SSM_st_despegue_izq_SSM2;
      }
      break;
    case SSM_st_tierra_izq_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_izq_SSM2;
      }
      else if (*dirf >= - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_tierra_SSM2;
      }
      else {
        outC->mem6 = SSM_st_tierra_izq_SSM2;
      }
      break;
    case SSM_st_despegue_der_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(105);
      *tdir = swan_lit_int32(110);
      *aleder = swan_lit_int32(115);
      *aleizq = swan_lit_int32(65);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_der_SSM2;
      }
      else if (*tierra) {
        outC->mem6 = SSM_st_tierra_der_SSM2;
      }
      else if (*dirf <= swan_lit_int32(300)) {
        outC->mem6 = SSM_st_despegue_SSM2;
      }
      else {
        outC->mem6 = SSM_st_despegue_der_SSM2;
      }
      break;
    case SSM_st_despegue_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(105);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*cruise) {
        outC->mem6 = SSM_st_cruise_SSM2;
      }
      else if (*tierra) {
        outC->mem6 = SSM_st_tierra_SSM2;
      }
      else if (*dirf < - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_despegue_izq_SSM2;
      }
      else if (*dirf > swan_lit_int32(300)) {
        outC->mem6 = SSM_st_despegue_der_SSM2;
      }
      else {
        outC->mem6 = SSM_st_despegue_SSM2;
      }
      break;
    case SSM_st_tierra_SSM2 :
      *lgr = swan_lit_int32(90);
      *elev = swan_lit_int32(90);
      *tdir = swan_lit_int32(90);
      *aleder = swan_lit_int32(90);
      *aleizq = swan_lit_int32(90);
      if (*emergency) {
        outC->mem6 = SSM_st_emergency_SSM2;
      }
      else if (*despegue) {
        outC->mem6 = SSM_st_despegue_SSM2;
      }
      else if (*dirf < - swan_lit_int32(300)) {
        outC->mem6 = SSM_st_tierra_izq_SSM2;
      }
      else if (*dirf > swan_lit_int32(300)) {
        outC->mem6 = SSM_st_tierra_der_SSM2;
      }
      else {
        outC->mem6 = SSM_st_tierra_SSM2;
      }
      break;
    default :
      /* this default branch is unreachable */
      break;
  }
}

#ifndef SWAN_USER_DEFINED_INIT
void operator0_init_module0(outC_operator0_module0 * restrict outC)
{
  outC->mem4 = swan_lit_int32(0);
  outC->mem5 = SSM_st_Tierra_SSM3;
  outC->mem6 = SSM_st_tierra_SSM2;
  outC->mem7 = SSM_st_tierra_SSM1;
  outC->mem1 = swan_lit_int32(0);
  outC->mem2 = swan_lit_int32(0);
  outC->mem3 = swan_lit_int32(0);
}
#endif /* SWAN_USER_DEFINED_INIT */


#ifndef SWAN_NO_EXTERN_CALL_TO_RESET
void operator0_reset_module0(outC_operator0_module0 * restrict outC)
{
  outC->mem5 = SSM_st_Tierra_SSM3;
  outC->mem6 = SSM_st_tierra_SSM2;
  outC->mem7 = SSM_st_tierra_SSM1;
  outC->mem1 = swan_lit_int32(0);
  outC->mem2 = swan_lit_int32(0);
  outC->mem3 = swan_lit_int32(0);
}
#endif /* SWAN_NO_EXTERN_CALL_TO_RESET */




/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** operator0_module0.c
*************************************************************$ */
