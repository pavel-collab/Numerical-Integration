#define CATCH_CONFIG_MAIN
#include <iostream>

#include "catch.hpp"

double f(double x) {
    return x;
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

TEST_CASE("Right rectangular") {
    double sum = RightRec(0, 1, 1000);
    REQUIRE(sum == 0.5);
}

TEST_CASE("Left rectangular") {
    double sum = LeftRec(0, 1, 1000);
    REQUIRE(sum == 0.5);
}

TEST_CASE("Trapez") {
    double sum = Trapez(0, 1, 1000);
    REQUIRE(sum == 0.5);
}

TEST_CASE("Sympson method") {
    double sum = Sympson(0, 1, 1000);
    REQUIRE(sum == 0.5);
}