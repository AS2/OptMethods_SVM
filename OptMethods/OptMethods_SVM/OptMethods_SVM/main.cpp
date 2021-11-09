#include <iostream>

#include "mat.h"
#include "SVMClassifier.h"

int main(void) {
  vec v1(3);
  matr m1(3, 3), m2(3, 3);

  v1.vect = { 1, 1, 1 };
  m1.matrix = { 1, 2, 3, 6, 5, 4, 0, 0, 0 };
  m2.matrix = { 0, 0, 0, 0, 0, 0, 7, 8, 9 };

  vec res((m1 + m2) * v1 * 6);
  //vec<3> res(v1 * 6 + v1);

  for (auto& i : res.vect)
    std::cout << i << " ";

  std::cout << res[2] << " ";


  //just init
  SVMClassifier<28> classifier;


  //

  try
  {
    auto TrainSet = LoadMNIST("MNIST/train-images.idx3-ubyte", "MNIST/train-labels.idx1-ubyte");
    auto TestSet = LoadMNIST("MNIST/t10k-images.idx3-ubyte", "MNIST/t10k-labels.idx1-ubyte");

    SVMClassifier<28*28> classifier;
    std::function<bool(DataElement<28*28> &El)> criteria = [](DataElement<28 * 28> &El) -> bool
    {
      return El.target >= 5;
    };


    classifier.train(TrainSet, 1e-4, criteria, MethodType::InnerPoint);

    // smoke test
    auto smokeTest = TestSet[rand() % TestSet.size()];
    bool res = classifier.process(smokeTest.input);

    if (res == criteria(smokeTest))
      std::cout << "Correct";
    else 
      std::cout << "Incorrect";

  }
  catch( std::exception &ex)
  {
    std::cout << ex.what();
  }
  return 0;
}
