#ifndef __yi_hash_table_h__
#define __yi_hash_table_h__
#include "yistd.h"

// ------------------------------
typedef struct {
	char* pszKey;
	void* value;
	yiDestroyer deletef;
} syiHashKeyValue;

// --------------------------
typedef void (*zxOnFoundPath)(const char* pszPath, unsigned int id);

// --------------------------
typedef unsigned int (*zxOnFoundPath2)(const char* pszPath, unsigned int id, const char* pszValue);

// ------------------------------
extern int yixSearchPath(const char* root, unsigned int key, int level, unsigned int max_layer, int shift_bit, unsigned int index_mask, zxOnFoundPath f);

// ------------------------------
extern int yixSearchPath(const char* root, unsigned int key, const char* pszStr, 
				int level, unsigned int max_layer, zxOnFoundPath2 f);

// ------------------------------
class cyiHashTable 
{
public:
	cyiHashTable(int nLayer, int nLayerBitCnt, int nKeyLength, yiDestroyer callback, Bool bUseMutex=False);
	~cyiHashTable();

	// ------------------------------
	static unsigned int GetHashLayer(const char* pszString, int nLayer, int nKeyLength=-1);

	
	// ------------------------------
	unsigned int Add(const char* pszStringKey, void* value);

	// ------------------------------
	unsigned int Add(unsigned int layer, const char* pszStringKey, void* value);

	// ------------------------------
	void Remove(unsigned int key);

	// ------------------------------
	const void* GetValue(unsigned int key, char** pszStringKey=0);

	// ------------------------------
	const void* GetValue(unsigned int key, const char* pszStringKey);

	// ------------------------------
	const void* GetValue(const char* pszString);

	// ------------------------------
	unsigned int GetKey(unsigned int layer, const char* pszString);

	// ------------------------------
	unsigned int GetKey(const char* pszString);

	// ------------------------------
	Bool SetValue(unsigned int id, void* p);

	// ------------------------------
	unsigned int GetUsedMemorySize();

	// ------------------------------
	int GetCount();

	// ------------------------------
	unsigned int GetIndexMask();
private:
	int m_nLayerBitCnt;
	int m_nLayer;
	int m_nTotalUsedMemorySize;
	int m_nKeyLength;
	int m_nElementCount;
	unsigned int m_max_id_mask;
	void* m_pLayerEntryPointx;
	yiDestroyer m_Destroyer;
	Handle m_hMutex;

};

#endif

