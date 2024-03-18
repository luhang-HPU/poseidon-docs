//
// Created by lxs on 24-2-20.
//

#ifndef POSEIDON_SOFTWAREBASE_H
#define POSEIDON_SOFTWAREBASE_H
#include "poseidon/ciphertext.h"
#include "poseidon/plaintext.h"
namespace poseidon{
    class SoftwareBase {
    protected:
        void ftt_fwd_b(const Plaintext &plain, Plaintext &result) const;
        void ftt_fwd_b(const Ciphertext &ciph, Ciphertext &result) const;
        void ftt_inv_b(const Plaintext &plain, Plaintext &result) const;
        void ftt_inv_b(const Ciphertext &ciph, Ciphertext &result) const;


    };
}



#endif //POSEIDON_SOFTWAREBASE_H
