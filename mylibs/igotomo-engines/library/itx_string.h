/**
 * Copyright 2015 (c) 
 * authorized by dongho.yoo.
 * 
 * @brief 文字列を扱うライブラリ
 *
*/
#ifndef _ITX_STRING_H_
#define _ITX_STRING_H_

template <size_t SIZE>
class ItxString : public IgxString
{
public:
    ItxString()
    {
        IgxString();
        m_str.str=m_buffer;
    }
    ItxString(char* p)
    {
        IgxString();
        m_str.str=m_buffer;
    }
    virtual ~ItxString() {};
private:    
    char m_buffer[SIZE];
};

#endif
