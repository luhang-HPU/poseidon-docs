//
// Created by lxs on 12/11/23.
//
#ifndef KEY_SWITCH_GHS
#define KEY_SWITCH_GHS
#include "poseidon/key/kswitchkeys.h"
#include "poseidon/keyswitch/keyswitch_base.h"
#include "poseidon/seal/util/rlwe.h"

namespace poseidon{


    class KSwitchGenGHS : public KSwitchGenBase
    {
    public:
        explicit KSwitchGenGHS(const PoseidonContext &context)
                : KSwitchGenBase(context)
        {
            auto key_context_data = context_.crt_context()->key_context_data();
            auto len_modulus_p = key_context_data->parms().P().size();
            auto len_modulus_q = key_context_data->parms().Q().size();

            if(len_modulus_p < len_modulus_q){
                throw invalid_argument("KSwitchGenGHS :size of modulus P is less than len_modulus_q!");
            }
        }


    protected:
        void generate_one_kswitch_key(const SecretKey &prev_secret_key,ConstRNSIter new_key, vector<PublicKey> &destination) const override;
    };

    class KSwitchGHS : public KSwitchBase
    {
    public:
        explicit KSwitchGHS(const PoseidonContext &context)
                :KSwitchBase(context)
        {}
        void apply_galois_inplace(
                Ciphertext &encrypted, uint32_t galois_elt,
                const GaloisKeys &galois_keys, MemoryPoolHandle pool) const override;
        void relinearize_internal(
                Ciphertext &encrypted, const RelinKeys &relin_keys,
                size_t destination_size, MemoryPoolHandle pool) const override;


    private:
        void switch_key_inplace(
                Ciphertext &encrypted,  RNSPoly &poly, const KSwitchKeys &kswitch_keys,
                std::size_t key_index, MemoryPoolHandle pool = MemoryManager::GetPool()) const;


    };
}

#endif