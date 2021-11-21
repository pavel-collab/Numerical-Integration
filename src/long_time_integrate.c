#define TEST_MODE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lib.h"

int main(int argc, char* argv[]) {
    
    #ifndef TEST_MODE
    if (argc != 4) {
        printf("Usage: %s <integration limits> <function(x)>\n", argv[0]);
        return -1;
    } 

    // integration limits
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    #else
    double a = 0;
    double b = 0;

    scanf("%lf %lf", &a, &b);
    printf("%lf %lf\n", a, b);
    #endif 

    // ===========================================================================================

    // create pipe to manage gcc from current programm
    int gcc_pipe[2];
    if (pipe(gcc_pipe) < 0) {
        perror("failed to create pipe");
        return -1;
    }

    // fork a process to execute gcc
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

    // write an instruction for a dynamic lib
    #ifndef TEST_MODE
    dprintf(gcc_pipe[1],
            "#include <math.h>\n"
                "double tmpfun(double x) {"
                "return %s;"
            "}\n", argv[3]);
    #else
    char function[20];
    scanf("%s", function);
    printf("function : %s\n", function);
    dprintf(gcc_pipe[1],
            "#include <math.h>\n"
                "double tmpfun(double x) {"
                "return %s;"
            "}\n", function);
    #endif
    close(gcc_pipe[1]);

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

    // ===========================================================================================

    printf("simple integrate rusult: %f\n", Trapez(a, b, 4 * 5000000, tmpfun));
    
    return 0;
}