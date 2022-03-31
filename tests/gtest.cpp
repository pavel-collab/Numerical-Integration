#include <iostream>
#include "../include/lib.h"
#include <cmath>
#include <gtest/gtest.h>

double f(double x) {
    return x;
}
double g(double x) {
    return 1/x;
}
double Laplass(double x) {
    return 0.3989422*exp(-x*x/2);
}

double RightRec(const double x_1, const double x_2, const unsigned N, double (*f)(double));
double LeftRec(const double x_1, const double x_2, const unsigned N, double (*f)(double));
double Rec(const double x_1, const double x_2, const unsigned N, double (*f)(double));
double Trapez(const double x_1, const double x_2, long int N, double (*f)(double));
double Sympson(const double x_1, const double x_2, const unsigned N, double (*f)(double));

TEST(NumericalIntegration, RightRec)
{
    double sum = RightRec(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 0.5);
}

TEST(NumericalIntegration, LeftRec) {
    double sum = LeftRec(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 0.5);
}

TEST(NumericalIntegration, Trapez) {
    double sum = Trapez(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 0.5);
}

TEST(NumericalIntegration, Sympson_method) {
    double sum = Sympson(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 0.5);
}

// --------------------------------------------------------------------------------------------
// Trapez

TEST(NumericalIntegration, Sin) {
    double sum = Trapez(0, 1.570796, 1000000, sin);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 1.0);
}

TEST(NumericalIntegration, Log) {
    double sum = Trapez(1, 10, 100000000, log);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 14.026);
}

TEST(NumericalIntegration, Garmonical_function) {
    double sum = Trapez(1, 200000, 100000000, g);
    sum = round(sum * 1000) / 1000;
    ASSERT_EQ(sum, 12.206);
}

TEST(NumericalIntegration, Exp) {
    double sum = Trapez(0, 10, 1000000000, exp);
    sum = round(sum * 10) / 10;
    ASSERT_EQ(sum, 22025.5);
}

TEST(NumericalIntegration, Sqrt) {
    double sum = Trapez(0.004, 1001, 10000000, sqrt);
    sum = round(sum * 100) / 100;
    ASSERT_EQ(sum, 21113.48);
}

TEST(NumericalIntegration, Asinh) {
    double sum = Trapez(0, 10, 1000000, asinh);
    sum = round(sum * 100) / 100;
    ASSERT_EQ(sum, 20.93);
}

// --------------------------------------------------------------------------------------------
// Trapez Laplass function

TEST(NumericalIntegration, Laplass) {
    double sum = Trapez(0, 0.05, 1000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.0199);

    sum = Trapez(0, 0.21, 1000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.0832);

    sum = Trapez(0, 0.42, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.1628);

    sum = Trapez(0, 0.91, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.3186);

    sum = Trapez(0, 1.15, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.3749);

    sum = Trapez(0, 1.25, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    ASSERT_EQ(sum, 0.3944);
}

// ===========================================================================================

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

// ===========================================================================================
