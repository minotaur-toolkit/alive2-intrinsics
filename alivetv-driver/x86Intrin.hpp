#include <array>
#include <immintrin.h>

#ifndef X86INTRINBINOP_H
#define X86INTRINBINOP_H

typedef void (*voidFunctionType)(void);

class X86IntrinBinOp final {
  static constexpr unsigned numOfX86Intrinsics = 47;
public:
  enum Op {
  /* llvm.x86.sse2.psrl.w */        sse2_psrl_w,
  /* llvm.x86.sse2.psrl.d */        sse2_psrl_d,
  /* llvm.x86.sse2.psrl.q */        sse2_psrl_q,
  /* llvm.x86.avx2.psrl.w */        avx2_psrl_w,
  /* llvm.x86.avx2.psrl.d */        avx2_psrl_d,
  /* llvm.x86.avx2.psrl.q */        avx2_psrl_q,
  /* llvm.x86.sse2.pavg.w */        sse2_pavg_w,
  /* llvm.x86.avx2.pavg.b */        avx2_pavg_b,
  /* llvm.x86.avx2.pavg.w */        avx2_pavg_w,
  /* llvm.x86.avx2.pshuf.b */       avx2_pshuf_b,
  /* llvm.x86.ssse2.pshuf.b.128 */  ssse3_pshuf_b_128,
  /* llvm.x86.mmx.padd.b */         mmx_padd_b,
  /* llvm.x86.mmx.padd.w */         mmx_padd_w,
  /* llvm.x86.mmx.padd.d */         mmx_padd_d,
  /* llvm.x86.mmx.punpckhbw */      mmx_punpckhbw,
  /* llvm.x86.mmx.punpckhwd */      mmx_punpckhwd,
  /* llvm.x86.mmx.punpckhdq */      mmx_punpckhdq,
  /* llvm.x86.mmx.punpcklbw */      mmx_punpcklbw,
  /* llvm.x86.mmx.punpcklwd */      mmx_punpcklwd,
  /* llvm.x86.mmx.punpckldq */      mmx_punpckldq,
  /* llvm.x86.sse2.psrai.w */       sse2_psrai_w,
  /* llvm.x86.sse2.psrai.d */       sse2_psrai_d,
  /* llvm.x86.avx2.psrai.w */       avx2_psrai_w,
  /* llvm.x86.avx2.psrai.d */       avx2_psrai_d,
  /* llvm.x86.avx512.psrai.w.512 */ avx512_psrai_w_512,
  /* llvm.x86.avx512.psrai.d.512 */ avx512_psrai_d_512,
  /* llvm.x86.avx512.psrai.q.128 */ avx512_psrai_q_128,
  /* llvm.x86.avx512.psrai.q.256 */ avx512_psrai_q_256,
  /* llvm.x86.avx512.psrai.q.512 */ avx512_psrai_q_512,
  /* llvm.x86.sse2.psrli.w */       sse2_psrli_w,
  /* llvm.x86.sse2.psrli.d */       sse2_psrli_d,
  /* llvm.x86.sse2.psrli.q */       sse2_psrli_q,
  /* llvm.x86.avx2.psrli.w */       avx2_psrli_w,
  /* llvm.x86.avx2.psrli.d */       avx2_psrli_d,
  /* llvm.x86.avx2.psrli.q */       avx2_psrli_q,
  /* llvm.x86.avx512.psrli.w.512 */ avx512_psrli_w_512,
  /* llvm.x86.avx512.psrli.d.512 */ avx512_psrli_d_512,
  /* llvm.x86.avx512.psrli.q.512 */ avx512_psrli_q_512,
  /* llvm.x86.sse2.pslli.w */       sse2_pslli_w,
  /* llvm.x86.sse2.pslli.d */       sse2_pslli_d,
  /* llvm.x86.sse2.pslli.q */       sse2_pslli_q,
  /* llvm.x86.avx2.pslli.w */       avx2_pslli_w,
  /* llvm.x86.avx2.pslli.d */       avx2_pslli_d,
  /* llvm.x86.avx2.pslli.q */       avx2_pslli_q,
  /* llvm.x86.avx512.pslli.w.512 */ avx512_pslli_w_512,
  /* llvm.x86.avx512.pslli.d.512 */ avx512_pslli_d_512,
  /* llvm.x86.avx512.pslli.q.512 */ avx512_pslli_q_512,
  };

  // the shape of a vector is stored as <# of lanes, element bits>
  static constexpr std::array<std::pair<unsigned, unsigned>, numOfX86Intrinsics> shape_op0 = {
  /* sse2_psrl_w */        std::make_pair(8, 16),
  /* sse2_psrl_d */        std::make_pair(4, 32),
  /* sse2_psrl_w */        std::make_pair(2, 64),
  /* avx2_psrl_w */        std::make_pair(16, 16),
  /* avx2_psrl_d */        std::make_pair(8, 32),
  /* avx2_psrl_w */        std::make_pair(4, 64),
  /* sse2_pavg_w */        std::make_pair(8, 16),
  /* avx2_pavg_b */        std::make_pair(32, 8),
  /* avx2_pavg_w */        std::make_pair(16, 16),
  /* avx2_pshuf_b */       std::make_pair(32, 8),
  /* ssse3_pshuf_b_128 */  std::make_pair(16, 8),
  /* mmx_padd_b */         std::make_pair(8, 8),
  /* mmx_padd_w */         std::make_pair(4, 16),
  /* mmx_padd_d */         std::make_pair(2, 32),
  /* mmx_punpckhbw */      std::make_pair(8, 8),
  /* mmx_punpckhwd */      std::make_pair(4, 16),
  /* mmx_punpckhdq */      std::make_pair(2, 32),
  /* mmx_punpcklbw */      std::make_pair(8, 8),
  /* mmx_punpcklwd */      std::make_pair(4, 16),
  /* mmx_punpckldq */      std::make_pair(2, 32),
  /* sse2_psrai_w */       std::make_pair(8, 16),
  /* sse2_psrai_d */       std::make_pair(4, 32),
  /* avx2_psrai_w */       std::make_pair(16, 16),
  /* avx2_psrai_d */       std::make_pair(8, 32),
  /* avx512_psrai_w_512 */ std::make_pair(32, 16),
  /* avx512_psrai_d_512 */ std::make_pair(16, 32),
  /* avx512_psrai_q_128 */ std::make_pair(2, 64),
  /* avx512_psrai_q_256 */ std::make_pair(4, 64),
  /* avx512_psrai_q_512 */ std::make_pair(8, 64),
  /* sse2_psrli_w */       std::make_pair(8, 16),
  /* sse2_psrli_d */       std::make_pair(4, 32),
  /* sse2_psrli_q */       std::make_pair(2, 64),
  /* avx2_psrli_w */       std::make_pair(16, 16),
  /* avx2_psrli_d */       std::make_pair(8, 32),
  /* avx2_psrli_q */       std::make_pair(4, 64),
  /* avx512_psrli_w_512 */ std::make_pair(32, 16),
  /* avx512_psrli_d_512 */ std::make_pair(16, 32),
  /* avx512_psrli_q_512 */ std::make_pair(8, 64),
  /* sse2_pslli_w */       std::make_pair(8, 16),
  /* sse2_pslli_d */       std::make_pair(4, 32),
  /* sse2_pslli_q */       std::make_pair(2, 64),
  /* avx2_pslli_w */       std::make_pair(16, 16),
  /* avx2_pslli_d */       std::make_pair(8, 32),
  /* avx2_pslli_q */       std::make_pair(4, 64),
  /* avx512_pslli_w_512 */ std::make_pair(32, 16),
  /* avx512_pslli_d_512 */ std::make_pair(16, 32),
  /* avx512_pslli_q_512 */ std::make_pair(8, 64),
  };
  static constexpr std::array<std::pair<unsigned, unsigned>, numOfX86Intrinsics> shape_op1 = {
  /* sse2_psrl_w */        std::make_pair(8, 16),
  /* sse2_psrl_d */        std::make_pair(4, 32),
  /* sse2_psrl_w */        std::make_pair(2, 64),
  /* avx2_psrl_w */        std::make_pair(8, 16),
  /* avx2_psrl_d */        std::make_pair(4, 32),
  /* avx2_psrl_w */        std::make_pair(2, 64),
  /* sse2_pavg_w */        std::make_pair(8, 16),
  /* avx2_pavg_b */        std::make_pair(32, 8),
  /* avx2_pavg_w */        std::make_pair(16, 16),
  /* avx2_pshuf_b */       std::make_pair(32, 8),
  /* ssse3_pshuf_b_128 */  std::make_pair(16, 8),
  /* mmx_padd_b */         std::make_pair(8, 8),
  /* mmx_padd_w */         std::make_pair(4, 16),
  /* mmx_padd_d */         std::make_pair(2, 32),
  /* mmx_punpckhbw */      std::make_pair(8, 8),
  /* mmx_punpckhwd */      std::make_pair(4, 16),
  /* mmx_punpckhdq */      std::make_pair(2, 32),
  /* mmx_punpcklbw */      std::make_pair(8, 8),
  /* mmx_punpcklwd */      std::make_pair(4, 16),
  /* mmx_punpckldq */      std::make_pair(2, 32),
  /* sse2_psrai_w */       std::make_pair(1, 32),
  /* sse2_psrai_d */       std::make_pair(1, 32),
  /* avx2_psrai_w */       std::make_pair(1, 32),
  /* avx2_psrai_d */       std::make_pair(1, 32),
  /* avx512_psrai_w_512 */ std::make_pair(1, 32),
  /* avx512_psrai_d_512 */ std::make_pair(1, 32),
  /* avx512_psrai_q_128 */ std::make_pair(1, 32),
  /* avx512_psrai_q_256 */ std::make_pair(1, 32),
  /* avx512_psrai_q_512 */ std::make_pair(1, 32),
  /* sse2_psrli_w */       std::make_pair(1, 32),
  /* sse2_psrli_d */       std::make_pair(1, 32),
  /* sse2_psrli_q */       std::make_pair(1, 32),
  /* avx2_psrli_w */       std::make_pair(1, 32),
  /* avx2_psrli_d */       std::make_pair(1, 32),
  /* avx2_psrli_q */       std::make_pair(1, 32),
  /* avx512_psrli_w_512 */ std::make_pair(1, 32),
  /* avx512_psrli_d_512 */ std::make_pair(1, 32),
  /* avx512_psrli_q_512 */ std::make_pair(1, 32),
  /* sse2_pslli_w */       std::make_pair(1, 32),
  /* sse2_pslli_d */       std::make_pair(1, 32),
  /* sse2_pslli_q */       std::make_pair(1, 32),
  /* avx2_pslli_w */       std::make_pair(1, 32),
  /* avx2_pslli_d */       std::make_pair(1, 32),
  /* avx2_pslli_q */       std::make_pair(1, 32),
  /* avx512_pslli_w_512 */ std::make_pair(1, 32),
  /* avx512_pslli_d_512 */ std::make_pair(1, 32),
  /* avx512_pslli_q_512 */ std::make_pair(1, 32),
  };
  static constexpr std::array<std::pair<unsigned, unsigned>, numOfX86Intrinsics> shape_ret = {
  /* sse2_psrl_w */        std::make_pair(8, 16),
  /* sse2_psrl_d */        std::make_pair(4, 32),
  /* sse2_psrl_w */        std::make_pair(2, 64),
  /* avx2_psrl_w */        std::make_pair(16, 16),
  /* avx2_psrl_d */        std::make_pair(8, 32),
  /* avx2_psrl_w */        std::make_pair(4, 64),
  /* sse2_pavg_w */        std::make_pair(8, 16),
  /* avx2_pavg_b */        std::make_pair(32, 8),
  /* avx2_pavg_w */        std::make_pair(16, 16),
  /* avx2_pshuf_b */       std::make_pair(32, 8),
  /* ssse3_pshuf_b_128 */  std::make_pair(16, 8),
  /* mmx_padd_b */         std::make_pair(8, 8),
  /* mmx_padd_w */         std::make_pair(4, 16),
  /* mmx_padd_d */         std::make_pair(2, 32),
  /* mmx_punpckhbw */      std::make_pair(8, 8),
  /* mmx_punpckhwd */      std::make_pair(4, 16),
  /* mmx_punpckhdq */      std::make_pair(2, 32),
  /* mmx_punpcklbw */      std::make_pair(8, 8),
  /* mmx_punpcklwd */      std::make_pair(4, 16),
  /* mmx_punpckldq */      std::make_pair(2, 32),
  /* sse2_psrai_w */       std::make_pair(8, 16),
  /* sse2_psrai_d */       std::make_pair(4, 32),
  /* avx2_psrai_w */       std::make_pair(16, 16),
  /* avx2_psrai_d */       std::make_pair(8, 32),
  /* avx512_psrai_w_512 */ std::make_pair(32, 16),
  /* avx512_psrai_d_512 */ std::make_pair(16, 32),
  /* avx512_psrai_q_128 */ std::make_pair(2, 64),
  /* avx512_psrai_q_256 */ std::make_pair(4, 64),
  /* avx512_psrai_q_512 */ std::make_pair(8, 64),
  /* sse2_psrli_w */       std::make_pair(8, 16),
  /* sse2_psrli_d */       std::make_pair(4, 32),
  /* sse2_psrli_q */       std::make_pair(2, 64),
  /* avx2_psrli_w */       std::make_pair(16, 16),
  /* avx2_psrli_d */       std::make_pair(8, 32),
  /* avx2_psrli_q */       std::make_pair(4, 64),
  /* avx512_psrli_w_512 */ std::make_pair(32, 16),
  /* avx512_psrli_d_512 */ std::make_pair(16, 32),
  /* avx512_psrli_q_512 */ std::make_pair(8, 64),
  /* sse2_pslli_w */       std::make_pair(8, 16),
  /* sse2_pslli_d */       std::make_pair(4, 32),
  /* sse2_pslli_q */       std::make_pair(2, 64),
  /* avx2_pslli_w */       std::make_pair(16, 16),
  /* avx2_pslli_d */       std::make_pair(8, 32),
  /* avx2_pslli_q */       std::make_pair(4, 64),
  /* avx512_pslli_w_512 */ std::make_pair(32, 16),
  /* avx512_pslli_d_512 */ std::make_pair(16, 32),
  /* avx512_pslli_q_512 */ std::make_pair(8, 64),
  };
  static std::array<voidFunctionType, numOfX86Intrinsics> func_ret;
  
  static constexpr unsigned return_intrinsic_count()
  {
	return numOfX86Intrinsics;
  }
};
  template<X86IntrinBinOp::Op input>
  constexpr std::pair<unsigned, unsigned> return_shape_op0()
  {
	return std::get<input>(X86IntrinBinOp::shape_op0);
  }
  template<X86IntrinBinOp::Op input>
  constexpr std::pair<unsigned, unsigned> return_shape_op1()
  {
	return std::get<input>(X86IntrinBinOp::shape_op1);
  }
  template<X86IntrinBinOp::Op input>
  constexpr std::pair<unsigned, unsigned> return_shape_ret()
  {
	return std::get<input>(X86IntrinBinOp::shape_ret);
  }
  template<X86IntrinBinOp::Op input>
  constexpr voidFunctionType return_function()
  {
	return std::get<input>(X86IntrinBinOp::func_ret);
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitSizeOp0() {
	constexpr int lanes = return_shape_op0<op>().first;
	constexpr int bitwidth = return_shape_op0<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitSizeOp1() {
	constexpr int lanes = return_shape_op1<op>().first;
	constexpr int bitwidth = return_shape_op1<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitSizeRet() {
	constexpr int lanes = return_shape_ret<op>().first;
	constexpr int bitwidth = return_shape_ret<op>().second;
	constexpr int bitSize = lanes * bitwidth;
	
	static_assert((bitSize == 128) || (bitSize == 256) || (bitSize == 512));
	return bitSize;
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitwidthOp0() {
	constexpr int bitwidth = return_shape_op0<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitwidthOp1() {
	constexpr int bitwidth = return_shape_op1<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
  }
  template<X86IntrinBinOp::Op op>
  constexpr int bitwidthRet() {
	constexpr int bitwidth = return_shape_ret<op>().second;
	
	static_assert((bitwidth == 8) || (bitwidth == 16) || (bitwidth == 32) || (bitwidth == 64));
	return bitwidth;
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
