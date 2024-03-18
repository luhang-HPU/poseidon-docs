//
// Created by cloudam on 12/19/23.
//

#ifndef POSEIDON_CKKSEVALUATOR_S_H
#define POSEIDON_CKKSEVALUATOR_S_H
#include "poseidon/scheme/ckks/CKKSEvaluatorBase.h"
#include "poseidon/scheme/SoftwareBase.h"
namespace poseidon{
    class CKKSEvaluator_S : public CKKSEvaluatorBase, public SoftwareBase
    {
    public:
        inline explicit CKKSEvaluator_S(const PoseidonContext &context)
        : CKKSEvaluatorBase(context)
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

        void read(Ciphertext &ciph) const override;
        void read(Plaintext &plain) const override;
        void ftt_fwd(const Plaintext &plain, Plaintext &result) const override;
        void ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const override;
        void ftt_inv(const Plaintext &plain, Plaintext &result) const override;
        void ftt_inv(const Ciphertext &ciph, Ciphertext &result) const override;
        void add_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const override;
        void multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const override;
        void add(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
        void sub(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
        void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
        void square_inplace( Ciphertext &ciph) const override;

        void relinearize(const Ciphertext &ciph, const RelinKeys &relin_keys, Ciphertext &result) const override;
        void rotate(const Ciphertext &ciph, int step, const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void conjugate(const Ciphertext &ciph, const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void rescale(const Ciphertext &ciph, Ciphertext &result) const override;
        void rescale_dynamic(const Ciphertext &ciph, Ciphertext &result, double min_scale) const override;
        void drop_modulus(const Ciphertext &ciph, Ciphertext &result, parms_id_type parms_id) const override;
        void raise_modulus(const Ciphertext &ciph, Ciphertext &result) const override;
        void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                    const RelinKeys &relin_key) const override;
        void multiply_relin_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                    const RelinKeys &relin_key) const override;

        void sub_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const override;
        void add_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const override;
    private:
        std::shared_ptr<KSwitchBase> kswitch_{nullptr};

        void add_plain_inplace(Ciphertext &ciph, const Plaintext &plain) const;
        void add_inplace(Ciphertext &ciph1, const Ciphertext &ciph2) const;
        void rescale_inplace(const Ciphertext &ciph, Ciphertext &result, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
        void ckks_multiply(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool) const;
        void multiply_inplace(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool = MemoryManager::GetPool()) const;
        void multiply_plain_inplace( Ciphertext &ciph, const Plaintext &plain) const;
    };
}



#endif //POSEIDON_CKKSEVALUATOR_S_H
