//
// Created by lxs on 23-7-12.
//
#include "poseidon/advance/polynomial_evaluation.h"
namespace poseidon {

    //
    tuple<Polynomial, Polynomial> splitCoeffs(const Polynomial &coeffs, int split) {

        vector<complex<double>> coeffsr_buffer(split);
        vector<complex<double>> coeffsq_buffer(coeffs.degree() - split + 1);
        auto coeffsq_max_degree = coeffs.max_degree();
        auto coeffsr_max_degree = coeffs.max_degree();
        //sr
        if (coeffs.max_degree() == coeffs.degree()) {
            coeffsr_max_degree = split - 1;
        } else {
            coeffsr_max_degree = coeffs.max_degree() - (coeffs.degree() - split + 1);
        }
        for (int i = 0; i < split; i++) {
            coeffsr_buffer[i] = coeffs.data()[i];
        }
        //sq
        coeffsq_buffer[0] = coeffs.data()[split];
        if (coeffs.basis_type() == Monomial) {
            for (auto i = split + 1; i < coeffs.degree() + 1; i++) {
                coeffsq_buffer[i - split] = coeffs.data()[i];
            }
        } else if (coeffs.basis_type() == Chebyshev) {
            auto j = 1;
            for (auto i = split + 1; i < coeffs.degree() + 1; i++) {
                coeffsq_buffer[i - split] = complex<double>(2.0, 0.0) * coeffs.data()[i];
                coeffsr_buffer[split - j] -= coeffs.data()[i];
                j++;
            }
        }

        bool lead = coeffs.lead();
        Polynomial coeffsq(coeffsq_buffer, coeffs.a(), coeffs.b(), coeffsq_max_degree, coeffs.basis_type(),lead);
        Polynomial coeffsr(coeffsr_buffer, coeffs.a(), coeffs.b(), coeffsr_max_degree, coeffs.basis_type(),false);

        return make_tuple(coeffsq, coeffsr);
    }

    void splitCoeffsPolyVector(const PolynomialVector &polys, PolynomialVector &coeffsq, PolynomialVector &coeffsr, int split){
        for(int i = 0; i < polys.polys().size(); i++){
            auto [a,b] = splitCoeffs(polys.polys()[i],split);
            coeffsq.polys().push_back(a);
            coeffsr.polys().push_back(b);
        }
    }


}