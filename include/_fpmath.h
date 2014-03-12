// Adopted from lib/libc/arm/_fpmath.h rev 255361

#define	_IEEE_WORD_ORDER	_BYTE_ORDER

union IEEEl2bits {
	long double	e;
	struct {
#if _BYTE_ORDER == _LITTLE_ENDIAN
#if _IEEE_WORD_ORDER == _LITTLE_ENDIAN
		unsigned int	manl	:32;
#endif
		unsigned int	manh	:20;
		unsigned int	exp	:11;
		unsigned int	sign	:1;
#if _IEEE_WORD_ORDER == _BIG_ENDIAN
		unsigned int	manl	:32;
#endif
#else	/* _BYTE_ORDER == _LITTLE_ENDIAN */
		unsigned int		sign	:1;
		unsigned int		exp	:11;
		unsigned int		manh	:20;
		unsigned int		manl	:32;
#endif
	} bits;
	struct {
#if _BYTE_ORDER == _LITTLE_ENDIAN
		unsigned long long man	:52;
		unsigned int	expsign	:12;
#else /* _BIG_ENDIAN */
		unsigned int	expsign	:12;
		unsigned long long man	:52;
#endif
	} xbits;
};

#define	LDBL_NBIT	0
#define	LDBL_IMPLICIT_NBIT
#define	mask_nbit_l(u)	((void)0)

#define	LDBL_MANH_SIZE	20
#define	LDBL_MANL_SIZE	32

#define	LDBL_TO_ARRAY32(u, a) do {			\
	(a)[0] = (uint32_t)(u).bits.manl;		\
	(a)[1] = (uint32_t)(u).bits.manh;		\
} while(0)
