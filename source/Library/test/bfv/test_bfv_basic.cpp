
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

    ParametersLiteral bfv_param_literal{
            BFV,
            11,
            11,
            40,
            5,
            0,
            65537,
            {},
            {}
    };
    vector<uint32_t> logQTmp{31,31,31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31};//,31,31,31,31}; //
    bfv_param_literal.set_log_modulus(logQTmp,logPTmp);

    PoseidonContext context(bfv_param_literal,poseidon::sec_level_type::none,false);
    BatchEncoder enc(context);
    KeyGenerator keygen(context);
    PublicKey publicKey;
    GaloisKeys galoisKeys;
    RelinKeys relinKeys;
    keygen.create_public_key(publicKey);
    keygen.create_galois_keys(vector<int>{0,-1,1},galoisKeys);
    keygen.create_relin_keys(relinKeys);

    Encryptor encryptor(context,publicKey);
    Decryptor decryptor(context,keygen.secret_key());

    Plaintext plainA,plainB,plain_res;
    Ciphertext ciphA,ciphB;
    auto slot_num = bfv_param_literal.slot();
    vector<uint64_t > a = {55,2,3};
    vector<uint64_t > b = {11,33,22};
    vector<uint64_t > message_res;

    enc.encode(a,plainA);
    enc.encode(b,plainB);
    encryptor.encrypt(plainA,ciphA);
    encryptor.encrypt(plainB,ciphB);

    Timestacs timestacs;
    auto eva = EvaluatorFactory::SoftFactory()->create(context);
    //======================== ADD =========================================
    print_example_banner("Example: ADD / ADD in bfv");
    timestacs.start();
    eva->add(ciphA,ciphB,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);
    auto message_want = a;
    for(auto i = 0; i < message_want.size(); i++){
        message_want[i] += b[i];
    }
    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }




    //======================== SUB =========================================
    print_example_banner("Example: SUB / SUB in bfv");
    timestacs.start();
    eva->sub(ciphA,ciphB,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++){
        message_want[i] -= b[i];
    }
    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }

    //======================== Mod Switch =========================================
    print_example_banner("Example: Mod Switch / Mod Switch in bfv");
    cout << "Before Mod Switch level : " << ciphA.level() << endl;
    timestacs.start();
    eva->drop_modulus_to_next(ciphA,ciphA);
    timestacs.end();
    cout << "After  Mod Switch level : " << ciphA.level() << endl;
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }
//======================== multiply =========================================
    print_example_banner("Example: MULTIPLY / MULTIPLY in bfv");
    timestacs.start();
    eva->multiply(ciphA,ciphA,ciphA);
    eva->relinearize(ciphA,relinKeys,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++){
        message_want[i] *= a[i];
    }
    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }
    for(auto i = 0; i < 10; ++i){
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }
    //======================== rotate row =========================================
    print_example_banner("Example: ROTATE_ROW / ROTATE_ROW in bfv");
    timestacs.start();
    eva->rotate_row(ciphA,1,galoisKeys,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }
    eva->rotate_row(ciphA,-1,galoisKeys,ciphA);
    //======================== rotate col =========================================
    print_example_banner("Example: ROTATE_COL / ROTATE_COL in bfv");
    timestacs.start();
    eva->rotate_col(ciphA,galoisKeys,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }
    eva->rotate_col(ciphA,galoisKeys,ciphA);

    //======================== ADD_PLAIN =========================================
    print_example_banner("Example: ADD_PLAIN / ADD_PLAIN in bfv");
    timestacs.start();
    eva->add_plain(ciphA,plainA,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++){
        message_want[i] += a[i];
    }
    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }



    //======================== MULTIPLY_PLAIN =========================================
    print_example_banner("Example: MULTIPLY_PLAIN / MULTIPLY_PLAIN in bfv");
    timestacs.start();
    eva->multiply_plain(ciphA,plainA,ciphA);
    timestacs.end();
    timestacs.print_time("TIME : ");
    decryptor.decrypt(ciphA,plain_res);
    enc.decode(plain_res,message_res);

    for(auto i = 0; i < message_want.size(); i++){
        message_want[i] *= a[i] ;
        message_want[i] %= 65537;
    }
    for(auto i = 0; i < message_want.size(); i++) {
        printf("source_data[%d] : %ld\n", i, message_want[i]);
        printf("result_data[%d] : %ld\n", i, message_res[i]);
    }


    return 0;
}