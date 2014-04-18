/*  Copyright (C) 2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_TYPES_FLOAT32X16_H
#define LIBSIMDPP_SIMDPP_TYPES_FLOAT32X16_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/setup_arch.h>
#include <simdpp/types/fwd.h>
#include <simdpp/types/any.h>
#include <simdpp/types/int32x16.h>
#include <simdpp/detail/construct_eval.h>

namespace simdpp {
#ifndef SIMDPP_DOXYGEN
namespace SIMDPP_ARCH_NAMESPACE {
#endif

#if SIMDPP_USE_AVX512 || SIMDPP_DOXYGEN

/// @defgroup simd_vec_fp
/// @{

/// Class representing float32x16 vector
template<>
class float32<16, void> : public any_float32<16, float32<16,void>> {
public:
    static const unsigned type_tag = SIMDPP_TAG_FLOAT;
    using base_vector_type = float32<16,void>;
    using expr_type = void;

#if SIMDPP_USE_AVX512
    using native_type = __m512;
#endif

    float32<16>() = default;
    float32<16>(const float32<16> &) = default;
    float32<16> &operator=(const float32<16> &) = default;

    template<class E> float32<16>(const float32<16,E>& d) { *this = d.eval(); }
    template<class V> explicit float32<16>(const any_vec<64,V>& d)
    {
        *this = bit_cast<float32<16>>(d.vec().eval());
    }
    template<class V> float32<16>& operator=(const any_vec<64,V>& d)
    {
        *this = bit_cast<float32<16>>(d.vec().eval()); return *this;
    }

    /// @{
    /// Construct from the underlying vector type
    float32<16>(const native_type& d) : d_(d) {}
    float32<16>& operator=(const native_type& d) { d_ = d; return *this; }
    /// @}

    /// Convert to the underlying vector type
    operator native_type() const { return d_; }

#ifndef SIMDPP_DOXYGEN
    template<class E> float32<16>(const expr_vec_construct<E>& e)
    {
        detail::construct_eval_wrapper(*this, e.expr());
    }
    template<class E> float32<16>& operator=(const expr_vec_construct<E>& e)
    {
        detail::construct_eval_wrapper(*this, e.expr()); return *this;
    }
#endif

    /// @{
    /// Access base vectors
    const float32<16>& operator[](unsigned) const { return *this; }
          float32<16>& operator[](unsigned)       { return *this; }
    /// @}

    float32<16> eval() const { return *this; }

    /// Creates a float32x4 vector with the contents set to zero
    static float32<16> zero() { return detail::make_zero(); }

private:
    native_type d_;
};

/// Class representing possibly optimized mask data for 4x 32-bit floating-point
/// vector
template<>
class mask_float32<16, void> : public any_float32<16, mask_float32<16,void>> {
public:
    static const unsigned type_tag = SIMDPP_TAG_MASK_FLOAT;
    using base_vector_type = mask_float32<16,void>;
    using expr_type = void;

#if SIMDPP_USE_AVX512
    using native_type = __mmask16;
#endif

    mask_float32<16>() = default;
    mask_float32<16>(const mask_float32<16> &) = default;
    mask_float32<16> &operator=(const mask_float32<16> &) = default;

    mask_float32<16>(const native_type& d) : d_(d) {}

    template<class E> explicit mask_float32<16>(const mask_int32<16,E>& d)
    {
        *this = bit_cast<mask_float32<16>>(d.eval());
    }
    template<class E> mask_float32<16>& operator=(const mask_int32<16,E>& d)
    {
        *this = bit_cast<mask_float32<16>>(d.eval()); return *this;
    }

    operator native_type() const { return d_; }

    /// Access the corresponding non-mask type
    float32<16> unmask() const
    {
    #if SIMDPP_USE_AVX512
        // FIXME: remove cross-domain access
        __m512i bits = _mm512_maskz_set1_epi32(d_, 0xffffffff);
        // return _mm512_castsi512_ps(bits); GCC BUG
        return (__m512) bits;
    #endif
    }

    const mask_float32<16>& operator[](unsigned) const { return *this; }
          mask_float32<16>& operator[](unsigned)       { return *this; }

    mask_float32<16> eval() const { return *this; }

private:
    native_type d_;
};

/// @} -- end defgroup

#endif // SIMDPP_USE_AVX512 || SIMDPP_DOXYGEN

#ifndef SIMDPP_DOXYGEN
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif