#include <stdio.h>

#include "world.h"

int main() {
  printf("Starting program ...\n");

  struct world_init *init = init_square1();

  struct world *world = init_world(init);
  dump_world(world);

  dispose_world(world, true);

  return 0;
}
