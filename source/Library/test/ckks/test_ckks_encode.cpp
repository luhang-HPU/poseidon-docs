
#include <bits/stdc++.h>

#include "poseidon/seal/modulus.h"
#include "poseidon/seal/memorymanager.h"
#include "poseidon/seal/util/globals.h"
#include "poseidon/seal/util/ntt.h"
#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/util/debug.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteral ckks_param_literal{
            CKKS,
            14,
            13,
            40,
            5,
            0,
            0,
            {},
            {}
    };
    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31};//,31,31,31,31}; //
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);

    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none,false);
    CKKSEncoder enc(context);

    Plaintext plain;
    auto slot_num = ckks_param_literal.slot();
    vector<complex<double>> message;
    sample_random_complex_vector(message,slot_num);


    //gmp_printf("cc : %.Zd\n",two_pow_64_2.get_mpz_t());
    Plaintext plaintext;
    double scale = std::pow(2.0,100);
    enc.encode(message, scale,plaintext);
    vector<complex<double>> message2;
    printf("cc : %.10lf\n",plaintext.scale());

    enc.decode(plaintext,message2);
    for(auto i = 0; i < 16; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message,message2);





    return 0;
}
