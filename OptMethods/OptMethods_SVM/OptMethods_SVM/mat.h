#pragma once

#include "vec.h"
#include "matr.h"

template<size_t size_w, size_t size_h>
vec<size_h> matr<size_w, size_h>::operator*(const vec<size_w>&v) {
  vec<size_h> newVec;

  for (unsigned int y = 0; y < size_h; y++)
    for (unsigned int x = 0; x < size_w; x++)
      newVec.vect[y] += matrix[y][x] * v.vect[x];

  return newVec;
}
