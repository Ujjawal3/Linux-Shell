#include "utils.h"
#include "prompt.h"
#include <sys/wait.h>
#include <bits/stdc++.h>

int main()
{
    while (true)
    {
        std::cout << "<< ";
        std::string cmd;
        std::getline(std::cin, cmd);
        
        trim(cmd);

        if (!cmd.size())
            continue;

        Prompt prompt(cmd);
        prompt.execute();
    }

    return 0;
}