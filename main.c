//* /usr/bin/time -o time.log --verbose ./out

#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#include "lib.h"

// main-function for a thread
void* ThreadIntegrate(void* arg) {

    thread_arg_t* args = (thread_arg_t*) arg;

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

int main() {
    // accuracy ratio
    const double eps = 0.000001;

    workarea_t workarea =  {
        .g_mutex = PTHREAD_MUTEX_INITIALIZER,
        .sum = 0
    };

    integrate_t int_arg_th1 = {
        .int_begin = 0,
        .int_end = 500,
        .point_amount = 5000000
    };
    integrate_t int_arg_th2 = {
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