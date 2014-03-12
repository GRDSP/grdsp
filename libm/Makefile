CC := c6x-none-elf-gcc
CFLAGS := -march=c674x -O2 -Wall

# TODO: not build at this time.
# s_lroundl.c s_lrintf.c s_llround.c e_sqrtl.c s_lrintl.c s_llrintf.c s_llrint.c \
# s_llrintl.c s_llroundf.c s_lroundf.c s_lrint.c s_fma.c s_nearbyint.c s_fmaf.c \
# s_lround.c s_llroundl.c	(requires fenv.h)
#
# NOTE: these files are intentionally not build due to the fact that long double is
#       the same type as double..
# e_acosl.c e_atan2l.c s_atanl.c e_asinl.c	(requires invtrig.h, expected)
# s_rintl.c s_nexttoward.c s_scalbnl.c s_tanl.c s_frexpl.c s_fmal.c s_asinhl.c \
# s_nextafterl.c e_atanhl.c s_sinl.c s_cbrtl.c s_cosl.c e_acoshl.c	(unsupported
# 	long double format)

SRCS := $(foreach x,\
	s_csqrtl.c \
	e_logf.c \
	s_logb.c \
	k_exp.c \
	e_acosf.c \
	e_j1.c \
	s_remquol.c \
	s_log1pf.c \
	s_copysignl.c \
	e_atanhf.c \
	e_fmodf.c \
	e_acoshf.c \
	s_cosf.c \
	e_log2.c \
	e_gammaf.c \
	s_fminf.c \
	e_gamma.c \
	s_cprojl.c \
	s_asinhf.c \
	e_cosh.c \
	s_tanf.c \
	s_ceilf.c \
	s_tgammaf.c \
	e_lgamma.c \
	s_fminl.c \
	e_expf.c \
	s_signgam.c \
	s_csinhf.c \
	e_hypotl.c \
	s_truncf.c \
	s_csqrt.c \
	s_modff.c \
	s_cimagf.c \
	s_truncl.c \
	e_lgammaf_r.c \
	e_remainder.c \
	e_log10.c \
	s_scalbnf.c \
	s_cimag.c \
	s_remquo.c \
	s_frexpf.c \
	s_cargf.c \
	s_ccoshf.c \
	e_powf.c \
	s_exp2.c \
	s_cexpf.c \
	k_sinf.c \
	catrig.c \
	s_ilogb.c \
	s_ccosh.c \
	s_finitef.c \
	e_coshf.c \
	e_j0f.c \
	e_j1f.c \
	s_log1p.c \
	s_significand.c \
	w_cabs.c \
	s_erff.c \
	s_conjf.c \
	s_fmaxf.c \
	e_hypotf.c \
	s_nan.c \
	e_asinf.c \
	s_roundl.c \
	e_sqrtf.c \
	w_drem.c \
	s_isnormal.c \
	k_cos.c \
	s_expm1.c \
	s_ilogbl.c \
	w_cabsl.c \
	s_crealf.c \
	s_exp2f.c \
	s_frexp.c \
	s_carg.c \
	s_rintf.c \
	s_ilogbf.c \
	s_scalbln.c \
	e_scalbf.c \
	e_log10f.c \
	s_floorl.c \
	w_cabsf.c \
	s_cprojf.c \
	s_fdim.c \
	e_jnf.c \
	s_sinf.c \
	s_cproj.c \
	s_logbl.c \
	s_fmaxl.c \
	s_tanhf.c \
	s_atan.c \
	s_csqrtf.c \
	s_trunc.c \
	s_modf.c \
	k_rem_pio2.c \
	s_nextafter.c \
	s_scalbn.c \
	s_modfl.c \
	e_log2f.c \
	e_acosh.c \
	s_fmax.c \
	s_copysign.c \
	s_fmin.c \
	imprecise.c \
	k_tan.c \
	e_log.c \
	s_cbrtf.c \
	s_nexttowardf.c \
	e_fmodl.c \
	e_atan2f.c \
	s_cexp.c \
	catrigf.c \
	e_rem_pio2.c \
	s_cbrt.c \
	s_finite.c \
	s_floor.c \
	s_rint.c \
	s_erf.c \
	s_creall.c \
	e_atanh.c \
	s_sin.c \
	s_creal.c \
	s_ctanh.c \
	e_acos.c \
	s_ctanhf.c \
	s_expm1f.c \
	s_cos.c \
	e_rem_pio2f.c \
	s_fabsl.c \
	s_conj.c \
	w_dremf.c \
	s_asinh.c \
	e_hypot.c \
	s_signbit.c \
	s_fabsf.c \
	e_sinh.c \
	s_logbf.c \
	s_ceill.c \
	k_tanf.c \
	s_nextafterf.c \
	s_round.c \
	k_expf.c \
	e_scalb.c \
	e_lgamma_r.c \
	s_csinh.c \
	e_gamma_r.c \
	s_significandf.c \
	s_atanf.c \
	e_jn.c \
	e_atan2.c \
	s_tan.c \
	e_remainderf.c \
	s_roundf.c \
	s_tanh.c \
	k_sin.c \
	e_sqrt.c \
	s_isnan.c \
	s_conjl.c \
	e_remainderl.c \
	e_j0.c \
	e_pow.c \
	s_copysignf.c \
	s_isfinite.c \
	s_floorf.c \
	e_sinhf.c \
	s_remquof.c \
	s_ceil.c \
	s_fabs.c \
	k_cosf.c \
	e_asin.c \
	e_lgammaf.c \
	e_fmod.c \
	s_cargl.c \
	s_cimagl.c \
	e_gammaf_r.c \
	e_exp.c, $(addprefix src/,$x)) \
	./bsdsrc/b_exp.c \
	./bsdsrc/b_tgamma.c \
	./bsdsrc/b_log.c \
	./compat/ldexp.c \
	./compat/isinf.c \

CFLAGS += -I../include -Isrc -Ibsdsrc
OBJS := $(subst .c,.o,$(SRCS))

libm.a: $(OBJS)
	ar cr $@ $^
	ranlib $@

clean:
	rm -f libm.a $(OBJS)
