#pragma once

#include <functional>

#include "DataSet.h"
#include "QuadraticProgramSolver.h"
#include "mat.h"

// we want to build function like
// w * x = b and return in process if we like 
// in MNIST Case DataDimension = 28*28
template <size_t DataDimension>
class SVMClassifier
{
  const unsigned int dim = DataDimension;
  vec<DataDimension> w; // dimension is dim
  double b;
  QuadraticProgramSolver<DataDimension> *Method;
public:
  
  // will calculate this->w and this->b;
  // return error
  double train(DataSet<DataDimension> train_set, double error_coeff, std::function<bool(DataElement<DataDimension>)> classifier)
  {
    //our task is to find w, b and err where 
    // w - vector dim
    // b - scalar
    // err - vector (1 coord for every element in train_set)
    const unsigned int size_for_solver = dim + 1 + train_set.size();
    //assert(train_set.size() == 0 || train_set[0].dimension() == dim);
    vec<size_for_solver> w_to_find;
    
    /// TARGET FUNC
    
    // identity without last 1 + train_set.size() lines like this
    // 1, 0, 0, ... 0
    // 0, 1, 0, ... 0
    // 0, 0, 1, ... 0
    // ... ... ... ..
    // 0, 0, 0, ... 0
    matr<size_for_solver, size_for_solver> Q;

    // like (0, 0, 0, 0, ... ,error_coeff, error_coeff,error_coeff, error_coeff)
    vec<size_for_solver> C;

    // target(goal) function is 
    // w_to_find.transposed * Q * w_to_find + C * w_to_find

    /// LIMITS
    
    // limit_A is calculated from 
    // ci * sum(from 0 to dim by j)(w_to_find[j]*xi[j]) - ci * w_to_find[dim] + w_to_find[dim + 1 + i] <= 1 where i <= dim - 1
    // and
    // -1.0 * w_to_find[i] <= 0 where i >= dim + 1
    matr<size_for_solver, size_for_solver> limit_A;
    for (int i = 0; i < dim + 1 + train_set.size(); i++)
    {
      vec<DataDimension> xi = train_set[i].input;
      double ci = classifier(train_set[i]) ? 1 : -1;

      // do not ask why. Just read some books idk (sasa4: too much rude)
      for (int j = 0; j < dim; j++)
        limit_A[i][j] = ci * xi[j];
      limit_A[i][dim] = -ci;
      limit_A[i][dim + 1 + i] = 1;
    }

    // is like (1, 1, ... , 1, FLT_INF, 0, ,0 ,0, ,0) 
    vec<size_for_solver> limit_b;

    w_to_find = Method->Solve(Q, C, limit_A, limit_b);

    for (int i = 0; i < dim; i++)
      w[i] = w_to_find[i];

    b = w_to_find[dim];
    
    double sum_err = 0;

    for (int i = dim + 1; i < dim + 1 + train_set.size(); i++)
      sum_err += w_to_find[i];

    return sum_err;
  }
  
  // return result of trained classifier
  bool process(vec<DataDimension> input)
  {
    //assert(input.dimension() == dim);
    return w * input - b >= 0;
  }
};
