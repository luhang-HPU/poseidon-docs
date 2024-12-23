# CKKS API

## Data Structures


Poseidon Supported parameter data structures, the following is specific to CKKS:

<br>

### 1. CKKS encoding and decoding class : **<font color='red'><span id="CKKSEncoder">CKKSEncoder</span> </font>**

**Description**: `CKKSEncoder` is used to encode and decode message for CKKS scheme.

**Functions**:

```cpp
CKKSEncoder(const PoseidonContext &context);
```

- **context** (const PoseidonContext &): The context specified by user, which includes the parameters of homomorphic encryption scheme, polynomial degree and security level.

**Usage**: Creates a CKKSEncoder instance initialized with the specified PoseidonContext.
<br>

```cpp
void encode(const std::vector<std::complex<double>> &values, parms_id_type parms_id, double scale, Plaintext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

void encode(const std::vector<double> &values, parms_id_type parms_id, double scale, Plaintext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

void encode(std::complex<double>, parms_id_type parms_id, double scale, Plaintext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

void encode(double value, parms_id_type parms_id, double scale, Plaintext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

void encode(int64_t value, parms_id_type parms_id, double scale, Plaintext &destination) const;
```

- **values** (const vector<complex<double\>>&, const vector\<double\>&, std::complex\<double\>, double, int64_t): The source number message.
- **parms_id** (parms_id_type): parms_id specifies the modulus level of the modulus chain to of the result plaintext.
- **scale** (double): Scaling parameter defining encoding precision.
- **destination** (Plaintext): The plaintext polynomial to overwrite with the result.

**Usage**: `encode` function encodes a vector of  numbers into a plaintext polynomial. It appends zeros to the end if the vector size is less than N/2 (N is the polynomial degree).

<br>


```cpp
void decode(const Plaintext &plain, std::vector<double> &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

void decode(const Plaintext &plain, std::vector<std::complex<double>> &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **plain** (const Plaintext &): The plaintext to be decoded.
- **destination** (std::vector\<double\>&, std::vector\<std::complex\<double\>\>&): A number vector to store the message.

**Usage**: `decode` function decodes a plaintext polynomial into a vector of numbers.

<br>

### 2. Plaintext matrix class : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**

**Description**: MatrixPlain is a class for storing plaintext matrix information.

**Members**:

- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (uint32_t): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (double): Indicates the scaling factor of the matrix.

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.

<br>

### 3. Group of Plaintext matrix in linear transform : **<font color='red'><span id="LinearMatrixGroup">LinearMatrixGroup</span> </font>**

**Description**: LinearMatrixGroup is a class for storing a group of plaintext matrix information.

**Functions**:

```cpp
LinearMatrixGroup() = default;
```

**Usage**: Creates an LinearMatrixGroup instance initialized.

```cpp
vector<MatrixPlain> &data();
```

**Usage**: The function is used to get the vector of MatrixPlain.

<br>


```cpp
vector<int> &rot_index();
```

**Usage**: The function is used to get all rotate index.

<br>


```cpp
void set_step(uint32_t step) 
```

- **step** (uint32_t): The number of modulus used during encoding matrix message to plaintext.

**Usage**: The function is used to set the scaling factor of Matrix Plain. When step is 1, the  scaling factor is one modulus.

<br>

```cpp
const int step() const;
```

**Usage**: The function get the step.

<br>


### 4. Homomorphic DFT Parameter class : **<font color='red'><span id="HomomorphicDFTMatrixLiteral">HomomorphicDFTMatrixLiteral</span> </font>**

**Description**: Parameter config class for homomorphic DFT.

**Functions**:

```cpp
HomomorphicDFTMatrixLiteral(LinearType type, uint32_t log_n, uint32_t log_slots, uint32_t level_start, vector<uint32_t> levels, bool repack_imag_to_real = false, double scaling = 1.0, bool bit_reversed = false, uint32_t log_bsgs_ratio = 0);
```

- **type** (LinearType): indicating the matrix type (DFT or IDFT).
- **log_n** (uint32_t): The logarithm of polynomial degree.
- **log_slots** (uint32_t): The logarithm of plaintext slots.
- **level_start** (uint32_t): The start level in modulus chain.
- **levels** (vector<uint32_t>): The levels of DFT process. 
- **repack_imag_to_real** (uint32_t): Allowing repackage image message to real message ciphertext.
- **scaling** (uint32_t): The scaling factor of HomomorphicDFT process.
- **bit_reversed** (uint32_t): Allow bit reversed (only support false).
- **log_bsgs_ratio** (uint32_t): The ratio of BSGS matrix (1 is recommend).

**Usage**: `HomomorphicDFTMatrixLiteral` class constructs the DFT or IDFT matrix.

<br>

```cpp
 void create(LinearMatrixGroup &mat_group, CKKSEncoder &encoder,uint32_t step);
```

- **mat_group** (LinearMatrixGroup &): The LinearMatrixGroup.
- **encoder** (CKKSEncoder &): The CKKSEncoder.
- **step** (uint32_t): The number of modulus used during encoding matrix message to plaintext.

**Usage**: Create the `LinearMatrixGroup` object according to the `HomomorphicDFTMatrixLiteral` parameter.

<br>

### 5.  Polynomial class : **<font color='red'><span id="Polynomial">Polynomial</span> </font>**

**Description**: The polynomial coefficients class in homomorphic polynomial evaluator.

<br>

### 6.  PolynomialVector class : **<font color='red'><span id="PolynomialVector">PolynomialVector</span> </font>**

**Description**: A vector of polynomials class in homomorphic polynomial evaluator.

**Functions**: 

```cpp
PolynomialVector() = default;
```

**Usage**: Construct an empty polynomial vector.

<br>

```cpp
PolynomialVector(const vector<Polynomial> &polys, const vector<vector<int>> &indexs, bool lead = true);
```

- **polys** (const vector<Polynomial> &): The  Polynomials.
- **indexs** (const vector<vector<int>> &): The  indexs of each Polynomial in slots.
- **lead**(bool): Whether is lead or not. 

**Usage**: Construct a polynomial vector.

<br>

```cpp
PolynomialVector(const PolynomialVector &copy) = default;
```

- **copy** (const PolynomialVector &copy): The  Polynomial.

**Usage**: Copy a given PolynomialVector to the current one.


<br>

```cpp
PolynomialVector(PolynomialVector &&source) = default;
```

- **source** (Polynomial &&): The  Polynomial.

**Usage**: Moves a given PolynomialVector to the current one

<br>

```cpp
PolynomialVector &operator = (const PolynomialVector &assign);
```

- **copy** (const PolynomialVector &copy): The  Polynomial.

**Usage**: Copy constructor.
<br>


```cpp
PolynomialVector &operator = (PolynomialVector &&assign) = default;
```

- **source** (Polynomial &&): The  Polynomial.

**Usage**: Move constructor.

<br>


```cpp
vector<Polynomial>  &polys();
```

**Usage**: Gets the Polynomials of PolynomialVector.

```cpp
vector<vector<int>> &index()
```

**Usage**: Gets the PolynomialVector index.

<br>


### 7. Homomorphic Mod polynomial class : **<font color='red'><span id="EvalModPoly">EvalModPoly</span> </font>**

**Description**: Parameter config class for homomorphic mod.

**Functions**: 

```cpp
EvalModPoly(const PoseidonContext &context, SineType type, double scaling_factor, uint32_t level_start, uint32_t 							log_message_ratio, uint32_t double_angle, uint32_t k, uint32_t arcsine_degree, uint32_t sine_degree);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **type** (SineType): Cosine Discrete, Sine Continuous or Cosine Continuous.
- **scaling_factor** (double): The scaling factor of HomomorphicMod process.
- **level_start** (uint32_t): The start level in modulus chain.
- **log_message_ratio** (uint32_t): The logarithm of message ratio.
- **double_angle** (uint32_t): The num of double angle.
- **k** (uint32_t): The rang if Chebyshev.
- **arcsine_degree** (uint32_t): arcSine poly degree.
- **sine_degree** (uint32_t): SineDegree poly degree.

**Usage**: Constructs homomorphic mod parameters.

<br>



## Evaluation Functions

### 1. Addition between ciphertexts : **<font color='red'> add</font>**

```cpp
void add(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result);
```

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing another ciphertext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `add` computes *result* = *ciph1* + *ciph2* . 

<br>




### 2. Addition of ciphertext and plaintext : **<font color='red'> add_plain</font>**

```cpp
void add_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result);
```

- **ciph** (Ciphertext): representing the addend.
- **plain** (Plaintext): representing the other addend.

- **result** (Ciphertext): storing the addition result.

**Usage**: `add_plain` computes *result* = *ciph* + *plain* .


<br>


### 3. Subtraction between ciphertexts : **<font color='red'> sub</font>**

```cpp
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result);
```

- **ciph1** (Ciphertext): representing the minuend.
- **ciph2** (Ciphertext): representing the subtrahend

- **result** (Ciphertext): storing the computation result.

**Usage**: `sub` computes *result* = *ciph1* - *ciph2*.

<br>

### 4. Multiplication between ciphertexts : **<font color='red'> multiply</font>** (**<font color='blue'> only software</font>**)

```cpp
void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
```

- **ciph1** (Ciphertext): representing a ciphertext.

- **ciph2** (Ciphertext): representing the other ciphertext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `multiply` computes *result* = *ciph1* * *ciph2*.

<br>

### 5.  Relinearization : **<font color='red'> relinearize</font>** (**<font color='blue'> only software</font>**)

```cpp
void relinearize(const Ciphertext &ciph, Ciphertext &result, const RelinKeys &relin_keys) const;
```

* **ciph** (Ciphertext): representing a ciphertext.

* **relin_keys** (RelinKeys): representing the relinearization key.
* **result** (Ciphertext): storing the computation result.

**Usage**: `relinearize` function performs relinearization operation on the ciphertext.

<br>



### 6. Multiplication between ciphertexts : **<font color='red'> multiply_relin</font>**

```cpp
void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result, const RelinKeys &relin_key) const;
```

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing the other ciphertext.
- **result** (Ciphertext): storing the computation result.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.

**Usage**: `multiply_relin` computes *result* = *ciph1* * *ciph2* and relinearize the ciphertext size of *result*.

<br>




### 7. Multiplication of ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```cpp
void multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain** (Plaintext): representing a plaintext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `multiply_plain` computes *result* = *ciph* * *plain*.

<br>




### 8. Rescale : **<font color='red'> rescale</font>**

```cpp
void rescale (const Ciphertext &ciph,Ciphertext &result) const;
```

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.

**Usage**: `rescale` performs a rescaling operation on a ciphertext.


<br>


### 9. Ciphertext rotation : **<font color='red'> rotate</font>**

```cpp
void rotate(const Ciphertext &ciph, Ciphertext &result, int step, const GaloisKeys &galois_keys) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after row rotation.
- **rot_step** (int): An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.
- **galois_keys** (GaloisKeys): representing the galois keys used for row rotation.

**Usage**: `rotate` rotates a ciphertext.


<br>


### 10. Take conjugate : **<font color='red'> conjugate</font>**

```cpp
void conjugate(const Ciphertext &ciph, const GaloisKeys &conj_keys, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing the ciphertext to be conjugated.
- **conj_keys** (GaloisKeys):  representing the galois key used for conjugation.
- **result** (Ciphertext): storing the resulting ciphertext after conjugation.

**Usage**: `conjugate` transforms the complex elements of ciphertext into its complex conjugate.

<br>



### 11. Matrix multiplication of ciphertext and plaintext : **<font color='red'> multiply_by_diag_matrix_bsgs</font>**

```cpp
void multiply_by_diag_matrix_bsgs(const Ciphertext &ciph, const MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain_mat** (MatrixPlain): representing a plaintext matrix.
- **result** (Ciphertext): storing the computation result.
- **rot_key** (GaloisKeys): representing the galois key used for rotations.


**Usage**: `multiply_by_diag_matrix_bsgs` multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.


<br>


### 12. Coefficient to plaintext slot : **<font color='red'> coeff_to_slot</font>**

```cpp
void coeff_to_slot(const Ciphertext &ciph, const LinearMatrixGroup &matrix_group, Ciphertext &result_real, Ciphertext &result_imag, const GaloisKeys &galoisKeys, const CKKSEncoder &encoder) const;
```

- **ciph** (Ciphertext): representing the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result_real** (Ciphertext): storing the real part of the transformed ciphertext.
- **result_imag** (Ciphertext): storing the imaginary part of the transformed ciphertext.
- **rot_key** (GaloisKeys): representing the galois key for rotation.
- **conj_key** (GaloisKeys): representing the galois key for conjugation.
- **encoder** (CKKSEncoder): representing the encoder used in the transformation process.

**Usage**: `coeff_to_slot` transforms a ciphertext from the coefficient domain to the slot domain.

<br>



### 13. Plaintext slot to coefficient : **<font color='red'> slot_to_coeff</font>**

```cpp
void slot_to_coeff(const Ciphertext &ciph_real, const Ciphertext &ciph_imag, const LinearMatrixGroup &matrix_group, Ciphertext &result, const GaloisKeys &galoisKeys, const CKKSEncoder &encoder) const;
```

- **ciph_real** (Ciphertext): representing the real part of the ciphertext to be transformed.
- **ciph_imag** (Ciphertext): representing the imaginary part of the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): representing the group of linear matrices used for the transformation.
- **result** (Ciphertext): storing the transformed ciphertext.
- **rot_key** (GaloisKeys): representing the galois key for rotation.
- **conj_key** (GaloisKeys): representing the galois key for conjugation.
- **encoder** (CKKSEncoder): representing the encoder used in the transformation process.

**Usage**: `slot_to_coeff` transforms a ciphertext from the slot domain to the coefficient domain.


<br>



### 14. Number Theoretic Transform (forward) : **<font color='red'> ntt_fwd</font>** 

```cpp
void ntt_fwd(const Plaintext &plain, Plaintext &result, parms_id_type parms_id = parms_id_zero) const;
void ntt_fwd(const Ciphertext &ciph, Ciphertext &result) const;
```

- **plain** (Plaintext): representing a plaintext.
- **ciph** (Ciphertext): representing a ciphertext.
- **id** (Ciphertext): target parms_id of plain(only used in BGV and BFV).
- **result** (Ciphertext / Plaintext): storing the transformed plaintext or ciphertext.

**Usage**: `ntt_fwd` performs the Number Theoretic Transform(NTT) on a plaintext or a ciphertext.

<br>




### 15. Number Theoretic Transform (inverse) : **<font color='red'> ntt_inv</font>**

```cpp
void ntt_inv(const Ciphertext &ciph, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext or Plaintext): storing the inverse transformed ciphertext or plaintext.

**Usage**: `ntt_inv` performs the Inverse Number Theoretic Transform(INTT) on a plaintext or a ciphertext.

<br>



### 17. Multiplication of ciphertext and constant value : **<font color='red'> multiply_const</font>**

```cpp
void multiply_const(const Ciphertext &ciph, complex<double> constData, double scale,Ciphertext &result, CKKSEncoder &encoder);
```

- **ciph** (Ciphertext): representing a ciphertext.
- **constData** (complex \<double\> ): representing a complex constant.
- **scale** (double): the scaling factor.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.

**Usage**: `multiply_const` computes *result* = *ciph* * *const*.

<br>




### 18. Addition of ciphertext and complex constant : **<font color='red'> add_const</font>**

```cpp
void add_const(const Ciphertext &ciph, double const_data, Ciphertext &result,const CKKSEncoder &encoder) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **constData** (double): representing a real constant.
- **result** (Ciphertext): storing the computation result.

**Usage**: `add_const` computes *result* = *ciph* + *const_data*.

<br>



### 19. Discrete Fourier Transform on ciphertext : **<font color='red'> dft</font>**

```cpp
void dft( Ciphertext &ciph, MatrixPlain& plain_mat,Ciphertext &result,const GaloisKeys &rot_key);
```

- **ciph** (Ciphertext): representing a ciphertext.
- **matrix_group** (LinearMatrixGroup): representing the group of linear matrices used for the DFT.
- **result** (Ciphertext): storing the transformed ciphertext.
- **rot_key** (GaloisKeys): representing the galois key for rotation.

**Usage**: `dft` function is used for Discrete Fourier Transform (DFT) of a ciphertext.

<br>




### 20. Dynamic rescale : **<font color='red'> rescale_dynamic</font>**

```cpp
void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result, double min_scale);
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext):representing a ciphertext.
- **min_scale** (double): representing the given scaling factor.

**Usage**: `rescale_dynamic` is used for multiple times of rescale of the ciphertext util the scale becomes satisfied with the *min_scale*.

<br>


### 21. Polynomial evaluation : **<font color='red'> evaluate_poly_vector</font>**

```cpp
void evaluate_poly_vector(const Ciphertext &ciph, Ciphertext &destination, const PolynomialVector &polys, double scale, const RelinKeys &relin_key, const CKKSEncoder &encoder) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **destination** (Ciphertext): storing the evaluated ciphertext.
- **polys** (PolynomialVector): representing the given polynomial vector.
- **scale** (double): representing the given scaling factor.
- **relin_key** (RelinKeys): representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): representing the encoder and decoder.

**Usage**: `evaluate_poly_vector` evaluates a polynomial. In detail, the coefficients of Chebyshev or Taylor polynomials are pre-calculated, then the polynomial approximation is calculated.

<br>




### 22. Evaluate modulo on the ciphertext vector : **<font color='red'> eval_mod</font>**

```cpp
void eval_mod(Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly, const RelinKeys &relin_key, CKKSEncoder &encoder);
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after the modular arithmetic.
- **eva_poly** (EvalModPoly): representing the given modular polynomial.
- **relin_key** (RelinKeys): representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): representing the encoder and decoder.

**Usage**: `eval_mod` performs modular arithmetic on a ciphertext.

<br>




### 23. Bootstrap : **<font color='red'> bootstrap</font>** 

```cpp
void bootstrap(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly, const LinearMatrixGroup &matrix_group0, const LinearMatrixGroup &matrix_group1, const RelinKeys &relin_key, const GaloisKeys &rot_key,const CKKSEncoder &encoder);
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the bootstrapped ciphertext.
- **eva_poly** (EvalModPoly): representing the given modular polynomial.
- **matrix_group0** (LinearMatrixGroup): representing the first group of linear matrices used for bootstrapping.
- **matrix_group1** (LinearMatrixGroup): representing the second group of linear matrices used for bootstrapping.
- **relin_key** (RelinKeys): representing the encryption key used for relinearization.
- **rot_key** (GaloisKeys): representing the encryption key used for rotation operations.
- **encoder** (CKKSEncoder): representing the encoder and decoder.

**Usage**: `bootstrap` is used for bootstrapping.





