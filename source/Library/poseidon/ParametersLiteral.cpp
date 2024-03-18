//
// Created by lxs on 5/19/23.
//

#include "poseidon/ParametersLiteral.h"
#include "poseidon/seal/util/numth.h"


namespace poseidon {
    using namespace util;
    ParametersLiteral::ParametersLiteral(SchemeType Type, uint32_t LogN, uint32_t LogSlots,
                      uint32_t LogScale, uint32_t HammingWeight, uint32_t q0_level, Modulus plain_modulus, const vector<Modulus> &Q, const vector<Modulus> &P,MemoryPoolHandle pool)
        :Type_(Type),LogN_(LogN),LogSlots_(LogSlots),Q_(Q),P_(P),LogScale_(LogScale),HammingWeight_(HammingWeight),plain_modulus_(plain_modulus),q0_level_(q0_level),pool_(std::move(pool))
    {
        compute_params_id();
    }

    void ParametersLiteral::set_log_modulus(const vector<uint32_t>& logQ,const vector<uint32_t>& logP) {
        //<prime_size, primes_num>
        auto factor = (uint64_t )2 << this->LogN_;
        //<prime_size, primes_vector>
        unordered_map <uint32_t, vector<Modulus>> primes_map;
        unordered_map<uint32_t, int> primes_num_map;
        for (unsigned int i : logQ) {
            //unordered_map<uint32_t, int>::iterator f = primes_num_map.find(logQ[i]);
            auto f = primes_num_map.find(i);
            if (f != primes_num_map.end()) {
                primes_num_map[i] += 1;
            } else {
                primes_num_map[i] = 1;
            }
        }

        for (unsigned int i : logP) {
            auto f = primes_num_map.find(i);
            if (f != primes_num_map.end()) {
                primes_num_map[i] += 1;
            } else {
                primes_num_map[i] = 1;
            }
        }

        for (auto p: primes_num_map) {
            //key : p.first--- prime_size  value:p.second--- prime_num
            //primes_map[p.first] = get_primes(factor,p.first,p.second);
            primes_map[p.first] = get_primes_raise_no_check(factor, safe_cast<int>( p.first),p.second);
        }

        for(auto p :  logQ){
            Q_.push_back(primes_map[p][0]);
            primes_map[p].erase(primes_map[p].begin());
        }
        for(auto p :  logP){
            P_.push_back(primes_map[p][0]);
            primes_map[p].erase(primes_map[p].begin());
        }
        compute_params_id();
    }

    void ParametersLiteral::compute_params_id() {
        auto q_size = Q_.size();
        auto p_size = P_.size();
        auto total_uint64_count = add_safe(size_t(1),size_t(1),size_t(1),q_size,p_size,size_t(1),size_t(1),size_t(1),size_t(1));

        auto parms_data = allocate_uint(total_uint64_count,pool_);
        uint64_t *plain_data_ptr =  parms_data.get();

        *plain_data_ptr++ = static_cast<uint64_t>(Type_);
        *plain_data_ptr++ = static_cast<uint64_t>(LogN_);
        *plain_data_ptr++ = static_cast<uint64_t>(LogSlots_);
        for(const auto &mod : Q_){
            *plain_data_ptr++ = mod.value();
        }
        for(const auto &mod : P_){
            *plain_data_ptr++ = mod.value();
        }

        *plain_data_ptr++ = static_cast<uint64_t>(LogScale_);
        *plain_data_ptr++ = static_cast<uint64_t>(HammingWeight_);
        *plain_data_ptr++ = static_cast<uint64_t>(plain_modulus_.value());
        *plain_data_ptr++ = static_cast<uint64_t>(q0_level_);
        HashFunction::hash(parms_data.get(), total_uint64_count, params_id_);
    }



    const map<size_t, tuple<vector<uint32_t>,vector<uint32_t>,uint32_t> > &GetDefaultLogCoeffModulus128()
    {
        static const map<size_t,tuple<vector<uint32_t>,vector<uint32_t>,uint32_t> > default_log_coeff_modulus_128{
                /*
                Polynomial modulus: 1x^1024 + 1
                Modulus count: 1
                Total bit count: 27
                */
                { 1024,
                        {
                                {26 },
                                {},
                                25
                        }},

                /*
                Polynomial modulus: 1x^2048 + 1
                Modulus count: 1
                Total bit count: 54
                */
                { 2048,
                        {
                                {53 },
                                {},
                                45
                        }},


                /*
                Polynomial modulus: 1x^4096 + 1
                Modulus count: 3
                Total bit count: 109 = 2 * 36 + 37
                */
                { 4096,
                        {
                                {35, 35 },
                                {36},
                                36
                        }},

                /*
                Polynomial modulus: 1x^8192 + 1
                Modulus count: 5
                Total bit count: 218 = 2 * 43 + 3 * 44
                */
                { 8192,
                        {
                                {42, 42, 43, 43},
                                {43},
                                43
                        }},


                /*
                Polynomial modulus: 1x^16384 + 1
                Modulus count: 9
                Total bit count: 438 = 3 * 48 + 6 * 49
                */
                { 16384,
                        {
                                {48, 48, 48, 49, 49,49, 49, 49 },
                                {48},
                                49
                        }},

                /*
                Polynomial modulus: 1x^32768 + 1
                Modulus count: 16
                Total bit count: 881 = 15 * 55 + 56
                */
                { 32768,
                        {
                                {55, 55, 55, 55, 55,
                                                    55, 55, 55, 55, 55,
                                        55, 55, 55, 55,55},
                                {55},
                                55
                        }}
        };

        return default_log_coeff_modulus_128;
    }


    const map<size_t, tuple<vector<uint32_t>,vector<uint32_t>,uint32_t> > &GetDefaultLogCoeffModulus192()
    {
        static const map<size_t,tuple<vector<uint32_t>,vector<uint32_t>,uint32_t> > default_log_coeff_modulus_192{
                /*
                Polynomial modulus: 1x^1024 + 1
                Modulus count: 1
                Total bit count: 19
                */
                { 1024,
                        {
                                {18 },
                                {},
                                15
                        }},

                /*
                Polynomial modulus: 1x^2048 + 1
                Modulus count: 1
                Total bit count: 37
                */
                { 2048,
                        {
                                {36 },
                                {},
                                33
                        }},


                /*
                Polynomial modulus: 1x^4096 + 1
                Modulus count: 3
                Total bit count: 75 = 3 * 25
                */
                { 4096,
                        {
                                {24, 24 },
                                {24},
                                22
                        }},

                /*
                Polynomial modulus: 1x^8192 + 1
                Modulus count: 4
                Total bit count: 152 = 4 * 38
                */
                { 8192,
                        {
                                {37, 37, 37},
                                {38},
                                38
                        }},


                /*
                Polynomial modulus: 1x^16384 + 1
                Modulus count: 6
                Total bit count: 300 = 6 * 50
                */
                { 16384,
                        {
                                {49, 49, 49, 49, 49 },
                                {49},
                                49
                        }},

                /*
                Polynomial modulus: 1x^32768 + 1
                Modulus count: 11
                Total bit count: 600 = 5 * 54 + 6 * 55
                */
                { 32768,
                        {
                                {54, 54, 54, 54, 54,54, 55, 55, 55, 55},
                                {55},
                                55
                        }}
        };

        return default_log_coeff_modulus_192;
    }

    const map<size_t, tuple<vector<uint32_t>,vector<uint32_t>,uint32_t > > &GetDefaultLogCoeffModulus256()
    {
        static const map<size_t,tuple<vector<uint32_t>,vector<uint32_t>,uint32_t > > default_log_coeff_modulus_256{
                /*
                Polynomial modulus: 1x^1024 + 1
                Modulus count: 1
                Total bit count: 14
                */
                { 1024,
                        {
                                {13 },
                                {},
                                12
                        }},

                /*
                Polynomial modulus: 1x^2048 + 1
                Modulus count: 1
                Total bit count: 29
                */
                { 2048,
                        {
                                {28 },
                                {},
                                25
                        }},


                /*
               Polynomial modulus: 1x^4096 + 1
               Modulus count: 1
               Total bit count: 58
               */
                { 4096,
                        {
                                {57 },
                                {},
                                45
                        }},

                /*
                Polynomial modulus: 1x^8192 + 1
                Modulus count: 3
                Total bit count: 118 = 2 * 39 + 40
                */
                { 8192,
                        {
                                {39, 39},
                                {39},
                                39
                        }},


                /*
                 Polynomial modulus: 1x^16384 + 1
                 Modulus count: 5
                 Total bit count: 237 = 3 * 47 + 2 * 48
                 */
                { 16384,
                        {
                                {47, 47, 47, 47 },
                                {48},
                                47
                        }},

                /*
                Polynomial modulus: 1x^32768 + 1
                Modulus count: 9
                Total bit count: 476 = 52 + 8 * 53
                */
                { 32768,
                        {
                                {52, 52, 53, 53, 53,53, 53, 53 },
                                {53},
                                53
                        }}
        };

        return default_log_coeff_modulus_256;
    }

    const map<size_t, tuple<vector<uint32_t>,vector<uint32_t>,uint32_t > > &GetDefaultLogCoeffModulusnone()
    {
        static const map<size_t,tuple<vector<uint32_t>,vector<uint32_t>,uint32_t > > default_log_coeff_modulus_none{
                /*
                Polynomial modulus: 1x^1024 + 1
                Modulus count: 1
                Total bit count: 14
                */
                { 1024,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }},

                /*
                Polynomial modulus: 1x^2048 + 1
                Modulus count: 1
                Total bit count: 29
                */
                { 2048,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }},


                /*
               Polynomial modulus: 1x^4096 + 1
               Modulus count: 1
               Total bit count: 58
               */
                { 4096,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }},

                /*
                Polynomial modulus: 1x^8192 + 1
                Modulus count: 3
                Total bit count: 118 = 2 * 39 + 40
                */
                { 8192,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }},


                /*
                 Polynomial modulus: 1x^16384 + 1
                 Modulus count: 5
                 Total bit count: 237 = 3 * 47 + 2 * 48
                 */
                { 16384,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }},

                /*
                Polynomial modulus: 1x^32768 + 1
                Modulus count: 9
                Total bit count: 476 = 52 + 8 * 53
                */
                { 32768,
                        {
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                {31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31,31, 31, 31, 31, 31 },
                                40
                        }}
        };

        return default_log_coeff_modulus_none;
    }






    ParametersLiteralDefault::ParametersLiteralDefault(SchemeType schemeType,uint32_t degree,sec_level_type sec_level,MemoryPoolHandle pool )
        : ParametersLiteral()
    {
        this->pool_ = std::move(pool);
        init(schemeType,degree, sec_level);
    };



    void ParametersLiteralDefault::init(SchemeType schemeType,uint32_t degree,sec_level_type sec_level){
        this->Type_ = schemeType;
        this->q0_level_ = 0;
        this->LogN_ = static_cast<uint32_t> (log2(static_cast<double >(degree)));
        this->LogSlots_ = this->LogN_-1;
        this->HammingWeight_ = 5;
        this->plain_modulus_ = 0;

        switch (sec_level) {
            case sec_level_type::none :
                set_log_modulus(std::get<0>(GetDefaultLogCoeffModulusnone().at(degree)), std::get<1>(GetDefaultLogCoeffModulusnone().at(degree)));
                this->LogScale_ = std::get<2>(GetDefaultLogCoeffModulusnone().at(degree));
                break;
            case sec_level_type::tc128 :
                set_log_modulus(std::get<0>(GetDefaultLogCoeffModulus128().at(degree)), std::get<1>(GetDefaultLogCoeffModulus128().at(degree)));
                this->LogScale_ = std::get<2>(GetDefaultLogCoeffModulus128().at(degree));
                break;
            case sec_level_type::tc192 :
                set_log_modulus(std::get<0>(GetDefaultLogCoeffModulus192().at(degree)), std::get<1>(GetDefaultLogCoeffModulus192().at(degree)));
                this->LogScale_ = std::get<2>(GetDefaultLogCoeffModulus192().at(degree));
                break;
            case sec_level_type::tc256 :
                set_log_modulus(std::get<0>(GetDefaultLogCoeffModulus256().at(degree)), std::get<1>(GetDefaultLogCoeffModulus256().at(degree)));
                this->LogScale_ = std::get<2>(GetDefaultLogCoeffModulus256().at(degree));
                break;
            default:
                set_log_modulus(std::get<0>(GetDefaultLogCoeffModulus128().at(degree)), std::get<1>(GetDefaultLogCoeffModulus128().at(degree)));
                this->LogScale_ = std::get<2>(GetDefaultLogCoeffModulus128().at(degree));
        }
    }
} // poseidon