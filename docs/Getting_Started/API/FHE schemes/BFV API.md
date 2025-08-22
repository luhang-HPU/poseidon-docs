# BFV API

## Data Structures


Poseidon supported parameter data structures, the following is specific to BFV：

<br>


### 1. BFV encoding and decoding class : **<font color='red'>BatchEncoder</font>**

**Description**：`BatchEncoder` is used to encode and decode message for BFV encryption scheme.

**Functions**：

```cpp
BatchEncoder(const PoseidonContext &context);
```

- **context** (const PoseidonContext &): The poseidon context.

**Usage**: Creates a BatchEncoder. It is necessary that the encryption parameters given through the PoseidonContext object support batching.


<br>


```cpp
void encode(const vector<uint32_t> &src, Plaintext &plain);
```

- **src** (const vector<uint32_t> &): The source message is an array of integer elements.
- **plain** (Plaintext &): The output plaintext .

**Usage**: Encoding a vector of integer numbers into a plaintext.

<br>


```cpp
void decode(const Plaintext &plain,vector<uint32_t> &res);
```

- **plain** const Plaintext &plain): The plaintext.
- **res** (vector<uint32_t> &): The result of message.

**Usage**: It is used to decode a plaintext into a vector of integer.

<br>


### 2. Plaintext matrix class : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**

- **Description**: MatrixPlain is a class for storing plaintext matrix information.

  ```c++
  MatrixPlain()
          : log_slots(0), n1(0), level(0),
            scale(1.0), rot_index{}, plain_vec_pool{sz, std::map<int, Plaintext>()}, read_idx(0),
            write_idx(0), is_precompute(false)
  ```

  **Members**:

  - **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

  - **N1** (uint32_t): Indicates the number of rows in a matrix.

  - **level** (uint32_t): Indicates the level of the ciphertext module chain in which the matrix resides.

  - **scale** (double): Indicates the scaling factor of the matrix.

  - **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

  - **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.

<br>

### 3. Evaluator class : **<font color='red'><span id="EvaluatorBfvBase">EvaluatorBfvBase</span> </font>**

**Description**: EvaluatorBfvBase is a evaluator for homomorphic computation for BFV scheme. 

**Members**: The member functions are listed in Chapter [Evaluation Functions](#Evaluation Functions).





## <a id = "Evaluation Functions">Evaluation Functions</a>



### 1. Addition of ciphertext and ciphertext: <font color='red'> add</font>

```cpp
void add(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result);
```

* **ciph1** (Ciphertext): representing the addend.
* **ciph2** (Ciphertext): representing the other addend.
* **result** (Ciphertext): storing the addition result.

**Usage**: `add` computes *result* = *ciph1* + *ciph2* . 



### 2. Addition of ciphertext and plaintext: **<font color='red'> add_plain</font>**

```cpp
void add_plain(Ciphertext &ciph, Plaintext &plain, Ciphertext &result);
```

* **ciph** (Ciphertext): representing the addend.

* **plain** (Plaintext): representing the other addend.

* **result** (Ciphertext): storing the addition result.

**Usage**: `add_plain` computes *result* = *ciph* + *plain* .



### 3. Subtraction ciphertext from ciphertext: **<font color='red'> sub</font>**

```cpp
void sub(Ciphertext &ciph1, Ciphertext &ciph2, Ciphertext &result);
```

* **ciph1** (Ciphertext): representing the minuend.

* **ciph2** (Ciphertext): representing the subtrahend

* **result** (Ciphertext): storing the computation result.

**Usage**: `sub` computes *result* = *ciph1* - *ciph2*.



### 4. Subtraction plaintext from ciphertext: **<font color='red'> sub_plain</font>**

```cpp
void sub_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result);
```

* **ciph** (Ciphertext): representing the minuend.

* **plain** (Plaintext): representing the subtrahend.

* **result** (Ciphertext): storing the computation result.

**Usage**: `sub_plain` computes *result* = *ciph* - *plain* .



### 5. Multiplication between ciphertexts: **<font color='red'> multiply</font>** (**<font color='blue'> only software</font>**)

```cpp
void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result);
```

* **ciph1** (Ciphertext): representing a ciphertext.
* **ciph2** (Ciphertext): representing the other ciphertext.
* **result** (Ciphertext): storing the computation result.

**Usage**: `multiply` computes *result* = *ciph1* * *ciph2*.




### 6. Multiplication between ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```cpp
void multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain** (Plaintext): representing a plaintext.
- **result** (Ciphertext): storing the computation result.

**Usage**: `multiply_plain` computes *result* = *ciph* * *plain*.




### 7. Multiplication with relinearization : **<font color='red'> multiply_relin</font>**

```cpp
void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result, const RelinKeys &relin_key) const;
```

- **ciph1** (Ciphertext): representing a ciphertext.
- **ciph2** (Ciphertext): representing the other ciphertext.
- **result** (Ciphertext): storing the computation result.
- **relin_key** (RelinKeys): A constant reference to a **RelinKeys** object, representing the relinearization key.

**Usage**: `multiply_relin` computes *result* = *ciph1* * *ciph2* and relinearize the ciphertext size of *result*.



### 8. Square inplace: **<font color='red'> square_inplace</font>** (**<font color='blue'>only software</font>**)

```cpp
void BFVEvaluator_S::square_inplace(Ciphertext &ciph) const;
```

- **ciph** (Ciphertext): representing a ciphertext.

**Usage**: `square_inplace` computes *ciph* = *ciph* ^ *2* inplace.




### 9.  Relinearization : **<font color='red'> relinearize</font>** (**<font color='blue'>only software</font>**)

```cpp
void relinearize(const Ciphertext &ciph, Ciphertext &result, const RelinKeys &relin_keys) const;
```

* **ciph** (Ciphertext): representing a ciphertext.
* **relin_keys** (RelinKeys): representing the relinearization key.
* **result** (Ciphertext): storing the computation result.

**Usage**: `relinearize` function performs relinearization operation on the ciphertext.




### 10. Ciphertext row rotation : **<font color='red'> rotate_col</font>**

```cpp
void rotate_col(const Ciphertext &ciph, Ciphertext &result, const GaloisKeys &galois_keys) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **galois_keys** (GaloisKeys): representing the galois keys used for rotation.
- **result** (Ciphertext): storing the ciphertext after column rotation.

**Usage**: `rotate_col`  performs a column rotation operation on a ciphertext.




### 11. Ciphertext column rotation : **<font color='red'> rotate_row</font>**

```c
void rotate_row(const Ciphertext &ciph, Ciphertext &result, int step, const GaloisKeys &galois_keys) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after row rotation.
- **step** (int): An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.
- **galois_keys** (GaloisKeys): representing the galois keys used for row rotation.

**Usage**: `rotate_row` performs a row rotation operation on a ciphertext.



### 12. Modulo drop: **<font color='red'> drop_modulus</font>** 

```c
void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext): storing the ciphertext after dropping the modulus.

* **level** (uint32_t): The modulus level to switch into.

**Usage**: `drop_modulus` drops the modulus into a specific level. `drop_modulus_to_next` drops the modulus into the next level.



### 13. Number Theoretic Transform (forward) : **<font color='red'> ntt_fwd</font>** 

```cpp
void ntt_fwd(const Plaintext &plain, Plaintext &result, parms_id_type parms_id = parms_id_zero) const;
void ntt_fwd(const Ciphertext &ciph, Ciphertext &result) const;
```

- **plain** (Plaintext): representing a plaintext.
- **ciph** (Ciphertext): representing a ciphertext.
- **id** (Ciphertext): target parms_id of plain(only used in BGV or BFV).
- **result** (Ciphertext/Plaintext): storing the transformed plaintext or ciphertext.

**Usage**: `ntt_fwd` performs the Number Theoretic Transform(NTT) on a plaintext or a ciphertext.



### 14. Number Theoretic Transform (inverse) : **<font color='red'> ntt_inv</font>**

```c
void ntt_inv(const Ciphertext &ciph, Ciphertext &result) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **result** (Ciphertext or Plaintext): storing the inverse transformed ciphertext or plaintext.

**Usage**: `ntt_inv` performs the Inverse Number Theoretic Transform(INTT) on a plaintext or a ciphertext.



### 15. Matrix multiplication of ciphertext and plaintext matrix : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

```cpp
void multiply_by_diag_matrix_bsgs(const Ciphertext &ciph, const MatrixPlain &plain_mat, Ciphertext &result, const GaloisKeys &rot_key) const;
```

- **ciph** (Ciphertext): representing a ciphertext.
- **plain_mat** (MatrixPlain): representing a plaintext matrix.
- **result** (Ciphertext): storing the computation result.
- **rot_key** (GaloisKeys): representing the galois key for rotation.

**Usage**: `multiply_by_diag_matrix_bsgs` multiplies a ciphertext with a plaintext matrix, performing the linear transformation over the ciphertext with the Baby-Step-Giant-Step (BSGS) algorithm to accelerate the operations. 

The BSGS algorithm can be performed with the following equation.

$$ \begin{aligned}
A \cdot z
& = \sum\limits_{0 \le j \le N_2} \sum\limits_{0 \le i \le N_1}
(u_{N_i \cdot j + i} \odot \rho(z;N_1 \cdot j + i))
\\
& = \sum\limits_{0 \le j < N_2} \rho (
\sum\limits_{0 \le i < N_1} \rho(u_{N_1 \cdot j + i};-N_1 \cdot j)
\odot \rho(z;i);N_1 \cdot j
)
\end{aligned} $$

notation:

$A$ denotes the plain matrix

$z$ denotes the ciphertext

$N_1$ is a divisor of $N/2$ and  $N_2$ is $N/2N_1$

$\odot$ denotes the Hadamard (component-wise) multiplication between vectors.

$\rho(c;s)$ denotes the rotation of ciphertext $c$, the rotation step is $s$ .
