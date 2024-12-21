#include "history.h"

CommandHistory::CommandHistory()
{
    std::ifstream file;
    file.open(history_file);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            history_queue.push_back(line);
        }
        file.close();
    }
    reset_pointer();
    ofile.open(history_file, std::ios_base::app);
}

std::string CommandHistory::current_command()
{
    if (history_queue.size() == 0)
    {
        return "";
    }
    return history_queue[history_pointer];
}

void CommandHistory::add_command(std::string command)
{
    if (history_queue.size() == max_history_size)
    {
        history_queue.pop_front();
    }
    history_queue.push_back(command);
    history_pointer = history_queue.size();
    ofile << command << std::endl;
}

void CommandHistory::reset_pointer()
{
    history_pointer = history_queue.size();
}

void CommandHistory::decrease_pointer()
{
    if (history_pointer > 0)
    {
        history_pointer--;
    }
}

int CommandHistory::increase_pointer()
{
    if (history_pointer < history_queue.size() - 1)
    {
        history_pointer++;
        return 1;
    }
    else
    {
        return 0;
    }
}
