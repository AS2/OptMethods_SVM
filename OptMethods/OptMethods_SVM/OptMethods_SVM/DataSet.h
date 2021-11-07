#pragma once

#include <functional>
#include "mat.h"

template <size_t DataDimension>
class DataElement
{
public:
  vec<DataDimension> input;
  int target;

  size_t dimension() {
    return input.size();
  }
};

template <size_t DataDimension>
class DataSet
{
private:
  std::vector<DataElement<DataDimension>> Data;
public:
  // load from file

  //get by []

  DataElement<DataDimension>& operator[] (const size_t& index) const {
    if (index >= Data.size())
      throw std::exception("['DataSet' exception ('operator[]')] - Too big index");

    return Data[index];
  }

  size_t size() {
    return Data.size();
  }

  // split in two by lambda
  std::pair<DataSet<DataDimension>, DataSet<DataDimension>> Split(std::function<bool(DataElement<DataDimension>)> classifier);

  // number of different target-s
};

// 0 ... 9

// 0 .. 4, 5 .. 9