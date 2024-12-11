#pragma once
#include "command.h"

class Prompt{
 
   std::string prompt;
   std::vector<Command> commands; 
   void parse();
   
   public:
   Prompt(std::string prompt);
   void execute();
   void wait_for_child_process();
};