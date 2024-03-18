#include "poseidon/SoftwareEvaluator.h"
#include "poseidon/util/debug.h"
namespace poseidon{

    void SoftwareEvaluator::add(const poseidon::Ciphertext &ciph1,const poseidon::Ciphertext &ciph2,
                                poseidon::Ciphertext &result) const {
        switch (scheme_) {
            case CKKS:
                ckks_eva_->add(ciph1,ciph2,result);
                break;
            case BFV:
                bfv_eva_->add(ciph1,ciph2,result);
                break;
            case BGV:
                bgv_eva_->add(ciph1,ciph2,result);
                break;
        }
    }

    void SoftwareEvaluator::sub(const Ciphertext &ciph1,const  Ciphertext &ciph2,Ciphertext &result) const {
        switch (scheme_) {
            case CKKS:
                ckks_eva_->sub(ciph1,ciph2,result);
                break;
            case BFV:
                bfv_eva_->sub(ciph1,ciph2,result);
                break;
            case BGV:
                bgv_eva_->sub(ciph1,ciph2,result);
                break;
        }

    }

    void SoftwareEvaluator::multiply_plain(const Ciphertext &ciph, const Plaintext &plain, Ciphertext &result) const {
        switch (scheme_) {
            case CKKS:
                ckks_eva_->multiply_plain(ciph,plain,result);
                break;
            case BFV:
                bfv_eva_->multiply_plain(ciph,plain,result);
                break;
            case BGV:
                bgv_eva_->multiply_plain(ciph,plain,result);
                break;
        }

    }

    void SoftwareEvaluator::add_plain(const Ciphertext &ciph, const Plaintext &plain,Ciphertext &result) const{
        switch (scheme_) {
            case CKKS:
                ckks_eva_->add_plain(ciph,plain,result);
                break;
            case BFV:
                bfv_eva_->add_plain(ciph,plain,result);
                break;
            case BGV:
                bgv_eva_->add_plain(ciph,plain,result);
                break;
        }
    }




    void SoftwareEvaluator::multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const
    {
        if(scheme_ == CKKS){
            ckks_eva_->multiply(ciph1,ciph2,result);
        }
        else if(scheme_ == BFV){
            bfv_eva_->multiply(ciph1,ciph2,result);
        }
        else{
            bgv_eva_->multiply(ciph1,ciph2,result);
        }
    }

    void SoftwareEvaluator::square_inplace( Ciphertext &ciph) const {
        if(scheme_ == CKKS){
            ckks_eva_->square_inplace(ciph);
        }
        else if(scheme_ == BFV){
            bfv_eva_->square_inplace(ciph);
        }
        else{
//            bgv_eva_->square_inplace(ciph);
        }
    }


    void SoftwareEvaluator::relinearize(const Ciphertext &ciph, const RelinKeys &relin_keys,
                                        Ciphertext &result) const {
        if(scheme_ == CKKS){
            ckks_eva_->relinearize(ciph,relin_keys,result);
        }
        else if(scheme_ == BFV){
            bfv_eva_->relinearize(ciph,relin_keys,result);
        }
        else{
            bgv_eva_->relinearize(ciph,relin_keys,result);
        }

    }

    void SoftwareEvaluator::rotate(const Ciphertext &ciph, int step,const GaloisKeys &galoisKeys, Ciphertext &result) const {
        if(scheme_ != CKKS){
            POSEIDON_THROW(config_error,"rotate is not support");
        }
        ckks_eva_->rotate(ciph,step,galoisKeys,result);
    }

    void SoftwareEvaluator::rotate_row(const Ciphertext &ciph, int step,const GaloisKeys &galoisKeys, Ciphertext &result) const {
        if(scheme_ != BFV && scheme_ != BGV){
            POSEIDON_THROW(config_error,"rotate_row is not support");
        }
        if(scheme_ == BFV){
            bfv_eva_->rotate_row(ciph,step,galoisKeys,result);
        }
        else{
            bgv_eva_->rotate_row(ciph,step,galoisKeys,result);
        }
    }
    void SoftwareEvaluator::rotate_col(const Ciphertext &ciph,const GaloisKeys &galoisKeys, Ciphertext &result) const {
        if(scheme_ != BFV && scheme_ != BGV){
            POSEIDON_THROW(config_error,"rotate_col is not support");
        }
        if(scheme_ == BFV){
            bfv_eva_->rotate_col(ciph,galoisKeys,result);
        }
        else{
            bgv_eva_->rotate_col(ciph,galoisKeys,result);
        }
    }

    void SoftwareEvaluator::conjugate(const Ciphertext &ciph, const GaloisKeys &galoisKeys, Ciphertext &result) const {
        ckks_eva_->conjugate(ciph,galoisKeys,result);
    }

    void SoftwareEvaluator::rescale(const Ciphertext &ciph, Ciphertext &result) const {
        ckks_eva_->rescale(ciph,result);
    }

    void SoftwareEvaluator::rescale_dynamic(const Ciphertext &ciph, Ciphertext &result, double min_scale) const {
        if(scheme_ != CKKS){
            throw invalid_argument("rescale_dynamic : only ckks support!");
        }
        ckks_eva_->rescale_dynamic(ciph,result,min_scale);
    }

    void SoftwareEvaluator::read(Ciphertext &ciph) const {

    }

//    void SoftwareEvaluator::multiplyByDiagMatrixBSGS(Ciphertext &ciph, MatrixPlain &plain_mat, Ciphertext &result,
//                                                     const GaloisKeys &rot_key) const {
//        if(scheme_ != ckks){
//            throw invalid_argument("rescale_dynamic : only ckks support!");
//        }
//        ckks_eva_->multiplyByDiagMatrixBSGS(ciph,plain_mat,result,rot_key);
//    }
}