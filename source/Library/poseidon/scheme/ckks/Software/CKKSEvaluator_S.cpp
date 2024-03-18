//
// Created by cloudam on 12/19/23.
//

#include "CKKSEvaluator_S.h"

#include <utility>
namespace poseidon
{
    void CKKSEvaluator_S::ftt_fwd(const Plaintext &plain ,Plaintext &result) const {
        ftt_fwd_b(plain,result);
    }
    void CKKSEvaluator_S::ftt_fwd(const Ciphertext &ciphertext, Ciphertext &result) const {
        ftt_fwd_b(ciphertext,result);
    }
    void CKKSEvaluator_S::ftt_inv(const Plaintext &plain ,Plaintext &result) const{
        ftt_inv_b(plain,result);
    }
    void CKKSEvaluator_S::ftt_inv(const Ciphertext &ciphertext ,Ciphertext &result) const {
        ftt_inv_b(ciphertext,result);
    }

    void CKKSEvaluator_S::add(const poseidon::Ciphertext &ciphertext1,const poseidon::Ciphertext &ciphertext2,
                                poseidon::Ciphertext &result) const {

        if(&result == &ciphertext1){
            add_inplace(result,ciphertext2);
        }
        else{
            result = ciphertext2;
            add_inplace(result, ciphertext1);
        }
    }
    void CKKSEvaluator_S::multiply_plain(const Ciphertext &ciphertext, const Plaintext &plain, Ciphertext &result) const {
        result = ciphertext;
        multiply_plain_inplace(result,plain);
    }

    void CKKSEvaluator_S::multiply_plain_inplace( Ciphertext &ciphertext, const Plaintext &plain) const {
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }

        if (!ciphertext.is_ntt_form())
        {
            throw invalid_argument("ckks encrypted must be in NTT form");
        }
        if (!plain.is_ntt_form())
        {
            throw invalid_argument("ckks plain must be in NTT form");
        }
        if (ciphertext.parms_id() != plain.parms_id())
        {
            throw invalid_argument("ciphertext and plain parameter mismatch");
        }

        auto &context_data = *context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto scale_bit_count_bound = context_data.total_coeff_modulus_bit_count();
        auto encrypted_size = ciphertext.size();

        for(auto i = 0; i < encrypted_size; i++){
            ciphertext[i].multiply(plain.poly(),ciphertext[i]);
        }

        ciphertext.scale() *= plain.scale();
        if (ciphertext.scale() <= 0 || (static_cast<uint32_t >(log2(ciphertext.scale())) >= scale_bit_count_bound))
        {
            throw invalid_argument("scale out of bounds");
        }
    }


    void CKKSEvaluator_S::add_plain_inplace(Ciphertext &ciphertext, const Plaintext &plain) const
    {
        if(!ciphertext.is_valid()){
            throw invalid_argument("multiply_plain_inplace : Ciphertext is empty!");
        }
        // Verify parameters.
        if (!ciphertext.is_ntt_form())
        {
            throw invalid_argument("ckks encrypted must be in NTT form");
        }
        if (!plain.is_ntt_form())
        {
            throw invalid_argument("ckks plain must be in NTT form");
        }
        if (ciphertext.parms_id() != plain.parms_id())
        {
            throw invalid_argument("encrypted and plain parameter mismatch");
        }
        if (!util::are_close<double>(ciphertext.scale(), plain.scale()))
        {
            throw invalid_argument("add_plain_inplace : scale mismatch");
        }
        ciphertext[0].add(plain.poly(),ciphertext[0]);
    }

    void CKKSEvaluator_S::add_plain(const Ciphertext &ciphertext, const Plaintext &plain,Ciphertext &result) const{
        result = ciphertext;
        add_plain_inplace(result, plain);
    }

    void CKKSEvaluator_S::sub(const Ciphertext &ciphertext1,const  Ciphertext &ciphertext2,Ciphertext &result) const {
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

    void CKKSEvaluator_S::add_inplace(poseidon::Ciphertext &ciphertext1,
                                        const poseidon::Ciphertext &ciphertext2) const {
        // Verify parameters.
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("add_inplace : ciphertext1 and ciphertext2 parameter mismatch");
        }
        if (ciphertext1.is_ntt_form() != ciphertext2.is_ntt_form())
        {
            throw invalid_argument("NTT form mismatch");
        }
        if (!util::are_close<double>(ciphertext1.scale(), ciphertext2.scale()))
        {
            throw invalid_argument("add_inplace : scale mismatch");
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

    void CKKSEvaluator_S::multiply(const Ciphertext &ciphertext1, const Ciphertext &ciphertext2, Ciphertext &result) const
    {

        if (&ciphertext2 == &result)
        {
            multiply_inplace(result, ciphertext1);
        }
        else
        {
            result = ciphertext1;
            multiply_inplace(result, ciphertext2);
        }
    }

    void CKKSEvaluator_S::square_inplace( Ciphertext &ciph) const {
        multiply_inplace(ciph, ciph);
    }


    void CKKSEvaluator_S::multiply_inplace(Ciphertext &ciphertext1, const Ciphertext &ciphertext2, MemoryPoolHandle pool) const
    {
        if (ciphertext1.parms_id() != ciphertext2.parms_id())
        {
            throw invalid_argument("multiply_inplace : ciphertext1 and ciphertext2 parameter mismatch");
        }
        ckks_multiply(ciphertext1, ciphertext2, std::move(pool));
    }

    void CKKSEvaluator_S::ckks_multiply(Ciphertext &ciphertext1, const Ciphertext &ciphertext2, MemoryPoolHandle pool) const
    {
        if (!(ciphertext1.is_ntt_form() && ciphertext2.is_ntt_form()))
        {
            throw invalid_argument("ciphertext1 or ciphertext2 must be in NTT form");
        }

        bool is_square = false;
        if(&ciphertext1 == &ciphertext2){
            is_square = true;
        }
        // Extract encryption parameters.
        auto &context_data = *context_.crt_context()->get_context_data(ciphertext1.parms_id());
        auto &parms = context_data.parms();
        auto &modulus = context_data.coeff_modulus();
        auto scale_bit_count_bound = context_data.total_coeff_modulus_bit_count();
        size_t coeff_count = parms.degree();
        size_t coeff_modulus_size = modulus.size();
        size_t ciphertext1_size = ciphertext1.size();
        size_t ciphertext2_size = ciphertext2.size();

        // Determine result.size()
        // Default is 3 (c_0, c_1, c_2)
        size_t dest_size = sub_safe(add_safe(ciphertext1_size, ciphertext2_size), size_t(1));
        // Size check
        if (!product_fits_in(dest_size, coeff_count, coeff_modulus_size))
        {
            throw logic_error("invalid parameters");
        }

        // Set up iterator for the base
        auto coeff_modulus = iter(modulus);
        // Prepare result

        ciphertext1.resize(context_, parms.parms_id(), dest_size);

        ciphertext1.is_ntt_form() = true;
        // Set up iterators for input ciphertexts
        PolyIter ciphertext1_iter = iter(ciphertext1);
        ConstPolyIter ciphertext2_iter = iter(ciphertext2);
        auto start = chrono::high_resolution_clock::now();

        if (dest_size == 3)
        {
            // We want to keep six polynomials in the L1 cache: x[0], x[1], x[2], y[0], y[1], temp.
            // For a 32KiB cache, which can store 32768 / 8 = 4096 coefficients, = 682.67 coefficients per polynomial,
            // we should keep the tile size at 682 or below. The tile size must divide coeff_count, i.e. be a power of
            // two. Some testing shows similar performance with tile size 256 and 512, and worse performance on smaller
            // tiles. We pick the smaller of the two to prevent L1 cache misses on processors with < 32 KiB L1 cache.
            size_t tile_size = min<size_t>(coeff_count, size_t(256));
            size_t num_tiles = coeff_count / tile_size;


            // Semantic misuse of RNSIter; each is really pointing to the data for each RNS factor in sequence
            ConstRNSIter ciphertext2_0_iter(*ciphertext2_iter[0], tile_size);
            ConstRNSIter ciphertext2_1_iter(*ciphertext2_iter[1], tile_size);
            RNSIter ciphertext1_0_iter(*ciphertext1_iter[0], tile_size);
            RNSIter ciphertext1_1_iter(*ciphertext1_iter[1], tile_size);
            RNSIter ciphertext1_2_iter(*ciphertext1_iter[2], tile_size);
            //auto stop = chrono::high_resolution_clock::now();
            //auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            //cout << "ggggggggggg TIME: " << duration.count() << " microseconds"<< endl;
            // Temporary buffer to store intermediate results
            POSEIDON_ALLOCATE_GET_COEFF_ITER(temp, tile_size, pool);

            if(is_square) {
                // Computes the output tile_size coefficients at a time
                // Given input tuples of polynomials x = (x[0], x[1], x[2]), y = (y[0], y[1]), computes
                // x = (x[0] * y[0], x[0] * y[1] + x[1] * y[0], x[1] * y[1])
                // with appropriate modular reduction
                POSEIDON_ITERATE(coeff_modulus, coeff_modulus_size, [&](auto I) {
                    POSEIDON_ITERATE(iter(size_t(0)), num_tiles, [&](POSEIDON_MAYBE_UNUSED auto J) {
                        // Compute third output polynomial, overwriting input
                        // x[2] = x[1] * y[1]
                        dyadic_product_coeffmod(
                                ciphertext1_1_iter[0], ciphertext2_1_iter[0], tile_size, I, ciphertext1_2_iter[0]);

                        // Compute second output polynomial, overwriting input
                        dyadic_product_coeffmod(
                                ciphertext1_0_iter[0], ciphertext1_0_iter[0], tile_size, I, ciphertext1_1_iter[0]);
                        // x[1] += x[1]
                        add_poly_coeffmod(ciphertext1_1_iter[0], ciphertext1_1_iter[0], tile_size, I,
                                          ciphertext1_1_iter[0]);


                        // Compute first output polynomial, overwriting input
                        // x[0] = x[0] * y[0]
                        dyadic_product_coeffmod(
                                ciphertext1_0_iter[0], ciphertext2_0_iter[0], tile_size, I, ciphertext1_0_iter[0]);

                        // Manually increment iterators
                        ciphertext1_0_iter++;
                        ciphertext1_1_iter++;
                        ciphertext1_2_iter++;
                        ciphertext2_0_iter++;
                        ciphertext2_1_iter++;
                    });
                });
            }
            else{
                // Computes the output tile_size coefficients at a time
                // Given input tuples of polynomials x = (x[0], x[1], x[2]), y = (y[0], y[1]), computes
                // x = (x[0] * y[0], x[0] * y[1] + x[1] * y[0], x[1] * y[1])
                // with appropriate modular reduction
                POSEIDON_ITERATE(coeff_modulus, coeff_modulus_size, [&](auto I) {
                    POSEIDON_ITERATE(iter(size_t(0)), num_tiles, [&](POSEIDON_MAYBE_UNUSED auto J) {
                        // Compute third output polynomial, overwriting input
                        // x[2] = x[1] * y[1]
                        dyadic_product_coeffmod(
                                ciphertext1_1_iter[0], ciphertext2_1_iter[0], tile_size, I, ciphertext1_2_iter[0]);

                        // Compute second output polynomial, overwriting input


                        // temp = x[1] * y[0]
                        dyadic_product_coeffmod(ciphertext1_1_iter[0], ciphertext2_0_iter[0], tile_size, I, temp);
                        // x[1] = x[0] * y[1]
                        dyadic_product_coeffmod(
                                ciphertext1_0_iter[0], ciphertext2_1_iter[0], tile_size, I, ciphertext1_1_iter[0]);
                        // x[1] += temp
                        add_poly_coeffmod(ciphertext1_1_iter[0], temp, tile_size, I, ciphertext1_1_iter[0]);



                        // Compute first output polynomial, overwriting input
                        // x[0] = x[0] * y[0]
                        dyadic_product_coeffmod(
                                ciphertext1_0_iter[0], ciphertext2_0_iter[0], tile_size, I, ciphertext1_0_iter[0]);

                        // Manually increment iterators
                        ciphertext1_0_iter++;
                        ciphertext1_1_iter++;
                        ciphertext1_2_iter++;
                        ciphertext2_0_iter++;
                        ciphertext2_1_iter++;
                    });
                });
            }
        }
        else
        {
            // Allocate temporary space for the result
            POSEIDON_ALLOCATE_ZERO_GET_POLY_ITER(temp, dest_size, coeff_count, coeff_modulus_size, pool);

            POSEIDON_ITERATE(iter(size_t(0)), dest_size, [&](auto I) {
                // We iterate over relevant components of ciphertext1 and ciphertext2 in increasing order for
                // ciphertext1 and reversed (decreasing) order for ciphertext2. The bounds for the indices of
                // the relevant terms are obtained as follows.
                size_t curr_ciphertext1_last = min<size_t>(I, ciphertext1_size - 1);
                size_t curr_ciphertext2_first = min<size_t>(I, ciphertext2_size - 1);
                size_t curr_ciphertext1_first = I - curr_ciphertext2_first;
                // size_t curr_ciphertext2_last = secret_power_index - curr_ciphertext1_last;

                // The total number of dyadic products is now easy to compute
                size_t steps = curr_ciphertext1_last - curr_ciphertext1_first + 1;

                // Create a shifted iterator for the first input
                auto shifted_ciphertext1_iter = ciphertext1_iter + curr_ciphertext1_first;

                // Create a shifted reverse iterator for the second input
                auto shifted_reversed_ciphertext2_iter = reverse_iter(ciphertext2_iter + curr_ciphertext2_first);

                POSEIDON_ITERATE(iter(shifted_ciphertext1_iter, shifted_reversed_ciphertext2_iter), steps, [&](auto J) {
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
            set_poly_array(temp, dest_size, coeff_count, coeff_modulus_size, ciphertext1.data());
        }

        // Set the scale
        ciphertext1.scale() *= ciphertext2.scale();
        if (ciphertext1.scale() <= 0 || (static_cast<uint32_t >(log2(ciphertext1.scale())) >= scale_bit_count_bound))
        {
            throw invalid_argument("scale out of bounds");
        }
    }

    void CKKSEvaluator_S::relinearize(const Ciphertext &encrypted, const RelinKeys &relin_keys,
                                        Ciphertext &result) const {
        kswitch_->relinearize(encrypted,relin_keys,result);
    }

    void CKKSEvaluator_S::rotate(const Ciphertext &encrypted, int step,const GaloisKeys &galoisKeys, Ciphertext &result) const {
        kswitch_->rotate(encrypted,step,galoisKeys,result);
    }

    void CKKSEvaluator_S::conjugate(const Ciphertext &encrypted, const GaloisKeys &galois_keys, Ciphertext &result) const {
        kswitch_->conjugate(encrypted,galois_keys,result);
    }


    void CKKSEvaluator_S::rescale_inplace(const Ciphertext &ciphertext,Ciphertext &result,MemoryPoolHandle pool) const
{
        if(!ciphertext.is_valid()){
            throw invalid_argument("rescale_inplace : ciphertext is empty");

        }
        if (!ciphertext.is_ntt_form())
        {
            throw invalid_argument("rescale_inplace : ckks encrypted must be in NTT form");
        }
        auto context_data_ptr = context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto &context_data = *context_data_ptr;
        auto &next_context_data = *context_data.next_context_data();
        auto &next_parms = next_context_data.parms();
        auto rns_tool = context_data.rns_tool();
        auto ntt_table = context_.crt_context()->small_ntt_tables();

        size_t encrypted_size = ciphertext.size();
        size_t coeff_count = next_parms.degree();
        size_t next_coeff_modulus_size = next_context_data.coeff_modulus().size();

        Ciphertext encrypted_copy(pool);
        encrypted_copy = ciphertext;
        POSEIDON_ITERATE(iter(encrypted_copy), encrypted_size, [&](auto I) {
            rns_tool->divide_and_round_q_last_ntt_inplace(I,ntt_table , pool);
        });
        result.resize(context_, next_context_data.parms().parms_id(), encrypted_size);
        POSEIDON_ITERATE(iter(encrypted_copy, result), encrypted_size, [&](auto I) {
            set_poly(get<0>(I), coeff_count, next_coeff_modulus_size, get<1>(I));
        });

        // Set other attributes
        result.is_ntt_form() = ciphertext.is_ntt_form();
        result.scale() = ciphertext.scale() / static_cast<double>(context_data.coeff_modulus().back().value());

    }

    void CKKSEvaluator_S::rescale(const Ciphertext &ciphertext,Ciphertext &result ) const {
        rescale_inplace(ciphertext,result);
    }

    void CKKSEvaluator_S::rescale_dynamic(const Ciphertext &ciphertext, Ciphertext &result, double min_scale) const {
        if (!ciphertext.is_ntt_form())
        {
            throw invalid_argument("ckks encrypted must be in NTT form");
        }

        auto context_data = context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto min_scaling_facor_div2 = (min_scale + 1) / 2;
        auto result_scale = ciphertext.scale();
        double scale_tmp = 0.0;
        auto &modulus = context_data->coeff_modulus();
        auto new_level = modulus.size() - 1;
        auto rescale_times = 0;

        while(true){
            scale_tmp = result_scale / safe_cast<double>(modulus[new_level].value());
            if(scale_tmp >= min_scaling_facor_div2){
                if(new_level == 0){
                    throw invalid_argument("rescale_dynamic failed : modulus chain is not enough!");
                }
                result_scale = scale_tmp;
                new_level--;
                rescale_times++;
            }
            else{
                break;
            }
        }


        for(int i = 0; i < rescale_times; i++){
            if(i==0)
                rescale_inplace(ciphertext,result);
            else{
                rescale_inplace(result,result);
            }
        }
    }

    void CKKSEvaluator_S::drop_modulus(const Ciphertext &ciphertext, Ciphertext &result, parms_id_type parms_id) const {
        if (!ciphertext.is_valid())
        {
            throw invalid_argument("drop_modulus : Ciphertext is empty");
        }

        auto encrypted_size = ciphertext.size();
        auto context_data = context_.crt_context()->get_context_data(parms_id);
        auto coeff_modulus_size = context_data->coeff_modulus().size();

        if(&ciphertext == &result){
            auto diff_coeff_modulus_size = ciphertext.coeff_modulus_size() - coeff_modulus_size;
            size_t p = 0;
            for(auto &poly : result.polys()){
                auto drop_num = diff_coeff_modulus_size * p;
                poly.drop(drop_num,coeff_modulus_size);
                p++;
            }
            result.resize(context_,parms_id,encrypted_size);
        }
        else {
            result.resize(context_,parms_id,encrypted_size);
            auto p = 0;
            for(auto &poly : result.polys()){
                poly.copy(ciphertext[p],coeff_modulus_size);
                p++;
            }
            result.is_ntt_form() = ciphertext.is_ntt_form();
            result.scale() = ciphertext.scale();
        }
    }

    void CKKSEvaluator_S::raise_modulus(const Ciphertext &ciphertext, Ciphertext &result) const {
        auto context_data = context_.crt_context()->get_context_data(ciphertext.parms_id());
        auto &coeff_modulus = context_data->coeff_modulus();
        auto first_param_id = context_.crt_context()->first_parms_id();
        auto first_context_data = context_.crt_context()->first_context_data();
        auto &first_coeff_modulus = first_context_data->coeff_modulus();
        auto coeff_modulus_size = ciphertext.coeff_modulus_size();
        auto encrypted_size = ciphertext.size();

        Ciphertext tmp = ciphertext;
        if(ciphertext.is_ntt_form()){
            for(auto i = 0; i < encrypted_size; ++i){
                tmp[i].dot_to_coeff();
            }
        }

        Pointer<RNSBase> base_current;
        try
        {
            base_current = allocate<RNSBase>(pool_, coeff_modulus, pool_);
        }
        catch (const invalid_argument &)
        {
            // Parameters are not valid
            throw invalid_argument("RNSBase's constructor  fail!");
        }

        vector<Modulus> coeff_modulus_raise;
        coeff_modulus_raise.insert(coeff_modulus_raise.end(),first_coeff_modulus.begin() + static_cast<uint32_t >(coeff_modulus_size),first_coeff_modulus.end());
        Pointer<RNSBase> base_raise;
        try
        {
            base_raise = allocate<RNSBase>(pool_, coeff_modulus_raise, pool_);
        }
        catch (const invalid_argument &)
        {
            // Parameters are not valid
            throw invalid_argument("RNSBase's constructor  fail!");
        }
        BaseConverter conv( *base_current, *base_raise,  pool_);

        result.resize(context_,first_param_id,encrypted_size);
        result.scale() = ciphertext.scale();
        for(auto i = 0; i < encrypted_size; ++i){
            result[i].copy(tmp[i],coeff_modulus_size);
            conv.fast_convert_array(tmp[i][0], result[i][coeff_modulus_size], pool_);
            result[i].coeff_to_dot();
        }
        result.is_ntt_form() = true;
    }

    void CKKSEvaluator_S::multiply_relin(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                         const RelinKeys &relin_key) const {
        multiply(ciph1, ciph2, result);
        relinearize(result,relin_key,result);
    }
    void CKKSEvaluator_S::multiply_relin_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,
                                                   const RelinKeys &relin_key) const {

        multiply_dynamic(ciph1, ciph2, result);
        relinearize(result,relin_key,result);
    }

    void CKKSEvaluator_S::sub_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const {
        auto levelA = ciph1.level();
        auto levelB = ciph2.level();
        double scaling_factor_ratio = 0.0;
        Ciphertext tmp_scale;
        bool has_tmp_scale_ciph1 = false;
        bool has_tmp_scale_ciph2 = false;

        if(util::are_close<double>(ciph1.scale(), ciph2.scale())){

        }
        else if(ciph1.scale() > ciph2.scale()){
            scaling_factor_ratio = ciph1.scale() / ciph2.scale();
            scaling_factor_ratio += 0.5;
            if(scaling_factor_ratio < min_scale_){
                throw invalid_argument("sub_dynamic : ciphertext scale don't support! ");
            }
            multiply_const(ciph2,scaling_factor_ratio,1.0,tmp_scale,encoder);
            tmp_scale.scale() = ciph1.scale();
            has_tmp_scale_ciph2 = true;
        }
        else{
            scaling_factor_ratio = ciph2.scale() / ciph1.scale();
            scaling_factor_ratio += 0.5;
            if(scaling_factor_ratio < min_scale_){
                throw invalid_argument("sub_dynamic : ciphertext scale don't support! ");
            }
            multiply_const(ciph1,scaling_factor_ratio,1.0,tmp_scale,encoder);
            tmp_scale.scale() = ciph2.scale();
            has_tmp_scale_ciph1 = true;
        }


        if(levelA > levelB){
            Ciphertext tmp;
            if(&result == &ciph2 ){
                if( !has_tmp_scale_ciph1)
                    drop_modulus(ciph1,tmp,ciph2.parms_id());
                else{
                    drop_modulus(tmp_scale,tmp,ciph2.parms_id());
                }
                if(has_tmp_scale_ciph2)
                    sub( tmp, tmp_scale,result);
                else
                    sub( tmp, ciph2,result);
            }
            else{
                if( !has_tmp_scale_ciph1)
                    drop_modulus(ciph1,result,ciph2.parms_id());
                else{
                    drop_modulus(tmp_scale,result,ciph2.parms_id());
                }

                if(has_tmp_scale_ciph2)
                    sub( result, tmp_scale,result);
                else
                    sub( result, ciph2,result);
            }
        }
        else if(levelB > levelA){
            Ciphertext tmp;
            if(&result == &ciph1 ){
                if( !has_tmp_scale_ciph2)
                    drop_modulus(ciph2,tmp,ciph1.parms_id());
                else{
                    drop_modulus(tmp_scale,tmp,ciph1.parms_id());
                }


                if(has_tmp_scale_ciph1)
                    sub( tmp_scale, tmp,result);
                else
                    sub( ciph1, tmp,result);

            }
            else{
                if( !has_tmp_scale_ciph2)
                    drop_modulus(ciph2,result,ciph1.parms_id());
                else{
                    drop_modulus(tmp_scale,result,ciph1.parms_id());
                }

                if(has_tmp_scale_ciph1)
                    sub( tmp_scale, result,result);
                else
                    sub( ciph1, result,result);
            }
        }
        else{
            if( has_tmp_scale_ciph1){
                sub( tmp_scale, ciph2,result);
            }
            else if(has_tmp_scale_ciph2){
                sub( ciph1, tmp_scale,result);
            }
            else{
                sub( ciph1, ciph2,result);
            }
        }
    }

    void CKKSEvaluator_S::add_dynamic(const Ciphertext &ciph1, const Ciphertext &ciph2, Ciphertext &result,const CKKSEncoder &encoder) const {
        auto levelA = ciph1.level();
        auto levelB = ciph2.level();
        double scaling_factor_ratio = 0.0;
        Ciphertext tmp_scale;
        bool has_tmp_scale_ciph1 = false;
        bool has_tmp_scale_ciph2 = false;

        if(util::are_close<double>(ciph1.scale(), ciph2.scale())){

        }
        else if(ciph1.scale() > ciph2.scale()){
            scaling_factor_ratio = ciph1.scale() / ciph2.scale();

            scaling_factor_ratio += 0.5;
            if(scaling_factor_ratio < min_scale_){
                throw invalid_argument("add_dynamic : ciphertext scale don't support! ");
            }
            //scaling_factor_int = safe_cast<int>(scaling_factor_ratio);
            multiply_const(ciph2,scaling_factor_ratio,1.0,tmp_scale,encoder);
            tmp_scale.scale() = ciph1.scale();
            has_tmp_scale_ciph2 = true;
        }
        else{
            scaling_factor_ratio = ciph2.scale() / ciph1.scale();
            scaling_factor_ratio += 0.5;
            if(scaling_factor_ratio < min_scale_){
                throw invalid_argument("add_dynamic : ciphertext scale don't support! ");
            }
            multiply_const(ciph2,scaling_factor_ratio,1.0,tmp_scale,encoder);
            tmp_scale.scale() = ciph2.scale();

            has_tmp_scale_ciph1 = true;

        }
        if(levelA > levelB){
            Ciphertext tmp;
            if(&result == &ciph2 ){
                if( !has_tmp_scale_ciph1)
                    drop_modulus(ciph1,tmp,ciph2.parms_id());
                else{
                    drop_modulus(tmp_scale,tmp,ciph2.parms_id());
                }
                if(has_tmp_scale_ciph2)
                    add( tmp, tmp_scale,result);
                else
                    add( tmp, ciph2,result);
            }
            else{
                if( !has_tmp_scale_ciph1)
                    drop_modulus(ciph1,result,ciph2.parms_id());
                else{
                    drop_modulus(tmp_scale,result,ciph2.parms_id());
                }

                if(has_tmp_scale_ciph2)
                    add( result, tmp_scale,result);
                else
                    add( result, ciph2,result);
            }
        }
        else if(levelB > levelA){
            Ciphertext tmp;
            if(&result == &ciph1 ){
                if( !has_tmp_scale_ciph2)
                    drop_modulus(ciph2,tmp,ciph1.parms_id());
                else{
                    drop_modulus(tmp_scale,tmp,ciph1.parms_id());
                }


                if(has_tmp_scale_ciph1)
                    add( tmp_scale, tmp,result);
                else
                    add( ciph1, tmp,result);

            }
            else{
                if( !has_tmp_scale_ciph2)
                    drop_modulus(ciph2,result,ciph1.parms_id());
                else{
                    drop_modulus(tmp_scale,result,ciph1.parms_id());
                }

                if(has_tmp_scale_ciph1)
                    add( tmp_scale, result,result);
                else
                    add( ciph1, result,result);
            }
        }
        else{
            if( has_tmp_scale_ciph1){
                add( tmp_scale, ciph2,result);
            }
            else if(has_tmp_scale_ciph2){
                add( ciph1, tmp_scale,result);
            }
            else{
                add( ciph1, ciph2,result);
            }
        }
    }

    void CKKSEvaluator_S::read(Ciphertext &ciph) const {

    }

    void CKKSEvaluator_S::read(Plaintext &plain) const {

    }


}