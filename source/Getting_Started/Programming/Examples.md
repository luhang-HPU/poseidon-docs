The ***poseidon/examples*** directory contains three subdirectories ***BFV***, ***BGV*** and ***CKKS*** , which contain basic example programming for different schemes.

All the BFV, BGV and CKKS examples support different security levels and polynomial degrees. Additionally, they can be executed in both software mode and hardware mode. If users wish to execute the examples in various scenarios, they can customize the security parameters to meet the requirement.


<br>

# How to Program with Poseidon

In this chapter, we will introduce an example based on BFV to show how to program with Poseidon.

<br>

## Prerequisites

Poseidon supports three different key switch schemes -- **BV**, **GHS** and **HYBRID**, of which the difference lies in the setting of modulus chain.

In **BV** key switch scheme, the size of modulus chain P is 1 and the size of modulus chain Q is unlimited.

In **GHS** key switch scheme, the size of modulus chain P is limited to be equal to the size of modulus chain Q.

In **HYBRID** key switch scheme, the size of modulus chain P and Q is unlimited.

For now, Poseidon implement **BV** key switch scheme for **BFV**, **BGV** and **CKKS**.


<br>

## Choose the Device Type

At the beginning of the program, we need to decide whether to run the program on the CPU or HPU.

```cpp
// To run the program with CPU, set the DEVICE_SOFTWARE
PoseidonFactory::get_instance()->set_device_type(DEVICE_SOFTWARE);

// To run the program with HPU, set the DEVICE_HARDWARE
PoseidonFactory::get_instance()->set_device_type(DEVICE_HARDWARE);
```

**Pay attention! Hardware mode (*DEVICE_HARDWARE*) is supported only when both the Poseidon hardware library and the HPU card are installed**.


<br>

## Parameter Specification & Encryption Class Initialization

Next, we need to specify the parameters which are the encryption scheme, polynomial degree and security level to initiate the context.

```cpp
// Here we choose the homomorphic encryption scheme to be BFV, 
// the polynomial degree to be 16384 and the security level to be tc128
ParametersLiteralDefault bfv_param_literal(BFV, 16384, poseidon::sec_level_type::tc128);

// Create the context with the customized parameter above    
PoseidonContext context = PoseidonFactory::get_instance()->create_poseidon_context(bfv_param_literal);
```



With the generated context, we could construct the key generator, encoder, encryptor, decryptor and evaluator class.

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

// Before performing the homomorphic computation, the evaluator class should be specified as EvaluatorBfvBase
std::shared_ptr<EvaluatorBfvBase> bfv_eva = PoseidonFactory::get_instance()->create_bfv_evaluator(context);
```


<br>

## Encoding and Encryption

We could encode the message with `BatchEncoder` and encrypt the plaintext with `Encryptor` .

```cpp
Plaintext plain1;
Ciphertext ciph1;
std::vector<int> message1 = {1, 2, 3};

enc.encode(message1, plain1);
encryptor.encrypt(plain1, ciph1);
```

The message type differs between the schemes. Integer is supported in **BFV** and **BGV**, while double and std::complex are supported in **CKKS**.


<br>

## Homomorphic Computation

We could execute the homomorphic computation by calling the member function of the `Evaluator` class.

```cpp
Ciphertext ciph1, ciph2, ciph3;

// homomorphic addition
bfv_eva->add(ciph1, ciph2, ciph1);
// homomorphic multiplication and relinearization
bfv_eva->multiply_relin(ciph1, ciph2, ciph1);
// mod switching
bfv_eva->drop_modulus_to_next(ciph1, ciph1);
```


<br>

## Decoding and Decryption

After all the homomorphic computations are done, we could decrypt the ciphertext result with `Decryptor` and decode the plaintext result with `Encoder`.

In the end, we get the homomorphic computation result **message_res** which should be the same as direct computation result on the message.

```cpp
decryptor.decrypt(ciph1, plain_res);
enc.decode(plain_res, message_res);
```


<br>

## Timer, Accuracy and Precision

Poseidon provides some useful gadgets for users.

<br>

* `TimeStacs` , the timer to record the total time spent on the homomorphic computation.

```cpp
Timestacs timestacs;
timestacs.start();
timestacs.end();
timestacs.print_time("TIME : ");
```

<br>

* Accuracy

In **BFV** and **BGV** examples, the result of homomorphic computation should be the same as the result calculated directly.

```cpp
for (auto i = 0; i < message_want.size(); i++)
{
    printf("source_data[%d] : %ld\n", i, message1[i]);
    printf("result_data[%d] : %ld\n", i, message_res[i]);
}
```

In **CKKS** examples, the result of homomorphic computation should be approximate to the result calculated directly, which may differ a little in decimal part.

```cpp
for (auto i = 0; i < 4; i++)
{
    printf("source_data[%d] : %.10lf + %.10lf I\n", i, message_want[i].real(), message_want[i].imag());
    printf("result_data[%d] : %.10lf + %.10lf I\n", i, message_res[i].real(), message_res[i].imag());
}
```

<br>

* `GetPrecisionStats`, the function for measurement of precision for **CKKS**

In **CKKS** examples, `GetPrecisionStats` calculates the mean square error of the result.

```cpp
util::GetPrecisionStats(message_want, message_res);
```


<br>

# Examples

<br>

##  BFV Basic Example

The **test_bfv_basic.cpp** exmaple tests a variety of **BFV** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_bfv_basic.cpp** example.

Every basic operation in the **test_bfv_basic.cpp** example will record the time spent and verify the correctness of homomorphic computation result.


<br>

## BFV Matrix Vector Multiplication Example

The **test_bfv_mult_matrix.cpp** example tests the multiplication of ciphertext by plaintext matrix. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 8192 and security level of tc128, to execute the **test_bfv_mult_matrix.cpp** example.

This example compares the homomorphic computation result with the correct answer.


<br>

## BGV Basic Example

The **test_bgv_basic.cpp** example tests a variety of **BGV** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_bgv_basic.cpp** example.

Every basic operation in the **test_bgv_basic.cpp** example will record the time spent and verify the correctness of homomorphic computation result.


<br>

## CKKS Basic Example

The **test_ckks_basic.cpp** example tests a variety of **CKKS** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation, conjugation, rescaling.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute  the **test_ckks_basic.cpp** example.

As the **CKKS** scheme is based on approximate computation, there might be decimal deviation bewteen the homomorphic computation results and the correct results. Every basic operation in the **test_ckks_basic.cpp** example will record the time spent and the precision of homomorphic computation results.


<br>

## CKKS Coeff to Slot & Slot to Coeff Example

The **test_ckks_slot_to_coeff.cpp** example tests the function of **coeff to slot** and **slot to coeff**, converting the ciphertext from coefficient mode to slot mode or converting the ciphertext from slot mode to coefficient mode.

The **coeff to slot** and **slot to coeff** function are two important operations in **CKKS** bootstrapping. In **CKKS** bootstrapping, it is necessary to perform a modulo operation on the coefficients of the polynomial. However, only homomorhpic addition and homomorphic multiplication could be performed to simulate the modulo operation. And the homomorphic addition and homomorphic multiplication are perform for slot. Therefore, it is necessary for the coefficients of polynomial to be converted to slot. When all the other operations are done, the slot needs to be converted to coefficients in the end.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_ckks_slot_to_coeff.cpp** example.


<br>

## CKKS Evaluate Poly Vector Example

The **test_ckks_evaluate_poly_vector.cpp** example tests the function of evaluating poly vector which is an important part of homomorphic modulo operation in **CKKS** bootstrapping. 

**evaluate_poly_vector** simulates the modulo function.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to execute the **test_ckks_evaluate_poly_vector.cpp** example.


<br>

## CKKS Bootstrap Example

The **test_ckks_bootstrap.cpp** example tests the bootstrap function of **CKKS**.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to execute the **test_ckks_bootstrap.cpp** example.

