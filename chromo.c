#include "chromo.h"

#include <stdio.h>
#include <memory.h>

#include <math.h>

struct chromo_init *get_chromo_init(uint32_t bits,
    uint32_t bytes, uint32_t ints, uint32_t doubles) {
  struct chromo_init *ret = malloc(sizeof(*ret));
  ret->bit_len = bits;
  ret->byte_len = bytes;
  ret->int_len = ints;
  ret->double_len = doubles;
  ret->byte_min = default_byte_min;
  ret->byte_max = default_byte_max;
  ret->int_min = default_int_min;
  ret->int_max = default_int_max;
  ret->double_min = default_double_min;
  ret->double_max = default_double_max;
  printf("useed: %d\n", default_byte_max);
  printf("inited: %d\n", ret->byte_max);
  return ret;
}

static uint32_t full_bytes(uint32_t bits) {
  return (bits + 7)/8;
}

static bool get_bit_at(uint8_t byte, uint8_t index) {
  uint8_t mask = 1 << (7-index);
  return (byte & mask)!=0;
}

static uint8_t set_bit_at(uint8_t byte, uint8_t index, bool value) {
  uint8_t mask = 1 << (7-index);
  if (value==0)
    return byte & (~mask);
  else
    return byte | mask;
}

struct chromo *init_chromo(struct chromo_init *init) {
  if (init==NULL)
    return NULL;

  struct chromo *ret = malloc(sizeof(*ret));
  if (ret==NULL)
    return NULL;
  memset(ret, 0x00, sizeof(*ret));

  ret->init = init;
  if (init->bit_len!=0)
    ret->bits = malloc(full_bytes(init->bit_len) * sizeof(uint8_t));
  if (init->byte_len!=0)
    ret->bytes = malloc(init->byte_len * sizeof(uint8_t));
  if (init->int_len)
    ret->bytes = malloc(init->int_len * sizeof(uint32_t));
  if (init->double_len)
    ret->bytes = malloc(init->double_len * sizeof(double));

  return ret;
}

void dispose_chromo(struct chromo *chromo) {
  free(chromo->bits);
  free(chromo->bytes);
  free(chromo->ints);
  free(chromo->doubles);
  free(chromo);
}

uint32_t chromo_bit_len(struct chromo *chromo) {
  return chromo->init->bit_len;
}

uint32_t chromo_byte_len(struct chromo *chromo) {
  return chromo->init->byte_len;
}

uint32_t chromo_int_len(struct chromo *chromo) {
  return chromo->init->int_len;
}

uint32_t chromo_double_len(struct chromo *chromo) {
  return chromo->init->double_len;
}

uint32_t chromo_len(struct chromo *chromo) {
  return chromo_bit_len(chromo) + chromo_byte_len(chromo) +
    chromo_int_len(chromo) + chromo_double_len(chromo);
}

enum chromo_data_type chromo_type_at(struct chromo *chromo, uint32_t index) {
  if (index<chromo_bit_len(chromo))
    return CHROMO_DATA_BIT;
  index -= chromo_bit_len(chromo);
  if (index<chromo_byte_len(chromo))
    return CHROMO_DATA_BYTE;
  index -= chromo_byte_len(chromo);
  if (index<chromo_int_len(chromo))
    return CHROMO_DATA_INT;
  index -= chromo_int_len(chromo);
  if (index<chromo_double_len(chromo))
    return CHROMO_DATA_DOUBLE;
  return CHROMO_DATA_NONE;
}

bool chromo_bit_get(struct chromo *chromo, uint32_t index) {
  if (index>=chromo_bit_len(chromo))
    return false;
  return get_bit_at(chromo->bits[full_bytes(index)], index%8);

}

uint8_t chromo_byte_get(struct chromo *chromo, uint32_t index) {
  if (index>=chromo_byte_len(chromo))
    return -1;
  return chromo->bytes[index];
}

uint32_t chromo_int_get(struct chromo *chromo, uint32_t index) {
  if (index>=chromo_int_len(chromo))
    return -1;
  return chromo->ints[index];
}

double chromo_double_get(struct chromo *chromo, uint32_t index) {
  if (index>=chromo_double_len(chromo))
    return NAN;
  return chromo->doubles[index];
}

bool chromo_bit_abs_get(struct chromo *chromo, uint32_t index) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_BIT)
    return false;
  return chromo_bit_get(chromo, index);
}

uint8_t chromo_byte_abs_get(struct chromo *chromo, uint32_t index) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_BYTE)
    return -1;
  return chromo_byte_get(chromo, index - chromo_bit_len(chromo));
}

uint32_t chromo_int_abs_get(struct chromo *chromo, uint32_t index) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_INT)
    return -1;
  return chromo_int_get(
      chromo, index - chromo_bit_len(chromo) - chromo_byte_len(chromo));
}

double chromo_double_abs_get(struct chromo *chromo, uint32_t index) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_DOUBLE)
    return NAN;
  return chromo_double_get(chromo, index - chromo_bit_len(chromo) -
      chromo_byte_len(chromo) - chromo_int_len(chromo));
}

bool chromo_bit_set(struct chromo *chromo, uint32_t index, bool value) {
  if (index>=chromo_bit_len(chromo))
    return false;
  uint32_t i = full_bytes(index);
  chromo->bits[i] = set_bit_at(chromo->bits[i], index%8, value);
  return true;
}

bool chromo_byte_set(struct chromo *chromo, uint32_t index, uint8_t value) {
  if (index >= chromo_byte_len(chromo))
    return false;
  if (value > chromo->init->byte_max)
    value = chromo->init->byte_max;
  else if (value < chromo->init->byte_min)
    value = chromo->init->byte_min;
  chromo->bytes[index] = value;
  return true;
}

bool chromo_int_set(struct chromo *chromo, uint32_t index, uint32_t value) {
  if (index>=chromo_int_len(chromo))
    return false;
  if (value > chromo->init->int_max)
    value = chromo->init->int_max;
  else if (value < chromo->init->int_min)
    value = chromo->init->int_min;
  chromo->ints[index] = value;
  return true;
}

bool chromo_double_set(struct chromo *chromo, uint32_t index, double value) {
  if (index>=chromo_double_len(chromo))
    return false;
  if (value > chromo->init->double_max)
    value = chromo->init->double_max;
  else if (value < chromo->init->double_min)
    value = chromo->init->double_min;
  chromo->doubles[index] = value;
  return true;
}

bool chromo_bit_abs_set(struct chromo *chromo, uint32_t index, bool value) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_BIT)
    return false;
  return chromo_bit_set(chromo, index, value);
}

bool chromo_byte_abs_set(struct chromo *chromo, uint32_t index, uint8_t value) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_BYTE)
    return false;
  return chromo_byte_set(chromo, index - chromo_bit_len(chromo), value);
}

bool chromo_int_abs_set(struct chromo *chromo, uint32_t index, uint32_t value) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_INT)
    return false;
  return chromo_int_set(chromo, index - chromo_bit_len(chromo) -
      chromo_byte_len(chromo), value);
}

bool chromo_double_abs_set(struct chromo *chromo, uint32_t index, double value) {
  if (chromo_type_at(chromo, index) != CHROMO_DATA_DOUBLE)
    return false;
  return chromo_double_set(chromo, index - chromo_bit_len(chromo) -
      chromo_byte_len(chromo) - chromo_int_len(chromo), value);
}

bool chromos_are_compatible(struct chromo *chromo1, struct chromo *chromo2) {
  if (memcmp(chromo1->init, chromo2->init, sizeof(*(chromo1->init)))==0)
    return true;
  return false;
}

bool chromo_copy_gene(struct chromo *to, struct chromo *from, uint32_t index) {
  enum chromo_data_type type = chromo_type_at(to, index);
  if (chromo_type_at(from, index)!=type)
    return false;

  switch (type) {
    case CHROMO_DATA_BIT:
      chromo_bit_abs_set(to, index, chromo_bit_abs_get(from, index));
      return true;
    case CHROMO_DATA_BYTE:
      chromo_byte_abs_set(to, index, chromo_byte_abs_get(from, index));
      return true;
    case CHROMO_DATA_INT:
      chromo_int_abs_set(to, index, chromo_int_abs_get(from, index));
      return true;
    case CHROMO_DATA_DOUBLE:
      chromo_double_abs_set(to, index, chromo_double_abs_get(from, index));
      return true;
    default:
      return false;
  }
}

bool chromo_copy(
    struct chromo *to, struct chromo *from, uint32_t start, uint32_t end) {
  if (!chromos_are_compatible(to, from))
    return false;
  if (end>=chromo_len(to) || start >= end)
    return false;
  uint32_t i;
  for (i=start; i<end; i++)
    chromo_copy_gene(to, from, i);
  return true;
}

bool chromo_mutate(struct chromo *chromo, uint32_t index) {
  enum chromo_data_type type = chromo_type_at(chromo, index);

  switch (type) {
    case CHROMO_DATA_BIT:
      chromo_bit_abs_set(chromo, index, rand()%2);
      return true;
    case CHROMO_DATA_BYTE: {
      uint8_t interval = chromo->init->byte_max - chromo->init->byte_min;
      chromo_byte_abs_set(chromo, index,
          (uint8_t)(rand()%interval + chromo->init->byte_min));
      return true;
    }
    case CHROMO_DATA_INT: {
      uint32_t interval = chromo->init->int_max - chromo->init->int_min;
      chromo_int_abs_set(chromo, index,
          rand()%interval + chromo->init->int_min);
      return true;
    }
    case CHROMO_DATA_DOUBLE: {
      double interval = chromo->init->double_max - chromo->init->double_min;
      chromo_double_abs_set(chromo, index,
          ((double)(rand())/(double)RAND_MAX)*interval + chromo->init->double_min);
      return true;
    default:
      return false;
    }
  }
}

struct chromo *chromo_merge(struct chromo *chromo1, struct chromo *chromo2,
    uint32_t *switch_points, uint32_t switch_length,
    uint32_t *mutation_points, uint32_t mutation_length) {
  if (!chromos_are_compatible(chromo1, chromo2))
    return NULL;

  struct chromo *ret = init_chromo(chromo1->init);
  int side = 0; // left chromo
  uint32_t i;
  uint32_t len = chromo_len(chromo1);

  if (switch_length==0)
    chromo_copy(ret, chromo1, 0, len);
  else {
    chromo_copy(ret, chromo1, 0, switch_points[0]);
    for (i=0; i<switch_length; i++) {
      side = !side;
      uint32_t end = switch_points[i+1];
      if (i==switch_length-1)
        end = len;
      if (side==0)
        chromo_copy(ret, chromo1, switch_points[i], end);
      else
        chromo_copy(ret, chromo2, switch_points[i], end);
    }
  }

  for (i=0; i<mutation_length; i++)
    chromo_mutate(ret, i);

  return ret;
}

void dump_chromo(const struct chromo *chromo) {
  uint32_t i;
  printf("Dumping chromo: bits=%d, bytes=%d, ints=%d, doubles=%d\n",
      chromo->init->bit_len, chromo->init->byte_len,
      chromo->init->int_len, chromo->init->double_len);

  printf("bits  :");
  for (i=0; i<chromo->init->bit_len; i++) {
    if (chromo_bit_get((struct chromo*)chromo, i)==true)
      printf(" 1");
    else
      printf(" 0");
    if (i%32==31 && i!=chromo->init->bit_len-1)
      printf("\n       ");
  }
  printf("\n");

  printf("bytes :");
  for (i=0; i<chromo->init->byte_len; i++) {
    printf(" %2x", chromo_byte_get((struct chromo*)chromo, i));
    if (i%24==23 && i!=chromo->init->byte_len-1)
      printf("\n       ");
  }
  printf("\n");

  printf("ints  :");
  for (i=0; i<chromo->init->int_len; i++) {
    printf(" %d", chromo_int_get((struct chromo*)chromo, i));
    if (i%16==15 && i!=chromo->init->int_len-1)
      printf("\n       ");
  }
  printf("\n");

  printf("double:");
  for (i=0; i<chromo->init->double_len; i++) {
    printf(" %lf", chromo_double_get((struct chromo*)chromo, i));
    if (i%8==7 && i!=chromo->init->double_len-1)
      printf("\n       ");
  }
  printf("\n");

}
