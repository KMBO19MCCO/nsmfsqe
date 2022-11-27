//
// Created by assasinfil on 04.10.2022.
//

#include <cmath>
#include <iostream>
#include <omp.h>
#include "excerpt.h"
#include "Timer.h"

#define MAX_DISTANCE 10e-5

using namespace std;

/*
  diff_of_products() computes a*b-c*d with a maximum error <= 1.5 ulp

  Claude-Pierre Jeannerod, Nicolas Louvet, and Jean-Michel Muller,
  "Further Analysis of Kahan's Algorithm for the Accurate Computation
  of 2x2 Determinants". Mathematics of Computation, Vol. 82, No. 284,
  Oct. 2013, pp. 2245-2264
*/
template<typename fp_t>
auto diff_of_products(fp_t a, fp_t b, fp_t c, fp_t d) {
    auto w = -d * c;
    auto e = fma(d, c, w);
    auto f = fma(a, b, w);
    return f + e;
}

/* compute the real roots of a quadratic equation: ax² + bx + c = 0,
   provided the discriminant b²-4ac is positive
*/
template<typename fp_t>
void solve_quadratic(fp_t a, fp_t b, fp_t c, fp_t *x0, fp_t *x1) {
    auto q = -0.5 * (b + copysign(sqrt(diff_of_products<fp_t>(b, b, 4.0 * a, c)), b));
    *x0 = q / a;
    *x1 = c / q;
}

template<typename fp_t>
fp_t testPolynomial(unsigned int roots_count) {
    fp_t x0, x1, max_absolute_error, max_relative_error;
    vector<fp_t> roots(roots_count), coefficients(roots_count + 1);
    generate_polynomial<fp_t>(roots_count, 0, roots_count, 0, MAX_DISTANCE, -1, 1, roots, coefficients);
    solve_quadratic<fp_t>(coefficients[2], coefficients[1], coefficients[0], &x0, &x1);
    vector<fp_t> roots_computed = {x1, x0};
    if (isnan(x1) and isnan(x0)) return 0.0;
    auto result = compare_roots<fp_t>(roots_computed.size(), roots.size(), roots_computed, roots, max_absolute_error,
                                      max_relative_error);
    switch (result) {
        case PR_2_INFINITE_ROOTS:
            cout << "INFINITE ROOTS";
            break;
        case PR_AT_LEAST_ONE_ROOT_IS_FAKE:
            cout << "AT LEAST ONE ROOT IS FAKE";
            break;
        case PR_AT_LEAST_ONE_ROOT_LOST:
            cout << "AT LEAST ONE ROOT LOST";
            break;
        default:
            break;
    }
    return max_absolute_error;
}


typedef float fp_t;

int main() {
    fp_t max_deviation = 0;
    auto cores = omp_get_num_procs();
    auto *deviations = new fp_t[cores];

#pragma omp parallel for
    for (auto i = 0; i < cores; ++i) {
        deviations[i] = 0;
    }

    cout << "Threads: " << cores << endl;
    cout << "Started" << endl;
    auto timer = Timer("compute");
    timer.start();

#pragma omp parallel for
    for (auto i = 0; i < 1000'1000; ++i) {
        auto thread_id = omp_get_thread_num();
        auto deviation = testPolynomial<fp_t>(2);
        if (deviation > deviations[thread_id] and deviation != numeric_limits<fp_t>::infinity()) {
            deviations[thread_id] = deviation;
        }
    }

    cout << "Computed, started searching" << endl;
    timer.stop();
    for (auto i = 0; i < cores; ++i) {
        if (deviations[i] > max_deviation) {
            max_deviation = deviations[i];
        }
    }
    cout << "Max deviation: " << max_deviation << endl;
    delete[] deviations;
    return 0;
}