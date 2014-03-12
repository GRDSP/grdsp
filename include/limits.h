#ifndef	_MACHINE__LIMITS_H_
#define	_MACHINE__LIMITS_H_

#include_next <limits.h>

#define	__CHAR_BIT	CHAR_BIT	/* number of bits in a char */

#define	__SCHAR_MAX	SCHAR_MAX	/* max value for a signed char */
#define	__SCHAR_MIN	SCHAR_MIN	/* min value for a signed char */

#define	__UCHAR_MAX	UCHAR_MAX	/* max value for an unsigned char */

#define	__USHRT_MAX	USHRT_MAX	/* max value for an unsigned short */
#define	__SHRT_MAX	SHRT_MAX	/* max value for a short */
#define	__SHRT_MIN	SHRT_MIN	/* min value for a short */

#define	__UINT_MAX	UINT_MAX	/* max value for an unsigned int */
#define	__INT_MAX	INT_MAX		/* max value for an int */
#define	__INT_MIN	INT_MIN		/* min value for an int */

#define	__ULONG_MAX	ULONG_MAX	/* max for an unsigned long */
#define	__LONG_MAX	LONG_MAX	/* max for a long */
#define	__LONG_MIN	LONG_MIN	/* min for a long */

			/* max value for an unsigned long long */
#define	__ULLONG_MAX	ULLONG_MAX
#define	__LLONG_MAX	LLONG_MAX	/* max value for a long long */
#define	__LLONG_MIN	LLONG_MIN	/* min for a long long */

#endif
