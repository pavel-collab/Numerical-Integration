//* gcc -o out -Wall -Wextra gnuplot_call.c -ldl

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include <math.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s <function(x)>\n", argv[0]);
        return -1;
    }

    // Call gcc

    int gcc_pipe[2];
    if (pipe(gcc_pipe) < 0) {
        perror("failed to create pipe");
        return -1;
    }

    pid_t my_id;
    if ((my_id = fork()) == 0) {
        // Connect pipe output to stdin, close unnesessary fds
        close(gcc_pipe[1]);

        if (dup2(gcc_pipe[0], fileno(stdin)) < 0) {
            perror("dup2");
            close(gcc_pipe[0]);
            return -1;
        }
        close(gcc_pipe[0]);

        // Start compilation
        execlp ("gcc", 
                "gcc", "-Wall", "-Wextra", 
                "-fPIC", "-fPIE", "-shared",
                "-O2", "-fomit-frame-pointer", "-march=native", "-mtune=native",
                "-xc", "-", 
                "-lm", 
                "-o", "tmp.so", NULL);
        perror("exec");
        return 1;
    }

    dprintf(gcc_pipe[1],
            "#include <math.h>\n"
                "double tmpfun(double x) {"
                "return %s;"
            "}\n", argv[1]);
    close(gcc_pipe[1]);

    // Wait for compilation end and check its status

    int status;
    waitpid(my_id, &status, 0);

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        printf("[err] compilation error, gcc exit code %d\n", WEXITSTATUS(status));
        return 2;
    }

    // Try to load compiled library

    void* tmplib = dlopen("./tmp.so", RTLD_LAZY);
    char* dlerrstr;
    if (!tmplib) {
        perror(dlerror());
        return -1;
    }

    double (*tmpfun)(double);
    //! man dlopen
    *(void **) (&tmpfun) = dlsym(tmplib, "tmpfun");
    if ((dlerrstr = dlerror()) != NULL) {
        puts(dlerrstr);
        return -1;
    }

    // Try ro use it

    FILE* plot_pipe = popen("gnuplot -persist", "w");
    fputs("plot '-' w line\n", plot_pipe);
    int i;
    for (i = 0; i < 512; i++) {
        double x = i * M_PI/256;
        fprintf(plot_pipe, "%lf %lf\n", x, (*tmpfun)(x));
    }

    return 0;
}