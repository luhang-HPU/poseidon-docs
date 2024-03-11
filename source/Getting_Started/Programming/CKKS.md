# CKKS 

## Data Structures




Poseidon Supported parameter data structures:

<br>

### 1. Memory address management : **<font color='red'><span id="MemoryManager">MemoryManager</span> </font>**


**Description**: MemoryManager is a class used to manage addresses and memory space in hardware mode. It is not recommended for users to use it directly.



<br>


### 2. Parameter class of the encryption scheme : **<font color='red'>ParametersLiteral</font>**

**Description**: ParametersLiteral  is a class for initializing encryption parameters.

**Functions**:
```cpp
ParametersLiteral(SchemeType schemeType, uint32_t LogN, uint32_t LogSlots, uint32_t LogScale, uint32_t HammingWeight, uint32_t q0_level, Modulus plain_modulus, const vector<Modulus> &Q, const vector<Modulus> &P);
```
- **schemeType** (SchemeType): Indicates the scheme type. The optional values are "CKKS", "BFV", "BGV".
- **LogN** (uint32_t): The logarithm of polynomial degree number.
- **LogSlots** (uint32_t):The logarithm of message slots number.
- **LogScale** (uint32_t):The logarithm of scaling factor.
- **HammingWeight** (uint32_t):The hamming weight of secrect key.
- **q0_level** (uint32_t):The rns level of q0.Its default value is 0.
- **plain_modulus** (Modulus):The plain modulus.Only used in BGV or BFV.
- **Q** (const vector<Modulus> &):Determined modulus value of modulus chain Q.
- **P** (const vector<Modulus> &):Determined modulus value of modulus chain P.

: Constructs ckks customized parameters.

<br>

```cpp
void set_log_modulus(const vector<uint32_t> &logQ,const vector<uint32_t> &logP);
```
- **logQ** (const vector<uint32_t> &):The number of bits for each module in the modulus chain Q.
- **logP** (const vector<uint32_t> &):The number of bits for each module in the modulus chain P.

: Set the number of bits for each module in the modulus chain.

<br>

```cpp
void set_modulus(const vector<Modulus> &mod_chain_q,const vector<Modulus> &mod_chain_p);
```
- **mod_chain_q** (const vector<Modulus> &):The value of each module in the modulus chain Q.
- **mod_chain_p** const vector<Modulus> &):The value of each module in the modulus chain P.

: Directly set the value of each module in the modulus chain.

<br>

```cpp
uint32_t degree() const;
```
: Return the polynomial degree.

<br>

```cpp
uint32_t slot() const
```
: Return the number of message slots.

<br>

```cpp
const parms_id_type& parms_id() const
```
: Return the id of the params.

<br>

```cpp
const SchemeType &scheme() const
```
: Return the scheme type. 

<br>

```cpp
uint32_t LogN() const
```
: Return the logarithm of polynomial degree number.

<br>

```cpp
uint32_t LogSlots() const
```
: Return the logarithm of message slots number.

<br>

```cpp
uint32_t HammingWeight() const
```
: Return the hamming weight.

<br>

```cpp
uint32_t q0_level() const
```
: Return the rns level of q0.

<br>

```cpp
const Modulus &plain_modulus() const
```
: Return the plain modulus.

<br>

```cpp
const vector<Modulus> &Q() const
```
: Return the modulus of modulus chain Q.

<br>

```cpp
const vector<Modulus> &P() const
```
: Return the modulus of modulus chain P.

<br>

```cpp
uint32_t LogScale() const
```
: Return the logarithm of scaling factor.

<br>

```cpp
uint32_t scale() const() 
```
: Return the scaling factor.



<br>


### 2. Parameter class of the default encryption scheme : **<font color='red'>ParametersLiteralDefault</font>**

**Description**: ParametersLiteralDefault is a class for initializing encryption parameters.It is a derived class of ParametersLiteral.

**Functions**:
```cpp
ParametersLiteralDefault(SchemeType schemeType,uint32_t degree,sec_level_type sec_level);
```
- **schemeType** (SchemeType): Indicates the scheme type. The optional values are "CKKS", "BFV", "BGV".
- **degree** (uint32_t): Indicates the degree num.The optional values are  "2048", "4096","8192","16384","32768".
- **sec_level** (sec_level_type): Indicates the security level.The optional values are  "none", "tc128","tc192","tc256".

: Constructs ckks default parameters.


<br>


### 3. Context information management class : **<font color='red'>PoseidonContext</font>**

**Description**: PoseidonContext is a class used to generate and manage context information.

**Functions**:
```cpp
PoseidonContext(const ParametersLiteral& paramLiteral,sec_level_type sec_level = sec_level_type::none,bool using_hardware = true);
```
- **paramLiteral** (const ParametersLiteral&): Indicates the encryption parameter used to calculate context information.
- **sec_level** (sec_level_type): The security level to check the parameters.
- **using_hardware** (bool): Whether to use hardware.

: Constructs poseidon context.

<br>

```cpp
 shared_ptr<const poseidon::ParametersLiteral> parameters_literal() const 
```
: Return the parameters list.

<br>

```cpp
KeySwitchVariant key_switch_variant() const ;
```
: Return the key switch variant of current parameter.

<br>

```cpp
shared_ptr<CrtContext> crt_context() const;
```
: Return the software context.

<br>


```cpp
shared_ptr<HardwareContext> hardware_context() const;
```
: Return the hardware context.

<br>

```cpp
bool using_hardware() const ;
```
: Return the status of using hardware.

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






### 4. Plaintext class : **<font color='red'><span id="Plaintext">Plaintext</span> </font>** 


**Description**: Plaintext is a class for storing plaintext information.

**Functions**:
```cpp
Plaintext(MemoryPoolHandle pool = MemoryManager::GetPool())
```
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs an empty plaintext allocating no memory.

<br>

```cpp
Plaintext(std::size_t coeff_count, MemoryPoolHandle pool = MemoryManager::GetPool());
```
- **coeff_count** (std::size_t): The number of (zeroed) coefficients in the plaintext polynomial.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs a plaintext representing a constant polynomial 0. The coefficient count of the polynomial is set to the given value. The capacity is set to the same value.

<br>

```cpp
Plaintext(const std::string &hex_poly, MemoryPoolHandle pool = MemoryManager::GetPool())
```
- **hex_poly** (const std::string &): The formatted polynomial string specifying the plaintext polynomial.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs a plaintext from a given hexadecimal string describing the plaintext polynomial.

: The string description of the polynomial must adhere to the format returned by to_string(),which is of the form "7FFx^3 + 1x^1 + 3" and summarized by the following rules:

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

<br>

```cpp
Plaintext(const Plaintext &copy) = default;
```
- **copy** (const Plaintext &):The plaintext to copy from.

: Creates a new plaintext by copying a given one.

<br>

```cpp
Plaintext(Plaintext &&source) = default;
```
- **source** (Plaintext &&source):The plaintext to move from.

: Creates a new plaintext by moving a given one.

<br>

```cpp
void reserve(std::size_t capacity)
```
- **capacity** (std::size_t):The capacity.

: Allocates enough memory to accommodate the backing array of a plaintext with given capacity.

<br>

```cpp
void resize(const PoseidonContext &context, parms_id_type parms_id, size_t size);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **size** (std::size_t): The number of coefficients in the plaintext polynomial.


: Resizes the plaintext to have a given coefficient count. The plaintext
is automatically reallocated if the new coefficient count does not fit in
the current capacity.

<br>

```cpp
void release();
```

: Resets the plaintext. This function releases any memory allocated by the
plaintext, returning it to the memory pool.

<br>

```cpp
Plaintext &operator=(const Plaintext &assign) = default;
```
- **assign** (const Plaintext &): The plaintext to copy from.

: Copies a given plaintext to the current one.

<br>

```cpp
Plaintext &operator=(Plaintext &&assign) = default;
```
- **assign** (const Plaintext &): The plaintext to move from.

: Moves a given plaintext to the current one.

<br>


```cpp
Plaintext &operator=(const std::string &hex_poly);
```
- **hex_poly** (const std::string &): The formatted polynomial string specifying the plaintext
polynomial.

: Sets the value of the current plaintext to the polynomial represented by
the given hexadecimal string.

: The string description of the polynomial must adhere to the format returned by to_string(),which is of the form "7FFx^3 + 1x^1 + 3" and summarized by the following rules:

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

<br>

```cpp
Plaintext &operator=(pt_coeff_type const_coeff);
```
- **const_coeff** (pt_coeff_type): The constant coefficient.

: Sets the value of the current plaintext to a given constant polynomial and
sets the parms_id to parms_id_zero, effectively marking the plaintext as
not NTT transformed. The coefficient count is set to one.

<br>

```cpp
void set_zero(std::size_t start_coeff, std::size_t length);
```
- **start_coeff** (std::size_t): The index of the first coefficient to set to zero.
- **length** (std::size_t): The number of coefficients to set to zero.

: Sets a given range of coefficients of a plaintext polynomial to zero; does
nothing if length is zero.

<br>

```cpp
void set_zero(std::size_t start_coeff);
```
- **start_coeff** (std::size_t): The index of the first coefficient to set to zero.

: Sets the plaintext polynomial coefficients to zero starting at a given index.

<br>

```cpp
void set_zero();
```

: Sets the plaintext polynomial to zero.

<br>

```cpp
const auto &dyn_array() const;
```

: Returns a reference to the backing DynArray object.

<br>

```cpp
pt_coeff_type *data();
```

: Returns a pointer to the beginning of the plaintext polynomial.

<br>

```cpp
const pt_coeff_type *data() const;
```

: Returns a const pointer to the beginning of the plaintext polynomial.

<br>

```cpp
pt_coeff_type *data(std::size_t coeff_index);
```
- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.


: Returns a pointer to a given coefficient of the plaintext polynomial.

<br>

```cpp
const pt_coeff_type *data(std::size_t coeff_index) const;
```
- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

: Returns a const pointer to a given coefficient of the plaintext polynomial.

<br>

```cpp
const pt_coeff_type &operator[](std::size_t coeff_index) const;
```
- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

: Returns a const reference to a given coefficient of the plaintext polynomial.

<br>

```cpp
pt_coeff_type &operator[](std::size_t coeff_index);
```
- **coeff_index** (std::size_t): The index of the coefficient in the plaintext polynomial.

: Returns a reference to a given coefficient of the plaintext polynomial.

<br>


```cpp
bool operator==(const Plaintext &compare) const;
```
- **compare** (const Plaintext &): The plaintext to compare against.

: Returns whether or not the plaintext has the same semantic value as a given
plaintext. Leading zero coefficients are ignored by the comparison.

<br>

```cpp
bool operator!=(const Plaintext &compare) const;
```
- **compare** (const Plaintext &): The plaintext to compare against.

: Returns whether or not the plaintext has a different semantic value than
a given plaintext. Leading zero coefficients are ignored by the comparison.

<br>

```cpp
bool is_ntt_form() const;
```

: Returns whether the plaintext is in NTT form.

<br>

```cpp
bool &is_ntt_form();
```

: Returns a reference of ntt form.

<br>

```cpp
double &scale();
```

: Returns a reference to the scale. This is only needed when using the ckks encryption scheme. The user should have little or no reason to ever change the scale by hand.

<br>

```cpp
const double &scale() const;
```

: Returns a constant reference to the scale. This is only needed when using the ckks encryption scheme.

<br>

```cpp
const parms_id_type &parms_id() const;
```

: Returns a constant reference to parameter id.

<br>

```cpp
arms_id_type &parms_id();
```

: Returns a reference to parameter id.

<br>

```cpp
bool is_valid(const PoseidonContext &context);
```
: Returns whether the plaintext parameter id valid.

<br>

```cpp
void compute_ckks_hardware_id() const;
```
: Compute the hardware id of this plaintext.

<br>

```cpp
MemoryPoolHandle pool() const;
```

: Returns the currently used MemoryPoolHandle.

<br>

```cpp
bool is_zero() const;
```

: Returns whether the current plaintext polynomial has all zero coefficients.

<br>

```cpp
std::size_t capacity() const;
```

: Returns the capacity of the current allocation.

<br>

```cpp
std::size_t coeff_count() const;
```

: Returns the coefficient count of the current plaintext polynomial.

<br>

```cpp
std::size_t significant_coeff_count() const;
```

: Returns the significant coefficient count of the current plaintext polynomial.

<br>

```cpp
std::size_t nonzero_coeff_count() const;
```

: Returns the non-zero coefficient count of the current plaintext polynomial.

<br>

```cpp
std::string to_string() const;
```

: Returns a human-readable string description of the plaintext polynomial.

<br>


### 5. Ciphertext class : **<font color='red'><span id="Ciphertext">Ciphertext</span> </font>** 


**Description**: Ciphertext is a class for storing ciphertext information.

**Functions**:
```cpp
Ciphertext(MemoryPoolHandle pool = MemoryManager::GetPool());
```
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool

: Constructs an empty ciphertext allocating no memory.

<br>

```cpp
Ciphertext(const PoseidonContext &context, MemoryPoolHandle pool = MemoryManager::GetPool());
```
- **context** (const Ciphertext &): The PoseidonContext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs an empty ciphertext with capacity 2. In addition to the capacity, the allocation size is determined by the highest-level parameters associated to the given PoseidonContext.

<br>

```cpp
Ciphertext(const PoseidonContext &context, parms_id_type parms_id, MemoryPoolHandle pool = MemoryManager::GetPool());
```
- **context** (const Ciphertext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs an empty ciphertext with capacity 2. In addition to the capacity, the allocation size is determined by the encryption parameters with given parms_id.

<br>

```cpp
Ciphertext(const PoseidonContext &context, parms_id_type parms_id, std::size_t size_capacity,MemoryPoolHandle pool = MemoryManager::GetPool());
```
- **context** (const Ciphertext &): The PoseidonContext.
- **parms_id** (parms_id_type): The id of current parameter.
- **size_capacity** (std::size_t): The capacity.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Constructs an empty ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the given encryption parameters.

<br>

```cpp
 Ciphertext(const Ciphertext &copy);
```
- **copy** (Ciphertext &&): The ciphertext to copy from.

: Creates a new ciphertext by copying a given one.

<br>

```cpp
Ciphertext(Ciphertext &&source) = default;
```
- **source** (Ciphertext &&): The ciphertext to move from.

: Creates a new ciphertext by moving a given one.

<br>

```cpp
Ciphertext(const Ciphertext &copy, MemoryPoolHandle pool);
```
- **copy** (const Ciphertext &copy): The ciphertext to copy from.
- **pool** MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Creates a new ciphertext by copying a given one.

<br>

```cpp
void reserve(const PoseidonContext &context, parms_id_type parms_id, std::size_t size_capacity);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The parms_id corresponding to the encryption.
- **size_capacity** (std::size_t): The capacity.

: Allocates enough memory to accommodate the backing array of a ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the encryption parameters corresponing to the given parms_id.

<br>

```cpp
void reserve(const PoseidonContext &context, std::size_t size_capacity);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **size_capacity** (std::size_t): The capacity.

: Allocates enough memory to accommodate the backing array of a ciphertext with given capacity. In addition to the capacity, the allocation size is determined by the highest-level parameters associated to the given PoseidonContext.

<br>

```cpp
void resize(const PoseidonContext &context, parms_id_type parms_id, std::size_t size);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **parms_id** (parms_id_type): The parms_id corresponding to the encryption.
- **size** (std::size_t): The new size.

: Resizes the ciphertext to given size, reallocating if the capacity of the ciphertext is too small. The ciphertext parameters are determined by the given PoseidonContext and parms_id.

<br>

```cpp
void resize(const PoseidonContext &context, std::size_t size);
```
- **context** (const PoseidonContext &): The PoseidonContext.
- **size** (std::size_t): The new size.

: Resizes the ciphertext to given size, reallocating if the capacity of the ciphertext is too small. The ciphertext parameters are determined by the highest-level parameters associated to the given PoseidonContext.

<br>

```cpp
void release();
```

: Resets the ciphertext. This function releases any memory allocated by the ciphertext, returning it to the memory pool. It also sets all encryption parameter specific size information to zero.

<br>



```cpp
Ciphertext &operator=(const Ciphertext &assign);
```
- **assign** (const Ciphertext &): The ciphertext to copy from.

: Copies a given ciphertext to the current one.

<br>


```cpp
Ciphertext &operator=(Ciphertext &&assign) = default;
```
- **assign** (Ciphertext &&): The ciphertext to move from.

: Moves a given ciphertext to the current one.

<br>


```cpp
 const auto &dyn_array() const;
```

: Returns a reference to the backing DynArray object.

<br>


```cpp
ct_coeff_type *data();
```

: Returns a pointer to the beginning of the ciphertext data.

<br>

```cpp
const ct_coeff_type *data() const;
```

: Returns a const pointer to the beginning of the ciphertext data.

<br>

```cpp
ct_coeff_type *data(std::size_t poly_index);
```
- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

: Returns a pointer to a particular polynomial in the ciphertext data.

<br>

```cpp
const ct_coeff_type *data(std::size_t poly_index) const;
```
- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

: Returns a const pointer to a particular polynomial in the ciphertext data.

<br>


```cpp
const RNSPoly &operator[] (std::size_t poly_index);
```
- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

: Returns a const RNSPoly class to a particular polynomial in the ciphertext data.

<br>

```cpp
const vector<RNSPoly> &polys() const;
```

: Returns a const reference of RNSPoly vector.

<br>

```cpp
const vector<RNSPoly> &polys() const;
```

: Returns a reference of RNSPoly vector.

<br>

```cpp
RNSPoly &operator[] (std::size_t poly_index);
```
- **poly_index** (std::size_t): The index of the polynomial in the ciphertext.

: Returns a RNSPoly class to a particular polynomial in the ciphertext data.

<br>

```cpp
bool is_ntt_form() const noexcept;
```

: Returns whether the ciphertext is in NTT form.

<br>

```cpp
bool &is_ntt_form() noexcept;
```

: Returns whether the ciphertext is in NTT form.

<br>

```cpp
parms_id_type &parms_id() noexcept;
```

: Returns a reference to parms_id.

<br>

```cpp
const parms_id_type &parms_id() const noexcept;
```

: Returns a const reference to parms_id.

<br>

```cpp
double &scale() noexcept;
```

: Returns a reference to the scale. This is only needed when using the ckks encryption scheme. The user should have little or no reason to ever change the scale by hand.

<br>

```cpp
const double &scale() const noexcept;
```

: Returns a constant reference to the scale. This is only needed when using the ckks encryption scheme.

<br>

```cpp
std::uint64_t &correction_factor() noexcept;
```

: Returns a reference to the correction factor. This is only needed when using the BGV encryption scheme. The user should have little or no reason to ever change the correction factor by hand.

<br>

```cpp
const std::uint64_t &correction_factor() const noexcept;
```

: Returns a constant reference to the correction factor. This is only needed when using the BGV encryption scheme.

<br>

```cpp
std::size_t coeff_modulus_size() const;
```

: Returns the number of primes in the coefficient modulus of the associated encryption parameters. This directly affects the allocation size of the ciphertext.

<br>

```cpp
std::size_t level() const;
```

: coeff_modulus_size - 1.

<br>

```cpp
std::size_t poly_modulus_degree() const;
```

: Returns the degree of the polynomial modulus of the associated encryption parameters. This directly affects the allocation size of the ciphertext.

<br>

```cpp
std::size_t size() const;
```

: Returns the size of the ciphertext.

<br>

```cpp
std::size_t size_capacity() const noexcept;
```

: Returns the capacity of the allocation. This means the largest size of the ciphertext that can be stored in the current allocation with the current encryption parameters.

<br>


```cpp
bool is_transparent() const;
```

: Check whether the current ciphertext is transparent, i.e. does not require a secret key to decrypt. In typical security models such transparent ciphertexts would not be considered to be valid. Starting from the second polynomial in the current ciphertext, this function returns true if all following coefficients are identically zero. Otherwise, returns false.

<br>


```cpp
MemoryPoolHandle pool() const noexcept;
```

: Returns the currently used MemoryPoolHandle.

<br>

```cpp
bool is_valid();
```
: Returns whether the ciphertext is valid.

<br>

```cpp
void compute_ckks_hardware_id() const;
```
: Compute the hardware id of this ciphertext.

<br>



### 6. Public key class : **<font color='red'><span id="PublicKey">PublicKey</span> </font>**  
**Description**: PublicKey is a class for storing public key information.

**Functions**:
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


**Description**:RelinKeys is a class for storing relinearized key information.


**Functions**:

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
const std::vector<std::vector<PublicKey>> &key() const;
```
: Returns a const reference to a Galois key. The returned Galois key corresponds to the given Galois element.

<br>






### 8. **<font color='red'><span id="GaloisKeys">GaloisKeys</span> </font>**:  Galois key class 

**Description**:GaloisKeys is a class for storing Galoiskeys information.

**Functions**: 

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

: Returns whether a Galois key corresponding to a given Galois element exists.

<br>

```cpp
const std::vector<std::vector<PublicKey>> &key(std::uint32_t galois_elt) const;
```
: Returns a const reference to a Galois key. The returned Galois key corresponds to the given Galois element.


<br>



### 9. **<font color='red'><span id="CKKSEncoder">CKKSEncoder</span> </font>** : CKKS encryption scheme  class  

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


### 10. Key generation class : **<font color='red'><span id="KeyGenerator">KeyGenerator</span> </font>**

**Description**: KeyGenerator is a class for generating keys.

**Functions**:
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





### 11. Encryption class : **<font color='red'><span id="Encryptor">Encryptor</span> </font>**

**Description**: Encryptor is a class used to encrypt plaintext.

**Functions**:

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
void set_public_key(const PublicKey &public_key);
```
- **public_key** (const PublicKey &):The public key.

: Give a new instance of public key.

<br>

```cpp
void set_secret_key(const SecretKey &secret_key);
```
- **secret_key** (const SecretKey &): The secret key.

: Give a new instance of secret key.


<br>


```cpp
void encrypt(const Plaintext &plain, Ciphertext &destination,MemoryPoolHandle pool = MemoryManager::GetPool())const;
```
- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** ( Ciphertext &): The ciphertext to overwrite with the encrypt.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a plaintext with the public key and stores the result in destination.

<br>


```cpp
void encrypt_zero(Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a zero plaintext with the public key and stores the result in destination.

<br>

```cpp
void encrypt_zero(parms_id_type parms_id, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```
- **parms_id** (parms_id_type): The parms_id for the resulting ciphertext.
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a zero plaintext with the public key and stores the result in destination.

<br>


```cpp
void encrypt_symmetric(const Plaintext &plain, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```
- **plain** (const Plaintext &): The plaintext to encrypt.
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a plaintext with the secret key and stores the result in destination.

<br>


```cpp
void encrypt_zero_symmetric(parms_id_type parms_id, Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```
- **parms_id** (parms_id_type): The parms_id for the resulting ciphertext
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a zero plaintext with the secret key and stores the result in destination.

<br>

```cpp
void encrypt_zero_symmetric(Ciphertext &destination, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
```
- **destination** (Ciphertext &): The ciphertext to overwrite with the encrypted plaintext.
- **pool** (MemoryPoolHandle): The MemoryPoolHandle pointing to a valid memory pool.

: Encrypts a zero plaintext with the secret key and stores the result in destination.

<br>


### 12. Decryption class : **<font color='red'><span id="Decryptor">Decryptor</span> </font>**

**Description**: Decryptor is a class for decrypting plaintext.

**Functions**:
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



### 13. Plaintext matrix class : **<font color='red'><span id="MatrixPlain">MatrixPlain</span> </font>**


**Description**: MatrixPlain is a class for storing plaintext matrix information.
 
**Members**:
- **LogSlots** (uint32_t): Indicates the logarithm to 2 of the number of matrix elements.

- **N1** (uint32_t): Indicates the number of rows in a matrix.

- **level** (uint32_t): Indicates the level of the ciphertext module chain in which the matrix resides.

- **scale** (double): Indicates the scaling factor of the matrix.

- **rot\_index** (vector<int>):Indicates the rotation index of a matrix element in a polynomial.

- **plain\_vec** (map<int,Plaintext>):Indicates the polynomial corresponding to the matrix elements.


<br>



### 14. Group of Plaintext matrix in linear transform : **<font color='red'><span id="LinearMatrixGroup">LinearMatrixGroup</span> </font>**

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


### 15. Homomorphic DFT Parameter class  : **<font color='red'><span id="HomomorphicDFTMatrixLiteral">HomomorphicDFTMatrixLiteral</span> </font>**

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



### 16.  Polynomial  class : **<font color='red'><span id="Polynomial">Polynomial</span> </font>**

**Description**:The polynomial coefficients class in homomorphic poly evaluator.

**Functions**: 
```cpp
Polynomial();
```
: C empty polynomial,and the type is Monomial.

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



### 17.  PolynomialVector  class : **<font color='red'><span id="PolynomialVector">PolynomialVector</span> </font>**

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


### 18. Homomorphic Mod polynomial class : **<font color='red'><span id="EvalModPoly">EvalModPoly</span> </font>**

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








### 19.  Evaluator Factory  class : **<font color='red'><span id="EvaluatorFactory">EvaluatorFactory</span> </font>**

**Description**: EvaluatorFactory is a class used to create the Poseidon algorithm library.

**Functions**: 
```cpp
auto create(const PoseidonContext &context)->std::shared_ptr< Envaluator >;
```
: A function used to create an Envaluator object based on the given PoseidonContext object can select the adopted full-homomorphic encryption algorithms such as BFV, CKKS, and their corresponding hardware or software libraries. The specific algorithm support is detailed in the next section.  







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


### 4. Multiplication between ciphertexts : **<font color='red'> multiply</font>**

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


### 5. Multiplication of ciphertext and plaintext : **<font color='red'> multiply_plain</font>**

```c
void multiply_plain(Ciphertext &ciph, Plaintext &plain,Ciphertext &result) override;
```

**Description**: This function performs homomorphic multiplication between a ciphertext and a plaintext.<br>

**Parameters**:
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>


<br>


### 6. Rescale : **<font color='red'> rescale</font>**

```c
void rescale (const Ciphertext &ciph,Ciphertext &result) const;
```

**Description**: This function performs a rescaling operation on a ciphertext.<br>

**Parameters**:
- **ciph** (Ciphertext):   A reference to a **Ciphertext** object, representing a ciphertext.<br>


<br>


### 7. Ciphertext rotation : **<font color='red'> rotate</font>**

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


### 8. Take conjugate : **<font color='red'> conjugate</font>**

```c
void conjugate(const Ciphertext &ciph, const GaloisKeys &conj_keys, Ciphertext &result) const;
```

**Description**: This function  is used to conjugate a ciphertext, that is, to transform each of its elements into its complex conjugate.<br>

**Parameters**:
- **ciph** (Ciphertext):    A reference to a **Ciphertext** object, representing the ciphertext to be conjugated.<br>
- **conj_keys** (GaloisKeys): A constant reference to a **GaloisKeys** object, representing the encryption key used for conjugation.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the resulting ciphertext after the conjugation.<br>

<br>



### 9. Matrix multiplication of ciphertext and plaintext : **<font color='red'> multiplyByDiagMatrixBSGS</font>**

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


### 10. Coefficient to plaintext slot : **<font color='red'> coeff_to_slot</font>**

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



### 11. Plaintext slot to coefficient : **<font color='red'> slot_to_coeff</font>**

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



### 12. Fast Fourier Transform (forward) : **<font color='red'> ftt_fwd</font>** 

```c
void ftt_fwd(Plaintext &plain ,Plaintext &result)
void ftt_fwd(Ciphertext &ciph, Ciphertext &result) override;
```

**Description**: This function is used for the Fast Fourier Transform (FFT) of a plaintext or ciphertext.

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

**Description**: This function is used for the Inverse Fast Fourier Transform (IFFT) of a ciphertext or plaintext.

**Parameters**: 
- **plain** (Plaintext): A reference to a **Plaintext** object, representing a plaintext.
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext or Plaintext): A reference to either a **Plaintext** or **Ciphertext** object, used to store the inverse transformed ciphertext or plaintext.


<br>



### 14. Read ciphertext information from accelerator card : **<font color='red'> read</font>**

```c
void read(Ciphertext &ciph) override;
```

**Description**: This function is used to read data from the acceleration card in the ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing the ciphertext to be read.



<br>

### 15. Multiplication of ciphertext and complex constant : **<font color='red'> multiply_const</font>**

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


### 16. Addition of ciphertext and complex constant : **<font color='red'> add_const</font>**

```cpp
void add_const(const Ciphertext &ciph, double constData, Ciphertext &result,const CKKSEncoder &encoder) const;
```

**Description**: This function is used for the homomorphic addition of a ciphertext and a real constant.

**Parameters**: 
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **constData** (double): A value of type double, representing a real constant.
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.



<br>

### 17. Discrete Fourier Transform on ciphertext : **<font color='red'> dft</font>**

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


### 18. Dynamic rescale : **<font color='red'> rescale_dynamic</font>**

```c
void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result, double min_scale);
```

**Description**: This function is used for dynamic rescaling of a ciphertext.

**Parameters**: 
- **ciph** (Ciphertext): A const reference to a **Ciphertext** object, representing a ciphertext.
- **result** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.
- **scale** (double):  Representing the given scaling factor.

<br>


### 19. Polynomial evaluation : **<font color='red'> evaluatePolyVector</font>**

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


### 20. Evaluate modulo on ciphertext vector : **<font color='red'> eval_mod</font>**

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


### 21. Bootstrap : **<font color='red'> bootstrap</font>**

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
### 22. Key switch : **<font color='red'> switch_key</font>**

```c
void switch_key(Ciphertext &ciph, Ciphertext &result, const vector<PublicKey> &switch_key);
```

**Description**: This function switches the key of a ciphertext.<br>

**Parameters**:
- **ciph** (Ciphertext): A reference to a **Ciphertext** object, representing a ciphertext.<br>
- **result** (Ciphertext): A reference to a **Ciphertext** object, used to store the computation result.<br>
- **switch_key** (PublicKey):  A constant reference to a vector of **PublicKey** objects, representing a given set of public keys. -->






