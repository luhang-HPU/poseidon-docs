//
// Created by cloudam on 12/19/23.
//

#include "poseidon/scheme/ckks/CKKSEvaluatorBase.h"
#include "poseidon/util/debug.h"
namespace poseidon{
    void CKKSEvaluatorBase::drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const {
        auto parms_id = context_.crt_context()->parms_id_map().at(level);
        drop_modulus(ciph,result,parms_id);
    }

    void CKKSEvaluatorBase::drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const{
        auto level = ciph.level();
        auto parms_id = context_.crt_context()->parms_id_map().at(level-1);
        drop_modulus(ciph,result,parms_id);
    }

    template<typename T, typename>
    void CKKSEvaluatorBase::multiply_const(const Ciphertext &ciph, T constData, double scale, Ciphertext &result,
                                           const CKKSEncoder &encoder) const {

        if(constData == 0.0 || constData == complex<double>(0.0,0.0)){
            multiply_const_direct(ciph, 0, result,encoder);
        }
        else{
            Plaintext tmp;
            encoder.encode(constData,ciph.parms_id(),scale,tmp);
            multiply_plain(ciph,tmp,result);
        }
    }





    void CKKSEvaluatorBase::multiply_const_direct(const Ciphertext &ciph, int constData, Ciphertext &result,
                                                  const CKKSEncoder &encoder) const {
        Plaintext tmp;
        encoder.encode(constData,ciph.parms_id(),tmp);
        multiply_plain(ciph,tmp,result);
    }

    template<typename T, typename>
    void CKKSEvaluatorBase::add_const(const Ciphertext &ciph, T constData, Ciphertext &result,
                                      const CKKSEncoder &encoder) const {

        if(constData == 0.0 || constData == complex<double>(0.0,0.0)){
            Plaintext tmp;
            encoder.encode(0,ciph.parms_id(),tmp);
            add_plain(ciph,tmp,result);
            return;
        }
        Plaintext tmp;
        encoder.encode(constData,ciph.parms_id(),ciph.scale(),tmp);
        add_plain(ciph,tmp,result);
    }






    void CKKSEvaluatorBase::multiply_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const {
        auto levelA = ciph1.level();
        auto levelB = ciph2.level();
        if(levelA > levelB){
            Ciphertext tmp;
            if(&result == &ciph2){
                drop_modulus(ciph1,tmp,ciph2.parms_id());
                multiply( ciph2, tmp,result);
            }
            else{
                drop_modulus(ciph1,result,ciph2.parms_id());
                multiply( result, ciph2,result);
            }
        }
        else if(levelB > levelA){
            Ciphertext tmp;
            if(&result == &ciph1){
                drop_modulus(ciph2,tmp,ciph1.parms_id());
                multiply( ciph1, tmp,result);
            }
            else{
                drop_modulus(ciph2,result,ciph1.parms_id());
                multiply( ciph1, result,result);
            }
        }
        else{
            multiply( ciph1, ciph2,result);
        }
    }

    void CKKSEvaluatorBase::multiplyByDiagMatrixBSGS(const Ciphertext &ciph,const MatrixPlain &plain_mat, Ciphertext &result,
                                                     const GaloisKeys &rot_key) const {

        auto [index, _, rotN2] = BSGSIndex(plain_mat.plain_vec, 1<<plain_mat.LogSlots, plain_mat.N1);
        map <int,Ciphertext> rot_ciph;
        Ciphertext ciph_inner_sum,ciph_inner,result_tmp,ciph_inner_tmp;
        Ciphertext ciph_rrr;
        for(auto j : rotN2){
            if(j != 0) {
                rotate(ciph, j, rot_key, rot_ciph[j]);
            }
        }

        int cnt0 = 0;
        for(const auto& j : index){
            int cnt1 = 0;
            for(auto i : index[j.first])
            {
                if(i == 0){
                    if(cnt1 == 0){
                        if(cnt0 == 0){
                            multiply_plain(ciph,plain_mat.plain_vec.at(j.first),result_tmp);
                        }
                        else{
                            multiply_plain(ciph,plain_mat.plain_vec.at(j.first),ciph_inner_sum);
                        }
                    }
                    else{
                        multiply_plain(ciph,plain_mat.plain_vec.at(j.first),ciph_inner);
                        if(cnt0 == 0){
                            add(result_tmp,ciph_inner,result_tmp);
                        }
                        else{
                            add(ciph_inner_sum,ciph_inner,ciph_inner_sum);
                        }
                    }
                }
                else{
                    if(cnt1 == 0){
                        if(cnt0 == 0){
                            multiply_plain(rot_ciph[i],plain_mat.plain_vec.at(i+j.first),result_tmp);
                        }
                        else{
                            multiply_plain(rot_ciph[i],plain_mat.plain_vec.at(i+j.first),ciph_inner_sum);
                        }
                    }
                    else{

                        multiply_plain(rot_ciph[i],plain_mat.plain_vec.at(i+j.first),ciph_inner);
                        if(cnt0 == 0){
                            add(result_tmp,ciph_inner,result_tmp);
                        }
                        else{
                            add(ciph_inner_sum,ciph_inner,ciph_inner_sum);
                        }
                    }
                }

                cnt1++;
            }
            if(cnt0 != 0){
                auto step_src = j.first;
                rotate(ciph_inner_sum,j.first,rot_key,ciph_inner);
                add(result_tmp,ciph_inner,result_tmp);
            }
            cnt0++;
        }
        rescale_dynamic(result_tmp,result,ciph.scale());
    }

    void CKKSEvaluatorBase::dft(const Ciphertext &ciph,const LinearMatrixGroup &matrix_group, Ciphertext &result,
                                const GaloisKeys &rot_key) const {

        multiplyByDiagMatrixBSGS(ciph,matrix_group.data()[0],result,rot_key);
        for(int i = 1; i < matrix_group.data().size(); i++){
            multiplyByDiagMatrixBSGS(result,matrix_group.data()[i],result,rot_key);
        }
    }

    void CKKSEvaluatorBase::coeff_to_slot(const Ciphertext &ciph, const LinearMatrixGroup &matrix_group,
                                          Ciphertext &result_real, Ciphertext &result_imag, const GaloisKeys &galoisKeys, const CKKSEncoder &encoder) const {
        Ciphertext ciph_tmp;
        dft(ciph,matrix_group,ciph_tmp,galoisKeys);
        conjugate(ciph_tmp,galoisKeys,result_imag);
        add(ciph_tmp,result_imag,result_real);
        sub(ciph_tmp,result_imag,result_imag);
        complex<double> const_data(0,-1);

        auto context_data = context_.crt_context()->get_context_data(result_imag.parms_id());
        multiply_const(result_imag,const_data,1.0,result_imag,encoder);
    }

    void CKKSEvaluatorBase::slot_to_coeff(const Ciphertext &ciph_real, const Ciphertext &ciph_imag,
                                          const LinearMatrixGroup &matrix_group, Ciphertext &result,
                                          const GaloisKeys &galoisKeys,const CKKSEncoder &encoder) const {
        complex<double> const_data(0,1);
        Ciphertext result_tmp;
        multiply_const(ciph_imag,const_data,1.0,result_tmp,encoder);
        add(result_tmp,ciph_real,result);
        dft(result,matrix_group,result,galoisKeys);
    }

    void CKKSEvaluatorBase::evaluatePolyVector(const Ciphertext &ciph, Ciphertext &destination,
                                               const PolynomialVector &polys, double scale, const RelinKeys &relin_key,
                                               const CKKSEncoder &encoder) const {
        map <uint32_t ,Ciphertext> monomialBasis;
        monomialBasis[1] = ciph;

        int logDegree = ceil(log2(polys.polys()[0].degree()));
        int logSplit = optimalSplit(logDegree);

        bool odd = true;
        bool even = true;

        for(auto p : polys.polys()) {
            auto [tmp0,  tmp1] = isOddOrEvenPolynomial(p);
            odd = odd && tmp0;
            even = even && tmp1;
        }

        bool isChebyshev = false;
        if(polys.polys()[0].basis_type() == Chebyshev){
            isChebyshev = true;
        }
        else{
            isChebyshev = false;
        }


        GenPower(monomialBasis,1 << logDegree,false,isChebyshev,scale,relin_key,encoder);
        for(int i = ((int64_t)1 << logSplit)-1 ; i > 2; i--){
            auto state = i&1;
            if(!(even || odd) || (state == 0 && even) || ((state == 1 && odd))){
                GenPower(monomialBasis, i,false, isChebyshev,scale, relin_key,encoder);
            }
        }


        //destination = monomialBasis[7];

        auto index = pow(2,logDegree);
        double targetScale = scale;
        auto targetLevel = monomialBasis.at(index).level();

        auto parmsId = monomialBasis.at(index).parms_id();
        uint32_t num = 0;
        recurse(monomialBasis,relin_key ,targetLevel , targetScale ,
                polys, logSplit,logDegree,  destination,encoder,odd,even,num);

        rescale_dynamic(destination,destination,targetScale);
        destination.scale() = targetScale;

    }

    void CKKSEvaluatorBase::GenPower(map<uint32_t, Ciphertext> &monomialBasis, uint32_t n, bool lazy, bool isChev,
                                     double min_scale, const RelinKeys &relin_key,const CKKSEncoder &encoder) const {
        genPower(monomialBasis,n,lazy,isChev,min_scale,relin_key,encoder);
        rescale_dynamic(monomialBasis[n],monomialBasis[n],min_scale);
    }



    void CKKSEvaluatorBase::genPower(map<uint32_t, Ciphertext> &monomialBasis, uint32_t n, bool lazy, bool isChev,
                                     double min_scale, const RelinKeys &relin_key,const CKKSEncoder &encoder) const {

        if(!monomialBasis[n].is_valid()){
            bool isPow2 = ((n & (n - 1)) == 0);
            int a,b,c =0;
            if(isPow2){
                a = n/2;
                b = a;
            }
            else{
                int k = ceil(log2((float)n)) - 1;
                a = (1 << k) - 1;
                b = n + 1 - (1 << k);
                if(isChev){
                    c = (int)(abs(a-b));
                }
            }

            genPower(monomialBasis,a,lazy && !isPow2,isChev,min_scale,relin_key,encoder);
            genPower(monomialBasis,b,lazy && !isPow2,isChev,min_scale,relin_key,encoder);

            if(lazy){
                throw invalid_argument("genPower: lazy should be false!");
            }
            else{

                printf("scale %.5Ff, a = %d ,b = %d, c = %d,n = %d\n",min_scale,a,b,c,n);
                printf("monomialBasis[%d] %.5Ff\n",a,monomialBasis[a].scale());
                rescale_dynamic(monomialBasis[a],monomialBasis[a],min_scale);
                rescale_dynamic(monomialBasis[b],monomialBasis[b],min_scale);
                multiply_relin_dynamic(monomialBasis[a],monomialBasis[b],monomialBasis[n],relin_key);

//                if(monomialBasis[a].parms_id() == monomialBasis[b].parms_id()){
//                    multiply_relin_dynamic(monomialBasis[a],monomialBasis[b],monomialBasis[n],relin_key);
//                }
//                else if(monomialBasis[a].metaData()->getLevel() > monomialBasis[b].metaData()->getLevel())
//                {
//                    int levelDiff = monomialBasis[a].metaData()->getLevel() - monomialBasis[b].metaData()->getLevel();
//                    Ciphertext tmp = monomialBasis[a];
//                    //low_modulus(tmp,levelDiff);
//                    multiply_dynamic(tmp,monomialBasis[b],monomialBasis[n],relin_key);
//                }
//                else{
//                    int levelDiff = monomialBasis[b].metaData()->getLevel() - monomialBasis[a].metaData()->getLevel();
//                    Ciphertext tmp = monomialBasis[b];
//                    //low_modulus(tmp,levelDiff);
//                    multiply_dynamic(monomialBasis[a],tmp,monomialBasis[n],relin_key);
//                }

            }

            if(isChev){
                add(monomialBasis[n],monomialBasis[n],monomialBasis[n]);
                if (c == 0) {
                    add_const(monomialBasis[n], -1.0, monomialBasis[n],encoder);
                } else {
                    // Since C[0] is not stored (but rather seen as the constant 1), only recurses on c if c!= 0
                    genPower(monomialBasis,c,lazy && !isPow2,isChev,min_scale,relin_key,encoder);


                    Ciphertext ciph_tmp;
                    //rescale_dynamic(monomialBasis[n],scale);
                    auto scale_tmp = monomialBasis[c].scale();
                    scale_tmp = monomialBasis[n].scale() / scale_tmp;

                    multiply_const(monomialBasis[c],1.0,scale_tmp,ciph_tmp,encoder);
                    //gmp_printf("gen power scale_tmp: %.8Ff \n",scale_tmp.get_mpf_t());
                    sub_dynamic(monomialBasis[n], ciph_tmp, monomialBasis[n],encoder);
                    //gmp_printf("level monomialBasis[%d], %d   %.5Ff\n",n,monomialBasis[n].metaData()->getLevel(),monomialBasis[n].metaData()->getScalingFactor().get_mpf_t());
                    //gmp_printf("level monomialBasis[%d], %d   %.5Ff\n",c,ciph_tmp.metaData()->getLevel(),ciph_tmp.metaData()->getScalingFactor().get_mpf_t());

                }
            }
        }
    }

    void CKKSEvaluatorBase::recurse(const map<uint32_t , Ciphertext> &monomialBasis, const RelinKeys &relinKeys,
                                    uint32_t targetLevel, double targetScale, const PolynomialVector &pol,
                                    uint32_t log_split, uint32_t log_degree, Ciphertext &destination,
                                    const CKKSEncoder &encoder, bool isOdd, bool isEven, uint32_t &num) const {


        double minTargetScale = min_scale_;
        double pow_scale;
        auto logSplit = log_split;
        auto logDegree = log_degree;
        auto pol_deg = pol.polys()[0].degree();
        auto parms = context_.parameters_literal();
        auto &modulus = parms->Q();
        if (pol_deg < (1 << logSplit)) {
            if (pol.polys()[0].lead() && (logSplit > 1) &&
                (pol.polys()[0].max_degree() % (1 << (logSplit + 1))) > (1 << (logSplit - 1))) {
                logDegree = log2(pol.polys()[0].degree() + 1);
                logSplit = logDegree >> 1;
                recurse(monomialBasis, relinKeys, targetLevel, targetScale, pol, logSplit, logDegree, destination,
                        encoder, isOdd, isEven, num);
                return;
            }
            auto targetScale_new = targetScale;
            auto [tag_level, tag_scale] = pre_scalar_level(isEven, isOdd, monomialBasis,
                                                           targetScale_new, targetLevel, pol, log_split, log_degree);
            gmp_printf("inside target level: %d,  target scale: %0.7lf\n",tag_level,tag_scale);

            evaluatePolyFromPolynomialBasis(isEven, isOdd, monomialBasis, relinKeys, tag_level, tag_scale,
                                            pol, logSplit, logDegree, destination, encoder);

            return;
        }
        auto nextPower = 1 << logSplit;
        while (nextPower < ((pol.polys()[0].degree() >> 1) + 1)) {
            nextPower <<= 1;
        }
        PolynomialVector coeffsq, coeffsr;
        coeffsq.index() = pol.index();
        coeffsr.index() = pol.index();
        splitCoeffsPolyVector(pol, coeffsq, coeffsr, nextPower);
        auto XPow = monomialBasis.at(nextPower);

        auto targetScale_new = targetScale;
        double tmpScale;
        auto targetScalePass = false;
        int new_targetLevel = targetLevel;

        if (num == 0 && pol.polys()[0].lead()) {

            while (!targetScalePass) {
                auto currentQi = safe_cast<double>(modulus[new_targetLevel - num].value());
                num++;
                targetScale_new *= currentQi;
                tmpScale = targetScale_new / XPow.scale();
                if (tmpScale >= minTargetScale) {
                    targetScale_new = tmpScale;
                    targetScalePass = true;
                }
            }

        } else if (pol.polys()[0].lead()) {

            while (!targetScalePass) {
                new_targetLevel++;
                auto currentQi = safe_cast<double>(modulus[new_targetLevel].value());
                targetScale_new *= currentQi;
                tmpScale = targetScale_new / XPow.scale();
                if (tmpScale >= minTargetScale) {
                    targetScale_new = tmpScale;
                    targetScalePass = true;
                    //break;
                }

            }
        } else {
            targetScale_new /= XPow.scale();
            pow_scale = targetScale_new;
            while (!targetScalePass) {
                new_targetLevel++;
                auto currentQi = safe_cast<double>(modulus[new_targetLevel].value());
                targetScale_new *= currentQi;
                tmpScale = targetScale_new / XPow.scale();
                if (tmpScale >= minTargetScale) {
                    targetScalePass = true;
                }
            }
        }

        printf("outside target level: %d,  target scale: %0.7lf\n",new_targetLevel,targetScale_new);

        Ciphertext res;
        recurse(monomialBasis, relinKeys, new_targetLevel, targetScale_new, coeffsq, logSplit, logDegree, res, encoder,
                isOdd, isEven, num);

        printf("1:res level: %zu,  target scale: %0.lf\n",res.level(),res.scale());


        if (!pol.polys()[0].lead()) {
            rescale_dynamic(res, res, pow_scale);
        } else {
            rescale_dynamic(res, res, context_.parameters_literal()->scale());
        }

        printf("2:res level: %zu,  target scale: %0.7lf\n",res.level(),res.scale());
        printf("3:XPow level: %zu,  target scale: %0.7lf\n",XPow.level(),XPow.scale());

        multiply_relin_dynamic(res, XPow, res, relinKeys);
        printf("3:MUL level: %zu,  target scale: %0.7lf\n",res.level(),res.scale());
        printf("4:new_targetLevel level: %d,  target scale: %0.7lf\n",new_targetLevel,targetScale_new);

        Ciphertext tmp;
        recurse(monomialBasis, relinKeys, res.level(), res.scale(), coeffsr, logSplit, logDegree, tmp, encoder,
                isOdd, isEven, num);
        //tmp.metaData()->setScalingFactor(targetScale_new);
        printf("########### title[%zu]\n",coeffsr.polys()[0].degree());

        rescale_dynamic(tmp, tmp, res.scale());
        gmp_printf("5:tmp level: %d,  target scale: %0.7lf\n",tmp.level(),tmp.scale());
        gmp_printf("5:res level: %d,  target scale: %0.7lf\n",res.level(),res.scale());
        add_dynamic(res, tmp, destination,encoder);



    }
    tuple<uint32_t, double>
    CKKSEvaluatorBase::pre_scalar_level(bool is_even, bool is_odd, const map<uint32_t, Ciphertext> &monomialBasis,
                                        double currentScale, uint32_t currentLevel, const PolynomialVector &pol,
                                        uint32_t log_split, uint32_t log_degree) const {

        auto X = monomialBasis;
        auto &slotsIndex = pol.index();
        auto minimumDegreeNonZeroCoefficient = pol.polys()[0].data().size() - 1;

        auto targetScale = currentScale;
        auto targetLevel = currentLevel;
        auto params = context_.parameters_literal();
        auto &modulus = params->Q();
        auto degree = params->degree();
        auto slots = degree >> 1;
        if (is_even)
            minimumDegreeNonZeroCoefficient--;

        size_t maximumCiphertextDegree = 0;
        for (int i = pol.polys()[0].degree(); i > 0; i--) {
            if (X.count(i)) {
                maximumCiphertextDegree = max(maximumCiphertextDegree, X.at(i).level());
            }
        }
        // If an index slot is given (either multiply polynomials or masking)
        if (!slotsIndex.empty()) {
            bool toEncode = false;
            // Allocates temporary buffer for coefficients encoding

            // If the degree of the poly is zero
            if (minimumDegreeNonZeroCoefficient == 0) {
                //throw ("minimumDegreeNonZeroCoefficient is zero");

                while (1) {

                    if (targetScale >= min_scale_) {
                        break;
                    } else {
                        throw invalid_argument("why!");
//                            targetLevel++;
                        //mpf_mul_ui(targetScale.get_mpf_t(),targetScale.get_mpf_t(),context_.crt_context()->primes_q()[targetLevel]);
//                            targetScale *= safe_cast<double>(modulus[targetLevel].value());
                    }
                }
            } else {
                //mult_plain
                for (int key = pol.polys()[0].degree(); key > 0; key--) {
                    auto reset = false;
                    // Loops over the polynomials
                    for (int i = 0; i < pol.polys().size(); i++) {
                        auto is_not_zero = isNotNegligible(pol.polys()[i].data()[key]);
                        // Looks for a non-zero coefficient
                        if (is_not_zero) {
                            toEncode = true;
                        }
                    }


                    if (toEncode) {
                        Plaintext tmp;
                        double scale;
                        while (1) {
                            scale = targetScale / X[key].scale();
                            if (scale >= min_scale_) {
                                break;
                            } else {
                                targetLevel++;

                                targetScale *= safe_cast<double>(modulus[targetLevel].value());
                            }
                        }

                        toEncode = false;
                    }
                }
            }
        } else {
            throw invalid_argument("slotsIndex is zero");
        }

        return make_tuple(targetLevel, targetScale);
    }

    void CKKSEvaluatorBase::evaluatePolyFromPolynomialBasis(bool is_even, bool is_odd,
                                                            const map<uint32_t, Ciphertext> &monomialBasis,
                                                            const RelinKeys &relinKeys, uint32_t targetLevel,
                                                            double targetScale, const PolynomialVector &pol,
                                                            uint32_t log_split, uint32_t log_degree,
                                                            Ciphertext &destination, const CKKSEncoder &encoder) const {


        auto X = monomialBasis;
        auto &slotsIndex = pol.index();
        auto minimumDegreeNonZeroCoefficient = pol.polys()[0].data().size() - 1;
        auto min_scale = min_scale_;
        auto &id_level_map = context_.crt_context()->parms_id_map();
        auto &parms_id = id_level_map.at(targetLevel);
        //auto min_scale = pol.;
        //mpf_div_2exp(min_scale.get_mpf_t(),min_scale.get_mpf_t(),1);
        auto slots = context_.parameters_literal()->slot();
        vector<complex<double>> values(slots);

        if(is_even)
            minimumDegreeNonZeroCoefficient--;

//        auto maximumCiphertextDegree = 0;
//        for (int i = pol.polys()[0].degree(); i > 0; i--){
//            if (X.count(i))  {
//                maximumCiphertextDegree = max(maximumCiphertextDegree, X.at(i).metaData()->getLevel());
//            }
//        }
        size_t maximumCiphertextDegree = 0;
        for (int i = pol.polys()[0].degree(); i > 0; i--) {
            if (X.count(i)) {
                maximumCiphertextDegree = max(maximumCiphertextDegree, X.at(i).level());
            }
        }

        // If an index slot is given (either multiply polynomials or masking)
        if(!slotsIndex.empty()) {
            bool toEncode = false;
            // Allocates temporary buffer for coefficients encoding

            // If the degree of the poly is zero
            if (minimumDegreeNonZeroCoefficient == 0) {
                //throw ("minimumDegreeNonZeroCoefficient is zero");
                if(!destination.is_valid()){
//                    destination.newMetaData(targetScale,true,targetLevel,degree_);
//                    destination.newPoly(context_,targetLevel+1,0);
                    destination.resize(context_,parms_id,2);
                    destination.is_ntt_form() = true;
                    destination.scale() = targetScale;
                }

                for(int i = 0;  i < pol.polys().size(); i++) {
                    auto aa = pol.polys()[i].data()[0];
                    bool is_not_zero = isNotNegligible(aa);
                    if (is_not_zero) {
                        toEncode = true;
                        for(auto j : slotsIndex[i]) {
                            values[j] = aa;
                        }
                    }
                }

                if (toEncode)
                {
                    toEncode = false;
                    Plaintext tmp;
                    auto level = destination.level();
                    auto degree = destination.poly_modulus_degree();
                    //tmp.newMetaData(scale,false,level,degree);
                    auto &parms_id_tmp = id_level_map.at(level);
                    encoder.encode(values,parms_id_tmp,destination.scale(),tmp);
                    add_plain(destination,tmp,destination);

                }

            }
            else{
                //mult_plain
                for(int key = pol.polys()[0].degree(); key > 0; key--){
                    auto  reset = false;
                    // Loops over the polynomials
                    for(int i = 0; i < pol.polys().size(); i++) {
                        auto is_not_zero = isNotNegligible(pol.polys()[i].data()[key]);
                        // Looks for a non-zero coefficient
                        if (is_not_zero) {
                            toEncode = true;

                            if(!reset){
                                for(int j = 0; j < values.size(); j++) {
                                    values[j] = 0.0;
                                }
                            }

                            for (auto j: slotsIndex[i]) {
                                values[j] = pol.polys()[i].data()[key];
                            }
                        }
                    }


                    if (toEncode)
                    {
                        Plaintext tmp;
                        auto level = X[key].level();
                        auto degree = X[key].poly_modulus_degree();
                        double scale;
                        scale = targetScale / X[key].scale();
                        auto &parms_id_tmp = id_level_map.at(level);
                        encoder.encode(values,parms_id_tmp,scale,tmp);
                        if(!destination.is_valid()){
                            multiply_plain(X.at(key), tmp, destination);
                        }
                        else{
                            Ciphertext ciph;
                            multiply_plain(X.at(key), tmp, ciph);
                            add_dynamic(ciph,destination,destination,encoder);
                        }
                        toEncode = false;
                    }
                }

                for(int j = 0; j < values.size(); j++) {
                    values[j] = 0.0;
                }

                for(int i = 0;  i < pol.polys().size(); i++) {
                    auto aa = pol.polys()[i].data()[0];
                    bool is_not_zero = isNotNegligible(aa);
                    if (is_not_zero) {
                        toEncode = true;
                        for(auto j : slotsIndex[i]) {
                            values[j] = aa;
                        }
                    }
                }

                if (toEncode)
                {
                    Plaintext tmp;
                    auto level = destination.level();

                    auto &parms_id_tmp = id_level_map.at(level);
                    encoder.encode(values,parms_id_tmp,targetScale,tmp);
                    add_plain(destination,tmp,destination);

                }


                destination.scale() = targetScale;
                if(destination.level() < targetLevel){
                    throw logic_error("destination : destination level is small than targetLevel level!");
                }
                else if(targetLevel < destination.level()){
                    drop_modulus(destination,destination,parms_id);
                }
            }
        }
        else{
            throw invalid_argument("slotsIndex is zero");
        }

    }

    void CKKSEvaluatorBase::eval_mod(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
                                     const RelinKeys &relin_key, const CKKSEncoder &encoder)
    {
        if(!ciph.is_valid() ){
            throw invalid_argument("eval_mod : ciphertext is empty!");
        }

        if(ciph.level() != eva_poly.LevelStart()){
            throw invalid_argument("eval_mod : level start not match!");
        }
        result = ciph;

        auto context_data = context_.crt_context()->get_context_data(ciph.parms_id());
        auto poly_modulus_degree = context_data->parms().degree();
        auto slot_num = poly_modulus_degree >> 1;
        auto &coeff_modulus = context_data->coeff_modulus();

        double prevScaleCt = result.scale();
        result.scale() = eva_poly.ScalingFactor();

        set_min_scale(eva_poly.ScalingFactor());
        auto target_scale  = eva_poly.ScalingFactor();
        vector <Polynomial> poly_sin{eva_poly.sinePoly()};
        vector <Polynomial> poly_asin{eva_poly.arcsinPoly()};


        vector<int> idx( slot_num);
        for(int i = 0; i < slot_num; i++){
            idx[i] = i;   // Index with all even slots
        }
        vector<vector<int>> slotsIndex(1,vector<int>(slot_num,0));
        slotsIndex[0] = idx; // Assigns index of all even slots to poly[0] = f(x)

        if (eva_poly.Type() == CosDiscrete || eva_poly.Type() == CosContinuous) {
            double const_data = -0.5 / (eva_poly.ScFac() * (eva_poly.sinePoly_B() - eva_poly.sinePoly_A()));
            cout << "const_data: " << const_data << endl;
            add_const(result, const_data, result, encoder);
        }


        PolynomialVector polys_sin(poly_sin,slotsIndex);
        Ciphertext tmp = result;
        evaluatePolyVector( tmp,result,polys_sin, target_scale, relin_key,encoder);
        // Double angle
        ;

        cout << "===============" << endl;
        auto sqrt2pi = eva_poly.Sqrt2Pi();
        for (auto i = 0; i < eva_poly.DoubleAngle(); i++) {
            sqrt2pi *= sqrt2pi;
            cout << "sqrt2pi: " << sqrt2pi <<endl;
            //if(i != 2)
            multiply_relin_dynamic(result,result,result,relin_key);
//            if(i == 2){
//                cout << result.scale() << endl;
//                multiply(result,result,result);
//                relinearize(result,relin_key,result);
//                return;
//            }
            add(result, result, result);
            add_const(result, -sqrt2pi, result,encoder);
            rescale_dynamic(result,result,target_scale);
        }
        //return;


        //printf("cc : %0.8lf %0.8lf\n",eva_poly.ScalingFactor(),prevScaleCt);
        double diff_scale =  eva_poly.ScalingFactor() / result.scale();
        if(diff_scale < coeff_modulus.back().value()){
            diff_scale *= coeff_modulus[result.level()].value();
            diff_scale *= coeff_modulus[result.level()-1].value();
        }
        multiply_const(result,1.0,diff_scale,result,encoder);
        rescale_dynamic(result,result,eva_poly.ScalingFactor());

        result.scale() = prevScaleCt;
    }

    void CKKSEvaluatorBase::bootstrap(const Ciphertext &ciph, Ciphertext &result, const EvalModPoly &eva_poly,
                                      const LinearMatrixGroup &matrix_group0, const LinearMatrixGroup &matrix_group1,
                                      const RelinKeys &relin_key, const GaloisKeys &galoisKeys,
                                      const CKKSEncoder &encoder)  {
        auto context_data = context_.crt_context()->get_context_data(ciph.parms_id());
        auto &params = context_data->parms();
        auto q0_level = params.q0_level();


        result = ciph;
        uint32_t bootstrap_ratio = eva_poly.MessageRatio();
//        uint32_t bootstrap_ratio = pow(2,13);
        double prevScaleCt = ciph.scale();
        double q0OverMessageRatio = context_.crt_context()->q0();
        q0OverMessageRatio = exp2(round(log2(q0OverMessageRatio /  (double)bootstrap_ratio)));
//        mpf_class new_scaling_factor = context_.crt_context()->q0();
//        gmp_printf("bootstrap q0_level: %d   q0: %.5Ff\n",q0_level,new_scaling_factor.get_mpf_t());
//        mpf_div_ui(new_scaling_factor.get_mpf_t(),new_scaling_factor.get_mpf_t(),bootstrap_ratio);
        auto level = result.level();
        auto level_diff = level - q0_level;

        if(level_diff > 1){
            auto parms_id = context_.crt_context()->parms_id_map().at(q0_level + 1);
            drop_modulus(result, result, parms_id);
        }

        printf("result scale: %.7f\n",result.scale());
//        printf("bootstrap_ratio scale: %.7f\n",bootstrap_ratio);

        //set_scale(ciph_tmp,mpf_class(q0OverMessageRatio),ciph_tmp);
        auto scale = q0OverMessageRatio / result.scale();
        printf("result scale: %.7f\n",scale);

        scale = round(scale);
        printf("scale: %.7f\n",scale);
        if(scale > 1){
            multiply_const_direct(result, safe_cast<int>(scale) ,result,encoder);
            result.scale() *= scale;
        }

        auto parms_id = context_.crt_context()->parms_id_map().at(q0_level);
        drop_modulus(result, result, parms_id);


        Ciphertext ciph_raise;
        read(result);
        raise_modulus(result,ciph_raise);


        auto scale_raise = eva_poly.ScalingFactor()/ ciph_raise.scale();
        scale_raise /=  eva_poly.MessageRatio();
        //scale_raise = round(scale_raise);
        printf("scale: %7f\n",scale_raise);
        if(scale_raise > 1 && scale_raise < 0x7FFFFFFF){
            cout << 111 << endl;
            multiply_const_direct(ciph_raise, safe_cast<int>(scale_raise) ,ciph_raise,encoder);
//            double cc = ciph_raise.scale() * scale_raise;
            ciph_raise.scale() *= scale_raise;
        }
        else if(scale_raise > 0x7FFFFFFF){
            cout << 222 << endl;
            multiply_const(ciph_raise, 1.0 ,scale_raise,ciph_raise,encoder);
        }
        else{

        }

//        result = ciph_raise;
//        return;



        Ciphertext res_real,res_imag;
        auto start = chrono::high_resolution_clock::now();

        coeff_to_slot(ciph_raise,matrix_group0,res_real,res_imag,galoisKeys,encoder);
//        result = res_real;
//        return;
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "coeff_to_slot EXP TIME: " << duration.count() << " microseconds"<< endl;


        Ciphertext res_real1,res_imag1;
        eval_mod(res_imag,res_imag1,eva_poly,relin_key,encoder);


        eval_mod(res_real,res_real1,eva_poly,relin_key,encoder);



        res_imag1.scale() = context_.parameters_literal()->scale();
        res_real1.scale() = context_.parameters_literal()->scale();
//
//
////        multiply_const(cipherA,(double)1/(evalModPoly.QDiff()*(evalModPoly.K())),cipherA);//
////        rescale(cipherA);
//
//
        Ciphertext res;
        cout << "======" << res_real1.level() << endl;
        slot_to_coeff(res_real1,res_imag1,matrix_group1,result,galoisKeys,encoder);


    }


}


