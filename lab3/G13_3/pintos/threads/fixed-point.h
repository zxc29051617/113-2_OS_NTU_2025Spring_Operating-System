#ifndef THREADS_FIXED_POINT_H
#define THREADS_FIXED_POINT_H

#include <stdint.h>

typedef int32_t fixed_t;

#define FIXED_F (1 << 14)

static inline fixed_t
int_to_fixed (int n)
{
  return n * FIXED_F;
}

static inline int
fixed_to_int_zero (fixed_t x)
{
  return x / FIXED_F;
}

static inline int
fixed_to_int_nearest (fixed_t x)
{
  return x >= 0 ? (x + FIXED_F / 2) / FIXED_F
                : (x - FIXED_F / 2) / FIXED_F;
}

static inline fixed_t
fixed_add (fixed_t x, fixed_t y)
{
  return x + y;
}

static inline fixed_t
fixed_sub (fixed_t x, fixed_t y)
{
  return x - y;
}

static inline fixed_t
fixed_add_int (fixed_t x, int n)
{
  return x + n * FIXED_F;
}

static inline fixed_t
fixed_sub_int (fixed_t x, int n)
{
  return x - n * FIXED_F;
}

static inline fixed_t
fixed_mul (fixed_t x, fixed_t y)
{
  return ((int64_t) x) * y / FIXED_F;
}

static inline fixed_t
fixed_mul_int (fixed_t x, int n)
{
  return x * n;
}

static inline fixed_t
fixed_div (fixed_t x, fixed_t y)
{
  return ((int64_t) x) * FIXED_F / y;
}

static inline fixed_t
fixed_div_int (fixed_t x, int n)
{
  return x / n;
}

#endif /* threads/fixed-point.h */
