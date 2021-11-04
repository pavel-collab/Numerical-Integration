#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// pipe :
// 0 -- чтение
// 1 -- запись

int main(int argc, char* argv[]) {

    // create pipe
    int pips_fds[2];
    if (pipe(pips_fds) < 0) {
        perror("failed to create pipe");
        return -1;
    }

    // create child process
    pid_t child_id = fork();
    if (child_id < 0) {
        perror("fork");
        return -1;
    }

    if (child_id == 0) {

        // close parents' pipe
        close(pips_fds[1]);
        // redirect stdin to pipe
        dup2(pips_fds[0], fileno(stdin));
        close(pips_fds[0]);

        // call gnuplot
        execlp("gnuplot", "gnuplot", NULL);
        perror("failed to exec gnuplot");
    }
    else {
        close(pips_fds[0]);

        // write a command for gnuplot to the pipe
        write(pips_fds[1], "plot cos(x)", strlen("plot cos(x)"));
        close(pips_fds[1]);

        // wait the end of child process
        wait(NULL);
    }

    
    exit(EXIT_SUCCESS);
}

