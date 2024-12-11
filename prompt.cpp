#include "prompt.h"
#include "utils.h"
#include <sys/wait.h>


Prompt::Prompt(std::string prompt) : prompt(prompt){
   parse();
}

void Prompt::parse()
{        
   std::istringstream ss(prompt);
   std::string command;

   while (std::getline(ss, command, '|')) {
      trim(command);
      if(command.size())commands.push_back(command);
   }
}

void Prompt::wait_for_child_process()
{
   for (int i = 0; i < commands.size(); i++)
      wait(NULL);
}

void Prompt::execute()
{
   int o_pp[2], n_pp[2];

   for (int i = 0; i < commands.size(); i++)
   {
      if (i != commands.size() - 1 && pipe(n_pp) == -1)
      {
         std::cout << "pipe() failed\n";
         exit(1);
      }

      pid_t pid = fork();
      if (pid == -1)
      {
         std::cout << "fork() failed\n";
         exit(1);
      }
      if (pid == 0)
      {

         commands[i].set_inp_out();
         if (i != 0)
         {
            close(o_pp[1]);
            if (dup2(o_pp[0], commands[i].inp) < 0)
            {
               perror("dup2() failed");
               exit(1);
            }
            close(o_pp[0]);
         }
         if (i != commands.size() - 1)
         {
            close(n_pp[0]);
            if (dup2(n_pp[1], commands[i].out) < 0)
            {
               perror("dup2() failed");
               exit(1);
            }
            close(n_pp[1]);
         }
         commands[i].execute();
      }
      else
      {
         if (i != 0)
         {
            close(o_pp[0]);
            close(o_pp[1]);
         }
         o_pp[0] = n_pp[0];
         o_pp[1] = n_pp[1];
      }
   }
   wait_for_child_process();
}
