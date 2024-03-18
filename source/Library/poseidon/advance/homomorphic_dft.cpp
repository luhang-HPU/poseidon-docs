#include <complex>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cmath>
#include "poseidon/advance/homomorphic_dft.h"
namespace poseidon{
    using namespace std;

    HomomorphicDFTMatrixLiteral::HomomorphicDFTMatrixLiteral(LinearType type, uint32_t logN, uint32_t logSlots,
                                                             uint32_t levelStart, vector<uint32_t> levels,
                                                             bool repackImag2Real,
                                                             double scaling,
                                                             bool bitReversed,
                                                             uint32_t logBSGSRatio)

            : type_(type), logN_(logN), logSlots_(logSlots), levelStart_(levelStart),
              levels_(std::move(levels)), repackImag2Real_(repackImag2Real),
              scaling_(scaling), bitReversed_(bitReversed), logBSGSRatio_(logBSGSRatio)
    {
    }

    LinearType HomomorphicDFTMatrixLiteral::getType() const { return type_; }
    uint32_t HomomorphicDFTMatrixLiteral::getLogN() const { return logN_; }
    uint32_t HomomorphicDFTMatrixLiteral::getLogSlots() const { return logSlots_; }
    uint32_t HomomorphicDFTMatrixLiteral::getLevelStart() const { return levelStart_; }
    const std::vector<uint32_t>& HomomorphicDFTMatrixLiteral::getLevels() const { return levels_; }
    bool HomomorphicDFTMatrixLiteral::getRepackImag2Real() const { return repackImag2Real_; }
    double HomomorphicDFTMatrixLiteral::getScaling() const { return scaling_; }
    bool HomomorphicDFTMatrixLiteral::getBitReversed() const { return bitReversed_; }
    uint32_t HomomorphicDFTMatrixLiteral::getLogBSGSRatio() const { return logBSGSRatio_; }


    uint32_t HomomorphicDFTMatrixLiteral::Depth(bool actual)
    {
        if(actual)
            return levels_.size();
        else
        {
            int depth = 0;
            for(auto iter : levels_)
                depth += iter;
            return depth;
        }
    }

    void HomomorphicDFTMatrixLiteral::create(LinearMatrixGroup &mat_group, CKKSEncoder &encoder,uint32_t step){
        auto context_data = encoder.context().crt_context()->first_context_data();
        auto &modulus = context_data->parms().Q();
        auto x = this->GenMatrices();
        mat_group.data().resize(x.size());
        mat_group.set_step(step);
        auto leveld = levelStart_;
        for(int i = 0; i < x.size(); i++){
            auto modulus_group = safe_cast<double>(modulus[leveld].value()) ;
            if(step == 2){
                modulus_group *= safe_cast<double>(modulus[leveld-1].value()) ;
            }
            else if(step > 2 || step < 1){
                throw invalid_argument("DFT step is too large!");
            }

            GenLinearTransformBSGS(mat_group.data()[i],mat_group.rot_index(), encoder, x[i],  leveld, modulus_group,logBSGSRatio_,logSlots_);
            leveld = leveld - step;
        }
    }

    vector<map<int, vector<complex<double> > > > HomomorphicDFTMatrixLiteral::GenMatrices()
    {
        auto logSlots = logSlots_;
        auto slots = 1 << logSlots;
        auto maxDepth = Depth(false);
        LinearType ltType = type_;
        bool bitreversed = bitReversed_;

        auto logdSlots = logSlots;
        if(logdSlots < logN_- 1 && repackImag2Real_)
            logdSlots++;

        vector<complex<double> > roots = GetRootsFloat64(slots << 2);

        vector<int> pow5((slots << 1) + 1);
        pow5[0] = 1;
        for (int i = 1; i < (slots << 1) + 1; i++)
        {
            pow5[i] = pow5[i - 1] * 5;
            //pow5[i] = pow5[i - 1] * 3;
            pow5[i] &= (slots << 2) - 1;
        }

        uint32_t fftLevel, depth, nextfftLevel;

        fftLevel = logSlots;

        vector<vector<complex<double> > > a, b, c;
        if (ltType == Encode)
        {
            tie(a, b, c) = ifftPlainVec(logSlots, 1 << logSlots, roots, pow5);
        }
        else
        {
            tie(a, b, c) = fftPlainVec(logSlots, 1 << logSlots, roots, pow5);
        }


        vector<map<int, vector<complex<double> > > > plainVector(maxDepth);

        vector<int> merge(maxDepth);
        for(auto i = 0; i < maxDepth; ++i)
        {
            depth = int(ceil(float(fftLevel) / float(maxDepth - i)));

            if(ltType == Encode)
                merge[i] = depth;
            else
                merge[merge.size()-i-1] = depth;

            fftLevel -= depth;
        }

        fftLevel = logSlots;
        for (int i = 0; i < maxDepth; i++)
        {
            if (logSlots != logdSlots && ltType == Decode && i == 0 && repackImag2Real_) {

                // Special initial matrix for the repacking before DFT
                plainVector[i] = genRepackMatrix(logSlots, bitreversed);

                // Merges this special initial matrix with the first layer of DFT
                plainVector[i] = multiplyFFTMatrixWithNextFFTLevel(plainVector[i], logSlots, 2*slots, fftLevel, a[logSlots-fftLevel], b[logSlots-fftLevel], c[logSlots-fftLevel], ltType, bitreversed);

                // Continues the merging with the next layers if the total depth requires it.
                nextfftLevel = fftLevel - 1;
                for(int j = 0; j < merge[i]-1; j++)
                {
                    plainVector[i] = multiplyFFTMatrixWithNextFFTLevel(plainVector[i], logSlots, 2*slots, nextfftLevel, a[logSlots-nextfftLevel], b[logSlots-nextfftLevel], c[logSlots-nextfftLevel], ltType, bitreversed);
                    nextfftLevel--;
                }

            } else {
                // First layer of the i-th level of the DFT
                plainVector[i] = genFFTDiagMatrix(logSlots, fftLevel, a[logSlots-fftLevel], b[logSlots-fftLevel], c[logSlots-fftLevel], ltType, bitreversed);

                // Merges the layer with the next levels of the DFT if the total depth requires it.
                nextfftLevel = fftLevel - 1;
                for(int j = 0; j < merge[i]-1; j++)
                {
                    plainVector[i] = multiplyFFTMatrixWithNextFFTLevel(plainVector[i], logSlots, slots, nextfftLevel, a[logSlots-nextfftLevel], b[logSlots-nextfftLevel], c[logSlots-nextfftLevel], ltType, bitreversed);
                    nextfftLevel--;
                }




            }

            fftLevel -= merge[i];
        }
        // Repacking after the IDFT (we multiply the last matrix with the vector [1, 1, ..., 1, 1, 0, 0, ..., 0, 0]).
        if(logSlots != logdSlots && ltType == Encode && repackImag2Real_) {
            for (auto j : plainVector[maxDepth-1])
            {
                for(int x = 0; x < slots; x++)
                    j.second[x+slots] = complex<double>(0, 0);
            }
        }

        // Rescaling of the DFT matrices.
        complex<double> scaling((double)scaling_, 0.0);

        // If no scaling (Default); set to 1
        if(scaling == complex<double>(0, 0))
            scaling = 1.0;

        // If DFT matrix, rescale by 1/N
        if(ltType == Encode) {
            scaling /= double(slots);

            // Real/Imag extraction 1/2 factor
            if(repackImag2Real_)
                scaling /= 2;
        }

        // Spreads the scale across the matrices
        scaling = complex<double>(pow(real(scaling), 1.0 / (float)Depth(false)), 0);

        for(auto &j : plainVector)
            for(auto &x : j)
                for(auto & i : x.second){
                    i *= scaling;
                }



        return plainVector;
    }


    vector<complex<double> > GetRootsFloat64(int NthRoot) {
        vector<complex<double> > roots(NthRoot + 1);

        int quarm = NthRoot >> 2;

        double angle = 2 * M_PI / static_cast<double>(NthRoot);

        for (int i = 0; i < quarm; i++) {
            roots[i] = complex<double>(cos(angle * static_cast<double>(i)), 0);
        }

        for (int i = 0; i < quarm; i++) {
            roots[quarm - i] += complex<double>(0, real(roots[i]));
        }

        for (int i = 1; i < quarm + 1; i++) {
            roots[i + 1 * quarm] = complex<double>(-real(roots[quarm - i]), imag(roots[quarm - i]));
            roots[i + 2 * quarm] = -roots[i];
            roots[i + 3 * quarm] = complex<double>(real(roots[quarm - i]), -imag(roots[quarm - i]));
        }

        roots[NthRoot] = roots[0];

        return roots;
    }

    tuple<vector<vector<complex<double> > >, vector<vector<complex<double> > >, vector<std::vector<complex<double> > > > ifftPlainVec(uint32_t logN, uint32_t dslots, vector<complex<double> > roots, vector<int> pow5) {

        int N, m, index, tt, gap, k, mask, idx1, idx2;

        N = 1 << logN;

        vector<vector<complex<double> > > a(logN, vector<complex<double> >(dslots));
        vector<vector<complex<double> > > b(logN, vector<complex<double> >(dslots));
        vector<vector<complex<double> > > c(logN, vector<complex<double> >(dslots));

        int size;
        if (2*N == dslots) {
            size = 2;
        } else {
            size = 1;
        }

        index = 0;
        for (m = N; m >= 2; m >>= 1) {
            tt = m >> 1;

            for (int i = 0; i < N; i += m) {
                gap = N / m;
                mask = (m << 2) - 1;

                for (int j = 0; j < m>>1; j++) {
                    k = ((m << 2) - (pow5[j] & mask)) * gap;

                    idx1 = i + j;
                    idx2 = i + j + tt;

                    for (int u = 0; u < size; u++) {
                        a[index][idx1+u*N] = 1;
                        a[index][idx2+u*N] = -roots[k];
                        b[index][idx1+u*N] = 1;
                        c[index][idx2+u*N] = roots[k];
                    }
                }
            }
            index++;
        }

        return make_tuple(a, b, c);
    }

    tuple<vector<vector<complex<double> > >, vector<vector<complex<double> > >, vector<std::vector<complex<double> > > > fftPlainVec(uint32_t logN, uint32_t dslots, vector<complex<double> > roots, vector<int> pow5) {

        int N, m, index, tt, gap, k, mask, idx1, idx2;

        N = 1 << logN;

        vector<vector<complex<double> > > a(logN, vector<complex<double> >(dslots));
        vector<vector<complex<double> > > b(logN, vector<complex<double> >(dslots));
        vector<vector<complex<double> > > c(logN, vector<complex<double> >(dslots));

        int size;
        if (2*N == dslots) {
            size = 2;
        } else {
            size = 1;
        }


        index = 0;
        for (m = 2; m <= N; m <<= 1) {
            tt = m >> 1;

            for (int i = 0; i < N; i += m) {
                gap = N / m;
                mask = (m << 2) - 1;

                for (int j = 0; j < m>>1; j++) {
                    k = (pow5[j] & mask) * gap;

                    idx1 = i + j;
                    idx2 = i + j + tt;

                    for (int u = 0; u < size; u++) {
                        a[index][idx1+u*N] = 1;
                        a[index][idx2+u*N] = -roots[k];
                        b[index][idx1+u*N] = roots[k];
                        c[index][idx2+u*N] = 1;
                    }
                }
            }
            index++;
        }

        return make_tuple(a, b, c);
    }

    void sliceBitReverseInPlace(std::vector<std::complex<double> >& slice, int N) {
        int bit, j = 0;

        for (auto i = 1; i < N; i++) {
            bit = N >> 1;

            while (j >= bit) {
                j -= bit;
                bit >>= 1;
            }

            j += bit;

            if (i < j) {
                std::swap(slice[i], slice[j]);
            }
        }
    }

    std::vector<std::complex<double> > add(const std::vector<std::complex<double> >& a, const std::vector<std::complex<double> >& b) {
        std::vector<std::complex<double> > res(a.size());

        for (size_t i = 0; i < a.size(); i++) {
            res[i] = a[i] + b[i];
        }

        return res;
    }

    void addToDiagMatrix(std::map<int, std::vector<std::complex<double> > >& diagMat, int index, const std::vector<std::complex<double> >& vec) {
        if (diagMat.find(index) == diagMat.end()) {
            diagMat[index] = vec;
        } else {
            diagMat[index] = add(diagMat[index], vec);
        }
    }

    std::map<int, std::vector<std::complex<double> > > genFFTDiagMatrix(uint32_t logL, uint32_t fftLevel, std::vector<std::complex<double> > a, std::vector<std::complex<double> > b, std::vector<std::complex<double> > c, LinearType ltType, bool bitreversed) {
        int rot;

        if ((ltType == Encode && !bitreversed) || (ltType == Decode && bitreversed)) {
            rot = 1 << (fftLevel - 1);
        } else {
            rot = 1 << (logL - fftLevel);
        }

        std::map<int, std::vector<std::complex<double> > > vectors;

        if (bitreversed) {
            sliceBitReverseInPlace(a, 1 << logL);
            sliceBitReverseInPlace(b, 1 << logL);
            sliceBitReverseInPlace(c, 1 << logL);

            if (a.size() > (1 << logL)) {
                sliceBitReverseInPlace(a, 1 << logL);
                sliceBitReverseInPlace(b, 1 << logL);
                sliceBitReverseInPlace(c, 1 << logL);
            }
        }

        addToDiagMatrix(vectors, 0, a);
        addToDiagMatrix(vectors, rot, b);
        addToDiagMatrix(vectors, (1 << logL) - rot, c);

        return vectors;
    }

    std::vector<std::complex<double> > mul(const std::vector<std::complex<double> >& a, const std::vector<std::complex<double> >& b) {
        std::vector<std::complex<double> > res(a.size());

        for (size_t i = 0; i < a.size(); i++) {
            res[i] = a[i] * b[i];
        }

        return res;
    }

    std::vector<std::complex<double> > rotate(const std::vector<std::complex<double> >& x, int n) {
        std::vector<std::complex<double> > y(x.size());

        int mask = int(x.size() - 1);

        // Rotates to the left
        for (size_t i = 0; i < x.size(); i++) {
            y[i] = x[(i+n) & mask];
        }

        return y;
    }

    std::map<int, std::vector<std::complex<double> > > multiplyFFTMatrixWithNextFFTLevel(const std::map<int, std::vector<std::complex<double> > >& vec, uint32_t logL, uint32_t N, uint32_t nextLevel, std::vector<std::complex<double> > a, std::vector<std::complex<double> > b, std::vector<std::complex<double> > c, LinearType ltType, bool bitreversed) {
        int rot;

        std::map<int, std::vector<std::complex<double> > > newVec;

        if ((ltType == Encode && !bitreversed) || (ltType == Decode && bitreversed)) {
            rot = (1 << (nextLevel - 1)) & (N - 1);
        } else {
            rot = (1 << (logL - nextLevel)) & (N - 1);
        }

        if (bitreversed) {
            sliceBitReverseInPlace(a, 1 << logL);
            sliceBitReverseInPlace(b, 1 << logL);
            sliceBitReverseInPlace(c, 1 << logL);

            if (a.size() > (1 << logL)) {
                sliceBitReverseInPlace(a, 1 << logL);
                sliceBitReverseInPlace(b, 1 << logL);
                sliceBitReverseInPlace(c, 1 << logL);
            }
        }

        for (auto & it : vec) {
            int i = it.first;
            std::vector<std::complex<double> > val = it.second;
            addToDiagMatrix(newVec, i, mul(val, a));
            addToDiagMatrix(newVec, (i + rot) & (N - 1), mul(rotate(val, rot), b));

            addToDiagMatrix(newVec, (i - rot) & (N - 1), mul(rotate(val, -rot), c));
            auto cc = newVec.find(8192)->second;
        }

        return newVec;
    }

    map<int, vector<complex<double> > > genRepackMatrix(uint32_t logL, bool bitreversed)
    {
        std::map<int, std::vector<std::complex<double> > > newVec;

        return newVec;

    }





//    vector<vector<uint32_t >> HomomorphicDFTMatrixLiteral::GenMatrices() {
//
//        auto mat_size = 1 << this->logN_;
//        auto ringT_index = crt_context_->primes().size();
//        int level = crt_context_->maxLevel();
//        auto coeff_modulus = crt_context_->ntt_contexts()[ringT_index].coeff_modulus();
//        auto roots_of_unity = crt_context_->ntt_contexts()[ringT_index].roots_of_unity();
//        auto roots_of_unity_inv = crt_context_->ntt_contexts()[ringT_index].roots_of_unity_inv();
//        auto mod_inv = crt_context_->ntt_contexts()[ringT_index].intt_conv();
//
//        vector<uint32_t >roots_of_unity_inv_bit_rev;
//        bit_reverse_u32(roots_of_unity_inv,roots_of_unity_inv_bit_rev,mat_size);
//
//
//
//        vector<vector<uint32_t >>mat(mat_size,vector<uint32_t>(mat_size,0));
//        if(type_ == Decode){
//            //matrix_operations::transpose_matrix(mat,mat_T1);
//            for(int i = 0; i < mat_size; i++){
//                Util::cal_x0_to_xn_1( roots_of_unity[i], coeff_modulus,mat[i],mat_size);
//                //multiply_poly_coeffmod(mat[i],roots_of_unity,roots_of_unity.size(),coeff_modulus,mat[i]);
//            }
//
//        }
//        else if(type_ == Encode){
//            for(int i = 0; i < mat_size; i++){
//                Util::cal_x0_to_xn_1( roots_of_unity_inv[i], coeff_modulus,mat[i],mat_size);
//                //multiply_poly_coeffmod(mat[i],roots_of_unity_inv,roots_of_unity_inv.size(),coeff_modulus,mat[i]);
//                multiply_poly_scalar_coeffmod(mat[i],roots_of_unity_inv.size(),mod_inv,coeff_modulus,mat[i]);
//
//            }
//        }
//
//        return mat;
//
//    }

//    MatrixPlain HomomorphicNTTMatrixLiteral::create(){
//        auto x = this->GenMatrices();
//
//
//        vector<vector<uint32_t >>mat_T1(x.size(),vector<uint32_t>(x.size(),0));
//        MatrixPlain matrixPlain;
//        int leveld = this->levelStart_;
//        matrix_operations::rotate_slots_matrix(x,mat_T1);
//        GenMatrixformBSGS(matrixPlain,matrixPlain.rot_index, batchEncoder_, mat_T1,
//                          this->levelStart_ ,1, 1, this->logSlots_);
//        return matrixPlain;
//    }








}







