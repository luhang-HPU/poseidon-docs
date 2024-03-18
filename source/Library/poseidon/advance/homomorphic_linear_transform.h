//
// Created by lxs on 23-6-19.
//

#ifndef POSEIDON_LINEAR_TRANSFORM_H
#define POSEIDON_LINEAR_TRANSFORM_H
#include <iostream>
#include <vector>
#include <map>
#include "poseidon/CKKSEncoder.h"
//#include "BatchEncoder.h"
#include "poseidon/util/matrix_operation.h"
using namespace std;



namespace poseidon {
    typedef map<int, vector<int>> IndexMap;
    bool IsInSliceInt(int x,const vector<int>& slice);

    class MatrixPlain{
    private:

    public:
        uint32_t LogSlots = 0;
        uint32_t N1 = 0;
        uint32_t level = 0;
        double scale = 1.0;
        //map <int,Plaintext> plain_vec;  //<index,plaintext>
        vector<int> rot_index{};
        map <int,Plaintext> plain_vec;
        MatrixPlain() = default;
    };

    class LinearMatrixGroup{
    public:
        LinearMatrixGroup() = default;
        inline  vector<MatrixPlain> &data() noexcept{
            return matrices_;
        }

        POSEIDON_NODISCARD inline  const vector<MatrixPlain> &data() const noexcept{
            return matrices_;
        }


        POSEIDON_NODISCARD inline  vector<int> &rot_index()  noexcept{
            return rotate_index_;
        }

        POSEIDON_NODISCARD inline  const vector<int> &rot_index()  const noexcept{
            return rotate_index_;
        }

        POSEIDON_NODISCARD inline   uint32_t step() const noexcept {
            return scalar_step_;
        }

        inline void set_step(uint32_t step)  noexcept {
            scalar_step_ = step;
        }
    private:
        vector<MatrixPlain> matrices_{};
        vector<int> rotate_index_{};
        uint32_t scalar_step_ = 0;
    };

    template<typename T>
    tuple<IndexMap, vector<int>, vector<int> >  BSGSIndex(const T& el, int slots, int N1) {
        std::map<int,bool> rotN1Map;
        std::map<int,bool> rotN2Map;
        std::vector<int> nonZeroDiags;

        IndexMap index;
        vector<int> rotN1, rotN2;
        // Convert el to nonZeroDiags
        nonZeroDiags.reserve(el.size());
        for (typename T::const_iterator it = el.begin(); it != el.end(); ++it) {
            nonZeroDiags.push_back(it->first);
        }

        for (int rot : nonZeroDiags) {
            rot &= (slots - 1);
            int idxN1 = ((rot / N1) * N1) & (slots - 1);
            int idxN2 = rot & (N1 - 1);
            index[idxN1].push_back(idxN2);
            if (rotN1Map.find(idxN1) == rotN1Map.end()) {
                rotN1Map[idxN1] = true;
                rotN1.push_back(idxN1);
            }
            rotN2Map[idxN2] = true;
        }

        rotN2.clear();
        for (auto & it : rotN2Map) {
            rotN2.push_back(it.first);
        }

        return make_tuple(index, rotN1, rotN2);
    }

    template<typename T>
    int FindBestBSGSRatio(const T& diagMatrix, int maxN, int logMaxRatio) {
        auto maxRatio = double(1 << logMaxRatio);

        for (int N1 = maxN; N1 >= 2; N1 >>= 1) {
            vector<int> rotN1, rotN2;
            IndexMap index;
            tie(index, rotN1, rotN2) = BSGSIndex(diagMatrix, maxN, N1);

            auto nbN1 = rotN1.size() - 1, nbN2 = rotN2.size() - 1;
            if (double(nbN1) / double(nbN2) == maxRatio) {
                return N1;
            }

            if ( double(nbN1)/ double(nbN2) > maxRatio) {
                return N1 * 2;
            }
        }

        return 1;
    }

    template<typename T>
    void copyRotInterface(vector<T> &a, vector<T> &b ,  int rot){
        size_t n = a.size();

        if(b.size() >= rot){
            copy(b.begin()+rot, b.end(), a.begin());
            copy(b.begin(),b.begin()+rot,a.begin()+n-rot);

        }
        else{
            copy(b.begin(),b.end(),a.begin()+n-rot);

        }

    }

    template<typename T>
    void copyRotInterface_bfv(vector<T> &a, vector<T> &b ,  int rot){
        //size_t n = a.size();
        //auto slots = n >> 1;
        a = matrix_operations::rotate_slots_vec(b,-rot);
    }



    template<typename T>
    void addMatrixRotToList(const  map<int, vector<T>> &value,vector<int> &rot_index,int N1,int slots,bool repack){
        int index = 0;
        bool exist;
        rot_index.push_back(0); //for conjugate
        for(auto i : value){
            index = (i.first / N1) *N1;

            if (repack){
                // Sparse repacking, occurring during the first IDFT matrix.
                index &= (2*slots - 1);
            } else {
                // Other cases
                index &= (slots - 1);
            }
            exist = IsInSliceInt(index, rot_index);
            if (index != 0 && !exist) {
                rot_index.push_back(index);
            }

            index = i.first & (N1 - 1);
            exist = IsInSliceInt(index, rot_index);
            if (index != 0 && !exist) {
                rot_index.push_back(index);
            }
        }
    }

    template<typename T>
    void GenLinearTransformBSGS(MatrixPlain &plain_mat,vector<int>& rotate_index, CKKSEncoder &encoder,  map<int, vector<T>> &value, uint32_t level ,double scale, uint32_t LogBSGSRatio, uint32_t  logSlots){
        auto slots = 1 << logSlots;
        auto N1 = FindBestBSGSRatio(value, slots, LogBSGSRatio);
        auto parms_id_map = encoder.context().crt_context()->parms_id_map();
        auto parms_id = parms_id_map.at(level);
        plain_mat.N1 = N1;
        plain_mat.LogSlots = logSlots;
        plain_mat.level = level;
        plain_mat.scale = scale;

        addMatrixRotToList(value,rotate_index,N1,slots,false);
        auto [index,n1,n2] = BSGSIndex(value,slots,N1);
        vector<T> values(slots);

        for(auto j : index){
            int a = 0;
            auto rot = -(j.first) & (slots - 1);
            for (auto i : index[j.first]) {
                copyRotInterface(values,value[j.first+i] , rot);
                encoder.encode(values, parms_id,scale,plain_mat.plain_vec[j.first+i]);
            }
        }
    }

    template<typename T>
    void GenMatrixformBSGS(MatrixPlain &plain_mat,vector<int>& rotate_index, CKKSEncoder &encoder, vector<vector<T >> mat_data , uint32_t level ,double scale, uint32_t LogBSGSRatio, uint32_t logSlots){

        map<int, vector<T>> value;
        for(int i = 0; i < mat_data.size(); i++){
            value[i] = mat_data[i];
        }
        auto parms_id_map = encoder.context().crt_context()->parms_id_map();
        auto parms_id = parms_id_map.at(level);
        auto slots = 1 << logSlots;
        auto N1 = FindBestBSGSRatio(value, slots, LogBSGSRatio);
        plain_mat.N1 = N1;
        plain_mat.LogSlots = logSlots;
        plain_mat.level = level;
        plain_mat.scale = scale;

        addMatrixRotToList(value,rotate_index,N1,slots,false);
        auto [index,n1,n2] = BSGSIndex(value,slots,N1);
        vector<T> values(slots);

        for(auto j : index){
            int a = 0;
            auto rot = -(j.first) & (slots - 1);
            for (auto i : index[j.first]) {
                //plain_mat.plain_vec[j+i]

                copyRotInterface(values,value[j.first+i] , rot);
                encoder.encode(values, parms_id,scale,plain_mat.plain_vec[j.first+i]);

            }
        }
    }

//    template<typename T>
//    void GenMatrixformBSGS(MatrixPlain &plain_mat, vector<int>& rotate_index, BatchEncoder &encoder, vector<vector<T >> mat_data , int level , mpf_class scale, int LogBSGSRatio, int  logSlots){
////        Plaintext a;
////        encoder.encode(mat_data[0],a);
//        map<int, vector<T>> value;
//        for(int i = 0; i < mat_data.size(); i++){
//            value[i] = mat_data[i];
//        }
//
//        auto slots = 1 << logSlots;
//        auto N1 = FindBestBSGSRatio(value, slots, LogBSGSRatio);
//        plain_mat.N1 = N1;
//        plain_mat.LogSlots = logSlots;
//        plain_mat.level = level;
//        plain_mat.scale = scale;
//
//        addMatrixRotToList(value,rotate_index,N1,slots,false);
//        auto [index,n1,n2] = BSGSIndex(value,slots,N1);
//        vector<T> values(slots);
//
//        for(auto j : index){
//            int a = 0;
//            //auto rot = -(j.first) & (slots - 1);
//            auto rot = j.first;
//            for (auto i : index[j.first]) {
//                //plain_mat.plain_vec[j+i]
//
//                copyRotInterface_bfv(values,value[j.first+i] , rot);
//                encoder.encode(values,plain_mat.plain_vec[j.first+i],level);
//                //encoder.encode(values,a);
//
//            }
//        }
//    }



}








#endif //POSEIDON_LINEAR_TRANSFORM_H
