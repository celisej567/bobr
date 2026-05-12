#include <string> 
#include <vector> 
#include <string_view>

#define CMD_ARGS_MAX 128
#define CMD_ARGS_RESERVE CMD_ARGS_MAX

class CMD
{

public:
    // Get an argument parameter as a string.
    static const char* GetArg( const char* strArg, const char* strDefault = NULL );

    // Get ID of an argument if it exists. Otherwise return 0
    // Case sensitive. Skips executable path.
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
