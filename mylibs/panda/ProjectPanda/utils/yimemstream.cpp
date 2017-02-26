#include "yimemstream.h"
#include "yiList.h"

typedef struct xStream {
	cyiList* memlist;
	int nPageSize;
	int nCurrentPos;		
} sxStream;


// ----------------------------
HandleMemStream yimsCreate(Bool bUseMutex, int pagesize)
{
	sxStream x;

	x.memlist = new cyiList(bUseMutex);
	x.nPageSize=pagesize;
	x.nCurrentPos=0;

	x.memlist->addref(yialloc(pagesize));

	return (HandleMemStream)yitoheap(&x, sizeof(x));
}

// ----------------------------
void yimsCreate(HandleMemStream  h)
{
	sxStream*x = (sxStream*)h;
	if (x==0)
		return ;

	if (x->memlist)
	{
		x->memlist->clear(yifree);
		delete x->memlist;
	}

	yifree(x);
}

// ----------------------------
static int xGetListIndex(int pagesize, int pos, int* remain)
{
	*remain=(pagesize - (pos%pagesize));
	return (pos/pagesize);
}

// ----------------------------
Bool yimsWrite(HandleMemStream h, const void* data, int n)
{
	sxStream*x = (sxStream*)h;
	int index;
	int cnt;
	int remain;
	int dataindex=0;
	int remain_size=n;
	char* buffer;

	if (x==0)
		return False;

	index = xGetCurrentListIndex(x->nPageSize, x->nCurrentPos, &remain);	

	x->memlist->lock();

XWRITE:
	if ((cnt=x->memlist->count())>index)
	{
		x->memlist->unlock();
		return False;
	}
	else if (cnt==index)
	{
		x->memlist->addref(yialloc(x->nPageSize));
	}

	{	
		cyiNode* p = x->memlist->GetNodeTopFrom(index);

		if (p==0)
		{
			x->memlist->unlock();
			return False;
		}
		buffer = (char*)p->ref;
	}
	

	{
		int pageindex = x->nCurrentPos%x->nPageSize;
		int copy_size = (remain > n) ? n : remain;

		memcpy(&data[dataindex], &buffer[pageindex], copy_size);
		x->nCurrentPos+=copy_size;
		remain_size-=copy_size;
		dataindex+=copy_size;

		if (remain_size==0)
		{
			goto XWRITE_END;
		}

		pageindex=0;
		index++;
	}

	goto XWRITE;

XWRITE_END:
	x->memlist->unlock();
	return True;
}

// ----------------------------
EXTERN_C Bool yimsRead(HandleMemStream h, void* buffer, int nBufferSize)
{
	sxStream*x = (sxStream*)h;
	int index;
	int cnt;
	int remain;
	int remain_size=n;
	int dataindex=0;
	char* buffer;

	if (x==0)
		return False;

	index = xGetCurrentListIndex(x->nPageSize, x->nCurrentPos, &remain);	

	x->memlist->lock();

XREAD:
	if ((cnt=x->memlist->count())>index)
	{
		x->memlist->unlock();
		return False;
	}
	else if (cnt==index)
	{
		x->memlist->addref(yialloc(x->nPageSize));
	}

	{	
		cyiNode* p = x->memlist->GetNodeTopFrom(index);

		if (p==0)
		{
			x->memlist->unlock();
			return False;
		}
		buffer = (char*)p->ref;
	}
	

	{
		int pageindex = x->nCurrentPos%x->nPageSize;
		int copy_size = (remain > n) ? n : remain;
		memcpy(&data[dataindex], &buffer[pageindex], copy_size);
		x->nCurrentPos+=copy_size;
		remain_size-=copy_size;
		dataindex+=copy_size;

		if (remain_size==0)
		{
			goto XREAD_END;
		}
		pageindex=0;
		index++;
	}

	goto XREAD;

XREAD_END:
	x->memlist->unlock();
	return True;
}

// ----------------------------
unsigned int yimsGetCurrentOffset(HandleMemStream h)
{
}
