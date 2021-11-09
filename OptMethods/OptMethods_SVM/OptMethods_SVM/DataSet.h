#pragma once

#include <functional>
#include "mat.h"

template <size_t DataDimension>
class DataElement
{
public:
  vec input = vec(DataDimension);
  int target;

  size_t dimension() {
    return input.dimension();
  }
};

template <size_t DataDimension>
class DataSet
{
private:
  std::vector<DataElement<DataDimension>> Data;
public:
  // load from file
  void Add(const DataElement<DataDimension> &A)
  {
    Data.push_back(A);
  }

  void Add(DataElement<DataDimension> && A)
  {
    Data.push_back(std::move(A));
  }

  DataElement<DataDimension> & CreateNewAndReturnIt()
  {
    Data.emplace_back();
    return Data.back();
  }

  void Reserve(size_t size)
  {
    Data.reserve(size);
  }

  //get by []

  DataElement<DataDimension>& operator[] (const size_t& index) {
    if (index >= Data.size())
      throw std::exception("['DataSet' exception ('operator[]')] - Too big index");

    return Data[index];
  }

  // number of different target-s
  size_t size() {
    return Data.size();
  }

  // split in two by lambda
  // (for example 0 ... 9 to 0 .. 4, 5 .. 9)
  std::pair<DataSet<DataDimension>, DataSet<DataDimension>> Split(std::function<bool(DataElement<DataDimension>)> classifier)
  {
    DataSet<DataDimension> first; 
    DataSet<DataDimension> second;
    for (auto & i : Data)
      if (classifier(i))
        first.Data(i);
      else
        second.Data(i);
   
    return std::make_pair(first, second);
  }
};

DataSet<28 * 28> LoadMNIST(std::string images, std::string labels);
