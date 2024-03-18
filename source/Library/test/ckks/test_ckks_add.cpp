
#include <bits/stdc++.h>

#include "poseidon/seal/modulus.h"
#include "poseidon/seal/memorymanager.h"
#include "poseidon/seal/util/globals.h"
#include "poseidon/seal/util/ntt.h"
#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
#include "poseidon/keygenerator.h"
#include "poseidon/util/precision.h"
#include "poseidon/Evaluator.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteralDefault aa(CKKS,2048,poseidon::sec_level_type::tc192);
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys galoisKeys;
    PoseidonContext context(aa,sec_level_type::tc192,false);


    CKKSEncoder enc(context);
    KeyGenerator keygen(context);
    keygen.create_public_key(public_key);
    Encryptor encryptor(context,public_key);
    Decryptor decryptor(context,keygen.secret_key());
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);

    Plaintext plain;
    auto slot_num = aa.slot();
    vector<complex<double>> message;
    sample_random_complex_vector(message,slot_num);


    //gmp_printf("cc : %.Zd\n",two_pow_64_2.get_mpz_t());
    Plaintext plaintext,plaintext2;

    enc.encode(message, aa.scale(),plaintext);
    vector<complex<double>> message2;
    printf("scale : %.10lf\n",plaintext.scale());

    Ciphertext ct,ct2;
    encryptor.encrypt(plaintext,ct);
    encryptor.encrypt(plaintext,ct2);

    ckks_eva->add(ct,ct2,ct2);
    decryptor.decrypt(ct2,plaintext2);
    enc.decode(plaintext2,message2);
    for(auto &m : message){
        m += m;
    }
    for(auto i = 0; i < 8; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message,message2);

    return 0;
}
