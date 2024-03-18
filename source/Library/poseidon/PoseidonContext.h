

#ifndef CKKS_PARAM_H
#define CKKS_PARAM_H

#include "poseidon/ParametersLiteral.h"
#include "poseidon/CrtContext.h"
#include "poseidon/seal/randomgen.h"
#include "poseidon/pke_params_defines.h"

using namespace std;
namespace poseidon
{
  class PoseidonContext{
    private:
      std::shared_ptr<const poseidon::ParametersLiteral> parameters_literal_{nullptr};
      std::shared_ptr<CrtContext> crt_context_{nullptr};
      std::shared_ptr<UniformRandomGeneratorFactory> random_generator_{nullptr};

      sec_level_type sec_level_ = sec_level_type::none;
      KeySwitchVariant key_switch_variant_ = GHS;
      bool using_hardware_ = false;

    public:
      explicit PoseidonContext(const ParametersLiteral& paramLiteral,sec_level_type sec_level = sec_level_type::none,bool using_hardware = false);
      PoseidonContext(const PoseidonContext &context) = default;
      ~PoseidonContext() = default;

        POSEIDON_NODISCARD inline  shared_ptr<const poseidon::ParametersLiteral> parameters_literal() const {
          return parameters_literal_;
        }

        POSEIDON_NODISCARD inline  KeySwitchVariant key_switch_variant() const {
            return key_switch_variant_;
        }

        POSEIDON_NODISCARD inline shared_ptr<CrtContext> crt_context() const {
            return crt_context_;
        }

      POSEIDON_NODISCARD inline  bool using_hardware() const {
          return using_hardware_;
      }





        inline void set_random_generator(std::shared_ptr<UniformRandomGeneratorFactory> random_generator) noexcept
        {
            random_generator_ = std::move(random_generator);
        }
        POSEIDON_NODISCARD inline std::shared_ptr<UniformRandomGeneratorFactory> random_generator() const noexcept
        {
            return random_generator_;
        }
      /**
        Getter methods.
        */
        /*
      void set_random_generator(std::shared_ptr<UniformRandomGeneratorFactory> random_generator) noexcept;
      std::shared_ptr<UniformRandomGeneratorFactory> random_generator() const noexcept;

      std::shared_ptr<poseidon::CrtContext> crt_context() const;
      std::shared_ptr<RNSTool> rns_tool(uint32_t index) const;
      inline std::shared_ptr<poseidon::util::GaloisTool> galois_tool() const
      {
          return this->galois_tool_;
      }


      const SchemeType scheme_type() const;
      const mpf_class scaling_factor() const;
      const uint32_t hamming_weight() const;
      const uint32_t poly_degree() const;
      const uint32_t plain_modulus() const;
         */
  };

  
}

#endif









