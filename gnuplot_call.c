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

    // Prepare sorce file
    FILE* f = fopen("tmp.c", "w");
    fprintf(f,
            "#include <math.h>\n"
            "double tmpfun(double x) {return %s;}\n", argv[1]);
    fclose(f);

    // Call gcc

    pid_t my_id;
    if ((my_id = fork()) == 0) {
        execlp("gcc", "gcc", "-w", "-Wall", "-fPIC", "-fPIE", "-shared", "-lm", "tmp.c", "-o", "tmp.so", NULL);
        perror("exec");
        return 1;
    }

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
    //! find out man dlopen
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