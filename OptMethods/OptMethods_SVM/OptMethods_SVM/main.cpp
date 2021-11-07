#include <iostream>

#include "mat.h"
#include "SVMClassifier.h"

int main(void) {
  vec<3> v1;
  matr<3, 3> m1, m2;

  v1.vect = { 1, 1, 1 };
  m1.matrix = { {1, 2, 3}, {6, 5, 4}, {0, 0, 0} };
  m2.matrix = { {0, 0, 0}, {0, 0, 0}, {7, 8, 9} };

  vec<3> res((m1 + m2) * v1 * 6);
  //vec<3> res(v1 * 6 + v1);

  for (auto& i : res.vect)
    std::cout << i << " ";

  std::cout << res[2] << " ";


  //just init
  SVMClassifier<28> classifier;

  std::cout << "kek";
  return 0;
}
