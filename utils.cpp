#include "utils.h"
#include "prompt.h"
#include "history.h"
#include <readline/readline.h>

extern Prompt* ptr;
extern CommandHistory command_history;

void cd(std::string path)
{
    if (path == "~")
    {
        path = getenv("HOME");
    }
    int chdir_status = chdir(path.c_str());
    if (chdir_status == -1)
    {
        std::cout << "Unable to cd in " << path << std::endl;
    }
    return;
}


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

int bind_up_arrow_key(int count, int key)
{
    command_history.decrease_pointer();
    rl_replace_line(command_history.current_command().c_str(), 0);
    return 0;
}

int bind_down_arrow_key(int count, int key)
{
    if (command_history.increase_pointer() == 0)
    {
        command_history.reset_pointer();
        rl_replace_line("", 0);
    }
    else
    {
        rl_replace_line(command_history.current_command().c_str(), 0);
    }
    return 0;
}