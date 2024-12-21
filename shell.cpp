#include "utils.h"
#include "prompt.h"
#include "history.h"
#include <sys/wait.h>
#include <bits/stdc++.h>
#include <readline/readline.h>



Prompt* ptr;
CommandHistory command_history;

int main()
{
    while (true)
    {
        signal (SIGTSTP, SIG_IGN);
        signal (SIGINT, SIG_IGN); 
        rl_initialize();
        rl_bind_keyseq("\\e[A", bind_up_arrow_key);
        rl_bind_keyseq("\\e[B", bind_down_arrow_key);

        std::string prompt_string="\033[1;31m" + std::filesystem::current_path().string() + "\033[0m" + "$ " ;

        char* userInput = readline(prompt_string.c_str());
        std::string cmd(userInput);

        trim(cmd);

        if (!cmd.size())
            continue;
        
        command_history.add_command(cmd);

        Prompt prompt(cmd);
        ptr=&prompt;
        prompt.execute();

        ptr=nullptr;
        free(userInput);
    }

    return 0;
}