# BFV (**<font color='blue'> Hardware Updating</font>**)

## Data Structures


Poseidon Supported parameter data structures, the following is specific to BFV：

<br>


### 1. BFV encryption scheme  class : **<font color='red'>BatchEncoder</font>**

**Description**：BatchEncoder is a class used for encoding and decoding in the BFV encryption scheme.<br>


**Functions**：

```cpp
BatchEncoder(const PoseidonContext &context);
```

- **context** (const PoseidonContext &): The poseidon context.

: Creates a BatchEncoder. It is necessary that the encryption parameters given through the SEALContext object support batching.


<br>


```cpp
void encode(const vector<uint32_t> &src, Plaintext &plain);
```

- **src** (const vector<uint32_t> &): The source message.
- **plain** (Plaintext &): The source message.

: A function used to encode a complex number vector into a plaintext polynomial.

<br>


```cpp
void decode(const Plaintext &plain,vector<uint32_t> &res);
```

- **plain** const Plaintext &plain): The plaintext.
- **res** (vector<uint32_t> &): The result of message.

: A function used to decode a plaintext polynomial into a complex number vector.

<br>

## Evaluation Functions

### 1. Ciphertext and ciphertext addition : **<font color='red'> add</font>**

```c
void add(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**：This function performs homomorphic addition on two ciphertexts.<br>

**Parameters**：

- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.

<br>

### 2. Ciphertext and plaintext addition : **<font color='red'> add_plain</font>**

```c
void add_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**：

- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
  <br>


### 3. Ciphertext and ciphertext subtraction : **<font color='red'> sub</font>**

```c
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**：This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameters**：

- **ciph1** (Ciphertext):  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
  <br>


### 4. Subtraction plaintext from ciphertext : **<font color='red'> sub_plain</font>**

```c
void sub_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const override;
```

**Description**: This function performs homomorphic operation of subtracting the plaintext from the plaintext.

**Parameters**：

* **ciph** (Ciphertext):  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).
* **plain** (Plaintext): A reference to a **Plaintext** object, representing the subtrahend (the number to be subtracted).
* **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
  <br>


### 5. Multiplication between ciphertexts : **<font color='red'> multiply</font>** (**<font color='blue'> only software</font>**)

```c
void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
```

**Description**：This function performs homomorphic multiplication between two ciphertexts.<br>

**Parameters**：

- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
  <br>


### 6. Multiplication between ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```c
void multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const override;
```

**Description**：This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**：

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
  <br>


### 7. Multiplication with relinearization : **<font color='red'> multiply_relin</font>**

```c
void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result, const RelinKeys &relin_key) const override;
```

**Description**: This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**:

- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.

<br>



### 8. Square inplace: **<font color='red'> square_inplace</font>**

```c
void BFVEvaluator_S::square_inplace( Ciphertext &ciph) const
```

**Description**: This function performs homomorphic square operation of the ciphertext inplace.<br>

**Parameters**:

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
  <br>


### 9.  Relinearization : **<font color='red'> relinearize</font>** (**<font color='blue'> only software</font>**)

```c
void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const override;
```

**Description**: This function  performs relinearization operation on the ciphertext.

**Parameters:**

* **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.
* **relin_keys** (RelinKeys): A reference to a **RelinKeys** object,representing the relinearization key.
* **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
  <br>


### 10. Ciphertext row rotation : **<font color='red'> rotate_col</font>**

```c
void rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const override;
```

**Description**：This function is used to perform a column rotation operation on a ciphertext.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **galois_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption keys used for row rotation.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after column rotation.
  <br>


### 11. Ciphertext column rotation : **<font color='red'> rotate_row</font>**

```c
void rotate_row(const Ciphertext &ciph,int rot_step, const GaloisKeys &galois_keys, Ciphertext &result) const override;
```

**Description**：This function is used to perform a row rotation operation on a ciphertext.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **rot_step** (int): An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.<br>
- **galois_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption keys used for row rotation.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after row rotation.

<br>

### 12. Modulo drop: **<font color='red'> drop_modulus</font>** 

```c
void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;
```

**Description**：This function drops the modulus at specific level . It switches a ciphertext encrypted by modulo q_1...q_k into a ciphertext encrypted modulus down to q_1...q_{k-1}.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after dropping the modulus.

* **level** (uint32_t):  The modulus level to switch into.

<br>

### 13. Fast Fourier Transform (forward) : **<font color='red'> ftt_fwd</font>** 

```c
void ftt_fwd(const Plaintext &plain, Plaintext &result) const override;
void ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const override;
```

**Description**: This function is used for the Fast Fourier Transform (FFT) of a plaintext or ciphertext.

**Parameters**: 

- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the transformed plaintext or ciphertext.

<br>

### 14. Fast Fourier Transform (inverse) : **<font color='red'> ftt_inv</font>**

```c
void ftt_inv(const Plaintext &plain, Plaintext &result) const override;
void ftt_inv(const Ciphertext &ciph, Ciphertext &result) const override;
```

**Description**: This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 

- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.




<!-- ### 8. Key switching : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**：This function switches the key of a ciphertext.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys. -->



<!-- ### 8. Ciphertext and plaintext matrix multiplication : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```c
void multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) override;
```


**Description**：This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat** (MatrixPlain):  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.
  -->
