#pragma once

#include "mat.h"

template<size_t DIM>
class QuadraticProgramSolver
{
  public:
  vec<DIM> Solve(matr<DIM, DIM> Q, vec<DIM> C, matr<DIM, DIM> A, vec<DIM> B) override;
  //vec Solve( matr Q, vec C, matr A, vec B) override;
};