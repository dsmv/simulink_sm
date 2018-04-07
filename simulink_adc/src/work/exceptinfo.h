
#ifndef __EXCEPTION_INFO_H__
#define __EXCEPTION_INFO_H__

#include "brd.h"
#include "utypes.h"

typedef  struct _except_info_t {
	BRDCHAR msg[256];
} except_info_t;

except_info_t except_info(const BRDCHAR *fmt, ...);

#endif //__EXCEPTION_INFO_H__

