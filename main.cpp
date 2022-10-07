//
// Created by assasinfil on 04.10.2022.
//

#include <cmath>
#include <iostream>
#include "excerpt.h"

using namespace std;

/*
  diff_of_products() computes a*b-c*d with a maximum error <= 1.5 ulp

  Claude-Pierre Jeannerod, Nicolas Louvet, and Jean-Michel Muller,
  "Further Analysis of Kahan's Algorithm for the Accurate Computation
  of 2x2 Determinants". Mathematics of Computation, Vol. 82, No. 284,
  Oct. 2013, pp. 2245-2264
*/
template<typename fp_t>
fp_t diff_of_products(fp_t a, fp_t b, fp_t c, fp_t d) {
    auto w = d * c;
    auto e = fma(-d, c, w);
    auto f = fma(a, b, -w);
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

int main() {
    unsigned p = 2;
    vector<float> roots(p);
    vector<float> coefficients(p + 1);
    auto result = generate_polynomial<float>(p, 0, 2, 0, 10.0 / 5, -10, 10, roots, coefficients);
    float x0, x1;
    solve_quadratic<float>(coefficients[0], coefficients[1], coefficients[3], &x0, &x1);
    vector<float> roots_computed = {x0, x1};
    cout << x0 << ' ' << x1 << endl;
    cout << roots[0] << ' ' << roots[1] << endl;
    float deviation = 0;
    auto result2 = compare_roots<float>(p, p, roots_computed, roots, deviation);
    cout << "deviation: " << deviation << endl;
    return 0;
}