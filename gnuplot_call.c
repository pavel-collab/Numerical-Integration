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
        execlp (
                // executable
                "gcc", 
                // argv[0], argv[1], ...
                "gcc", "-Wall", "-Wextra", 
                // produce position-independent dynamic library (shared option)
                "-fPIC", "-fPIE", "-shared",
                // enable optimisations
                "-O2", "-fomit-frame-pointer", "-march=native", "-mtune=native",
                // read input from stdin, treat it as a C source
                "-xc", "-",
                // link with linkm
                "-lm", 
                // save output to ymp.so
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

    // fputs("set terminal png size 800, 600\n", plot_pipe);
    // fputs("set output './images/result.png'\n", plot_pipe);

    fputs("set xlabel 'X'\n", plot_pipe);
    fputs("set ylabel 'Y'\n", plot_pipe);
    fputs("set grid\n", plot_pipe);
    fputs("set xrange [-4*pi:4*pi]\n", plot_pipe);
    fputs("set yrange [-1.5:1.5]\n", plot_pipe);
    fputs("set title 'My plot'\n", plot_pipe);

    fputs("plot '-' w line\n", plot_pipe);

    for (int i = -1024; i < 1024; i++) {
        double x = i * M_PI/256;
        fprintf(plot_pipe, "%lf %lf\n", x, (*tmpfun)(x));
    }

    return 0;
}