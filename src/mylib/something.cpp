#include "mytypes.h"
#include "string"
#include "libs.h"
#include "iostream"

class CMyLib : public IMyLib
{
public:
    virtual void bebra() {std::cout << "bullshit" << std::endl ;} ;
    virtual void aboba() {std::cout << "Something"<< std::endl ;} ;
};

static CMyLib bobr;

extern "C"
{
    EXPORT const char* ReturnSomeString()
    {
        return "something";
    }

    EXPORT IMyLib*  GetMyLib()
    {
        return &bobr;
    }
}