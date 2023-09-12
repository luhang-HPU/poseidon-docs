
#  Encode/Decode, Encrypt/Decrypt  
**main.cpp**
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <poseidon/Release/ParametersLiteral.h>
#include <poseidon/Release/PoseidonContext.h>
#include <poseidon/Release/define.h>
#include <poseidon/Release/BatchEncoder.h>
#include <poseidon/Release/KeyGenerator.h>
#include <poseidon/Release/Encryptor.h>
#include <poseidon/Release/Decryptor.h>
#include <poseidon/Release/CKKSEncoder.h>

using namespace std;

// Make use of the Poseidon namespace for easier access to its classes and methods.
using namespace poseidon;

int main() {

    // Initialize CKKS parameters using default literal and given degree (8192).
    CKKSParametersLiteralDefault param_literal(degree_8192);
    PoseidonContext context(param_literal);

    // Calculate the degree based on the logarithmic value provided by param_literal.
    auto degree = 1 << param_literal.LogN;

    // Declare public key, plaintexts and ciphertext variables.
    PublicKey public_key;
    Plaintext plainA, plainRes;
    Ciphertext ciphA;

    // Initialize the key generator with the given context.
    KeyGenerator kgen(context);

    // Generate the public key.
    kgen.create_public_key(public_key);

    // Initialize the encryptor and decryptor using the public and secret keys respectively.
    Encryptor enc1(context, public_key, kgen.secret_key());
    Decryptor dec1(context, kgen.secret_key());

    // Random engine for generating random values.
    default_random_engine e;

    //===================== Initialize random data ============================
    
    // Vector to store complex double values.
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;

    // Matrix to store vectors of complex double values.
    std::vector<vector<std::complex<double>>> mat;

    // Determine the size of the vector based on logarithmic slot values.
    int vec_size = 1 << param_literal.LogSlots;
    mat.resize(vec_size);

    // Message vectors to store complex double values.
    vector<complex<double>> message;
    vector<complex<double>> message1(vec_size);

    // Populate the message vector with random complex values of determined size.
    sample_random_complex_vector(message, vec_size);

    // Initialize the CKKS encoder with the provided context.
    CKKSEncoder ckksenc(context);

    // Encode the message into a plaintext using the scaling factor from the context.
    ckksenc.encode(message, plainA, context.scaling_factor());

    // Encrypt the plaintext and store the result in a ciphertext.
    enc1.encrypt(plainA, ciphA);

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec1.decrypt(ciphA, plainRes);

    // Decode the plaintext back into the message vector.
    ckksenc.decode(plainRes, message1);

    // Display the first 10 values of the source and result vectors for comparison.
    for(int i = 0; i < 10; i++){
        printf("source vec[%d] : %.10lf \n", i, message[i]);
        printf("result vec[%d] : %.10lf \n", i, message1[i]);
    }
    
    return 0;
}


```



#  Bootstraping
**main.cpp**
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <iostream>
#include <fstream>   
#include "poseidon/Release/define.h"
#include "poseidon/Release/homomorphic_DFT.h"
#include "poseidon/Release/linear_transform.h"

#include <cstdlib>
#include <vector>
#include <cmath>
#include <complex>
#include "poseidon/Release/util/number_theory.h"
#include "poseidon/Release/hardware/ConfigGen.h"
#include <gmpxx.h>
#include "poseidon/Release/Ciphertext.h"

#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/BatchEncoder.h"
#include "poseidon/Release/random/random_sample.h"


#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/ParametersLiteral.h"
#include "poseidon/Release/rlwe.h"
#include "poseidon/Release/RelinKeys.h"

#include "poseidon/Release/HardwareEvaluator.h"
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/homomorphic_mod.h"
#include "poseidon/Release/util/precision.h"
using  namespace  poseidon;

//===================== boostrap ======================================
int main(){
    
    //Define constants for the bootstrap process
    uint32_t q0_bit = 63;
    auto q_def = 31;
    vector<uint32_t> logQTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31}; 
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31};
    ParametersLiteral ckks_param_literal(CKKS, 11, 10, logQTmp, logPTmp, q_def, 30,0,1);

    //=====================config======================================
    PoseidonContext context(ckks_param_literal);
    auto q0 = context.crt_context()->q0();
    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result,vec_result1;
    std::vector<vector<std::complex<double>>> mat;
    int mat_size = 1 << ckks_param_literal.LogSlots;
    mat.resize(mat_size);

    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    sample_random_complex_vector(message, mat_size);
    //=====================init  Plain & Ciph =========================
    Plaintext plainA,plainB,plainRes;
    Ciphertext cipherA,cipherB,cipherRes;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<uint32_t> rot_elemt;
    //

    CKKSEncoder ckks_encoder(context);

    //=====================EvalMod  ========================
    auto level_start = ckks_param_literal.LogQ.size() - 1;

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
    auto SlotsToCoeffsScaling = (double)context.scaling_factor().get_ui();
    SlotsToCoeffsScaling = SlotsToCoeffsScaling / ((double)evalModPoly.ScalingFactor().get_ui() / (double)evalModPoly.MessageRatio() );

    HomomorphicDFTMatrixLiteral d(0, ckks_param_literal.LogN, ckks_param_literal.LogSlots, level_start, vector<int>(3,1), true, CoeffsToSlotsScaling, false, 1);
    HomomorphicDFTMatrixLiteral x(1, ckks_param_literal.LogN, ckks_param_literal.LogSlots,  8 , vector<int>(3,1), true, SlotsToCoeffsScaling, false, 1);
    LinearMatrixGroup mat_group;
    LinearMatrixGroup mat_group_dec;
    d.create(mat_group,ckks_encoder,2);
    x.create(mat_group_dec,ckks_encoder,1);


    //=====================keys  =========================
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(mat_group.rot_index(),rotKeys);
    kgen.create_conj_keys(conjKeys);

    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    //encode
    ckks_encoder.encode(message,plainA,(int64_t) 1 << q_def );
    //encrypt
    enc.encrypt(plainA,cipherA);


    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);


    auto start = chrono::high_resolution_clock::now();
    ckks_eva->bootstrap(cipherA,cipherRes,evalModPoly,mat_group,mat_group_dec,relinKeys,rotKeys,conjKeys,ckks_encoder);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;
    ckks_eva->read(cipherRes);
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

# Using Chebyshev Polynomial Evaluation in Heart Disease Prediction
**main.cpp**
```cpp
// Include necessary Poseidon library headers for different functionalities.

#include <iostream>
#include <fstream>   
#include <cstdlib>
#include <vector>
#include <cmath>
#include <complex>
#include <math.h>
#include <gmpxx.h>

#include "poseidon/Release/define.h"
#include "poseidon/Release/homomorphic_DFT.h"
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/util/number_theory.h"
#include "poseidon/Release/hardware/ConfigGen.h"

#include "poseidon/Release/Ciphertext.h"

#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/BatchEncoder.h"
#include "poseidon/Release/random/random_sample.h"

#include "poseidon/Release/random/RandomGen.h"
#include "poseidon/Release/random/Blake2xbPRNG.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/ParametersLiteral.h"
#include "poseidon/Release/rlwe.h"
#include "poseidon/Release/RelinKeys.h"

#include "poseidon/Release/Evaluator.h"
#include "poseidon/Release/HardwareEvaluator.h"
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/homomorphic_mod.h"
#include "poseidon/Release/util/chebyshev_interpolation.h"


//===================== Chebyshev  ============================
using namespace std;
using namespace poseidon;

double this_fun(double x)  {
    return  exp(x) / (exp(x) + 1);
    //return sin(6.283185307179586 * x) ;
}

int main()
{

    CKKSParametersLiteralDefault ckks_param_literal(degree_2048);
    PoseidonContext context(ckks_param_literal);

    //=====================init data ============================
    int vec_size = 1 << ckks_param_literal.LogSlots;
    double age, sbp, dbp, chl, height, weight;
    age = 26;
    sbp = 100;
    dbp = 70;
    chl = 100;
    height = 71;
    weight = 180;
    //输入个人身体参数，例：26, 100, 70, 100, 72, 180
    // printf("Please input your age:");
    // scanf("%lf", &age);
    // printf("\nPlease input your SBP:");
    // scanf("%lf", &sbp);
    // printf("\nPlease input your DBP:");
    // scanf("%lf", &dbp);
    // printf("\nPlease input your CHL:");
    // scanf("%lf", &chl);
    // printf("\nPlease input your height:");
    // scanf("%lf", &height);
    // printf("\nPlease input your weight:");
    // scanf("%lf", &weight);
    // printf("\n");

    //create message
    vector<complex<double>> message_age, message_sbp, message_dbp, message_chl, message_height, message_weight, vec_result;
    message_age.resize(vec_size);
    message_sbp.resize(vec_size);
    message_dbp.resize(vec_size);
    message_chl.resize(vec_size);
    message_height.resize(vec_size);
    message_weight.resize(vec_size);

    //The address with a message index of 0 stores the original value of the corresponding body data
    message_age[0] = age;
    message_sbp[0] = sbp;
    message_dbp[0] = dbp;
    message_chl[0] = chl;
    message_height[0] = height;
    message_weight[0] = weight;

    //Coef storage corresponding coefficient
    double coef_age = 0.072;
    double coef_sbp = 0.013;
    double coef_dbp = -0.029;
    double coef_chl = 0.008;
    double coef_height = -0.053;
    double coef_weight = 0.021;

    //The coefficients of Taylor expansion
    double taylor_coef_0 = 1.0 / 2;
    double taylor_coef_1 = 1.0 / 4;
    double taylor_coef_3 = -1.0 / 48;
    double taylor_coef_5 = 1.0 / 480;
    double taylor_coef_7 = -17.0 / 80640;
    double taylor_coef_9 = 31 / 1451520;

    //=====================init  Plain & Ciph =========================
    Plaintext plain_age, plain_sbp, plain_dbp, plain_chl, plain_height, plain_weight, plain_result;
    Ciphertext cipher_age, cipher_sbp, cipher_dbp, cipher_chl, cipher_height, cipher_weight, cipher_x, cipher_x_square, cipher_result;
    PublicKey public_key;
    RelinKeys relinKeys;
    // GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    // vector<uint32_t> rot_elemt;
    CKKSEncoder ckks_encoder(context);
//=====================poly init===================================
    auto a = -8.0;
    auto b = 8.0;
    auto deg = 64;
    printf("Evaluation of the function f(x) for even slots and g(x) for odd slots in the range [%0.2f, %0.2f] (degree of approximation: %d)\n", a, b, deg);

    auto approxF = util::Approximate(this_fun, a, b, deg);
    approxF.lead() = true;
    //auto approxG = util::Approximate(g, a, b, deg);
    vector <Polynomial> poly_v{approxF};
    vector<vector<int>> slotsIndex(1,vector<int>(context.poly_degree() >> 1,0));
    vector<int> idxF(context.poly_degree() >> 1);

    for(int i = 0; i < context.poly_degree() >> 1; i++){
        idxF[i] = i;   // Index with all even slots
    }
    slotsIndex[0] = idxF; // Assigns index of all even slots to poly[0] = f(x)

    PolynomialVector polys(poly_v,slotsIndex);

    //=====================keys  =========================
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_conj_keys(conjKeys);
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //-------------------encode--------------------------------
    ckks_encoder.encode(message_age, plain_age, context.scaling_factor());
    ckks_encoder.encode(message_sbp, plain_sbp, context.scaling_factor());
    ckks_encoder.encode(message_dbp, plain_dbp, context.scaling_factor());
    ckks_encoder.encode(message_chl, plain_chl, context.scaling_factor());
    ckks_encoder.encode(message_height, plain_height, context.scaling_factor());
    ckks_encoder.encode(message_weight, plain_weight, context.scaling_factor());

    //-------------------encrypt--------------------------------
    enc.encrypt(plain_age,cipher_age);
    enc.encrypt(plain_sbp,cipher_sbp);
    enc.encrypt(plain_dbp,cipher_dbp);
    enc.encrypt(plain_chl,cipher_chl);
    enc.encrypt(plain_height,cipher_height);
    enc.encrypt(plain_weight,cipher_weight);

    //-------------------------calculate----------------------------------
    //Create CKKS Evaluator
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);

    auto start = chrono::high_resolution_clock::now();

    //Compute x = 0.072∙Age+0.013∙SBP-0.029∙DBP+0.008∙CHL-0.053∙height+0.021∙weight

    ckks_eva->multiply_const(cipher_age, coef_age, cipher_age);
    ckks_eva->multiply_const(cipher_sbp, coef_sbp, cipher_sbp);
    ckks_eva->multiply_const(cipher_dbp, coef_dbp, cipher_dbp);
    ckks_eva->multiply_const(cipher_chl, coef_chl, cipher_chl);
    ckks_eva->multiply_const(cipher_height, coef_height, cipher_height);
    ckks_eva->multiply_const(cipher_weight, coef_weight, cipher_weight);

    ckks_eva->add(cipher_age, cipher_sbp, cipher_x);
    ckks_eva->add(cipher_x, cipher_dbp, cipher_x);
    ckks_eva->add(cipher_x, cipher_chl, cipher_x);
    ckks_eva->add(cipher_x, cipher_height, cipher_x);
    ckks_eva->add(cipher_x, cipher_weight, cipher_x);
    ckks_eva->rescale(cipher_x);

    //Compute e^x/(e^x+1)
    ckks_eva->multiply_const(cipher_x,(2.0/(double)(b-a)),cipher_x);
    ckks_eva->rescale(cipher_x);
    ckks_eva->evaluatePolyVector(cipher_x,cipher_result,polys,cipher_x.metaData()->getScalingFactor(),relinKeys,ckks_encoder);
    ckks_eva->read(cipher_result);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    //---------------------decode & decrypt-------------------------------
    dec.decrypt(cipher_result, plain_result);
    ckks_encoder.decode(plain_result, vec_result);

    printf("answer after FHE = %.8f \n",real(vec_result[0]));

    //expected answer
    double x = coef_age * age + coef_sbp * sbp + coef_dbp * dbp + coef_chl * chl + coef_height * height + coef_weight * weight;

    printf("expected answer = %.8f \n",exp(x) / (exp(x) + 1));

    return 0;
}

```

#  PIR
```cpp
// Standard libraries for input/output and mathematical operations
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <complex>
#include <gmpxx.h>

// Poseidon library headers for Fully Homomorphic Encryption operations and utilities
#include "poseidon/Release/define.h"
#include "poseidon/Release/homomorphic_DFT.h"
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/util/number_theory.h"
#include "poseidon/Release/hardware/ConfigGen.h"
#include <gmpxx.h>
#include "poseidon/Release/Ciphertext.h"

#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/BatchEncoder.h"
#include "poseidon/Release/random/random_sample.h"

#include "poseidon/Release/random/RandomGen.h"
#include "poseidon/Release/random/Blake2xbPRNG.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/ParametersLiteral.h"
#include "poseidon/Release/rlwe.h"
#include "poseidon/Release/RelinKeys.h"

#include "poseidon/Release/Evaluator.h"
#include "poseidon/Release/HardwareEvaluator.h"
#define RNS_C 2
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/util/matrix_operation.h"

// Using the Poseidon namespace for clarity
using namespace poseidon;

int main() {
    //===================== Configuration ============================
    // Initialize CKKS parameters with a default degree of 2048
    CKKSParametersLiteralDefault ckks_param_literal(degree_2048);
    PoseidonContext context(ckks_param_literal);

    //===================== Initialize Random Data ===================
    // Create vectors to hold complex numbers and results
    std::vector<std::complex<double>> vec, vec_result, vec_result1;
    int mat_size = 1 << ckks_param_literal.LogSlots; // Determine matrix size based on log slots
    std::vector<vector<std::complex<double>>> mat(mat_size, vector<complex<double>>(mat_size, 0));
    std::vector<vector<std::complex<double>>> mat_T(mat_size);
    std::vector<vector<std::complex<double>>> mat_T1;
    
    // Create a message vector initialized with zeros
    vector<complex<double>> message(mat_size, 0);
    message[1] = 1;  // Set the second element to 1

    vector<complex<double>> message_tmp(mat_size);
    vector<complex<double>> message_sum(mat_size << 1, 0.0);

    //===================== Initialize Plaintext & Ciphertext ========
    Plaintext plainA, plainB, plainRes, plainRes1, plainT;
    Ciphertext cipherA, cipherB, cipherRes, cipherRes1, cipherRes2, cipherRes3;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    CKKSEncoder ckks_encoder(context);

    //===================== Generate Matrices ========================
    MatrixPlain matrixPlain;
    // Populate the matrix with random complex numbers
    for (int i = 0; i < mat_size; i++) {
        sample_random_complex_vector2(mat[i], mat_size);
    }

    // Matrix operations
    auto level = context.crt_context()->maxLevel();
    matrix_operations::transpose_matrix(mat, mat_T1); // Transpose the matrix
    for (int i = 0; i < mat.size(); i++) {
        matrix_operations::diagonal(mat_T1, i, mat_T[i]);
    }
    GenMatrixformBSGS(matrixPlain, matrixPlain.rot_index, ckks_encoder, mat_T, level, context.crt_context()->primes_q()[level], 1, ckks_param_literal.LogSlots);

    //===================== Generate Keys ============================
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(matrixPlain.rot_index, rotKeys);
    kgen.create_conj_keys(conjKeys);

    Encryptor enc(context, public_key, kgen.secret_key());
    Decryptor dec(context, kgen.secret_key());

    //===================== Operations ===============================
    // Encode the message
    ckks_encoder.encode(message, plainA, context.scaling_factor());

    // Encrypt the plaintext
    enc.encrypt(plainA, cipherA);

    // Evaluation operations on the ciphertext
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);
    auto start = chrono::high_resolution_clock::now();
    ckks_eva->multiplyByDiagMatrixBSGS(cipherA, matrixPlain, cipherRes, rotKeys);
    ckks_eva->read(cipherRes);

    // Decrypt and decode the resulting ciphertext
    dec.decrypt(cipherRes, plainRes);
    ckks_encoder.decode(plainRes, vec_result);

    // Display results
    for (int i = 0; i < 8; i++) {
        printf("result vec[%d] : %0.10f + %0.10f I \n", i, real(mat[1][i]), imag(mat[1][i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n", i, real(vec_result[i]), imag(vec_result[i]));
    }

    return 0;
}

```

**CMakeLists.txt**
```
cmake_minimum_required(VERSION 3.10)
project(example)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

#link_directories(${LIBUV_LIBRARIES})
add_executable(example main.cpp )
target_link_libraries (${PROJECT_NAME} /usr/poseidon/libposeidon_shared.so gmp)
#target_link_libraries (${PROJECT_NAME} poseidon gmp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

```