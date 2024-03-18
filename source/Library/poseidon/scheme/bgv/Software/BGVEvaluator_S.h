//
// Created by lxs on 24-2-20.
//

#ifndef POSEIDON_BGVEvaluator_S_H
#define POSEIDON_BGVEvaluator_S_H
#include "poseidon/scheme/bgv/BGVEvaluatorBase.h"
#include "poseidon/scheme/SoftwareBase.h"
namespace poseidon{
    class BGVEvaluator_S : public BGVEvaluatorBase, public SoftwareBase{
    public:
        inline explicit BGVEvaluator_S(const PoseidonContext &context)
                : BGVEvaluatorBase(context)
        {
            if(context_.key_switch_variant() == BV){
                kswitch_ = make_shared<KSwitchBV>(context);
            }
            else if(context_.key_switch_variant() == GHS){
                kswitch_ = make_shared<KSwitchGHS>(context);

            }
            else if(context_.key_switch_variant() == HYBRID){
                kswitch_ = make_shared<KSwitchHybrid>(context);
            }
        }
        void ftt_fwd(const Plaintext &plain ,Plaintext &result) const override;
        void ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const override;
        void ftt_inv(const Plaintext &plain ,Plaintext &result) const override;
        void ftt_inv(const Ciphertext &ciph ,Ciphertext &result) const override;
        void add(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const override;
        void sub(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const override;
        void add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const override;
        void sub_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const override;
        void multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const override;
        void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
        void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const override;
        void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const RelinKeys &relin_key) const override;
        void rotate_row(const Ciphertext &ciph,int rot_step, const GaloisKeys &galois_keys, Ciphertext &result) const override;
        void rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const override;
        void drop_modulus(const Ciphertext &ciph, Ciphertext &result, parms_id_type parms_id) const override;
    private:
        std::shared_ptr<KSwitchBase> kswitch_{nullptr};

        void add_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const;
        void sub_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const;
        void add_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2) const;
        void bgv_multiply(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool) const;
        void multiply_inplace(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
        void multiply_plain_inplace( Ciphertext &ciph, const Plaintext &plain) const;
        void transform_to_ntt_inplace(Plaintext &plain, parms_id_type parms_id, MemoryPoolHandle pool) const;


    };
}



#endif //POSEIDON_BGVEvaluator_S_H
