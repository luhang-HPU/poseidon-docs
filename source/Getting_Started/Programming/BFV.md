# BFV 

## Data Structures





Poseidon Supported parameter data structures：


### 1. Memory address management : **<font color='red'><span id="MemoryPool">MemoryPool</span> </font>**

**Description**：MemoryPool is a class for managing addresses and memory space.

**Members**：
- **degree_type** (DegreeType): Indicates the degree of a polynomial. The optional values are "degree_2048", "degree_4096", "degree_8192", "degree_16384", or "degree_32768" (default) .

**Functions**： 
```cpp
getInstance(DegreeType degree_type);
```
: Used to create Memorypool.



### 2. Parameter class of the BFV encryption scheme : **<font color='red'>BFVParametersLiteralDefault</font>**


**Description**：BFVParametersLiteralDefault is a class used for initializing encryption parameters.

**Members**：
- **degreeType** (DegreeType): Represents the degree of the polynomial.

**Functions**： The functions consist only of the constructor.

### 3. Context information management class : **<font color='red'>PoseidonContext</font>**


**Description**：PoseidonContext is a class used to generate and manage context information.


**Members**：
- **paramLiteral** (const ParametersLiteral): Indicates the encryption parameter used to calculate context information.

**Functions**：
```cpp
void const uint32_t poly_degree(); 
```
: The degree of a polynomial is returned.

```cpp
const mpf_class scaling_factor() const;
```
: The scaling_factor is returned.

```cpp
void set_random_generator(std::shared_ptr< UniformRandomGeneratorFactory > random_generator);
```
: Used to set up a random number generator.



### 4. Generate pseudo-random numbers class : **<font color='red'>Blake2xbPRNGFactory </font>**


**Description**：Blake2xbPRNGFactory is a class for generating pseudorandom numbers.

**Members**：null.

**Functions**：Only constructors.




### 5. Plaintext class : **<font color='red'><span id="Plaintext">Plaintext</span> </font>**     

**Description**：Plaintext is a class for storing plaintext information.

**Members**：null.

**Functions**：
```cpp
const RNSPolynomial* poly() const;
```
: Used to get a polynomial pointer.

```cpp
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.

```cpp
int newMetaData(const mpf_class &scaling_factor,bool isNTT,int level,int poly_degree);
```
: A function to create new metadata.

```cpp
int newPoly(const PoseidonContext& context,int level);
```
: A function for creating a new polynomial.
   


### 6. Ciphertext class : **<font color='red'><span id="Ciphertext">Ciphertext</span> </font>**

**Description**：Ciphertext is a class for storing ciphertext information.

**Members**：null.

**Functions**：
```cpp
void newPoly(const PoseidonContext& context, int rns_num_q, int rns_num_p);
```
: A function for creating a new polynomial.

```cpp
void newMetaData(const mpf_class &scaling_factor, bool isNTT, int level, int poly_degree);
```
: A function to create new metadata.

```cpp
bool isValid();
```
: Used to check whether the ciphertext polynomial is valid.

```cpp
RNSPolynomial* c0() const;
```
: The function used to get the first part of the polynomial pointer. 

```cpp
RNSPolynomial* **c1() const;
```
: A function to get a pointer to the second part polynomial.

```cpp
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.




### 7. Public key class : **<font color='red'><span id="PublicKey">PublicKey</span> </font>**

**Description**：PublicKey is a class for storing public key information.

**Members**：null.

**Functions**：
```cpp
inline Ciphertext data() const noexcept;
```
: The function is used to get the key.




### 8. Relinearize key class : **<font color='red'><span id="RelinKeys">RelinKeys</span> </font>**

**Description**：RelinKeys is a class for storing relinearized key information.
 ();

**Members**：null.

**Functions**：
```cpp
inline std::size_t size() const noexcept;
```
: The function is used to get the number of keys.

```cpp
inline auto &data() noexcept;
```
: The function is used to get the address of the key data.


### 9. Galois key class : **<font color='red'><span id="GaloisKeys">GaloisKeys</span> </font>**

**Description**：GaloisKeys is a class for storing Galoiskeys information.

**Members**：null.

**Functions**： 
```cpp
static inline std::size_t get_index(std::uint32_t galois_elt);
```
: A function used to obtain the corresponding index of a Galois element.

```cpp
inline bool has_key(std::uint32_t galois_elt) const;
```
: A function used to determine whether a given Galois element exists.



### 10. BFV encryption scheme  class : **<font color='red'>BatchEncoder</font>**


**Description**：BatchEncoder is a class used for encoding and decoding in the BFV encryption scheme.<br>

**Members**：
- **paramLiteral** (const ParametersLiteral): Represents the encryption parameters used for computing context information.<br>

**Functions**：
```cpp
int encode(vector<complex<double>> vec, Plaintext &plain, const mpf_class scaling_factor);
```
: A function used to encode a complex number vector into a plaintext polynomial.

```cpp
int decode(const Plaintext &plain, vector<complex<double>>& vec);
```
: A function used to decode a plaintext polynomial into a complex number vector.

### 11. Plaintext matrix : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**

**Description**：MatrixPlain is a class for storing plaintext matrix information.

**Members**：
- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (int): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (mpf_class): Indicates the scaling factor of the matrix.

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.






### 12. Key generation class : **<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**

**Description**：KeyGenerator is a class for generating keys.

**Members**：
- **paramLiteral (const ParametersLiteral)**: Indicates the encryption parameter used to calculate context information.

**Functions**：
```cpp
inline void create_public_key(PublicKey &destination) const;
```
: A function for creating a public key. 

```cpp
inline void create_relin_keys(RelinKeys &destination);
```
: A function to create a relinearized key.

```cpp
inline void create_galois_keys(const std::vector<int> &steps, GaloisKeys &destination);
```
: A function used to create a rotation key based on a given rotation step vector.

```cpp
inline void create_conj_keys(GaloisKeys &destination);
```
: A function used to create a conjugate rotation key.





### 13. Encryption class : **<font color='red'><span id="Encryptor">Encryptor</span> </font>**

**Description**：Encryptor is a class used to encrypt plaintext.

**Members**：

- **paramLiteral** (const ParametersLiteral): Indicates the encryption parameter used to calculate context information.

- **secret\_key** (const SecretKey)：Indicates the key involved in the encryption operation.

**Functions**：
```cpp
void encrypt(const Plaintext &plain, Ciphertext &destination)const;
```
: A function used to encrypt plaintext.





### 14. Decryption class ：**<font color='red'><span id="Decryptor">Decryptor</span> </font>**

**Description**：Decryptor is a class for decrypting plaintext.

**Members**：

- **paramLiteral** (const ParametersLiteral): Indicates the encryption parameter used to calculate context information.

- **secret\_key** (const SecretKey)：Indicates the key involved in the encryption operation.

**Functions**：
```cpp
void decrypt(const Plaintext &plain, Ciphertext &destination) const;
```
: A function used to decrypt the plaintext.





### 15. Evaluator Factory  class ：**<font color='red'><span id="EvaluatorFactory">EvaluatorFactory</span> </font>**

**Description**：EvaluatorFactory is a class used to create the Poseidon algorithm library.

**Members**：null.

**Functions**： 
```cpp
auto create(const PoseidonContext &context) ->std::shared_ptr< Envaluator >;
```
: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted full-homomorphic encryption algorithms such as BFV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  







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



### 2. Ciphertext and plaintext addition : **<font color='red'> add_plain</font>**

```c
void add_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.



### 3. Ciphertext and ciphertext subtraction : **<font color='red'> sub</font>**

```c
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**：This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameters**：
- **ciph1** (Ciphertext):  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.



### 4. Ciphertext and ciphertext multiplication : **<font color='red'> multiply</font>**

```c
void multiply(Ciphertext &ciph0, Ciphertext &ciph1, Ciphertext &result, const RelinKeys &relin_key) override;
```

**Description**：This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**：
- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.



### 5. Ciphertext and plaintext multiplication : **<font color='red'> **multiply_plain**</font>**

```c
void multiply_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**：This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>



### 6. Rescaling : **<font color='red'> rescale</font>**

```c
void rescale(Ciphertext &ciph) override;
```

**Description**：This function performs a rescaling operation on a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>



### 7. Ciphertext row rotation : **<font color='red'> rotate_col</font>**

```c
void rotate_col(Ciphertext &encrypted, const GaloisKeys &galois_keys, Ciphertext &destination);
```

**Description**：This function is used to perform a column rotation operation on a ciphertext.<br>

**Parameters**：
- **encrypted** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **galois_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption keys used for row rotation.<br>
- **destination** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after rotation.



### 8. Ciphertext column rotation : **<font color='red'> rotate_row</font>**

```c
void rotate_row(Ciphertext &encrypted, int rot_step, const GaloisKeys &galois_keys, Ciphertext &destination);
```

**Description**：This function is used to perform a row rotation operation on a ciphertext.<br>

**Parameters**：
- **encrypted** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **rot_step** (int): An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.<br>



### 9. Key switching : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**：This function switches the key of a ciphertext.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys.



### 10. Ciphertext and plaintext matrix multiplication : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```c
void multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) override;
```


**Description**：This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**：
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat** (MatrixPlain):  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.


