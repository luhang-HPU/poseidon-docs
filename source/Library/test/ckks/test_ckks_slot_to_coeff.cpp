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
    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31,31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31,31,31,31,31};//,
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
//    PoseidonContext context(ckks_param_literal);
    PoseidonContext context(ckks_param_literal,sec_level_type::none,false);

    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result,vec_result1;
    std::vector<vector<std::complex<double>>> mat;
    int mat_size = 1 << ckks_param_literal.LogSlots();
    mat.resize(mat_size);
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message_tmp(mat_size);
    vector<complex<double>> message_sum(mat_size << 1,0.0);
    sample_random_complex_vector(message, mat_size);
//===================== coeff_to_slot compare data ==================
//    FFT fft(mat_size);
//    vector<complex<double>> ftt_inv,ftt_tt;
//    fft.embedding_inv(message,ftt_inv);
//    fft.embedding(ftt_inv,ftt_tt);


//    for(auto i = 0; i < 8; i++){
//        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
//        printf("result_data[%d] : %.10lf + %.10lf I\n",i,ftt_inv[i].real() ,ftt_inv[i].imag());
//        printf("result_data[%d] : %.10lf + %.10lf I\n",i,ftt_tt[i].real() ,ftt_tt[i].imag());
//
//    }

    //=====================init  Plain & Ciph =========================
    Plaintext plainA,plainB,plainRes,plainRes1,plainT;
    Ciphertext cipherA,cipherB,cipherRes,cipherRes1,cipherRes2,cipherRes3;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys conjKeys;
    GaloisKeys rotKeys;
    vector<uint32_t> rot_elemt;

    auto level_start = logQTmp.size() - 1;
    CKKSEncoder ckks_encoder(context);
    HomomorphicDFTMatrixLiteral d(0, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(),
                                  level_start, vector<uint32_t>(3,1), true, 1.0, false, 1);
    HomomorphicDFTMatrixLiteral x(1, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(),
                                  level_start-3, vector<uint32_t>(3,1), true, 1.0, false, 1);

    LinearMatrixGroup mat_group;
    LinearMatrixGroup mat_group2;

    d.create(mat_group,ckks_encoder,1);
    x.create(mat_group2,ckks_encoder,1);

//=====================keys  =========================
    //
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(mat_group.rot_index(),rotKeys);


    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());



    //gmp_printf("cc : %.Zd\n",two_pow_64_2.get_mpz_t());
    Plaintext plaintext,plaintext2,plaintext3;
    vector<complex<double>> message2,message3;
    double scale = std::pow(2.0,40);
    ckks_encoder.encode(message, scale,plaintext);
    printf("scale : %.10lf\n",plaintext.scale());
    Ciphertext ct,ct2;
    enc.encrypt(plaintext,ct);
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);
    ckks_eva->coeff_to_slot(ct,mat_group,cipherRes,cipherRes1,rotKeys,ckks_encoder);
    ckks_eva->slot_to_coeff(cipherRes,cipherRes1,mat_group2,cipherRes,rotKeys,ckks_encoder);
    ckks_eva->read(cipherRes);


    dec.decrypt(cipherRes,plaintext2);
    ckks_encoder.decode(plaintext2,message2);


    auto ftt_inv = message2;
    for(auto i = 0; i < 8; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message[i].real() ,message[i].imag());
        printf("resu  vec[%d]   : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }

    util::GetPrecisionStats(message,message2);



    return 0;
}
