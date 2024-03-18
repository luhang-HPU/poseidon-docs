
#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
#include "poseidon/keygenerator.h"
#include "poseidon/util/precision.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteral aa{
            CKKS,
            11,
            10,
            40,
            5,
            0,
            0,
            {},
            {}
    };

//    vector<uint32_t> logQTmp{25,33,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31};//,31,31,31,31}; //
//    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31};//,31,31,31,31,31,31,31,31,31,31,31};  //
    vector<uint32_t> logQTmp{25,33,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31};//,31,31,31,31,31,
    aa.set_log_modulus(logQTmp,logPTmp);

    PublicKey public_key;
    PoseidonContext context(aa);
    CKKSEncoder enc(context);
    KeyGenerator keygen(context);
    keygen.create_public_key(public_key);
    Encryptor encryptor(context,public_key,keygen.secret_key());
    Decryptor decryptor(context,keygen.secret_key());
    Plaintext plain;
    auto slot_num = 1 << 10;
    vector<complex<double>> message;
    vector<complex<double>> message_zero(slot_num,0.0);

    sample_random_complex_vector(message,slot_num);


    //gmp_printf("cc : %.Zd\n",two_pow_64_2.get_mpz_t());
    Plaintext plaintext,plaintext2;
    double scale = std::pow(2.0,51);
    enc.encode(message, scale,plaintext);
    vector<complex<double>> message2;
    printf("scale : %.10lf\n",plaintext.scale());

    Ciphertext ct,ct2;
    encryptor.encrypt(plaintext,ct);
    decryptor.decrypt(ct,plaintext2);
    enc.decode(plaintext2,message2);
    for(auto i = 0; i < 8; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message,message2);
    encryptor.encrypt_zero(ct);
    decryptor.decrypt(ct,plaintext2);
    enc.decode(plaintext2,message2);

    for(auto i = 0; i < 8; i++){
        //printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("encrypt_zero[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message_zero,message2);

    encryptor.encrypt_zero(context.crt_context()->parms_id_map().at(3),ct2);
    decryptor.decrypt(ct2,plaintext2);
    enc.decode(plaintext2,message2);

    for(auto i = 0; i < 8; i++){
        //printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("encrypt_zero_id[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message_zero,message2);

    encryptor.encrypt_zero_symmetric(ct2);
    decryptor.decrypt(ct2,plaintext2);
    enc.decode(plaintext2,message2);

    for(auto i = 0; i < 8; i++){
        //printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("encrypt_zero_symmetric[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }


    return 0;
}
