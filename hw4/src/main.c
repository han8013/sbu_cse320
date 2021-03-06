#include "sfish.h"
#include "debug.h"
#include <signal.h>


/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){
    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
    char *cmd;
    char shellPrompt[1024];

    changePrompt(shellPrompt);
    signal(SIGALRM,alarmHandler);
    signal(SIGUSR2,killHandler);
    // signal(SIGTSTP, SIG_IGN);
    signal(SIGTSTP,blockHandler);


    struct sigaction sa;
    sa.sa_handler = sig_child;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &sa, NULL);

    while((cmd = readline(shellPrompt)) != NULL) {

        // strcpy(buffer,cmd);
        if (eval(cmd,shellPrompt) == -1)
        {
            break;
        }

        // printf("%s\n",buffer);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */
        // info("Length of command entered: %ld\n", strlen(cmd));
        /* You WILL lose points if your shell prints out garbage values. */
    }
    /* Don't forget to free allocated memory, and close file descriptors. */
    free(cmd);

    return EXIT_SUCCESS;
}
