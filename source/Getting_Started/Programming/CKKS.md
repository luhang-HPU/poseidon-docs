# CKKS 

## Data Structures




Poseidon Supported parameter data structures, the following is specific to CKKS:

<br>

### 1. CKKS encryption scheme  class : **<font color='red'><span id="CKKSEncoder">CKKSEncoder</span> </font>**

**Description**: CKKSEncoder is a class for encoding and decoding CKKS encryption schemes.

- **paramLiteral** (const ParametersLiteral): Indicates the encryption parameter used to calculate context information.

**Functions**:

```cpp
CKKSEncoder(const PoseidonContext &context);
```

- **context** (const PoseidonContext &): The PoseidonContext.

: Creates a CKKSEncoder instance initialized with the specified PoseidonContext.
<br>

```cpp
void encode(const std::vector<std::complex<double>> &values, parms_id_type parms_id, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(const std::vector<double> &values, parms_id_type parms_id, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(std::complex<double>, parms_id_type parms_id, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(double value, parms_id_type parms_id, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
 void encode(int64_t value, parms_id_type parms_id, double scale, Plaintext &destination) const;
```

- **values** (const vector<complex<double>> & /const vector<double> & /std::complex<double> / double / int64_t): The source message.
- **parms_id** (parms_id_type): parms_id determining the encryption parameters to
  be used by the result plaintext.
- **scale** (double): Scaling parameter defining encoding precision.
- **destination** (Plaintext): The plaintext polynomial to overwrite with the
  result.

: Encodes a vector of double-precision floating-point real or complex numbers
into a plaintext polynomial. Append zeros if vector size is less than N/2.
Dynamic memory allocations in the process are allocated from the memory
pool pointed to by the given MemoryPoolHandle.

<br>

```cpp
void encode(const std::vector<std::complex<double>> &values, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(const std::vector<double> &values, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(std::complex<double> value, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(double value, double scale, Plaintext &destination,MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void encode(int64_t value,  double scale, Plaintext &destination) const;
```

- **values** (const vector<complex<double>> & /const vector<double> & / double / std::complex<double> / int64_t): The source message.
- **scale** (double): Scaling parameter defining encoding precision.
- **destination** (Plaintext): The plaintext polynomial to overwrite with the
  result.

: Encodes a vector of double-precision floating-point real or complex numbersinto a plaintext polynomial. Append zeros if vector size is less than N/2.The encryption parameters used are the top level parameters for the givencontext. Dynamic memory allocations in the process are allocated from the memory pool pointed to by the given MemoryPoolHandle.

<br>


```cpp
void decode(const Plaintext &plain, std::vector<double> &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

```cpp
void decode(const Plaintext &plain, std::vector<std::complex<double>> &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **plain** (const Plaintext &): The source message.
- **destination** (std::vector<double> & /  std::vector<std::complex<double>> &):Vector value type (double or std::complex<double>).


: Decodes a plaintext polynomial into double-precision floating-point real or complex numbers. Dynamic memory allocations in the process are allocated from the memory pool pointed to by the given MemoryPoolHandle.

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

: Creates an LinearMatrixGroup instance initialized.

```cpp
vector<MatrixPlain> &data();
```

: The function is used to get the vector of MatrixPlain.

<br>


```cpp
vector<int> &rot_index();
```

: The function is used to get all rotate index.

<br>


```cpp
void set_step(uint32_t step) 
```

- **step** (uint32_t): The number of modulus used during encoding matrix message to plaintext.

: The function is used to set the scaling factor of Matrix Plain. When step is 1, the  scaling factor is one modulus.

<br>

```cpp
const int step() const;
```

: The function get the step.

<br>


### 4. Homomorphic DFT Parameter class  : **<font color='red'><span id="HomomorphicDFTMatrixLiteral">HomomorphicDFTMatrixLiteral</span> </font>**

**Description**: Parameter config class for homomorphic DFT.

**Functions**:

```cpp
HomomorphicDFTMatrixLiteral(DFTType type, uint32_t logN, uint32_t logSlots,uint32_t levelStart, vector<uint32_t> levels,
                            bool repackImag2Real,double scaling,bool bitReversed,uint32_t logBSGSRatio);
```

- **type** (DFTType): DFT or IDFT.
- **logN** (uint32_t): The logarithm of polynomial degree.
- **logSlots** (uint32_t): The logarithm of plaintext slots.
- **levelStart** (uint32_t): The start level in modulus chain.
- **levels** (vector<uint32_t>): The levels of DFT process not the level in modulus chain. 
- **repackImag2Real** (uint32_t): Allow repackage image message to real message ciphertext.
- **scaling** (uint32_t): The scaling factor of HomomorphicDFT process.
- **bitReversed** (uint32_t): Allow bit reversed(only support false).
- **logBSGSRatio** (uint32_t): The ratio of BSGS matrix, 1 is recommend.

: Constructs  homomorphic DFT parameters.

<br>

```cpp
 void create(LinearMatrixGroup &mat_group, CKKSEncoder &encoder,uint32_t step);
```

- **mat_group** (LinearMatrixGroup &): The LinearMatrixGroup.
- **encoder** (CKKSEncoder &): The CKKSEncoder.
- **step** (uint32_t): The number of modulus used during encoding matrix message to plaintext.

: Create the LinearMatrixGroup according to the HomomorphicDFTMatrixLiteral.

<br>

### 5.  Polynomial  class : **<font color='red'><span id="Polynomial">Polynomial</span> </font>**

**Description**:The polynomial coefficients class in homomorphic poly evaluator.

**Functions**: 

```cpp
Polynomial();
```

: Empty polynomial, and the type is Monomial.

<br>

```cpp
Polynomial(const Polynomial &copy) = default;
```

- **copy** (const Polynomial &): The  Polynomial.

: Copy constructor.

<br>

```cpp
Polynomial(Polynomial &&source) = default;
```

- **source** (Polynomial &&): The  Polynomial.

: Move constructor.

<br>

```cpp
vector<complex<double>> &data();
```

: Get the Polynomial coefficient data.

<br>

### 6.  PolynomialVector  class : **<font color='red'><span id="PolynomialVector">PolynomialVector</span> </font>**

**Description**:A vector of polynomials  class in homomorphic poly evaluator.

**Functions**: 

```cpp
PolynomialVector() = default;
```

: Construct an empty polynomial vector.

<br>

```cpp
PolynomialVector(const vector<Polynomial> &polys, const vector<vector<int>> &indexs,bool lead = true);
```

- **polys** (const vector<Polynomial> &): The  Polynomials.
- **indexs** (const vector<vector<int>> &): The  indexs of each Polynomial in slots.
- **lead**(bool): Whether is  lead or not. 

: Construct a polynomial vector.

<br>

```cpp
PolynomialVector(const PolynomialVector &copy) = default;
```

- **copy** (const PolynomialVector &copy): The  Polynomial.

: Copys a given PolynomialVector to the current one


<br>

```cpp
PolynomialVector(PolynomialVector &&source) = default;
```

- **source** (Polynomial &&): The  Polynomial.

: Moves a given PolynomialVector to the current one

<br>

```cpp
PolynomialVector &operator = (const PolynomialVector &assign);
```

- **copy** (const PolynomialVector &copy): The  Polynomial.

: Copy constructor.
<br>


```cpp
PolynomialVector &operator = (PolynomialVector &&assign) = default;
```

- **source** (Polynomial &&): The  Polynomial.

: Move constructor.

<br>


```cpp
vector<Polynomial>  &polys();
```

: Gets the Polynomials of PolynomialVector.

```cpp
vector<vector<int>> &index()
```

: Gets the PolynomialVector index.

<br>


### 7. Homomorphic Mod polynomial class : **<font color='red'><span id="EvalModPoly">EvalModPoly</span> </font>**

**Description**: Parameter config class for homomorphic mod.

**Functions**: 

```cpp
EvalModPoly(const PoseidonContext &context,SineType type,double scaling_factor,uint32_t levelStart,uint32_t logMessageRatio,uint32_t doubleAngle,uint32_t K,uint32_t arcSineDegree,uint32_t SineDegree);

```

- **context** (const PoseidonContext &): The PoseidonContext.
- **type** (SineType): CosDiscrete,SinContinuous or CosContinuous.
- **scaling_factor** (double): The scaling factor of HomomorphicMod process.
- **levelStart** (uint32_t): The start level in modulus chain.
- **logMessageRatio** (uint32_t): The logarithm of message ratio.
- **doubleAngle** (uint32_t): The num of double angle.
- **K** (uint32_t): The rang if Chebyshev.
- **arcSineDegree** (uint32_t): arcSine poly degree.
- **SineDegree** (uint32_t): SineDegree poly degree.

: Constructs  homomorphic Mod parameters.

<br>

## Evaluation Functions

### 1. Addition between ciphertexts : **<font color='red'> add</font>**

```c
void add(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**: This function performs homomorphic addition on two ciphertexts.<br>

**Parameters**:

- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>


### 2. Addition of ciphertext and plaintext : **<font color='red'> add_plain</font>**

```c
void add_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**:This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameters**:

- **ciph1** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>


### 3. Subtraction between ciphertexts : **<font color='red'> sub</font>**

```c
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result) override;
```

**Description**: This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameters**:

- **ciph1** (Ciphertext):  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.


<br>

### 4. Multiplication between ciphertexts : **<font color='red'> multiply</font>** (**<font color='blue'> only software</font>**)

```c
void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
```

**Description**：This function performs homomorphic multiplication between two ciphertexts.<br>

**Parameters**：

- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>

  <br>

  ### 5.  Relinearization : **<font color='red'> relinearize</font>** (**<font color='blue'> only software</font>**)

```c
void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const override;
```

**Description**: This function  performs relinearization operation on the ciphertext.

**Parameters:**

* **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.
* **relin_keys** (RelinKeys): A reference to a **RelinKeys** object,representing the relinearization key.
* **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.

   <br>



### 6. Multiplication between ciphertexts : **<font color='red'> multiply_relin</font>**

```c
void multiply_relin(Ciphertext &ciph0, Ciphertext &ciph1, Ciphertext &result, const RelinKeys &relin_key) override;
```

**Description**:This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameters**:

- **ciph1** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **ciph2** (Ciphertext): A reference to a **Ciphertext** object, representing another ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.


<br>


### 7. Multiplication of ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```c
void multiply_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**: This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**:

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>


<br>


### 8. Rescale : **<font color='red'> rescale</font>**

```c
void rescale (const Ciphertext &ciph,Ciphertext &result) const;
```

**Description**: This function performs a rescaling operation on a ciphertext.<br>

**Parameters**:

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>


<br>


### 9. Ciphertext rotation : **<font color='red'> rotate</font>**

```c
void rotate(const Ciphertext &ciph,int step, const GaloisKeys &galoisKeys, Ciphertext &result) const;
```

**Description**: This function is used to rotate a ciphertext.<br>

**Parameters**:

- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **step** (int):  An integer, indicating the rotation step length. A positive number represents a rotation to the right, and a negative number represents a rotation to the left.
- **galoisKeys** (const GaloisKeys &):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store  the rotated ciphertext.<br>


<br>


### 10. Take conjugate : **<font color='red'> conjugate</font>**

```c
void conjugate(const Ciphertext &ciph, const GaloisKeys &conj_keys, Ciphertext &result) const;
```

**Description**: This function  is used to conjugate a ciphertext, that is, to transform each of its elements into its complex conjugate.<br>

**Parameters**:

- **ciph** (Ciphertext):    A reference to a **Ciphertext** object, representing the ciphertext to be conjugated.<br>
- **conj_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the resulting ciphertext after the conjugation.<br>

<br>



### 11. Matrix multiplication of ciphertext and plaintext : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```c
void multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) override;
```

**Description**: This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameters**:

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain_mat** (MatrixPlain):  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **rot_key** (GaloisKeys):   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.


<br>


### 12. Coefficient to plaintext slot : **<font color='red'> coeff_to_slot</font>**

```c
void coeff_to_slot(const Ciphertext &ciph,const LinearMatrixGroup& matrix_group,Ciphertext &result_real,Ciphertext &result_imag,const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) const;
```

**Description**:  This function is used to transform a ciphertext from the coefficient domain to the slot domain.

**Parameters**:  

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result_real** (Ciphertext): A reference to a **Ciphertext** object, used to store the real part of the transformed ciphertext.
- **result_imag** (Ciphertext): A reference to a **Ciphertext** object, used to store the imaginary part of the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.

<br>



### 13. Plaintext slot to coefficient : **<font color='red'> slot_to_coeff</font>**

```c
void slot_to_coeff(Ciphertext &ciph_real, Ciphertext &ciph_imag, LinearMatrixGroup &matrix_group, Ciphertext &result, const GaloisKeys &rot_key, const GaloisKeys &conj_key, CKKSEncoder &encoder) override;
```

**Description**: This function is used to transform a ciphertext from the slot domain to the coefficient domain.

**Parameters**: 

- **ciph_real** (Ciphertext): A reference to a **Ciphertext** object, representing the real part of the ciphertext to be transformed.
- **ciph_imag** (Ciphertext): A reference to a **Ciphertext** object, representing the imaginary part of the ciphertext to be transformed.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the transformation.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **conj_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder used in the transformation process.


<br>



### 14. Fast Fourier Transform (forward) : **<font color='red'> ftt_fwd</font>** 

```c
void ftt_fwd(Plaintext &plain ,Plaintext &result);
void ftt_fwd(Plaintext &plain ,Plaintext &result,parms_id_type id);
void ftt_fwd(Ciphertext &ciph, Ciphertext &result);
```

**Description**: This function is used for the Fast Fourier Transform (FFT) of a plaintext or ciphertext.

**Parameters**: 

- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **id** (parms_id_type): Target parms_id of plain.(BGV or BFV)
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the transformed plaintext or ciphertext.


<br>


### 15. Fast Fourier Transform (inverse) : **<font color='red'> ftt_inv</font>**

```c
void ftt_inv(Ciphertext &ciph ,Ciphertext &result);
```

**Description**: This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 

- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.


<br>



### 16. Read ciphertext information from accelerator card : **<font color='red'> read</font>**

```c
void read(Ciphertext &ciph) override;
```

**Description**: This function is used to read data from the acceleration card in the ciphertext.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing the ciphertext to be read.



<br>

### 17. Multiplication of ciphertext and complex constant : **<font color='red'> multiply_const</font>**

```c
void multiply_const(const Ciphertext &ciph, complex<double> constData, double scale,Ciphertext &result, CKKSEncoder &encoder) override;
```

**Description**: This function is used for the homomorphic multiplication of a ciphertext and a complex constant.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **constData** (complex < double> ): A complex<double> object, representing a complex constant.
- **scale** (double): The scaling factor.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.



<br>


### 18. Addition of ciphertext and complex constant : **<font color='red'> add_const</font>**

```cpp
void add_const(const Ciphertext &ciph, double constData, Ciphertext &result,const CKKSEncoder &encoder) const;
```

**Description**: This function is used for the homomorphic addition of a ciphertext and a real constant.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **constData** (double): A value of type double, representing a real constant.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.



<br>

### 19. Discrete Fourier Transform on ciphertext : **<font color='red'> dft</font>**

```c
void dft( Ciphertext &ciph, MatrixPlain& plain_mat,Ciphertext &result,const GaloisKeys &rot_key);
```

**Description**: This function is used for the Discrete Fourier Transform (DFT) of a ciphertext.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **matrix_group** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the group of linear matrices used for the DFT.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the transformed ciphertext.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.


<br>


### 20. Dynamic rescale : **<font color='red'> rescale_dynamic</font>**

```c
void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result, double min_scale);
```

**Description**: This function is used for dynamic rescaling of a ciphertext.

**Parameters**: 

- **ciph** (Ciphertext): A const reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **scale** (double):  Representing the given scaling factor.

<br>


### 21. Polynomial evaluation : **<font color='red'> evaluatePolyVector</font>**

```c
void evaluatePolyVector(const Ciphertext &ciph,Ciphertext &destination,const PolynomialVector &polys,double scale,const RelinKeys &relin_key,const CKKSEncoder &encoder) const;
```

**Description**: This function is used to evaluate a polynomial. The coefficients of Chebyshev or Taylor polynomials are pre-calculated, and then the polynomial calculation is performed.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **destination** (Ciphertext): A reference to a **Ciphertext** object, used to store the evaluated ciphertext.
- **polys** (PolynomialVector): A constant reference to a **PolynomialVector** object, representing the given polynomial vector.
- **scale** (double): Representing the given scaling factor.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.


<br>


### 22. Evaluate modulo on ciphertext vector : **<font color='red'> eval_mod</font>**

```c
void eval_mod(Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly, const RelinKeys &relin_key, CKKSEncoder &encoder) override;
```

**Description**: This function is used to perform modular arithmetic on a ciphertext.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the ciphertext after the modular arithmetic.
- **eva_poly** (EvalModPoly): A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.

<br>


### 23. Bootstrap : **<font color='red'> bootstrap</font>** 

```c
void bootstrap(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
const LinearMatrixGroup &matrix_group0, const LinearMatrixGroup &matrix_group1,
const RelinKeys &relin_key, const GaloisKeys &rot_key,const CKKSEncoder &encoder) ;
```

**Description**: This function is used to bootstrap a ciphertext.

**Parameters**: 

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the bootstrapped ciphertext.
- **eva_poly** (EvalModPoly): A constant reference to an **EvalModPoly** object, representing the given modular polynomial.
- **matrix_group0** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the first group of linear matrices used for bootstrapping.
- **matrix_group1** (LinearMatrixGroup): A reference to a **LinearMatrixGroup** object, representing the second group of linear matrices used for bootstrapping.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the encryption key used for relinearization.
- **rot_key** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for rotation operations.
- **encoder** (CKKSEncoder): A reference to a **CKKSEncoder** object, representing the encoder and decoder.


<br>

<!-- 

### 24. Key switch : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**: This function switches the key of a ciphertext.<br>

**Parameters**:

- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys. -->





