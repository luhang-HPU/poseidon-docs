# Basic APIs

## Data Structures

The following data structures are applicable for all the three FHE schemes (**BFV**, **BGV** and **CKKS**) supported by Poseidon.

<br>

### 1. Memory address management : **<font color='red'><span id="MemoryManager">MemoryManager</span> </font>**

**Description**: `MemoryManager` class is used to manage memory space in both software and hardware mode. It is not recommended for users to use it directly.

<br>


### 2. Parameter class of the encryption scheme : **<font color='red'>ParametersLiteral</font>**

**Description**: `ParametersLiteral` class stores the modulus data and modulus-related information, which is used to initialize Poseidon context.

**Functions**:

```cpp
ParametersLiteral(SchemeType scheme_type, uint32_t log_n, uint32_t log_slots,
                      uint32_t log_scale, uint32_t hamming_weight, uint32_t q0_level,
                      Modulus plain_modulus, const vector<Modulus> &q, const vector<Modulus> &p,
                      sec_level_type sec_level = poseidon::sec_level_type::none,
                      MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **scheme_type** (SchemeType): Indicates the scheme type, in which options are *CKKS*, *BFV* and *BGV*.
- **log_n** (uint32_t): The logarithm of polynomial degree number.
- **log_slots** (uint32_t): The logarithm of message slots number.
- **log_scale** (uint32_t): The logarithm of scaling factor.
- **hamming_weight** (uint32_t): The hamming weight of secret key.
- **q0_level** (uint32_t): The RNS level of q_0 (the default value is 0).
- **plain_modulus** (Modulus): The plain modulus (only used in BGV or BFV scheme).
- **q** (const vector<Modulus> &): Determined modulus value of modulus chain Q.
- **p** (const vector<Modulus> &): Determined modulus value of modulus chain P.
- **sec_level** (sec_level_type): the security level -- none, tc128, tc192, tc256.

**Usage**: Constructs customized parameters.



```cpp
void set_log_modulus(const vector<uint32_t> &log_q, const vector<uint32_t> &log_p);
```

- **log_q** (const vector<uint32_t> &):The number of bits for each modulus in the modulus chain Q.
- **log_p** (const vector<uint32_t> &):The number of bits for each modulus in the modulus chain P.

**Usage**: Set the number of bits for each modulus in the modulus chain.



```cpp
void set_modulus(const vector<Modulus> &mod_chain_q, const vector<Modulus> &mod_chain_p);
```

- **mod_chain_q** (const vector<Modulus> &): The value of each module in the modulus chain Q.
- **mod_chain_p** const vector<Modulus> &): The value of each module in the modulus chain P.

**Usage**: Directly set the value of each module in the modulus chain.



```cpp
uint32_t degree() const;
```

**Usage**: Return the ciphertext polynomial degree which is the power of 2 (usually to be 4096, 8192, 16384, 32768, 65536).



```cpp
uint32_t slot() const;
```

**Usage**: Return the number of slots in ciphertext.



```cpp
const parms_id_type & parms_id() const;
```

**Usage**: Return the id of modulus level presented by 256 bits.



```cpp
const SchemeType &scheme() const;
```

**Usage**: Return the scheme type (BFV, BGV or CKKS). 



```cpp
uint32_t LogN() const;
```

**Usage**: Return the logarithm of polynomial degree = log2(polynomial degree).



```cpp
uint32_t LogSlots() const;
```

**Usage**: Return the logarithm of slot size = log2(slots).



```cpp
uint32_t HammingWeight() const;
```

**Usage**: Return the hamming weight (non-zero bit size) of security parameter.



```cpp
uint32_t q0_level() const;
```

**Usage**: Return the level of the first modulus $q_0$ in modulus chain Q.



```cpp
const Modulus &plain_modulus() const;
```

**Usage**: Return the plaintext polynomial modulus.



```cpp
const vector<Modulus> &Q() const;
```

**Usage**: Return the modulus of modulus chain Q.



```cpp
const vector<Modulus> &P() const;
```

**Usage**: Return the modulus of modulus chain P.



```cpp
uint32_t LogScale() const;
```

**Usage**: Return the logarithm of scaling factor = log2(scaling factor).



```cpp
uint32_t scale() const();
```

**Usage**: Return the scaling factor.

<br>


### 3. Parameter class of the default encryption scheme : **<font color='red'>ParametersLiteralDefault</font>**

**Description**: `ParametersLiteralDefault` is a class for initializing encryption parameters, which is a derived class of `ParametersLiteral`.

**Functions**:

```cpp
ParametersLiteralDefault(SchemeType scheme_type, uint32_t degree,
                             sec_level_type sec_level = poseidon::sec_level_type::tc128,
                             MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **scheme_type** (SchemeType): Indicates the scheme type, in which the options are *CKKS*, *BFV*, *BGV*.
- **degree** (uint32_t): Indicates the degree number, in which the options are *2048*, *4096*, *8192*, *16384*, *32768*, *65536*.
- **sec_level** (sec_level_type): Indicates the security level, in which the options are *none*, *tc128*, *tc192*, *tc256*.

**Usage**: Constructs the default parameters.

<br>


### 4. Context information management class : **<font color='red'>PoseidonContext</font>**

**Description**: `PoseidonContext` is a class used to generate and manage context information.

**Functions**:

```cpp
PoseidonContext(const ParametersLiteral& param_literal, bool using_hardware = true);
```

- **param_literal** (const ParametersLiteral&): Indicates the encryption parameter which is used to calculate context information.
- **using_hardware** (bool): Whether to use HPU hardware accelerator card.

**Usage**: Constructs poseidon context.



```cpp
 shared_ptr<const poseidon::ParametersLiteral> parameters_literal() const;
```

**Usage**: Return the parameters list.



```cpp
KeySwitchVariant key_switch_variant() const;
```

**Usage**: Return the key switch variant of current parameter (BV, GHS or HYBRID)



```cpp
shared_ptr<CrtContext> crt_context() const;
```

**Usage**: Return the software context.




```cpp
shared_ptr<HardwareContext> hardware_context() const;
```

**Usage**: Return the hardware context.



```cpp
bool using_hardware() const;
```

**Usage**: Return the status of whether to use HPU hardware accelerator card.



```cpp
void set_random_generator(std::shared_ptr<UniformRandomGeneratorFactory> random_generator);
```

- **random_generator** (std::shared_ptr\<UniformRandomGeneratorFactory\>): A shared pointer of the random generator.

**Usage**: Used to set up a random number generator. 




```cpp
std::shared_ptr<UniformRandomGeneratorFactory> random_generator() const;
```

**Usage**: The random number generator is returned.

<br>


### 5. Plaintext class : **<font color='red'><span id="Plaintext">Plaintext</span> </font>** 

**Description**: `Plaintext` is a class for storing plaintext information.

**Functions**:

```cpp
Plaintext(MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs an empty plaintext without allocating memory.



```cpp
Plaintext(std::size_t coeff_count, MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **coeff_count** (std::size_t): The number of (zeroed) coefficients in the plaintext polynomial.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs a plaintext representing a constant polynomial 0. The coefficient count of the polynomial is set to the given value. The capacity is set to the same value.



```cpp
Plaintext(const std::string &hex_poly, MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **hex_poly** (const std::string &): The formatted polynomial string specifying the plaintext polynomial.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs a plaintext from a given hexadecimal string describing the plaintext polynomial.

Note: The string description of the polynomial must adhere to the format returned by to_string(), which is of the form "7FFx^3 + 1x^1 + 3" and summarized by the following rules: 

    1. Terms are listed in order of strictly decreasing exponent 
    2. Coefficient values are non-negative and in hexadecimal format (upper
    and lower case letters are both supported)
    3. Exponents are positive and in decimal format
    4. Zero coefficient terms (including the constant term) may be (but do
    not have to be) omitted
    5. Term with the exponent value of one must be exactly written as x^1
    6. Term with the exponent value of zero (the constant term) must be written
    as just a hexadecimal number without exponent
    7. Terms must be separated by exactly <space>+<space> and minus is not
    allowed
    8. Other than the +, no other terms should have whitespace



```cpp
Plaintext(const Plaintext &copy) = default;
```

- **copy** (const Plaintext &):The plaintext to copy from.

**Usage**: Creates a new plaintext by copying a given one.



```cpp
Plaintext(Plaintext &&source) = default;
```

- **source** (Plaintext &&source):The plaintext to move from.

**Usage**: Creates a new plaintext by moving a given one.



```cpp
void reserve(std::size_t capacity);
```

- **capacity** (std::size_t):The capacity.

**Usage**: Allocates enough memory to accommodate the backing array of a plaintext with given capacity.



```cpp
void resize(const PoseidonContext &context, parms_id_type parms_id, size_t size);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **size** (std::size_t): The number of coefficients in the plaintext polynomial.

**Usage**: Resizes the plaintext to have the given coefficient count. The plaintext is automatically reallocated if the new coefficient count does not fit in
the current capacity.



```cpp
void release();
```

**Usage**: Resets the plaintext. This function releases any memory allocated by the plaintext, returning it to the memory pool.



```cpp
Plaintext &operator=(const Plaintext &assign) = default;
```

- **assign** (const Plaintext &): The plaintext to copy from.

**Usage**: Copies a given plaintext to the current one.



```cpp
Plaintext &operator=(Plaintext &&assign) = default;
```

- **assign** (const Plaintext &): The plaintext to move from.

**Usage**: Moves a given plaintext to the current one.




```cpp
Plaintext &operator=(const std::string &hex_poly);
```

- **hex_poly** (const std::string &): The formatted polynomial string specifying the plaintext
  polynomial.

**Usage**: Sets the value of the current plaintext to the polynomial represented by
the given hexadecimal string.

Note: The string description of the polynomial must adhere to the format returned by to_string(),which is of the form "7FFx^3 + 1x^1 + 3" and summarized by the following rules

    1. Terms are listed in order of strictly decreasing exponent 
    2. Coefficient values are non-negative and in hexadecimal format (upper
    and lower case letters are both supported)
    3. Exponents are positive and in decimal format
    4. Zero coefficient terms (including the constant term) may be (but do
    not have to be) omitted
    5. Term with the exponent value of one must be exactly written as x^1
    6. Term with the exponent value of zero (the constant term) must be written
    as just a hexadecimal number without exponent
    7. Terms must be separated by exactly <space>+<space> and minus is not
    allowed
    8. Other than the +, no other terms should have whitespace



```cpp
Plaintext &operator=(pt_coeff_type const_coeff);
```

- **const_coeff** (pt_coeff_type): The constant coefficient.

**Usage**: Sets the value of the current plaintext to a given constant polynomial and sets the parms_id to parms_id_zero, effectively marking the plaintext as not NTT transformed. The coefficient count is set to one.



```cpp
void set_zero(std::size_t start_coeff, std::size_t length);
```

- **start_coeff** (std::size_t): The index of the first coefficient to set to zero.
- **length** (std::size_t): The number of coefficients to set to zero.

**Usage**: Sets a given range of coefficients of a plaintext polynomial to zero; does nothing if length is zero.



```cpp
void set_zero(std::size_t start_coeff);
```

- **start_coeff** (std::size_t): The index of the first coefficient to set to zero.

**Usage**: Sets the plaintext polynomial coefficients to zero starting at a given index.



```cpp
void set_zero();
```

**Usage**: Sets the plaintext polynomial to zero.



```cpp
const auto &dyn_array() const;
```

**Usage**: Returns a reference to the backing `DynArray` object.



```cpp
pt_coeff_type *data();
```

**Usage**: Returns a pointer to the beginning of the plaintext polynomial.



```cpp
const pt_coeff_type *data() const;
```

**Usage**: Returns a const pointer to the beginning of the plaintext polynomial.



```cpp
pt_coeff_type *data(std::size_t coeff_index);
```

- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.


**Usage**: Returns a pointer to a given coefficient of the plaintext polynomial.



```cpp
const pt_coeff_type *data(std::size_t coeff_index) const;
```

- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

**Usage**: Returns a const pointer to a given coefficient of the plaintext polynomial.



```cpp
const pt_coeff_type &operator[](std::size_t coeff_index) const;
```

- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

**Usage**: Returns a const reference to a given coefficient of the plaintext polynomial.



```cpp
pt_coeff_type &operator[](std::size_t coeff_index);
```

- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

**Usage**: Returns a reference to a given coefficient of the plaintext polynomial.




```cpp
bool operator==(const Plaintext &compare) const;
```

- **compare** (const Plaintext &): The plaintext to compare against.

**Usage**: Returns whether or not the plaintext has the same semantic value as a given
plaintext. Leading zero coefficients are ignored by the comparison.



```cpp
bool operator!=(const Plaintext &compare) const;
```

- **compare** (const Plaintext &): The plaintext to compare against.

**Usage**: Returns whether or not the plaintext has a different semantic value than
a given plaintext. Leading zero coefficients are ignored by the comparison.



```cpp
bool is_ntt_form() const;
```

**Usage**: Returns whether the plaintext is in NTT form.



```cpp
bool &is_ntt_form();
```

**Usage**: Returns a reference of ntt form.



```cpp
double &scale();
```

**Usage**: Returns a reference to the scale. This is only needed when using the ckks encryption scheme. The user should have little or no reason to ever change the scale by hand.



```cpp
const double &scale() const;
```

**Usage**: Returns a constant reference to the scale. This is only needed when using the ckks encryption scheme.



```cpp
const parms_id_type &parms_id() const;
```

**Usage**: Returns a constant reference to parameter id.



```cpp
arms_id_type &parms_id();
```

**Usage**: Returns a reference to parameter id.



```cpp
bool is_valid(const PoseidonContext &context);
```

**Usage**: Returns whether the plaintext parameter id valid.



```cpp
void compute_ckks_hardware_id() const;
```

**Usage**: Compute the hardware id of this plaintext.



```cpp
MemoryPoolHandle pool() const;
```

**Usage**: Returns the currently used MemoryPoolHandle.



```cpp
bool is_zero() const;
```

**Usage**: Returns whether the current plaintext polynomial has all zero coefficients.



```cpp
std::size_t capacity() const;
```

**Usage**: Returns the capacity of the current allocation.



```cpp
std::size_t coeff_count() const;
```

**Usage**: Returns the coefficient count of the current plaintext polynomial.



```cpp
std::size_t significant_coeff_count() const;
```

**Usage**: Returns the significant coefficient count of the current plaintext polynomial.



```cpp
std::size_t nonzero_coeff_count() const;
```

**Usage**: Returns the non-zero coefficient count of the current plaintext polynomial.



```cpp
std::string to_string() const;
```

**Usage**: Returns a human-readable string description of the plaintext polynomial.

<br>


### 6. Ciphertext class : **<font color='red'><span id="Ciphertext">Ciphertext</span> </font>** 

**Description**: Ciphertext is a class for storing ciphertext information.

**Functions**:

```cpp
Ciphertext(MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool

**Usage**: Constructs an empty ciphertext allocating no memory.



```cpp
Ciphertext(const PoseidonContext &context, MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **context** (const Ciphertext &): The PoseidonContext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs an empty ciphertext with capacity 2. In addition to the capacity, the allocation size is determined by the highest-level parameters associated to the given PoseidonContext.



```cpp
Ciphertext(const PoseidonContext &context, parms_id_type parms_id, MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **context** (const Ciphertext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs an empty ciphertext with capacity 2. In addition to the capacity, the allocation size is determined by the encryption parameters with given parms_id.



```cpp
Ciphertext(const PoseidonContext &context, parms_id_type parms_id, std::size_t size_capacity,MemoryPoolHandle pool = MemoryManager::GetPool());
```

- **context** (const Ciphertext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **size_capacity** (std::size_t): The capacity.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Constructs an empty ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the given encryption parameters.



```cpp
 Ciphertext(const Ciphertext &copy);
```

- **copy** (Ciphertext &&): The ciphertext to copy from.

**Usage**: Creates a new ciphertext by copying a given one.



```cpp
Ciphertext(Ciphertext &&source) = default;
```

- **source** (Ciphertext &&): The ciphertext to move from.

**Usage**: Creates a new ciphertext by moving a given one.



```cpp
Ciphertext(const Ciphertext &copy, MemoryPoolHandle pool);
```

- **copy** (const Ciphertext &copy): The ciphertext to copy from.
- **pool** MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Creates a new ciphertext by copying a given one.



```cpp
void reserve(const PoseidonContext &context, parms_id_type parms_id, std::size_t size_capacity);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The parms_id corresponding to the encryption.
- **size_capacity** (std::size_t): The capacity.

**Usage**: Allocates enough memory to accommodate the backing array of a ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the encryption parameters corresponing to the given parms_id.



```cpp
void reserve(const PoseidonContext &context, std::size_t size_capacity);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **size_capacity** (std::size_t): The capacity.

**Usage**: Allocates enough memory to accommodate the backing array of a ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the highest-level parameters associated to the given PoseidonContext.



```cpp
void resize(const PoseidonContext &context, parms_id_type parms_id, std::size_t size);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The parms_id corresponding to the encryption.
- **size** (std::size_t): The new size.

**Usage**: Resizes the ciphertext to given size, reallocating if the capacity of the ciphertext is too small. The ciphertext parameters are determined by the given PoseidonContext and parms_id.



```cpp
void resize(const PoseidonContext &context, std::size_t size);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **size** (std::size_t): The new size.

**Usage**: Resizes the ciphertext to given size, reallocating if the capacity of the ciphertext is too small. The ciphertext parameters are determined by the highest-level parameters associated to the given PoseidonContext.



```cpp
void release();
```

**Usage**: Resets the ciphertext. This function releases any memory allocated by the ciphertext, returning it to the memory pool. It also sets all encryption parameter specific size information to zero.



```cpp
Ciphertext &operator=(const Ciphertext &assign);
```

- **assign** (const Ciphertext &): The ciphertext to copy from.

**Usage**: Copies a given ciphertext to the current one.




```cpp
Ciphertext &operator=(Ciphertext &&assign) = default;
```

- **assign** (Ciphertext &&): The ciphertext to move from.

**Usage**: Moves a given ciphertext to the current one.




```cpp
 const auto &dyn_array() const;
```

**Usage**: Returns a reference to the backing DynArray object.




```cpp
ct_coeff_type *data();
```

**Usage**: Returns a pointer to the beginning of the ciphertext data.



```cpp
const ct_coeff_type *data() const;
```

**Usage**: Returns a const pointer to the beginning of the ciphertext data.



```cpp
ct_coeff_type *data(std::size_t poly_index);
```

- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

**Usage**: Returns a pointer to a particular polynomial in the ciphertext data.



```cpp
const ct_coeff_type *data(std::size_t poly_index) const;
```

- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

**Usage**: Returns a const pointer to a particular polynomial in the ciphertext data.




```cpp
const RNSPoly &operator[] (std::size_t poly_index);
```

- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

**Usage**: Returns a const RNSPoly class to a particular polynomial in the ciphertext data.



```cpp
const vector<RNSPoly> &polys() const;
```

**Usage**: Returns a const reference of RNSPoly vector.



```cpp
const vector<RNSPoly> &polys() const;
```

**Usage**: Returns a reference of RNSPoly vector.



```cpp
RNSPoly &operator[] (std::size_t poly_index);
```

- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

**Usage**: Returns a RNSPoly class to a particular polynomial in the ciphertext data.



```cpp
bool is_ntt_form() const noexcept;
```

**Usage**: Returns whether the ciphertext is in NTT form.



```cpp
bool &is_ntt_form() noexcept;
```

**Usage**: Returns whether the ciphertext is in NTT form.



```cpp
parms_id_type &parms_id() noexcept;
```

**Usage**: Returns a reference to parms_id.



```cpp
const parms_id_type &parms_id() const noexcept;
```

**Usage**: Returns a const reference to parms_id.



```cpp
double &scale() noexcept;
```

**Usage**: Returns a reference to the scale. This is only needed when using the ckks encryption scheme. The user should have little or no reason to ever change the scale by hand.



```cpp
const double &scale() const noexcept;
```

**Usage**: Returns a constant reference to the scale. This is only needed when using the ckks encryption scheme.



```cpp
std::uint64_t &correction_factor() noexcept;
```

**Usage**: Returns a reference to the correction factor. This is only needed when using the BGV encryption scheme. The user should have little or no reason to ever change the correction factor by hand.



```cpp
const std::uint64_t &correction_factor() const noexcept;
```

**Usage**: Returns a constant reference to the correction factor. This is only needed when using the BGV encryption scheme.



```cpp
std::size_t coeff_modulus_size() const;
```

**Usage**: Returns the number of primes in the coefficient modulus of the associated encryption parameters. This directly affects the allocation size of the ciphertext.



```cpp
std::size_t level() const;
```

**Usage**: Returns the level, which equals coeff_modulus_size - 1.



```cpp
std::size_t poly_modulus_degree() const;
```

**Usage**: Returns the degree of the polynomial modulus of the associated encryption parameters. This directly affects the allocation size of the ciphertext.



```cpp
std::size_t size() const;
```

**Usage**: Returns the size of the ciphertext.



```cpp
std::size_t size_capacity() const noexcept;
```

**Usage**: Returns the capacity of the allocation. This means the largest size of the ciphertext that can be stored in the current allocation with the current encryption parameters.




```cpp
bool is_transparent() const;
```

**Usage**: Check whether the current ciphertext is transparent, i.e. does not require a secret key to decrypt. In typical security models such transparent ciphertexts would not be considered to be valid. Starting from the second polynomial in the current ciphertext, this function returns true if all following coefficients are identically zero. Otherwise, returns false.




```cpp
MemoryPoolHandle pool() const noexcept;
```

**Usage**: Returns the currently used MemoryPoolHandle.



```cpp
bool is_valid();
```

**Usage**: Returns whether the ciphertext is valid.



```cpp
void compute_ckks_hardware_id() const;
```

**Usage**: Compute the hardware id of this ciphertext.

<br>



### 7. Public key class : **<font color='red'><span id="PublicKey">PublicKey</span> </font>**  

**Description**: PublicKey is a class for storing public key information.

**Functions**:

```cpp
PublicKey();
```

**Usage**: Constructs an empty publicKey allocating no memory.



```cpp
PublicKey(const PublicKey &copy) = default;
```

- **copy** (const PublicKey &): The given PublicKey

**Usage**: Creates a new publicKey by copying a given one.



```cpp
PublicKey(PublicKey &&source) = default;
```

- **source** (PublicKey &&): The given PublicKey

**Usage**: Moves a new publicKey by moving a given one.



```cpp
PublicKey &operator=(const PublicKey &assign) = default;
```

- **assign** (const PublicKey &assign): The given PublicKey

**Usage**: Copies a given publicKey to the current one.



```cpp
PublicKey &operator=(PublicKey &&assign) = default;
```

- **assign** (PublicKey &&): The given PublicKey

**Usage**: Moves a given publicKey to the current one.



```cpp
const Ciphertext data() const noexcept;
```

**Usage**: The function is used to get the key.

<br>

### 8. Relinearize key class : **<font color='red'><span id="RelinKeys">RelinKeys</span> </font>** 


**Description**:RelinKeys is a class for storing relinearized key information.


**Functions**:

```cpp
RelinKeys();
```

**Usage**: Constructs an empty RelinKeys allocating no memory.



```cpp
RelinKeys(const RelinKeys &copy) = default;
```

- **copy** (const RelinKeys &): The given RelinKeys

**Usage**: Creates a new RelinKeys by copying a given one.



```cpp
RelinKeys(RelinKeys &&source) = default;
```

- **source** (RelinKeys &&): The given RelinKeys

**Usage**: Moves a new RelinKeys by moving a given one.



```cpp
RelinKeys &operator=(const RelinKeys &assign) = default;
```

- **assign** (const RelinKeys &): The given RelinKeys

**Usage**: Copies a given RelinKeys to the current one.



```cpp
RelinKeys &operator=(RelinKeys &&assign) = default;
```

- **assign** (RelinKeys &&): The given RelinKeys

**Usage**: Moves a given RelinKeys to the current one.



```cpp
const std::vector<std::vector<PublicKey>> &key() const;
```

**Usage**: Returns a const reference to a Galois key. The returned Galois key corresponds to the given Galois element.

<br>


### 9. Galois key class : **<font color='red'><span id="GaloisKeys">GaloisKeys</span> </font>**

**Description**:GaloisKeys is a class for storing Galoiskeys information.

**Functions**: 

```cpp
GaloisKeys();
```

**Usage**: Constructs an empty GaloisKeys allocating no memory.



```cpp
GaloisKeys(const GaloisKeys &copy) = default;
```

- **copy** (const GaloisKeys &): The given GaloisKeys

**Usage**: Creates a new GaloisKeys by copying a given one.



```cpp
GaloisKeys(GaloisKeys &&source) = default;
```

- **source** (GaloisKeys &&): The given GaloisKeys

**Usage**: Moves a new GaloisKeys by moving a given one.



```cpp
GaloisKeys &operator=(const GaloisKeys &assign) = default;
```

- **assign** (const GaloisKeys &): The given GaloisKeys

**Usage**: Copies a given GaloisKeys to the current one.



```cpp
GaloisKeys &operator=(GaloisKeys &&assign) = default;
```

- **assign** (GaloisKeys &&): The given GaloisKeys

**Usage**: Moves a given GaloisKeys to the current one.



```cpp
static inline std::size_t get_index(uint32_t galois_elt);
```

- **galois_elt** (uint32_t):The Galois element.

**Usage**: A function used to obtain the corresponding index of a Galois element.




```cpp
inline bool has_key(uint32_t galois_elt) const;
```

- **galois_elt** (uint32_t):The Galois element.

**Usage**: Returns whether a Galois key corresponding to a given Galois element exists.



```cpp
const std::vector<std::vector<PublicKey>> &key(std::uint32_t galois_elt) const;
```

**Usage**: Returns a const reference to a Galois key. The returned Galois key corresponds to the given Galois element.

<br>


### 10. Key generation class : **<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**

**Description**: KeyGenerator is a class for generating keys.

**Functions**:

```cpp
KeyGenerator(const PoseidonContext &params);
```

- **params** (const PoseidonContext &): The poseidon context.

**Usage**: Constructs the key generator without keys.





```cpp
KeyGenerator(const PoseidonContext& params,const SecretKey &sk);
```

- **params** (const PoseidonContext &): The poseidon context.
- **sk** (const SecretKey &): The secret key.

**Usage**: Constructs the key generator with secret key.



```cpp
inline void create_public_key(PublicKey &destination) const;
```

- **destination** (PublicKey &):The PublicKey which is used to store the computation result.

**Usage**: A function for creating a public key. 



```cpp
inline void create_relin_keys(RelinKeys &destination);
```

- **destination** (RelinKeys &):The RelinKeys which is used to store the computation result.

**Usage**: A function to create a relinearized key.



```cpp
inline void create_galois_keys(const std::vector<int> &steps, GaloisKeys &destination);
```

- **steps** (const std::vector<int> &): The group of rotate steps which is used to create galois keys.
- **destination** (RelinKeys &):A GaloisKeys which is used to store the computation result.

**Usage**: A function used to create a rotation key based on a given rotation step vector.



```cpp
inline void create_conj_keys(GaloisKeys &destination);
```

- **destination** (RelinKeys &): The GaloisKeys which is used to store the computation result.

**Usage**: A function used to create a conjugate rotation key. It is only used in CKKS conjugate.

<br>

### 11. Encryption class : **<font color='red'><span id="Encryptor">Encryptor</span> </font>**

**Description**: Encryptor is a class used to encrypt plaintext.

**Functions**:

```cpp
Encryptor(const PoseidonContext &context, const SecretKey &secret_key);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **secret_key** (const SecretKey &): The secret key.

**Usage**: Creates an Encryptor instance initialized with the specified PoseidonContext and secret key.



```cpp
Encryptor(const PoseidonContext &context, const PublicKey &public_key);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **public_key** (const PublicKey &): The public key.

**Usage**: Creates an Encryptor instance initialized with the specified PoseidonContext and public key.



```cpp
Encryptor(const PoseidonContext &context, const PublicKey &public_key, const SecretKey &secret_key);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **public_key** (const PublicKey &): The public key.
- **secret_key** (const SecretKey &): The secret key.

**Usage**: Creates an Encryptor instance initialized with the specified PoseidonContext,secret key, and public key.




```cpp
void set_public_key(const PublicKey &public_key);
```

- **public_key** (const PublicKey &):The public key.

**Usage**: Give a new instance of public key.



```cpp
void set_secret_key(const SecretKey &secret_key);
```

- **secret_key** (const SecretKey &): The secret key.

**Usage**: Give a new instance of secret key.





```cpp
void encrypt(const Plaintext &plain, Ciphertext &destination,MemoryPoolHandle pool = MemoryManager::GetPool())const;
```

- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** ( Ciphertext &): The ciphertext to overwrite with the encrypt.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a plaintext with the public key and stores the result in destination.




```cpp
void encrypt_zero(Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a zero plaintext with the public key and stores the result in destination.



```cpp
void encrypt_zero(parms_id_type parms_id, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **parms_id** (parms_id_type): The parms_id for the resulting ciphertext.
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a zero plaintext with the public key and stores the result in destination.




```cpp
void encrypt_symmetric(const Plaintext &plain, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a plaintext with the secret key and stores the result in destination.




```cpp
void encrypt_zero_symmetric(parms_id_type parms_id, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **parms_id** (parms_id_type): The parms_id for the resulting ciphertext
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a zero plaintext with the secret key and stores the result in destination.



```cpp
void encrypt_zero_symmetric(Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```

- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

**Usage**: Encrypts a zero plaintext with the secret key and stores the result in destination.

<br>


### 12. Decryption class : **<font color='red'><span id="Decryptor">Decryptor</span> </font>**

**Description**: Decryptor is a class for decrypting plaintext.

**Functions**:

```cpp
Decryptor(const PoseidonContext &context, const SecretKey &secret_key);
```

- **context** (const PoseidonContext &): The PoseidonContext.
- **secret_key** (const SecretKey &): The secret key.

**Usage**: Creates a Decryptor instance initialized with the specified PoseidonContext and secret key.




```cpp
void decrypt(const Ciphertext &encrypted, Plaintext &destination);
```

- **encrypted** (const Ciphertext &): The ciphertext to decrypt.
- **destination** (Plaintext &): The plaintext to overwrite with the decrypted.

**Usage**: Decrypts a Ciphertext and stores the result in the destination parameter.

<br>


### 13.  Factory Method Pattern class : **<font color='red'><span id="EvaluatorFactory">PoseidonFactory</span> </font>**

**Description**: PoseidonFactory class is used to create the Poseidon algorithm library.

**Functions**: 

```cpp
auto create(const PoseidonContext &context)->std::shared_ptr< Envaluator >;
```

**Usage**: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted FHE algorithms such as BFV, BGV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  



```cpp
static PoseidonFactory *get_instance()
```

**Usage**: The `PoseidonFactory` implements the singleton pattern. This function is used to get the static object of PoseidonFactory.



```cpp
PoseidonContext create_poseidon_context(const ParametersLiteral &param_literal,
                            			sec_level_type sec_level = sec_level_type::none) const;
```

**Usage**: `create_poseidon_context` is used to create `PoseidonContext` class based on the given `param_literal` .



```cpp
std::unique_ptr<EvaluatorBfvBase> create_bfv_evaluator(PoseidonContext &context) const
std::unique_ptr<EvaluatorBgvBase> create_bgv_evaluator(PoseidonContext &context) const
std::unique_ptr<EvaluatorCkksBase> create_ckks_evaluator(PoseidonContext &context) const
```

**Usage**: `create_bfv_evaluator` is used to create `evaluator` class for BFV scheme.

`create_bgv_evaluator` is used to create `evaluator` class for BGV scheme.

`create_ckks_evaluator` is used to create `evaluator` class for CKKS scheme.



```cpp
DEVICE_TYPE get_device_type() const
```

**Usage**: Getting the device type. It returns *DEVICE_TYPE::DEVICE_SOFTWAR* when using software mode, and returns *DEVICE_TYPE::DEVICE_HARDWARE* when using hardware mode.



```cpp
void set_device_type(DEVICE_TYPE type)
```

**Usage**: Setting the device type.

<br>
