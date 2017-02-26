#include "yiEncoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_MASK 0x3C3C3C3C
#define ENCODE_MASK2  0xC3C3C3C3

#define __YOTSUYA_RANDOM_NUMBER__ 0xe7350ac1
#define __STD_MASK__ 0x3C2D4B5A

// ---------------------------------------------------
cyiEncoder::cyiEncoder(int nRandNo, int mask, int nPacked, eEncoderType type)
{
	m_type = type;
	m_nRandNo = nRandNo;
	m_nPacked = nPacked;

	if (mask==0)
	{
		m_nMask=ENCODE_MASK;
	}
	else
	{
		m_nMask=mask;
	}

	m_nReverseMask=~m_nMask;
}

// ---------------------------------------------------
cyiEncoder::~cyiEncoder()
{
}

//#define NOENCODING 1

// ---------------------------------------------------
void* cyiEncoder::Encode(void* out_buffer, const void* in_buffer, int in_buffersize)
{
//#ifndef NOENCODING
	int n = in_buffersize>>2;/*32bit*/
	char* tempBuffer = (char*)out_buffer;

	if (m_nPacked!=0)
	{
		tempBuffer = (char*)malloc(in_buffersize<<1);
	}

	int* pBuffer = (int*)tempBuffer ;
	int* pBuffer2 = &pBuffer[n];
	int* inBuffer = (int*)in_buffer;

	for (int i=0; i<n; i++)
	{
		//pBuffer[i] = (pBuffer[i]>>16) | (pBuffer[i]<<16);
		pBuffer[i] = (inBuffer[i]&m_nMask) | (m_nRandNo&m_nReverseMask);
		pBuffer2[i] = (inBuffer[i]&m_nReverseMask) | (m_nRandNo&m_nMask);
	}

	if (m_nPacked!=0)
	{
		void* p = cyiEncoder::pack(out_buffer, pBuffer, m_nPacked, in_buffersize<<1);
		free(tempBuffer);

		if (p==0)
		{
			return 0;
		}
	}

	return out_buffer;
//#endif
}

// ---------------------------------------------------
void* cyiEncoder::Decode(void* out_buffer, const void* in_buffer, int in_buffersize)
{
	char* tempBuffer=(char*)in_buffer;
	if (m_nPacked!=0)
	{
		tempBuffer = (char*)malloc(in_buffersize<<1);
		void* p = cyiEncoder::unpack(tempBuffer, in_buffer, m_nPacked, in_buffersize<<1);
		if (p==0)
		{
			return 0;
		}
	}

	int n = in_buffersize>>2;/*32bit*/
	int* pBuffer = (int*)tempBuffer;
	int* pBuffer2 = &pBuffer[n];
	int* pOut = (int*)out_buffer;

	for (int i=0; i<n; i++)
	{
		//pBuffer[i] = (pBuffer[i]>>16) | (pBuffer[i]<<16);
		pOut[i] = ((pBuffer[i]&m_nMask) | (pBuffer2[i]&m_nReverseMask));
	}

	if (m_nPacked!=0)
	{
		free(tempBuffer);
	}

	return out_buffer;
}

// ---------------------------------------------------
void* cyiEncoder::pack(void* out_buffer, const void* in_buffer, int nPacked, int in_buffersize)
{
	if (nPacked==0)
	{
		memcpy(out_buffer, in_buffer, in_buffersize);
		return out_buffer;
	}

	if ( nPacked>=(in_buffersize>>1))
	{
		return NULL;
	}

	char* out = (char*)out_buffer;
	char* in= (char*)in_buffer;
	int i=0, cnt=0, cnt2=in_buffersize>>1;
	int max=in_buffersize>>1;

	memcpy(&out[i], &in[cnt2], nPacked);
	i+=nPacked;
	memcpy(&out[i], &in[cnt], nPacked);

	while(i!=in_buffersize)
	{
		i+=nPacked;
		cnt2+=nPacked;

		int nRemained;
		if ((nRemained = (cnt2+nPacked)-in_buffersize)>0)
		{
				
			memcpy(&out[i], &in[cnt2], nRemained);
			i+=nRemained;
			cnt+=nPacked;
			memcpy(&out[i], &in[cnt], nRemained);
			break;	
		}
		
		memcpy(&out[i], &in[cnt2], nPacked);

		i+=nPacked;
		cnt+=nPacked;
		memcpy(&out[i], &in[cnt], nPacked);
	}

	return out_buffer;
}

// ---------------------------------------------------
void* cyiEncoder::unpack(void* out_buffer, const void* in_buffer, int nPacked, int in_buffersize)
{
	if (nPacked==0)
	{
		memcpy(out_buffer, in_buffer, in_buffersize);
		return out_buffer;
	}

	if(nPacked>=(in_buffersize>>1))
	{
		return NULL;
	}

	char* out = (char*)out_buffer;
	char* in= (char*)in_buffer;
	int i=0, cnt=0, cnt2=in_buffersize>>1;

	memcpy(&out[cnt2], &in[i], nPacked);
	i+=nPacked;
	memcpy(&out[cnt], &in[i], nPacked);

	while(i!=in_buffersize)
	{
		i+=nPacked;
		cnt2+=nPacked;

		int nRemained;
		if ((nRemained = (cnt2+nPacked)-in_buffersize)>0)
		{
			memcpy(&out[cnt2], &in[i], nRemained);
			i+=nRemained;
			cnt+=nPacked;
			memcpy(&out[cnt], &in[i], nRemained);
			break;	
		}

		memcpy(&out[cnt2], &in[i], nPacked);

		i+=nPacked;
		cnt+=nPacked;
		memcpy(&out[cnt], &in[i], nPacked);
	}

	return out_buffer;
}

// ---------------------------------------------------
bool cyiEncoder::securityCode(const char* str, int randNo, int mask, int* s1, int* s2, int* s3, int* s4)
{
	char buffer[256];
	char out_buffer[512];
	int len = strlen(str);

	memset(buffer, 0, 256);
	strncpy(buffer, str, 256);

	cyiEncoder en(randNo, mask);
	en.Encode((void*)out_buffer, (const void*)buffer, 256);

	int* pOut = (int*)out_buffer;
	if (s1)
	{
		*s1 = ((pOut[0] << 2) | (pOut[1] & 0x0000003));
	}

	if (s2)
	{
		*s2 = ((pOut[1] << 2) | (pOut[2] & 0x0000003));
	}

	if (s3)
	{
		*s3 = ((pOut[2] << 2) | (pOut[3] & 0x0000003));
	}
	if (s4)
	{
		*s4 = ((pOut[3] << 2) | (pOut[4] & 0x0000003));
	}

	return true;
}

#define STRING_MASK 0x3C5AA5C3

// ---------------------------------------
char* cyiEncoder::estr(const char* str)
{
	char buffer[MAX_ENCODE_STRING];

	int n = strlen(str);
	n+=8-(7&n);


	if (n>MAX_ENCODE_STRING)
	{
		return NULL;
	}

	int r = (int)rand();
	r=((r&0xFFFFF00F)|(n<<4));

	memcpy(buffer, str, n);

	cyiEncoder en(r, STRING_MASK);

	if (en.Encode((void*)&str[4], (void*)buffer, n)==NULL)
	{
		return NULL;
	}
	
	int* p =  (int*)str;
	*p=r;

	return (char*)str;
}

// ---------------------------------------
char* cyiEncoder::dstr(const char* str)
{
	int* p =  (int*)str;
	int r = *p;	
	int n = ((r&0x00000FF0)>>4);	

	if (n>MAX_ENCODE_STRING)
	{
		return NULL;
	}

	char buffer[512];
	memcpy(buffer, &str[4], n<<1);

	cyiEncoder en(r, STRING_MASK);

	if (en.Decode((void*)str, (void*)buffer, n)==NULL)
	{
		return false;
	}

	return (char*)str;
}


unsigned long cyiEncoder::GetYotsuyaSecurityCode(const char* pszServerName)
{
	int security;
	cyiEncoder::securityCode(pszServerName, __YOTSUYA_RANDOM_NUMBER__ , __STD_MASK__ , &security);//&m_security.nRandNo, &m_security.nMask, &m_security.nSecurity);

	return (unsigned long)security;
}

