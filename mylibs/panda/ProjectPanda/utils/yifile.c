#include "yifile.h"
#include "yistd.h"

int __buffer_size_once = __BUFFERSIZE_PER_ONCE__;

#ifdef __WIN32__
#include "yifile.win32.c"
#else
#include "yifile.posix.c"
#endif

// --------------------------------------------------------------------
void yiSetBuffer(int n)
{
	__buffer_size_once = n;
}


