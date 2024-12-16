The examples directory contains BFV, BGV and CKKS basic operation examples.

All the BFV, BGV and CKKS basic examples supports different security level, different polynomial degree, and can be executed on both CPU or HPU accelerator card. In particular, if users wish to test the examples in various scenarios, they can customize the security parameters to meet the requirement.



# Prerequisites

There are three different key switch schemes in the project ---- `BV`, `GHS` and `Hybrid` scheme, in which the modulus setting regulation is different from one another.

In `BV` key switch scheme, the number of modulus chain P is 1 and the number of modulus chain Q is variable.

In `GHS` key switch scheme, the number of modulus chain P is limited to be equal to the number of modulus chain Q.

In Poseidon, we use the `BV` key switch scheme in BFV and BGV scheme and `GHS` key switch scheme in CKKS scheme (the Hybrid key switch scheme is unused for now). 



Here, we introduce a BFV example to show how to program with the Poseidon library.



## Choose the device type

First of all, we need to decide whether to run the program on the CPU or HPU. 

If we would like to run the program on the CPU, we choose the `DEVICE_SOFTWARE` device type. 

If we would like to implement the HPU card to accelerate the process, we could choose the `DEVICE_HARDWARE` device type.

The software mode is always applicable, however the hardware mode is only supported when **the hardware library and the HPU card are both installed**.

```cpp
// To run the program with CPU, set the DEVICE_SOFTWARE
PoseidonFactory::get_instance()->set_device_type(DEVICE_SOFTWARE);

// To run the program with HPU, set the DEVICE_HARDWARE
PoseidonFactory::get_instance()->set_device_type(DEVICE_HARDWARE);
```



## Parameter Specification & Encryption Class Initialization

We need to specify the parameters to initiate the context, including the encryption scheme, the polynomial degree and the security level.

```cpp
// Here we choose the homomorphic encryption scheme to be BFV, 
// the polynomial degree to be 16384, 
// the security level to be tc128
ParametersLiteralDefault bfv_param_literal(BFV, 16384, poseidon::sec_level_type::tc128);

// Create the context with the customized parameter    
PoseidonContext context = PoseidonFactory::get_instance()->create_poseidon_context(bfv_param_literal);
```



With the context generated, we could construct the key generator, encoder, encryptor, decryptor and evaluator class.

```cpp
BatchEncoder enc(context);

KeyGenerator keygen(context);
PublicKey public_key;
GaloisKeys galois_keys;
RelinKeys relin_keys;
keygen.create_public_key(public_key);
keygen.create_galois_keys(galois_keys);
keygen.create_relin_keys(relin_keys);

Encryptor encryptor(context, public_key);
Decryptor decryptor(context, keygen.secret_key());

// Before performing the homomorphic computation, the evaluator class should be specified
std::shared_ptr<EvaluatorBfvBase> bfv_eva = PoseidonFactory::get_instance()->create_bfv_evaluator(context);
```



## Encoding and Encryption

We could encode the message with the `BatchEncoder` class and encrypt the plaintext the `Encryptor` class.

```cpp
Plaintext plain1;
Ciphertext ciph1;
std::vector<int> message1 = {1, 2, 3};

enc.encode(message1, plain1);
encryptor.encrypt(plain1, ciph1);
```



In BFV and BGV scheme, integer is supported, therefore the type of message is `std::vector<int>`.

In CKKS scheme, double and complex number are supported, therefore the message could be `std::vector<double>` or `std::vector<std::complex<double>>` type.



## Homomorphic Computation

We could execute the homomorphic computation by calling the member function of the `Evaluator` class.

```cpp
bfv_eva->add(ciph1, ciph2, ciph1);
```



As above, most of the homomorphic computation function supports the inplace computation. That is, the result ciphertext could be one of the operand ciphertext.



## Decoding and Decryption

After all the homomorphic computations are done, we could decrypt the ciphertext result with the `Decryptor` class and decode the plaintext result with the `Encoder` class.

```cpp
decryptor.decrypt(ciph1, plain_res);
enc.decode(plain_res, message_res);
```



## Time and Correctness Measurement

In Poseidon examples, `TimeStac` class is used to measure the time spent on the homomorphic computation.

```cpp
Timestacs timestacs;
timestacs.start();
timestacs.end();
timestacs.print_time("TIME : ");
```



In BFV and BGV examples, the integer type homomorphic computation result and the integer calculated directly should be the same.

```cpp
for (auto i = 0; i < message_want.size(); i++)
{
    printf("source_data[%d] : %ld\n", i, message1[i]);
    printf("result_data[%d] : %ld\n", i, message_res[i]);
}
```



In CKKS examples, the double type homomorphic computation result and the integer calculated directly may differ in decimal number.

The `GetPrecisionStats` function give the mean square error of the result.

```cpp
 for (auto i = 0; i < 4; i++)
    {
        printf("source_data[%d] : %.10lf + %.10lf I\n", i, message_want[i].real(),
               message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n", i, message_res[i].real(),
               message_res[i].imag());
    }
    util::GetPrecisionStats(message_want, message_res);
```





# Examples



##  BFV Basic

The `test_bfv_basic.cpp` exmaple tests a variety of BFV homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to run the `test_bfv_basic.cpp` example.

Every basic operation in the `test_bfv_basic.cpp` example will record the time spent and verify the correctness of homomorphic computation result.



## BFV Matrix Vector Multiplication

The `test_bfv_mult_matrix.cpp` example tests the multiplication of ciphertext by plaintext matrix. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 8192 and security level of tc128, to run the `test_bfv_mult_matrix.cpp` example.

This example compares the homomorphic computation result with the correct answer.



## BGV Basic

The `test_bgv_basic.cpp` example tests a variety of BGV homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to run the `test_bgv_basic.cpp` example.

Every basic operation in the `test_bgv_basic.cpp` example will record the time spent and verify the correctness of homomorphic computation result.



## CKKS Basic

The `test_ckks_basic.cpp` example tests a variety of BGV homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation, conjugation, rescaling.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to run the `test_ckks_basic.cpp` example.

As the CKKS scheme is based on approximate computation, there might be decimal deviation bewteen the homomorphic computation results and the correct results. Every basic operation in the `test_ckks_basic.cpp` example will record the time spent and the precision of homomorphic computation results.



## CKKS Coeff to Slot & Slot to Coeff

The `test_ckks_slot_to_coeff.cpp` example tests the function of `coeff to slot` and `slot to coeff`, converting the ciphertext from coefficient mode to slot mode or converting the ciphertext from slot mode to coefficient mode.

The `coeff to slot` and `slot to coeff` function are two important operations in CKKS bootstrapping. In CKKS bootstrapping, it is necessary to perform a modulo operation on the coefficients of the polynomial. However, only homomorhpic addition and homomorphic multiplication could be performed to simulate the modulo operation. And the homomorphic addition and homomorphic multiplication are perform for slot. Therefore, it is necessary for the coefficients of polynomial to be converted to slot. When all the other operations are done, the slot needs to be converted to coefficients in the end.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to run the `test_ckks_slot_to_coeff.cpp` example.



## CKKS Evaluate Poly Vector

The `test_ckks_evaluate_poly_vector.cpp` example tests the function of evaluating poly vector which is an important part of homomorphic modulo operation in CKKS bootstrapping. 

`evaluate_poly_vector` simulates the modulo function.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to run the `test_ckks_evaluate_poly_vector.cpp` example.



## CKKS Bootstrap

The `test_ckks_bootstrap.cpp` example tests the bootstrap function of CKKS.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to run the `test_ckks_bootstrap.cpp` example.

