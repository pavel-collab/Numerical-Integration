//* /usr/bin/time -o time.log --verbose ./out

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//TODO: уменьшить накладные расходы
//TODO: переименовать функции, переменные и файлы

#include "lib.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Usage: %s <integration limits> <function(x)>\n", argv[0]);
        return -1;
    } 

    // количество потоков
    int thread_amount = 4;

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
    dprintf(gcc_pipe[1],
            "#include <math.h>\n"
                "double tmpfun(double x) {"
                "return %s;"
            "}\n", argv[3]);
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

    // integration limits
    double a = atof(argv[1]);
    double b = atof(argv[2]);

    volatile double main_sum = 0;

    //! выделим массив структур на куче (возможно, не лучшая идея, подумать над этим)
    // arg_t* thread_args = (arg_t*) calloc(thread_amount, sizeof(arg_t));
    arg_t thread_args[thread_amount];

    // в цикле задаем аргументы каждому процессу
    for (int i = 0; i < thread_amount; i++) {
        arg_t arg = {
            .g_mutex = PTHREAD_MUTEX_INITIALIZER,
            .sum = &main_sum,
            .function = tmpfun,
            .int_begin = a + ((b - a) / thread_amount) * i,
            .int_end = a + ((b - a) / thread_amount) * (i + 1),
            .point_amount = 5000000
        };

        thread_args[i] = arg;
    }

    pthread_t thread_id[thread_amount]; 

    for (int i = 0; i < thread_amount; ++i) {
        if (errno = pthread_create(&thread_id[i], NULL, ThrTrapez, &thread_args[i])) {
            perror("pthread_create");
        return 1;
    }
    }

    // wait for a thread [thread_id]
    for (int i = 0; i < thread_amount; ++i) {
        pthread_join(thread_id[i], NULL);
    }

    printf("result = %lf\n", main_sum);

    return 0;
}

//! подумать, как уменьшить накладные расходы:: calloc -> придумать, как генерить аргументы для потоков

//* threads:
//? pthread_exit()

//TODO: CMake

//TODO: соединить программу с использованием gnuplot
// --------------------------------------------------------------------------------------------
//TODO: compare the speed of programm working with threads and without ones
//TODO: unit tests 

//TODO: придумать, как реализовать идею с коэффициентом точности

//* computering numerical value of integral by the threads
//* use the main thread to make a plot (using gnuplot call)