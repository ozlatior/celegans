#ifndef __CELEGANS_WORLD_H__
#define __CELEGANS_WORLD_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define WORLD_PROPERTY_COUNT      (3)

enum world_property_names {
  WORLD_PROP_TEMPERATURE,
  WORLD_PROP_SMELL,
  WORLD_PROP_FOOD
};

static uint8_t world_property_propagation[WORLD_PROPERTY_COUNT] = { 1, 10, 0 };

struct world_property_source {
  uint16_t x;
  uint16_t y;
  enum world_property_names property;
  uint8_t intensity;
};

struct world_init {
  uint16_t size_x;
  uint16_t size_y;
  uint16_t start_x;
  uint16_t start_y;
  size_t n_sources;
  struct world_property_source *sources;
};

struct world_cell {
  uint8_t values[WORLD_PROPERTY_COUNT];
};

struct world {
  struct world_init *init;
  struct world_cell *cells;
};

/* square 1 world: 32x32, surrounded by fire, food every 8 cells */
struct world_init *init_square1();

/* square 2 world: 64x64, surrounded by fire, food every 8 cells */
struct world_init *init_square2();

/* square 3 world: 64x64, surrounded by fire, food every 8, fire ever 8 */
struct world_init *init_square3();

struct world *init_world(struct world_init *init);

void dispose_world_init(struct world_init *init);
void dispose_world(struct world *world, bool init_as_well);

struct world_cell *cell_at(const struct world *world, uint16_t x, uint16_t y);
uint8_t value_at(const struct world *world,
  uint16_t x, uint16_t y, enum world_property_names k);
uint8_t value_left(const struct world *world,
  uint16_t x, uint16_t y, enum world_property_names k);
uint8_t value_right(const struct world *world,
  uint16_t x, uint16_t y, enum world_property_names k);
uint8_t value_top(const struct world *world,
  uint16_t x, uint16_t y, enum world_property_names k);
uint8_t value_bottom(const struct world *world,
  uint16_t x, uint16_t y, enum world_property_names k);

void dump_world(const struct world *world);

#endif // __CELEGANS_WORLD_H__
