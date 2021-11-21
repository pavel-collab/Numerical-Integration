#define CATCH_CONFIG_MAIN
#include <iostream>

#include "catch.hpp"
#include "lib.h"
#include <cmath>

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
double Trapez(const double x_1, const double x_2, const unsigned N, double (*f)(double));
double Sympson(const double x_1, const double x_2, const unsigned N, double (*f)(double));

// --------------------------------------------------------------------------------------------

TEST_CASE("Right rectangular") {
    double sum = RightRec(0, 1, 1000000, f);

    // точность -- 3 знака после запятой
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 0.5);
}

TEST_CASE("Left rectangular") {
    double sum = LeftRec(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 0.5);
}

TEST_CASE("Trapez") {
    double sum = Trapez(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 0.5);
}

TEST_CASE("Sympson method") {
    double sum = Sympson(0, 1, 1000000, f);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 0.5);
}

// --------------------------------------------------------------------------------------------
// Trapez

TEST_CASE("Sin") {
    double sum = Trapez(0, 1.570796, 1000000, sin);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 1.0);
}

TEST_CASE("Tan") {
    double sum = Trapez(0, 1000, 1000000, tan);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 272.002);
}

TEST_CASE("Log") {
    double sum = Trapez(1, 10, 100000000, log);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 14.025);
}

TEST_CASE("1/x") {
    double sum = Trapez(1, 200000, 100000000, g);
    sum = round(sum * 1000) / 1000;
    REQUIRE(sum == 12.206);
}

TEST_CASE("Exp") {
    double sum = Trapez(0, 10, 1000000, exp);
    sum = round(sum * 100) / 100;
    REQUIRE(sum == 22025.46);
}

TEST_CASE("Sqrt") {
    double sum = Trapez(0.004, 1001, 10000000, sqrt);
    sum = round(sum * 100) / 100;
    REQUIRE(sum == 21113.48);
}

TEST_CASE("Asinh") {
    double sum = Trapez(0, 10, 1000000, asinh);
    sum = round(sum * 100) / 100;
    REQUIRE(sum == 20.93);
}

// --------------------------------------------------------------------------------------------
// Trapez Laplass function

TEST_CASE("Laplass t = 0.05") {
    double sum = Trapez(0, 0.05, 1000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.0199);
}

TEST_CASE("Laplass t = 0.21") {
    double sum = Trapez(0, 0.21, 1000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.0832);
}

TEST_CASE("Laplass t = 0.42") {
    double sum = Trapez(0, 0.42, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.1628);
}

TEST_CASE("Laplass t = 0.91") {
    double sum = Trapez(0, 0.91, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.3186);
}

TEST_CASE("Laplass t = 1.15") {
    double sum = Trapez(0, 1.15, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.3749);
}

TEST_CASE("Laplass t = 1.25") {
    double sum = Trapez(0, 1.25, 10000000, Laplass);
    sum = round(sum * 10000) / 10000;
    REQUIRE(sum == 0.3944);
}

// ===========================================================================================
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

// ===========================================================================================