#define _POSIX_C_SOURCE 200809L
#define MAX_ARGS 128
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>  // For pid_t
#include <unistd.h>  // For fork() and execvp(), isatty()

int tokenize(char* line, char** argv);

int main(void){
    char *line = NULL;
    size_t capacity = 0;
    int interactive = isatty(STDIN_FILENO);

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
        char* argv[MAX_ARGS];
        int argc=tokenize(line, argv);
        if(argc<=0) continue;
        for(int i=0; i<argc; i++){
            printf("argv[%d]: %s\n", i, argv[i]);
        }
    }
    free(line);
    return 0;
}

int tokenize(char* line, char** argv){
    int argc=0;
    char*p =line;
    while(*p!='\0'){
        while(isspace((unsigned char)*p)) p++;
        if(*p=='\0') break; // End of line
        if(argc>=MAX_ARGS-1){
            fprintf(stderr, "MyShell: too many arguments\n");
            return -1;
        }
        argv[argc++] = p;
        while(*p!='\0' && !isspace((unsigned char)*p)) p++;
        if(*p!='\0'){
            *p='\0';
            p++;
        }
    }
    argv[argc]=NULL;
    return argc;
}