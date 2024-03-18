//
// Created by lxs on 24-2-20.
//

#include "poseidon/scheme/bfv/Software/BFVEvaluator_S.h"
#include "poseidon/seal/util/scalingvariant.h"
namespace poseidon{
    void BFVEvaluator_S::ftt_fwd(const Plaintext &plain ,Plaintext &result) const{
        ftt_fwd_b(plain,result);
    }
    void BFVEvaluator_S::ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const {
        ftt_fwd_b(ciph,result);
    }
    void BFVEvaluator_S::ftt_inv(const Plaintext &plain ,Plaintext &result) const {
        ftt_inv_b(plain,result);
    }
    void BFVEvaluator_S::ftt_inv(const Ciphertext &ciph ,Ciphertext &result) const {
        ftt_inv_b(ciph,result);
    }
    void BFVEvaluator_S::add(const Ciphertext &ciphertext1,const  Ciphertext &ciphertext2,Ciphertext &result) const {
        if(&result == &ciphertext1){
            add_inplace(result,ciphertext2);
        }
        else{
            result = ciphertext2;
            add_inplace(result, ciphertext1);
        }
    }
    void BFVEvaluator_S::sub(const Ciphertext &ciphertext1,const  Ciphertext &ciphertext2,Ciphertext &result) const {
        if(!ciphertext1.is_valid() || !ciphertext2.is_valid()){
            throw invalid_argument("sub : ciphertext1 and ciphertext2 parameter mismatch");
        }
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("sub : ciphertext1 and ciphertext2 parameter mismatch");
        }
        if (ciphertext1.is_ntt_form() != ciphertext2.is_ntt_form())
        {
            throw invalid_argument("sub : NTT form mismatch");
        }
        if (!util::are_close<double>(ciphertext1.scale(), ciphertext2.scale()))
        {
            throw invalid_argument("sub : scale mismatch");
        }

        // Extract encryption parameters.
        auto &context_data = *context_.crt_context()->get_context_data(ciphertext1.parms_id());
        auto &parms = context_data.parms();
        auto &coeff_modulus = context_data.coeff_modulus();
        //auto &plain_modulus = parms.plain_modulus();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = coeff_modulus.size();
        size_t ciphertext1_size = ciphertext1.size();
        size_t ciphertext2_size = ciphertext1.size();
        size_t max_count = max(ciphertext1_size, ciphertext2_size);
        size_t min_count = min(ciphertext1_size, ciphertext2_size);

        // Size check
        if (!product_fits_in(max_count, coeff_count))
        {
            throw logic_error("invalid parameters");
        }

        // Prepare result
        result.resize(context_,ciphertext1.parms_id(),ciphertext1.size());
        result.is_ntt_form() = ciphertext1.is_ntt_form();
        for(auto i = 0; i < min_count; i++){
            ciphertext1[i].sub(ciphertext2[i],result[i]);
        }
        // Copy the remainding polys of the array with larger count into ciphertext1
        if(ciphertext1_size < ciphertext2_size)
        {
            for(auto i = min_count; i < max_count; ++i){
                result[i].copy(ciphertext2[i]);
            }
        }
    }
    void BFVEvaluator_S::add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        add_plain_inplace(result, plain);
    }

    void BFVEvaluator_S::sub_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        sub_plain_inplace(result, plain);
    }
    void BFVEvaluator_S::multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        multiply_plain_inplace(result, plain);
    }
    void BFVEvaluator_S::multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const {
        if (&ciph2 == &result)
        {
            multiply_inplace(result, ciph1);
        }
        else
        {
            result = ciph1;
            multiply_inplace(result, ciph2);
        }
    }

    void BFVEvaluator_S::square_inplace( Ciphertext &ciph) const{
        multiply_inplace(ciph,ciph);
    }

    void BFVEvaluator_S::relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const {
        kswitch_->relinearize(ciph1,relin_keys,result);
    }
    void BFVEvaluator_S::multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const RelinKeys &relin_key) const {}
    void BFVEvaluator_S::rotate_row(const Ciphertext &ciph,int rot_step, const GaloisKeys &galois_keys, Ciphertext &result) const {
        result = ciph;
        kswitch_->rotate_internal(result, rot_step, galois_keys, pool_);
    }
    void BFVEvaluator_S::rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const {
        result = ciph;
        kswitch_->conjugate_internal(result, galois_keys, pool_);
    }
    void BFVEvaluator_S::drop_modulus(const Ciphertext &ciphertext, Ciphertext &result, parms_id_type parms_id) const {
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }

        if (ciphertext.is_ntt_form())
        {
            throw invalid_argument("rescale_inplace : BFV encrypted must not be in NTT form");
        }
        auto context_data_ptr = context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto &context_data = *context_data_ptr;
        auto &next_context_data = *context_data.next_context_data();
        auto &next_parms = next_context_data.parms();
        auto rns_tool = context_data.rns_tool();

        size_t encrypted_size = ciphertext.size();
        size_t coeff_count = next_parms.degree();
        size_t next_coeff_modulus_size = next_context_data.coeff_modulus().size();

        Ciphertext encrypted_copy(pool_);
        encrypted_copy = ciphertext;
        POSEIDON_ITERATE(iter(encrypted_copy), encrypted_size, [&](auto I) {
            rns_tool->divide_and_round_q_last_inplace(I , pool_);
        });
        result.resize(context_, next_context_data.parms().parms_id(), encrypted_size);
        POSEIDON_ITERATE(iter(encrypted_copy, result), encrypted_size, [&](auto I) {
            set_poly(get<0>(I), coeff_count, next_coeff_modulus_size, get<1>(I));
        });

        // Set other attributes
        result.is_ntt_form() = ciphertext.is_ntt_form();

    }


    void BFVEvaluator_S::add_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const{
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }
        // Verify parameters.
        if (ciphertext.is_ntt_form())
        {
            throw invalid_argument("BFV encrypted must not be  NTT form");
        }
        if (plain.is_ntt_form())
        {
            throw invalid_argument("BFV plain must not be NTT form");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        multiply_add_plain_with_scaling_variant(plain, context_data, *iter(ciphertext));
    }

    void BFVEvaluator_S::sub_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const{
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }
        // Verify parameters.
        if (ciphertext.is_ntt_form())
        {
            throw invalid_argument("BFV encrypted must not be  NTT form");
        }
        if (plain.is_ntt_form())
        {
            throw invalid_argument("BFV plain must not be NTT form");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        multiply_sub_plain_with_scaling_variant(plain, context_data, *iter(ciphertext));
    }
    void BFVEvaluator_S::add_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2) const{
        // Verify parameters.
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("add_inplace : ciphertext1 and ciphertext2 parameter mismatch");
        }
        if (ciphertext1.is_ntt_form() != ciphertext2.is_ntt_form())
        {
            throw invalid_argument("NTT form mismatch");
        }


        auto &context_data = *context_.crt_context()->get_context_data(ciphertext1.parms_id());
        auto &parms = context_data.parms();
        size_t coeff_count = parms.degree();
        size_t ciphertext1_size = ciphertext1.size();
        size_t ciphertext2_size = ciphertext2.size();
        size_t max_count = max(ciphertext1_size, ciphertext2_size);
        size_t min_count = min(ciphertext1_size, ciphertext2_size);

        // Size check
        if (!product_fits_in(max_count, coeff_count))
        {
            throw logic_error("invalid parameters");
        }
        // Prepare result
        ciphertext1.resize(context_, context_data.parms().parms_id(), max_count);
        // Add ciphertexts
        for(auto i = 0; i < min_count; i++){
            ciphertext1[i].add(ciphertext2[i],ciphertext1[i]);
        }

        // Copy the remainding polys of the array with larger count into ciphertext1
        if (ciphertext1_size < ciphertext2_size)
        {
            for(auto i = min_count; i < max_count; ++i){
                ciphertext1[i].copy(ciphertext2[i]);
            }
        }
    }

    void BFVEvaluator_S::bfv_multiply(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool) const{
        if (encrypted1.is_ntt_form() || encrypted2.is_ntt_form())
        {
            throw invalid_argument("encrypted1 or encrypted2 cannot be in NTT form");
        }

        bool is_square = false;
        if(&encrypted1 == &encrypted2){
            is_square = true;
        }
        // Extract encryption parameters.
        auto &context_data = *context_.crt_context()->get_context_data(encrypted1.parms_id());
        auto &parms = context_data.parms();
        size_t coeff_count = parms.degree();
        size_t base_q_size = context_data.coeff_modulus().size();
        size_t encrypted1_size = encrypted1.size();
        size_t encrypted2_size = encrypted2.size();
        uint64_t plain_modulus = parms.plain_modulus().value();

        auto rns_tool = context_data.rns_tool();
        size_t base_Bsk_size = rns_tool->base_Bsk()->size();
        size_t base_Bsk_m_tilde_size = rns_tool->base_Bsk_m_tilde()->size();

        // Determine destination.size()
        size_t dest_size = sub_safe(add_safe(encrypted1_size, encrypted2_size), size_t(1));

        // Size check
        if (!product_fits_in(dest_size, coeff_count, base_Bsk_m_tilde_size))
        {
            throw logic_error("invalid parameters");
        }

        // Set up iterators for bases
        auto base_q = iter(context_data.coeff_modulus());
        auto base_Bsk = iter(rns_tool->base_Bsk()->base());

        // Set up iterators for NTT tables
        auto base_q_ntt_tables = iter(context_.crt_context()->small_ntt_tables());
        auto base_Bsk_ntt_tables = iter(rns_tool->base_Bsk_ntt_tables());

        // Microsoft SEAL uses BEHZ-style RNS multiplication. This process is somewhat complex and consists of the
        // following steps:
        //
        // (1) Lift encrypted1 and encrypted2 (initially in base q) to an extended base q U Bsk U {m_tilde}
        // (2) Remove extra multiples of q from the results with Montgomery reduction, switching base to q U Bsk
        // (3) Transform the data to NTT form
        // (4) Compute the ciphertext polynomial product using dyadic multiplication
        // (5) Transform the data back from NTT form
        // (6) Multiply the result by t (plain_modulus)
        // (7) Scale the result by q using a divide-and-floor algorithm, switching base to Bsk
        // (8) Use Shenoy-Kumaresan method to convert the result to base q

        // Resize encrypted1 to destination size
        encrypted1.resize(context_, context_data.parms().parms_id(), dest_size);

        // This lambda function takes as input an IterTuple with three components:
        //
        // 1. (Const)RNSIter to read an input polynomial from
        // 2. RNSIter for the output in base q
        // 3. RNSIter for the output in base Bsk
        //
        // It performs steps (1)-(3) of the BEHZ multiplication (see above) on the given input polynomial (given as an
        // RNSIter or ConstRNSIter) and writes the results in base q and base Bsk to the given output
        // iterators.
        auto behz_extend_base_convert_to_ntt = [&](auto I) {
            // Make copy of input polynomial (in base q) and convert to NTT form
            // Lazy reduction
            set_poly(get<0>(I), coeff_count, base_q_size, get<1>(I));
            ntt_negacyclic_harvey_lazy(get<1>(I), base_q_size, base_q_ntt_tables);

            // Allocate temporary space for a polynomial in the Bsk U {m_tilde} base
            POSEIDON_ALLOCATE_GET_RNS_ITER(temp, coeff_count, base_Bsk_m_tilde_size, pool);

            // (1) Convert from base q to base Bsk U {m_tilde}
            rns_tool->fastbconv_m_tilde(get<0>(I), temp, pool);

            // (2) Reduce q-overflows in with Montgomery reduction, switching base to Bsk
            rns_tool->sm_mrq(temp, get<2>(I), pool);

            // Transform to NTT form in base Bsk
            // Lazy reduction
            ntt_negacyclic_harvey_lazy(get<2>(I), base_Bsk_size, base_Bsk_ntt_tables);
        };

        // Allocate space for a base q output of behz_extend_base_convert_to_ntt for encrypted1
        POSEIDON_ALLOCATE_GET_POLY_ITER(encrypted1_q, encrypted1_size, coeff_count, base_q_size, pool);

        // Allocate space for a base Bsk output of behz_extend_base_convert_to_ntt for encrypted1
        POSEIDON_ALLOCATE_GET_POLY_ITER(encrypted1_Bsk, encrypted1_size, coeff_count, base_Bsk_size, pool);

        // Perform BEHZ steps (1)-(3) for encrypted1
        POSEIDON_ITERATE(iter(encrypted1, encrypted1_q, encrypted1_Bsk), encrypted1_size, behz_extend_base_convert_to_ntt);

        // Repeat for encrypted2
        POSEIDON_ALLOCATE_GET_POLY_ITER(encrypted2_q, encrypted2_size, coeff_count, base_q_size, pool);
        POSEIDON_ALLOCATE_GET_POLY_ITER(encrypted2_Bsk, encrypted2_size, coeff_count, base_Bsk_size, pool);

        POSEIDON_ITERATE(iter(encrypted2, encrypted2_q, encrypted2_Bsk), encrypted2_size, behz_extend_base_convert_to_ntt);

        // Allocate temporary space for the output of step (4)
        // We allocate space separately for the base q and the base Bsk components
        POSEIDON_ALLOCATE_ZERO_GET_POLY_ITER(temp_dest_q, dest_size, coeff_count, base_q_size, pool);
        POSEIDON_ALLOCATE_ZERO_GET_POLY_ITER(temp_dest_Bsk, dest_size, coeff_count, base_Bsk_size, pool);

        if(is_square){
            // Perform BEHZ step (4): dyadic multiplication on arbitrary size ciphertexts
            POSEIDON_ITERATE(iter(size_t(0)), dest_size, [&](auto I) {
                // We iterate over relevant components of encrypted1 and encrypted2 in increasing order for
                // encrypted1 and reversed (decreasing) order for encrypted2. The bounds for the indices of
                // the relevant terms are obtained as follows.
                size_t curr_encrypted1_last = min<size_t>(I, encrypted1_size - 1);
                size_t curr_encrypted2_first = min<size_t>(I, encrypted2_size - 1);
                size_t curr_encrypted1_first = I - curr_encrypted2_first;
                // size_t curr_encrypted2_last = I - curr_encrypted1_last;

                // The total number of dyadic products is now easy to compute
                size_t steps = curr_encrypted1_last - curr_encrypted1_first + 1;

                // This lambda function computes the ciphertext product for BFV multiplication. Since we use the BEHZ
                // approach, the multiplication of individual polynomials is done using a dyadic product where the inputs
                // are already in NTT form. The arguments of the lambda function are expected to be as follows:
                //
                // 1. a ConstPolyIter pointing to the beginning of the first input ciphertext (in NTT form)
                // 2. a ConstPolyIter pointing to the beginning of the second input ciphertext (in NTT form)
                // 3. a ConstModulusIter pointing to an array of Modulus elements for the base
                // 4. the size of the base
                // 5. a PolyIter pointing to the beginning of the output ciphertext
                auto behz_ciphertext_square = [&](ConstPolyIter in_iter, ConstModulusIter base_iter, size_t base_size,
                                                  PolyIter out_iter) {
                    // Compute c0^2
                    dyadic_product_coeffmod(in_iter[0], in_iter[0], base_size, base_iter, out_iter[0]);

                    // Compute 2*c0*c1
                    dyadic_product_coeffmod(in_iter[0], in_iter[1], base_size, base_iter, out_iter[1]);
                    add_poly_coeffmod(out_iter[1], out_iter[1], base_size, base_iter, out_iter[1]);

                    // Compute c1^2
                    dyadic_product_coeffmod(in_iter[1], in_iter[1], base_size, base_iter, out_iter[2]);
                };

                // Perform the BEHZ ciphertext square both for base q and base Bsk
                behz_ciphertext_square(encrypted1_q, base_q, base_q_size, temp_dest_q);
                behz_ciphertext_square(encrypted1_Bsk, base_Bsk, base_Bsk_size, temp_dest_Bsk);
            });
        }
        else {
            // Perform BEHZ step (4): dyadic multiplication on arbitrary size ciphertexts
            POSEIDON_ITERATE(iter(size_t(0)), dest_size, [&](auto I) {
                // We iterate over relevant components of encrypted1 and encrypted2 in increasing order for
                // encrypted1 and reversed (decreasing) order for encrypted2. The bounds for the indices of
                // the relevant terms are obtained as follows.
                size_t curr_encrypted1_last = min<size_t>(I, encrypted1_size - 1);
                size_t curr_encrypted2_first = min<size_t>(I, encrypted2_size - 1);
                size_t curr_encrypted1_first = I - curr_encrypted2_first;
                // size_t curr_encrypted2_last = I - curr_encrypted1_last;

                // The total number of dyadic products is now easy to compute
                size_t steps = curr_encrypted1_last - curr_encrypted1_first + 1;

                // This lambda function computes the ciphertext product for BFV multiplication. Since we use the BEHZ
                // approach, the multiplication of individual polynomials is done using a dyadic product where the inputs
                // are already in NTT form. The arguments of the lambda function are expected to be as follows:
                //
                // 1. a ConstPolyIter pointing to the beginning of the first input ciphertext (in NTT form)
                // 2. a ConstPolyIter pointing to the beginning of the second input ciphertext (in NTT form)
                // 3. a ConstModulusIter pointing to an array of Modulus elements for the base
                // 4. the size of the base
                // 5. a PolyIter pointing to the beginning of the output ciphertext
                auto behz_ciphertext_product = [&](ConstPolyIter in1_iter, ConstPolyIter in2_iter,
                                                   ConstModulusIter base_iter, size_t base_size, PolyIter out_iter) {
                    // Create a shifted iterator for the first input
                    auto shifted_in1_iter = in1_iter + curr_encrypted1_first;

                    // Create a shifted reverse iterator for the second input
                    auto shifted_reversed_in2_iter = reverse_iter(in2_iter + curr_encrypted2_first);

                    // Create a shifted iterator for the output
                    auto shifted_out_iter = out_iter[I];

                    POSEIDON_ITERATE(iter(shifted_in1_iter, shifted_reversed_in2_iter), steps, [&](auto J) {
                        POSEIDON_ITERATE(iter(J, base_iter, shifted_out_iter), base_size, [&](auto K) {
                            POSEIDON_ALLOCATE_GET_COEFF_ITER(temp, coeff_count, pool);
                            dyadic_product_coeffmod(get<0, 0>(K), get<0, 1>(K), coeff_count, get<1>(K), temp);
                            add_poly_coeffmod(temp, get<2>(K), coeff_count, get<1>(K), get<2>(K));
                        });
                    });
                };

                // Perform the BEHZ ciphertext product both for base q and base Bsk
                behz_ciphertext_product(encrypted1_q, encrypted2_q, base_q, base_q_size, temp_dest_q);
                behz_ciphertext_product(encrypted1_Bsk, encrypted2_Bsk, base_Bsk, base_Bsk_size, temp_dest_Bsk);
            });
        }

        // Perform BEHZ step (5): transform data from NTT form
        // Lazy reduction here. The following multiply_poly_scalar_coeffmod will correct the value back to [0, p)
        inverse_ntt_negacyclic_harvey_lazy(temp_dest_q, dest_size, base_q_ntt_tables);
        inverse_ntt_negacyclic_harvey_lazy(temp_dest_Bsk, dest_size, base_Bsk_ntt_tables);

        // Perform BEHZ steps (6)-(8)
        POSEIDON_ITERATE(iter(temp_dest_q, temp_dest_Bsk, encrypted1), dest_size, [&](auto I) {
            // Bring together the base q and base Bsk components into a single allocation
            POSEIDON_ALLOCATE_GET_RNS_ITER(temp_q_Bsk, coeff_count, base_q_size + base_Bsk_size, pool);

            // Step (6): multiply base q components by t (plain_modulus)
            multiply_poly_scalar_coeffmod(get<0>(I), base_q_size, plain_modulus, base_q, temp_q_Bsk);

            multiply_poly_scalar_coeffmod(get<1>(I), base_Bsk_size, plain_modulus, base_Bsk, temp_q_Bsk + base_q_size);

            // Allocate yet another temporary for fast divide-and-floor result in base Bsk
            POSEIDON_ALLOCATE_GET_RNS_ITER(temp_Bsk, coeff_count, base_Bsk_size, pool);

            // Step (7): divide by q and floor, producing a result in base Bsk
            rns_tool->fast_floor(temp_q_Bsk, temp_Bsk, pool);

            // Step (8): use Shenoy-Kumaresan method to convert the result to base q and write to encrypted1
            rns_tool->fastbconv_sk(temp_Bsk, get<2>(I), pool);
        });
    }
    void BFVEvaluator_S::multiply_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2, MemoryPoolHandle pool) const{
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("multiply_inplace : ciphertext1 and ciphertext2 parameter mismatch");
        }
        bfv_multiply(ciphertext1, ciphertext2, std::move(pool));
    }
    void BFVEvaluator_S::multiply_plain_inplace( Ciphertext &encrypted, const Plaintext &plain) const{
        if(!encrypted.is_valid()){
            POSEIDON_THROW(invalid_error,"ciphertext is empty");
        }

        // Verify parameters.
        if (encrypted.is_ntt_form())
        {
            POSEIDON_THROW(invalid_error,"BFV encrypted must not be in NTT form");
        }
        if (plain.is_ntt_form())
        {
            POSEIDON_THROW(invalid_error,"BFV plain must not be in NTT form");
        }

//        auto plain_copy = plain;
//
//        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
//        auto &coeff_modulus = context_data.coeff_modulus();
//        auto poly_modulus_degree = ciphertext.poly_modulus_degree();
//        auto coeff_modulus_size = ciphertext.coeff_modulus_size();
//        auto plain_ntt_table = context_.crt_context()->plain_ntt_tables();
//
//        cout << plain_copy.coeff_count() << endl;
//        ftt_fwd(ciphertext,ciphertext);
//        cout << plain_copy.data()[poly_modulus_degree - 2] << endl;
//         ntt_negacyclic_harvey(plain_copy.data(),*plain_ntt_table);
//        cout << plain_copy.data()[poly_modulus_degree - 2] << endl;
////        uint64_t *install = new uint64_t [poly_modulus_degree];
////        for(auto i = 0; i < poly_modulus_degree; ++i){
////            install[i] = 60000;
////        }
//
//        for(auto i = 0; i < coeff_modulus_size; ++i){
//            dyadic_product_coeffmod(plain_copy.data(),ciphertext[0][i].begin(),poly_modulus_degree,coeff_modulus[i],ciphertext[0][i].begin());
//            dyadic_product_coeffmod(plain_copy.data(),ciphertext[1][i].begin(),poly_modulus_degree,coeff_modulus[i],ciphertext[1][i].begin());
//
//        }
//        inverse_ntt_negacyclic_harvey(plain_copy.data(),*plain_ntt_table);
//        cout << plain_copy.data()[poly_modulus_degree - 2] << endl;
//        ftt_inv(ciphertext,ciphertext);
        auto &context_data = *context_.crt_context()->get_context_data(encrypted.parms_id());
        auto &parms = context_data.parms();
        auto &coeff_modulus = context_data.coeff_modulus();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = coeff_modulus.size();

        uint64_t plain_upper_half_threshold = context_data.plain_upper_half_threshold();
        auto plain_upper_half_increment = context_data.plain_upper_half_increment();
        auto ntt_tables = iter(context_.crt_context()->small_ntt_tables());

        size_t encrypted_size = encrypted.size();
        size_t plain_coeff_count = plain.coeff_count();
        size_t plain_nonzero_coeff_count = plain.nonzero_coeff_count();

        // Size check
        if (!product_fits_in(encrypted_size, coeff_count, coeff_modulus_size))
        {
            throw logic_error("invalid parameters");
        }

        /*
        Optimizations for constant / monomial multiplication can lead to the presence of a timing side-channel in
        use-cases where the plaintext data should also be kept private.
        */
        if (plain_nonzero_coeff_count == 1)
        {
            // Multiplying by a monomial?
            size_t mono_exponent = plain.significant_coeff_count() - 1;

            if (plain[mono_exponent] >= plain_upper_half_threshold)
            {
                if (!context_data.using_fast_plain_lift())
                {
                    // Allocate temporary space for a single RNS coefficient
                    POSEIDON_ALLOCATE_GET_COEFF_ITER(temp, coeff_modulus_size, pool_);

                    // We need to adjust the monomial modulo each coeff_modulus prime separately when the coeff_modulus
                    // primes may be larger than the plain_modulus. We add plain_upper_half_increment (i.e., q-t) to
                    // the monomial to ensure it is smaller than coeff_modulus and then do an RNS multiplication. Note
                    // that in this case plain_upper_half_increment contains a multi-precision integer, so after the
                    // addition we decompose the multi-precision integer into RNS components, and then multiply.
                    add_uint(plain_upper_half_increment, coeff_modulus_size, plain[mono_exponent], temp);
                    context_data.rns_tool()->base_q()->decompose(temp, pool_);
                    negacyclic_multiply_poly_mono_coeffmod(
                            encrypted, encrypted_size, temp, mono_exponent, coeff_modulus, encrypted, pool_);
                }
                else
                {
                    // Every coeff_modulus prime is larger than plain_modulus, so there is no need to adjust the
                    // monomial. Instead, just do an RNS multiplication.
                    negacyclic_multiply_poly_mono_coeffmod(
                            encrypted, encrypted_size, plain[mono_exponent], mono_exponent, coeff_modulus, encrypted, pool_);
                }
            }
            else
            {
                // The monomial represents a positive number, so no RNS multiplication is needed.
                negacyclic_multiply_poly_mono_coeffmod(
                        encrypted, encrypted_size, plain[mono_exponent], mono_exponent, coeff_modulus, encrypted, pool_);
            }



            return;
        }

        // Generic case: any plaintext polynomial
        // Allocate temporary space for an entire RNS polynomial
        auto temp(allocate_zero_poly(coeff_count, coeff_modulus_size, pool_));

        if (!context_data.using_fast_plain_lift())
        {
            StrideIter<uint64_t *> temp_iter(temp.get(), coeff_modulus_size);

            POSEIDON_ITERATE(iter(plain.data(), temp_iter), plain_coeff_count, [&](auto I) {
                auto plain_value = get<0>(I);
                if (plain_value >= plain_upper_half_threshold)
                {
                    add_uint(plain_upper_half_increment, coeff_modulus_size, plain_value, get<1>(I));
                }
                else
                {
                    *get<1>(I) = plain_value;
                }
            });

            context_data.rns_tool()->base_q()->decompose_array(temp_iter, coeff_count, pool_);
        }
        else
        {
            // Note that in this case plain_upper_half_increment holds its value in RNS form modulo the coeff_modulus
            // primes.
            RNSIter temp_iter(temp.get(), coeff_count);
            POSEIDON_ITERATE(iter(temp_iter, plain_upper_half_increment), coeff_modulus_size, [&](auto I) {
                POSEIDON_ITERATE(iter(get<0>(I), plain.data()), plain_coeff_count, [&](auto J) {
                    get<0>(J) =
                            POSEIDON_COND_SELECT(get<1>(J) >= plain_upper_half_threshold, get<1>(J) + get<1>(I), get<1>(J));
                });
            });
        }

        // Need to multiply each component in encrypted with temp; first step is to transform to NTT form
        RNSIter temp_iter(temp.get(), coeff_count);
        ntt_negacyclic_harvey(temp_iter, coeff_modulus_size, ntt_tables);

        POSEIDON_ITERATE(iter(encrypted), encrypted_size, [&](auto I) {
            POSEIDON_ITERATE(iter(I, temp_iter, coeff_modulus, ntt_tables), coeff_modulus_size, [&](auto J) {
                // Lazy reduction
                ntt_negacyclic_harvey_lazy(get<0>(J), get<3>(J));
                dyadic_product_coeffmod(get<0>(J), get<1>(J), coeff_count, get<2>(J), get<0>(J));
                inverse_ntt_negacyclic_harvey(get<0>(J), get<3>(J));
            });
        });



    }

}