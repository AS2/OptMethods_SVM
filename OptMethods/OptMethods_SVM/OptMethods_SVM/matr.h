#pragma once

#include "vec.h"

#include <vector>
#include <utility>

/*
 * size_w - ÿ»–»Õ¿ Ã¿“–»÷€ (n)
 * size_h - ¬€—Œ“¿ Ã¿“–»÷€ (m)
 */
class matr {
public:
  size_t size_w, size_h;
  std::vector<double> matrix; // array of horizontal lines

  // default constructor
  matr(size_t size_w, size_t size_h) {
    this->size_w = size_w;
    this->size_h = size_h;
    matrix = std::vector<double>(size_w * size_h, 0);
  }

  // copy constructor
  matr(const matr& mtr) {
    size_w = mtr.size_w;
    size_h = mtr.size_h;
    matrix = std::vector<double>(size_w * size_h, 0);
    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        matrix[y * size_w + x] = mtr.matrix[y * size_w + x];
  }

  // move constructor
  matr(matr&& mtr) {
    size_w = mtr.size_w;
    size_h = mtr.size_h;
    matrix = std::move(mtr.matrix);
  }

  // copy operator=
  matr& operator= (const matr& mtr) {
    if (&mtr != this) {
      size_w = mtr.size_w;
      size_h = mtr.size_h;
      matrix = std::vector<double>(size_w * size_h, 0);
      for (unsigned int y = 0; y < size_h; y++)
        for (unsigned int x = 0; x < size_w; x++)
          matrix[y * size_w + x] = mtr.matrix[y * size_w + x];
    }
    return *this;
  }

  // move operator=
  matr& operator= (matr&& mtr) {
    size_w = mtr.size_w;
    size_h = mtr.size_h;
    matrix = std::move(mtr.matrix);
    return *this;
  }

  // transponse
  matr makeTransponse () {
    matr mtrTrnsp(size_h, size_w);

    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        mtrTrnsp.matrix[x * size_h + y] = matrix[y * size_w + x];

    return mtrTrnsp;
  }

  // sum operator
  matr operator+(const matr& mtr) {
    matr newMatr(size_w, size_h);

    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        newMatr.matrix[y * size_w + x] = matrix[y * size_w + x] + mtr.matrix[y * size_w + x];

    return newMatr;
  }

  matr& operator+=(const matr& mtr) {
    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        matrix[y * size_w + x] += mtr.matrix[y * size_w + x];

    return *this;
  }

  // mul operator
  vec operator*(const vec& v) {
    vec newVec(size_h);

    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        newVec.vect[y] += matrix[y * size_w + x] * v.vect[x];

    return newVec;
  }

  // vec mul operators
  double * operator[](unsigned int index) {
    if (index >= size_h)
      throw std::exception("Uncorrect index of vec");

    return &matrix[index * size_w];
  }
};
