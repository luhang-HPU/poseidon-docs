//
// Created by lxs on 23-6-16.
//

#ifndef POSEIDON_EVALUATOR_H
#define POSEIDON_EVALUATOR_H
#include "PoseidonContext.h"
#include "poseidon/plaintext.h"
#include "poseidon/ciphertext.h"
#include "poseidon/keyswitch/keyswitch_bv.h"
#include "poseidon/keyswitch/keyswitch_ghs.h"
#include "poseidon/scheme/ckks/ckks.h"
#include "poseidon/scheme/bfv/bfv.h"
#include "poseidon/scheme/bgv/bgv.h"

namespace poseidon {

    enum EnvaluatorType{
        Hardware,
        Software
    };

    class Evaluator {
    public:
        inline explicit Evaluator(const PoseidonContext &context,MemoryPoolHandle pool = MemoryManager::GetPool())
          :context_(context),pool_(std::move(pool)),scheme_(context.parameters_literal()->scheme())
        {


        }
    protected:
//        std::shared_ptr<KSwitchBase> kswitch_{nullptr};
        std::shared_ptr<CKKSEvaluatorBase> ckks_eva_{nullptr};
        std::shared_ptr<BFVEvaluatorBase> bfv_eva_{nullptr};
        std::shared_ptr<BGVEvaluatorBase> bgv_eva_{nullptr};
        SchemeType scheme_;
        MemoryPoolHandle pool_;
        PoseidonContext context_;
        template <typename T, typename S>
        POSEIDON_NODISCARD inline bool are_same_scale(const T &value1, const S &value2) noexcept
        {
            return util::are_close<double>(value1.scale(), value2.scale());
        }

    public:
        void square(const Ciphertext &ciph,Ciphertext &result) const;
        void multiply_many(const vector<Ciphertext> &encrypteds, const RelinKeys &relin_keys, Ciphertext &destination) const;
        void exponentiate_inplace(Ciphertext &encrypted, uint64_t exponent, const RelinKeys &relin_keys) const;
        void exponentiate(const Ciphertext &encrypted, uint64_t exponent, const RelinKeys &relin_keys,Ciphertext &destination) const;

        void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
        void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;
        void add_const(const Ciphertext &ciph, double constData, Ciphertext &result,const CKKSEncoder &encoder) const;
        void multiply_const_direct(const Ciphertext &ciph,int constData,Ciphertext &result,const CKKSEncoder &encoder) const;
        void multiply_const(const Ciphertext &ciph, double constData, double scale, Ciphertext &result,const CKKSEncoder &encoder) const;
        void multiply_const(const Ciphertext &ciph, complex<double> constData, double scale, Ciphertext &result,const CKKSEncoder &encoder) const;
        void multiply_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
        void multiply_relin(const Ciphertext &ciph1,const Ciphertext &ciph2,Ciphertext &result, const RelinKeys &relin_key) const;
        void multiply_relin_dynamic(const Ciphertext &ciph1,const Ciphertext &ciph2,Ciphertext &result, const RelinKeys &relin_key) const;
        void add_dynamic(const Ciphertext &ciph1,const Ciphertext &ciph2,Ciphertext &result, const CKKSEncoder &encoder) const;
        void sub_dynamic(const Ciphertext &ciph1,const Ciphertext &ciph2,Ciphertext &result, const CKKSEncoder &encoder) const;

        void multiplyByDiagMatrixBSGS( Ciphertext &ciph, MatrixPlain& plain_mat,Ciphertext &result,const GaloisKeys &rot_key) const;
        void dft(const Ciphertext &ciph,const LinearMatrixGroup& matrix_group,Ciphertext &result,const GaloisKeys &rot_key) const;
        void coeff_to_slot(const Ciphertext &ciph,const LinearMatrixGroup& matrix_group,Ciphertext &result_real,Ciphertext &result_imag,
                           const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) const;
        void slot_to_coeff(const Ciphertext &ciph_real,const Ciphertext &ciph_imag,const LinearMatrixGroup &matrix_group, Ciphertext &result,
                           const GaloisKeys &galoisKeys,CKKSEncoder &encoder) const;

        void evaluatePolyVector(const Ciphertext &ciph,Ciphertext &destination,const PolynomialVector &polys,double scale,
                                const RelinKeys &relin_key,const CKKSEncoder &encoder) const;

        void eval_mod(const Ciphertext &ciph,Ciphertext &result,const EvalModPoly &eva_poly,const RelinKeys &relin_key,const CKKSEncoder &encoder);
        void bootstrap(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
                                          const LinearMatrixGroup &matrix_group0, const LinearMatrixGroup &matrix_group1,
                                          const RelinKeys &relin_key, const GaloisKeys &rot_key,const CKKSEncoder &encoder) ;


    public:

        virtual void read(Ciphertext &ciph) const = 0;
        virtual void add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;

        virtual void add(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void sub(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const = 0;
        virtual void square_inplace( Ciphertext &ciph) const = 0;
        virtual void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const = 0;
        virtual void rotate(const Ciphertext &ciph,int step, const GaloisKeys &galoisKeys, Ciphertext &result) const = 0;
        virtual void rotate_row(const Ciphertext &ciph,int step, const GaloisKeys &galoisKeys, Ciphertext &result) const = 0;
        virtual void rotate_col(const Ciphertext &ciph, const GaloisKeys &galoisKeys, Ciphertext &result) const = 0;
        virtual void conjugate(const Ciphertext &ciph, const GaloisKeys &relin_keys, Ciphertext &result) const = 0;
        virtual void rescale(const Ciphertext &ciph,Ciphertext &result) const = 0;
        virtual void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result,double min_scale) const = 0;









    };

    class EvaluatorFactory{
    public:
        EvaluatorFactory() = default;
        virtual ~EvaluatorFactory() = default;
        static auto DefaultFactory() -> std::shared_ptr<EvaluatorFactory>;
        static auto SoftFactory() -> std::shared_ptr<EvaluatorFactory>;
        auto create(const PoseidonContext &context) -> std::shared_ptr<Evaluator>;
        //auto create(const poseidon::PoseidonContext &context,const RelinKeys &relinKeys) -> std::shared_ptr<Evaluator>;

    protected:
        virtual auto create_impl(const PoseidonContext &context) -> std::shared_ptr<Evaluator> = 0;
        //virtual auto create_impl(const PoseidonContext &context,const RelinKeys &relinKeys) -> std::shared_ptr<Evaluator> = 0;
    };

} // poseidon

#endif //POSEIDON_EVALUATOR_H
