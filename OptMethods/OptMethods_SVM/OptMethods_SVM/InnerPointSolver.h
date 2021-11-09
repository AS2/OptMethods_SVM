#pragma once
#include "QuadraticProgramSolver.h"

class InnerPointSolver : public QuadraticProgramSolver
{
public:
  // result size is DIM
  // Q size is DIM * DIM
  // C size is DIM
  // A size is (DIM, Number of restrictions)
  // B size is Number of restrictions
  //vec<DIM> Solve(matr<DIM, DIM> Q, vec<DIM> C, matr<DIM, Restrictions> A, vec<Restrictions> B) override;
  vec Solve(idiot_matr &Q, idiot_vec &C, idiot_matr &A, idiot_vec &B) override
  {
    return vec(Q.size_h);
  }
};