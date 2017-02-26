#pragma once

#include "yi.h"

// -------------------------------
enum eResourceType {
	eRT_DRAW_OBJECT=0,
	eRT_TEXT,
	eRT_POS,
	eRT_BINARY,



	eRT_MAX_RESOURCE_TYPE
};

// -------------------------------
enum eResourceSystem {
	eRS_LIST,
	eRS_ARRAY
};


#define AddDrawObject(id, data) Add(eRT_DRAW_OBJECT, id, data)
#define AddText(id, data) Add(eRT_TEXT, id, data)
#define AddBinary(id, data) Add(eRT_BINARY, id, data)

#define GetDrawObject(id) Get(eRT_DRAW_OBJECT, id)
#define GetText(id) Get(eRT_TEXT, id)
#define GetBinary(id) Get(eRT_BINARY, id)

#define SetDrawObject(id) Set(eRT_DRAW_OBJECT, id)
#define SetText(id) Set(eRT_TEXT, id)
#define SetBinary(id) Set(eRT_BINARY, id)



// -------------------------------
class yiResourceManager
{
public:
	yiResourceManager(void);
	~yiResourceManager(void);

	// ----------------------------------------
	static yiResourceManager* GetInstance();

	static char* LoadString(HINSTANCE hResourceModule, UINT id);

	// ----------------------------------------
	BOOL DefineResource(eResourceType type, eResourceSystem system, UINT nMinimumId, UINT nMaxCnt);

	// ----------------------------------------
	BOOL Add(eResourceType type, UINT resId, void* p);

	// ----------------------------------------
	void* Get(eResourceType type, UINT resId);

	// ----------------------------------------
	void* Set(eResourceType type, UINT resId, void* data);


private:
	static yiResourceManager* m_me;

	BOOL m_bIsObjectList[eRT_MAX_RESOURCE_TYPE];
	//cyiList* m_list[eRT_MAX_RESOURCE_TYPE];
	void**	 m_array[eRT_MAX_RESOURCE_TYPE];
	UINT m_range[eRT_MAX_RESOURCE_TYPE];

};
