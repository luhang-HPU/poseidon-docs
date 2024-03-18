#ifndef RANDOM_SAMPLE_H
#define RANDOM_SAMPLE_H

#include <cmath>
//#include <gmpxx.h>
#include <complex>
#include <random>
#include<vector>
#include<complex>
#include<vector>
#include<complex>
#include<algorithm>


using namespace std;
namespace poseidon{
    //v1.0
    int sample_hamming_weight_vector(int *sample, int length, int hamming_weight);
    //int sample_uniform(const mpz_t min_val,const mpz_t max_val,mpz_t *uniform_res, int num_samples);
    int sample_triangle(int *uniform_res,int num_samples);
    void sample_random_complex_vector(vector<complex<double>> &vec,int length);
    void sample_random_vector(vector<uint32_t> &vec,int length,int max);
    void sample_random_complex_vector2(std::vector< complex<double> > &vec,int length);






}
#endif