//
// Created by lxs on 24-2-20.
//

#include "poseidon/scheme/SoftwareBase.h"
namespace poseidon{
    void SoftwareBase::ftt_fwd_b(const Plaintext &plain ,Plaintext &result) const {
        if(!plain.is_valid()){
            throw invalid_argument("ftt_fwd : Plaintext is empty!");
        }
        result = plain;
        result.poly().coeff_to_dot();
    }
    void SoftwareBase::ftt_fwd_b(const Ciphertext &ciphertext, Ciphertext &result) const {
        if(!ciphertext.is_valid()){
            throw invalid_argument("ftt_fwd : Ciphertext is empty!");
        }
        result = ciphertext;
        for(auto &p : result.polys()){
            p.coeff_to_dot();
        }
    }
    void SoftwareBase::ftt_inv_b(const Plaintext &plain ,Plaintext &result) const{
        if(!plain.is_valid()){
            throw invalid_argument("ftt_inv : Plaintext is empty!");
        }
        result = plain;
        result.poly().dot_to_coeff();
    }
    void SoftwareBase::ftt_inv_b(const Ciphertext &ciphertext ,Ciphertext &result) const {
        if(!ciphertext.is_valid()){
            throw invalid_argument("ftt_inv : Ciphertext is empty!");
        }
        result = ciphertext;
        for(auto &p : result.polys()){
            p.dot_to_coeff();
        }
    }

}
