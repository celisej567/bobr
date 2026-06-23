#include <string> 
#include <vector> 
#include <string_view>
#include "mytypes.h"

#define CMD_ARGS_MAX 128
#define CMD_ARGS_RESERVE CMD_ARGS_MAX

class CMD
{

public:
    // Get an argument parameter as a string.
    // Case sensitive.
    static const char* GetArg( const char* strArg, const char* strDefault = NULL );

    // Get an argument parameter as an int.
    // Case sensitive.
    static bool GetArgBool( const char* strArg, bool bDefault = false);

    // Get an argument parameter as an int.
    // Case sensitive.
    static int GetArgInt( const char* strArg, int iDefault = 0 );

    // Get an argument parameter as an float.
    // Case sensitive.
    static float GetArgFloat( const char* strArg, float fDefault = 0 );

    // Get an argument parameter as an double.
    // Case sensitive.
    static double GetArgDouble( const char* strArg, double dDefault = 0 );

    // Get ID of an argument if it exists. Otherwise return 0
    // Case sensitive.
    static uint FindArg( const char* strArg );

    // Returns an executable path.
    // Usually its always there. If its not or theres some garbage - something REALLY BAD happened.
    static const char* GetExecutable();
protected:

    // You can try and call this mid runtime, but i whouldnt.
    static void ProcessArguments(int argc, char **argv);
private:
    /// TODO (celisej): change this to new vector/stack/array type when one will be done
    //                  and change CMD_ARGS_RESERVE and CMD_ARGS_MAX to be separate
    static char* m_Args[CMD_ARGS_RESERVE];
    static uint m_ArgsCount;

    // no im not proud of this dont hount me in my nightmares pls
    friend int main(int argc, char **argv);
};
