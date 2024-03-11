#  CKKS Basic
```cpp
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

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteralDefault aa(CKKS,4096,poseidon::sec_level_type::none);
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys galoisKeys;
    PoseidonContext context(aa,poseidon::sec_level_type::none);

    CKKSEncoder enc(context);
    KeyGenerator keygen(context);
    keygen.create_public_key(public_key);
    keygen.create_relin_keys(relinKeys);
    keygen.create_galois_keys(vector<int>{0,1},galoisKeys);


    Encryptor encryptor(context,public_key);
    Decryptor decryptor(context,keygen.secret_key());
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);

    auto slot_num = aa.slot();
    vector<complex<double>> message,message2;
    vector<complex<double>>message_want(slot_num);
    vector<complex<double>> message_res;

    sample_random_complex_vector(message,slot_num);
    sample_random_complex_vector(message2,slot_num);

    Plaintext plaintext,plaintext2,plaintext_res;
    double scale = std::pow(2.0,51);
    enc.encode(message, scale,plaintext);
    enc.encode(message2, scale,plaintext2);

    Ciphertext ct,ct2,ct_res;
    encryptor.encrypt(plaintext,ct);
    encryptor.encrypt(plaintext2,ct2);

    Timestacs timestacs;
//======================== ADD =========================================
    print_example_banner("Example: ADD / ADD in ckks");
    timestacs.start();
    ckks_eva->add(ct,ct2,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("ADD TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] += message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


//======================== SUB =========================================
    print_example_banner("Example: SUB / SUB in ckks");
    timestacs.start();
    ckks_eva->sub(ct,ct2,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);
    timestacs.print_time("SUB TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] -= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== ROTATE =========================================
    print_example_banner("Example: ROTATE / ROTATE in ckks");
    timestacs.start();
    ckks_eva->rotate(ct,1,galoisKeys,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("ROTATE TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] = message[i];
    }
    std::rotate(message_want.begin(),message_want.begin()+1,message_want.end());

    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== conj =========================================
    print_example_banner("Example: CONJUGATE / CONJUGATE in ckks");
    timestacs.start();
    ckks_eva->conjugate(ct,galoisKeys,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("CONJUGATE TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] = std::conj(message[i]);
    }


    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== ADD_PLAIN =========================================
    print_example_banner("Example: ADD_PLAIN / ADD_PLAIN in ckks");
    timestacs.start();
    ckks_eva->add_plain(ct,plaintext_res,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);
    timestacs.print_time("ADD_PLAIN TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] += message[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== MULT_PLAIN =========================================
    print_example_banner("Example: MULT_PLAIN / MULT_PLAIN in ckks");


    Ciphertext tmp,tmp2;
    timestacs.start();
    tmp = ct;
    timestacs.end();
    timestacs.print_time("copy_tmp TIME: ");
    timestacs.start();
    tmp.resize(context,ct.parms_id(),2);
    timestacs.end();
    timestacs.print_time("resize_tmp TIME: ");

    timestacs.start();
    ckks_eva->multiply_plain(ct,plaintext_res,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);
    timestacs.print_time("MULT_PLAIN TIME: ");

    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== RESCALE =========================================
    print_example_banner("Example: RESCALE / RESCALE in ckks");
    timestacs.start();
    ckks_eva->rescale(ct_res,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);


    timestacs.print_time("RESCALE TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== MULTIPLY_RELIN_DYNAMIC =========================================
    print_example_banner("Example: MULTIPLY_RELIN_DYNAMIC / MULTIPLY_RELIN_DYNAMIC in ckks");
    timestacs.start();
    ckks_eva->multiply_relin_dynamic(ct_res,ct2,ct_res,relinKeys);
    timestacs.end();
    ckks_eva->rescale(ct_res,ct_res);
    ckks_eva->read(ct_res);


    timestacs.print_time("MULTIPLY_RELIN_DYNAMIC TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== MULT_CONST =========================================
    print_example_banner("Example: MULT_CONST_DIRECT/ MULT_CONST_DIRECT in ckks");
    timestacs.start();
    ckks_eva->multiply_const_direct(ct_res,2,ct_res,enc);
    ckks_eva->read(ct_res);

    timestacs.end();
    timestacs.print_time("MULT_CONST_DIRECT TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= 2;
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== MULT_CONST =========================================
    print_example_banner("Example: MULT_CONST / MULT_CONST in ckks");
    timestacs.start();
    ckks_eva->multiply_const(ct_res,-5.0, 1.0,ct_res,enc);
    ckks_eva->read(ct_res);

    timestacs.end();
    timestacs.print_time("MULT_CONST TIME: ");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= -5;
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    return 0;
}


```


# Evaluate Poly
```cpp
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
    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31}; //,31,31,31,31,31,31,31,31,31,31
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31};

    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none);

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
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);
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

    return 0;
}

```


# Matrix vector multiplication

```cpp
#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
#include "poseidon/keygenerator.h"
#include "poseidon/util/precision.h"
#include "poseidon/Evaluator.h"
#include "poseidon/RNSPoly.h"
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
            13,
            12,
            40,
            5,
            0,
            0,
            {},
            {}
    };

//    vector<uint32_t> logQTmp{25,33,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31};//,31,31,31,31}; //
//    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31};//,31,31,31,31,31,31,31,31,31,31,31};  //
    vector<uint32_t> logQTmp{31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31,31,31};//,
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
//    PoseidonContext context(ckks_param_literal);
    PoseidonContext context(ckks_param_literal,sec_level_type::none);


    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result,vec_result1;
    int mat_size = 1 << ckks_param_literal.LogSlots();
    std::vector<vector<std::complex<double>>> mat(mat_size,vector<complex<double>>(mat_size,0));
    std::vector<vector<std::complex<double>>> mat_T(mat_size);//(mat_size,vector<complex<double>>(mat_size));
    std::vector<vector<std::complex<double>>> mat_T1;
    //create message
    vector<complex<double>> message(mat_size,0);
    sample_random_complex_vector2(message,mat_size);

    vector<complex<double>> message_tmp(mat_size);
    vector<complex<double>> message_sum(mat_size << 1,0.0);

    //=====================init  Plain & Ciph =========================
    Plaintext plainA,plainB,plainRes,plainRes1,plainT;
    Ciphertext cipherA,cipherB,cipherRes,cipherRes1,cipherRes2,cipherRes3;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys conjKeys;
    GaloisKeys rotKeys;
    vector<uint32_t> rot_elemt;
    CKKSEncoder ckks_encoder(context);
    //=====================GenMatrices  ========================
    MatrixPlain matrixPlain;

    for(int i = 0; i < mat_size; i++){
        sample_random_complex_vector2(mat[i],mat_size);
    }
    auto &modulus = context.crt_context()->first_context_data()->coeff_modulus();
    int level = modulus.size() - 1;
    //matrix_operations::transpose_matrix(mat,mat_T1);
    for(int i = 0; i < mat.size(); i++){
        matrix_operations::diagonal(mat, i,mat_T[i]);
    }


    GenMatrixformBSGS(matrixPlain,matrixPlain.rot_index, ckks_encoder, mat_T,
                      level , safe_cast<double>(modulus.back().value()) , 1, ckks_param_literal.LogSlots());


//=====================keys  =========================
    //
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);

    kgen.create_relin_keys(relinKeys);

    kgen.create_galois_keys(matrixPlain.rot_index,rotKeys);


    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());



    //gmp_printf("cc : %.Zd\n",two_pow_64_2.get_mpz_t());
    Plaintext plaintext,plaintext2;
    double scale = std::pow(2.0,51);
    ckks_encoder.encode(message, scale,plaintext);
    vector<complex<double>> message2;
    printf("scale : %.10lf\n",plaintext.scale());

    Ciphertext ct,ct2;
    enc.encrypt(plaintext,ct);
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);
    ckks_eva->multiplyByDiagMatrixBSGS(ct,matrixPlain,cipherRes,rotKeys);
    ckks_eva->read(cipherRes);
    dec.decrypt(cipherRes,plaintext2);

    ckks_encoder.decode(plaintext2,message2);
    matrix_operations::matrix_vector_multiply(mat, message, message_tmp);
    for(auto i = 0; i < 8; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_tmp[i].real() ,message_tmp[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message2[i].real() ,message2[i].imag());
    }
    util::GetPrecisionStats(message_tmp,message2);
    return 0;
}
```
# Bootstrap
##  Method
<div style="text-align: justify">

Bootstrapping mainly consists of four steps: **ModRaise**, **CoeffToSlot**, **EvalMod**, **SlotToCoeff**
<br>

<center>

![b1](b1.png)
![b2](b2.png)
![bf](bf.png)

</center>

<style>
    img[alt="b1"]{
        width:500px;
    }
    img[alt="b2"]{
        width:500px;
    }
    img[alt="bf"]{
        width:450px;
    }
</style>

<br>

**<font color='red'>Optimization: Chebyshev </font>**

<center>

![b3](b3.png)
![b4](b4.png)
![b5](b5.png)

</center>

<style>
    img[alt="b3"]{
        width:300px;
    }
    img[alt="b4"]{
        width:300px;
    }
    img[alt="b5"]{
        width:600px;
    }
</style>
<br>
</div> 

## Code
```cpp

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

int main() {
    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    uint32_t q0_bit = 63;
    auto q_def = 40;

    ParametersLiteral ckks_param_literal{
            CKKS,
            15,
            14,
            40,
            1,
            1,
            0,
            {},
            {}
    };


    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31}; //,31,31,31,31,31,31,31,31,31,31
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31};
    //vector<uint32_t> logPTmp{60};//,
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none);
    auto q0 = context.crt_context()->q0();




    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result,vec_result1;
    std::vector<vector<std::complex<double>>> mat;
    int mat_size = 1 << ckks_param_literal.LogSlots();
    mat.resize(mat_size);
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    sample_random_complex_vector(message, mat_size);
    for(auto &m : message){
        m = sin(m);
    }

    //=====================init  Plain & Ciph =========================
    Plaintext plainA,plainB,plainRes,plainRes1,plainT;
    Ciphertext cipherA,cipherB,cipherRes,cipherRes1,cipherRes2,cipherRes3;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<uint32_t> rot_elemt;
    //

    CKKSEncoder ckks_encoder(context);

    //=====================EvalMod  ========================
    auto level_start = ckks_param_literal.Q().size() - 1;

    EvalModPoly evalModPoly(context, CosDiscrete,(uint64_t)1 << (q0_bit - 25) ,level_start-6,8, 3,16,0,30);

    auto scFac = evalModPoly.ScFac();
    double K = evalModPoly.K();
    auto qDiff = evalModPoly.QDiff();
    auto q0OverMessageRatio = exp2(round(log2((double)q0 / (double)evalModPoly.MessageRatio()) ) ) ;
    // If the scale used during the EvalMod step is smaller than Q0, then we cannot increase the scale during
    // the EvalMod step to get a free division by MessageRatio, and we need to do this division (totally or partly)
    // during the CoeffstoSlots step

    auto CoeffsToSlotsScaling = 1.0;
    CoeffsToSlotsScaling *= evalModPoly.qDiv() / (K * scFac * qDiff);

    auto SlotsToCoeffsScaling = ckks_param_literal.scale();
    SlotsToCoeffsScaling = SlotsToCoeffsScaling / ((double)evalModPoly.ScalingFactor() / (double)evalModPoly.MessageRatio() );
    //auto SlotsToCoeffsScaling = 1;


    HomomorphicDFTMatrixLiteral d(0, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(), level_start, vector<uint32_t>(3,1), true, CoeffsToSlotsScaling, false, 1);
    HomomorphicDFTMatrixLiteral x(1, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(),  7 , vector<uint32_t>(3,1), true, SlotsToCoeffsScaling, false, 1);
    LinearMatrixGroup mat_group;
    LinearMatrixGroup mat_group_dec;
    d.create(mat_group,ckks_encoder,2);
    x.create(mat_group_dec,ckks_encoder,1);
    //=====================keys  =========================
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    //rotKeys = kgen.create_galois_keys(mat_group.rot_index(),kgen.secret_key());
    //GaloisKeys rotKeys;
    kgen.create_galois_keys(mat_group.rot_index(),rotKeys);
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    //encode
    ckks_encoder.encode(message,(int64_t) 1 << q_def ,plainA);
    //encrypt
    enc.encrypt(plainA,cipherA);

    //evaluate
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);


    // scale the message to Delta = Q/MessageRatio
    auto start = chrono::high_resolution_clock::now();
    cout << "bootstraping start..."<< endl;

    ckks_eva->bootstrap(cipherA,cipherRes,evalModPoly,mat_group,mat_group_dec,relinKeys,rotKeys,ckks_encoder);
    ckks_eva->read(cipherRes);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    //decode & decrypt
    dec.decrypt(cipherRes,plainRes);
    ckks_encoder.decode(plainRes,vec_result);
    for(int i = 0; i < 10; i++){

        printf("source vec[%d] : %0.10f + %0.10f I \n",i,(real(message[i])), imag(message[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,(real(vec_result[i])), imag(vec_result[i]));
    }
    GetPrecisionStats(vec_result,message);
    return 0;
}
```
