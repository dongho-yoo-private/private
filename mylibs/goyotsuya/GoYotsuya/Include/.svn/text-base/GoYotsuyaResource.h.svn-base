#pragma once

#include "yi.h"
#include "yiList.h"
#include "yiBitmap.h"
#include "resource_user.h"

// -------------------------------------
#define MAX_RESOURCE_COUNT 1024

// -------------------------------------
enum eResourceType {
	eRT_BITMAP = 0,
	eRT_DIBMP,
	eRT_DRAW_BMP,
	eRT_STRING,
	eRT_MAX_CNT
};



// -------------------------------------
#define GET_STRING(id) ((const char*) cGoYotsuyaResource::GetInstance()->Get(id, eRT_STRING))

// -------------------------------------
#define GET_BITMAP(id) ((cyiDrawObject*) cGoYotsuyaResource::GetInstance()->Get(id, eRT_BITMAP))

// -------------------------------------
#define GET_RESOURCE(id) ((void*) cGoYotsuyaResource::GetInstance()->Get(id, eRT_USER))


// -------------------------------------
#define LOAD_STRING(id) ((const char*) cGoYotsuyaResource::GetInstance()->Load(id, eRT_STRING))

// -------------------------------------
#define LOAD_BITMAP(id, type) ((cyiBitmap*) cGoYotsuyaResource::GetInstance()->Load(id, eRT_BITMAP, type))

// -------------------------------------
#define LOAD_BITMAP_BY_DIB(id, type) ((Image*) cGoYotsuyaResource::GetInstance()->Load(id, eRT_BITMAP, type, 1))

// -------------------------------------
#define LOAD_BITMAP_CASHED(id) ((Image*) cGoYotsuyaResource::GetInstance()->Load(id, eRT_BITMAP, "", 2))

// -------------------------------------
#define LOAD_RESOURCE(id, type) ((void*) cGoYotsuyaResource::GetInstance()->Load(id, eRT_USER, type))

// -------------------------------------
#define DEFINE_IMAGE_RESOURCE(s, max) cGoYotsuyaResource::GetInstance()->Define(s, max, eRT_BITMAP)

// -------------------------------------
#define DEFINE_STRING_RESOURCE(s, max) cGoYotsuyaResource::GetInstance()->Define(s, max, eRT_STRING)

// -------------------------------------
#define DEFINE_USER_RESOURCE(s, max) cGoYotsuyaResource::GetInstance()->Define(s, max, eRT_USER)

// -------------------------------------
class __declspec(dllexport)  cGoYotsuyaResource
{
public:
	cGoYotsuyaResource();
	~cGoYotsuyaResource();

	// -------------------------------------
	static cGoYotsuyaResource* GetInstance();

	// -------------------------------------
	void Define(UINT id, UINT maxcnt, eResourceType type);

	// -------------------------------------
	void* Load(UINT id, eResourceType type, const char* pType=0, UINT nExtraParam=0);

	// -------------------------------------
	Bool RegisterDynamicObject(eDynamicResourceId id, void* p, int size);

	// -------------------------------------
	void* GetDynamicResourceObject(eDynamicResourceId id); 

	// -------------------------------------
	BOOL Add(UINT id, eResourceType type, void* resource, BOOL bRegister=FALSE);

	// -------------------------------------
	void* Get(UINT id, eResourceType type);

	// -------------------------------------
	void* Set(UINT id, eResourceType type, void* pres);

	// -------------------------------------
	BOOL LoadCashedBitmap();

	void* GetCashedBitmap(UINT id, UINT w,UINT  h);

	// -------------------------------------
	void UnLoadCashedBitmap();

private:
	static cGoYotsuyaResource* m_me;

	static HMODULE m_hModule;

	// -------------------------------------
	void* m_d_resources[eDRID_DRESOUCE_MAX];
	void** m_resources[eRT_MAX_CNT];
	UINT  m_resStartIndex[eRT_MAX_CNT];

	friend BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 );

};