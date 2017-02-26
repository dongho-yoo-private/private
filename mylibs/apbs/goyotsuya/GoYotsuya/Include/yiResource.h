#pragma once

#include "yi.h"
#include "yiStd.h"


enum eResourceTypex
{
	eRTX_IMAGE=0,
	eRTX_IMAGE_CACHED,
	eRTX_BIN,
	eRTX_TEMPORARY,
	eRTX_CNT
};

// ------------------------------------------------------
class xEXPORT cyiResource
{
public:
	cyiResource(void);
	~cyiResource(void);

	// --------------------------------
	static cyiResource* GetInstance(eResourceTypex type);

	// --------------------------------
	static void SetInstance(eResourceTypex type, cyiResource* p);


	// --------------------------------
	Bool Load(const char* pszFileName);

	// --------------------------------
	Bool Register(int id, void* p, int size);

	// --------------------------------
	Bool Register(int id, int subKey, void* p, int size);

	// --------------------------------
	void* Get(int id, void* p=0, int* size=0);

	// --------------------------------
	void* Get(int id, int subKey, void* p=0, int* size=0);


private:
	cyiList m_list;
	char    szFileName[256];
	eResourceTypex m_type;

	static cyiResource* m_me[eRTX_CNT];

	HandleFileStream m_hResourceStream;

};
