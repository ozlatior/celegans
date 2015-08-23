#include "chromo.h"
#include <stdio.h>

int main() {
  uint32_t i;

  printf("Chromosomes with a single type of data\n");

  struct chromo_init *init;
  struct chromo *chromo;

  init = get_chromo_init(128, 0, 0, 0);
  chromo = init_chromo(init);
  for (i=0; i<128; i++)
    chromo_bit_set(chromo, i, i%2);
  dump_chromo(chromo);
  dispose_chromo(chromo);
  free(init);

  init = get_chromo_init(0, 128, 0, 0);
  chromo = init_chromo(init);
  for (i=0; i<128; i++)
    chromo_byte_set(chromo, i, i);
  dump_chromo(chromo);
  dispose_chromo(chromo);
  free(init);

  return 0;
}
