# Examples


**main.cpp**
```cpp
#include <poseidon/Release/ParametersLiteral.h>
#include <poseidon/Release/PoseidonContext.h>
#include <poseidon/Release/define.h>
#include <poseidon/Release/BatchEncoder.h>
#include <poseidon/Release/KeyGenerator.h>
#include <poseidon/Release/Encryptor.h>
#include <poseidon/Release/Decryptor.h>
#include <poseidon/Release/CKKSEncoder.h>

using namespace std;

using namespace poseidon;
int main() {

    CKKSParametersLiteralDefault param_literal(degree_8192);
    PoseidonContext context(param_literal);
    auto degree = 1 << param_literal.LogN;
    
    PublicKey public_key;
    Plaintext plainA,plainRes;
    Ciphertext ciphA;
    //KeyGenerator kgen1(context,aa);
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    Encryptor enc1(context,public_key,kgen.secret_key());
    Decryptor dec1(context,kgen.secret_key());


    default_random_engine e;
    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result;
    std::vector<vector<std::complex<double>>> mat;
    int vec_size = 1 << param_literal.LogSlots;
    mat.resize(vec_size);
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1(vec_size);
    sample_random_complex_vector(message, vec_size);


    CKKSEncoder ckksenc(context);
    ckksenc.encode(message,plainA,context.scaling_factor());

    enc1.encrypt(plainA,ciphA);
    dec1.decrypt(ciphA,plainRes);
    ckksenc.decode(plainRes,message1);
    for(int i = 0; i < 10; i++){
        //printf("source vec[%d] : %0.10f + %0.10f I \n",i,real(messageReal2[i].real()),imag(messageReal2[i].real()));
        printf("source vec[%d] : %.10lf \n",i,message[i]);
        printf("result vec[%d] : %.10lf \n",i,message1[i]);
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