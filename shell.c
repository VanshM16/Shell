#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>  // For pid_t
#include <unistd.h>  // For fork() and execvp(), isatty()


int main(void){
    char *line = NULL;
    size_t capacity = 0;
    int interactive = isatty(STDIN_FILENO);
    printf("MyShell> ");
    fflush(stdout);

    ssize_t length = getline(&line, &capacity, stdin);
    for( ; ;){
        if(interactive){
            printf("MyShell> ");
            fflush(stdout);
        }
        ssize_t length = getline(&line, &capacity, stdin);
        if(length==-1){
            if(ferror(stdin)) perror("MyShell: read");
            else if(interactive) putchar('\n');
            break;
        }
        if(length==1 && line[0]=='\n') continue;
        printf("You entered: %s", line);
    }
    free(line);
    return 0;
}