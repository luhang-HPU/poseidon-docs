//
// Created by lxs on 23-6-16.
//

#ifndef POSEIDON_SOFTWAREEVALUATOR_H
#define POSEIDON_SOFTWAREEVALUATOR_H
#include "poseidon/PoseidonContext.h"
#include "poseidon/Evaluator.h"
#include "poseidon/seal/util/polyarithsmallmod.h"

namespace poseidon {

    class SoftwareEvaluator : public Evaluator
    {
    public:
        explicit SoftwareEvaluator(const PoseidonContext &context)
            : Evaluator(context)
        {
            auto scheme = context.parameters_literal()->scheme();
            if(scheme == CKKS){
                ckks_eva_ = make_shared<CKKSEvaluator_S>(context);
            }
            else if(scheme == BFV){
                bfv_eva_ = make_shared<BFVEvaluator_S>(context);
            }
            else if(scheme == BGV){
                bgv_eva_ = make_shared<BGVEvaluator_S>(context);
            }
        }

        void read(Ciphertext &ciph) const override;
        void add_plain(const Ciphertext &ciph, const Plaintext &plain,Ciphertext &result) const override;
        void multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const override;
        void add(const poseidon::Ciphertext &ciph1,const poseidon::Ciphertext &ciph2,poseidon::Ciphertext &result) const override;
        void sub( const Ciphertext &ciph1, const Ciphertext &ciph2,Ciphertext &result) const override;
        void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const override;
        void square_inplace( Ciphertext &ciph) const override;

        void relinearize(const Ciphertext &ciph, const RelinKeys &relin_keys, Ciphertext &result) const override;
        void rotate(const Ciphertext &ciph, int step,const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void rotate_row(const Ciphertext &ciph, int step,const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void rotate_col(const Ciphertext &ciph,const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void conjugate(const Ciphertext &ciph, const GaloisKeys &galoisKeys, Ciphertext &result) const override;
        void rescale(const Ciphertext &ciph,Ciphertext &result) const override;
        void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result,double min_scale) const override;
        //void multiplyByDiagMatrixBSGS( Ciphertext &ciph, MatrixPlain& plain_mat,Ciphertext &result,const GaloisKeys &rot_key) const override;


    };

    class SoftwareEnvaluatorFactory : public EvaluatorFactory
    {
    protected:
        auto create_impl(const PoseidonContext &context) -> shared_ptr<Evaluator> override
        {
            return  make_shared<SoftwareEvaluator>(context);
        }
    };
} // poseidon

#endif //POSEIDON_SOFTWAREEVALUATOR_H
