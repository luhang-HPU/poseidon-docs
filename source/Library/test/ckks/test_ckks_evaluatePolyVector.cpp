

#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
#include "poseidon/keygenerator.h"
#include "poseidon/util/precision.h"
#include "poseidon/Evaluator.h"
#include "poseidon/util/debug.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;
double fii(double x)  {
    //return  1 / (exp(-x) + 1);
    return sin(6.283185307179586 * x) ;
    //return 0.0001;
    //return (x > 0) ? 1.0 : -1.0;
}
int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteral ckks_param_literal{
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
    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31, 31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31};//,31,31,31,31}; //

    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none,false);

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
    for(int i = 0; i < mat_size; i++){
        message[i] = complex<double>(0.79 - (double)i/mat_size,0.38 -(double)(i)/mat_size);
    }
    for(int i = 0; i < message.size(); i++){
        message[i].imag(0);
    }
    vector<complex<double>> message1(message.size());
    for (size_t i = 0; i < context.parameters_literal()->degree() >> 1; i++) {
        message1[i].real(fii(message[i].real())) ;
    }

    //
    //=====================poly init===================================
    auto a = -4.0;
    auto b = 4.0;
    auto deg = 64;
    printf("Evaluation of the function f(x) for even slots and g(x) for odd slots in the range [%0.2f, %0.2f] (degree of approximation: %d)\n", a, b, deg);
    auto approxF = util::Approximate(fii, a, b, deg);
    approxF.lead() = true;
    //auto approxG = util::Approximate(g, a, b, deg);
    vector <Polynomial> poly_v{approxF};
    vector<vector<int>> slotsIndex(1,vector<int>(context.parameters_literal()->degree() >> 1,0));
    vector<int> idxF(context.parameters_literal()->degree() >> 1);


    for(int i = 0; i < context.parameters_literal()->degree() >> 1; i++){
        idxF[i] = i;   // Index with all even slots
    }

    slotsIndex[0] = idxF; // Assigns index of all even slots to poly[0] = f(x)


    PolynomialVector polys(poly_v,slotsIndex);
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
    vector<uint32_t> rot_elemt;

    auto level_start = logQTmp.size() - 1;
    CKKSEncoder ckks_encoder(context);




//=====================keys  =========================
    //
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);

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


    auto scale_tmp = safe_cast<double>(ckks_param_literal.Q().back().value()) ;
    ckks_eva->multiply_const(ct,(2.0/(double)(b-a)),scale_tmp,ct,ckks_encoder);


    ckks_eva->rescale(ct,ct);
    Timestacs time;
    time.start();
    ckks_eva->evaluatePolyVector(ct,ct2,polys,ct.scale(),relinKeys,ckks_encoder);
    time.end();
    printf("ct scale : %.10lf\n",ct.scale());
    time.print_time("evaluatePolyVector time :");
    ckks_eva->read(ct2);
    dec.decrypt(ct2,plaintext2);
    ckks_encoder.decode(plaintext2,message2);


    auto ftt_inv = message2;
    for(auto i = 0; i < 8; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message1[i].real() ,message1[i].imag());
        printf("result  vec[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());


    }






    util::GetPrecisionStats(message1,message2);






//    poly *= poly2;
//    poly.dot_to_coeff();
//    poly.coeff_to_dot();

    return 0;
}
