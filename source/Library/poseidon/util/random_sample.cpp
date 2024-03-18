#include "poseidon/util/random_sample.h"
namespace poseidon{
       int sample_hamming_weight_vector(int *sample, int length, int hamming_weight){

           int total_weight = 0;
           int index = 0;
           int r = 0;
           while(total_weight < hamming_weight){
               index = rand()%length;
               if(sample[index] == 0){
                   r = rand()%2;
                   if(r == 0){
                       sample[index] = -1;
                   }
                   else{
                       sample[index] = 1;
                   }
                   total_weight ++;
               }
           }

           return 0;
       }



/*

Args:
        min_val (mpz_t): Minimum value (inclusive).
        max_val (mpz_t): Maximum value (exclusive).
        num_samples (int): Number of samples to be drawn.
*/
//       int sample_uniform(const mpz_t min_val,const mpz_t max_val,mpz_t *uniform_res, int num_samples){
//           gmp_randstate_t state;
//           gmp_randinit_mt(state);
//           //gmp_randseed_ui(state,(unsigned int)(time(NULL)));
//           gmp_randseed_ui(state,1);
//           for(int i = 0; i < num_samples; i++){
//               mpz_init(uniform_res[i]);
//           }
//
//           mpz_t tmp;
//           mpz_init(tmp);
//           mpz_sub(tmp,max_val,min_val);
//           for(int i = 0; i < num_samples; i++){
//               mpz_urandomm(uniform_res[i],state,tmp);
//               mpz_add(uniform_res[i],uniform_res[i],min_val);
//           }
//
//           //mpz_clear(state);
//           mpz_clear(tmp);
//           return 0;
//
//       }

       int sample_triangle(int *uniform_res,int num_samples){
           int r = 0;
           //srand( (unsigned)time( NULL ));
           srand(1);
           for(int i = 0; i < num_samples; i++){
               r = rand()%4;
               if(r == 0){
                   uniform_res[i] = -1;
               }
               else if(r == 1){
                   uniform_res[i] = 1;
               }
               else{
                   uniform_res[i] = 0;
               }
           }

           return 0;
       }

       void sample_random_complex_vector(vector<complex<double>> &vec,int length){
           vec.resize(length);
           for(int i = 0;i < length;i++){
               vec[i].imag(rand()/(RAND_MAX+1.0));
               vec[i].real(rand()/(RAND_MAX+1.0));
           }
       }

    void sample_random_vector(vector<uint32_t> &vec,int length,int max){
        vec.resize(length);
        std::random_device rd;
        default_random_engine e(1);
        uniform_int_distribution<unsigned >u(0, max);
        for(int i = 0; i < length; i++){
            //vec.push_back(u(e));
            vec[i] = u(e);
        }
    }

       void sample_random_complex_vector2(std::vector< complex<double> > &vec,int length){

           std::vector<complex<double>> vec_tmp;
           double real_data = 0;
           double imag_data = 0;
           //vec_tmp.resize(length);
           for(int i = 0;i < length;i++){
               real_data = rand()/(RAND_MAX+1.0);
               imag_data = rand()/(RAND_MAX+1.0);
               std::complex<double> const_data(real_data,imag_data);
               vec_tmp.push_back(const_data);

           }
           vec.swap(vec_tmp);
       }

}
