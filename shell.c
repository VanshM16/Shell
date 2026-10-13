#define _POSIX_C_SOURCE 200809L
#define MAX_ARGS 128
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For waitpid()
#include <unistd.h>  // For fork() and execvp(), isatty()
#include <errno.h>

int tokenize(char* line, char** argv);
int lastStatus=0;
int run_command(char** argv);

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
        run_command(argv);
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
int run_command(char** argv){
    pid_t pid=fork();
    if(pid<0) {
        perror("MyShell: fork");
        return -1;
    }
    if(pid==0){
        execvp(argv[0], argv);
        int error = errno;
        perror(argv[0]);
        if(error==ENOENT) _exit(127);
        else _exit(126);
    }
    int status;
    if(waitpid(pid, &status, 0)==-1){
        perror("MyShell: waitpid");
        return 1;
    }
    if(WIFEXITED(status)){
        lastStatus=WEXITSTATUS(status);
    } else if(WIFSIGNALED(status)){
        lastStatus=128+WTERMSIG(status);
    }
return lastStatus;
}