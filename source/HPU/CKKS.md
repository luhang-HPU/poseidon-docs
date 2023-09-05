# CKKS 

## Parameter Data Structure




Poseidon Supported parameter data structures：


### 1. Memory address management : MemoryPool
**<font color='red'>MemoryPool</font>** (**DegreeType** degree_type);

**Description**：MemoryPool is a class for managing addresses and memory space.

**Parameter**：degree_type: DegreeType indicates the degree of a polynomial. 

​						The optional value can be degree_2048,degree_4096, degree_8192, degree_16384, or degree_32768.

**Member function**： getInstance(DegreeType degree_type) : The class used to create Memorypool.

### 2. Parameter class of the CKKS encryption scheme : CKKSParametersLiteralDefault
### 3. Context information management class : PoseidonContext
### 4. Generate pseudo-random numbers class : Blake2xbPRNGFactory
### 5. Plaintext class : Plaintext      
### 6. Ciphertext class : Ciphertext
### 7. Public key class : PublicKey
### 8. Relinearize key class : RelinKeys
### 9.   Galois key class : GaloisKeys
### 10. CKKS encryption scheme  class : CKKSEncoder
### 11. Plaintext matrix : MatrixPlain
### 12. Key generation class : KeyGenerator
### 13. Encryption class : Encryptor
### 14. Decryption class : Decryptor
### 15.  Envaluator Factory  class : EnvaluatorFactory




## API
### 1. Addition between ciphertexts : add
**void** **<font color='red'> add</font>** (**Ciphertext** &ciph1, **Ciphertext** &ciph2, **Ciphertext** &result) override;

**Description**：This function performs homomorphic addition on two ciphertexts.<br>

**Parameter**：**ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 2. Addition of ciphertext and plaintext : add_plain
**void** **<font color='red'> add_plain</font>** (**Ciphertext** &ciph, **Plaintext** &plain,**Ciphertext** &result) override;

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameter**：**ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 3. Subtraction between ciphertexts : sub
**void** **<font color='red'> sub</font>** (**Ciphertext** &ciph1, **Ciphertext** &ciph2, **Ciphertext** &result) override;

**Description**：This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameter**：**ciph1**:  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 4. Multiplication between ciphertexts : multiply
**void** **<font color='red'> multiply</font>** (**Ciphertext** &ciph0, **Ciphertext** &ciph1, **Ciphertext** &result, const **RelinKeys** &relin_key) override;

**Description**：This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameter**：**ciph1**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **relin_key**: A constant reference to a **RelinKeys** object, representing the relinearization key.



### 5. Multiplication of ciphertext and plaintext : multiply_plain
**void** **<font color='red'> multiply_plain</font>** (**Ciphertext** &ciph, **Plaintext** &plain,**Ciphertext** &result) override;

**Description**：This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameter**：**ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>



### 6. Rescale : rescale
**void** **<font color='red'> rescale</font>** (**Ciphertext** &ciph) override;

**Description**：This function performs a rescaling operation on a ciphertext.<br>

**Parameter**：**ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>



### 7. Ciphertext rotation : rotate
### 8. Take conjugate : conjugate
### 9. Matrix multiplication of ciphertext and plaintext : multiplyByDiagMatrixBSGS
**void** **<font color='red'> multiplyByDiagMatrixBSGS</font>** (**Ciphertext** &ciph, **MatrixPlain** &plain_mat, **Ciphertext** &result, **const GaloisKeys** &rot_key) override;

**Description**：This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain_mat**:  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **rot_key**:   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.



### 10. Coefficient to plaintext slot : coeff_to_slot
### 11. Plaintext slot to coefficient : slot_to_coeff
### 12. Fast Fourier Transform (forward) : ftt_fwd
### 13. Fast Fourier Transform (inverse) : ftt_inv
### 14. Read ciphertext information from accelerator card : read
### 15. Multiplication of ciphertext and complex constant : multiply_const
### 16. Addition of ciphertext and complex constant : add_const
### 17. Discrete Fourier Transform on ciphertext : dft
### 18. Dynamic rescale : rescale_dynamic
### 19. Polynomial evaluation : evaluatePolyVector
### 20. Evaluate modulo on ciphertext vector : eval_mod
### 21. Bootstrap : bootstrap
### 22. Key switch : switch_key
**void** **<font color='red'> switch_key</font>** (**Ciphertext** &ciph, **Ciphertext** &result, const **vector<PublicKey>** &switch_key);

**Description**：This function switches the key of a ciphertext.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **switch_key**:  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys.






