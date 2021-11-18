#include <stdio.h>
#include <math.h>

#include "lib.h"

double RightRec(const double x_1, const double x_2, const unsigned N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + i * d) * d;
    }
    return sum;
}

double LeftRec(const double x_1, const double x_2, const unsigned N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + (i + 1) * d) * d;
    }
    return sum;
}

double Rec(const double x_1, const double x_2, const unsigned N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += f((a + b) / 2) * d;
    }
    return sum;
}

double Trapez(const double x_1, const double x_2, const unsigned N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += 0.5 * (f(a) + f(b)) * d;
    }
    return sum;
}

double Sympson(const double x_1, const double x_2, const unsigned N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += (d / 6) * (f(a) + f(b) + 4 * f((a + b) / 2));
    }
    return sum;
}

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

void* ThrTrapez(void* arg) {

    arg_t* args = (arg_t*) arg;

    double (*f)(double) = args->function;
    double x_1 = args->int_begin;
    double x_2 = args->int_end;

    int N = args->point_amount;

    double local_sum = 0;
    double d = (x_2 - x_1) / N;

    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;

        local_sum += 0.5 * (f(a) + f(b)) * d;
    }

    pthread_mutex_lock(&args->g_mutex);
    *(args->sum) += local_sum;
    pthread_mutex_unlock(&args->g_mutex);

    return NULL;
}

void* ThrRightRec(void* arg) {

    arg_t* args = (arg_t*) arg;

    double (*f)(double) = args->function;
    double x_1 = args->int_begin;
    double x_2 = args->int_end;

    int N = args->point_amount;

    double local_sum = 0;
    double d = (x_2 - x_1) / N;

    for (int i = 0; i < N; ++i) {
        local_sum+= f(x_1 + i * d) * d;
    }

    pthread_mutex_lock(&args->g_mutex);
    *(args->sum) += local_sum;
    pthread_mutex_unlock(&args->g_mutex);

    return NULL;
}

void* ThrLeftRec(void* arg) {

    arg_t* args = (arg_t*) arg;

    double (*f)(double) = args->function;
    double x_1 = args->int_begin;
    double x_2 = args->int_end;

    int N = args->point_amount;

    double local_sum = 0;
    double d = (x_2 - x_1) / N;

    for (int i = 0; i < N; ++i) {
        local_sum+= f(x_1 + (i + 1) * d) * d;
    }

    pthread_mutex_lock(&args->g_mutex);
    *(args->sum) += local_sum;
    pthread_mutex_unlock(&args->g_mutex);

    return NULL;
}

void* ThrRec(void* arg) {

    arg_t* args = (arg_t*) arg;

    double (*f)(double) = args->function;
    double x_1 = args->int_begin;
    double x_2 = args->int_end;

    int N = args->point_amount;

    double local_sum = 0;
    double d = (x_2 - x_1) / N;

    for (int i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        local_sum += f((a + b) / 2) * d;
    }

    pthread_mutex_lock(&args->g_mutex);
    *(args->sum) += local_sum;
    pthread_mutex_unlock(&args->g_mutex);

    return NULL;
}

void* ThrSympson(void* arg) {

    arg_t* args = (arg_t*) arg;

    double (*f)(double) = args->function;
    double x_1 = args->int_begin;
    double x_2 = args->int_end;

    int N = args->point_amount;

    double local_sum = 0;
    double d = (x_2 - x_1) / N;

    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        local_sum += (d / 6) * (f(a) + f(b) + 4 * f((a + b) / 2));
    }

    pthread_mutex_lock(&args->g_mutex);
    *(args->sum) += local_sum;
    pthread_mutex_unlock(&args->g_mutex);

    return NULL;
}