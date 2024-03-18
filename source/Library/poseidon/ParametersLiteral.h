//
// Created by lxs on 5/19/23.
//

#ifndef POSEIDON_PARAMETERSLITERAL_H
#define POSEIDON_PARAMETERSLITERAL_H
#include <iostream>
#include <vector>
#include "poseidon/defines.h"
#include "poseidon/pke_params_defines.h"
#include "poseidon/seal/memorymanager.h"
#include "poseidon/seal/modulus.h"
#include "poseidon/seal/util/hash.h"
using namespace std;

namespace poseidon {

    using parms_id_type = util::HashFunction::hash_block_type;
    const parms_id_type parms_id_zero = util::HashFunction::hash_zero_block;


    class ParametersLiteral {
    public:

        inline ParametersLiteral() = default;
        ParametersLiteral(SchemeType Type, uint32_t LogN, uint32_t LogSlots,
                          uint32_t LogScale, uint32_t HammingWeight, uint32_t q0_level, Modulus plain_modulus, const vector<Modulus> &Q, const vector<Modulus> &P,
                          MemoryPoolHandle pool = MemoryManager::GetPool());
        void set_log_modulus(const vector<uint32_t> &logQ,const vector<uint32_t> &logP);
        inline void set_modulus(const vector<Modulus> &mod_chain_q,const vector<Modulus> &mod_chain_p){
            this->Q_ = mod_chain_q;
            this->P_ = mod_chain_p;
            compute_params_id();
        }

        void compute_params_id();
        POSEIDON_NODISCARD inline uint32_t degree() const
        {
            return 1 << LogN_;
        }

        POSEIDON_NODISCARD inline uint32_t slot() const
        {
            return 1 << LogSlots_;
        }

        POSEIDON_NODISCARD inline const parms_id_type& parms_id() const
        {
            return params_id_;
        }

        POSEIDON_NODISCARD inline const SchemeType &scheme() const
        {
            return Type_;
        }

        POSEIDON_NODISCARD inline uint32_t LogN() const
        {
            return LogN_;
        }

        POSEIDON_NODISCARD inline uint32_t LogSlots() const
        {
            return LogSlots_;
        }
        POSEIDON_NODISCARD inline uint32_t HammingWeight() const
        {
            return HammingWeight_;
        }

        POSEIDON_NODISCARD inline uint32_t q0_level() const
        {
            return q0_level_;
        }

        POSEIDON_NODISCARD inline const Modulus &plain_modulus() const
        {
            return plain_modulus_;
        }

        POSEIDON_NODISCARD inline const vector<Modulus> &Q() const
        {
            return Q_;
        }

        POSEIDON_NODISCARD inline const vector<Modulus> &P() const
        {
            return P_;
        }

        POSEIDON_NODISCARD inline  uint32_t LogScale() const
        {
            return LogScale_;
        }

        POSEIDON_NODISCARD inline  double scale() const
        {
            return pow(2.0,LogScale_);
        }

    protected:
        SchemeType Type_;
        uint32_t LogN_ = 0;
        uint32_t LogSlots_ = 0;
        uint32_t LogScale_ = 0;
        uint32_t HammingWeight_ = 0;
        uint32_t q0_level_ = 0;   //merge primes as q0
        Modulus plain_modulus_ = 0;
        vector<Modulus> Q_{};
        vector<Modulus> P_{};
        MemoryPoolHandle pool_;
        parms_id_type params_id_ = parms_id_zero;
    };



    class ParametersLiteralDefault : public ParametersLiteral{
        private:
            void init(SchemeType schemeType,uint32_t degree,sec_level_type sec_level);
        public:
        ParametersLiteralDefault(SchemeType schemeType,uint32_t degree,sec_level_type sec_level, MemoryPoolHandle pool = MemoryManager::GetPool());
    };
/*
    class BFVParametersLiteralDefault : public ParametersLiteral{
    private:
        int init(DegreeType degreeType);
    public:
        BFVParametersLiteralDefault(DegreeType degreeType);
    };

    class BGVParametersLiteralDefault : public ParametersLiteral{
    private:
        int init(DegreeType degreeType);
    public:
        BGVParametersLiteralDefault(DegreeType degreeType);
    };

*/

}

//
//
namespace std{
    template <>
    struct hash<poseidon::parms_id_type>{
        std::size_t operator()(const poseidon::parms_id_type &params_id) const{
            std::uint64_t result = 17;
            result = 31 * result + params_id[0];
            result = 31 * result + params_id[1];
            result = 31 * result + params_id[2];
            result = 31 * result + params_id[3];
            return static_cast<std::size_t>(result);
        }
    };


//    template <>
//    struct hash<poseidon::ParametersLiteral>{
//        std::size_t operator()(const poseidon::ParametersLiteral &params) const{
//            hash<poseidon::parms_id_type> param_id_hash;
//            return parms_id_hash(params.
//        }
    };
//
//}

#endif //POSEIDON_PARAMETERSLITERAL_H




