
#include "FileSystemStuff.h"
#include "string"

std::string GetFileString(std::string str)
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

std::vector<std::string> MySplitString(std::string buff)
{
    std::vector<std::string> retvec;
    if(buff.empty())
        std::abort();

    size_t i = 0;
    std::string temp;
    while(buff[i])
    {
        if(buff[i] == '\n')
        {
            if( !(temp.empty()) )
            {
                retvec.push_back(temp);
                temp.clear();
            }
            i++;
            continue;
        }

        temp+=buff[i];
        i++;
    }

    return retvec;
}