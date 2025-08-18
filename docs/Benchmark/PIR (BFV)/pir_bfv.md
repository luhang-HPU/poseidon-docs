# Introduction

<div style="text-align: justify">


Private Information Retrieval (PIR), as an extension of Information Retrieval, ensures users submit query requests to the data source without being perceived or leaked. PIR protocol is supposed to meet two demands at the same time:

1. Correctness: Users get correct data as they request.
2. Security: The server does not know the true query conditions.

![pir](../../Image/Benchmark/PIR (BFV)/1.png)

<style>
    img[alt="pir"]{
        width:700px;
    }
</style> 

<br>
<br>



 

## Basic PIR

Suppose the amount of the data from the client is `n`. The basic procedure is as follows.

1. The client generates the secret key and the public key `(sk, pk)` of BFV algorithm.
2. The client generates an n-dimension vector `(0, ..., 0, 1, 0, ..., 0)`, which sets `1` at the position of the index inquired and `0` at the other.
3. The client encrypts the inquiry vector with the public key and sends the ciphertext $(E(0), ..., E(0), E(1), E(0), ..., E(0))$ to the server.
4. The server receives the encrypted inquiry vector, multiplied with an n-demension vector constructed by the local data $(B_1, B_2, ..., B_n)$, gets $E(0 \cdot B_1 + ... + 0 \cdot B_{i-1} + 1 \cdot B_i + 0 \cdot B_{i+1} + ... + 0 \cdot B_n)$ and sends it to the client.
5. The client decrypts the ciphertext and gets the inquired data $B_i$.


![basic](../../Image/Benchmark/PIR (BFV)/5.png)

<style>
    img[alt="basic"]{
        width:700px;
    }
</style>


The basic PIR protocol can be extracted into four functions: (Setup, Query, Answer, Extract).


![function](../../Image/Benchmark/PIR (BFV)/6.png)

<style>
    img[alt="function"]{
        width:700px;
    }
</style>


## Data Packing

Consider the basic PIR protocol, which consumes a lot on the computation and the communication of the query requests since $n$ ciphertexts should be generated and sent each time.

Thus, we pack multiple data into one homomorphic encryption plaintext.


![packing](../../Image/Benchmark/PIR (BFV)/2.png)

<style>
    img[alt="packing"]{
        width:700px;
    }
</style>



## Vector Transformer

We convert data from the linear vector into a 2-dimension matrix.

When we use a 2-dimension inquiry, most query requests can be supported within the two inquiry ciphertexts.

![convert](../../Image/Benchmark/PIR (BFV)/3.png)

<style>
    img[alt="convert"]{
        width:700px;
    }
</style>

<br>

## Multiple Inquiries

We construct probabilistic batch code based on Cuckoo Hash, to divide the database into several batches, and execute multiple inquiries at the same time according to different batches.

![Multi-query](../../Image/Benchmark/PIR (BFV)/4.png)

<style>
    img[alt="Multi-query"]{
        width:700px;
    }
</style>





## Code

**Typedef**:

```cpp
typedef std::vector<Plaintext> Database;
typedef std::vector<std::vector<Ciphertext>> PirQuery;
typedef std::vector<Ciphertext> PirReply;
```



**Member function (Class Client)** : 

```cpp
PirQuery generate_query(std::uint64_t desiredIndex);
```

* `desiredIndex` (std::uint64_t): the index of element desired to be queried.

* `return value` (PirQuery): the query matrix of encrypted index.

**Usage**: generate_query` generates the query of element index.



```cpp
int generate_serialized_query(std::uint64_t desiredIndex, std::stringstream &stream);
```

`desiredIndex` (std::uint64_t): the index of element desired to be queried.

`stream` (std::stringstream&): the output string.

`return value` (int): the number of bytes written into the stringstream.

**Usage**: `generate_serialized_query` generates the query and serializeds it into the provided stream.



```cpp
Plaintext decode_reply(PirReply &reply);
```

`reply` (PirReply): the reply received from the server.

`return value` (Plaintext): the plaintext of queried result.

**Usage**: `decode_reply` decode the reply data into plaintext.



**Member funtion (Class Server)** :

```cpp
void set_galois_key(std::uint32_t client_id, GaloisKeys galkey);
```

* `client_id` (std::uint32_t): used to distinguish between clients if there is more than one client.
* `galkey` (GaloisKeys): the galois key received from the client.

**Usage**: `set_galois_key` sets the galois key for server.



```cpp
std::vector<Ciphertext> expand_query(const Ciphertext &encrypted, std::uint32_t m,
                                         std::uint32_t client_id);
```

* `encrypted` (const Ciphertext): the query ciphertext from the client.
* `m` (std::uint32_t): size of the matrix dimension.
* `client` (std::uint32_t): used to distinguish between clients if there is more than one client.
* `return value` (std::vector\<Ciphertext\>): the expanded ciphertexts.

**Usage**: `expand_query` expand the query into ciphertext vector.



```cpp
PirQuery deserialize_query(std::stringstream &stream);
```

* `stream` (std::stringstream): the serialized stream received from the client.
* `return value` (PirQuery): the query ciphertext deserialized from the stream.

**Usage**: `deserialize_query` deserialize the stream into query of ciphertexts.



```cpp
PirReply generate_reply(PirQuery &query, std::uint32_t client_id);
```

* `query` (PirQuery): the ciphertext query from client.
* `client_id` (std::uint32_t): used to distinguish between clients if there is more than one client.
* `return value` (PirReply): the reply to be sent to client.

**Usage**: `generate_reply` multiplies the ciphertext query by the database, generating the reply ciphertext.



```cpp
int serialize_reply(PirReply &reply, std::stringstream &stream);
```

* `reply` (PirReply): the reply to be sent to client.
* `stream` (std::stringstream): the serialized stream of reply.
* `return value` (int): the number of bytes written.

**Usage**: `serialize_reply` serialize the reply into stream.



## Performance (Updating)

### System Environment

* System: Ubuntu 20.04.6 LTS
* CPU: Intel(R) Xeon(R) Platinum 8160 CPU @ 2.10GHz
* RAM: 128G
* HPU: Zhangjiang-1



### Comparison

We compare the performance of PoseidonPIR with [SealPIR](https://github.com/microsoft/SealPIR?tab=readme-ov-file) that is based on the paper [PIR with compressed queries and amortized query processing](https://eprint.iacr.org/2017/1142.pdf) .



|                     | SealPIR | PoseidonPIR |
| ------------------- | ------- | ----------- |
| Pre-processing (ms) | 1033    | 201         |
| Query (ms)          | 7       | 7           |
| Reply (ms)          | 1385    | 4257        |
| Decode (ms)         | 23      | 38          |

