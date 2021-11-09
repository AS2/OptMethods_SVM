#pragma once

#include <vector>
#include <utility>

class vec {
public:
  size_t size = 0;
  std::vector<double> vect;

  // default constructor
  vec(size_t size) {
    this->size = size;
    vect = std::vector<double>(size, 0);
  }

  // copy constructor
  vec(const vec& v) {
    size = v.size;
    vect = std::vector<double>(size, 0);
    for (unsigned int i = 0; i < size; i++)
      vect[i] = v.vect[i];
  }

  // move constructor
  vec(vec&& v) {
    size = v.size;
    vect = std::move(v.vect);
  }

  // copy operator=
  vec& operator= (const vec& v) {
    if (&v != this) {
      size = v.size;
      vect = std::vector<double>(size, 0);
      for (unsigned int i = 0; i < size; i++)
        vect[i] = v.vect[i];
    }
    return *this;
  }

  // move operator=
  vec& operator= (vec&& v) {
    size = v.size;
    vect = std::move(v.vect);
    return *this;
  }

  // sum operator
  vec operator+ (const vec& v) {
    vec newVec(size);

    for (unsigned int i = 0; i < size; i++)
      newVec.vect[i] = v.vect[i] + vect[i];

    return newVec;
  }

  // dot operators
  double operator* (const vec& v) {
    double res = 0;
    for (unsigned int i = 0; i < size; i++)
      res += vect[i] * v.vect[i];

    return res;
  }

  vec operator* (const double& t) {
    vec newVec(size);
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
