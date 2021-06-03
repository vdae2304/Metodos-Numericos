#include <cstdio>
#include "matrix.h"
#include "interpolate.h"
using namespace num_met;
int main() {
    const int npts = 5;
    double x[] = {-3, -1, 2, 4, 7};
    double y[] = {1, 0, 0, 2, 1};

    printf("Coeficientes:\n");
    matrix<double> coeff = spline_interpolate(npts, x, y);
    for (int i = 0; i < coeff.rows(); ++i) {
        for (int j = 0; j < coeff.columns(); ++j)
            printf("%.6f ", coeff[i][j]);
        printf("\n");
    }

    printf("S(0) = %.6f\n", piecewise_polynomial(0.0, coeff));
    return 0;
}
