//
// Created by lxs on 23-6-29.
//

#include "poseidon/advance/homomorphic_mod.h"

using namespace poseidon::util;
namespace poseidon {
    EvalModPoly::EvalModPoly(const PoseidonContext &context,SineType type,double scaling_factor,uint32_t levelStart,
                             uint32_t logMessageRatio,uint32_t doubleAngle,uint32_t K,uint32_t arcSineDegree,uint32_t SineDegree)
            :type_(type),scaling_factor_(scaling_factor),levelStart_(levelStart),logMessageRatio_(logMessageRatio)
    {
        this->doubleAngle_ = doubleAngle;
        if(type == SinContinuous)
            this->doubleAngle_ = 0;


        this->scFac_ = exp2((double)this->doubleAngle_);
        this->K_ = (double)K / scFac_;
        auto Q = context.crt_context()->q0();

        this->qDiff_ = Q / exp2(round(log2(Q )));
        this->q_div_ = (double)scaling_factor_ / exp2(round(log2(Q)));
        if(q_div_ > 1){
            q_div_ = 1;
        }
        if(arcSineDegree > 0){
            this->sqrt2Pi_ = 1.0;
            vector<complex<double>> arcBuffer;

            arcBuffer.resize(arcSineDegree + 1);
            arcBuffer[1] = 0.15915494309189535 * complex<double>(qDiff_, 0);
            //arcBuffer[1] = complex<double>(1, 0);

            for(int i = 3; i < arcSineDegree + 1; i +=2){
                arcBuffer[i] = arcBuffer[i-2] * complex<double>((double)(i*i-4*i+4)/(double)(i*i-i), 0);
            }
            arcSinePoly_.data() = arcBuffer;
            arcSinePoly_.lead() = true;
            arcSinePoly_.a() = 0;
            arcSinePoly_.b() = 0;
            arcSinePoly_.max_degree() = arcSineDegree;
        }
        else{
            //this->sqrt2Pi_ = pow(0.15915494309189535*qDiff_,1.0/scFac_);
            this->sqrt2Pi_ = pow(1/(2*M_PIl)*qDiff_,1.0/scFac_);

        }

        switch (type_) {
            case SinContinuous:
                sinePoly_ = Approximate(sin2pi2pi, -K, K, SineDegree);
                sinePoly_.lead() = true;
                sinePoly_A_ = -K_;
                sinePoly_B_ = K_;

                break;
            case CosDiscrete:
                sinePoly_.lead() = true;
                sinePoly_A_ = -K_;
                sinePoly_B_ = K_;
                sinePoly_.lead() = true;

                sinePoly_.a() = -K_;
                sinePoly_.b() = K_; //this K_ is the  size of doubleAngle
                sinePoly_.basis_type() = Chebyshev;
                sinePoly_.data() = ApproximateCos(K, SineDegree, (double)(1<<logMessageRatio), doubleAngle); //this K is total size
                sinePoly_.max_degree() = sinePoly_.data().size() - 1;
                break;

            case CosContinuous:
                exit(0);

        }

        for (int i = 0; i < sinePoly_.data().size(); i++) {
            this->sinePoly_.data()[i] *= complex<double>(sqrt2Pi_, 0);
        }

    }



    int optimalSplit(int logDegree) {
        int logSplit = logDegree >> 1;
//        auto a = (1 << logSplit) + (1 << (logDegree - logSplit)) + logDegree - logSplit - 3;
//        auto b = (1 << (logSplit + 1)) + (1 << (logDegree - logSplit - 1)) + logDegree - logSplit - 4;
        if (logDegree - logSplit > logSplit) {
            logSplit++;
        }
        return logSplit;
    }

    bool isNotNegligible(complex<double> c) {
        if (abs(real(c)) > util::IsNegligibleThreshold || abs(imag(c)) > util::IsNegligibleThreshold){
            return true;
        }
        else{
            return false;
        }
    }
    pair<bool,bool> isOddOrEvenPolynomial(Polynomial &poly) {
        bool even = true;
        bool odd = true;
        auto poly_degree = poly.max_degree();
        auto &data = poly.data();

        for(int i = 0; i < poly_degree; i++){
            auto isnotnegligible = isNotNegligible(data[i]);

            auto state = i & 1;

            odd = odd && (state != 0 && isnotnegligible);
            even = even && (state != 1 && isnotnegligible);
            if (!odd && !even) {
                break;
            }
        }
        // If even or odd, then sets the expected zero coefficients to zero
        if (even || odd) {
            int start = 0;
            if(even) {
                start = 1;
            }
            for (int i = start; i < poly_degree; i += 2) {
                poly.data()[i] = complex<double>(0, 0);
            }
        }
        return make_pair(odd,even);
    }


} // poseidon