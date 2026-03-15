
#include "FileSystemStuff.h"
#include "string"

std::string GetFileString(const char* str)
{
    std::ifstream inFile(str/*, std::ios_base::ate*/);
    if (!inFile.is_open())
    {
        std::cout << "Error opening file \"" << str << "\"." << std::endl;
        return {};
    }

    //inFile.seekg(0);
    std::stringstream ghghgh;

    ghghgh << inFile.rdbuf();


    //char *strbuf;
    //strbuf = new char[(int)inFile.tellg()]();

    //strcpy(strbuf, ghghgh.str().c_str());

    return ghghgh.str();
}