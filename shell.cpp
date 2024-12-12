#include "utils.h"
#include "prompt.h"
#include <sys/wait.h>
#include <bits/stdc++.h>
#include <readline/readline.h>


Prompt* ptr;

int main()
{
    while (true)
    {
        signal (SIGTSTP, SIG_IGN);
        signal (SIGINT, SIG_IGN); 

        char* userInput = readline(">> ");
        std::string cmd(userInput);

        trim(cmd);

        if (!cmd.size())
            continue;

        Prompt prompt(cmd);
        if(!prompt.commands[0].args.empty() && prompt.commands[0].args[0]=="exit")
        {
            return 0;
        }
        ptr=&prompt;
        prompt.execute();
        ptr=nullptr;
    }

    return 0;
}