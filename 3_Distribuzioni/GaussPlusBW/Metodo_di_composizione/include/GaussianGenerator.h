#ifndef GAUSSIANGENERATOR_H
#define GAUSSIANGENERATOR_H

#include "TRandom3.h"
#include <iostream>

// generatore di numeri pseudocasuali gaussiano
// utilizza come generatore TRandom3 di root
//

class GaussianGenerator
{
    public:
        GaussianGenerator();
        GaussianGenerator(double mean, double sigma);
        virtual ~GaussianGenerator();
        double GetRandomUniform(); // restituisce num in [0,1]
        double GetRandom();
        void SetNtoSum(int N);
        void Refresh(); // ricalcola le costanti
        double GetMean();
        double GetSigma();


    protected:

    private:
        TRandom3* m_RndNum;
        int m_ntosum; // num di valori da sommare
        double m_mean;
        double m_sigma;
                // queste costanti sono calcolate una sola volta
        // per risparmiare tempo nella generazionw
        double m_offset;
        double m_coeff;

};

#endif // GAUSSIANGENERATOR_H
