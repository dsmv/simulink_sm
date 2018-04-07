
#include "exceptinfo.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <getopt.h>
#endif

#include <fcntl.h>
#include <signal.h>


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

except_info_t except_info(const BRDCHAR *fmt, ...)
{
    except_info_t err;
    va_list argptr;
    va_start(argptr, fmt);
    
	BRDC_vsprintf( err.msg, fmt, argptr);
    
    return err;
}

//-----------------------------------------------------------------------------
