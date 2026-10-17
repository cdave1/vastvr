#ifndef _VAST_COMMON_COMPAT_H_
#define _VAST_COMMON_COMPAT_H_

#define _CRT_SECURE_NO_WARNINGS 1

#include <errno.h>
#include <io.h>
#include <stdio.h>

///
/// Prior to visual studio 2015, snprintf and vsnprintf were undefined.
///
#if defined(_MSC_VER) && _MSC_VER <= 1900

#ifndef EOVERFLOW
#define EOVERFLOW EFBIG
#endif

#define open _open
#define read _read
#define close _close

#define strdup _strdup
#define strncpy strncpy_s

#define sscanf sscanf_s
#define snprintf _snprintf
#define vsnprintf _vsnprintf

inline int vast_vsnprintf(char *s, size_t n, const char *fmt, va_list ap) {
	int ret;
	va_list ap_copy;

	if (n == 0) {
		return _vscprintf(fmt, ap);
	} else if (n > INT_MAX) {
		return EOVERFLOW;
	}

	memset(s, 0, n);
	va_copy(ap_copy, ap);
	ret = _vsnprintf_s(s, n - 1, n, fmt, ap_copy);
	va_end(ap_copy);
	if (ret == -1) {
		ret = _vscprintf(fmt, ap);
	}

	return ret;
}


inline int vast_snprintf(char *s, size_t n, const char *fmt, ...) {
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vast_vsnprintf(s, n, fmt, ap);
	va_end(ap);

	return ret;
}

#endif

#endif