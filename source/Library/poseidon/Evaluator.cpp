//
// Created by lxs on 23-6-16.
//

#include "poseidon/Evaluator.h"

#include "poseidon/SoftwareEvaluator.h"
namespace poseidon {
    auto EvaluatorFactory::DefaultFactory() -> std::shared_ptr<EvaluatorFactory> {
//        static shared_ptr<EvaluatorFactory> default_factory{ new  HardwareEnvaluatorFactory};
        static shared_ptr<EvaluatorFactory> default_factory{ new  SoftwareEnvaluatorFactory};
        return default_factory;
    }

    auto EvaluatorFactory::SoftFactory() -> std::shared_ptr<EvaluatorFactory> {
        static shared_ptr<EvaluatorFactory> default_factory{ new  SoftwareEnvaluatorFactory};
        return default_factory;
    }

    auto EvaluatorFactory::create(const poseidon::PoseidonContext &context) -> std::shared_ptr<Evaluator> {
        return create_impl(context);
    }



    void Evaluator::drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const{
        switch (scheme_) {
            case CKKS:
                ckks_eva_->drop_modulus(ciph,result,level);
                break;
            case BFV:
                bfv_eva_->drop_modulus(ciph,result,level);
                break;
            case BGV:
                bgv_eva_->drop_modulus(ciph,result,level);
                break;
        }
    }
    void Evaluator::drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const{
        switch (scheme_) {
            case CKKS:
                ckks_eva_->drop_modulus_to_next(ciph,result);
                break;
            case BFV:
                bfv_eva_->drop_modulus_to_next(ciph,result);
                break;
            case BGV:
                bgv_eva_->drop_modulus_to_next(ciph,result);
                break;
        }
    }



    void Evaluator::add_const(const Ciphertext &ciph, double constData, Ciphertext &result,const CKKSEncoder &encoder) const{

        if(scheme_ != CKKS){
            throw invalid_argument("multiply_const : only ckks support!");
        }
        ckks_eva_->add_const(ciph,constData,result,encoder);
    }


//    auto EvaluatorFactory::create(const poseidon::PoseidonContext &context,const RelinKeys &relinKeys) -> std::shared_ptr<Evaluator> {
//        return create_impl(context,relinKeys);
//    }
    void Evaluator::multiply_const_direct(const Ciphertext &ciph,int constData,Ciphertext &result,const CKKSEncoder &encoder) const{
        if(scheme_ != CKKS){
            throw invalid_argument("multiply_const_direct : only ckks support!");
        }
        ckks_eva_->multiply_const_direct(ciph,constData,result,encoder);
    }



    void Evaluator::multiply_const(const Ciphertext &ciph, double constData, double scale, Ciphertext &result,const CKKSEncoder &encoder) const{

        if(scheme_ != CKKS){
            throw invalid_argument("multiply_const : only ckks support!");
        }
        ckks_eva_->multiply_const(ciph,constData,scale,result,encoder);
    }

    void Evaluator::multiply_const(const Ciphertext &ciph, complex<double> constData, double scale, Ciphertext &result,const CKKSEncoder &encoder) const{
        if(scheme_ != CKKS){
            throw invalid_argument("multiply_const : only ckks support!");
        }
        ckks_eva_->multiply_const(ciph,constData,scale,result,encoder);
    }

    void Evaluator::square(const Ciphertext &ciph,Ciphertext &result) const{
        result = ciph;
        square_inplace(result);
    }
    void Evaluator::multiply_many(const vector<Ciphertext> &encrypteds, const RelinKeys &relin_keys, Ciphertext &destination) const{
        if (encrypteds.size() == 0)
        {
            throw invalid_argument("encrypteds vector must not be empty");
        }

        for (size_t i = 0; i < encrypteds.size(); i++)
        {
            if (&encrypteds[i] == &destination)
            {
                throw invalid_argument("encrypteds must be different from destination");
            }
        }

        // There is at least one ciphertext
        auto context_data_ptr = context_.crt_context()->get_context_data(encrypteds[0].parms_id());
        if (!context_data_ptr)
        {
            throw invalid_argument("encrypteds is not valid for encryption parameters");
        }

        // Extract encryption parameters.
        auto &context_data = *context_data_ptr;
        auto &parms = context_data.parms();

        if (parms.scheme() != BFV && parms.scheme() != BGV)
        {
            POSEIDON_THROW(config_error,"scheme don't support multiply_many");
        }

        // If there is only one ciphertext, return it.
        if (encrypteds.size() == 1)
        {
            destination = encrypteds[0];
            return;
        }

        // Do first level of multiplications
        vector<Ciphertext> product_vec;
        for (size_t i = 0; i < encrypteds.size() - 1; i += 2)
        {
            Ciphertext temp(context_, context_data.parms().parms_id(), pool_);
            if (encrypteds[i].data() == encrypteds[i + 1].data())
            {
                square(encrypteds[i], temp);
            }
            else
            {
                multiply(encrypteds[i], encrypteds[i + 1], temp);
            }
            relinearize(temp, relin_keys, temp);
            product_vec.emplace_back(move(temp));
        }
        if (encrypteds.size() & 1)
        {
            product_vec.emplace_back(encrypteds.back());
        }

        // Repeatedly multiply and add to the back of the vector until the end is reached
        for (size_t i = 0; i < product_vec.size() - 1; i += 2)
        {
            Ciphertext temp(context_, context_data.parms().parms_id(), pool_);
            multiply(product_vec[i], product_vec[i + 1], temp);
            relinearize(temp, relin_keys, temp);
            product_vec.emplace_back(move(temp));
        }

        destination = product_vec.back();
    }

    void Evaluator::exponentiate_inplace(Ciphertext &encrypted, uint64_t exponent, const RelinKeys &relin_keys) const{
        // Verify parameters.
        auto context_data_ptr = context_.crt_context()->get_context_data(encrypted.parms_id());
        if (!context_data_ptr)
        {
            throw invalid_argument("encrypted is not valid for encryption parameters");
        }
        if (!context_.crt_context()->get_context_data(relin_keys.parms_id()))
        {
            throw invalid_argument("relin_keys is not valid for encryption parameters");
        }

        if (exponent == 0)
        {
            throw invalid_argument("exponent cannot be 0");
        }

        // Fast case
        if (exponent == 1)
        {
            return;
        }

        // Create a vector of copies of encrypted
        vector<Ciphertext> exp_vector(static_cast<size_t>(exponent), encrypted);
        multiply_many(exp_vector, relin_keys, encrypted);
    }

    void Evaluator::exponentiate(const Ciphertext &encrypted, uint64_t exponent, const RelinKeys &relin_keys,Ciphertext &destination) const{
        destination = encrypted;
        exponentiate_inplace(destination, exponent,relin_keys);
    }




    void Evaluator::multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                           const RelinKeys &relin_key) const {
        if(scheme_ != CKKS){
            throw invalid_argument("multiply_relin : only ckks support!");
        }
        ckks_eva_->multiply_relin(ciph1,ciph2,result,relin_key);
    }

    void Evaluator::multiply_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const {
        if(scheme_ != CKKS){
            throw invalid_argument("multiply_relin_dynamic : only ckks support!");
        }
        ckks_eva_->multiply_dynamic(ciph1,ciph2,result);
    }

    void Evaluator::multiply_relin_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                           const RelinKeys &relin_key) const {
        if(scheme_ != CKKS){
            throw invalid_argument("multiply_relin_dynamic : only ckks support!");
        }
        ckks_eva_->multiply_relin_dynamic(ciph1,ciph2,result,relin_key);
    }



    void Evaluator::multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result,
                                             const GaloisKeys &rot_key) const {
        if(scheme_ != CKKS){
            throw invalid_argument("multiplyByDiagMatrixBSGS : only ckks support!");
        }
        ckks_eva_->multiplyByDiagMatrixBSGS(ciph,plain_mat,result,rot_key);

    }

    void Evaluator::dft(const Ciphertext &ciph, const LinearMatrixGroup &matrix_group, Ciphertext &result,
                        const GaloisKeys &rot_key) const {
        if(scheme_ != CKKS){
            throw invalid_argument("dft : only ckks support!");
        }
        ckks_eva_->dft(ciph,matrix_group,result,rot_key);
    }

    void
    Evaluator::coeff_to_slot(const Ciphertext &ciph, const LinearMatrixGroup &matrix_group, Ciphertext &result_real,
                             Ciphertext &result_imag, const GaloisKeys &galoisKeys, const CKKSEncoder &encoder) const {
        if(scheme_ != CKKS){
            throw invalid_argument("coeff_to_slot : only ckks support!");
        }
        ckks_eva_->coeff_to_slot(ciph,matrix_group,result_real,result_imag,galoisKeys,encoder);

    }

    void Evaluator::slot_to_coeff(const Ciphertext &ciph_real, const Ciphertext &ciph_imag,
                                  const LinearMatrixGroup &matrix_group, Ciphertext &result,
                                  const GaloisKeys &galoisKeys, CKKSEncoder &encoder) const {
        if(scheme_ != CKKS){
            throw invalid_argument("rescale_dynamic : only ckks support!");
        }
        ckks_eva_->slot_to_coeff(ciph_real,ciph_imag,matrix_group,result,galoisKeys,encoder);
    }

    void Evaluator::evaluatePolyVector(const Ciphertext &ciph, Ciphertext &destination, const PolynomialVector &polys,
                                       double scale, const RelinKeys &relin_key, const CKKSEncoder &encoder) const {
        if(scheme_ != CKKS){
            throw invalid_argument("evaluatePolyVector : only ckks support!");
        }
        ckks_eva_->evaluatePolyVector(ciph,destination,polys,scale,relin_key,encoder);
    }

    void Evaluator::add_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                const CKKSEncoder &encoder) const {

        if(scheme_ != CKKS){
            throw invalid_argument("add_dynamic : only ckks support!");
        }
        ckks_eva_->add_dynamic(ciph1,ciph2,result,encoder);
    }

    void Evaluator::sub_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                const CKKSEncoder &encoder) const {

        if(scheme_ != CKKS){
            throw invalid_argument("sub_dynamic : only ckks support!");
        }
        ckks_eva_->sub_dynamic(ciph1,ciph2,result,encoder);
    }


    void Evaluator::eval_mod(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
                             const RelinKeys &relin_key, const CKKSEncoder &encoder) {
        if(scheme_ != CKKS){
            throw invalid_argument("eval_mod : only ckks support!");
        }
        ckks_eva_->eval_mod(ciph,result,eva_poly,relin_key,encoder);

    }

    void Evaluator::bootstrap(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
                              const LinearMatrixGroup &matrix_group0, const LinearMatrixGroup &matrix_group1,
                              const RelinKeys &relin_key, const GaloisKeys &galoisKeys,
                          const CKKSEncoder &encoder)  {

        if(scheme_ != CKKS){
            throw invalid_argument("bootstrap : only ckks support!");
        }

        ckks_eva_->bootstrap(ciph,result,eva_poly,matrix_group0,matrix_group1,relin_key,galoisKeys,encoder);

    }



} // poseidon