//
// Created by lxs on 24-2-20.
//

#include "poseidon/scheme/bgv/Software/BGVEvaluator_S.h"
#include "poseidon/seal/util/scalingvariant.h"
namespace poseidon{
    void BGVEvaluator_S::ftt_fwd(const Plaintext &plain ,Plaintext &result) const{
        ftt_fwd_b(plain,result);
    }
    void BGVEvaluator_S::ftt_fwd(const Ciphertext &ciph, Ciphertext &result) const {
        ftt_fwd_b(ciph,result);
    }
    void BGVEvaluator_S::ftt_inv(const Plaintext &plain ,Plaintext &result) const {
        ftt_inv_b(plain,result);
    }
    void BGVEvaluator_S::ftt_inv(const Ciphertext &ciph ,Ciphertext &result) const {
        ftt_inv_b(ciph,result);
    }
    void BGVEvaluator_S::add(const Ciphertext &ciphertext1,const  Ciphertext &ciphertext2,Ciphertext &result) const {
        if(&result == &ciphertext1){
            add_inplace(result,ciphertext2);
        }
        else{
            result = ciphertext2;
            add_inplace(result, ciphertext1);
        }
    }
    void BGVEvaluator_S::sub(const Ciphertext &ciphertext1,const  Ciphertext &ciphertext2,Ciphertext &result) const {
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
    void BGVEvaluator_S::add_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        add_plain_inplace(result, plain);
    }

    void BGVEvaluator_S::sub_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        sub_plain_inplace(result, plain);
    }
    void BGVEvaluator_S::multiply_plain(const Ciphertext &ciph,const  Plaintext &plain,Ciphertext &result) const {
        result = ciph;
        multiply_plain_inplace(result, plain);
    }
    void BGVEvaluator_S::multiply(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result) const {
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
    void BGVEvaluator_S::relinearize(const Ciphertext &ciph1, const RelinKeys &relin_keys, Ciphertext &result) const {
        kswitch_->relinearize(ciph1,relin_keys,result);
    }
    void BGVEvaluator_S::multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const RelinKeys &relin_key) const {}
    void BGVEvaluator_S::rotate_row(const Ciphertext &ciph,int rot_step, const GaloisKeys &galois_keys, Ciphertext &result) const {
        result = ciph;
        kswitch_->rotate_internal(result, rot_step, galois_keys, pool_);
    }
    void BGVEvaluator_S::rotate_col(const Ciphertext &ciph, const GaloisKeys &galois_keys, Ciphertext &result) const {
        result = ciph;
        kswitch_->conjugate_internal(result, galois_keys, pool_);
    }
    void BGVEvaluator_S::drop_modulus(const Ciphertext &ciphertext, Ciphertext &result, parms_id_type parms_id) const {
        if(!ciphertext.is_valid()){
            POSEIDON_THROW(invalid_error,"ciphertext is empty");
        }

        if (!ciphertext.is_ntt_form())
        {
            POSEIDON_THROW(config_error,"BGV encrypted must not be in NTT form");
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
            rns_tool->mod_t_and_divide_q_last_ntt_inplace(I, context_.crt_context()->small_ntt_tables(), pool_);
        });
        result.resize(context_, next_context_data.parms().parms_id(), encrypted_size);
        POSEIDON_ITERATE(iter(encrypted_copy, result), encrypted_size, [&](auto I) {
            set_poly(get<0>(I), coeff_count, next_coeff_modulus_size, get<1>(I));
        });

        // Set other attributes
        result.is_ntt_form() = ciphertext.is_ntt_form();
        result.correction_factor() = multiply_uint_mod(
                ciphertext.correction_factor(), rns_tool->inv_q_last_mod_t(), next_parms.plain_modulus());

    }


    void BGVEvaluator_S::add_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const{
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }
        // Verify parameters.
        if (!ciphertext.is_ntt_form())
        {
            throw invalid_argument("BGV encrypted must  be  NTT form");
        }
        if (plain.is_ntt_form())
        {
            throw invalid_argument("BGV plain must not be NTT form");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto &parms = context_data.parms();
        auto &coeff_modulus = context_data.coeff_modulus();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = coeff_modulus.size();

        Plaintext plain_copy = plain;
        multiply_poly_scalar_coeffmod(
                plain.data(), plain.coeff_count(), ciphertext.correction_factor(), parms.plain_modulus(),
                plain_copy.data());
        transform_to_ntt_inplace(plain_copy, ciphertext.parms_id(), move(pool_));
        RNSIter encrypted_iter(ciphertext.data(), coeff_count);
        ConstRNSIter plain_iter(plain_copy.data(), coeff_count);
        add_poly_coeffmod(encrypted_iter, plain_iter, coeff_modulus_size, coeff_modulus, encrypted_iter);
    }

    void BGVEvaluator_S::sub_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const{
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }
        // Verify parameters.
        if (ciphertext.is_ntt_form())
        {
            throw invalid_argument("BGV encrypted must not be  NTT form");
        }
        if (plain.is_ntt_form())
        {
            throw invalid_argument("BGV plain must not be NTT form");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        multiply_sub_plain_with_scaling_variant(plain, context_data, *iter(ciphertext));
    }
    void BGVEvaluator_S::add_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2) const{
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

    void BGVEvaluator_S::bgv_multiply(Ciphertext &encrypted1, const Ciphertext &encrypted2, MemoryPoolHandle pool) const{
        if (!encrypted1.is_ntt_form() || !encrypted2.is_ntt_form())
        {
            throw invalid_argument("encrypted1 or encrypted2 must be in NTT form");
        }
        // Extract encryption parameters.
        auto &context_data = *context_.crt_context()->get_context_data(encrypted1.parms_id());
        auto &parms = context_data.parms();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = context_data.coeff_modulus().size();
        size_t encrypted1_size = encrypted1.size();
        size_t encrypted2_size = encrypted2.size();

        // Determine destination.size()
        // Default is 3 (c_0, c_1, c_2)
        size_t dest_size = sub_safe(add_safe(encrypted1_size, encrypted2_size), size_t(1));

        // Set up iterator for the base
        auto coeff_modulus = iter(context_data.coeff_modulus());

        // Prepare destination
        encrypted1.resize(context_, context_data.parms().parms_id(), dest_size);

        // Convert c0 and c1 to ntt
        // Set up iterators for input ciphertexts
        PolyIter encrypted1_iter = iter(encrypted1);
        ConstPolyIter encrypted2_iter = iter(encrypted2);

        if (dest_size == 3)
        {
            // We want to keep six polynomials in the L1 cache: x[0], x[1], x[2], y[0], y[1], temp.
            // For a 32KiB cache, which can store 32768 / 8 = 4096 coefficients, = 682.67 coefficients per polynomial,
            // we should keep the tile size at 682 or below. The tile size must divide coeff_count, i.e. be a power of
            // two. Some testing shows similar performance with tile size 256 and 512, and worse performance on smaller
            // tiles. We pick the smaller of the two to prevent L1 cache misses on processors with < 32 KiB L1 cache.
            size_t tile_size = min<size_t>(coeff_count, size_t(256));
            size_t num_tiles = coeff_count / tile_size;
#ifdef POSEIDON_DEBUG
            if (coeff_count % tile_size != 0)
            {
                throw invalid_argument("tile_size does not divide coeff_count");
            }
#endif

            // Semantic misuse of RNSIter; each is really pointing to the data for each RNS factor in sequence
            ConstRNSIter encrypted2_0_iter(*encrypted2_iter[0], tile_size);
            ConstRNSIter encrypted2_1_iter(*encrypted2_iter[1], tile_size);
            RNSIter encrypted1_0_iter(*encrypted1_iter[0], tile_size);
            RNSIter encrypted1_1_iter(*encrypted1_iter[1], tile_size);
            RNSIter encrypted1_2_iter(*encrypted1_iter[2], tile_size);

            // Temporary buffer to store intermediate results
            POSEIDON_ALLOCATE_GET_COEFF_ITER(temp, tile_size, pool);

            // Computes the output tile_size coefficients at a time
            // Given input tuples of polynomials x = (x[0], x[1], x[2]), y = (y[0], y[1]), computes
            // x = (x[0] * y[0], x[0] * y[1] + x[1] * y[0], x[1] * y[1])
            // with appropriate modular reduction
            POSEIDON_ITERATE(coeff_modulus, coeff_modulus_size, [&](auto I) {
                POSEIDON_ITERATE(iter(size_t(0)), num_tiles, [&](POSEIDON_MAYBE_UNUSED auto J) {
                    // Compute third output polynomial, overwriting input
                    // x[2] = x[1] * y[1]
                    dyadic_product_coeffmod(
                            encrypted1_1_iter[0], encrypted2_1_iter[0], tile_size, I, encrypted1_2_iter[0]);

                    // Compute second output polynomial, overwriting input
                    // temp = x[1] * y[0]
                    dyadic_product_coeffmod(encrypted1_1_iter[0], encrypted2_0_iter[0], tile_size, I, temp);
                    // x[1] = x[0] * y[1]
                    dyadic_product_coeffmod(
                            encrypted1_0_iter[0], encrypted2_1_iter[0], tile_size, I, encrypted1_1_iter[0]);
                    // x[1] += temp
                    add_poly_coeffmod(encrypted1_1_iter[0], temp, tile_size, I, encrypted1_1_iter[0]);

                    // Compute first output polynomial, overwriting input
                    // x[0] = x[0] * y[0]
                    dyadic_product_coeffmod(
                            encrypted1_0_iter[0], encrypted2_0_iter[0], tile_size, I, encrypted1_0_iter[0]);

                    // Manually increment iterators
                    encrypted1_0_iter++;
                    encrypted1_1_iter++;
                    encrypted1_2_iter++;
                    encrypted2_0_iter++;
                    encrypted2_1_iter++;
                });
            });
        }
        else
        {
            // Allocate temporary space for the result
            POSEIDON_ALLOCATE_ZERO_GET_POLY_ITER(temp, dest_size, coeff_count, coeff_modulus_size, pool);

            POSEIDON_ITERATE(iter(size_t(0)), dest_size, [&](auto I) {
                // We iterate over relevant components of encrypted1 and encrypted2 in increasing order for
                // encrypted1 and reversed (decreasing) order for encrypted2. The bounds for the indices of
                // the relevant terms are obtained as follows.
                size_t curr_encrypted1_last = min<size_t>(I, encrypted1_size - 1);
                size_t curr_encrypted2_first = min<size_t>(I, encrypted2_size - 1);
                size_t curr_encrypted1_first = I - curr_encrypted2_first;
                // size_t curr_encrypted2_last = secret_power_index - curr_encrypted1_last;

                // The total number of dyadic products is now easy to compute
                size_t steps = curr_encrypted1_last - curr_encrypted1_first + 1;

                // Create a shifted iterator for the first input
                auto shifted_encrypted1_iter = encrypted1_iter + curr_encrypted1_first;

                // Create a shifted reverse iterator for the second input
                auto shifted_reversed_encrypted2_iter = reverse_iter(encrypted2_iter + curr_encrypted2_first);

                POSEIDON_ITERATE(iter(shifted_encrypted1_iter, shifted_reversed_encrypted2_iter), steps, [&](auto J) {
                    // Extra care needed here:
                    // temp_iter must be dereferenced once to produce an appropriate RNSIter
                    POSEIDON_ITERATE(iter(J, coeff_modulus, temp[I]), coeff_modulus_size, [&](auto K) {
                        POSEIDON_ALLOCATE_GET_COEFF_ITER(prod, coeff_count, pool);
                        dyadic_product_coeffmod(get<0, 0>(K), get<0, 1>(K), coeff_count, get<1>(K), prod);
                        add_poly_coeffmod(prod, get<2>(K), coeff_count, get<1>(K), get<2>(K));
                    });
                });
            });

            // Set the final result
            set_poly_array(temp, dest_size, coeff_count, coeff_modulus_size, encrypted1.data());
        }

        // Set the correction factor
        encrypted1.correction_factor() =
                multiply_uint_mod(encrypted1.correction_factor(), encrypted2.correction_factor(), parms.plain_modulus());
    }
    void BGVEvaluator_S::multiply_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2, MemoryPoolHandle pool) const{
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("multiply_inplace : ciphertext1 and ciphertext2 parameter mismatch");
        }
        bgv_multiply(ciphertext1, ciphertext2, std::move(pool));
    }
    void BGVEvaluator_S::multiply_plain_inplace( Ciphertext &ciphertext, const Plaintext &plain) const{
        if(!ciphertext.is_valid()){
            POSEIDON_THROW(invalid_error,"ciphertext is empty");
        }

        // Verify parameters.
        if (!ciphertext.is_ntt_form())
        {
            POSEIDON_THROW(invalid_error,"BGV encrypted must be in NTT form");
        }
        if (plain.is_ntt_form())
        {
            POSEIDON_THROW(invalid_error,"BGV plain must be in NTT form");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto &parms = context_data.parms();
        auto &coeff_modulus = context_data.coeff_modulus();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = coeff_modulus.size();
        size_t encrypted_ntt_size = ciphertext.size();

        // Size check
        if (!product_fits_in(encrypted_ntt_size, coeff_count, coeff_modulus_size))
        {
            throw logic_error("invalid parameters");
        }


        else if (ciphertext.is_ntt_form() && !plain.is_ntt_form())
        {
            Plaintext plain_copy = plain;
            transform_to_ntt_inplace(plain_copy, ciphertext.parms_id(), move(pool_));
            plain_copy.poly().multiply(ciphertext[0],ciphertext[0]);
            plain_copy.poly().multiply(ciphertext[1],ciphertext[1]);
        }


    }


    void BGVEvaluator_S::transform_to_ntt_inplace(Plaintext &plain, parms_id_type parms_id, MemoryPoolHandle pool) const
    {
        // Verify parameters.
        

        auto context_data_ptr = context_.crt_context()->get_context_data(parms_id);
        if (!context_data_ptr)
        {
            throw invalid_argument("parms_id is not valid for the current context");
        }
        if (plain.is_ntt_form())
        {
            throw invalid_argument("plain is already in NTT form");
        }
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        // Extract encryption parameters.
        auto &context_data = *context_data_ptr;
        auto &parms = context_data.parms();
        auto &coeff_modulus = context_data.coeff_modulus();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = coeff_modulus.size();
        size_t plain_coeff_count = plain.coeff_count();

        uint64_t plain_upper_half_threshold = context_data.plain_upper_half_threshold();
        auto plain_upper_half_increment = context_data.plain_upper_half_increment();

        auto ntt_tables = iter(context_.crt_context()->small_ntt_tables());

        // Size check
        if (!product_fits_in(coeff_count, coeff_modulus_size))
        {
            throw logic_error("invalid parameters");
        }

        // Resize to fit the entire NTT transformed (ciphertext size) polynomial
        // Note that the new coefficients are automatically set to 0
        plain.resize(context_,parms_id,coeff_count * coeff_modulus_size);
        RNSIter plain_iter(plain.data(), coeff_count);

        if (!context_data.using_fast_plain_lift())
        {
            // Allocate temporary space for an entire RNS polynomial
            // Slight semantic misuse of RNSIter here, but this works well
            POSEIDON_ALLOCATE_ZERO_GET_RNS_ITER(temp, coeff_modulus_size, coeff_count, pool);

            POSEIDON_ITERATE(iter(plain.data(), temp), plain_coeff_count, [&](auto I) {
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

            context_data.rns_tool()->base_q()->decompose_array(temp, coeff_count, pool);

            // Copy data back to plain
            set_poly(temp, coeff_count, coeff_modulus_size, plain.data());
        }
        else
        {
            // Note that in this case plain_upper_half_increment holds its value in RNS form modulo the coeff_modulus
            // primes.

            // Create a "reversed" helper iterator that iterates in the reverse order both plain RNS components and
            // the plain_upper_half_increment values.
            auto helper_iter = reverse_iter(plain_iter, plain_upper_half_increment);
            advance(helper_iter, -safe_cast<ptrdiff_t>(coeff_modulus_size - 1));

            POSEIDON_ITERATE(helper_iter, coeff_modulus_size, [&](auto I) {
                POSEIDON_ITERATE(iter(*plain_iter, get<0>(I)), plain_coeff_count, [&](auto J) {
                    get<1>(J) =
                            POSEIDON_COND_SELECT(get<0>(J) >= plain_upper_half_threshold, get<0>(J) + get<1>(I), get<0>(J));
                });
            });
        }

        // Transform to NTT domain
        ntt_negacyclic_harvey(plain_iter, coeff_modulus_size, ntt_tables);

        plain.parms_id() = parms_id;
    }

}