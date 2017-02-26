#include "yi.h"
#include "yiHashFunc.h"
#include "yiEncoder.h"

#include <string.h>
#include <stdio.h>

#define HASH_FUCN_RAND_NO 0xe733091a

// ---------------------------------------
unsigned int yihf_general(const char* p, int max_shift)
{
	int n = strlen(p);
	int i=0;
	unsigned int sum=0;

	for (i=0; i<n; i++)
	{
		sum+=(unsigned int)p[i]*71;
	}

	return __shift_mod__(max_shift, sum);
}

//
unsigned int yihf_additional32(const char* p, int max_shift)
{
}


// ---------------------------------------
unsigned int yihf_additional(const char* p, int max_shift)
{
	int n = strlen(p);
	int i=0;
	unsigned int sum=0;

	for (i=0; i<n; i++)
	{
		sum+=(unsigned int)p[i];
		sum+=(unsigned int)((p[i]<<(__shift_mod__(max_shift, i))));
	}

	return __shift_mod__(max_shift, sum);
}

// ---------------------------------------
unsigned int yihf_sub(const char* p, int max_shift)
{
	int n = strlen(p);
	int i=0;
	unsigned int sum=0;

	for (i=0; i<n; i++)
	{
		sum+=(unsigned int)p[i];
		sum+=(unsigned int)((p[i]>>(__shift_mod__(5, i)+1)));
	}

	return __shift_mod__(max_shift, sum);
}

// ---------------------------------------
unsigned int yihf_encoded_additional(const char* p, int max_shift)
{
	char buffer[64];
	char buffer_encoded[128];
	int n = strlen(p);
	int i=0;
	unsigned int sum=0;

	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, p);

	int ft = (unsigned int)p[n-1]<<16; 

	cyiEncoder en(HASH_FUCN_RAND_NO+ft, 0x3c2D4B5A);
	en.Encode(buffer_encoded, buffer, 64);

	for (i=0; i<128; i++)
	{
		sum+=(unsigned int)buffer_encoded[i];
		sum+=((buffer_encoded[i]<<(__shift_mod__(max_shift, i)+1)));
	}

	return __shift_mod__(max_shift, sum);
}

// ---------------------------------------
unsigned int yihf_shift_additional(const char* p, int max_shift)
{
	char buffer[64];
	char buffer_encoded[128];
	int n = strlen(p);
	int i=0;
	unsigned int sum=0;

	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, p);

	int ft = (unsigned int)p[n-1]<<16; 

	cyiEncoder en(HASH_FUCN_RAND_NO+ft, 0x3c3c3c3c);
	en.Encode(buffer_encoded, buffer, 64);

	for (i=0; i<128; i++)
	{
		sum+=(unsigned int)buffer_encoded[i];
	}

	return __shift_mod__(max_shift, sum);
}


