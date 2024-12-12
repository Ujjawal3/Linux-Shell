#pragma once

#include <bits/stdc++.h>

class Command{
    std::string inp_file, out_file;

    void parse(const std::string&);
    void parse_args(const std::string& to_execute);

    public:
    std::vector<std::string> args;
    
    int inp, out;
    Command(const std::string&);
    ~Command();
    void execute();
    void set_inp_out();
};