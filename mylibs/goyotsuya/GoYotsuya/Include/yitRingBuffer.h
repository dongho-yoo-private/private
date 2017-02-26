#pragma once
#include "yistd.h"
#include <iostream>

using namespace std;

#define RINGBUFFER_FULL (-1)
#define RINGBUFFER_EMPTY (-1)

template <typename T> class cyitrb
{
public:
	cyitrb(int nMaxElementCount, Bool bUsingByRing=False, Bool bIsOverwrite=False);
	~cyitrb(void);

	// ----------------------------
	int push(T val);

	// ----------------------------
	int push(T* val);

	// ----------------------------
	int pop(T& val);

	// ----------------------------
	int pop(T*& val);

	// ----------------------------
	int enq(T val);

	// ----------------------------
	int enq(T* val);

	// ----------------------------
	int deq(T& val);

	// ----------------------------
	int deq(T*& val);

	// ----------------------------
	int count();

	// ----------------------------
	T& get(int n);

	// ----------------------------
	T* getp(int n);

	// ----------------------------
	void clear();

private:
	T* m_buffer;
	int m_nMaxElementCount;
	int m_nCount;
	int m_nTop;
	int m_nBottom;
	Bool m_bIsOverwrite;
	Bool m_bUsingByRing;
};


template<typename T> cyitrb<T>::cyitrb(int nMaxElementCount, Bool bUsingByRing, Bool bIsOverwrite)
: m_nMaxElementCount(nMaxElementCount),
  m_bIsOverwrite(bIsOverwrite),
  m_buffer(0),
  m_nCount(0),
  m_nTop(0),
  m_nBottom(0),
  m_bUsingByRing(bUsingByRing)

{
	m_buffer=(T*)yialloci(sizeof(T*)*nMaxElementCount);
}

template<typename T> cyitrb<T>::~cyitrb(void)
{
	if (m_buffer)
	{
		yifree(m_buffer);
	}
}

// ----------------------------
template<typename T> int cyitrb<T>::push(T val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	m_buffer[m_nBottom++]=val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::push(T* val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	T** x = (T**)m_buffer;
	x[m_nBottom++]=val;
	//m_buffer[m_nBottom++]=val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::pop(T& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}
	val = m_buffer[--m_nBottom];
	m_buffer[m_nBottom]=0;

	if (m_nBottom<0)
	{
		m_nBottom=m_nMaxElementCount-1;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::pop(T*& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}

	//val = m_buffer[--m_nBottom];
	T** x = (T**)m_buffer;
	val = x[--m_nBottom];
	x[m_nBottom]=0;

	if (m_nBottom<0)
	{
		m_nBottom=m_nMaxElementCount-1;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::enq(T val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	m_buffer[m_nBottom++]=val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::enq(T* val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	T** x = (T**)m_buffer;
	x[m_nBottom++]=val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::deq(T& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}

	val = m_buffer[m_nTop];
	m_buffer[m_nTop++]=0;

	if (m_nTop==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::deq(T*& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}

	T** x=(T**)m_buffer;
	val = x[m_nTop];
	x[m_nTop++]=0;

	if (m_nTop==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitrb<T>::count()
{
	return m_nCount;
}

// ----------------------------
template<typename T> T& cyitrb<T>::get(int n)
{
	int index;
	int start=m_nTop;

	for (int i=0; i<n; i++)
	{
		start++;
		if (start==m_nMaxElementCount)
			start=0;
	}

	return m_buffer[start];
}

// ----------------------------
template<typename T> T* cyitrb<T>::getp(int n)
{
	int start=m_nTop;

	for (int i=0; i<n; i++)
	{
		start++;
		if (start==m_nMaxElementCount)
			start=0;
	}

	T** x=(T**)m_buffer;
	
	return x[start];
}

// ----------------------------
template<typename T> void cyitrb<T>::clear()
{
	m_nTop=0;
	m_nBottom=0;
	m_nCount=0;
	memset(m_buffer, 0, sizeof(T*)*m_nMaxElementCount);

}
