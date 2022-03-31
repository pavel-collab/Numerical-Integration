#ifndef LIB_H_
#define LIB_H_

#include <pthread.h>

// стуктура аргумента, передаваемого, как параметр для каждого потока
typedef struct {
    pthread_mutex_t g_mutex;
    volatile double* sum;

    double (*function)(double);
    double int_begin;
    double int_end;

    int point_amount;
} arg_t;

// различные варианты функций численного интегрирования
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// метод правых прямоугольников
double RightRec(const double x_1, const double x_2, long int N, double (*f)(double));
// метод левых прямоугольников
double LeftRec(const double x_1, const double x_2, long int N, double (*f)(double));
// метод прямоугольников
double Rec(const double x_1, const double x_2, long int N, double (*f)(double));
// метод трапеций
double Trapez(const double x_1, const double x_2, const long int N, double (*f)(double));
// метод Симсона
double Sympson(const double x_1, const double x_2, long int N, double (*f)(double));

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// функции численного интегрирования для многопоточного алгоритма
void* ThrTrapez(void* arg);
void* ThrRightRec(void* arg);
void* ThrLeftRec(void* arg);
void* ThrRec(void* arg);
void* ThrSympson(void* arg);

// ===========================================================================================
// ===========================================================================================
// реализация функций

double RightRec(const double x_1, const double x_2, long int N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + i * d) * d;
    }
    return sum;
}

double LeftRec(const double x_1, const double x_2, long int N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + (i + 1) * d) * d;
    }
    return sum;
}

double Rec(const double x_1, const double x_2, long int N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += f((a + b) / 2) * d;
    }
    return sum;
}

double Trapez(const double x_1, const double x_2, long int N, double (*f)(double)) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += 0.5 * (f(a) + f(b)) * d;
    }
    return sum;
}

double Sympson(const double x_1, const double x_2, long int N, double (*f)(double)) {
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

#endif