
#include <bits/stdc++.h>

#include "poseidon/seal/modulus.h"

#include "poseidon/PoseidonContext.h"
#include "poseidon/batchencoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/debug.h"
#include "poseidon/key/keys.h"
#include "poseidon/keygenerator.h"
#include "poseidon/Evaluator.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteral ckks_param_literal{
            BGV,
            14,
            14,
            40,
            5,
            0,
            65537,
            {},
            {}
    };
    vector<uint32_t> logQTmp{31,31,31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{60};//,31,31,31,31}; //
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);

    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none,false);
    BatchEncoder enc(context);
    KeyGenerator keygen(context);
    PublicKey publicKey;
    keygen.create_public_key(publicKey);
    Encryptor encryptor(context,publicKey);
    Decryptor decryptor(context,keygen.secret_key());

    Plaintext plain,plain_res;
    Ciphertext ciph;
    auto slot_num = ckks_param_literal.slot();
    vector<uint64_t > a = {55,2,3};
    vector<uint64_t > b;
    enc.encode(a,plain);
    encryptor.encrypt(plain,ciph);
    decryptor.decrypt(ciph,plain_res);
    enc.decode(plain_res,b);


    cout << b[0] << endl;
    cout << b[1] << endl;
    cout << b[2] << endl;
    cout << b[3] << endl;





    return 0;
}
