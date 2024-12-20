#include "utils.h"
#include "prompt.h"
#include <sys/wait.h>
#include <bits/stdc++.h>
#include <readline/readline.h>
// #include <filesystem>


Prompt* ptr;

int main()
{
    while (true)
    {
        signal (SIGTSTP, SIG_IGN);
        signal (SIGINT, SIG_IGN); 

        std::string prompt_string="\033[1;31m" + std::filesystem::current_path().string() + "\033[0m" + "$ " ;

        char* userInput = readline(prompt_string.c_str());
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
        free(userInput);
    }

    return 0;
}