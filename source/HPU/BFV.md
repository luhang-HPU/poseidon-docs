# BFV 

## Parameter Data Structure





Poseidon Supported parameter data structures：


### 1. Memory address management : MemoryPool
**<font color='red'>MemoryPool</font>** (**DegreeType** degree_type);

**Description**：MemoryPool is a class for managing addresses and memory space.

**Parameter**：degree_type: DegreeType indicates the degree of a polynomial. 

​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; The optional value can be degree_2048,degree_4096, degree_8192, degree_16384, or degree_32768.

**Member function**： getInstance(DegreeType degree_type) : The class used to create Memorypool.

### 2. Parameter class of the BFV encryption scheme : BFVParametersLiteralDefault
**<font color='red'>BFVParametersLiteralDefault</font>** (**DegreeType** degree_type);

**Description**：BFVParametersLiteralDefault is a class used for initializing encryption parameters.

**Parameter**：**degreeType**: Of type DegreeType, representing the degree of the polynomial.

**Member function**： The member functions consist only of the constructor.

### 3. Context information management class : PoseidonContext
### 4. Generate pseudo-random numbers class : Blake2xbPRNGFactory
### 5. Plaintext class : Plaintext      
### 6. Ciphertext class : Ciphertext
### 7. Public key class : PublicKey
### 8. Relinearize key class : RelinKeys
### 9. Galois key class : GaloisKeys
### 10. BFV encryption scheme  class : BatchEncoder
**<font color='red'>BatchEncoder</font>** (const **PoseidonContext**& params);

**Description**：BatchEncoder is a class used for encoding and decoding in the BFV encryption scheme.<br>

**Parameter**：**paramLiteral**: Of type ParametersLiteral, representing the encryption parameters used for computing context information.<br>

**Member function**： int **encode**(vector<complex<double>> vec, Plaintext &plain, const mpf_class scaling_factor):
A function used to encode a complex number vector into a plaintext polynomial.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8195;​&#8195;​&#8195;​&#8195;​​&#8194;​​&#8194;&#8201; int **decode**(const Plaintext &plain, vector<complex<double>>& vec): 
A function used to decode a plaintext polynomial into a complex number vector.

### 11. Plaintext matrix : MatrixPlain
### 12. Key generation class : KeyGenerator
### 13. Encryption class : Encryptor
### 14. Decryption class ：Decryptor
### 15. Envaluator Factory  class ：EnvaluatorFactory

## API
### 1. Ciphertext and ciphertext addition : add
**void** **<font color='red'> add</font>** (**Ciphertext** &ciph1, **Ciphertext** &ciph2, **Ciphertext** &result) override;

**Description**：This function performs homomorphic addition on two ciphertexts.<br>

**Parameter**：**ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 2. Ciphertext and plaintext addition : add_plain
**void** **<font color='red'> add_plain</font>** (**Ciphertext** &ciph, **Plaintext** &plain,**Ciphertext** &result) override;

**Description**：This function performs homomorphic addition between a ciphertext and a plaintext.<br>

**Parameter**：**ciph1**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 3. Ciphertext and ciphertext subtraction : sub
**void** **<font color='red'> sub</font>** (**Ciphertext** &ciph1, **Ciphertext** &ciph2, **Ciphertext** &result) override;

**Description**：This function performs homomorphic subtraction between two ciphertexts.<br>

**Parameter**：**ciph1**:  A reference to a **Ciphertext** object, representing the minuend (the number from which another is to be subtracted).<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing the subtrahend (the number to be subtracted).<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.



### 4. Ciphertext and ciphertext multiplication : multiply
**void** **<font color='red'> multiply</font>** (**Ciphertext** &ciph0, **Ciphertext** &ciph1, **Ciphertext** &result, const **RelinKeys** &relin_key) override;

**Description**：This function performs homomorphic multiplication between two ciphertexts and uses the relinearization key to reduce the ciphertext size.<br>

**Parameter**：**ciph1**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **ciph2**: A reference to a **Ciphertext** object, representing another ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **relin_key**: A constant reference to a **RelinKeys** object, representing the relinearization key.



### 5. Ciphertext and plaintext multiplication : multiply_plain
**void** **<font color='red'> multiply_plain</font>** (**Ciphertext** &ciph, **Plaintext** &plain,**Ciphertext** &result) override;

**Description**：This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameter**：**ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain**: A reference to a **Plaintext** object, representing a plaintext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>




### 6. Rescaling : rescale
**void** **<font color='red'> rescale</font>** (**Ciphertext** &ciph) override;

**Description**：This function performs a rescaling operation on a ciphertext.<br>

**Parameter**：**ciph**:   A reference to a **Ciphertext** object, representing a ciphertext.<br>



### 7. Ciphertext row rotation : rotate_row
**void** **<font color='red'> rotate_col</font>** (**Ciphertext** &encrypted, **const GaloisKeys** &galois_keys, **Ciphertext** &destination);

**Description**：This function is used to perform a column rotation operation on a ciphertext.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **galois_keys**: A constant reference to a **GaloisKeys** object, representing the encryption keys used for row rotation.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **destination**: A reference to a **Ciphertext** object, used to store the ciphertext after rotation.



### 8. Ciphertext column rotation : rotate_col
**void** **<font color='red'> rotate_row</font>** (**Ciphertext** &encrypted, **int** rot_step, **const GaloisKeys** &galois_keys, **Ciphertext** &destination);

**Description**：This function is used to perform a row rotation operation on a ciphertext.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **rot_step**: An integer representing the rotation step length; a positive value indicates a right rotation while a negative value indicates a left rotation.<br>



### 9. Key switching : switch_key
**void** **<font color='red'> switch_key</font>** (**Ciphertext** &ciph, **Ciphertext** &result, const **vector<PublicKey>** &switch_key);

**Description**：This function switches the key of a ciphertext.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **switch_key**:  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys.



### 10. Ciphertext and plaintext matrix multiplication : multiplyByDiagMatrixBSGS
**void** **<font color='red'> multiplyByDiagMatrixBSGS</font>** (**Ciphertext** &ciph, **MatrixPlain** &plain_mat, **Ciphertext** &result, **const GaloisKeys** &rot_key) override;

**Description**：This function multiplies a ciphertext with a plaintext matrix homomorphically, using the BSGS algorithm to accelerate rotation operations.<br>

**Parameter**：**encrypted**: A reference to a **Ciphertext** object, representing a ciphertext.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **plain_mat**:  A reference to a **MatrixPlain** object, representing a plaintext matrix.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **result**: A reference to a **Ciphertext** object, used to store the computation result.<br>
​&#8195;​&#8195; ​&#8195; ​&#8195; ​&#8195; ​&#8194; **rot_key**:   A constant reference to a **GaloisKeys** object, representing the encryption key used for rotations.


## Version
<table>
    <tr>
        <th colspan="2" style=" text-align:center; vertical-align:middle;height:40px;">Version Update History:</th>
    </tr>
    <tr>
        <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;">2022.12.01</td>
        <td style="width:1300px; text-align:center; vertical-align:middle;">First version created</td>
    </tr>
        <tr>
        <td style="text-align:center; vertical-align:middle; height:40px;">2023.04.01</td>
        <td style="text-align:center; vertical-align:middle;">Updated CKKS API</td>
    </tr>
    <tr>
        <td style="text-align:center; vertical-align:middle; height:40px;">2023.06.01 </td>
        <td style="text-align:center; vertical-align:middle;">Second version created</td>
    </tr>
    <tr>
        <td style="text-align:center; vertical-align:middle; height:40px;">2023.07.01 </td>
        <td style="text-align:center; vertical-align:middle;">Updated BFV API</td>
    </tr>
</table>

