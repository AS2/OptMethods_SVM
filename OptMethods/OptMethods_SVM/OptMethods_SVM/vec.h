#pragma once

#include <vector>
#include <utility>

template<size_t size>
class vec {
public:
  std::vector<double> vect;

  // default constructor
  vec() {
    vect = std::vector<double>(size, 0);
  }

  // copy constructor
  vec(const vec<size>& v) {
    vect = std::vector<double>(size, 0);
    for (unsigned int i = 0; i < size; i++)
      vect[i] = v.vect[i];
  }

  // move constructor
  vec(vec<size>&& v) {
    vect = std::move(v.vect);
  }

  // copy operator=
  vec<size>& operator= (const vec<size>& v) {
    if (&v != this) {
      vect = std::vector<double>(size, 0);
      for (unsigned int i = 0; i < size; i++)
        vect[i] = v.vect[i];
    }
    return *this;
  }

  // move operator=
  vec<size>& operator= (vec<size>&& v) {
    vect = std::move(v.vect);
    return *this;
  }

  // sum operator
  vec<size> operator+ (const vec<size>& v) {
    vec<size> newVec;

    for (unsigned int i = 0; i < size; i++)
      newVec.vect[i] = v.vect[i] + vect[i];

    return newVec;
  }

  // dot operators
  double operator* (const vec<size>& v) {
    double res = 0;
    for (unsigned int i = 0; i < size; i++)
      res += vect[i] * v.vect[i];

    return res;
  }

  vec<size> operator* (const double& t) {
    vec<size> newVec;
    for (unsigned int i = 0; i < size; i++)
      newVec.vect[i] = t * vect[i];

    return newVec;
  }

  double& operator[](unsigned int index) {
    if (index >= vect.size())
      throw std::exception("Uncorrect index of vec");

    return vect[index];
  }

  size_t dimension() {
    return vect.size();
  }
};
