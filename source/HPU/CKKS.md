# CKKS 

## Parameter Data Structure




Poseidon Supported parameter data structures：


### 1. Memory address management : MemoryPool
**<font color='red'><span id="MemoryPool">MemoryPool</span> </font>** (DegreeType **degree_type**)

**Description**： MemoryPool is a class for managing addresses and memory space.

**Parameters**：
- **degree_type**: Indicates the degree of a polynomial. <br>The optional value can be degree_2048,degree_4096, degree_8192, degree_16384, or degree_32768.

**Functions**： 
```c++
getInstance(DegreeType degree_type);
```
: Used to create Memorypool.

### 2. Parameter class of the CKKS encryption scheme : CKKSParametersLiteralDefault
**<font color='red'>CKKSParametersLiteralDefault</font>**  (SchemeType **Type**, uint32_t **LogN**, uint32_t **LogSlots**,  const vector<uint32_t>& **LogQ**,  const vector<uint32_t>& **LogP**, uint32_t **LogScale**, uint32_t **H**, uint32_t **T = 0**, int **q0_level = 0**);

**Description**： CKKSParametersLiteralDefault is a class for initialization encryption parameters.

**Parameters**：
- **degreeType**: Indicates the degree of a polynomial.
- **Type**: Indicates the type of the encryption scheme.
- **LogN**: Indicates the logarithm of the ring degree. 
- **LogSlots**: Indicates the logarithm of slots. 
- **LogQ**: Indicates the logarithm of the modulus of the ciphertext prime number. 
- **LogP**: Indicates the logarithm of the modulus of the key switching auxiliary prime. 
- **LogScale**: Indicates the logarithm of the scaling factor. 
- **H**: Indicates the weight of Hamming. 
- **T**: Indicates the plaintext modulus
- **q0_level**: Indicates the level of q0.

**Functions**：Only constructors.



### 3. Context information management class : PoseidonContext
**<font color='red'>PoseidonContext</font>** (const ParametersLiteral **&paramLiteral**)


**Description**： PoseidonContext is a class used to generate and manage context information.


**Parameters**：
- **paramLiteral**: Indicates the encryption parameter used to calculate context information.

**Functions**：
```c++
void const uint32_t poly_degree(); 
```
: The degree of a polynomial is returned.

```c++
const mpf_class scaling_factor() const;
```
: The scaling_factor is returned.

```c++
void set_random_generator(std::shared_ptr< UniformRandomGeneratorFactory > random_generator);
```
: Used to set up a random number generator.



### 4. Generate pseudo-random numbers class : Blake2xbPRNGFactory
**<font color='red'>Blake2xbPRNGFactory </font>**();



**Description**： Blake2xbPRNGFactory is a class for generating pseudorandom numbers.

**Parameters**：null.

**Functions**：Only constructors.




### 5. Plaintext class : Plaintext
**<font color='red'><span id="Plaintext">Plaintext</span> </font>**();

**Description**： Plaintext is a class for storing plaintext information.

**Parameters**：null.

**Functions**：
```c++
const RNSPolynomial* poly() const;
```
: Used to get a polynomial pointer.

```c++
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.

```c++
int newMetaData(const mpf_class &scaling_factor,bool isNTT,int level,int poly_degree);
```
: A function to create new metadata.

```c++
int newPoly(const PoseidonContext& context,int level);
```
: A function for creating a new polynomial.
     


### 6. Ciphertext class : Ciphertext
**<font color='red'><span id="Ciphertext">Ciphertext</span> </font>**();

**Description**： Ciphertext is a class for storing ciphertext information.

**Parameters**：null.

**Functions**：
```c++
void newPoly(const PoseidonContext& context, int rns_num_q, int rns_num_p);
```
: A function for creating a new polynomial.

```c++
void newMetaData(const mpf_class &scaling_factor, bool isNTT, int level, int poly_degree);
```
: A function to create new metadata.

```c++
bool isValid();
```
: Used to check whether the ciphertext polynomial is valid.

```c++
RNSPolynomial* c0() const;
```
: The function used to get the first part of the polynomial pointer. 

```c++
RNSPolynomial* **c1() const;
```
: A function to get a pointer to the second part polynomial.

```c++
MetaData* metaData() const;
```
: A function that gets Pointers to metadata.



### 7. Public key class : PublicKey
**<font color='red'><span id="PublicKey">PublicKey</span> </font>**();

**Description**： PublicKey is a class for storing public key information.

**Parameters**：null.

**Functions**：
```c++
inline Ciphertext data() const noexcept;
```
: The function is used to get the key.



### 8. Relinearize key class : RelinKeys
**<font color='red'><span id="RelinKeys">RelinKeys</span> </font>**();

**Description**：RelinKeys is a class for storing relinearized key information.


**Parameters**：null.

**Functions**：
```c++
inline std::size_t size() const noexcept;
```
: The function is used to get the number of keys.

```c++
inline auto &data() noexcept;
``` add
: The function is used to get the address of the key data.



### 9. Galois key class : GaloisKeys
**<font color='red'><span id="GaloisKeys">GaloisKeys</span> </font>**();

**Description**：GaloisKeys is a class for storing Galoiskeys information.

**Parameters**：null.

**Functions**： 
```c++
static inline std::size_t get_index(std::uint32_t galois_elt);
```
: A function used to obtain the corresponding index of a Galois element.

```c++
inline bool has_key(std::uint32_t galois_elt) const;
```
: A function used to determine whether a given Galois element exists.


### 10. CKKS encryption scheme  class : CKKSEncoder
**<font color='red'><span id="CKKSEncoder">CKKSEncoder</span> </font>**(const PoseidonContext& **params**);

**Description**： CKKSEncoder is a class for encoding and decoding CKKS encryption schemes.

**Parameters**：
- **paramLiteral**: Indicates the encryption parameter used to calculate context information.

**Functions**：
```c++
int encode(vector<complex<double>> vec, Plaintext &plain, const mpf_class scaling_factor);
```
: A function used to encode a complex vector into a plaintext polynomial.

```c++
int decode(const Plaintext &plain, vector<complex<double>>& vec);
```
: A function used to decode plaintext polynomials into complex vectors. 



### 11. Plaintext matrix : MatrixPlain
**<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**();

**Description**： MatrixPlain is a class for storing plaintext matrix information.
 
**Parameters**：
- **LogSlots**: Indicates the logarithm to 2 of the number of matrix elements.

- **N1**: Indicates the number of rows in a matrix.

- **level**: Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale**: Indicates the scaling factor of the matrix.

- **rot\_index**:Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec**:Indicates the polynomial corresponding to the matrix elements.



### 12. Key generation class : KeyGenerator
**<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**(const PoseidonContext& **params**);

**Description**： KeyGenerator is a class for generating keys.

**Parameters**：
- **paramLiteral**: Indicates the encryption parameter used to calculate context information.

**Functions**：
```c++
inline void create_public_key(PublicKey &destination) const;
```
: A function for creating a public key. 

```c++
inline void create_relin_keys(RelinKeys &destination);
```
: A function to create a relinearized key.

```c++
inline void create_galois_keys(const std::vector<int> &steps, GaloisKeys &destination);
```
: A function used to create a rotation key based on a given rotation step vector.

```c++
inline void create_conj_keys(GaloisKeys &destination);
```
: A function used to create a conjugate rotation key.




### 13. Encryption class : Encryptor
**<font color='red'><span id="Encryptor">Encryptor</span> </font>**(const PoseidonContext **&context**, const SecretKey **&secret\_key**);

**Description**： Encryptor is a class used to encrypt plaintext.

**Parameters**：

- **paramLiteral**: Indicates the encryption parameter used to calculate context information.

- **secret\_key**：Indicates the key involved in the encryption operation.

**Functions**：
```c++
void encrypt(const Plaintext &plain, Ciphertext &destination)const;
```
: A function used to encrypt plaintext.



### 14. Decryption class : Decryptor
**<font color='red'><span id="Decryptor">Decryptor</span> </font>**(const PoseidonContext **&context**, const SecretKey **&secret\_key**);

**Description**： Decryptor is a class for decrypting plaintext.

**Parameters**：

- **paramLiteral**: Indicates the encryption parameter used to calculate context information.

- **secret\_key**：Indicates the key involved in the encryption operation.

**Functions**：
```c++
void decrypt(const Plaintext &plain, Ciphertext &destination) const;
```
: A function used to decrypt the plaintext.



### 15.  Envaluator Factory  class : EnvaluatorFactory
**<font color='red'><span id="EnvaluatorFactory">EnvaluatorFactory</span> </font>**();

**Description**： EnvaluatorFactory is a class used to create the Poseidon algorithm library.

**Parameters**：null.

**Functions**： 
```c++
auto create(const PoseidonContext &context) ->std::shared_ptr< Envaluator >;
```
: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted full-homomorphic encryption algorithms such as BFV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  







## API
### 1. Addition between ciphertexts : add
void **<font color='red'> add</font>** (Ciphertext **&ciph1**, Ciphertext **&ciph2**, Ciphertext **&result**) override;

**Description**： This function performs homomorphic addition on two ciphertexts.<br>

**Parameters**：
- **ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 2. Addition of ciphertext and plaintext : add_plain
void **<font color='red'> add_plain</font>** (Ciphertext **&ciph**, Plaintext **&plain**,Ciphertext **&result**) override;

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 3. Subtraction between ciphertexts : sub
void **<font color='red'> sub</font>** (Ciphertext **&ciph1**, Ciphertext **&ciph2**, Ciphertext **&result**) override;

**Description**： This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameters**：
- **ciph1**:  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
- **ciph2**: A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 4. Multiplication between ciphertexts : multiply
void **<font color='red'> multiply</font>** (Ciphertext **&ciph0**, Ciphertext **&ciph1**, Ciphertext **&result**, const RelinKeys **&relin_key**) override;

**Description**：This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**：
- **ciph1**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key**: A constant reference to a **RelinKeys** object, representing the relinearization key.



### 5. Multiplication of ciphertext and plaintext : multiply_plain
void **<font color='red'> multiply_plain</font>** (Ciphertext **&ciph**, Plaintext **&plain**,Ciphertext **&result**) override;

**Description**： This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**：
- **ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>



### 6. Rescale : rescale
void **<font color='red'> rescale</font>** (Ciphertext **&ciph**) override;

**Description**： This function performs a rescaling operation on a ciphertext.<br>

**Parameters**：
- **ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>



### 7. Ciphertext rotation : rotate
void **<font color='red'> rotate</font>** (Ciphertext **&ciph**, Ciphertext **&result**, const GaloisKeys **&rot_key**,  int **k**) override;

**Description**： This function is used to rotate a ciphertext.<br>

**Parameters**：
- **ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result**: A reference to a **Ciphertext** object, used to store  the rotated ciphertext.<br>
- **rot_key**:   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.<br>
- **k**:  An integer, indicating the rotation step length. A positive number represents a rotation to the right, and a negative number represents a rotation to the left.




### 8. Take conjugate : conjugate
void **<font color='red'> conjugate</font>** (Ciphertext **&ciph**, Ciphertext **&result**, const GaloisKeys **&conj_key**) override;

**Description**： This function  is used to conjugate a ciphertext, that is, to transform each of its elements into its complex conjugate.<br>

**Parameters**：
- **ciph**:    A reference to a **Ciphertext** object, representing the ciphertext to be conjugated.<br>
- **result**: A reference to a **Ciphertext** object, used to store the resulting ciphertext after the conjugation.<br>
- **conj_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.<br>




### 9. Matrix multiplication of ciphertext and plaintext : multiplyByDiagMatrixBSGS
void **<font color='red'> multiplyByDiagMatrixBSGS</font>** (Ciphertext **&ciph**, MatrixPlain **&plain_mat**, Ciphertext **&result**, const GaloisKeys **&rot_key**) override;

**Description**： This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**：
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat**:  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key**:   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.



### 10. Coefficient to plaintext slot : coeff_to_slot
void **<font color='red'> coeff_to_slot</font>** (Ciphertext **&ciph**, LinearMatrixGroup **&matrix_group**, Ciphertext **&result_real**, Ciphertext **&result_imag**, const GaloisKeys **&rot_key**, const GaloisKeys **&conj_key**, CKKSEncoder **&encoder**) override;

**Description**：  This function is used to transform a ciphertext from the coefficient domain to the slot domain.

**Parameters**:  
- **ciph**: A reference to a **Ciphertext** object, representing the ciphertext to be transformed.
- **matrix_group**: A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result_real**: A reference to a **Ciphertext** object, used to store the real part of the transformed ciphertext.
- **result_imag**: A reference to a **Ciphertext** object, used to store the imaginary part of the transformed ciphertext.
- **rot_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.



### 11. Plaintext slot to coefficient : slot_to_coeff
**void** **<font color='red'> slot_to_coeff</font>** (**Ciphertext** &ciph_real, **Ciphertext** &ciph_imag, **LinearMatrixGroup** &matrix_group, **Ciphertext** &result, **const GaloisKeys** &rot_key, **const GaloisKeys** &conj_key, **CKKSEncoder** &encoder) override;

**Description**： This function is used to transform a ciphertext from the slot domain to the coefficient domain.

**Parameters**: 
- **ciph_real**: A reference to a **Ciphertext** object, representing the real part of the ciphertext to be transformed.
- **ciph_imag**: A reference to a **Ciphertext** object, representing the imaginary part of the ciphertext to be transformed.
- **matrix_group**: A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result**: A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.




### 12. Fast Fourier Transform (forward) : ftt_fwd
void **<font color='red'> ftt_fwd</font>** (Plaintext **&plain** ,Plaintext **&result**)<br>
void **<font color='red'> ftt_fwd</font>** (Ciphertext **&ciph**, Ciphertext **&result**) override;

**Description**： This function is used for the Fast Fourier Transform (FFT) of a plaintext or ciphertext.

**Parameters**: 
- **plain**: A reference to a **Plaintext** object, representing a plaintext.
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **result**: A reference to either a **Plaintext** or **Ciphertext** object, used to store the transformed plaintext or ciphertext.



### 13. Fast Fourier Transform (inverse) : ftt_inv
void **<font color='red'> ftt_inv</font>** (Plaintext **&plain** ,Plaintext **&result**) override;<br>
void **<font color='red'> ftt_inv</font>** (Ciphertext **&ciph** ,Ciphertext **&result**) override;

**Description**： This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 
- **plain**: A reference to a **Plaintext** object, representing a plaintext.
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **result**: A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.




### 14. Read ciphertext information from accelerator card : read
void **<font color='red'> read</font>** (Ciphertext **&ciph**) override;

**Description**： This function is used to read data from the acceleration card in the ciphertext.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing the ciphertext to be read.



### 15. Multiplication of ciphertext and complex constant : multiply_const
void **<font color='red'> multiply_const</font>** (Ciphertext **&ciph**, complex<double> **constData**, Ciphertext **&result**, CKKSEncoder **&encoder**, bool **isDirect** = false) override;

**Description**： This function is used for the homomorphic multiplication of a ciphertext and a complex constant.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **constData**: A complex<double> object, representing a complex constant.
- **result**: A reference to a **Ciphertext** object, used to store the computation result.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder.
- **isDirect**: A boolean indicating whether to directly multiply the ciphertext, instead of first encoding the constant to plaintext and then multiplying.



### 16. Addition of ciphertext and complex constant : add_const
void **<font color='red'> add_const</font>** (Ciphertext **&ciph**, double **constData**, Ciphertext **&result**) override;

**Description**： This function is used for the homomorphic addition of a ciphertext and a real constant.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **constData**: A value of type double, representing a real constant.
- **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 17. Discrete Fourier Transform on ciphertext : dft
void **<font color='red'> dft</font>** (Ciphertext **&ciph**, LinearMatrixGroup **&matrix_group**, Ciphertext **&result**, const GaloisKeys **&rot_key**) override;

**Description**： This function is used for the Discrete Fourier Transform (DFT) of a ciphertext.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **matrix_group**: A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the DFT.
- **result**: A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.



### 18. Dynamic rescale : rescale_dynamic
void **<font color='red'> rescale_dynamic</font>** (Ciphertext **&ciph**, mpf_class **scale**);

**Description**： This function is used for dynamic rescaling of a ciphertext.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **scale**: An mpf_class object, representing the given scaling factor.


### 19. Polynomial evaluation : evaluatePolyVector
void **<font color='red'> evaluatePolyVector</font>** (Ciphertext **&ciph**, Ciphertext **&destination**, const PolynomialVector **&polys**, mpf_class **scalingfactor**, const RelinKeys **&relin_key**, CKKSEncoder **&encoder**) override;<br>
void **<font color='red'> evaluatePolyVector</font>** (Ciphertext **&ciph**, Ciphertext **&destination**, const *PolynomialVector **&polys**, mpf_class **scalingfactor**, const RelinKeys &**relin_key**, CKKSEncoder **&encoder**, Decryptor **&dec**);

**Description**： This function is used to evaluate a polynomial. The coefficients of Chebyshev or Taylor polynomials are pre-calculated, and then the polynomial calculation is performed.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **destination**: A reference to a **Ciphertext** object, used to store the evaluated ciphertext.
- **polys**: A constant reference to a **PolynomialVector** object, representing the given polynomial vector.
- **scalingfactor**: An **mpf_class** object, representing the given scaling factor.
- **relin_key**: A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder.
- **dec** (optional): A reference to a **Decryptor** object, representing the decryptor.



### 20. Evaluate modulo on ciphertext vector : eval_mod
void **<font color='red'> eval_mod</font>** (Ciphertext **&ciph**, Ciphertext **&result**, const EvalModPoly **&eva_poly**, const RelinKeys **&relin_key**, CKKSEncoder **&encoder**) override;

**Description**： This function is used to perform modular arithmetic on a ciphertext.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **result**: A reference to a **Ciphertext** object, used to store the ciphertext after the modular arithmetic.
- **eva_poly**: A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **relin_key**: A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder.


### 21. Bootstrap : bootstrap
void **<font color='red'> bootstrap</font>** (Ciphertext **&ciph**, Ciphertext **&result**, const EvalModPoly **&eva_poly**, LinearMatrixGroup **& matrix_group0**, LinearMatrixGroup **&matrix_group1**, const RelinKeys **&relin_key**, const GaloisKeys **&rot_key**, const GaloisKeys **&conj_key**, CKKSEncoder **&encoder**) override;

**Description**： This function is used to bootstrap a ciphertext.

**Parameters**: 
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.
- **result**: A reference to a **Ciphertext** object, used to store the bootstrapped ciphertext.
- **eva_poly**: A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **matrix_group0**: A reference to a **LinearMatrixGroup** object, representing the first group of linear matrices used for bootstrapping.
- **matrix_group1**: A reference to a **LinearMatrixGroup** object, representing the second group of linear matrices used for bootstrapping.
- **relin_key**: A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **rot_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key**: A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation operations.
- **encoder**: A reference to a **CKKSEncoder** object, representing the encoder and decoder.



### 22. Key switch : switch_key
void **<font color='red'> switch_key</font>** (Ciphertext **&ciph**, Ciphertext **&result**, const vector<PublicKey> **&switch_key**);

**Description**： This function switches the key of a ciphertext.<br>

**Parameters**：
- **ciph**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key**:  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys.






