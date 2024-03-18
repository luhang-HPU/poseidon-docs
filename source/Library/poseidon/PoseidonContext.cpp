#include "poseidon/PoseidonContext.h"
#include "poseidon/util/exception.h"
namespace poseidon{
    PoseidonContext::PoseidonContext(const ParametersLiteral& paramLiteral,sec_level_type sec_level,bool using_hardware)
            :parameters_literal_(make_shared<const poseidon::ParametersLiteral>(paramLiteral)),sec_level_(sec_level),using_hardware_(using_hardware)
    {

        if(using_hardware_){
            POSEIDON_THROW(config_error,"don't support hardware");
        }

        auto base_p_size = paramLiteral.P().size();
        auto base_q_size = paramLiteral.Q().size();
        if(base_p_size < 1){
            key_switch_variant_ = none;
        }
        else if(base_p_size == base_q_size){
            key_switch_variant_ = GHS;
        }
        else if(base_p_size == 1){
            key_switch_variant_ = BV;
        }
        else{
            key_switch_variant_ = HYBRID;
        }

        crt_context_ = make_shared<poseidon::CrtContext>(parameters_literal_,sec_level_);
        if(using_hardware){
            if(key_switch_variant_ == BV){
                throw invalid_argument("hardware don't support BV variant");
            }
            else if(key_switch_variant_ == HYBRID){
                throw invalid_argument("hardware don't support HYBRID variant");
            }
        }

        auto rng = make_shared<Blake2xbPRNGFactory>(Blake2xbPRNGFactory());
        set_random_generator(rng);

    }
}
