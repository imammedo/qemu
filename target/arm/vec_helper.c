/*
 * ARM AdvSIMD / SVE Vector Operations
 *
 * Copyright (c) 2018 Linaro
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/helper-proto.h"
#include "tcg/tcg-gvec-desc.h"
#include "fpu/softfloat.h"


/* Note that vector data is stored in host-endian 64-bit chunks,
   so addressing units smaller than that needs a host-endian fixup.  */
#ifdef HOST_WORDS_BIGENDIAN
#define H1(x)  ((x) ^ 7)
#define H2(x)  ((x) ^ 3)
#define H4(x)  ((x) ^ 1)
#else
#define H1(x)  (x)
#define H2(x)  (x)
#define H4(x)  (x)
#endif

#define SET_QC() env->vfp.xregs[ARM_VFP_FPSCR] |= CPSR_Q

static void clear_tail(void *vd, uintptr_t opr_sz, uintptr_t max_sz)
{
    uint64_t *d = vd + opr_sz;
    uintptr_t i;

    for (i = opr_sz; i < max_sz; i += 8) {
        *d++ = 0;
    }
}

/* Signed saturating rounding doubling multiply-accumulate high half, 16-bit */
static uint16_t inl_qrdmlah_s16(CPUARMState *env, int16_t src1,
                                int16_t src2, int16_t src3)
{
    /* Simplify:
     * = ((a3 << 16) + ((e1 * e2) << 1) + (1 << 15)) >> 16
     * = ((a3 << 15) + (e1 * e2) + (1 << 14)) >> 15
     */
    int32_t ret = (int32_t)src1 * src2;
    ret = ((int32_t)src3 << 15) + ret + (1 << 14);
    ret >>= 15;
    if (ret != (int16_t)ret) {
        SET_QC();
        ret = (ret < 0 ? -0x8000 : 0x7fff);
    }
    return ret;
}

uint32_t HELPER(neon_qrdmlah_s16)(CPUARMState *env, uint32_t src1,
                                  uint32_t src2, uint32_t src3)
{
    uint16_t e1 = inl_qrdmlah_s16(env, src1, src2, src3);
    uint16_t e2 = inl_qrdmlah_s16(env, src1 >> 16, src2 >> 16, src3 >> 16);
    return deposit32(e1, 16, 16, e2);
}

void HELPER(gvec_qrdmlah_s16)(void *vd, void *vn, void *vm,
                              void *ve, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    int16_t *d = vd;
    int16_t *n = vn;
    int16_t *m = vm;
    CPUARMState *env = ve;
    uintptr_t i;

    for (i = 0; i < opr_sz / 2; ++i) {
        d[i] = inl_qrdmlah_s16(env, n[i], m[i], d[i]);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

/* Signed saturating rounding doubling multiply-subtract high half, 16-bit */
static uint16_t inl_qrdmlsh_s16(CPUARMState *env, int16_t src1,
                                int16_t src2, int16_t src3)
{
    /* Similarly, using subtraction:
     * = ((a3 << 16) - ((e1 * e2) << 1) + (1 << 15)) >> 16
     * = ((a3 << 15) - (e1 * e2) + (1 << 14)) >> 15
     */
    int32_t ret = (int32_t)src1 * src2;
    ret = ((int32_t)src3 << 15) - ret + (1 << 14);
    ret >>= 15;
    if (ret != (int16_t)ret) {
        SET_QC();
        ret = (ret < 0 ? -0x8000 : 0x7fff);
    }
    return ret;
}

uint32_t HELPER(neon_qrdmlsh_s16)(CPUARMState *env, uint32_t src1,
                                  uint32_t src2, uint32_t src3)
{
    uint16_t e1 = inl_qrdmlsh_s16(env, src1, src2, src3);
    uint16_t e2 = inl_qrdmlsh_s16(env, src1 >> 16, src2 >> 16, src3 >> 16);
    return deposit32(e1, 16, 16, e2);
}

void HELPER(gvec_qrdmlsh_s16)(void *vd, void *vn, void *vm,
                              void *ve, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    int16_t *d = vd;
    int16_t *n = vn;
    int16_t *m = vm;
    CPUARMState *env = ve;
    uintptr_t i;

    for (i = 0; i < opr_sz / 2; ++i) {
        d[i] = inl_qrdmlsh_s16(env, n[i], m[i], d[i]);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

/* Signed saturating rounding doubling multiply-accumulate high half, 32-bit */
uint32_t HELPER(neon_qrdmlah_s32)(CPUARMState *env, int32_t src1,
                                  int32_t src2, int32_t src3)
{
    /* Simplify similarly to int_qrdmlah_s16 above.  */
    int64_t ret = (int64_t)src1 * src2;
    ret = ((int64_t)src3 << 31) + ret + (1 << 30);
    ret >>= 31;
    if (ret != (int32_t)ret) {
        SET_QC();
        ret = (ret < 0 ? INT32_MIN : INT32_MAX);
    }
    return ret;
}

void HELPER(gvec_qrdmlah_s32)(void *vd, void *vn, void *vm,
                              void *ve, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    int32_t *d = vd;
    int32_t *n = vn;
    int32_t *m = vm;
    CPUARMState *env = ve;
    uintptr_t i;

    for (i = 0; i < opr_sz / 4; ++i) {
        d[i] = helper_neon_qrdmlah_s32(env, n[i], m[i], d[i]);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

/* Signed saturating rounding doubling multiply-subtract high half, 32-bit */
uint32_t HELPER(neon_qrdmlsh_s32)(CPUARMState *env, int32_t src1,
                                  int32_t src2, int32_t src3)
{
    /* Simplify similarly to int_qrdmlsh_s16 above.  */
    int64_t ret = (int64_t)src1 * src2;
    ret = ((int64_t)src3 << 31) - ret + (1 << 30);
    ret >>= 31;
    if (ret != (int32_t)ret) {
        SET_QC();
        ret = (ret < 0 ? INT32_MIN : INT32_MAX);
    }
    return ret;
}

void HELPER(gvec_qrdmlsh_s32)(void *vd, void *vn, void *vm,
                              void *ve, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    int32_t *d = vd;
    int32_t *n = vn;
    int32_t *m = vm;
    CPUARMState *env = ve;
    uintptr_t i;

    for (i = 0; i < opr_sz / 4; ++i) {
        d[i] = helper_neon_qrdmlsh_s32(env, n[i], m[i], d[i]);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcaddh)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float16 *d = vd;
    float16 *n = vn;
    float16 *m = vm;
    float_status *fpst = vfpst;
    uint32_t neg_real = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = neg_real ^ 1;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 15;
    neg_imag <<= 15;

    for (i = 0; i < opr_sz / 2; i += 2) {
        float16 e0 = n[H2(i)];
        float16 e1 = m[H2(i + 1)] ^ neg_imag;
        float16 e2 = n[H2(i + 1)];
        float16 e3 = m[H2(i)] ^ neg_real;

        d[H2(i)] = float16_add(e0, e1, fpst);
        d[H2(i + 1)] = float16_add(e2, e3, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcadds)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float32 *d = vd;
    float32 *n = vn;
    float32 *m = vm;
    float_status *fpst = vfpst;
    uint32_t neg_real = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = neg_real ^ 1;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 31;
    neg_imag <<= 31;

    for (i = 0; i < opr_sz / 4; i += 2) {
        float32 e0 = n[H4(i)];
        float32 e1 = m[H4(i + 1)] ^ neg_imag;
        float32 e2 = n[H4(i + 1)];
        float32 e3 = m[H4(i)] ^ neg_real;

        d[H4(i)] = float32_add(e0, e1, fpst);
        d[H4(i + 1)] = float32_add(e2, e3, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcaddd)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float64 *d = vd;
    float64 *n = vn;
    float64 *m = vm;
    float_status *fpst = vfpst;
    uint64_t neg_real = extract64(desc, SIMD_DATA_SHIFT, 1);
    uint64_t neg_imag = neg_real ^ 1;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 63;
    neg_imag <<= 63;

    for (i = 0; i < opr_sz / 8; i += 2) {
        float64 e0 = n[i];
        float64 e1 = m[i + 1] ^ neg_imag;
        float64 e2 = n[i + 1];
        float64 e3 = m[i] ^ neg_real;

        d[i] = float64_add(e0, e1, fpst);
        d[i + 1] = float64_add(e2, e3, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcmlah)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float16 *d = vd;
    float16 *n = vn;
    float16 *m = vm;
    float_status *fpst = vfpst;
    intptr_t flip = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = extract32(desc, SIMD_DATA_SHIFT + 1, 1);
    uint32_t neg_real = flip ^ neg_imag;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 15;
    neg_imag <<= 15;

    for (i = 0; i < opr_sz / 2; i += 2) {
        float16 e2 = n[H2(i + flip)];
        float16 e1 = m[H2(i + flip)] ^ neg_real;
        float16 e4 = e2;
        float16 e3 = m[H2(i + 1 - flip)] ^ neg_imag;

        d[H2(i)] = float16_muladd(e2, e1, d[H2(i)], 0, fpst);
        d[H2(i + 1)] = float16_muladd(e4, e3, d[H2(i + 1)], 0, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcmlah_idx)(void *vd, void *vn, void *vm,
                             void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float16 *d = vd;
    float16 *n = vn;
    float16 *m = vm;
    float_status *fpst = vfpst;
    intptr_t flip = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = extract32(desc, SIMD_DATA_SHIFT + 1, 1);
    uint32_t neg_real = flip ^ neg_imag;
    uintptr_t i;
    float16 e1 = m[H2(flip)];
    float16 e3 = m[H2(1 - flip)];

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 15;
    neg_imag <<= 15;
    e1 ^= neg_real;
    e3 ^= neg_imag;

    for (i = 0; i < opr_sz / 2; i += 2) {
        float16 e2 = n[H2(i + flip)];
        float16 e4 = e2;

        d[H2(i)] = float16_muladd(e2, e1, d[H2(i)], 0, fpst);
        d[H2(i + 1)] = float16_muladd(e4, e3, d[H2(i + 1)], 0, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcmlas)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float32 *d = vd;
    float32 *n = vn;
    float32 *m = vm;
    float_status *fpst = vfpst;
    intptr_t flip = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = extract32(desc, SIMD_DATA_SHIFT + 1, 1);
    uint32_t neg_real = flip ^ neg_imag;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 31;
    neg_imag <<= 31;

    for (i = 0; i < opr_sz / 4; i += 2) {
        float32 e2 = n[H4(i + flip)];
        float32 e1 = m[H4(i + flip)] ^ neg_real;
        float32 e4 = e2;
        float32 e3 = m[H4(i + 1 - flip)] ^ neg_imag;

        d[H4(i)] = float32_muladd(e2, e1, d[H4(i)], 0, fpst);
        d[H4(i + 1)] = float32_muladd(e4, e3, d[H4(i + 1)], 0, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcmlas_idx)(void *vd, void *vn, void *vm,
                             void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float32 *d = vd;
    float32 *n = vn;
    float32 *m = vm;
    float_status *fpst = vfpst;
    intptr_t flip = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint32_t neg_imag = extract32(desc, SIMD_DATA_SHIFT + 1, 1);
    uint32_t neg_real = flip ^ neg_imag;
    uintptr_t i;
    float32 e1 = m[H4(flip)];
    float32 e3 = m[H4(1 - flip)];

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 31;
    neg_imag <<= 31;
    e1 ^= neg_real;
    e3 ^= neg_imag;

    for (i = 0; i < opr_sz / 4; i += 2) {
        float32 e2 = n[H4(i + flip)];
        float32 e4 = e2;

        d[H4(i)] = float32_muladd(e2, e1, d[H4(i)], 0, fpst);
        d[H4(i + 1)] = float32_muladd(e4, e3, d[H4(i + 1)], 0, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

void HELPER(gvec_fcmlad)(void *vd, void *vn, void *vm,
                         void *vfpst, uint32_t desc)
{
    uintptr_t opr_sz = simd_oprsz(desc);
    float64 *d = vd;
    float64 *n = vn;
    float64 *m = vm;
    float_status *fpst = vfpst;
    intptr_t flip = extract32(desc, SIMD_DATA_SHIFT, 1);
    uint64_t neg_imag = extract32(desc, SIMD_DATA_SHIFT + 1, 1);
    uint64_t neg_real = flip ^ neg_imag;
    uintptr_t i;

    /* Shift boolean to the sign bit so we can xor to negate.  */
    neg_real <<= 63;
    neg_imag <<= 63;

    for (i = 0; i < opr_sz / 8; i += 2) {
        float64 e2 = n[i + flip];
        float64 e1 = m[i + flip] ^ neg_real;
        float64 e4 = e2;
        float64 e3 = m[i + 1 - flip] ^ neg_imag;

        d[i] = float64_muladd(e2, e1, d[i], 0, fpst);
        d[i + 1] = float64_muladd(e4, e3, d[i + 1], 0, fpst);
    }
    clear_tail(d, opr_sz, simd_maxsz(desc));
}

/* Floating-point trigonometric starting value.
 * See the ARM ARM pseudocode function FPTrigSMul.
 */
static float16 float16_ftsmul(float16 op1, uint16_t op2, float_status *stat)
{
    float16 result = float16_mul(op1, op1, stat);
    if (!float16_is_any_nan(result)) {
        result = float16_set_sign(result, op2 & 1);
    }
    return result;
}

static float32 float32_ftsmul(float32 op1, uint32_t op2, float_status *stat)
{
    float32 result = float32_mul(op1, op1, stat);
    if (!float32_is_any_nan(result)) {
        result = float32_set_sign(result, op2 & 1);
    }
    return result;
}

static float64 float64_ftsmul(float64 op1, uint64_t op2, float_status *stat)
{
    float64 result = float64_mul(op1, op1, stat);
    if (!float64_is_any_nan(result)) {
        result = float64_set_sign(result, op2 & 1);
    }
    return result;
}

#define DO_3OP(NAME, FUNC, TYPE) \
void HELPER(NAME)(void *vd, void *vn, void *vm, void *stat, uint32_t desc) \
{                                                                          \
    intptr_t i, oprsz = simd_oprsz(desc);                                  \
    TYPE *d = vd, *n = vn, *m = vm;                                        \
    for (i = 0; i < oprsz / sizeof(TYPE); i++) {                           \
        d[i] = FUNC(n[i], m[i], stat);                                     \
    }                                                                      \
}

DO_3OP(gvec_fadd_h, float16_add, float16)
DO_3OP(gvec_fadd_s, float32_add, float32)
DO_3OP(gvec_fadd_d, float64_add, float64)

DO_3OP(gvec_fsub_h, float16_sub, float16)
DO_3OP(gvec_fsub_s, float32_sub, float32)
DO_3OP(gvec_fsub_d, float64_sub, float64)

DO_3OP(gvec_fmul_h, float16_mul, float16)
DO_3OP(gvec_fmul_s, float32_mul, float32)
DO_3OP(gvec_fmul_d, float64_mul, float64)

DO_3OP(gvec_ftsmul_h, float16_ftsmul, float16)
DO_3OP(gvec_ftsmul_s, float32_ftsmul, float32)
DO_3OP(gvec_ftsmul_d, float64_ftsmul, float64)

#ifdef TARGET_AARCH64

DO_3OP(gvec_recps_h, helper_recpsf_f16, float16)
DO_3OP(gvec_recps_s, helper_recpsf_f32, float32)
DO_3OP(gvec_recps_d, helper_recpsf_f64, float64)

DO_3OP(gvec_rsqrts_h, helper_rsqrtsf_f16, float16)
DO_3OP(gvec_rsqrts_s, helper_rsqrtsf_f32, float32)
DO_3OP(gvec_rsqrts_d, helper_rsqrtsf_f64, float64)

#endif
#undef DO_3OP
