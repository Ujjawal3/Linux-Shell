#include "utils.h"
#include "prompt.h"

extern Prompt* ptr;

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

void ctrl_c_handler(int num)
{
    if(ptr)
    kill(-ptr->child_pgid, SIGKILL);
}

void ctrl_z_handler(int num)
{
    if(ptr)
    ptr->background_process=true;
}