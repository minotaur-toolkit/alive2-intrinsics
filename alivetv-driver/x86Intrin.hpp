#include <array>
#include <immintrin.h>

#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsX86.h"

#include "ir/instr.h"

#ifndef TESTERX86INTRINBINOP_H
#define TESTERX86INTRINBINOP_H

typedef void (*voidFunctionType)(void);


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
  constexpr int bitSizeOp0() {
	constexpr int lanes = return_shape_op0<op>().first;
	constexpr int bitwidth = return_shape_op0<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<binOp op>
  constexpr int bitSizeOp1() {
	constexpr int lanes = return_shape_op1<op>().first;
	constexpr int bitwidth = return_shape_op1<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<binOp op>
  constexpr int bitSizeRet() {
	constexpr int lanes = return_shape_ret<op>().first;
	constexpr int bitwidth = return_shape_ret<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<binOp op>
  constexpr int bitwidthOp0() {
	constexpr int bitwidth = return_shape_op0<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
  }
  template<binOp op>
  constexpr int bitwidthOp1() {
	constexpr int bitwidth = return_shape_op1<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
  }
  template<binOp op>
  constexpr int bitwidthRet() {
	constexpr int bitwidth = return_shape_ret<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
  }
  template<unsigned long int index>
  constexpr binOp getOp()
  {
   	return static_cast<IR::X86IntrinBinOp::Op>(index);
  }

inline __m128i mm_srl_epi16(__m128i a, __m128i b) { return _mm_srl_epi16(a, b); }
inline __m128i mm_srl_epi32(__m128i a, __m128i b) { return _mm_srl_epi32(a, b); }
inline __m128i mm_srl_epi64(__m128i a, __m128i b) { return _mm_srl_epi64(a, b); }
inline __m256i mm256_srl_epi16(__m256i a, __m128i b) { return _mm256_srl_epi16(a, b); }
inline __m256i mm256_srl_epi32(__m256i a, __m128i b) { return _mm256_srl_epi32(a, b); }
inline __m256i mm256_srl_epi64(__m256i a, __m128i b) { return _mm256_srl_epi64(a, b); }
inline __m128i mm_avg_epu16(__m128i a, __m128i b) { return _mm_avg_epu16(a, b); }
inline __m256i mm256_avg_epu8(__m256i a, __m256i b) { return _mm256_avg_epu8(a, b); }
inline __m256i mm256_avg_epu16(__m256i a, __m256i b) { return _mm256_avg_epu16(a, b); }
inline __m256i mm256_shuffle_epi8(__m256i a, __m256i b) { return _mm256_shuffle_epi8(a, b); }
inline __m128i mm_shuffle_epi8(__m128i a, __m128i b) { return _mm_shuffle_epi8(a, b); }
inline __m64 mm_add_pi8(__m64 a, __m64 b) { return _mm_add_pi8(a, b); }
inline __m64 mm_add_pi16(__m64 a, __m64 b) { return _mm_add_pi16(a, b); }
inline __m64 mm_add_pi32(__m64 a, __m64 b) { return _mm_add_pi32(a, b); }
inline __m64 mm_unpackhi_pi8(__m64 a, __m64 b) { return _mm_unpackhi_pi8(a, b); }
inline __m64 m_punpckhwd(__m64 a, __m64 b) { return _m_punpckhwd(a, b); }
inline __m64 mm_unpackhi_pi32(__m64 a, __m64 b) { return _mm_unpackhi_pi32(a, b); }
inline __m64 mm_unpacklo_pi8(__m64 a, __m64 b) { return _mm_unpacklo_pi8(a, b); }
inline __m64 mm_unpacklo_pi16(__m64 a, __m64 b) { return _mm_unpacklo_pi16(a, b); }
inline __m64 mm_unpacklo_pi32(__m64 a, __m64 b) { return _mm_unpacklo_pi32(a, b); }
inline __m128i mm_sra_epi16(__m128i a, __m128i b) { return _mm_sra_epi16(a, b); }
inline __m128i mm_sra_epi32(__m128i a, __m128i b) { return _mm_sra_epi32(a, b); }
inline __m256i mm256_sra_epi16(__m256i a, __m128i b) { return _mm256_sra_epi16(a, b); }
inline __m256i mm256_sra_epi32(__m256i a, __m128i b) { return _mm256_sra_epi32(a, b); }
inline __m512i mm512_sra_epi16(__m512i a, __m128i b) { return _mm512_sra_epi16(a, b); }
inline __m512i mm512_sra_epi32(__m512i a, __m128i b) { return _mm512_sra_epi32(a, b); }
inline __m128i mm_sra_epi64(__m128i a, __m128i b) { return _mm_sra_epi64(a, b); }
inline __m256i mm256_sra_epi64(__m256i a, __m128i b) { return _mm256_sra_epi64(a, b); }
inline __m512i mm512_sra_epi64(__m512i a, __m128i b) { return _mm512_sra_epi64(a, b); }



#endif
