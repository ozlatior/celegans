#ifndef __CELEGANS_CHROMO_H__
#define __CELEGANS_CHROMO_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

static uint8_t default_byte_min   = 0;
static uint8_t default_byte_max   = 255;
static uint32_t default_int_min   = 0;
static uint32_t default_int_max   = UINT_MAX;
static double default_double_min  = -DBL_MAX;
static double default_double_max  = DBL_MAX;

enum chromo_data_type {
  CHROMO_DATA_NONE,
  CHROMO_DATA_BIT,
  CHROMO_DATA_BYTE,
  CHROMO_DATA_INT,
  CHROMO_DATA_DOUBLE
};

struct chromo_init {
  uint32_t bit_len;
  uint32_t byte_len;
  uint32_t int_len;
  uint32_t double_len;
  uint8_t byte_min;
  uint8_t byte_max;
  uint32_t int_min;
  uint32_t int_max;
  double double_min;
  double double_max;
};

struct chromo {
  struct chromo_init *init;
  uint8_t *bits;
  uint8_t *bytes;
  uint32_t *ints;
  double *doubles;
};

struct chromo_init *get_chromo_init(uint32_t bits,
    uint32_t bytes, uint32_t ints, uint32_t doubles);

static uint32_t full_bytes(uint32_t bits);
static bool get_bit_at(uint8_t byte, uint8_t index);
static uint8_t set_bit_at(uint8_t byte, uint8_t index, bool value);

struct chromo *init_chromo(struct chromo_init *init);

void dispose_chromo(struct chromo *chromo);

uint32_t chromo_bit_len(struct chromo *chromo);
uint32_t chromo_byte_len(struct chromo *chromo);
uint32_t chromo_int_len(struct chromo *chromo);
uint32_t chromo_double_len(struct chromo *chromo);
uint32_t chromo_len(struct chromo *chromo);

enum chromo_data_type chromo_type_at(struct chromo *chromo, uint32_t index);

bool chromo_bit_get(struct chromo *chromo, uint32_t index);
uint8_t chromo_byte_get(struct chromo *chromo, uint32_t index);
uint32_t chromo_int_get(struct chromo *chromo, uint32_t index);
double chromo_double_get(struct chromo *chromo, uint32_t index);

bool chromo_bit_abs_get(struct chromo *chromo, uint32_t index);
uint8_t chromo_byte_abs_get(struct chromo *chromo, uint32_t index);
uint32_t chromo_int_abs_get(struct chromo *chromo, uint32_t index);
double chromo_double_abs_get(struct chromo *chromo, uint32_t index);

bool chromo_bit_set(struct chromo *chromo, uint32_t index, bool value);
bool chromo_byte_set(struct chromo *chromo, uint32_t index, uint8_t value);
bool chromo_int_set(struct chromo *chromo, uint32_t index, uint32_t value);
bool chromo_double_set(struct chromo *chromo, uint32_t index, double value);

bool chromo_bit_abs_set(struct chromo *chromo, uint32_t index, bool value);
bool chromo_byte_abs_set(struct chromo *chromo, uint32_t index, uint8_t value);
bool chromo_int_abs_set(struct chromo *chromo, uint32_t index, uint32_t value);
bool chromo_double_abs_set(struct chromo *chromo, uint32_t index, double value);

bool chromos_are_compatible(struct chromo *chromo1, struct chromo *chromo2);

bool chromo_copy_gene(struct chromo *to, struct chromo *from, uint32_t index);
bool chromo_copy(
    struct chromo *to, struct chromo *from, uint32_t start, uint32_t end);

bool chromo_mutate(struct chromo *chromo, uint32_t index);

struct chromo *chromo_merge(struct chromo *chromo1, struct chromo *chromo2,
    uint32_t *switch_points, uint32_t switch_length,
    uint32_t *mutation_points, uint32_t mutation_length);

void dump_chromo(const struct chromo *chromo);

#endif // __CELEGANS_CHROMO_H__
