//
// Created by lxs on 12/11/23.
//
#ifndef KEY_SWITCH_BV
#define KEY_SWITCH_BV
#include "poseidon/key/kswitchkeys.h"
#include "poseidon/keyswitch/keyswitch_base.h"
#include "poseidon/seal/util/rlwe.h"
namespace poseidon{


    class KSwitchGenBV : public KSwitchGenBase
    {
    public:
        explicit KSwitchGenBV(const PoseidonContext &context)
         : KSwitchGenBase(context) {
            auto key_context_data = context_.crt_context()->key_context_data();
            auto len_modulus_p = key_context_data->parms().P().size();
            if (len_modulus_p != 1) {
                throw invalid_argument("KSwitchGenBV :size of modulus P is not 1!");
            }
        }

    protected:
        void generate_one_kswitch_key(const SecretKey &prev_secret_key,ConstRNSIter new_key, vector<PublicKey> &destination) const override;
    };



    class KSwitchBV : public KSwitchBase {
    public:
        explicit KSwitchBV(const PoseidonContext &context)
            :KSwitchBase(context)
        {}

        void relinearize_internal(Ciphertext &encrypted, const RelinKeys &relin_keys,size_t destination_size, MemoryPoolHandle pool) const override;
        void apply_galois_inplace(Ciphertext &encrypted, uint32_t galois_elt,const GaloisKeys &galois_keys, MemoryPoolHandle pool) const override;


    private:
        void switch_key_inplace(
                Ciphertext &encrypted, util::ConstRNSIter target_iter, const KSwitchKeys &kswitch_keys,
                std::size_t key_index, MemoryPoolHandle pool = MemoryManager::GetPool()) const;

        void switch_key_inplace(
                Ciphertext &encrypted,  RNSPoly &poly, const KSwitchKeys &kswitch_keys,
                std::size_t key_index, MemoryPoolHandle pool = MemoryManager::GetPool()) const;






    };

}

#endif