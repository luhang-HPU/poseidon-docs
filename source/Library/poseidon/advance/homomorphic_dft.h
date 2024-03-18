//
// Created by cloudam on 12/18/23.
//
#ifndef POSEIDON_HOMOMORPHIC_DFT_H
#define POSEIDON_HOMOMORPHIC_DFT_H
#include "poseidon/advance/homomorphic_linear_transform.h"
#include <complex>
#include <vector>
#include <map>
using namespace std;
namespace poseidon{
    typedef int LinearType;
    const LinearType Encode = 0;
    const LinearType Decode = 1;

    vector<complex<double> > GetRootsFloat64(int NthRoot);
    tuple<vector<vector<complex<double> > >, vector<vector<complex<double> > >, vector<std::vector<complex<double> > > > ifftPlainVec(uint32_t logN, uint32_t dslots, vector<complex<double> > roots, vector<int> pow5);
    tuple<vector<vector<complex<double> > >, vector<vector<complex<double> > >, vector<std::vector<complex<double> > > > fftPlainVec(uint32_t logN, uint32_t dslots, vector<complex<double> > roots, vector<int> pow5);
    void sliceBitReverseInPlace(std::vector<std::complex<double> >& slice, int N);
    void addToDiagMatrix(std::map<int, std::vector<std::complex<double> > >& diagMat, int index, const std::vector<std::complex<double> >& vec);
    std::vector<std::complex<double> > add(const std::vector<std::complex<double> >& a, const std::vector<std::complex<double> >& b);
    std::vector<std::complex<double> > mul(const std::vector<std::complex<double> >& a, const std::vector<std::complex<double> >& b);
    std::vector<std::complex<double> > rotate(const std::vector<std::complex<double> >& x, int n);
    std::map<int, std::vector<std::complex<double> > > genFFTDiagMatrix(uint32_t logL, uint32_t fftLevel, std::vector<std::complex<double> > a, std::vector<std::complex<double> > b, std::vector<std::complex<double> > c, LinearType ltType, bool bitreversed);
    std::map<int, std::vector<std::complex<double> > > multiplyFFTMatrixWithNextFFTLevel(const std::map<int, std::vector<std::complex<double> > >& vec, uint32_t logL, uint32_t N, uint32_t nextLevel, std::vector<std::complex<double> > a, std::vector<std::complex<double> > b, std::vector<std::complex<double> > c, LinearType ltType, bool bitreversed);
    map<int, vector<complex<double> > > genRepackMatrix(uint32_t logL, bool bitreversed);
    class HomomorphicDFTMatrixLiteral {
    private:
        // Member variables
        LinearType type_;
        uint32_t logN_;
        uint32_t logSlots_;
        uint32_t levelStart_;
        std::vector<uint32_t> levels_;
        bool repackImag2Real_;
        double scaling_;
        bool bitReversed_;
        uint32_t logBSGSRatio_;
        uint32_t Depth(bool actual);

    public:
        POSEIDON_NODISCARD LinearType getType() const;
        POSEIDON_NODISCARD uint32_t getLogN() const;
        POSEIDON_NODISCARD uint32_t getLogSlots() const;
        POSEIDON_NODISCARD uint32_t getLevelStart() const ;
        POSEIDON_NODISCARD const vector<uint32_t> &getLevels() const ;
        POSEIDON_NODISCARD bool getRepackImag2Real() const ;
        POSEIDON_NODISCARD double getScaling() const ;
        POSEIDON_NODISCARD bool getBitReversed() const ;
        POSEIDON_NODISCARD uint32_t getLogBSGSRatio() const ;
        vector<map<int, vector<complex<double> > > > GenMatrices();
        void create(LinearMatrixGroup &mat_group,  CKKSEncoder &encoder,uint32_t step);

        HomomorphicDFTMatrixLiteral(LinearType type, uint32_t logN, uint32_t logSlots,
                                    uint32_t levelStart, vector<uint32_t> levels,
                                    bool repackImag2Real = false,
                                    double scaling = 1.0,
                                    bool bitReversed = false,
                                    uint32_t logBSGSRatio = 0);
    };



//    class HomomorphicNTTMatrixLiteral {
//    private:
//        // Member variables
//        LinearType type_;
//        uint32_t logN_;
//        uint32_t logSlots_;
//        uint32_t levelStart_;
//        uint32_t logBSGSRatio_;
//        shared_ptr<CrtContext> crt_context_ = nullptr;
//        BatchEncoder batchEncoder_;
//
//
//
//    public:
//        [[nodiscard]] LinearType getType() const;
//        [[nodiscard]] uint32_t getLogN() const;
//        uint32_t getLogSlots() const;
//        uint32_t getLevelStart() const ;
//        uint32_t getLogBSGSRatio() const ;
//        vector<vector<uint32_t>> GenMatrices();
//        MatrixPlain create();
//
//        HomomorphicNTTMatrixLiteral(LinearType type, uint32_t logN, uint32_t logSlots,
//                                    uint32_t levelStart,const PoseidonContext &context,BatchEncoder &batchEncoder)
//                : type_(type), logN_(logN), logSlots_(logSlots), levelStart_(levelStart),crt_context_(context.crt_context()),batchEncoder_(batchEncoder)
//        {}
//
//    };

}



#endif //POSEIDON_HOMOMORPHIC_DFT_H
