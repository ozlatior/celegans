#include "world.h"

#include <stdio.h>
#include <memory.h>

#include <math.h>

struct world_init* init_square1() {
  struct world_init *ret = malloc(sizeof(*ret));
  ret->size_x = 32;
  ret->size_y = 32;
  ret->start_x = 16;
  ret->start_y = 16;
  ret->n_sources = 124 + 18; // 124 fire sources, 9 food, 9 smell
  ret->sources = malloc(ret->n_sources * sizeof(struct world_property_source));
  size_t sc, i, j;
  sc = 0;
  for (i=0; i<ret->size_x; i++) {
    struct world_property_source source;
    source.x = i;
    source.y = 0;
    source.property = WORLD_PROP_TEMPERATURE;
    source.intensity = 100;
    ret->sources[sc] = source;
    sc++;
    source.y = ret->size_y-1;
    ret->sources[sc] = source;
    sc++;
  }
  for (i=1; i<ret->size_y-1; i++) {
    struct world_property_source source;
    source.x = 0;
    source.y = i;
    source.property = WORLD_PROP_TEMPERATURE;
    source.intensity = 100;
    ret->sources[sc] = source;
    sc++;
    source.x = ret->size_x-1;
    ret->sources[sc] = source;
    sc++;
  }
  return ret;
}

static uint8_t propagated_value_at(uint8_t initial, uint16_t sx, uint16_t sy,
    uint16_t x, uint16_t y, uint8_t propagation_constant)
{
  double distance = sqrt((x-sx)*(x-sx)+(y-sy)*(y-sy));
  double factor = 1/((1+distance)*(1+distance));
  return initial * factor * propagation_constant;
}

struct world *init_world(struct world_init *init) {
  if (init==NULL)
    return NULL;

  struct world *ret = malloc(sizeof(*ret));
  if (ret==NULL)
    return NULL;

  ret->init = init;
  ret->cells = malloc(init->size_x * init->size_y * sizeof(struct world_cell));
  memset(ret->cells, 0, init->size_x * init->size_y * sizeof(struct world_cell));
  if (ret->cells==NULL) {
    free(ret);
    return NULL;
  }

  uint16_t x, y, k;
  for (y=0; y<init->size_y; y++)
    for (x=0; x<init->size_x; x++)
      for (k=0; k<init->n_sources; k++)
        cell_at(ret, x, y)->values[init->sources[k].property] +=
          propagated_value_at(init->sources[k].intensity,
            init->sources[k].x, init->sources[k].y, x, y,
            world_property_propagation[init->sources[k].property]);

  return ret;
}

void dispose_world_init(struct world_init *init) {
  free(init->sources);
  free(init);
}

void dispose_world(struct world *world, bool init_as_well) {
  if (init_as_well)
    dispose_world_init(world->init);
  free(world->cells);
  free(world);
}

struct world_cell *cell_at(const struct world *world, uint16_t x, uint16_t y) {
  if (x>world->init->size_x || y>world->init->size_y)
    return NULL;
  return &(world->cells[y*world->init->size_x+x]);
}

uint8_t value_at(const struct world *world, uint16_t x, uint16_t y,
  enum world_property_names k)
{
  struct world_cell *cell = cell_at(world, x, y);
  if (cell==NULL)
    return -1;
  return cell->values[k];
}

uint8_t value_left(const struct world *world, uint16_t x, uint16_t y,
  enum world_property_names k)
{
  struct world_cell *cell = cell_at(world, x-1, y);
  if (cell==NULL)
    return -1;
  return cell->values[k];
}

uint8_t value_right(const struct world *world, uint16_t x, uint16_t y,
  enum world_property_names k)
{
  struct world_cell *cell = cell_at(world, x+1, y);
  if (cell==NULL)
    return -1;
  return cell->values[k];
}

uint8_t value_top(const struct world *world, uint16_t x, uint16_t y,
  enum world_property_names k)
{
  struct world_cell *cell = cell_at(world, x, y-1);
  if (cell==NULL)
    return -1;
  return cell->values[k];
}

uint8_t value_bottom(const struct world *world, uint16_t x, uint16_t y,
  enum world_property_names k)
{
  struct world_cell *cell = cell_at(world, x, y+1);
  if (cell==NULL)
    return -1;
  return cell->values[k];
}

void dump_world(const struct world *world) {
  printf("Dumping world: size_x=%d, size_y=%d, start_x=%d, start_y=%d\n",
      world->init->size_x, world->init->size_y,
      world->init->start_x, world->init->start_y);
  printf("A world with %d cells: \n", world->init->size_x * world->init->size_y);
  
  uint16_t x, y, k;
  printf("    ");
  for (x=0; x<world->init->size_x; x++)
    printf(" %03d", x);
  printf("\n");
  printf("    ");
  for (x=0; x<world->init->size_x; x++)
    printf(" ===");
  printf("\n");
  for (y=0; y<world->init->size_y; y++) {
    for (k=0; k<WORLD_PROPERTY_COUNT; k++) {
      if (k==0)
        printf(" %03d", y);
      else
        printf("    ");
      for (x=0; x<world->init->size_x; x++) {
        struct world_cell *cell = cell_at(world, x, y);
        if (cell->values[k]!=0)
          printf(" %03d", cell->values[k]);
        else
          printf(" ...");
      }
      printf("\n");
    }
  }
}
