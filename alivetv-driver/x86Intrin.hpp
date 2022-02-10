#include <array>
#include <immintrin.h>

#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsX86.h"

#include "ir/instr.h"

#ifndef TESTERX86INTRINBINOP_H
#define TESTERX86INTRINBINOP_H

using binOp = IR::X86IntrinBinOp::Op;

class TesterX86IntrinBinOp final {
public:
  // ir/instr.h members of X86IntrinBinOp
  // numOfX86Intrinsics
  // enum Op
  // shape_op0
  // shape_op1
  // shape_ret  	
  
  static std::array<llvm::Intrinsic::ID, IR::X86IntrinBinOp::numOfX86Intrinsics> intrin_id;
};


template<binOp input>
constexpr std::pair<unsigned, unsigned> return_shape_op0()
{
  return std::get<input>(IR::X86IntrinBinOp::shape_op0);
}


template<binOp input>
constexpr std::pair<unsigned, unsigned> return_shape_op1()
{
  return std::get<input>(IR::X86IntrinBinOp::shape_op1);
}


template<binOp input>
constexpr std::pair<unsigned, unsigned> return_shape_ret()
{
  return std::get<input>(IR::X86IntrinBinOp::shape_ret);
}


template<binOp op>
constexpr int bitSizeOp0() 
{
  constexpr int lanes = return_shape_op0<op>().first;
  constexpr int bitwidth = return_shape_op0<op>().second;
  constexpr int bitSize = lanes * bitwidth;
  
  static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
  return bitSize;
}


// Operator 1 may have only single integers as input
template<binOp op>
constexpr int bitSizeOp1() 
{
  constexpr int lanes = return_shape_op1<op>().first;
  constexpr int bitwidth = return_shape_op1<op>().second;
  constexpr int bitSize = lanes * bitwidth;
  
  static_assert(  (bitSize == 32) ||
		  (bitSize == 128) || (bitSize == 256) || (bitSize == 512));
  return bitSize;
}


template<binOp op>
constexpr int bitSizeRet() 
{
  constexpr int lanes = return_shape_ret<op>().first;
  constexpr int bitwidth = return_shape_ret<op>().second;
  constexpr int bitSize = lanes * bitwidth;
  
  static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
  return bitSize;
}


template<binOp op>
constexpr int bitwidthOp0() 
{
  constexpr int bitwidth = return_shape_op0<op>().second;
  
  static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
  return bitwidth;
}


template<binOp op>
constexpr int bitwidthOp1() 
{
  constexpr int bitwidth = return_shape_op1<op>().second;
  
  static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
  return bitwidth;
}


template<binOp op>
constexpr int bitwidthRet() 
{
  constexpr int bitwidth = return_shape_ret<op>().second;
  
  static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
  return bitwidth;
}


template<unsigned long int index>
constexpr binOp getOp()
{
  return static_cast<IR::X86IntrinBinOp::Op>(index);
}

#endif
