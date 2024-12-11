#include "utils.h"

void trim(std::string& str)
{
    for(int i=0;i<str.size();i++)
    {
        if(str[i]!=' ')
        {str=str.substr(i);break;}
    }
    while(str.size() && str.back()==' ')
    str.pop_back();
}

