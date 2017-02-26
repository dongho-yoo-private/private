#include "igx_stdlib.h"
#include <stdlib.h>

bool_t igx_is_number(const char* buffer)
{
	bit32_t cnt=0;
	bool_t bFoundComa=false;
	bool_t bIsMinus=false;

	while(*buffer)
	{
		if (cnt==0)
		{
			if (*buffer==' ')
			{
				buffer++;
				continue;
			}
			if (*buffer=='-')
			{
				buffer++;
				cnt++;
				bIsMinus=true;
				continue;
			}
			if (*buffer=='.')
			{
				return false;
			}
		}

		if (*buffer=='.')
		{
			if (bIsMinus==true && cnt==1)
			{
				return false;
			}
			else if (bFoundComa==true)
			{
				return false;
			}

			bFoundComa=true;
			buffer++;
			cnt++;
			continue;
		}

		if (*buffer < '0' ||
				*buffer> '9')
			return false;

		buffer++;
		cnt++;
	}

	return true;

}
bool_t igx_atoi(const char* buffer, number_t* n)
{
	int cnt=0;
	char temp[32];
    const size_t kMaxNumberLen = sizeof(temp);
	char* _buff=(char*)buffer;
	int len=0;

	while(*_buff!=0 && cnt-(kMaxNumberLen-1)!=0)
	{
		if (*_buff==' ')
		{
			if (cnt==0)
			{
				continue;
			}
		}

		if (*_buff>'9' ||
				*_buff<'0')
		{
			if (len!=0)
				return false;

			if (*_buff!='-')
			{
				return false;
			}
		}
		temp[cnt++]=*_buff++;
	}

	temp[cnt]=0;
	*n= atoi(temp);

	return true;
}
bool_t igx_atof(const char* buffer, float* n)
{
	int cnt=0;
	char temp[32];
    const size_t kMaxNumberLen = sizeof(temp);
	char* _buff=(char*)buffer;

    /** floatは何桁まで数えれば良い？*/
	while(*_buff!=0 && cnt-(kMaxNumberLen-1)!=0)
	{
		if (*_buff==' ')
		{
			if (cnt==0)
			{
				continue;
			}
		}

		if (*_buff>'9' ||
				*_buff<'0')
		{
			if (cnt!=0)
			{
				if (*_buff!='.')
				{
					return false;
				}
			}
			else if (*_buff!='-')
			{
				return false;
			}
		}
		temp[cnt++]=*_buff++;
	}

	temp[cnt]=0;
	*n= atof(temp);

	return true;
}
bit32_t igx_itoa(char* buffer, number_t number,  const bit32_t n)
{
    static char* num_ascii = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    bit32_t remain;
    bit32_t place = n;
    bit32_t before=0, len=0, i, half;

    for (;(remain=number%place)!=0;place*=n, ++len)
    {
        bit32_t num = (remain-before);
        buffer[len] = num_ascii[num];
    }
    buffer[len]=0;

    half = (len>>1);
    for (i=0; i<half; ++i)
    {
        char temp = buffer[i];
        buffer[i]=buffer[len-1];
        buffer[len-1]=buffer[i];
    }

    return len;
}

