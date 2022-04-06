//* /usr/bin/time -o time.log --verbose ./out
#define RESEARCH
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

    if (argc < 4) {
        printf("Usage: %s <integration limits> \"<function(x)>\"\n", argv[0]);
        return -1;
    } 

    // integration limits
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    
    // количество потоков
    int thread_amount = (argc == 5) ? atoi(argv[4]) : 2;

    //! отличная идея, но требует доработки
    //! возможно, рост количества потоков должен быть нелинейным
    // thread_amount = ((b - a) / 250000 > 1) ? trunc((b - a) / 250000) : 1;
    // printf("thread_amount = %d\n", thread_amount);

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

    volatile double main_sum = 0;
    // mutex init
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    arg_t thread_args[thread_amount];

    // в цикле задаем аргументы каждому процессу
    for (int i = 0; i < thread_amount; i++) {
        thread_args[i].g_mutex = mutex;
        thread_args[i].sum = &main_sum;
        thread_args[i].function = tmpfun;
        thread_args[i].int_begin = a + ((b - a) / thread_amount) * i;
        thread_args[i].int_end = a + ((b - a) / thread_amount) * (i + 1);
        thread_args[i].point_amount = 5000000;
    }

    pthread_t thread_id[thread_amount]; 

    #ifdef RESEARCH
    clock_t start = clock();
    #endif

    for (int i = 0; i < thread_amount; ++i) {
        if (errno = pthread_create(&thread_id[i], NULL, ThrTrapez, &thread_args[i])) {
            //! прибивать потоки и чистить ресурсы в случае аварийной остановки программы
            perror("pthread_create");
        return 1;
    }
    }

    // wait for a thread [thread_id]
    for (int i = 0; i < thread_amount; ++i) {
        pthread_join(thread_id[i], NULL);
    }

    #ifdef RESEARCH
    if (argc != 6) {
        printf("Error, no file to write data\n");
        return -1;
    }
    char* file_name = argv[5];
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    FILE* fd = fopen(file_name, "ab");
    if (fd == NULL) {
        printf("Wrong open file\n");
        return -1;
    }
    fprintf(fd, "%lf\n", seconds);
    fclose(fd);
    #endif

    #ifndef RESEARCH
    printf("result = %lf\n", main_sum);
    #endif

    return 0;
}