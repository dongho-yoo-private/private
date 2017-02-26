#ifndef __ysEncoder_h__
#define __ysEncoder_h__

#define MAX_ENCODE_STRING (255-4)

// -------------------------------
enum eEncoderType 
{
	ET_TYPE_NORMAL,
	ET_TYPE_NO_RECOVERY
};

// -------------------------------
class cyiEncoder
{
public:
	cyiEncoder(int nRandNo, int mask=0, int nPacked=0, eEncoderType type=ET_TYPE_NORMAL);
	~cyiEncoder();

	// retval: zero is success.
	void* Encode(void* out_buffer, const void* in_buffer, int in_buffersize);

	// retval: zero is success.
	void* Decode(void* out_buffer, const void* in_buffer, int in_buffersize);

	// 
	static void* pack(void* out_buffer, const void* in_buffer, int nPacked, int in_buffersize);

	// 
	static void* unpack(void* out_buffer, const void* in_buffer, int nPacked, int in_buffersize);

	// 
	static bool securityCode(const char* str, int randNo, int mask, int* s1, int* s2, int* s3=0, int* s4=0);

	// 
	static char* estr(const char* str);//, int randNo, int mask, int* s1, int* s2, int* s3=0, int* s4=0);

	// 
	static char* dstr(const char* str);//, int randNo, int mask, int* s1, int* s2, int* s3=0, int* s4=0);

private:
	eEncoderType m_type;
	int m_nPacked;
	int m_nRandNo;
	int m_nMask;
	int m_nReverseMask;
};


#endif
