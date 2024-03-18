//
// Created by cloudam on 12/20/23.
//

#ifndef POSEIDON_FFT_H
#define POSEIDON_FFT_H
#include <complex>
#include "poseidon/seal/util/common.h"
#include "poseidon/seal/util/croots.h"
#include "poseidon/seal/util/defines.h"
#include "poseidon/seal/util/dwthandler.h"
#include "poseidon/seal/memorymanager.h"
using namespace std;
namespace poseidon {
    template<
            typename T_out, typename = std::enable_if_t<
                    std::is_same<std::remove_cv_t<T_out>, double>::value ||
                    std::is_same<std::remove_cv_t<T_out>, std::complex<double>>::value>>
    POSEIDON_NODISCARD inline T_out from_complex(std::complex<double> in);

    template<>
    POSEIDON_NODISCARD inline double from_complex(std::complex<double> in) {
        return in.real();
    }

    template<>
    POSEIDON_NODISCARD inline std::complex<double> from_complex(std::complex<double> in) {
        return in;
    }

    namespace util {
        template<>
        class Arithmetic<std::complex<double>, std::complex<double>, double> {
        public:
            Arithmetic() = default;

            POSEIDON_NODISCARD static inline std::complex<double> add(const std::complex<double> &a, const std::complex<double> &b) {
                return a + b;
            }

            POSEIDON_NODISCARD static inline std::complex<double> sub(const std::complex<double> &a, const std::complex<double> &b) {
                return a - b;
            }

            POSEIDON_NODISCARD static inline std::complex<double> mul_root(const std::complex<double> &a, const std::complex<double> &r) {
                return a * r;
            }

            POSEIDON_NODISCARD static inline std::complex<double> mul_scalar(const std::complex<double> &a, const double &s) {
                return a * s;
            }

            POSEIDON_NODISCARD static inline std::complex<double> mul_root_scalar(const std::complex<double> &r, const double &s) {
                return r * s;
            }

            POSEIDON_NODISCARD static inline std::complex<double> guard(const std::complex<double> &a) {
                return a;
            }
        };
     // namespace util

        using ComplexArith = util::Arithmetic<std::complex<double>, std::complex<double>, double>;
        using FFTHandler = util::DWTHandler<std::complex<double>, std::complex<double>, double>;

        class FFT {
        public:
            explicit FFT(size_t coeff_count,MemoryPoolHandle pool = MemoryManager::GetPool());
            void embedding_inv(const vector<std::complex<double>> &vec,vector<std::complex<double>> &vec_res) const;
            void embedding(const vector<std::complex<double>> &vec,vector<std::complex<double>> &vec_res) const;


        private:
            uint32_t coeff_count_ = 0;
            std::shared_ptr<util::ComplexRoots> complex_roots_;

            // Holds 1~(n-1)-th powers of root in bit-reversed order, the 0-th power is left unset.
            util::Pointer<std::complex<double>> root_powers_;

            // Holds 1~(n-1)-th powers of inverse root in scrambled order, the 0-th power is left unset.
            util::Pointer<std::complex<double>> inv_root_powers_;

            util::Pointer<std::size_t> matrix_reps_index_map_;



            ComplexArith complex_arith_;

            FFTHandler fft_handler_;
            MemoryPoolHandle pool_;
        };

    }

#endif //POSEIDON_FFT_H
}