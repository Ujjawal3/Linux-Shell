#pragma once
#include "command.h"

class Prompt{
 
   std::string prompt;
   void parse();
   
   public:
   Prompt(std::string prompt);
   std::vector<Command> commands; 
   bool background_process;
   int child_pgid;
   void execute();
   void wait_for_child_process();
};