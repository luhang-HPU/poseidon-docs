//
// Created by cloudam on 12/25/23.
//

#ifndef POSEIDON_FFTHANDLER_H
#define POSEIDON_FFTHANDLER_H
#include "poseidon/seal/util/dwthandler.h"

namespace poseidon{
    namespace util{
        template <typename ValueType, typename RootType,typename IndexType, typename ScalarType>
        class FFTHandler {

        public:
            FFTHandler()= default;

            explicit FFTHandler(const Arithmetic<ValueType, RootType, ScalarType> &num_struct, const IndexType &rou_group) : arithmetic_(num_struct),rou_group_(rou_group)
            {}

            void embedding_inv(ValueType *values, int log_n, const RootType *roots,const RootType *rou_group, const ScalarType *scalar = nullptr) const;

            void embedding(ValueType *values, int log_n, const RootType *roots, const RootType *rou_group,const ScalarType *scalar = nullptr) const;

        private:
            Arithmetic<ValueType, RootType, ScalarType> arithmetic_;
            IndexType rou_group_;
        };

        template<typename ValueType, typename RootType,typename IndexType, typename ScalarType>
        void FFTHandler<ValueType, RootType,IndexType, ScalarType>::embedding(ValueType *values, int log_n, const RootType *roots,const RootType *rou_group,
                                                                    const ScalarType *scalar) const {




        }

        template<typename ValueType, typename RootType,typename IndexType, typename ScalarType>
        void FFTHandler<ValueType, RootType,IndexType, ScalarType>::embedding_inv(ValueType *values, int log_n, const RootType *roots,const RootType *rou_group,
                                                                   const ScalarType *scalar) const {

            // constant transform size
            size_t n = size_t(1) << log_n;
            size_t slot = n >> 1;
            // registers to hold temporary values
            RootType r;
            ValueType u;
            ValueType v;
            // pointers for faster indexing
            ValueType *x = nullptr;
            ValueType *y = nullptr;
            // variables for indexing
            std::size_t gap = 1;
            std::size_t m = n >> 1;
            size_t idx = 0;
            size_t rou_idx = 0;
            size_t index_even = 0;
            size_t index_odd = 0;

            /*
            for (; m > 1; m >>= 1){
                idx = 1 << (m + 2);
                for(auto j = 0; j < slot; j = j + (1 << m)){
                    for(auto i = 0; i < (1 << (m - 1)); i++){
                        index_even = j + i;
                        index_odd = j + i + (1 << (m - 1));
                        rou_idx =

                    }
                }

                gap <<= 1;

            }

            */
        }


    }
}

#endif //POSEIDON_FFTHANDLER_H
