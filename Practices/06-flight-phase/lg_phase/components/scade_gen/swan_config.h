/* swan_config.h — platform adaptation for ESP32 / Xtensa GCC
 * Provides the base types and literal macros expected by
 * code generated with the Ansys Scade One Swan Code Generator. */
#ifndef SWAN_CONFIG_H_
#define SWAN_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

/* Base types */
typedef bool     swan_bool;
typedef char     swan_char;
typedef int8_t   swan_int8;
typedef int16_t  swan_int16;
typedef int32_t  swan_int32;
typedef int64_t  swan_int64;
typedef uint8_t  swan_uint8;
typedef uint16_t swan_uint16;
typedef uint32_t swan_uint32;
typedef uint64_t swan_uint64;
typedef float    swan_float32;
typedef double   swan_float64;

/* Boolean constants */
#define swan_true  true
#define swan_false false

/* Literal constructors */
#define swan_lit_int8(v)    ((swan_int8)(v))
#define swan_lit_int16(v)   ((swan_int16)(v))
#define swan_lit_int32(v)   ((swan_int32)(v))
#define swan_lit_int64(v)   ((swan_int64)(v))
#define swan_lit_uint8(v)   ((swan_uint8)(v))
#define swan_lit_uint16(v)  ((swan_uint16)(v))
#define swan_lit_uint32(v)  ((swan_uint32)(v))
#define swan_lit_uint64(v)  ((swan_uint64)(v))
#define swan_lit_float32(v) ((swan_float32)(v))
#define swan_lit_float64(v) ((swan_float64)(v))

#endif /* SWAN_CONFIG_H_ */