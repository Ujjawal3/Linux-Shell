#include "prompt.h"
#include "utils.h"
#include <sys/wait.h>

Prompt::Prompt(std::string prompt) : prompt(prompt){
   background_process=false;
   child_pgid=0;
   parse();
}

void Prompt::parse()
{        
   if(prompt.back()=='&')
   {background_process=true;prompt.pop_back();}

   std::istringstream ss(prompt);
   std::string command;

   while (std::getline(ss, command, '|')) {
      trim(command);
      if(command.size())commands.push_back(command);
   }
}

void Prompt::wait_for_child_process()
{
   int status;
   pid_t pid;

   while(true){
      pid = waitpid(-child_pgid, &status, WNOHANG);
      if(background_process || pid==-1 )
      break;
   }
}

void Prompt::execute()
{
   int o_pp[2], n_pp[2];

   bool pipe_initialized=false;

   for (int i = 0; i < commands.size(); i++)
   {
      if(commands[i].exec_spl_commands())continue;

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
         signal (SIGTSTP, SIG_IGN);
         signal (SIGINT, SIG_IGN); 
         
         commands[i].set_inp_out();
         if (pipe_initialized)
         {
            close(o_pp[1]);
            if (dup2(o_pp[0], commands[i].inp) < 0)
            {
               perror("dup2() failed");
               exit(1);
            }
            close(o_pp[0]);
            setpgid(0, child_pgid);
         }
         else
         {
            pipe_initialized=true;
            child_pgid=getpid();
            setpgrp();
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
         if (pipe_initialized)
         {
            close(o_pp[0]);
            close(o_pp[1]);
         }
         else{
            pipe_initialized=true;
            child_pgid=pid;
            signal (SIGTSTP, ctrl_z_handler);
            signal (SIGINT, ctrl_c_handler); 
         }

         setpgid(pid, child_pgid);
         o_pp[0] = n_pp[0];
         o_pp[1] = n_pp[1];
      }
   }
   wait_for_child_process();
}
