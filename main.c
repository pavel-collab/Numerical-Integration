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

//TODO: лютый рефактор
//TODO: уменьшить накладные расходы
//TODO: переименовать функции, переменные и файлы

#include "lib.h"

// main-function for a thread
void* ThreadIntegrate(void* arg) {

    thread_arg_t* args = (thread_arg_t*) arg;

    double (*f)(double) = args->integrate_arg->function;

    double x_1 = args->integrate_arg->int_begin;
    double x_2 = args->integrate_arg->int_end;

    int N = args->integrate_arg->point_amount;

    double local_sum = 0;
    
    double d = (x_2 - x_1) / N;

    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;

        //! использовать для каждого потока отдельную облать памяти, чтобы нити не ждали друг друга
        //! конечный результат складывать в основном потоке
        local_sum += 0.5 * (f(a) + f(b)) * d;
    }

    pthread_mutex_lock(&args->workarea_arg->g_mutex);
    args->workarea_arg->sum += local_sum;
    pthread_mutex_unlock(&args->workarea_arg->g_mutex);

    printf("thread has been compleated\n");
    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Usage: %s <integration limits> <function(x)>\n", argv[0]);
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
            "}\n", argv[3]);
    close(gcc_pipe[1]);

    //! большое время ожидания может быть здесь, пока родиель ждет завершения работы ребенка
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

    // for all threads
    workarea_t workarea =  {
        .g_mutex = PTHREAD_MUTEX_INITIALIZER,
        .sum = 0
    };

    // integration limits
    double a = atof(argv[1]);
    double b = atof(argv[2]);

    // allocate memory for arrays of arguments
    thread_arg_t* thread_args = (thread_arg_t*) calloc(4, sizeof(thread_arg_t));
    if (thread_args == NULL) {
        perror("calloc");
        return -1;
    }

    integrate_t* int_args = (integrate_t*) calloc(4, sizeof(integrate_t));
    if (int_args == NULL) {
        perror("calloc");
        return -1;
    }

    // in loop define arrays of qrgs
    for (int i = 0; i < 4; i++) {
        integrate_t i_arg = {
            .function = tmpfun,

            .int_begin = a + ((b - a) / 4) * i,
            .int_end = a + ((b - a) / 4) * (i + 1),
            .point_amount = 5000000
        };

        int_args[i] = i_arg;

        thread_arg_t t_args = {
            .integrate_arg = int_args + i,
            .workarea_arg = &workarea
        };

        thread_args[i] = t_args;
    }

    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_t thread_id3;
    pthread_t thread_id4;

    if (errno = pthread_create(&thread_id1, NULL, ThreadIntegrate, thread_args)) {
        perror("pthread_create");
        return 1;
    }
    if (errno = pthread_create(&thread_id2, NULL, ThreadIntegrate, thread_args + 1)) {
        perror("pthread_create");
        return 1;
    }
    if (errno = pthread_create(&thread_id3, NULL, ThreadIntegrate, thread_args + 2)) {
        perror("pthread_create");
        return 1;
    }
    if (errno = pthread_create(&thread_id4, NULL, ThreadIntegrate, thread_args + 3)) {
        perror("pthread_create");
        return 1;
    }

    // wait for a thread [thread_id]
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);
    pthread_join(thread_id4, NULL);

    printf("result = %lf\n", workarea.sum);

    return 0;
}

//! разделяю интегрирование на 4 потока, но программа с потоками работает в 4 раза медленнее
//! чем программа без потоков
//! вероятно это связано с большими затратами на системные вызовы и преимущество будет заметно
//! при больших промежутках и/или на более сложных подинтегральных функциях

//! подумать, как уменьшить накладные расходы:: calloc -> придумать, как генерить аргументы для потоков

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