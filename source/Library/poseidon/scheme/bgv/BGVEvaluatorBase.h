//
// Created by lxs on 24-2-20.
//

#ifndef POSEIDON_BGVEVALUATORBASE_H
#define POSEIDON_BGVEVALUATORBASE_H
#include "poseidon/ciphertext.h"
#include "poseidon/plaintext.h"
#include "poseidon/key/relinkeys.h"
#include "poseidon/key/galoiskeys.h"
#include "poseidon/keyswitch/keyswitch.h"
namespace poseidon{
    class BGVEvaluatorBase {
    public:
        void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
        void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;

    public:
        virtual void ftt_fwd(const Plaintext &plain ,Plaintext &result ) const = 0;
        virtual void ftt_fwd(const Ciphertext &ciph ,Ciphertext &result) const = 0;
        virtual void ftt_inv(const Plaintext &plain ,Plaintext &result ) const = 0;
        virtual void ftt_inv(const Ciphertext &ciph ,Ciphertext &result) const = 0;
        virtual void add(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void sub(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void sub_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const = 0;
        virtual void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const = 0;
        virtual void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const RelinKeys &relin_key) const = 0;
        virtual void rotate_row(const Ciphertext &ciph,int rot_step, const GaloisKeys &galois_keys, Ciphertext &result) const = 0;
        virtual void rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const = 0;
        virtual void drop_modulus(const Ciphertext &ciph, Ciphertext &result, parms_id_type parms_id) const = 0;

    protected:
        inline explicit BGVEvaluatorBase(const PoseidonContext &context,MemoryPoolHandle pool = MemoryManager::GetPool())
         : context_(context),pool_(std::move(pool))
        {
        }
        PoseidonContext context_;
        MemoryPoolHandle pool_;




    };
}



#endif //POSEIDON_BFVEVALUATORBASE_H
