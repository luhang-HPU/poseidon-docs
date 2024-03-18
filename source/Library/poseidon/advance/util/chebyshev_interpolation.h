//
// Created by lxs on 23-7-12.
//

#ifndef POSEIDON_CHEBYSHEV_INTERPOLATION_H
#define POSEIDON_CHEBYSHEV_INTERPOLATION_H


#include <complex>
#include <vector>
#include "poseidon/advance/polynomial_evaluation.h"
using namespace std;

namespace poseidon{
    enum SineType{
        CosDiscrete,
        SinContinuous,
        CosContinuous
    };
    namespace util{

        typedef double (*Fun_D)(double );
        typedef complex<double> (*Fun_Comp)(const complex<double> &);
        Polynomial Approximate(Fun_D fun,double a,double b,int degree);
        Polynomial Approximate(Fun_Comp fun,double a,double b,int degree);

        vector<double> chebyshevNodes(int n, double a, double b);
        vector<complex<double>> chebyCoeffs(const vector<double> &nodes , const vector<complex<double>> &fi, double a,double b );
    }
}

#endif //POSEIDON_CHEBYSHEV_INTERPOLATION_H
