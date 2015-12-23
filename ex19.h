#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Monster {
  Object proto;
  int hit_points;
}

typedef struct Monster Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);
// TODO: add another monster
struct Room {
  Object proto;

  Monster *bad_guy;
  struct Room *north;
  struct Room *south;
  struct Room *east;
  struct Room *west;
}

typedef struct Room Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_init(void *self);

struct Map {
  Object proto;
  Room *start;
  Room *location;
}

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);

#endif

// -Call NEW(Room, "Hello") --> Object_new(sizeof(Room), RoomProto, "Hello.")
// -Inside Object_new, I allocate a piece of memory that's Room in size, but point a Object *el pointer at it.
// -Since C puts the Room.proto field first, that means the el pointer is really only pointing at enough of the
// block of memory to see a full Object struct, no idea that it is even called proto
// -Then uses this Object *el pointer to set the contents of the piece of memory correclt with *el = proto,
// you can copy structs, and that *el means the value of whatever el points at, assign the proto struct to
// whatever el points as
// -Now that this mystery struct is filled in with the right data from proto, the function can then call init
// or destroy on the Object, but the cool part is whoever called this function can change these out for
// whatever ones they want.
