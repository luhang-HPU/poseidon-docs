# Operations for Hardware Only



### 1. Read ciphertext information from accelerator card : **<font color='red'> read</font>**

```c
void read(Ciphertext &ciph);
```

- **ciph** (Ciphertext): representing the ciphertext to be read.

Usage: `read` function is used to read data from the acceleration card in the ciphertext. For example, after the user executes hardware evaluation function, the latest result value is still stored in HPU card.  The user need to execute the `read` function to read the latest result value from the HPU card to the RAM.

