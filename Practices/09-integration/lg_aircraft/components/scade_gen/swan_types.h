/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** Command: swan_cg.exe config.json -student
*************************************************************$ */
#ifndef SWAN_TYPES_H_
#define SWAN_TYPES_H_

#include "swan_config.h"

/* module0::operator0/#17: */
typedef enum swan_tag_SSM_ST_SSM1 {
  /* module0::operator0/#17:tierra: */ SSM_st_tierra_SSM1,
  /* module0::operator0/#17:despegue: */ SSM_st_despegue_SSM1,
  /* module0::operator0/#17:cruise: */ SSM_st_cruise_SSM1,
  /* module0::operator0/#17:aterrizaje: */ SSM_st_aterrizaje_SSM1,
  /* module0::operator0/#17:emergencia: */ SSM_st_emergencia_SSM1
} SSM_ST_SSM1;
/* module0::operator0/#54: */
typedef enum swan_tag_SSM_ST_SSM2 {
  /* module0::operator0/#54:tierra: */ SSM_st_tierra_SSM2,
  /* module0::operator0/#54:despegue: */ SSM_st_despegue_SSM2,
  /* module0::operator0/#54:despegue_der: */ SSM_st_despegue_der_SSM2,
  /* module0::operator0/#54:tierra_izq: */ SSM_st_tierra_izq_SSM2,
  /* module0::operator0/#54:despegue_izq: */ SSM_st_despegue_izq_SSM2,
  /* module0::operator0/#54:aterrizaje_izq: */ SSM_st_aterrizaje_izq_SSM2,
  /* module0::operator0/#54:tierra_der: */ SSM_st_tierra_der_SSM2,
  /* module0::operator0/#54:cruise_der: */ SSM_st_cruise_der_SSM2,
  /* module0::operator0/#54:cruise: */ SSM_st_cruise_SSM2,
  /* module0::operator0/#54:cruise_izq: */ SSM_st_cruise_izq_SSM2,
  /* module0::operator0/#54:aterrizaje: */ SSM_st_aterrizaje_SSM2,
  /* module0::operator0/#54:aterrizaje_der: */ SSM_st_aterrizaje_der_SSM2,
  /* module0::operator0/#54:emergency: */ SSM_st_emergency_SSM2
} SSM_ST_SSM2;
/* module0::operator0/#311: */
typedef enum swan_tag_SSM_ST_SSM3 {
  /* module0::operator0/#311:Tierra: */ SSM_st_Tierra_SSM3,
  /* module0::operator0/#311:Aire: */ SSM_st_Aire_SSM3,
  /* module0::operator0/#311:EMERGENCIA_ON: */ SSM_st_EMERGENCIA_ON_SSM3,
  /* module0::operator0/#311:EMERGENCIA_OFF: */ SSM_st_EMERGENCIA_OFF_SSM3
} SSM_ST_SSM3;
#endif /* SWAN_TYPES_H_ */
/* $ Ansys Scade One - Swan Code Generator - Version 2.4.0 - Build 0955 
** swan_types.h
*************************************************************$ */
