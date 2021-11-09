#pragma once

#include "mat.h"

enum MethodType
{
  NONE = 0, 
  InnerPoint,
  Gradient
};

// this class is represents matrix but using lesser data
struct idiot_matr
{
  size_t size_w, size_h;
  std::function<double(int, int)> get; // this function will return what placed in matrix cell with coords x, y
};

struct idiot_vec
{
  size_t size;
  std::function<double(int)> get; // this function will return what placed in vertex cell with coord x
};

class QuadraticProgramSolver
{
  public:
  //virtual vec<DIM> Solve(matr<DIM, DIM> Q, vec<DIM> C, matr<DIM, Restrictions> A, vec<Restrictions> B) override;
  virtual vec Solve(idiot_matr &Q, idiot_vec &C, idiot_matr &A, idiot_vec &B)
  {
    return vec(Q.size_h);
  }
};


#include "InnerPointSolver.h"
//#include "GradientSolver.h"