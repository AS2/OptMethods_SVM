#pragma once

#include "vec.h"

#include <vector>
#include <utility>

/*
 * size_w - ÿ»–»Õ¿ Ã¿“–»÷€ (n)
 * size_h - ¬€—Œ“¿ Ã¿“–»÷€ (m)
 */
template<size_t size_w, size_t size_h>
class matr {
public:
  std::vector<std::vector<double>> matrix; // array of horizontal lines

  // default constructor
  matr() {
    matrix = std::vector<std::vector<double>>(size_h, std::vector<double>(size_w, 0));
  }

  // copy constructor
  matr(const matr<size_w, size_h>& mtr) {
    matrix = std::vector<std::vector<double>>(size_h, std::vector<double>(size_w, 0));
    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        matrix[y][x] = mtr.matrix[y][x];
  }

  // move constructor
  matr(matr<size_w, size_h>&& mtr) {
    matrix = std::move(mtr.matrix);
  }

  // copy operator=
  matr<size_w, size_h>& operator= (const matr<size_w, size_h>& mtr) {
    if (&mtr != this) {
      matrix = std::vector<std::vector<double>>(size_h, std::vector<double>(size_w, 0));
      for (unsigned int y = 0; y < size_h; y++)
        for (unsigned int x = 0; x < size_w; x++)
          matrix[y][x] = mtr.matrix[y][x];
    }
    return *this;
  }

  // move operator=
  matr<size_w, size_h>& operator= (matr<size_w, size_h>&& mtr) {
    matrix = std::move(mtr.matrix);
    return *this;
  }

  // transponse
  matr<size_h, size_w> makeTransponse () {
    matr<size_h, size_w> mtrTrnsp;

    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        mtrTrnsp.matrix[x][y] = matrix[y][x];

    return mtrTrnsp;
  }

  // sum operator
  matr<size_w, size_h> operator+(const matr<size_w, size_h>& mtr) {
    matr<size_w, size_h> newMatr;

    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        newMatr.matrix[y][x] = matrix[y][x] + mtr.matrix[y][x];

    return newMatr;
  }

  matr<size_w, size_h>& operator+=(const matr<size_w, size_h>& mtr) {
    for (unsigned int y = 0; y < size_h; y++)
      for (unsigned int x = 0; x < size_w; x++)
        matrix[y][x] += mtr.matrix[y][x];

    return *this;
  }

  // mul operator
  vec<size_h> operator*(const vec<size_w>& v);

  // vec mul operators
  std::vector<double>& operator[](unsigned int index) {
    if (index >= size_h)
      throw std::exception("Uncorrect index of vec");

    return matrix[index];
  }
};
