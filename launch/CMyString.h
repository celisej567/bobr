#pragma once

class CMyString
{
public:
    CMyString()
    {
        pBuff = 0;
    }

    CMyString( char* str )
    {
        pBuff = (void*)str;
    }

    ~CMyString()
    {
        if(pBuff)
            delete[] pBuff;
    }

    const char* Get() { return (const char*)pBuff; }


    void* pBuff;
};