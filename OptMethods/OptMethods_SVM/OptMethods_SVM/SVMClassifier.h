#pragma once

#include <functional>

#include "DataSet.h"
#include "QuadraticProgramSolver.h"
#include "mat.h"

//#define EXPLANATION

// we want to build function like
// w * x = b and return in process if we like 
// in MNIST Case DataDimension = 28*28
template <size_t DataDimension>
class SVMClassifier
{
  const size_t dim = DataDimension;
  vec w = vec(dim); // dimension is dim
  double b;

  // will calculate this->w and this->b;
  // return error
  double train_internal(DataSet<DataDimension> &train_set, double error_coeff, std::function<bool(DataElement<DataDimension> &)> classifier, QuadraticProgramSolver *Method)
  {
    //our task is to find w, b and err where 
    // w - vector dim
    // b - scalar
    // err - vector (1 coord for every element in train_set)
    const size_t size_for_solver = dim + 1 + train_set.size();
    //assert(train_set.size() == 0 || train_set[0].dimension() == dim);
    vec w_to_find(size_for_solver);
    
    // Quadratic programming problem is when 
    
    // you have
    // matrix Q
    // vector C
    // matrix limit_A
    // vector limit_b

    // and you want to get vector w_to_find
    // 1/2 * w_to_find.transposed * Q * w_to_find + C.transposed * w_to_find - min
    // where limit_A * w_to_find <= limit_b
    

    // SVM is about finding vector w and scalar b and vector err so
    // 1/2 * w.transposed * w + sum(err) * error_coeff - min
    // where
    // train_set[i].input *  (w * ci - b) >= 1-err[i] for 1 <= i <= train_set.size()  (ci = classifier(train_set[i]) ? 1 : -1)
    // err[i] >= 0

    /// TARGET FUNC    
    // identity without last 1 + train_set.size() lines like this
    // 1, 0, 0, ... 0
    // 0, 1, 0, ... 0
    // 0, 0, 1, ... 0
    // ... ... ... ..
    // 0, 0, 0, ... 0
    idiot_matr Q;
    Q.size_w = size_for_solver;
    Q.size_h = size_for_solver;
    Q.get = [&](int x, int y) {
      return (x == y && x < dim) ? 1 : 0;
    };

    /*
    matr Q(size_for_solver, size_for_solver);
    for (int i = 0; i < dim; i++)
      Q[i][i] = 1;
    */

    // like (0, 0, 0, 0, ... ,error_coeff, error_coeff,error_coeff, error_coeff)
    idiot_vec C;
    C.size = size_for_solver;
    C.get = [&](int x) {
      return (x >= dim + 1) ? error_coeff : 0;
    };

    /*
    vec C(size_for_solver);
    for (size_t i = dim + 1; i < size_for_solver; i++)
      C[i] = error_coeff;
    */
    // target(goal) function is 
    // w_to_find.transposed * Q * w_to_find + C * w_to_find

    /// LIMITS
    
#ifdef EXPLANATION
    // limit_A is calculated from 
    // ci * sum(from 0 to dim by j)(w_to_find[j]*xi[j]) - ci * w_to_find[dim] + w_to_find[dim + 1 + i] >= 1 where i <= train_set_size;
    // and
    // -1.0 * w_to_find[dim + 1 + i] <= 0 where i <= train_set_size

    // so we have train_set.size() * 2 limits
    matr limit_A(size_for_solver, train_set.size() * 2);

    // first limit
    for (int i = 0; i < train_set.size(); i++)
    {
      vec xi = train_set[i].input;
      double ci = classifier(train_set[i]) ? 1 : -1;

      // do not ask why. Just read some books idk (sasa4: too much rude)
      // ok. this cycle makes ci * xi * w 
      for (int j = 0; j < dim; j++)
        limit_A[i][j] = ci * xi[j];

      // ok. this makes -ci * b 
      limit_A[i][dim] = -ci;
      // this makes 1 * err[i]
      limit_A[i][dim + 1 + i] = 1;

      // so it sums up to  ci * xi * w  -ci * b + 1 * err[i] = ci*(w * xi - b) + err[i]. 
      // we want it to be >= 1
      // A >= B <=> -A <= -B

      for (int j = 0; j < dim; j++)
        limit_A[i][j] = -limit_A[i][j];

      limit_A[i][dim] = -limit_A[i][dim];
      limit_A[i][dim + 1 + i] = -limit_A[i][dim + 1 + i];


      // also we want err[i] be greater than 0
      limit_A[i + dim + 1][i + dim + 1]
    }

    // second limit
    for (int i = 0; i < train_set.size(); i++)
    {
      limit_A[i + train_set.size()][dim + 1 + i] = -1.0;
    }
#else
    // explanation is under define so preprocessor will remove it and we will have faster code
    /*
    matr limit_A(size_for_solver, train_set.size() * 2);
    for (int i = 0; i < train_set.size(); i++)
    {
      vec xi = train_set[i].input;
      double ci = classifier(train_set[i]) ? 1 : -1;
      for (int j = 0; j < dim; j++)
        limit_A[i][j] = -ci * xi[j];

      limit_A[i][dim] = ci;
      limit_A[i][dim + 1 + i] = -1;
      limit_A[i + train_set.size()][dim + 1 + i] = -1.0;
    }
    */
    idiot_matr limit_A;
    limit_A.size_w = size_for_solver;
    limit_A.size_h = train_set.size() * 2;

    limit_A.get = [&](int x, int y) {
      if (y < train_set.size())
      {
        vec xi = train_set[y].input;
        double ci = classifier(train_set[y]) ? 1 : -1;
        if (x < dim)
          return -ci * xi[x];

        if (x == dim)
          return ci;
        if (x == dim + 1 + y)
          return -1.0;
      }
      else
      {
        if (x == ((y - train_set.size()) + dim + 1))
          return -1.0;
      }
      return 0.0;
    };

#endif

    // is like (-1, -1, ... , -1, 0, ,0 ,0, ,0) 
    /*
    vec limit_b(train_set.size() * 2);

    for (int i = 0; i < train_set.size() * 2; i++)
      if (i < train_set.size())
        limit_b[i] = -1;
      else 
        limit_b[i] = 0;
    */
    idiot_vec limit_b;
    limit_b.size = train_set.size();
    limit_b.get = [&](int x) {
      return (x < train_set.size()) ? -1.0 : 0.0;
    };

    if (Method != nullptr)
      w_to_find = Method->Solve(Q, C, limit_A, limit_b);

    for (int i = 0; i < dim; i++)
      w[i] = w_to_find[i];

    b = w_to_find[dim];
    
    double sum_err = 0;

    for (int i = dim + 1; i < dim + 1 + train_set.size(); i++)
      sum_err += w_to_find[i];

    return sum_err;
  }
public:

  // will calculate with choosen method
  // return error
  double train(DataSet<DataDimension> &train_set, double error_coeff, std::function<bool(DataElement<DataDimension> &)> classifier, MethodType type )
  {
    switch (type)
    {
    case NONE:
      break;
    case InnerPoint:
    {
      InnerPointSolver solver;
      return train_internal(train_set, error_coeff, classifier, &solver);
    }
    default:
      break;
    }
    return -1;
  }


  // return result of trained classifier
  bool process(vec input)
  {
    if (input.dimension() != dim)
      throw std::exception("input.dimension() != dim in process in SVMClassifier");
    return w * input - b >= 0;
  }
};
