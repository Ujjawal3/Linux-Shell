#pragma once
#include <bits/stdc++.h>


class CommandHistory
{
    std::string history_file = ".history";
    int max_history_size = 1000;
    int history_pointer = 0;
    std::deque<std::string> history_queue;
    std::ofstream ofile;

public:
    CommandHistory();
    std::string current_command();
    void add_command(std::string command);
    void reset_pointer();
    void decrease_pointer();
    int increase_pointer();
};