#include "utils.h"
#include "command.h"
#include <fcntl.h>

Command::Command(const std::string &command) : inp_file(""), out_file(""),
                                               inp(STDIN_FILENO), out(STDOUT_FILENO)
{
    parse(command);
}

Command::~Command()
{
    if (inp != STDIN_FILENO)
        close(inp);
    if (out != STDOUT_FILENO)
        close(out);
}

void Command::parse(const std::string &command)
{
    int inp = command.find("<");
    int out = command.find(">");
    std::string to_execute;

    if (inp == std::string::npos && out == std::string::npos)
        to_execute = command;
    else if (inp == std::string::npos && out != std::string::npos)
    {
        to_execute = command.substr(0, out);
        trim(to_execute);
        out_file = command.substr(out + 1);
        trim(out_file);
    }
    else if (inp != std::string::npos && out == std::string::npos)
    {
        to_execute = command.substr(0, inp);
        trim(to_execute);
        inp_file = command.substr(inp + 1);
        trim(inp_file);
    }
    else
    {
        to_execute = command.substr(0, inp);
        trim(to_execute);
        inp_file = command.substr(inp + 1, out - inp - 1);
        trim(inp_file);
        out_file = command.substr(out + 1);
        trim(out_file);
    }
    parse_args(to_execute);
}

void Command::parse_args(const std::string &to_execute)
{
    std::string tmp;
    char DELIM = ' ';

    for (int i = 0; i < to_execute.size(); i++)
    {
        if (to_execute[i] == '\'' || to_execute[i] == '\"')
        {
            for (int j = i + 1; j < to_execute.size(); j++)
            {
                if (to_execute[j] == to_execute[i])
                {
                    args.push_back(to_execute.substr(i+1, j - i - 1));
                    i = j;
                    break;
                }
                else if (to_execute[j] == '\\' && j + 1 < to_execute.size())
                    j++;
                else if (j == to_execute.size() - 1)
                {
                    args.push_back(to_execute.substr(i));
                    i = j;
                    break;
                }
            }
        }
        else if (to_execute[i] == '\\')
        {
            tmp.push_back(to_execute[i]);
            if (i + 1 < tmp.size())
            {
                i++;
                tmp.push_back(to_execute[i]);
            }
        }
        else if (to_execute[i] == DELIM)
        {
            trim(tmp);
            if (!tmp.empty())
                args.push_back(tmp);
            tmp = "";
        }
        else
            tmp.push_back(to_execute[i]);
    }
    trim(tmp);
    if (!tmp.empty())
        args.push_back(tmp);
}

void Command::execute()
{
    char *args_array[args.size() + 1];

    for (int i = 0; i < args.size(); i++)
        args_array[i] = (char *)args[i].c_str();
    args_array[args.size()] = NULL;

    execvp(args_array[0], args_array);
    std::cerr << "Error in executing command \n";

    exit(0);
}

void Command::set_inp_out()
{
    if (!inp_file.empty())
    {
        if ((inp = open(inp_file.c_str(), O_RDONLY)) < 0)
        {
            perror("open() failed");
            exit(1);
        }
        if (dup2(inp, STDIN_FILENO) < 0)
        {
            perror("dup2() failed");
            exit(1);
        }
    }

    if (!out_file.empty())
    {
        if ((out = open(out_file.c_str(), O_WRONLY | O_CREAT, 0644)) < 0)
        {
            perror("open() failed");
            exit(1);
        }
        if (dup2(out, STDOUT_FILENO) < 0)
        {
            perror("dup2() failed");
            exit(1);
        }
    }
}