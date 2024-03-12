# BGV 

## Data Structures


Poseidon Supported parameter data structures：

<br>


### 1. Memory address management : **<font color='red'><span id="MemoryManager">MemoryManager</span> </font>**

**Description**：MemoryManager is a class used to manage addresses and memory space in hardware mode. It is not recommended for users to use it directly.


<br>



### 2. Parameter class of the BGV encryption scheme : **<font color='red'>BGVParametersLiteralDefault</font>**

**Description**： BGVParametersLiteralDefault is a class for initialization encryption parameters.

**Functions**:
```cpp
BGVParametersLiteralDefault(DegreeType degreeType);
```
- **degreeType** (DegreeType): Indicates the degree of a polynomial. The optional values are "degree_2048", "degree_4096", "degree_8192", "degree_16384", or "degree_32768"

: Constructs bgv default parameters.



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


**Functions**：
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

​     


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




### 9. BGV encryption scheme  class : **<font color='red'>BatchEncoder</font>**

**Description**：BatchEncoder is a class used for encoding and decoding in the BGV/BFV encryption scheme.<br>


**Functions**：
```cpp
BatchEncoder(const PoseidonContext &context);
```
- **context** (const PoseidonContext &): The poseidon context.

: Creates a BatchEncoder. It is necessary that the encryption parameters given through the PoseidonContext object support batching.


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


<!-- ### 10. Plaintext matrix : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**

**Description**：MatrixPlain is a class for storing plaintext matrix information.

**Members**：
- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (uint32_t): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (mpf_class): Indicates the scaling factor of the matrix.

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.
 -->




<br>

### 10. Key generation class : **<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**

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






### 11. Encryption class : **<font color='red'><span id="Encryptor">Encryptor</span> </font>**

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


### 12. Decryption class : **<font color='red'><span id="Decryptor">Decryptor</span> </font>**

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




### 13. Evaluator Factory  class ：**<font color='red'><span id="EvaluatorFactory">EvaluatorFactory</span> </font>**

**Description**：EvaluatorFactory is a class used to create the Poseidon algorithm library.

**Members**：null.

**Functions**： 
```cpp
auto create(const PoseidonContext &context) ->std::shared_ptr< Envaluator >;
```
: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted full-homomorphic encryption algorithms such as BFV, BGV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  







## Evaluation Functions
### 1. Addition between ciphertexts : **<font color='red'> add</font>**

```c
void add(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
```

**Description**：This function performs homomorphic addition on two ciphertexts.<br>

**Parameters**：

- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.

<br>

### 2. Addition of ciphertext and plaintext : **<font color='red'> add_plain</font>**

```c
void add_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const override;
```

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**：

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.

<br>

### 3. Ciphertext and ciphertext subtraction : **<font color='red'> sub</font>**

```c
void sub(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
```

**Description**: This function performs homomorphic subtraction between two ciphertexts.<br>

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

### 5. Multiplication between ciphertexts : **<font color='red'> multiply</font>**

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

**Description**:This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**:

- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.

<br>

### 8.  Relinearization : **<font color='red'> relinearize</font>**

```c
void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const override;
```

**Description**: This function  performs relinearization operation on the ciphertext.

**Parameters:**

* **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.
* **relin_keys** (RelinKeys): A reference to a **RelinKeys** object,representing the relinearization key.
* **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.

<br>

### 9. Ciphertext row rotation : **<font color='red'> rotate_col</font>**

```c
void rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const override;
```

**Description**：This function is used to perform a column rotation operation on a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **galois_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption keys used for row rotation.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after column rotation.

<br>

### 10. Ciphertext column rotation : **<font color='red'> rotate_row</font>**

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

### 11. Modulo drop: **<font color='red'> drop_modulus</font>**
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

### 12. Fast Fourier Transform (forward) : **<font color='red'> ftt_fwd</font>** 

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

### 13. Fast Fourier Transform (inverse) : **<font color='red'> ftt_inv</font>**

```c
void ftt_inv(const Plaintext &plain, Plaintext &result) const override;
void ftt_inv(const Ciphertext &ciph, Ciphertext &result) const override;
```

**Description**: This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 

- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.

<br>

<!-- ### 8. Key switching : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**：This function switches the key of a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys. -->



<!-- ### 8. Ciphertext and plaintext matrix multiplication : **<font color='red'> multiplyByDiagMatrixBSGS</font>** -->

<!-- ```c
void multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) override;

```


**Description**：This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat** (MatrixPlain):  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations. -->

