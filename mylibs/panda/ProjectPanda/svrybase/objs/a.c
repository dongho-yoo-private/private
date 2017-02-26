#include <stdio.h>
#include <stdarg.h>

#define SYSLOG(a, format, ...) printf("%s:(%d) "format, __FILE__, __LINE__, __VA_ARGS__) 
#define PRINTF(...) printf(__VA_ARGS__)
main()
{
	SYSLOG(10, "alsdfalsqwlkfjladfk %d %d\n", 10, 20);
	PRINTF("test \n");
}

