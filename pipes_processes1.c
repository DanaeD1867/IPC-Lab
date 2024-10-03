// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        close(fd1[1]); 
        
  
        // Wait for child to send a string 
        wait(NULL); 

        char pipe2_input[100]; 
        read(fd2[0], pipe2_input, 100); 
        close(fd2[0]); 
  

        strcat(pipe2_input, fixed_str2);  

        printf("P1 received and concatenated string: %s\n", pipe2_input); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        
        // Read a string using first pipe 
        char user_str[100]; 
        read(fd1[0], user_str, 100); 
        close(fd1[0]); 
  
        strcat(user_str, fixed_str); 
        printf("Pipe 2 received and concatenated string: %s\n", user_str); 

        char user_str2[100]; 
        printf("Enter a second string: "); 
        scanf("%s", user_str2); 

        
        strcat(user_str2, fixed_str); 
        strcat(user_str2, fixed_str2); 

        write(fd2[1], user_str, strlen(user_str)+1);
        close(fd2[1]);  
  
        exit(0); 
    } 
    return 0; 
} 