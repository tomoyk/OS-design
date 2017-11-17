#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
        char line[128];
        char command[128];
        int st;

        fprintf(stderr, "%s", "welcome > ");
        while(fgets(line, sizeof(line), stdin) != NULL) {
                if(sscanf(line, "%s", command) > 0) {
                        if(fork() == 0) {
                                if(execlp(command, command, NULL) < 0) {
                                        exit(EXIT_FAILURE);
                                }
                        } else {
                                wait(&st);
                        }
                }
                fprintf(stderr, "%s", "> ");
        }
}

