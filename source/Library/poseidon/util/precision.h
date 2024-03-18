//
// Created by lxs on 8/25/23.
//

#ifndef POSEIDON_PRECISION_H
#define POSEIDON_PRECISION_H
#include <vector>
#include <complex>
#define PRECISION_TITLE \
"┌─────────┬───────┬───────┬───────┐\n"  \
"│    Log2 │ REAL  │ IMAG  │ L2    │\n"  \
"├─────────┼───────┼───────┼───────┤\n"  \
"│MIN Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
"│MAX Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
"│AVG Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
"└─────────┴───────┴───────┴───────┘\n"  \



using namespace std;

namespace poseidon {
    namespace util{


    struct PrecisionStats{
        double Real;
        double Imag;
        double L2;
    };
    void GetPrecisionStats(vector<complex<double>> value_test,vector<complex<double>> value_want);


    }
} // poseidon

#endif //POSEIDON_PRECISION_H
