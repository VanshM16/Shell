#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>  // For pid_t
#include <unistd.h>  // For fork() and execvp(), isatty()


int main(){
    char *line = NULL;
    size_t capacity = 0;
    printf("MyShell> ");
    fflush(stdout);

    ssize_t length = getline(&line, &capacity, stdin);
    while(length!=-1){
        printf("You entered: %s", line);
        printf("MyShell> ");
        fflush(stdout);
        length = getline(&line, &capacity, stdin);
        if(length==-1){
            printf("Error reading input\n");
        }
    }
    free(line);
}