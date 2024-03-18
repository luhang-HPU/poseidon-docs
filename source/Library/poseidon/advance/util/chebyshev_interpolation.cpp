//
// Created by lxs on 23-7-12.
//

#include "poseidon/advance/util/chebyshev_interpolation.h"
#include "poseidon/advance/polynomial_evaluation.h"
namespace poseidon{
    namespace util{

        Polynomial Approximate(Fun_Comp fun,double a,double b,int degree){
            vector<double> nodes;
            nodes = util::chebyshevNodes(degree,a,b);
            vector<complex<double>> fi(degree);
            for(int i = 0; i < nodes.size(); i++){
                fi[i] = fun(complex<double>(nodes[i],0));
            }
            auto buffer = util::chebyCoeffs(nodes,fi,a,b);
            Polynomial poly(buffer,a,b,degree-1,Chebyshev);
            return poly;
        }
        Polynomial Approximate(Fun_D fun,double a,double b,int degree){
            vector<double> nodes;
            nodes = util::chebyshevNodes(degree,a,b);
            vector<complex<double>> fi(degree);
            for(int i = 0; i < nodes.size(); i++){
                fi[i] = complex<double>(fun(nodes[i]),0);
            }

            auto buffer = util::chebyCoeffs(nodes,fi,a,b);
            Polynomial poly(buffer,a,b,degree-1,Chebyshev);
            return poly;
        }

        vector<double> chebyshevNodes(int n, double a, double b){
            vector<double> u(n,0);
            auto x = 0.5 * (a + b);
            auto y = 0.5 * (b - a);
            for(int k = 1; k < n + 1; k++){
                u[k - 1] = x + y * cos(((double)k - 0.5) * M_PI / (double)n);
            }
            return u;
        }

        vector<complex<double>> chebyCoeffs(const vector<double> &nodes , const vector<complex<double>> &fi, double a,double b ) {
            complex<double> u, Tprev, T, Tnext;
            size_t n = nodes.size();
            vector<complex<double>> coeffs(n);

            for (int i = 0; i < n; i++){
                u = complex<double>((2*nodes[i]-a-b)/(b-a), 0);
                Tprev = 1.0;
                T = u;
                for(int j = 0; j < n; j++) {
                    coeffs[j] += fi[i] * Tprev;
                    Tnext = complex<double>(2,0) *u*T - Tprev;
                    Tprev = T;
                    T = Tnext;
                }
            }

            coeffs[0] /= complex<double>(double(n), 0);
            for(int i = 1; i < n; i++){
                coeffs[i] *= (2.0 / complex<double>(double(n), 0));
            }
            return coeffs;
        }
    }

}

