//
// Created by cloudam on 12/19/23.
//

#ifndef POSEIDON_CKKSEVALUATORBASE_H
#define POSEIDON_CKKSEVALUATORBASE_H
#include "poseidon/PoseidonContext.h"
#include "poseidon/plaintext.h"
#include "poseidon/ciphertext.h"
#include "poseidon/keyswitch/keyswitch.h"
#include "poseidon/advance/homomorphic_dft.h"
#include "poseidon/advance/polynomial_evaluation.h"
#include "poseidon/advance/homomorphic_mod.h"
namespace poseidon{
    class CKKSEvaluatorBase {


    public:
        void drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const;
        void drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const;


        void multiply_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const;
        void multiplyByDiagMatrixBSGS(const Ciphertext &ciph,const MatrixPlain& plain_mat,Ciphertext &result,const GaloisKeys &rot_key) const;
        void dft(const Ciphertext &ciph,const LinearMatrixGroup& matrix_group,Ciphertext &result,const GaloisKeys &rot_key) const;
        void coeff_to_slot(const Ciphertext &ciph,const LinearMatrixGroup& matrix_group,Ciphertext &result_real,Ciphertext &result_imag,
                           const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) const;
        void slot_to_coeff(const Ciphertext &ciph_real,const Ciphertext &ciph_imag,const LinearMatrixGroup &matrix_group, Ciphertext &result,
                           const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) const;

        void evaluatePolyVector(const Ciphertext &ciph,Ciphertext &destination,const PolynomialVector &polys,double scale,
                                const RelinKeys &relin_key,const CKKSEncoder &encoder) const;

        void eval_mod(const Ciphertext &ciph,Ciphertext &result,const EvalModPoly &eva_poly,const RelinKeys &relin_key,const  CKKSEncoder &encoder);

        void bootstrap(const Ciphertext &ciph,Ciphertext &result,const EvalModPoly &eva_poly,const LinearMatrixGroup& matrix_group0,const LinearMatrixGroup &matrix_group1,
                       const RelinKeys &relin_key, const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) ;

        void multiply_const_direct(const Ciphertext &ciph,int constData,Ciphertext &result,const CKKSEncoder &encoder) const;

        template <typename T, typename = std::enable_if_t<
                std::is_same<std::remove_cv_t<T>, double>::value ||
                std::is_same<std::remove_cv_t<T>, std::complex<double>>::value>>
        void multiply_const(const Ciphertext &ciph,T constData,double scale, Ciphertext &result,const CKKSEncoder &encoder) const;


        template <typename T, typename = std::enable_if_t<
                std::is_same<std::remove_cv_t<T>, double>::value ||
                std::is_same<std::remove_cv_t<T>, std::complex<double>>::value>>
        void add_const(const Ciphertext &ciph,T constData, Ciphertext &result,const CKKSEncoder &encoder) const;

    public:
        virtual void read(Plaintext &plain) const = 0;
        virtual void read(Ciphertext &ciph) const = 0;



        virtual void ftt_fwd(const Plaintext &plain ,Plaintext &result) const = 0;
        virtual void ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const = 0;
        virtual void ftt_inv(const Plaintext &plain ,Plaintext &result) const = 0;
        virtual void ftt_inv(const Ciphertext &ciph ,Ciphertext &result) const = 0;
        virtual void add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const = 0;
        virtual void add(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void sub(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const = 0;
        virtual void multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const = 0;
        virtual void square_inplace( Ciphertext &ciph) const = 0;

        virtual void relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const = 0;
        virtual void rotate(const Ciphertext &ciph,int step, const GaloisKeys &galoisKeys, Ciphertext &result) const = 0;
        virtual void conjugate(const Ciphertext &ciph, const GaloisKeys &galoisKeys, Ciphertext &result) const = 0;
        virtual void rescale(const Ciphertext &ciph,Ciphertext &result ) const = 0;
        virtual void rescale_dynamic(const Ciphertext &ciph,Ciphertext &result,double min_scale) const = 0;
        virtual void drop_modulus(const Ciphertext &ciph,Ciphertext &result,parms_id_type parmsIdType) const = 0;
        virtual void raise_modulus(const Ciphertext &ciph,Ciphertext &result) const = 0;
        virtual void multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const RelinKeys &relin_key) const = 0;
        virtual void multiply_relin_dynamic(const Ciphertext &ciph1,const Ciphertext &ciph2,Ciphertext &result, const RelinKeys &relin_key) const = 0;
        virtual void sub_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const = 0;
        virtual void add_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const = 0;



    private:
        inline void set_min_scale(double scale)
        {
            min_scale_ = scale;
        }


        void GenPower(map <uint32_t ,Ciphertext> &monomialBasis,uint32_t n,bool lazy,bool isChev,double min_scale,const RelinKeys &relin_key,const CKKSEncoder &encoder) const;
        void genPower(map <uint32_t,Ciphertext> &monomialBasis,uint32_t n,bool lazy,bool isChev,double min_scale,const RelinKeys &relin_key,const CKKSEncoder &encoder) const;
        //void evaluatePolyFromPolynomialBasis(bool is_even,bool is_odd, const map <int,Ciphertext> &monomialBasis,const RelinKeys &relinKeys ,uint32_t targetLevel ,double targetScale ,
        //                                const PolynomialVector &pol, uint32_t log_split,uint32_t log_degree, Ciphertext &destination ,const CKKSEncoder &encoder) const;

        void recurse(const map <uint32_t ,Ciphertext> &monomialBasis,const RelinKeys &relinKeys ,uint32_t targetLevel ,double targetScale ,
                     const PolynomialVector &pol, uint32_t log_split,uint32_t log_degree, Ciphertext &destination,const CKKSEncoder &encoder,bool isOdd,bool isEven,uint32_t &num) const;
        POSEIDON_NODISCARD tuple<uint32_t ,double> pre_scalar_level(bool is_even,bool is_odd, const map <uint32_t,Ciphertext> &monomialBasis,
                                                                    double currentScale,uint32_t currentLevel,const PolynomialVector &pol, uint32_t log_split,uint32_t log_degree) const;

        void evaluatePolyFromPolynomialBasis(bool is_even,bool is_odd, const map <uint32_t,Ciphertext> &monomialBasis,const RelinKeys &relinKeys ,uint32_t targetLevel ,double targetScale ,
                                             const PolynomialVector &pol, uint32_t log_split,uint32_t log_degree, Ciphertext &destination ,const CKKSEncoder &encoder) const;

    protected:
        inline explicit CKKSEvaluatorBase(const PoseidonContext &context,MemoryPoolHandle pool = MemoryManager::GetPool())
                :context_(context),min_scale_(std::pow(2.0,context.parameters_literal()->LogScale())),pool_(std::move(pool))
        {

        }
        PoseidonContext context_;
        double min_scale_;
        MemoryPoolHandle pool_;



    };





}



#endif //POSEIDON_CKKSEVALUATORBASE_H
