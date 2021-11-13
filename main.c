//* /usr/bin/time -o time.log --verbose ./out

#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lib.h"

// main-function for a thread
void* ThreadIntegrate(void* arg) {

    thread_arg_t* args = (thread_arg_t*) arg;

    double (*f)(double) = args->integrate_arg->function;

    double x_1 = args->integrate_arg->int_begin;
    double x_2 = args->integrate_arg->int_end;

    int N = args->integrate_arg->point_amount;

    // double sum = 0;
    
    double d = (x_2 - x_1) / N;

    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;

        pthread_mutex_lock(&args->workarea_arg->g_mutex);
        args->workarea_arg->sum += 0.5 * (f(a) + f(b)) * d;
        pthread_mutex_unlock(&args->workarea_arg->g_mutex);
    }

    printf("thread has been compleated\n");
    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s <function(x)>\n", argv[0]);
        return -1;
    } 

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
            "}\n", argv[1]);
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

    workarea_t workarea =  {
        .g_mutex = PTHREAD_MUTEX_INITIALIZER,
        .sum = 0
    };

    integrate_t int_arg_th1 = {
        .function = tmpfun,

        .int_begin = 0,
        .int_end = 500,
        .point_amount = 5000000
    };
    integrate_t int_arg_th2 = {
        .function = tmpfun,

        .int_begin = 500,
        .int_end = 1000,
        .point_amount = 5000000
    };

    thread_arg_t thread_args1 = {
        .integrate_arg = &int_arg_th1,
        .workarea_arg = &workarea
    };
    thread_arg_t thread_args2 = {
        .integrate_arg = &int_arg_th2,
        .workarea_arg = &workarea
    };

    pthread_t thread_id1, thread_id2;

    if (errno = pthread_create(&thread_id1, NULL, ThreadIntegrate, &thread_args1)) {
        perror("pthread_create");
        return 1;
    }
    if (errno = pthread_create(&thread_id2, NULL, ThreadIntegrate, &thread_args2)) {
        perror("pthread_create");
        return 1;
    }

    // wait for a thread [thread_id]
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    printf("result = %lf\n", workarea.sum);

    return 0;
}

//* threads:
//? pthread_exit()

//TODO: CMake
//TODO: другие способы синхронизации нитей

//TODO: соединить программу с использованием gnuplot
// --------------------------------------------------------------------------------------------
//TODO: using the threads to make the pragramm faster
//TODO: compare the speed of programm working with threads and without ones
//TODO: unit tests 

//TODO: придумать, как реализовать идею с коэффициентом точности

//* computering numerical value of integral by the threads
//* use the main thread to make a plot (using gnuplot call)