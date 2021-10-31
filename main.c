//* /usr/bin/time -o time.log --verbose ./out

#include <stdio.h>
#include <math.h>

// коэффициент точности
const double eps = 0.000001;

double f(double x) {
    return x*x;
}

double RightRec(const double x_1, const double x_2, const unsigned N) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + i * d) * d;
    }
    return sum;
}

double LeftRec(const double x_1, const double x_2, const unsigned N) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        sum += f(x_1 + (i + 1) * d) * d;
    }
    return sum;
}

double Rec(const double x_1, const double x_2, const unsigned N) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += f((a + b) / 2) * d;
    }
    return sum;
}

double Trapez(const double x_1, const double x_2, const unsigned N) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += 0.5 * (f(a) + f(b)) * d;
    }
    return sum;
}

double Sympson(const double x_1, const double x_2, const unsigned N) {
    double sum = 0;
    double d = (x_2 - x_1) / N;
    for (unsigned i = 0; i < N; i++) {
        double a = x_1 + i * d;
        double b = x_1 + (i + 1) * d;
        sum += (d / 6) * (f(a) + f(b) + 4 * f((a + b) / 2));
    }
    return sum;
}

int main() {
    
    // пределы интегрирования
    int x_1 = 1;
    int x_2 = 2;
    // количество отрезков
    unsigned N = 1000;

    printf("Right rectangular method: ans = %lg\n", RightRec(x_1, x_2, N));
    printf("Left rectangular method: ans = %lg\n", LeftRec(x_1, x_2, N));
    printf("Left rectangular method: ans = %lg\n", Rec(x_1, x_2, N));
    printf("Trapez rectangular method: ans = %lg\n", Trapez(x_1, x_2, N));
    printf("Symson method: ans = %lg\n", Sympson(x_1, x_2, N));

    // -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    
    // начальное разбиение
    unsigned T = 10;
    unsigned i = 1;
    double diff;

    do {
        diff = fabs(Sympson(x_1, x_2, T * i) - Sympson(x_1, x_2, T * (i + 1)));
        i++;
    }
    while (diff > eps);
    printf("T = %d\n", T * i);

    return 0;
}

//TODO: read more about using dlopen dlsym dlclose
//TODO: watch the АКОС-18 using the dynamic libraries
//TODO: using the gnuplot to plot
//TODO: using the threads to make the pragramm faster
//TODO: compare the speed of programm working with threads and without ones
//TODO: unit tests 