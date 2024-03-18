//
// Created by lxs on 8/25/23.
//

#include "precision.h"

namespace poseidon {
    namespace util{


    PrecisionStats deltaToPrecision(const PrecisionStats &c) {
        return PrecisionStats{log2(1 / c.Real), log2(1 / c.Imag),log2(1 / c.L2)};
    }


    void GetPrecisionStats(vector<complex<double>> value_test,vector<complex<double>> value_want){
        double deltaReal, deltaImag, deltaL2;
        auto slots = value_want.size();
        vector<double> precReal(slots);
        vector<double> precImag(slots);
        vector<double> precL2(slots);
        PrecisionStats diff[slots];
        PrecisionStats MeanDelta{0,0,0};
        PrecisionStats MaxDelta{0,0,0};
        PrecisionStats MinDelta{1,1,1};

        for(int i = 0; i < slots; i++){
            deltaReal = abs(real(value_test[i]) - real(value_want[i]));
            deltaImag = abs(imag(value_test[i]) - imag(value_want[i]));
            deltaL2 = sqrt(deltaReal*deltaReal + deltaImag*deltaImag);
            precReal[i] = log2(1 / deltaReal);
            precImag[i] = log2(1 / deltaImag);
            precL2[i] = log2(1 / deltaL2);
            diff[i].Real = deltaReal;
            diff[i].Imag = deltaImag;
            diff[i].L2 = deltaL2;
            MeanDelta.Real += deltaReal;
            MeanDelta.Imag += deltaImag;
            MeanDelta.L2 += deltaL2;

            if (deltaReal > MaxDelta.Real) {
                MaxDelta.Real = deltaReal;
            }
            if (deltaImag > MaxDelta.Imag) {
                MaxDelta.Imag = deltaImag;
            }
            if (deltaImag > MaxDelta.L2) {
                MaxDelta.L2 = deltaL2;
            }

            if (deltaReal < MinDelta.Real) {
                MinDelta.Real = deltaReal;
            }

            if (deltaImag < MinDelta.Imag) {
                MinDelta.Imag = deltaImag;
            }

            if (deltaL2 < MinDelta.L2) {
                MinDelta.L2 = deltaL2;
            }
        }

        auto MinPrecision = deltaToPrecision(MaxDelta);
        auto MaxPrecision = deltaToPrecision(MinDelta);
        MeanDelta.Real /= (double)slots;
        MeanDelta.Imag /= (double)slots;
        MeanDelta.L2 /= (double)slots;
        auto MeanPrecision = deltaToPrecision(MeanDelta);
        printf(PRECISION_TITLE,
               MinPrecision.Real,MinPrecision.Imag,MinPrecision.L2,
               MaxPrecision.Real,MaxPrecision.Imag,MaxPrecision.L2,
               MeanPrecision.Real,MeanPrecision.Imag,MeanPrecision.L2
               );
    }





//"│    Log2 │ REAL  │ IMAG  │ L2    │\n"  \
//"├─────────┼───────┼───────┼───────┤\n"  \
//"│MIN Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
//"│MAX Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
//"│AVG Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
//"│MED Prec │ %5.2f │ %5.2f │ %5.2f │\n"  \
//"└─────────┴───────┴───────┴───────┘\n" ;
    }
} // poseidon