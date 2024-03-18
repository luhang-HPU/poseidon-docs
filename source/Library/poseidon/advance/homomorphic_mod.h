//
// Created by lxs on 23-6-29.
//

#ifndef POSEIDON_HOMOMORPHIC_MOD_H
#define POSEIDON_HOMOMORPHIC_MOD_H
#include "poseidon/PoseidonContext.h"
#include "poseidon/advance/polynomial_evaluation.h"
#include "poseidon/advance/util/chebyshev_interpolation.h"
#include "poseidon/advance/util/cosine_approx.h"
#include "poseidon/seal/util/common.h"



#include <utility>
namespace poseidon {


    int optimalSplit(int logDegree);
    bool isNotNegligible(complex<double> c) ;
    pair<bool,bool> isOddOrEvenPolynomial(Polynomial &coeffs);


    class EvalModPoly {
    public:
        EvalModPoly(const PoseidonContext &context,SineType type,double scaling_factor,uint32_t levelStart,uint32_t logMessageRatio,uint32_t doubleAngle,uint32_t K,uint32_t arcSineDegree,uint32_t SineDegree);


        inline static complex<double> sin2pi2pi(const complex<double> &x )  {
            return sin(6.283185307179586 * x) ;
        }


        inline SineType Type() const{
            return this->type_;
        }

        inline int K() const{
            return this->K_;
        }

        inline double qDiv() const{
            return this->q_div_;
        }

        inline int LevelStart() const{
            return this->levelStart_;
        }

        inline  double ScalingFactor() const {
            return this->scaling_factor_;
        }

        inline  uint32_t MessageRatio() const {
            return  1 << this->logMessageRatio_;
        }

        inline  uint32_t DoubleAngle() const {
            return this->doubleAngle_;
        }

        inline  double QDiff() const {
            return this->qDiff_;
        }

        inline  double ScFac() const {
            return this->scFac_;
        }

        inline  double Sqrt2Pi() const {
            return this->sqrt2Pi_;
        }

        inline int32_t sinePoly_A() const{
            return sinePoly_A_;
        }
        inline int32_t sinePoly_B() const{
            return sinePoly_B_;
        }
        inline const Polynomial &sinePoly()const{
            return this->sinePoly_;
        }

        inline const Polynomial &arcsinPoly() const{
            return this->arcSinePoly_;
        }

    private:
        // Member variables
        SineType type_;
        double scaling_factor_;
        uint32_t levelStart_;
        uint32_t logMessageRatio_;
        uint32_t doubleAngle_;
        uint32_t K_;
        double q_div_;
        double qDiff_;
        double scFac_;
        double sqrt2Pi_;
        Polynomial sinePoly_;
        int32_t sinePoly_A_;
        int32_t sinePoly_B_;

        Polynomial arcSinePoly_;
    };
}






// poseidon

#endif //POSEIDON_HOMOMORPHIC_MOD_H
