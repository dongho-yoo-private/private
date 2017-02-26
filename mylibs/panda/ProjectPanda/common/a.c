

// ---------------------------
typedef struct xsycFileHeader {
	int prefix; // "YOTS":
	int size;
	unsigned short check_sum_valid; // 0:not use, n:interval
	unsigned short flag16; // 16: is original, 15: is shadow, 14: is backup 13:size is fixed 12: fixed record ... reserved
	int check_sum; // if check_sum_valid is zero, it is record size. if record size zero, no checksum.
}sycFileHeader;

#include <stdio.h>

#define offsetof(st, member) &(((st*)0)->member)

main()
{
	char* p = "YOTS";
	char buffer[5];
	memset(buffer, 0, 5);
	long* pLong = (long*)p;

	printf("0x%x\n", *pLong);

	*((long*)buffer)=*pLong;
	printf("%s\n", buffer);

	printf("size offset: %d\n", offsetof(sycFileHeader, size));	
	printf("flag offset: %d\n", offsetof(sycFileHeader, flag16));	
}

