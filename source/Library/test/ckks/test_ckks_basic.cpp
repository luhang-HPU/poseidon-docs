
#include <bits/stdc++.h>

#include "poseidon/seal/modulus.h"
#include "poseidon/seal/memorymanager.h"
#include "poseidon/seal/util/globals.h"
#include "poseidon/seal/util/ntt.h"
#include "poseidon/PoseidonContext.h"
#include "poseidon/CKKSEncoder.h"
#include "poseidon/plaintext.h"
#include "poseidon/util/random_sample.h"
#include "poseidon/encryptor.h"
#include "poseidon/decryptor.h"
#include "poseidon/keygenerator.h"
#include "poseidon/util/precision.h"
#include "poseidon/Evaluator.h"
#include "poseidon/util/debug.h"
using namespace std;

using namespace poseidon;
using namespace poseidon::util;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;

    ParametersLiteralDefault aa(CKKS,16384,poseidon::sec_level_type::tc192);
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys galoisKeys;
    PoseidonContext context(aa,sec_level_type::tc192,false);
    CKKSEncoder enc(context);
    KeyGenerator keygen(context);
    keygen.create_public_key(public_key);

    keygen.create_relin_keys(relinKeys);
    keygen.create_galois_keys(vector<int>{0,1},galoisKeys);

    Encryptor encryptor(context,public_key);
    Decryptor decryptor(context,keygen.secret_key());
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);

    auto slot_num = aa.slot();
    vector<complex<double>> message,message2;
    vector<complex<double>>message_want(slot_num);
    vector<complex<double>> message_res;

    sample_random_complex_vector(message,slot_num);
    sample_random_complex_vector(message2,slot_num);

    Plaintext plaintext,plaintext2,plaintext_res;
    double scale = std::pow(2.0,40);
    enc.encode(message, scale,plaintext);
    enc.encode(message2, scale,plaintext2);

    Ciphertext ct,ct2,ct_res;
    encryptor.encrypt(plaintext,ct);
    encryptor.encrypt(plaintext2,ct2);

    Timestacs timestacs;
//======================== ADD =========================================
    print_example_banner("Example: ADD / ADD in ckks");
    timestacs.start();
    ckks_eva->add(ct,ct2,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);
    timestacs.print_time("ADD TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);

    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] += message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


//======================== SUB =========================================
    print_example_banner("Example: SUB / SUB in ckks");
    timestacs.start();
    ckks_eva->sub(ct,ct2,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("SUB TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] -= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== ADD_PLAIN =========================================
    print_example_banner("Example: ADD_PLAIN / ADD_PLAIN in ckks");
    timestacs.start();
    ckks_eva->add_plain(ct,plaintext_res,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("ADD_PLAIN TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] += message[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== ROTATE =========================================
    print_example_banner("Example: ROTATE / ROTATE in ckks");
    timestacs.start();
    ckks_eva->rotate(ct,1,galoisKeys,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("ROTATE TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] = message[i];
    }
    std::rotate(message_want.begin(),message_want.begin()+1,message_want.end());

    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== conj =========================================
    print_example_banner("Example: CONJUGATE / CONJUGATE in ckks");
    timestacs.start();
    ckks_eva->conjugate(ct,galoisKeys,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);

    timestacs.print_time("CONJUGATE TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] = std::conj(message[i]);
    }


    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== MULT_PLAIN =========================================
    print_example_banner("Example: MULT_PLAIN / MULT_PLAIN in ckks");
    timestacs.start();
    ckks_eva->multiply_plain(ct,plaintext_res,ct_res);
    timestacs.end();
    ckks_eva->read(ct_res);
    ckks_eva->rescale_dynamic(ct_res,ct_res,scale);

    timestacs.print_time("MULT_PLAIN TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

//======================== MULTIPLY =========================================
    print_example_banner("Example: MULTIPLY / MULTIPLY in ckks");
    timestacs.start();
    ckks_eva->multiply(ct,ct2,ct_res);
    timestacs.end();

    timestacs.print_time("MULTIPLY TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== RELIN =========================================
    print_example_banner("Example: RELIN / RELIN in ckks");
    timestacs.start();
    ckks_eva->relinearize(ct_res,relinKeys,ct_res);
    timestacs.end();
    timestacs.print_time("RELIN TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== RESCALE =========================================
    print_example_banner("Example: RESCALE / RESCALE in ckks");
    timestacs.start();
    ckks_eva->rescale_dynamic(ct_res,ct_res,scale);
    timestacs.end();

    timestacs.print_time("RESCALE TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    message_want = message;
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);

    //======================== MULTIPLY_RELIN_DYNAMIC =========================================
    print_example_banner("Example: MULTIPLY_RELIN_DYNAMIC / MULTIPLY_RELIN_DYNAMIC in ckks");
    timestacs.start();
    ckks_eva->multiply_relin_dynamic(ct_res,ct2,ct_res,relinKeys);
    //ckks_eva->rescale_dynamic(ct_res,ct_res,install.scale());
    timestacs.end();
    timestacs.print_time("MULTIPLY_RELIN_DYNAMIC TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== MULT_CONST =========================================
    print_example_banner("Example: MULT_CONST_DIRECT/ MULT_CONST_DIRECT in ckks");
    timestacs.start();
    ckks_eva->multiply_const_direct(ct_res,1,ct_res,enc);
    timestacs.end();
    timestacs.print_time("MULT_CONST_DIRECT TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= 1;
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);


    //======================== MULT_CONST =========================================
    print_example_banner("Example: MULT_CONST / MULT_CONST in ckks");
    timestacs.start();
    cout << ct_res.level() << endl;
    cout << ct_res.scale() << endl;

    ckks_eva->multiply_const(ct_res,-0.7, pow(2,40),ct_res,enc);
    timestacs.end();
    timestacs.print_time("MULT_CONST TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] *= -0.7;
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);
//======================== ADD_DYNAMIC =========================================

    print_example_banner("Example: ADD_DYNAMIC / ADD_DYNAMIC in ckks");
    timestacs.start();
    ckks_eva->add_dynamic(ct_res,ct2, ct_res,enc);
    timestacs.end();
    timestacs.print_time("ADD_DYNAMIC TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] += message2[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);
    //======================== SUB_DYNAMIC =========================================

    print_example_banner("Example: SUB_DYNAMIC / SUB_DYNAMIC in ckks");
    timestacs.start();
    ckks_eva->sub_dynamic(ct_res,ct, ct_res,enc);
    timestacs.end();
    timestacs.print_time("SUB_DYNAMIC TIME:");
    decryptor.decrypt(ct_res,plaintext_res);
    enc.decode(plaintext_res,message_res);
    //message_want = message
    for(auto i = 0; i < slot_num; i++){
        message_want[i] -= message[i];
    }
    for(auto i = 0; i < 4; i++){
        printf("source_data[%d] : %.10lf + %.10lf I\n",i,message_want[i].real() ,message_want[i].imag());
        printf("result_data[%d] : %.10lf + %.10lf I\n",i,message_res[i].real() ,message_res[i].imag());
    }
    util::GetPrecisionStats(message_want,message_res);
    return 0;
}
