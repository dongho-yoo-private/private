#include "yiResource.h"
#include "yistd.h"
#include "yifile.h"


typedef struct xsResourceHeader {
	int id;
	int subKey;
	int size;
	int check_sum;
} sResourceHeader;

typedef struct xsRawBitmapHeader {
	int w;
	int h;
} sRawBitmapHeader;


typedef struct xsResourceData {
	int id;
	int subKey;
	int size;
	void* p;
}sResourceData;

#include "yiShape.h"
#include "yiEnv.h"

cyiResource::cyiResource(void)
:m_list(True)
{
	m_hResourceStream=0;
}


// --------------------------------
static void xResourceFree(void* data)
{
	sResourceData* p = (sResourceData*)data;

	Bitmap* bmp = (Bitmap*)p->p;
	delete bmp;

	yifree(p);
}

// --------------------------------
cyiResource::~cyiResource(void)
{

	if (m_hResourceStream)
	{
		yiFileStreamClose(m_hResourceStream);
	}

	if (m_type== eRTX_IMAGE_CACHED ||
			m_type==eRTX_IMAGE )
	{
		m_list.clear(xResourceFree);
	}
	else
	{
		m_list.clear(yifree);
	}
}

// --------------------------------
cyiResource* cyiResource::m_me[eRTX_CNT] = {0, 0, 0};

// --------------------------------
cyiResource* cyiResource::GetInstance(eResourceTypex type)
{
	if (m_me[type]==0)
	{
		m_me[type] = new cyiResource();
		m_me[type]->m_type = type;
	}

	return m_me[type];
}

// --------------------------------
void cyiResource::SetInstance(eResourceTypex type, cyiResource* p)
{
	if (m_me[type]==0)
	{
		m_me[type] = p;
	}
}


// --------------------------------
Bool cyiResource::Load(const char* pszFileName)
{
	int size;
	char* p = (char* )yiFileLoad(pszFileName, &size);

	if (p==0)
	{
		m_hResourceStream = yiFileStreamOpen(pszFileName);
		return False;
	}

	if (m_type== eRTX_IMAGE_CACHED ||
			m_type==eRTX_IMAGE )
	{
		sResourceHeader* pHeader=(sResourceHeader*)p;
		while(((char*)pHeader-p)<size)
		{
			sRawBitmapHeader* data = (sRawBitmapHeader*)&pHeader[1];
			if (pHeader->check_sum!=yiGetCheckSum(data, size, 32))
			{
				return False;
			}

			Bitmap* bmp = cyiShape::BlockToBitmap((color_t*)&data[1], data->w, data->h);
			sResourceData* el = (sResourceData*)yialloc(sizeof(sResourceData));
			el->id=pHeader->id;
			el->id=pHeader->subKey;
			el->size = pHeader->size;
			el->p=bmp;
			m_list.addref(el);

			pHeader=(sResourceHeader*)(&((char*)pHeader)[pHeader->size+sizeof(sResourceHeader)]);
		}
	}
	else
	{
		sResourceHeader* pHeader=(sResourceHeader*)p;
		while(((char*)pHeader-p)<size)
		{
			void* data = (void*)&pHeader[1];
			if (pHeader->check_sum!=yiGetCheckSum(data, size, 4))
			{
				return False;
			}

			sResourceData* el = (sResourceData*)yialloc(sizeof(sResourceData));
			el->id=pHeader->id;
			el->size = pHeader->size;
			el->subKey = pHeader->subKey;
			el->p=yitoheap(data, pHeader->size);
			m_list.addref(el);

			pHeader=(sResourceHeader*)(&((char*)pHeader)[pHeader->size+sizeof(sResourceHeader)]);
		}
	}

	yiFileUnload(p);

	m_hResourceStream = yiFileStreamOpen(pszFileName);
	return True;
}

// --------------------------------
Bool cyiResource::Register(int id, void* p, int size)
{
	return Register(id, 0, p, size);
}

// --------------------------------
Bool cyiResource::Register(int id, int subKey, void* p, int size)
{
	if (Get(id, subKey)==0)
	{
		if (m_type==eRTX_IMAGE_CACHED ||
				m_type==eRTX_IMAGE)
		{
			Bitmap* bmp = (Bitmap*)p;

			m_list.lock();
			sResourceData* el = (sResourceData*)yialloc(sizeof(sResourceData));
			el->id=id;
			el->subKey=subKey;
			el->size = 0;
			el->p=bmp;
			m_list.addref(el);
			m_list.unlock();

			int w = bmp->GetWidth();
			int h = bmp->GetHeight();
			int   bmpSize = ((w*h)<<2);
			int   bmpAllSize = bmpSize+ sizeof(sRawBitmapHeader);

			sResourceHeader* res = (sResourceHeader*)yialloc(sizeof(sResourceHeader)+bmpAllSize);
			sRawBitmapHeader* header = (sRawBitmapHeader*)&res[1];

			cyiShape::BitmapToBlock(bmp, (color_t*)&header[1]);
			header->w=w;
			header->h=h;
			res->size=bmpSize+sizeof(sRawBitmapHeader);
			res->check_sum = yiGetCheckSum(header, bmpAllSize, 32);
			res->id=id;
			res->subKey=subKey;
			yiFileStreamWrite(m_hResourceStream, res, bmpAllSize+sizeof(sResourceHeader));
			yifree(res);
		}
		else if (m_type==eRTX_TEMPORARY)
		{
			sResourceData* el = (sResourceData*)yialloc(sizeof(sResourceData));
			el->id=id;
			el->subKey=subKey;
			el->size = 0;
			el->p=p;
			m_list.lock();
			m_list.addref(el);
			m_list.unlock();
		}
		else
		{
			return False;
		}

	}

	return True;
}

// --------------------------------
void* cyiResource::Get(int id, void* buffer, int* size)
{
	return Get(id, 0, buffer, size);
}

// --------------------------------
void* cyiResource::Get(int id, int subKey, void* buffer, int* size)
{
	m_list.lock();

	for (cyiNode* p = m_list.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		if (p->ref==0)
		{	
			m_list.unlock();
			return 0;
		}
		sResourceData* data = (sResourceData*)p->ref;

		if (data->id==id &&
			data->subKey==subKey)
		{
			if (buffer)
			{
				memcpy(buffer, data->p, data->size);
				if (size)
					*size=data->size;
				m_list.unlock();
				return buffer;
			}
			else
			{
				m_list.unlock();
				return data->p;
			}
		}
	}

	m_list.unlock();

	return 0;
}