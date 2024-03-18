//
// Created by lxs on 12/15/23.
//

#ifndef POSEIDON_RNSPOLY_H
#define POSEIDON_RNSPOLY_H
#include "poseidon/PoseidonContext.h"
#include "poseidon/seal/util/polycore.h"
#include "poseidon/seal/util/polyarithsmallmod.h"



namespace poseidon{
    enum PolyType{
        ternary, // 0 -1 1 for keys
        gaussian,//[0 , q] for a
        centered_binomial, //for noise
        nothing,
        zero

    };
    class RNSPoly {
    public:
        RNSPoly() = default;
        RNSPoly(const shared_ptr<CrtContext> &context_data,std::uint64_t *data,parms_id_type parms_id);
        RNSPoly(const PoseidonContext &context,std::uint64_t *data,parms_id_type parms_id,PolyType = nothing,const shared_ptr<UniformRandomGenerator>& prng = nullptr);
        RNSPoly(const PoseidonContext &context,parms_id_type parms_id,PolyType = nothing,const shared_ptr<UniformRandomGenerator>& prng = nullptr);
        RNSPoly(const PoseidonContext &context,parms_id_type parms_id,bool enable_rns_p);

        //RNSPoly(std::shared_ptr< CrtContext> crt_context,std::uint64_t *data,parms_id_type parms_id);
        //RNSPoly(std::shared_ptr< CrtContext> crt_context,parms_id_type parms_id);
        RNSPoly(const RNSPoly &copy) = default;
        RNSPoly(RNSPoly &&source) = default;
        RNSPoly &operator=(const RNSPoly &assign) = default;
        RNSPoly &operator=(RNSPoly &&assign) = default;

        void set_random(const PoseidonContext &context,PolyType randomType,const shared_ptr<UniformRandomGenerator>& prng = nullptr) const;
        void resize(const PoseidonContext &context, parms_id_type parms_id,bool enable_rns_p = false);


        POSEIDON_NODISCARD inline size_t rns_num_q() const{
            return rns_num_q_;
        }

        POSEIDON_NODISCARD inline auto const &get() const{
            return buffer_;
        }

        POSEIDON_NODISCARD inline size_t rns_num_p()const{
            return rns_num_p_;
        }

        POSEIDON_NODISCARD inline size_t rns_num_total()const{
            return rns_num_total_;
        }

        POSEIDON_NODISCARD inline size_t poly_degree() const{
            return poly_degree_;
        }

        POSEIDON_NODISCARD inline size_t rns_p_offset() const{
            return rns_p_offset_;
        }

//        POSEIDON_NODISCARD inline bool is_ntt_form()const{
//            return is_ntt_form_;
//        }
//
//        POSEIDON_NODISCARD inline bool &is_ntt_form(){
//            return is_ntt_form_;
//        }

        inline void set_parms_id(parms_id_type parms_id){
            parms_id_ = parms_id;
        }

        inline const parms_id_type &parms_id() const{
            return parms_id_;
        }

        inline  ConstPolyIter const_poly_iter() const{
            return const_poly_iter_;
        }

        inline PolyIter poly_iter(){
            return poly_iter_;
        }

        inline void set_hardware_id(uint32_t id) {
            this->hardware_id_ = id;
        }

        inline const uint32_t &hardware_id() {
            return this->hardware_id_ ;
        }

        inline const uint32_t hardware_id() const{
            return this->hardware_id_ ;
        }

        void compute_ckks_hardware_id() const;

        void dot_to_coeff() ;
        void coeff_to_dot() ;
        void dot_to_coeff_lazy() ;
        void coeff_to_dot_lazy() ;
        POSEIDON_NODISCARD  RNSIter operator[](std::size_t rns_index) const;
        POSEIDON_NODISCARD  const uint64_t *data() const;
        POSEIDON_NODISCARD uint64_t * data() ;

        void operator*=(const RNSPoly &poly);
        void operator*=(uint64_t scalar);
        void operator+=(const RNSPoly &poly);
        void operator+=(uint64_t scalar);
        void operator-=(const RNSPoly &poly);
        void negate();

        void copy(const RNSPoly &operand) const;
        void copy(const RNSPoly &operand,size_t rns_num) const;
        void copy(const RNSPoly &operand,size_t operand_rns_idx,size_t rns_num,size_t res_rns_idx) const;

        //for drop modulus
        void drop(size_t rns_count,size_t rns_num) const;
        void add(const RNSPoly &operand,RNSPoly &result) const;
        void sub(const RNSPoly &operand,RNSPoly &result) const;
        void multiply(const RNSPoly &operand,RNSPoly &result) const;
        void add_scalar(uint64_t scalar,RNSPoly &result) const;
        void multiply_scalar(uint64_t scalar,RNSPoly &result ) const;












    private:
        //friendly function only_used by base conv
        inline void set_rns_p(size_t num){
            rns_num_p_ = num;
        }
        std::shared_ptr< CrtContext> crt_context_{nullptr};
        std::shared_ptr< const CrtContext::ContextData> context_data{nullptr};
        std::shared_ptr< const CrtContext::ContextData> key_context_data{nullptr};

        size_t poly_degree_ = 0;
        size_t rns_num_q_ = 0;// level + 1
        size_t rns_num_p_ = 0;
        size_t rns_num_total_ = 0;
        size_t rns_p_offset_ = 0;
        //bool is_ntt_form_ = false;
        parms_id_type parms_id_ = parms_id_zero;
        uint64_t *data_ = nullptr;
        //Pointer<uint64_t> buffer_;
        DynArray<uint64_t> buffer_;
        PolyIter poly_iter_;
        ConstPolyIter const_poly_iter_;
        MemoryPoolHandle pool_ = MemoryManager::GetPool();
        mutable uint32_t hardware_id_ = 0;
    };


//    class RNSIt {
//    public:
//        RNSIt(std::uint64_t *data)
//                :data_(data)
//        {
//
//        }
//
//
//
//        void dot_to_coeff() const;
//        void coeff_to_dot() const;
//        inline size_t rns_num_q() const{
//            return rns_num_q_;
//        }
//
//        inline size_t rns_num_p()const{
//            return rns_num_p_;
//        }
//
//        inline size_t poly_degree() const{
//            return poly_degree_;
//        }
//
//        inline const uint64_t *data() const {
//            return data_;
//        }
//
//        inline size_t rns_p_offset() const{
//            return rns_p_offset_;
//        }
//
//        inline  parms_id_type parms_id() const {
//            return parms_id_;
//        }
//
//        inline bool is_ntt_form()const{
//            return is_ntt_form_;
//        }
//
//        inline bool &is_ntt_form(){
//            return is_ntt_form_;
//        }
//
//        inline void set_parms_id(parms_id_type parms_id){
//            parms_id_ = parms_id;
//        }
//
//
//
//        POSEIDON_NODISCARD  inline RNSIter operator[](std::size_t rns_index) const
//        {
//
//            if(rns_index <= rns_num_total_){
//                throw std::invalid_argument("rns_index is out of range");
//            }
//            return poly_iter_.data(rns_index);
//        }
//
//        void operator*=(const RNSPoly &poly)
//        {
//
//            if(!poly.is_ntt_form()){
//                throw std::invalid_argument("RNSPoly *= : right data is not ntt form!");
//            }
//            if(!this->is_ntt_form()){
//                throw std::invalid_argument("RNSPoly *= : lift data is not ntt form!");
//            }
//
//            if(this->parms_id_ != poly.parms_id()){
//                throw invalid_argument("RNSPoly *= : param_id not match!");
//            }
//
//            if(rns_num_p_ == poly.rns_num_p()){
//                throw invalid_argument("RNSPoly *= : rns_num_p not match!");
//            }
//
//            auto context_data = crt_context_->get_context_data(parms_id_);
//            auto key_context_data = crt_context_->key_context_data();
//            auto modulus_q = context_data->parms().Q();
//            auto modulus_p = key_context_data->parms().P();
//
//
//            for (int i = 0; i < rns_num_q_; ++i) {
//                dyadic_product_coeffmod(
//                        data_ + i * poly_degree_, poly.data_ + i * poly_degree_, poly_degree_,
//                        modulus_q[i], data_ + i * poly_degree_);
//            }
//            for(int i = rns_num_q_; i < rns_num_q_ + rns_num_p_; ++i){
//                dyadic_product_coeffmod(
//                        data_ + i * poly_degree_, poly.data_ + i * poly_degree_, poly_degree_,
//                        modulus_p[i-rns_num_q_], data_ + i * poly_degree_);
//            }
//        }
//
//
//
//
//    private:
//        //friendly function only_used by base conv
//        inline void set_rns_p(size_t num){
//            rns_num_p_ = num;
//        }
//        std::shared_ptr< CrtContext> crt_context_{nullptr};
//        size_t poly_degree_ = 0;
//        size_t rns_num_q_ = 0;// level + 1
//        size_t rns_num_p_ = 0;
//        size_t rns_num_total_ = 0;
//        size_t rns_p_offset_ = 0;
//        bool is_ntt_form_ = false;
//        parms_id_type parms_id_ = parms_id_zero;
//        uint64_t *data_ = nullptr;
//        PolyIter poly_iter_;
//        ConstPolyIter const_poly_iter_;
//    };
}



#endif //POSEIDON_RNSPOLY_H
