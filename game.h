#ifndef _game_h
#define _game_h

#include "object.h"

struct Monster {
  Object proto;
  int hit_points;
};

typedef struct Monster Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);

struct Goblin {
  Object proto;
  int hit_points;
};

typedef struct Goblin Goblin;

int Goblin_attack(void *self, int damage);
int Goblin_init(void *self);

struct Room {
  Object proto;

  Monster *bad_guy;
  Goblin *goblin;
  struct Room *north;
  struct Room *south;
  struct Room *east;
  struct Room *west;
};

typedef struct Room Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_init(void *self);

struct Map {
  Object proto;
  Room *start;
  Room *location;
};

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);

#endif
