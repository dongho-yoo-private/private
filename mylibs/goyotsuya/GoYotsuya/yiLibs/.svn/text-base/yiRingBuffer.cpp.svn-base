#include "yitRingBuffer.h"

template<typename T> cyitRingBuffer<T>::cyitRingBuffer(int nMaxElementCount, Bool bIsOverwrite)
: m_nMaxElementCount(nMaxElementCount),
  m_bIsOverwrite(bIsOverwrite),
  m_buffer(0),
  m_nCount(0),
  m_nTop(0),
  m_nBottom(0)

{
	m_buffer=(T**)yialloc(sizeof(T*)*nMaxElementCount);
}

template<typename T> cyitRingBuffer<T>::~cyitRingBuffer(void)
{
	if (m_buffer)
	{
		yifree(m_buffer);
	}
}

// ----------------------------
template<typename T> int cyitRingBuffer<T>::push(T& val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	m_buffer[m_nBottom++]=&val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitRingBuffer<T>::pop(T& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}

	val = m_buffer[m_nBottom--];

	if (m_nBottom<0)
	{
		m_nBottom=m_nMaxElementCount-1;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitRingBuffer<T>::enq(T& val)
{
	if (m_nCount==m_nMaxElementCount)
	{
		if (m_bIsOverwrite==False)
		{
			return RINGBUFFER_FULL;
		}
	}

	m_buffer[m_nBottom++]=&val;

	if (m_nBottom==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(++m_nCount));
}

// ----------------------------
template<typename T> int cyitRingBuffer<T>::deq(T& val)
{
	if (m_nCount==0)
	{
		return RINGBUFFER_EMPTY;
	}

	val = m_buffer[m_nTop++];

	if (m_nTop==m_nMaxElementCount)
	{
		m_nTop=0;
	}

	return (m_nMaxElementCount-(--m_nCount));
}

// ----------------------------
template<typename T> int cyitRingBuffer<T>::count()
{
	return m_nCount;
}

// ----------------------------
template<typename T> T& cyitRingBuffer<T>::get(int n)
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
template<typename T> void cyitRingBuffer<T>::clear()
{
	m_nTop=0;
	m_nBottom=0;
	m_nCount=0;
	return 0;
}
