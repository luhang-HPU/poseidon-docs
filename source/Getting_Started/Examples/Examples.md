
#  Encode/Decode, Encrypt/Decrypt  
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

// Make use of the Poseidon namespace for easier access to its classes and methods.
using namespace std;
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
    Encryptor enc(context, public_key, kgen.secret_key());
    Decryptor dec(context, kgen.secret_key());

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
    enc.encrypt(plainA, ciphA);

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec.decrypt(ciphA, plainRes);

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

# Plaintext-Ciphertext Multiplication
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <iostream>
#include <vector>
#include <complex>
#include "poseidon/Release/Ciphertext.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/Evaluator.h"
#include "poseidon/Release/ParametersLiteral.h"

// Make use of the Poseidon namespace for easier access to its classes and methods.
using namespace poseidon::util;
using namespace poseidon;

int main(){
    //=====================config======================================
    // Initialize CKKS parameters using default literal and given degree (4096).
    CKKSParametersLiteralDefault ckks_param_literal(degree_4096);
    poseidon::PoseidonContext context(ckks_param_literal);
    //=====================init random data ============================
    // Vector to store complex double values.
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;
    int vec_size = 1 << ckks_param_literal.LogSlots;
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    // Populate the message vector with random complex values of determined size.
    sample_random_complex_vector(message, vec_size);
    sample_random_complex_vector(message1, vec_size);
    //=====================init keys & Plain & Ciph =========================
    // Declare public key, plaintexts and ciphertext variables.
    Plaintext plainA,plainB,plainRes;
    Ciphertext cipherA,cipherB,cipherRes;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<int> steps{1,2,3,64,512};

    // Initialize the CKKS encoder with the provided context.
    CKKSEncoder ckks_encoder(context);
    //=====================keys  =========================
    // Initialize the key generator with the given context.
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(steps,rotKeys);
    kgen.create_conj_keys(conjKeys);
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    // Encode the message into a plaintext using the scaling factor from the context.
    ckks_encoder.encode(message,plainA,context.scaling_factor());
    ckks_encoder.encode(message1,plainB, context.scaling_factor());
    // Encrypt the plaintext and store the result in a ciphertext.
    enc.encrypt(plainA,cipherA);
    enc.encrypt(plainB,cipherB);
    printf("Doing config...\n");
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);

    //multiply_plain
    print_example_banner("Example: Multiply Plain / Multiply Plain in CKKS");
    auto start = chrono::high_resolution_clock::now();
    ckks_eva->multiply_plain(cipherA,plainB,cipherRes);
    ckks_eva->read(cipherRes);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec.decrypt(cipherRes,plainRes);

    // Decode the plaintext back into the message vector.
    ckks_encoder.decode(plainRes,vec_result);

    // Display the first 10 values of the source and result vectors for comparison.
    for(int i = 0; i < 4; i++){
        printf("source vec[%d] : %0.10f + %0.10f I \n",i,real(message[i] * message1[i]),imag(message[i] * message1[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,real(vec_result[i]), imag(vec_result[i]));
    }
    return 0;
}
```

# Ciphertext-Ciphertext Multiplication
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <iostream>
#include <vector>
#include <complex>
#include "poseidon/Release/Ciphertext.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/Evaluator.h"
#include "poseidon/Release/ParametersLiteral.h"

// Make use of the Poseidon namespace for easier access to its classes and methods.
using namespace poseidon::util;
using namespace poseidon;

int main(){
    //=====================config======================================
    // Initialize CKKS parameters using default literal and given degree (4096).
    CKKSParametersLiteralDefault ckks_param_literal(degree_4096);
    poseidon::PoseidonContext context(ckks_param_literal);
    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;
    int vec_size = 1 << ckks_param_literal.LogSlots;
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    sample_random_complex_vector(message, vec_size);
    sample_random_complex_vector(message1, vec_size);

    //=====================init keys & Plain & Ciph =========================
    // Declare public key, plaintexts and ciphertext variables.
    Plaintext plainA,plainB,plainRes;
    Ciphertext cipherA,cipherB,cipherRes;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<int> steps{1,2,3,64,512};

    // Initialize the CKKS encoder with the provided context.
    CKKSEncoder ckks_encoder(context);
    //=====================keys  =========================
     // Initialize the key generator with the given context.
    KeyGenerator kgen(context);

   // Generate the public key, relin key, galois key, conjugate key.
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(steps,rotKeys);
    kgen.create_conj_keys(conjKeys);

    // Initialize the encryptor and decryptor using the public and secret keys respectively.
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    // Encode the message into a plaintext using the scaling factor from the context.
    ckks_encoder.encode(message,plainA,context.scaling_factor());
    ckks_encoder.encode(message1,plainB, context.scaling_factor());
    // Encrypt the plaintext and store the result in a ciphertext.
    enc.encrypt(plainA,cipherA);
    enc.encrypt(plainB,cipherB);
    printf("Doing config...\n");
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);

    //multiply
    print_example_banner("Example: Multiply / Multiply in CKKS");
    auto start = chrono::high_resolution_clock::now();
    ckks_eva->multiply(cipherA,cipherB,cipherRes,relinKeys);
    ckks_eva->read(cipherRes);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec.decrypt(cipherRes,plainRes);
    // Decode the plaintext back into the message vector.
    ckks_encoder.decode(plainRes,vec_result);

    // Display the first 10 values of the source and result vectors for comparison.
    for(int i = 0; i < 4; i++){
        printf("source vec[%d] : %0.10f + %0.10f I \n",i,real(message[i] * message1[i]),imag(message[i] * message1[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,real(vec_result[i]), imag(vec_result[i]));
    }
    return 0;
}
```


# Ciphertext Rotation
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <iostream>
#include <vector>
#include <complex>
#include "poseidon/Release/Ciphertext.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/Evaluator.h"
#include "poseidon/Release/ParametersLiteral.h"

// Make use of the Poseidon namespace for easier access to its classes and methods.
using namespace poseidon::util;
using namespace poseidon;

int main(){
    //=====================config======================================
    // Initialize CKKS parameters using default literal and given degree (4096).
    CKKSParametersLiteralDefault ckks_param_literal(degree_4096);
    poseidon::PoseidonContext context(ckks_param_literal);
    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;
    int vec_size = 1 << ckks_param_literal.LogSlots;
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    sample_random_complex_vector(message, vec_size);
    sample_random_complex_vector(message1, vec_size);

    //=====================init keys & Plain & Ciph =========================
    // Declare public key, plaintexts and ciphertext variables.
    Plaintext plainA,plainB,plainRes;
    Ciphertext cipherA,cipherB,cipherRes;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<int> steps{1,2,3,64,512};
    CKKSEncoder ckks_encoder(context);
    //=====================keys  =========================
    KeyGenerator kgen(context);
    // Generate the public key, relin key, galois key, conjugate key.
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(steps,rotKeys);
    kgen.create_conj_keys(conjKeys);
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    // Encode the message into a plaintext using the scaling factor from the context.
    ckks_encoder.encode(message,plainA,context.scaling_factor());
    ckks_encoder.encode(message1,plainB, context.scaling_factor());
    // Encrypt the plaintext and store the result in a ciphertext.
    enc.encrypt(plainA,cipherA);
    enc.encrypt(plainB,cipherB);
    printf("Doing config...\n");
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);

    //rotate
    print_example_banner("Example: Rotation / Rotation in CKKS");
    auto start = chrono::high_resolution_clock::now();
    ckks_eva->rotate(cipherA,cipherRes,rotKeys,1);
    ckks_eva->read(cipherRes);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec.decrypt(cipherRes,plainRes);

    // Decode the plaintext back into the message vector.
    ckks_encoder.decode(plainRes,vec_result);

    // Display the first 10 values of the source and result vectors for comparison.
    for(int i = 0; i < 4; i++){
        printf("source vec[%d] : %0.10f + %0.10f I \n",i,real(message[i]),imag(message[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,real(vec_result[i]), imag(vec_result[i]));
    }
    return 0;
}

```



# Matrix Multiplication
```cpp
// Include necessary Poseidon library headers for different functionalities.
#include <iostream>
#include <vector>
#include <complex>
#include <gmpxx.h>
#include "poseidon/Release/linear_transform.h"
#include "poseidon/Release/Ciphertext.h"
#include "poseidon/Release/util/matrix_operation.h"
#include "poseidon/Release/CKKSEncoder.h"
#include "poseidon/Release/BatchEncoder.h"
#include "poseidon/Release/random/random_sample.h"
#include "poseidon/Release/KeyGenerator.h"
#include "poseidon/Release/Encryptor.h"
#include "poseidon/Release/Decryptor.h"
#include "poseidon/Release/ParametersLiteral.h"
#include "poseidon/Release/RelinKeys.h"
#include "poseidon/Release/Evaluator.h"

// Make use of the Poseidon namespace for easier access to its classes and methods.
using  namespace  poseidon;

int main(){
    //=====================config======================================
    // Initialize CKKS parameters using default literal and given degree (2048).
    CKKSParametersLiteralDefault ckks_param_literal(degree_2048);
    PoseidonContext context(ckks_param_literal);
    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;
    int mat_size = 1 << ckks_param_literal.LogSlots;
    std::vector<vector<std::complex<double>>> mat(mat_size,vector<complex<double>>(mat_size,0));
    std::vector<vector<std::complex<double>>> mat_T(mat_size);
    //Create random message.
    vector<complex<double>> message(mat_size,0);
    sample_random_complex_vector(message,mat_size);


    //=====================init  Plain & Ciph =========================
    // Declare public key, plaintexts and ciphertext variables.
    Plaintext plainA,plainRes;
    Ciphertext cipherA,cipherB,cipherRes;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<uint32_t> rot_elemt;
    // Initialize the CKKS encoder with the provided context.
    CKKSEncoder ckks_encoder(context);
    //=====================GenMatrices  ========================
    MatrixPlain matrixPlain;
    //Create random matrix message.
    for(int i = 0; i < mat_size; i++){
        sample_random_complex_vector(mat[i],mat_size);
    }

    //Create random matrix message.
   
    //Matrix data preprocessing.
    for(int i = 0; i < mat.size(); i++){
        matrix_operations::diagonal(mat, i,mat_T[i]);
    }

    //Matrix data to Plaintext.
    int level = context.crt_context()->maxLevel();
    GenMatrixformBSGS(matrixPlain,matrixPlain.rot_index, ckks_encoder, mat_T,
                      level ,context.crt_context()->primes_q()[level], 1, ckks_param_literal.LogSlots);

    //=====================keys  =========================
     // Initialize the key generator with the given context.
    KeyGenerator kgen(context);
    // Generate the public key, relin key, galois key, conjugate key.
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    kgen.create_galois_keys(matrixPlain.rot_index,rotKeys);
    // kgen.create_galois_keys(rot_elemt,rotKeys);
    kgen.create_conj_keys(conjKeys);

    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());
    //===================== Doing ==============================
    // Encode the message into a plaintext using the scaling factor from the context.
    ckks_encoder.encode(message,plainA,context.scaling_factor());

    // Encrypt the plaintext and store the result in a ciphertext.
    enc.encrypt(plainA,cipherA);

    //evaluate
    auto ckks_eva = EvaluatorFactory::DefaultFactory()->create(context);

    auto start = chrono::high_resolution_clock::now();
    ckks_eva->multiplyByDiagMatrixBSGS(cipherA,matrixPlain,cipherRes,rotKeys);
    ckks_eva->read(cipherRes);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EVA TIME: " << duration.count() << " microseconds"<< endl;

    // Decrypt the ciphertext and store the result back into a plaintext.
    dec.decrypt(cipherRes,plainRes);

    // Decode the plaintext back into the message vector.
    ckks_encoder.decode(plainRes,vec_result);

    //Calculate actual results.
    vector<complex<double>> message_tmp(mat_size);
    matrix_operations::matrix_vector_multiply(mat, message, message_tmp);

    // Display the first 10 values of the source and result vectors for comparison.

    for(int i = 0; i < 8; i++){
        printf("source vec[%d] : %0.10f + %0.10f I \n",i,real(message_tmp[i]), imag(message_tmp[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,real(vec_result[i]), imag(vec_result[i]));
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


add_executable(example main.cpp )
target_link_libraries (${PROJECT_NAME} /usr/poseidon/libposeidon_shared.so gmp)

target_link_libraries(${PROJECT_NAME} Threads::Threads)

```