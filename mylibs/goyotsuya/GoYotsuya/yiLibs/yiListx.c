#include "yiListx.h"
#include "yimutex.h"

typedef struct {
	syiNode* top;
	syiNode* tail;
	int count;
	HandleMutexx hMutex;
} sListc;

// -----------------------
static syiNode* __getr__(sListc* s, int n)
{
	int i=0;
	syiNode* p = s->tail;

	if (p==0)
		return 0;

	for (i=0; i<n; i++)
	{
		p=p->pp;

		if (p==0)
			return False;	
	}

	return p;
}

// -----------------------
static syiNode* __get__(sListc* s, int n)
{
	int i=0;
	syiNode* p = s->top;

	if (p==0)
		return 0;

	for (i=0; i<n; i++)
	{
		p=p->np;

		if (p==0)
		{
			return 0;	
		}
	}

	return p;
}
// -----------------------
static Bool __create_node__(sListc* s, int n, /*yiAnyValue data, */void* ref)
{
	syiNode* p = (syiNode*)yialloci(sizeof(syiNode));

	if (p==0)
		return False;

	if (s->top==0)
	{
		p->np=0;
		p->pp=0;
		s->top=p;
		s->tail=p;
	}
	else if (n==s->count)
	{
		p->np=0;
		p->pp=s->tail;
		s->tail->np=p;
		s->tail=p;
	}
	else if (n==0)
	{
		p->pp=0;
		p->np=s->top;
		s->top->pp = p;	
		s->top=p;
	}
	else
	{
		syiNode* next = __get__(s, n); 
		syiNode* prev; 

		if (next==0) 
			return False;

		prev = next->pp;

		p->pp = prev;
		p->np = next;
		next->pp = p;
		prev->np = p; 
	}

	p->ref = ref;
	s->count++;

	return True;
}

// -------------------------------
HandleList yiListCreate(Bool bUseMutex, int nSpinCount)
{
	sListc s = {0, 0, 0, 0};
	if (bUseMutex)
		s.hMutex = yiMutexCreateEx(nSpinCount);

	return (HandleList)yitoheap(&s, sizeof(s));
}

// -------------------------------
void yiListDestroy(HandleList hList)
{
	yiListClear(hList);
	yifree(hList);
}

// -------------------------------
int yiListAdd(HandleList hList, IntPtr data)
{
	sListc* s = (sListc*)hList;
	return	__create_node__(s, s->count, (void*)data);
}

// -------------------------------
int yiListInsert(HandleList hList, int index, IntPtr data)
{
	sListc* s = (sListc*)hList;
	return	__create_node__(s, index, (void*)data);
}

// -------------------------------
void yiListRemove(HandleList hList, syiNode* node)
{
	sListc* s = (sListc*)hList;
	//if (node==0)
	//	return ;

	syiNode* prev = node->pp;
	syiNode* next = node->np;

	if (prev==0)
	{
		s->top=next;
	}
	else
	{
		prev->np=next;
	}

	if (next==0)
	{
		s->tail=prev;
	}
	else
	{
		next->pp = prev;
	}

	yifree(node);

	s->count--;
	return ;
}


// -------------------------------
IntPtr yiListRemoveEx(HandleList hList, int index)
{
	syiNode* p = __get__((sListc*)hList, index);
	IntPtr ret=0;

	if (p==0)
		return 0;

	ret = p->n;

	yiListRemove(hList, p);


	return ret;
}

// -------------------------------
void yiListClear(HandleList hList)
{
	sListc* s = (sListc*)hList;
	int i;

	for (i=0; i<s->count; i++)
	{
		yiListRemoveEx(hList, 0);
	}
}

// -------------------------------
syiNode* yiListGetTopFrom(HandleList hList, int index)
{
	return __get__((sListc*)hList, index);
}

// -------------------------------
syiNode* yiListGetTailFrom(HandleList hList, int index)
{
	return __getr__((sListc*)hList, index);
}

// -------------------------------
int yiListCount(HandleList hList)
{
	return ((sListc*)hList)->count;
}
