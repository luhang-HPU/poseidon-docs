
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

#include "poseidon/keyswitch/keyswitch_bv.h"
#include "poseidon/RNSPoly.h"
#include "poseidon/CKKSEncoder.h"
using namespace std;
using namespace poseidon;

int main() {

    cout << BANNER  << endl;
    cout << "POSEIDON SOFTWARE  VERSION:" <<POSEIDON_VERSION << endl;
    cout << "" << endl;


    uint32_t q0_bit = 55;
    auto q_def = 44;

    ParametersLiteral ckks_param_literal{
            CKKS,
            11,
            10,
            44,
            5,
            1,
            0,
            {},
            {}
    };


    vector<uint32_t> logQTmp{28,27,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31};//,31,31,31,31}; //
    vector<uint32_t> logPTmp{31,31,31,31,31,31,31,31,31,31,31}; //,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31,  31,31,31,31,31,31,31,31,31,31
    //vector<uint32_t> logPTmp{60};//,
    ckks_param_literal.set_log_modulus(logQTmp,logPTmp);
    PoseidonContext context(ckks_param_literal,poseidon::sec_level_type::none,false);
    auto q0 = context.crt_context()->q0();




    //=====================init random data ============================
    std::vector<std::complex<double>> vec;
    std::vector<std::complex<double>> vec_result,vec_result1;
    std::vector<vector<std::complex<double>>> mat;
    int mat_size = 1 << ckks_param_literal.LogSlots();
    mat.resize(mat_size);
    //create message
    vector<complex<double>> message;
    vector<complex<double>> message1;
    sample_random_complex_vector(message, mat_size);
    for(auto &m : message){
        m = sin(m);
    }

    //=====================init  Plain & Ciph =========================
    Plaintext plainA,plainB,plainRes,plainRes1,plainT;
    Ciphertext cipherA,cipherB,cipherRes,cipherRes1,cipherRes2,cipherRes3;
    PublicKey public_key;
    RelinKeys relinKeys;
    GaloisKeys rotKeys;
    GaloisKeys conjKeys;
    vector<uint32_t> rot_elemt;
    //

    CKKSEncoder ckks_encoder(context);

    //=====================EvalMod  ========================
    auto level_start = ckks_param_literal.Q().size() - 1;

    EvalModPoly evalModPoly(context, CosDiscrete,(uint64_t)1 << 60 ,level_start-6,9, 3,16,0,30);
    auto scFac = evalModPoly.ScFac();
    double K = evalModPoly.K();
    auto qDiff = evalModPoly.QDiff();
    auto q0OverMessageRatio = exp2(round(log2((double)q0 / (double)evalModPoly.MessageRatio()) ) ) ;
    // If the scale used during the EvalMod step is smaller than Q0, then we cannot increase the scale during
    // the EvalMod step to get a free division by MessageRatio, and we need to do this division (totally or partly)
    // during the CoeffstoSlots step

    auto CoeffsToSlotsScaling = 1.0;
    CoeffsToSlotsScaling *= evalModPoly.qDiv() / (K * scFac * qDiff);

    auto SlotsToCoeffsScaling = ckks_param_literal.scale();
    SlotsToCoeffsScaling = SlotsToCoeffsScaling / ((double)evalModPoly.ScalingFactor() / (double)evalModPoly.MessageRatio() );
    //auto SlotsToCoeffsScaling = 1;


    HomomorphicDFTMatrixLiteral d(0, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(), level_start, vector<uint32_t>(3,1), true, CoeffsToSlotsScaling, false, 1);
    HomomorphicDFTMatrixLiteral x(1, ckks_param_literal.LogN(), ckks_param_literal.LogSlots(),  11 , vector<uint32_t>(3,1), true, SlotsToCoeffsScaling, false, 1);
    LinearMatrixGroup mat_group;
    LinearMatrixGroup mat_group_dec;
    d.create(mat_group,ckks_encoder,2);
    x.create(mat_group_dec,ckks_encoder,2);





    //=====================keys  =========================
    KeyGenerator kgen(context);
    kgen.create_public_key(public_key);
    kgen.create_relin_keys(relinKeys);
    //rotKeys = kgen.create_galois_keys(mat_group.rot_index(),kgen.secret_key());
    //GaloisKeys rotKeys;
    kgen.create_galois_keys(mat_group.rot_index(),rotKeys);
    Encryptor enc(context,public_key,kgen.secret_key());
    Decryptor dec(context,kgen.secret_key());

    //===================== Doing ==============================
    //encode
    ckks_encoder.encode(message,(int64_t) 1 << q_def ,plainA);
    //encrypt
    enc.encrypt(plainA,cipherA);

    //evaluate
    auto ckks_eva = EvaluatorFactory::SoftFactory()->create(context);


    // scale the message to Delta = Q/MessageRatio

    auto start = chrono::high_resolution_clock::now();
    //ckks_eva->multiply_const(cipherA,10,cipherA);
    //ckks_eva->multiply(cipherA,cipherA,cipherA,relinKeys);
    cout << "bootstraping start..."<< endl;

    ckks_eva->bootstrap(cipherA,cipherRes,evalModPoly,mat_group,mat_group_dec,relinKeys,rotKeys,ckks_encoder);



    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "EXP TIME: " << duration.count() << " microseconds"<< endl;

    //decode & decrypt

    dec.decrypt(cipherRes,plainRes);

    ckks_encoder.decode(plainRes,vec_result);


    for(int i = 0; i < 10; i++){

        printf("source vec[%d] : %0.10f + %0.10f I \n",i,(real(message[i])), imag(message[i]));
        printf("result vec[%d] : %0.10f + %0.10f I \n",i,(real(vec_result[i])), imag(vec_result[i]));
    }
    GetPrecisionStats(vec_result,message);


    return 0;
}
