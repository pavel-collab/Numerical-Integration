#ifndef LIB_H_
#define LIB_H_

#include <pthread.h>

// --------------------------------------------------------------------------------------------
double f(double x);
// --------------------------------------------------------------------------------------------

typedef struct {
    pthread_mutex_t g_mutex;
    volatile double sum;
} workarea_t;

typedef struct {
    double (*function)(double);

    double int_begin;
    double int_end;

    int point_amount;
} integrate_t;

typedef struct {
    workarea_t* workarea_arg;
    integrate_t* integrate_arg;
} thread_arg_t;

double RightRec(const double x_1, const double x_2, const unsigned N);

double LeftRec(const double x_1, const double x_2, const unsigned N);

double Rec(const double x_1, const double x_2, const unsigned N);

double Trapez(const double x_1, const double x_2, const unsigned N, double (*f)(double));

double Sympson(const double x_1, const double x_2, const unsigned N);

#endif