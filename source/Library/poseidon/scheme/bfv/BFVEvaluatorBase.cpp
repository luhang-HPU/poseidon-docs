//
// Created by lxs on 24-2-20.
//

#include "poseidon/scheme/bfv/BFVEvaluatorBase.h"

namespace poseidon{
    void BFVEvaluatorBase::drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const{
        auto parms_id = context_.crt_context()->parms_id_map().at(level);
        drop_modulus(ciph,result,parms_id);
    }

    void BFVEvaluatorBase::drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const{
        auto level = ciph.level();
        auto parms_id = context_.crt_context()->parms_id_map().at(level-1);
        drop_modulus(ciph,result,parms_id);
    }


}
