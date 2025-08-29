# Examples

<br>

##  BFV Basic Example

The [test_bfv_basic.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/bfv/test_bfv_basic.cpp) exmaple tests a variety of **BFV** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_bfv_basic.cpp** example.

Every basic operation in the **test_bfv_basic.cpp** example will record the time spent and verify the correctness of homomorphic computation result.


<br>

## BFV Matrix Vector Multiplication Example

The [test_bfv_mult_matrix.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/bfv/test_bfv_mult_matrix.cpp) example tests the multiplication of ciphertext by plaintext matrix. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 8192 and security level of tc128, to execute the **test_bfv_mult_matrix.cpp** example.

This example compares the homomorphic computation result with the correct answer.


<br>

## BGV Basic Example

The [test_bgv_basic.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/bgv/test_bgv_basic.cpp) example tests a variety of **BGV** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation. 

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_bgv_basic.cpp** example.

Every basic operation in the **test_bgv_basic.cpp** example will record the time spent and verify the correctness of homomorphic computation result.


<br>

## CKKS Basic Example

The [test_ckks_basic.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/ckks/test_ckks_basic.cpp) example tests a variety of **CKKS** homomorphic operations, including addition of ciphetexts, addition of ciphertext and plaintext, substraction ciphertext from ciphertext, substraction plaintext from ciphertext, multiplication of ciphertext by ciphertext, multiplication of ciphertext by plaintext, mod switching, NTT/inverse NTT and rotation, conjugation, rescaling.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute  the **test_ckks_basic.cpp** example.

As the **CKKS** scheme is based on approximate computation, there might be decimal deviation bewteen the homomorphic computation results and the correct results. Every basic operation in the **test_ckks_basic.cpp** example will record the time spent and the precision of homomorphic computation results.


<br>

## CKKS Coeff to Slot & Slot to Coeff Example

The [test_ckks_slot_to_coeff.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/ckks/test_ckks_slot_to_coeff.cpp) example tests the function of **coeff to slot** and **slot to coeff**, converting the ciphertext from coefficient mode to slot mode or converting the ciphertext from slot mode to coefficient mode.

The **coeff to slot** and **slot to coeff** function are two important operations in **CKKS** bootstrapping. In **CKKS** bootstrapping, it is necessary to perform a modulo operation on the coefficients of the polynomial. However, only homomorhpic addition and homomorphic multiplication could be performed to simulate the modulo operation. And the homomorphic addition and homomorphic multiplication are perform for slot. Therefore, it is necessary for the coefficients of polynomial to be converted to slot. When all the other operations are done, the slot needs to be converted to coefficients in the end.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 16384 and security level of tc_128, to execute the **test_ckks_slot_to_coeff.cpp** example.


<br>

## CKKS Evaluate Poly Vector Example

The [test_ckks_evaluate_poly_vector.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/ckks/test_ckks_evaluate_poly_vector.cpp) example tests the function of evaluating poly vector which is an important part of homomorphic modulo operation in **CKKS** bootstrapping. 

**evaluate_poly_vector** simulates the modulo function.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to execute the **test_ckks_evaluate_poly_vector.cpp** example.


<br>

## CKKS Bootstrap Example

The [test_ckks_bootstrap.cpp](https://github.com/luhang-HPU/poseidon/blob/main/examples/ckks/test_ckks_bootstrap.cpp) example tests the bootstrap function of **CKKS**.

Users can choose customized parameters or default parameters, which are software version, polynomial degree of 4096 and security level of tc_128, to execute the **test_ckks_bootstrap.cpp** example.

