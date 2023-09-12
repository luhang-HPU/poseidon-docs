# CKKS 

## Data Structures




Poseidon Supported parameter data structures：

<br>


### 1. Memory address management : **<font color='red'><span id="MemoryManager">MemoryManager</span> </font>**


**Description**： MemoryPool is a class used to manage addresses and memory space in hardware mode. It is not recommended for users to it directly.


<br>


### 2. Parameter class of the CKKS encryption scheme : **<font color='red'>CKKSParametersLiteralDefault</font>**

**Description**： CKKSParametersLiteralDefault is a class for initialization encryption parameters.

**Functions**:
```cpp
CKKSParametersLiteralDefault(DegreeType degreeType);
```
- **degreeType** (DegreeType): Indicates the degree of a polynomial. The optional values are "degree_2048", "degree_4096", "degree_8192", "degree_16384", or "degree_32768"

: Constructs ckks default parameters.


<br>


### 3. Context information management class : **<font color='red'>PoseidonContext</font>**

**Description**： PoseidonContext is a class used to generate and manage context information.

**Functions**：
```cpp
PoseidonContext(const ParametersLiteral &paramLiteral);
```
- **paramLiteral** (const ParametersLiteral &): Indicates the encryption parameter used to calculate context information.

: Constructs poseidon context.

<br>

```cpp
const uint32_t poly_degree() const; 
```
: The degree of the polynomial is returned.

<br>

```cpp
const mpf_class scaling_factor() const;
```
: The scaling_factor is returned.

<br>

```cpp
const SchemeType scheme_type() const;
```
: The scheme_type is returned，CKKS or BFV or BGV.

<br>


```cpp
const uint32_t hamming_weight() const;
```
: The hamming_weight is returned.

<br>


```cpp
void set_random_generator(std::shared_ptr< UniformRandomGeneratorFactory> random_generator);
```
- **random_generator** (std::shared_ptr< UniformRandomGeneratorFactory>): A shared pointer of the random generator.

: Used to set up a random number generator. 

<br>


```cpp
std::shared_ptr<UniformRandomGeneratorFactory> random_generator() const;
```
: The random number generator is returned.

<br>


```cpp
std::shared_ptr<poseidon::CrtContext> crt_context() const;
```
: The CRT context is returned.

<br>




### 4. Plaintext class : **<font color='red'><span id="Plaintext">Plaintext</span> </font>** 


**Description**： Plaintext is a class for storing plaintext information.

**Functions**：
```cpp
Plaintext();
```
: Constructs an empty plaintext allocating no memory.

<br>

```cpp
Plaintext(const Plaintext &copy);
```
- **copy** (const Plaintext &): The given Plaintext

: Creates a new plaintext by copying a given one.

<br>


```cpp
Plaintext(Plaintext &&assign);
```
- **assign** (Plaintext &&): The given Plaintext

: Creates a new plaintext by moving a given one.

<br>


```cpp
Plaintext &operator=(const Plaintext &copy);
```
- **copy** (const Plaintext &): The given Plaintext

: Copies a given plaintext to the current one.

<br>


```cpp
Plaintext &operator=(Plaintext &&assign);
```
- **assign** (Plaintext &&): The given Plaintext

: Moves a new plaintext by moving a given one.

<br>

```cpp
const RNSPolynomial* poly() const;
```
: Used to get a polynomial pointer.

<br>


```cpp
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.

<br>


```cpp
void newMetaData(const mpf_class &scaling_factor,bool isNTT,uint32_t level,uint32_t poly_degree);
```
- **scaling_factor** (const mpf_class &): The scaling factor.
- **isNTT** (bool): The plain is NTT form or not.
- **level** (uint32_t): The level in modulus chain.
- **poly_degree** (uint32_t): The degree of polynomial.

: A function to create new metadata.

<br>


```cpp
void newPoly(const PoseidonContext &context,uint32_t level);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **level** (level): The level in modulus chain.

: A function for creating a new polynomial.

<br>

     


### 5. Ciphertext class : **<font color='red'><span id="Ciphertext">Ciphertext</span> </font>** 


**Description**： Ciphertext is a class for storing ciphertext information.

**Functions**：
```cpp
Ciphertext();
```
: Constructs an empty ciphertext allocating no memory.

<br>

```cpp
Ciphertext(const Ciphertext &copy);
```
- **copy** (const Ciphertext &): The given Ciphertext

: Creates a new ciphertext by copying a given one.

<br>

```cpp
Ciphertext(Ciphertext &&source) = default;
```
- **source** (Ciphertext &&): The given Ciphertext

: Moves a new ciphertext by moving a given one.

<br>

```cpp
Ciphertext &operator=(const Ciphertext &copy);
```
- **copy** (const Ciphertext &): The given Ciphertext

: Copies a given ciphertext to the current one.

<br>

```cpp
Ciphertext &operator=(Ciphertext &&assign);
```
- **assign** (Ciphertext &&): The given Ciphertext

: Moves a given ciphertext to the current one.

<br>


```cpp
void newPoly(const PoseidonContext &context, uint32_t rns_num_q, uint32_t rns_num_p);
```
- **context** (const PoseidonContext &):
- **rns_num_q** (uint32_t):
- **rns_num_p** (uint32_t):

: A function for creating a new polynomial.

<br>

```cpp
void newMetaData(const mpf_class &scaling_factor, bool isNTT, uint32_t level, uint32_t poly_degree);
```
- **scaling_factor** (const mpf_class &): 
- **isNTT** (bool):
- **level** (uint32_t):
- **poly_degree** (uint32_t):

: A function to create new metadata.

<br>

```cpp
bool isValid();
```
: Used to check whether the ciphertext polynomial is valid.

<br>

```cpp
RNSPolynomial* c0() const;
```
: The function used to get the first part of the polynomial pointer. 

<br>

```cpp
RNSPolynomial* c1() const;
```
: A function to get a pointer to the second part polynomial.

<br>

<br>

```cpp
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.

<br>


### 6. Public key class : **<font color='red'><span id="PublicKey">PublicKey</span> </font>**  
**Description**： PublicKey is a class for storing public key information.

**Functions**：
```cpp
PublicKey();
```

: Constructs an empty publicKey allocating no memory.

<br>

```cpp
PublicKey(const PublicKey &copy) = default;
```
- **copy** (const PublicKey &): The given PublicKey

: Creates a new publicKey by copying a given one.

<br>

```cpp
PublicKey(PublicKey &&source) = default;
```
- **source** (PublicKey &&): The given PublicKey

: Moves a new publicKey by moving a given one.

<br>

```cpp
PublicKey &operator=(const PublicKey &assign) = default;
```
- **assign** (const PublicKey &assign): The given PublicKey

: Copies a given publicKey to the current one.

<br>

```cpp
PublicKey &operator=(PublicKey &&assign) = default;
```
- **assign** (PublicKey &&): The given PublicKey

: Moves a given publicKey to the current one.

<br>

```cpp
const Ciphertext data() const noexcept;
```
: The function is used to get the key.

<br>





### 7. Relinearize key class : **<font color='red'><span id="RelinKeys">RelinKeys</span> </font>** 


**Description**：RelinKeys is a class for storing relinearized key information.


**Functions**：

```cpp
RelinKeys();
```
: Constructs an empty RelinKeys allocating no memory.

<br>

```cpp
RelinKeys(const RelinKeys &copy) = default;
```
- **copy** (const RelinKeys &): The given RelinKeys

: Creates a new RelinKeys by copying a given one.

<br>

```cpp
RelinKeys(RelinKeys &&source) = default;
```
- **source** (RelinKeys &&): The given RelinKeys

: Moves a new RelinKeys by moving a given one.

<br>

```cpp
RelinKeys &operator=(const RelinKeys &assign) = default;
```
- **assign** (const RelinKeys &): The given RelinKeys

: Copies a given RelinKeys to the current one.

<br>

```cpp
RelinKeys &operator=(RelinKeys &&assign) = default;
```
- **assign** (RelinKeys &&): The given RelinKeys

: Moves a given RelinKeys to the current one.

<br>

```cpp
std::vector<std::vector<PublicKey>> &data();
```
: The function is used to get the key.

<br>

```cpp
inline std::size_t size() const noexcept;
```
: The function is used to get the number of keys.

<br>




### 8. **<font color='red'><span id="GaloisKeys">GaloisKeys</span> </font>**:  Galois key class 

**Description**：GaloisKeys is a class for storing Galoiskeys information.

**Functions**： 

```cpp
GaloisKeys();
```
: Constructs an empty GaloisKeys allocating no memory.

<br>

```cpp
GaloisKeys(const GaloisKeys &copy) = default;
```
- **copy** (const GaloisKeys &): The given GaloisKeys

: Creates a new GaloisKeys by copying a given one.

<br>

```cpp
GaloisKeys(GaloisKeys &&source) = default;
```
- **source** (GaloisKeys &&): The given GaloisKeys

: Moves a new GaloisKeys by moving a given one.

<br>

```cpp
GaloisKeys &operator=(const GaloisKeys &assign) = default;
```
- **assign** (const GaloisKeys &): The given GaloisKeys

: Copies a given GaloisKeys to the current one.

<br>

```cpp
GaloisKeys &operator=(GaloisKeys &&assign) = default;
```
- **assign** (GaloisKeys &&): The given GaloisKeys

: Moves a given GaloisKeys to the current one.

<br>

```cpp
static inline std::size_t get_index(uint32_t galois_elt);
```
- **galois_elt** (uint32_t):The Galois element.

: A function used to obtain the corresponding index of a Galois element.

<br>


```cpp
inline bool has_key(uint32_t galois_elt) const;
```
- **galois_elt** (uint32_t):The Galois element.

: A function used to determine whether a given Galois element exists.

<br>



### 9. **<font color='red'><span id="CKKSEncoder">CKKSEncoder</span> </font>** : CKKS encryption scheme  class  

**Description**： CKKSEncoder is a class for encoding and decoding CKKS encryption schemes.

- **paramLiteral** (const ParametersLiteral): Indicates the encryption parameter used to calculate context information.

**Functions**：
```cpp
void encode(vector<complex<double>> src, Plaintext &plain, const mpf_class scaling_factor);
```
- **src** (vector<complex<double>> ): The source message.
- **plain** (Plaintext &): The plaintext which is used to store the computation result.
- **scaling_factor** (const mpf_class): The scaling factor during encode.

: A function used to encode a complex vector into a plaintext polynomial.
<br>


```cpp
void decode(const Plaintext &plain, vector<complex<double>> &rec);
```
- **plain** (Plaintext &): The plaintext.
- **rec** (const mpf_class): The result of message.

: A function used to decode plaintext polynomials into complex vectors. 

<br>




### 10. Plaintext matrix : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**


**Description**： MatrixPlain is a class for storing plaintext matrix information.
 
**Members**：
- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (int): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (mpf_class): Indicates the scaling factor of the matrix.

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.


<br>

### 11. Key generation class : **<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**

**Description**： KeyGenerator is a class for generating keys.

**Functions**：
```cpp
KeyGenerator(const PoseidonContext &params);
```
- **params** (const PoseidonContext &): The poseidon context.

: Constructs the key generator without keys.


<br>


```cpp
KeyGenerator(const PoseidonContext& params,const SecretKey &sk);
```
- **params** (const PoseidonContext &): The poseidon context.
- **sk** (const SecretKey &): The secret key.

: Constructs the key generator with secret key.

<br>

```cpp
inline void create_public_key(PublicKey &destination) const;
```
- **destination** (PublicKey &):The PublicKey which is used to store the computation result.

: A function for creating a public key. 

<br>

```cpp
inline void create_relin_keys(RelinKeys &destination);
```
- **destination** (RelinKeys &):The RelinKeys which is used to store the computation result.

: A function to create a relinearized key.

<br>

```cpp
inline void create_galois_keys(const std::vector<int> &steps, GaloisKeys &destination);
```
- **steps** (const std::vector<int> &): The group of rotate steps which is used to create galois keys.
- **destination** (RelinKeys &):A GaloisKeys which is used to store the computation result.

: A function used to create a rotation key based on a given rotation step vector.

<br>

```cpp
inline void create_conj_keys(GaloisKeys &destination);
```
- **destination** (RelinKeys &): The GaloisKeys which is used to store the computation result.

: A function used to create a conjugate rotation key. It is only used in CKKS conjugate.

<br>





### 12. Encryption class : **<font color='red'><span id="Encryptor">Encryptor</span> </font>**

**Description**： Encryptor is a class used to encrypt plaintext.

**Functions**：

```cpp
Encryptor(const PoseidonContext &context, const SecretKey &secret_key);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **secret_key** (const SecretKey &): The secret key.

: Creates an Encryptor instance initialized with the specified PoseidonContext and secret key.

<br>

```cpp
Encryptor(const PoseidonContext &context, const PublicKey &public_key);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **public_key** (const PublicKey &): The public key.

: Creates an Encryptor instance initialized with the specified PoseidonContext and public key.

<br>

```cpp
Encryptor(const PoseidonContext &context, const PublicKey &public_key, const SecretKey &secret_key);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **public_key** (const PublicKey &): The public key.
- **secret_key** (const SecretKey &): The secret key.

: Creates an Encryptor instance initialized with the specified PoseidonContext,secret key, and public key.

<br>


```cpp
void encrypt(const Plaintext &plain, Ciphertext &destination)const;
```
- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** ( Ciphertext &): The ciphertext to overwrite with the encrypt.

: A function used to encrypt plaintext with public key.

<br>


```cpp
void encrypt_with_secret_key(const Plaintext &plain, Ciphertext &destination) const;
```
- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** ( Ciphertext &): The ciphertext to overwrite with the encrypt.

: A function used to encrypt plaintext with secret key.

<br>


```cpp
void set_secret_key(const SecretKey &secret_key);
```
- **secret_key** (const SecretKey &): The secret key.

: Give a new instance of secret key.

<br>


```cpp
void set_public_key(const PublicKey &public_key)
```
- **public_key** (const PublicKey &): The public key.

: Give a new instance of public key.

<br>


### 13. Decryption class : **<font color='red'><span id="Decryptor">Decryptor</span> </font>**

**Description**： Decryptor is a class for decrypting plaintext.

**Functions**：
```cpp
Decryptor(const PoseidonContext &context, const SecretKey &secret_key);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **secret_key** (const SecretKey &): The secret key.

: Creates a Decryptor instance initialized with the specified PoseidonContext and secret key.

<br>


```cpp
void decrypt(const Ciphertext &encrypted, Plaintext &destination);
```
- **encrypted** (const Ciphertext &): The ciphertext to decrypt.
- **destination** (Plaintext &): The plaintext to overwrite with the decrypted.

: Decrypts a Ciphertext and stores the result in the destination parameter.

<br>



### 14.  Evaluator Factory  class : **<font color='red'><span id="EvaluatorFactory">EvaluatorFactory</span> </font>**

**Description**： EvaluatorFactory is a class used to create the Poseidon algorithm library.

**Functions**： 
```cpp
auto create(const PoseidonContext &context)->std::shared_ptr< Envaluator >;
```
: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted full-homomorphic encryption algorithms such as BFV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  







## Evaluation Functions
### 1. Addition between ciphertexts : **<font color='red'> add</font>**

```c
void add(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**： This function performs homomorphic addition on two ciphertexts.<br>

**Parameters**：
- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>


### 2. Addition of ciphertext and plaintext : **<font color='red'> add_plain</font>**

```c
void add_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>


### 3. Subtraction between ciphertexts : **<font color='red'> sub</font>**

```c
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**： This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameters**：
- **ciph1** (Ciphertext):  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>


### 4. Multiplication between ciphertexts : **<font color='red'> multiply</font>**

```c
void multiply(Ciphertext &ciph0, Ciphertext &ciph1, Ciphertext &result, const RelinKeys &relin_key) override;
```

**Description**：This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**：
- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.


<br>


### 5. Multiplication of ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```c
void multiply_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**： This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>


<br>


### 6. Rescale : **<font color='red'> rescale</font>**

```c
void rescale(Ciphertext &ciph) override;
```

**Description**： This function performs a rescaling operation on a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>


<br>


### 7. Ciphertext rotation : **<font color='red'> rotate</font>**

```c
void rotate(Ciphertext &ciph, Ciphertext &result, const GaloisKeys &rot_key,  int k) override;
```

**Description**： This function is used to rotate a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store  the rotated ciphertext.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.<br>
- **k** (int):  An integer, indicating the rotation step length. A positive number represents a rotation to the right, and a negative number represents a rotation to the left.



<br>


### 8. Take conjugate : **<font color='red'> conjugate</font>**

```c
void conjugate(Ciphertext &ciph, Ciphertext &result, const GaloisKeys &conj_key) override;
```

**Description**： This function  is used to conjugate a ciphertext, that is, to transform each of its elements into its complex conjugate.<br>

**Parameters**：
- **ciph** (Ciphertext):    A reference to a **Ciphertext** object, representing the ciphertext to be conjugated.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the resulting ciphertext after the conjugation.<br>
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.<br>


<br>



### 9. Matrix multiplication of ciphertext and plaintext : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```c
void multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) override;
```

**Description**： This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat** (MatrixPlain):  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.


<br>


### 10. Coefficient to plaintext slot : **<font color='red'> coeff_to_slot</font>**

```c
void coeff_to_slot(Ciphertext &ciph, LinearMatrixGroup &matrix_group, Ciphertext &result_real, Ciphertext &result_imag, const GaloisKeys &rot_key, const GaloisKeys &conj_key, CKKSEncoder &encoder) override;
```

**Description**：  This function is used to transform a ciphertext from the coefficient domain to the slot domain.

**Parameters**:  
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result_real** (Ciphertext): A reference to a **Ciphertext** object, used to store the real part of the transformed ciphertext.
- **result_imag** (Ciphertext): A reference to a **Ciphertext** object, used to store the imaginary part of the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.

<br>



### 11. Plaintext slot to coefficient : **<font color='red'> slot_to_coeff</font>**

```c
void slot_to_coeff(Ciphertext &ciph_real, Ciphertext &ciph_imag, LinearMatrixGroup &matrix_group, Ciphertext &result, const GaloisKeys &rot_key, const GaloisKeys &conj_key, CKKSEncoder &encoder) override;
```

**Description**： This function is used to transform a ciphertext from the slot domain to the coefficient domain.

**Parameters**: 
- **ciph_real** (Ciphertext): A reference to a **Ciphertext** object, representing the real part of the ciphertext to be transformed.
- **ciph_imag** (Ciphertext): A reference to a **Ciphertext** object, representing the imaginary part of the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.


<br>



### 12. Fast Fourier Transform (forward) : **<font color='red'> ftt_fwd</font>** 

```c
void ftt_fwd(Plaintext &plain ,Plaintext &result)
void ftt_fwd(Ciphertext &ciph, Ciphertext &result) override;
```

**Description**： This function is used for the Fast Fourier Transform (FFT) of a plaintext or ciphertext.

**Parameters**: 
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the transformed plaintext or ciphertext.


<br>


### 13. Fast Fourier Transform (inverse) : **<font color='red'> ftt_inv</font>**

```c
void ftt_inv(Plaintext &plain ,Plaintext &result) override;
void ftt_inv(Ciphertext &ciph ,Ciphertext &result) override;
```

**Description**： This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.


<br>



### 14. Read ciphertext information from accelerator card : **<font color='red'> read</font>**

```c
void read(Ciphertext &ciph) override;
```

**Description**： This function is used to read data from the acceleration card in the ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing the ciphertext to be read.



<br>

### 15. Multiplication of ciphertext and complex constant : **<font color='red'> multiply_const</font>**

```c
void multiply_const(Ciphertext &ciph, complex<double> constData, Ciphertext &result, CKKSEncoder &encoder, bool isDirect = false) override;
```

**Description**： This function is used for the homomorphic multiplication of a ciphertext and a complex constant.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **constData** (complex < double> ): A complex<double> object, representing a complex constant.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.
- **isDirect** (bool): A boolean indicating whether to directly multiply the ciphertext, instead of first encoding the constant to plaintext and then multiplying.


<br>


### 16. Addition of ciphertext and complex constant : **<font color='red'> add_const</font>**

```c
void add_const(Ciphertext &ciph, double constData, Ciphertext &result) override;
```

**Description**： This function is used for the homomorphic addition of a ciphertext and a real constant.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **constData** (double): A value of type double, representing a real constant.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.



<br>

### 17. Discrete Fourier Transform on ciphertext : **<font color='red'> dft</font>**

```c
void dft(Ciphertext &ciph, LinearMatrixGroup &matrix_group, Ciphertext &result, const GaloisKeys &rot_key) override;
```

**Description**： This function is used for the Discrete Fourier Transform (DFT) of a ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the DFT.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.


<br>


### 18. Dynamic rescale : **<font color='red'> rescale_dynamic</font>**

```c
void rescale_dynamic(Ciphertext &ciph, mpf_class scale);
```

**Description**： This function is used for dynamic rescaling of a ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **scale** (mpf_class): An mpf_class object, representing the given scaling factor.

<br>


### 19. Polynomial evaluation : **<font color='red'> evaluatePolyVector</font>**

```c
void evaluatePolyVector(Ciphertext &ciph, Ciphertext &destination, const PolynomialVector &polys, mpf_class scalingfactor, const RelinKeys &relin_key, CKKSEncoder &encoder**) override;
void evaluatePolyVector(Ciphertext &ciph, Ciphertext &destination, const PolynomialVector &polys, mpf_class scalingfactor, const RelinKeys &relin_key, CKKSEncoder &encoder, Decryptor &dec);
```

**Description**： This function is used to evaluate a polynomial. The coefficients of Chebyshev or Taylor polynomials are pre-calculated, and then the polynomial calculation is performed.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **destination** (Ciphertext): A reference to a **Ciphertext** object, used to store the evaluated ciphertext.
- **polys** (PolynomialVector): A constant reference to a **PolynomialVector** object, representing the given polynomial vector.
- **scalingfactor** (mpf_class): An **mpf_class** object, representing the given scaling factor.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.
- **dec** (Decryptor) (optional): A reference to a **Decryptor** object, representing the decryptor.


<br>


### 20. Evaluate modulo on ciphertext vector : **<font color='red'> eval_mod</font>**

```c
void eval_mod(Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly, const RelinKeys &relin_key, CKKSEncoder &encoder) override;
```

**Description**： This function is used to perform modular arithmetic on a ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after the modular arithmetic.
- **eva_poly** (EvalModPoly): A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.

<br>


### 21. Bootstrap : **<font color='red'> bootstrap</font>**

```c
void bootstrap(Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly, LinearMatrixGroup & matrix_group0, LinearMatrixGroup &matrix_group1, const RelinKeys &relin_key, const GaloisKeys &rot_key, const GaloisKeys &conj_key, CKKSEncoder &encoder) override;
```

**Description**： This function is used to bootstrap a ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the bootstrapped ciphertext.
- **eva_poly** (EvalModPoly): A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **matrix_group0** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the first group of linear matrices used for bootstrapping.
- **matrix_group1** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the second group of linear matrices used for bootstrapping.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation operations.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.


<br>


<!-- ### 22. Key switch : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**： This function switches the key of a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys. -->






