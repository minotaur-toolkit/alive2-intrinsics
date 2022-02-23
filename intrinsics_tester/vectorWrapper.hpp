// Copyright (c) 2021-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: February 23, 2022

#include "vectorutil.hpp"

#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H

template <unsigned ibitWidth, unsigned ibitSize, typename T>
struct VectorWrapper {
  VectorWrapper() {}

  VectorWrapper(T input) : vector(input) {
    std::variant<int64_t *, int32_t *, int16_t *, int8_t *> valsVariant;

    if constexpr (bitWidth == 64)
      valsVariant = (int64_t *)&input;
    else if constexpr (bitWidth == 32)
      valsVariant = (int32_t *)&input;
    else if constexpr (bitWidth == 16)
      valsVariant = (int16_t *)&input;
    else if constexpr (bitWidth == 8)
      valsVariant = (int8_t *)&input;
    else
      throw std::invalid_argument(
          "VC: Bitwidth must be powers of 2 between 2 and 64");

    for (unsigned i = 0; i < bitSize / bitWidth; ++i)
      std::visit(
          [&](auto &&arg) { vals.push_back(static_cast<unsigned>(arg[i])); },
          valsVariant);
  }

  bool operator==(const VectorWrapper &p) const {
    if (vals.size() != p.vals.size())
      return false;

    for (unsigned i = 0; i < vals.size(); ++i)
      if (vals[i] != p.vals[i])
        return false;

    return true;
  }

  void printVec() { ::printVec<bitWidth>(vector); }

  T vector;
  static constexpr unsigned bitSize = ibitSize;
  static constexpr unsigned bitWidth = ibitWidth;
  std::vector<unsigned> vals;
};

struct VectorWrapperHashFn {
  template <unsigned iBW, unsigned iBS, class T>
  std::size_t operator()(const VectorWrapper<iBW, iBS, T> &vec) const {
    unsigned retVal = 0;
    for (unsigned i = 0; i < vec.bitSize / vec.bitWidth; ++i)
      retVal += (vec.vals[i] << i);

    return static_cast<std::size_t>(retVal);
  }
};

#endif
