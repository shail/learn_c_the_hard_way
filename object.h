// ifndef - if not defined, if there is then it skips all the following code
#ifndef _object_h
#define _object_h

typedef enum {
  NORTH, SOUTH, EAST, WEST
} Direction;

typedef struct {
  char *description;
  int (*init)(void *self);
  void (*describe)(void *self);
  void (*destroy)(void *self);
  void *(*move)(void *self, Direction direction);
  int (*attack)(void *self, int damage);
} Object;

int Object_init(void *self);
void Object_destroy(void *self);
void Object_describe(void *self);
void *Object_move(void *self, Direction direction);
int Object_attack(void *self, int damage);
void *Object_new(size_t size, Object proto, char *description);

// This makes a new macro and it works like a template function that spits out the code on the right, whenever
// you use the macro on the left. T##Proto says to concat Proto at the end of T
#define NEW(T, N) Object_new(sizeof(T), T##Proto, N)
// Lets you write obj->proto.blah as obj->_(blah)
#define _(N) proto.N

#endif
