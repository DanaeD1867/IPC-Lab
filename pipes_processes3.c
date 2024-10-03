#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2]; 

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  pid_t p1, p2, p3; 



  if(pipe(pipefd) == -1){
    fprintf(stderr, "Pipe Failed" ); 
    return 1; 
  }

  p1 = fork();

  if (p1 < 0){
     fprintf(stderr, "ForkFailed" ); 
    return 1; 
  }
  if(p1 == 0)
    {

      dup2(pipefd[1], STDOUT_FILENO);

    
      close(pipefd[0]); 
      close(pipefd[1]);

      // execute cat
      execvp("cat", cat_args);
    }

  if(pipe(pipefd2) == -1){
    fprintf(stderr, "Pipe Failed" ); 
    return 1; 
  }

  p2 = fork(); 

  if(p2 < 0){
    fprintf(stderr, "Fork Failed" ); 
    return 1; 
  }

  if(p2 == 0)
    {

      dup2(pipefd[0], STDIN_FILENO);
      dup2(pipefd2[1], STDOUT_FILENO);

      // close unused unput half of pipe

      close(pipefd[0]);
      close(pipefd[1]); 
      close(pipefd2[0]);
      close(pipefd2[1]);

      // execute cat

      execvp("grep", grep_args);
    }

    close(pipefd[0]); 
    close(pipefd[1]); 

    p3 = fork();  

  if(p3 < 0){
    fprintf(stderr, "Fork Failed" ); 
    return 1; 
  }

  if(p3 == 0){
    dup2(pipefd2[0], STDIN_FILENO); 

    close(pipefd2[1]); 
    close(pipefd2[0]); 

    execvp("sort", sort_args); 

  }

  close(pipefd2[1]); 
  close(pipefd2[0]);

  return 0; 

}