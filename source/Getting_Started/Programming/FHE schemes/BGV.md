# BGV

## Data Structures


Poseidon Supported parameter data structures, the following is specific to BGV：

<br>


### 1. BGV encoding and decoding class : **<font color='red'>BatchEncoder</font>**

**Description**：`BatchEncoder` is used to encode and decode message in the BGV encryption scheme.

**Functions**：

```cpp
BatchEncoder(const PoseidonContext &context);
```

- **context** (const PoseidonContext &): The poseidon context.

**Usage**: Construction function creates a BatchEncoder. It is necessary that the encryption parameters given through the `PoseidonContext` object support batching.

<br>


```cpp
void encode(const vector<uint32_t> &src, Plaintext &plain);
```

- **src** (const vector<uint32_t> &): The source message.
- **plain** (Plaintext &): The source message.

**Usage**: A function used to encode a vector of integer into a plaintext.

<br>


```cpp
void decode(const Plaintext &plain,vector<uint32_t> &res);
```

- **plain** (const Plaintext &plain): The plaintext.
- **res** (vector<uint32_t> &): The result message.

**Usage**: A function used to decode a plaintext into a vector of integer.

<br>



### 2. Plaintext matrix class : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**

**Description**: MatrixPlain is a class for storing plaintext matrix.

**Members**:

- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (uint32_t): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (double): Indicates the scaling factor of the matrix(only used in CKKS).

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.

<br>



### 3. Evaluator class : **<font color='red'><span id="EvaluatorBfvBase">EvaluatorBgvBase</span> </font>**

**Description**: `EvaluatorBgvBase` is a evaluator for homomorphic computation for BGV scheme. 

**Members**: The member functions are listed in Chapter [Evaluation Functions](#Evaluation Functions).

<br>



## <a id = "Evaluation Functions">Evaluation Functions</a>

### 1. Addition between ciphertexts : **<font color='red'> add</font>**

```cpp
void add(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
```

**Description**：This function performs homomorphic addition on two ciphertexts.<br>

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing another ciphertext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `add` computes `result` = `ciph1` + `ciph2` . 



### 2. Addition of ciphertext and plaintext : **<font color='red'> add_plain</font>**

```cpp
void add_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain** (Plaintext): representing a plaintext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `add_plain` computes `result` = `ciph` + `plain` .



### 3. Ciphertext and ciphertext subtraction : **<font color='red'> sub</font>**

```cpp
void sub(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
```

- **ciph1** (Ciphertext): representing the minuend (the number from which another is to be subtracted).
- **ciph2** (Ciphertext): representing the subtrahend (the number to be subtracted).
- **result** (Ciphertext): storing the computation result.

**Usage**: `sub` computes `result` = `ciph1` - `ciph2`.



### 4. Subtraction plaintext from ciphertext : **<font color='red'> sub_plain</font>**

```cpp
void sub_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const;
```

* **ciph** (Ciphertext): representing the minuend (the number from which another is to be subtracted).
* **plain** (Plaintext): representing the subtrahend (the number to be subtracted).
* **result** (Ciphertext): storing the computation result.
* **Usage**: `sub_plain` computes `result` = `ciph` - `plain` .

<br>



### 5. Multiplication between ciphertexts : **<font color='red'> multiply</font>**  (**<font color='blue'> only software</font>**)

```cpp
void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
```

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing another ciphertext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `multiply` computes `result` = `ciph1` * `ciph2`.

<br>



### 6. Multiplication between ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```cpp
void multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain** (Plaintext): representing a plaintext.
- **result** (Ciphertext): storing the multiplication result.

**Usage**: `multiply_plain` computes `result` = `ciph` * `plain`.

<br>



### 7. Multiplication with relinearization : **<font color='red'> multiply_relin</font>**

```cpp
void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result, const RelinKeys &relin_key) const;
```

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing another ciphertext.
- **result** (Ciphertext): storing the computation result.
- **relin_key** (RelinKeys): representing the relinearization key.

**Usage**: `multiply_relin` computes `result` = `ciph1` * `ciph2` and relinearize the ciphertext size of `result`.

<br>

### 8.  Relinearization : **<font color='red'> relinearize</font>**  (**<font color='blue'> only software</font>**)

```cpp
void relinearize(const Ciphertext &ciph, Ciphertext &result, const RelinKeys &relin_keys) const;
```

* **ciph** (Ciphertext): representing a ciphertext.
* **result** (Ciphertext): storing the computation result.
* **relin_keys** (RelinKeys): representing the relinearization key.

**Usage**: `relinearize` function performs relinearization operation on the ciphertext.

<br>

### 9. Ciphertext row rotation : **<font color='red'> rotate_col</font>**

```c
void rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const override;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after column rotation.
- **galois_keys** (GaloisKeys): representing the encryption keys used for row rotation.

**Usage**: `rotate_col`  performs a column rotation operation on a ciphertext.

<br>

### 10. Ciphertext column rotation : **<font color='red'> rotate_row</font>**

```c
void rotate_row(const Ciphertext &ciph, Ciphertext &result, int step, const GaloisKeys &galois_keys) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after row rotation.
- **step** (int): An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.
- **galois_keys** (GaloisKeys): representing the galois keys used for row rotation.

**Usage**: `rotate_row` performs a row rotation operation on a ciphertext.

<br>

### 11. Modulo drop: **<font color='red'> drop_modulus</font>**

```c
void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after dropping the modulus.

* **level** (uint32_t):  The modulus level to switch into.

**Usage**: `drop_modulus` drops the modulus into a specific level. `drop_modulus_to_next` drops the modulus into the next level.

<br>

### 12. Number Theoretic Transform (forward) : **<font color='red'> ntt_fwd</font>** 

```c
void ntt_fwd(const Plaintext &plain, Plaintext &result, parms_id_type parms_id = parms_id_zero) const;
void ntt_fwd(const Ciphertext &ciph, Ciphertext &result) const;
```

- **plain** (Plaintext): representing a plaintext.
- **ciph** (Ciphertext): representing a ciphertext.
- **id** (Ciphertext): target parms_id of plain(only used in BGV and BFV).
- **result** (Ciphertext / Plaintext): store the transformed plaintext or ciphertext.

**Usage**: `ntt_fwd` performs the Number Theoretic Transform(NTT) on a plaintext or a ciphertext.

<br>

### 13. Number Theoretic Transform (inverse) : **<font color='red'> ntt_inv</font>**

```c
void ntt_inv(const Ciphertext &ciph, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext / Plaintext): storing the inverse transformed ciphertext or plaintext.

**Usage**: `ntt_inv` performs the Inverse Number Theoretic Transform(INTT) on a plaintext or a ciphertext.

<br>

### 14. Matrix multiplication of ciphertext and plaintext matrix : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```c
void multiply_by_diag_matrix_bsgs(const Ciphertext &ciph, const MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain_mat** (MatrixPlain): representing a plaintext matrix.
- **result** (Ciphertext): storing the computation result.
- **rot_key** (GaloisKeys): representing the galois key for rotation.

**Usage**: `multiply_by_diag_matrix_bsgs` multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.
