#include "vectorutil.hpp"

#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H

template<typename T>
struct VectorWrapper
{
  VectorWrapper(T input, unsigned inputBitWidth, unsigned inputBitSize)
    	 : vector(input), bitSize(inputBitSize), bitWidth(inputBitWidth)
  {
    std::variant<int64_t*, int32_t*, int16_t*, int8_t*> valsVariant;
    
    if (bitWidth == 64)
      valsVariant = (int64_t*)&input;
    else if (bitWidth == 32)
      valsVariant = (int32_t*)&input;
    else if (bitWidth == 16)
      valsVariant = (int16_t*)&input;
    else if (bitWidth == 8)
      valsVariant = (int8_t*)&input;
    else
      throw std::invalid_argument("Bitwidth must be powers of 2 between 2 and 64");
  
    for(unsigned i = 0; i < bitSize / bitWidth; ++i)
      std::visit([&](auto&& arg){vals.push_back(static_cast<unsigned>(arg[i]));}, valsVariant);
  }

  bool operator==(const VectorWrapper &p) const {
    if(vals.size() != p.vals.size())
      return false;
    
    for(unsigned i = 0; i < vals.size(); ++i)
      if(vals[i] != p.vals[i])
        return false;

    return true;
  }

  T vector;
  const unsigned bitSize;
  const unsigned bitWidth;
  std::vector<unsigned> vals;
};

struct VectorWrapperHashFn
{
    template <class T>
    std::size_t operator() (const VectorWrapper<T> &vec) const
    {
      unsigned retVal = 0;
      for(unsigned i = 0; i < vec.bitSize / vec.bitWidth; ++i)
	retVal += (vec.vals[i] << i);

      return static_cast<std::size_t>(retVal);
    }
};

#endif
