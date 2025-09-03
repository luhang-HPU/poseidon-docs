
# Poseidon Introduction

Poseidon is a **homomorphic computation library** specifically designed for **Homomorphic Computing Unit (HPU)**, which supports three widely-used **fully homomorphic encryption (FHE)** algorithms: *BFV*, *BGV* and *CKKS*. 

Poseidon library consists of basic data structures and advanced homomorphic evaluation. In *BFV* and *BGV*, all kinds of homomorphic computations like addition, multiplication, relinearization, rotation is supported for integer messages. In *CKKS*,  homomorphic computations like addition, multiplication, relinearization, rotation, conjugation and even bootstrap is supported for float messages and complex messages.

Moreover, Poseidon enables users to build up more flexible functionality and programmability in a wide range of business scenarios with the low-level homomorphic computation primitives. On the one hand, users could integrate Poseidon with upper-level privacy computing framework, *Secretflow* or *Primihub*, executing *Private Set Intersection (PSI)*, *Private Information Retrieval (PIR)* and more complicated encrypted inference of *DNN* and *LLM* in the end-to-end way. On the other hand, all the FHE-relate parameters are configurable in Poseidon, users could build their own application as they wish with default parameter or self-customized parameter.

See section *Getting Started* for details in installation and programming.