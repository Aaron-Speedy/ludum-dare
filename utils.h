#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <assert.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

typedef struct {
  int x, y;
} Vec2I;

float randf(float min, float max);
int randi(int min, int max);
int sign(int x);
void clampf(float *x, float min, float max);
void clampi(int *x, int min, int max);
int in_bounds(int x, int max);
int vec2i_at(Vec2I vec, int index);

#define List_F(T) T *items; size_t cap, count
#define List(T) struct { List_F(T); }

#define da_push(xs, x) \
  do { \
    if ((xs)->count >= (xs)->cap) { \
      if ((xs)->cap == 0) (xs)->cap = 256; \
      else (xs)->cap *= 2; \
      (xs)->items = realloc((xs)->items, (xs)->cap*sizeof(*(xs)->items)); \
    } \
 \
    (xs)->items[(xs)->count++] = (x); \
  } while (0)

#define da_pop(xs) \
  do { \
    assert ((xs)->count > 0); \
    (xs)->count -= 1; \
  } while (0)

#define da_last(xs) (xs)->items[(xs)->count - 1]

#define da_init(xs) \
do { \
  (xs)->items = malloc((xs)->cap * sizeof((xs)->items[0])); \
} while (0);

#ifdef UTILS_IMPL
#define UTILS_IMPL

float randf(float min, float max) {
  float scale = rand() / (float) RAND_MAX;
  return min + scale * ( max - min );
}

int randi(int min, int max) {
  return (rand() % (max - min)) + min + 1;
}

int sign(int x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

void clampf(float *x, float min, float max) {
  if (*x < min) *x = min;
  else if (*x > max) *x = max;
}

void clampi(int *x, int min, int max) {
  if (*x < min) *x = min;
  else if (*x > max) *x = max;
}

int in_bounds(int x, int max) {
  if (x < 0) return 0;
  if (x >= max) return 0;
  return 1;
}

int vec2i_at(Vec2I vec, int index) {
  switch (index) {
    case 0:
      return vec.x;
    case 1:
      return vec.y;
    default:
      assert(!"Out of bounds");
  }
}

#endif

#endif
