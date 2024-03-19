//
// Created by lxs on 12/14/23.
//

#ifndef POSEIDON_RNSTOOL_QP_H
#define POSEIDON_RNSTOOL_QP_H
#include <unordered_map>
#include "poseidon/seal/util/rns.h"
#include "poseidon/defines.h"
#include "poseidon/seal/util/polycore.h"
#include "poseidon/seal/util/polyarithsmallmod.h"
using namespace std;
namespace poseidon{
    namespace util{

        class RNSTool_QP {
            friend RNSIter;
        public:
            RNSTool_QP(std::size_t poly_modulus_degree, const RNSBase &coeff_modulus_q, const RNSBase &coeff_modulus_p,const Modulus &plain_modulus,
                       MemoryPoolHandle pool);

            RNSTool_QP(const RNSTool_QP &copy) = delete;

            RNSTool_QP(RNSTool_QP &&source) = delete;

            RNSTool_QP &operator=(const RNSTool_QP &assign) = delete;

            RNSTool_QP &operator=(RNSTool_QP &&assign) = delete;

            void mod_up(ConstRNSIter input, RNSIter destination, MemoryPoolHandle pool) const;
            void mod_up_copy_q(ConstRNSIter input,size_t base_idx,RNSIter destination, MemoryPoolHandle pool) const;
            void mod_up_base_q(RNSIter input, size_t base_idx, MemoryPoolHandle pool) const; //for hybrid,modup to other base_q rns
            void mod_up_base_p(ConstRNSIter input, size_t base_idx,RNSIter destination, MemoryPoolHandle pool) const; //for hybrid,modup to base_p rns
            void mod_up_from_one_base_q(RNSIter input, size_t base_idx, MemoryPoolHandle pool) const; //for hybrid
            void mod_up_from_one_base_p(ConstRNSIter input, size_t base_idx,RNSIter destination, MemoryPoolHandle pool) const; //for hybrid


            void mod_down(ConstRNSIter input_q,ConstRNSIter input_p, RNSIter destination, MemoryPoolHandle pool) const;
            POSEIDON_NODISCARD inline auto p_mod_qi() const noexcept
            {
                return prod_P_mod_qi_.get();
            }

            POSEIDON_NODISCARD inline auto base_q() const noexcept
            {
                return base_q_.get();
            }

            POSEIDON_NODISCARD inline auto base_p() const noexcept
            {
                return base_p_.get();
            }

            POSEIDON_NODISCARD inline auto base_qp() const noexcept
            {
                return base_qp_.get();
            }


        private:
            class RNSTool_Inter {
            public:
                RNSTool_Inter(const RNSBase &q,const RNSBase &p,size_t decomp_idx,MemoryPoolHandle pool = MemoryManager::GetPool())
                 :pool_(std::move(pool))
                {
                    initialize(q,p,decomp_idx);
                }

                POSEIDON_NODISCARD inline size_t base_start_idx() const{
                    return base_start_idx_;
                }
                POSEIDON_NODISCARD inline size_t base_last_idx() const{
                    return base_last_idx_;
                }
                POSEIDON_NODISCARD inline size_t base_end_idx() const{
                    return base_end_idx_;
                }
                POSEIDON_NODISCARD inline const unordered_map<size_t ,const Pointer<BaseConverter>>  &single_obase_q_conv_map() const{
                    return single_obase_q_conv_map_;
                }

                POSEIDON_NODISCARD inline const Pointer<BaseConverter> &obase_p_conv() const{
                    return obase_p_conv_;
                }

            private:

                void initialize(const RNSBase &q,const RNSBase &p,size_t decomp_idx);
                size_t base_start_idx_ = 0;
                size_t base_last_idx_ = 0;
                size_t base_end_idx_ = 0;
                MemoryPoolHandle pool_;
                Pointer<RNSBase> base_qi_; // for hybrid
                unordered_map<size_t ,const Pointer<BaseConverter>> single_obase_q_conv_map_;
                Pointer<BaseConverter> obase_p_conv_;
            };



            std::size_t coeff_count_ = 0;

            MemoryPoolHandle pool_;

            Pointer<RNSBase> base_q_;

//            vector<Pointer<RNSBase>> base_qi_; // for hybrid
            vector<RNSTool_Inter> rnstool_decomp_;

            Pointer<RNSBase> base_p_;

            Pointer<RNSBase> base_qp_;

            // Base converter: q --> p
            Pointer<BaseConverter> base_q_to_p_conv_;
            // Base converter: p --> q
            Pointer<BaseConverter> base_p_to_q_conv_;

            // Base converter: qp --> q
            Pointer<BaseConverter> base_qp_to_q_conv_;

            // prod(p) mod qi
            Pointer<MultiplyUIntModOperand> prod_P_mod_qi_;
            // prod(p)^-1 mod qi
            Pointer<MultiplyUIntModOperand> prod_p_inv_mod_qi_;

            /**
            Generates the pre-computations for the given parameters.
            */
            void initialize(std::size_t poly_modulus_degree, const RNSBase &q, const RNSBase &p,const Modulus &t);
        };
    }
}


#endif //POSEIDON_RNSTOOL_QP_H