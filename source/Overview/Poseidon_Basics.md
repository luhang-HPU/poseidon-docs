
# Poseidon Basics

<div style="text-align: justify">

Poseidon is a Homomorphic Computing Library specifically designed for Homomorphic Computing Unit (HPU). It contains basic data structures and evaluation functions for Fully Homomorphic Encryption (FHE).

Poseidon supports three widely-used FHE algorithms: CKKS, BFV and BGV. CKKS relies on approximate computing that takes noise as part of its precision loss. The decrypted plaintext might be different from the original plaintext for encryption. BFV and BGV, apart from CKKS, have exactly identical decrypted plaintext, but it only applies for the encryption of integer values. 

Poseidon completely supports CKKS, including simple evaluations like ciphertext-ciphertext/plaintext addition/multiplication, as well as the complex operations like rotation, conjungate, keyswitch, taylor/Chebyshev series expansion and even bootstrap. It enables more flexible functionality and programmability in a wide range of business scenarios. For example, you could integrate Poseidon with upper-level privacy computing framework, i.e. secretflow or primihub, executing PSI, PIR and more complicated encrypted inference of DNNs and LLMs, in an end-to-end manner. Poseidon does not support bootstrap in BFV and BGV for now. However, for the applications that only require limited leveled multiplications, the functionalities of BFV and BGV provided in Poseidon are sufficient enough. 

All the FHE-related parameters are configurable in Poseidon. You can use pre-set parameters by default or customized parameters as you wish. Poseidon provides easy parameter setting for each FHE algorithm, implemented by the parameter setting class and API (see Section "Getting Started").

<div>