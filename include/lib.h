#ifndef LIB_H_
#define LIB_H_

#include <pthread.h>

typedef struct {
    pthread_mutex_t g_mutex;
    volatile double* sum;

    double (*function)(double);
    double int_begin;
    double int_end;

    int point_amount;
} arg_t;

double RightRec(const double x_1, const double x_2, const unsigned N, double (*f)(double));

double LeftRec(const double x_1, const double x_2, const unsigned N, double (*f)(double));

double Rec(const double x_1, const double x_2, const unsigned N, double (*f)(double));

double Trapez(const double x_1, const double x_2, const long int N, double (*f)(double));

double Sympson(const double x_1, const double x_2, const unsigned N, double (*f)(double));

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

void* ThrTrapez(void* arg);

void* ThrRightRec(void* arg);

void* ThrLeftRec(void* arg);

void* ThrRec(void* arg);

void* ThrSympson(void* arg);

#endif