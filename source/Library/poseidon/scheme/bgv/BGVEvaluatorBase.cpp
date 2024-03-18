
#include "poseidon/scheme/bgv/BGVEvaluatorBase.h"

namespace poseidon{
    void BGVEvaluatorBase::drop_modulus(const Ciphertext &ciph, Ciphertext &result, uint32_t level) const{
        auto parms_id = context_.crt_context()->parms_id_map().at(level);
        drop_modulus(ciph,result,parms_id);
    }

    void BGVEvaluatorBase::drop_modulus_to_next(const Ciphertext &ciph, Ciphertext &result) const{
        auto level = ciph.level();
        auto parms_id = context_.crt_context()->parms_id_map().at(level-1);
        drop_modulus(ciph,result,parms_id);
    }


}
