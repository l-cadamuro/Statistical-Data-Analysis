#ifndef BREITWIGNERGENERATOR_H
#define BREITWIGNERGENERATOR_H

#include "TRandom3.h"
#include <iostream>


class BreitWignerGenerator
{
    public:
        BreitWignerGenerator();
        BreitWignerGenerator(double mean, double fwhm);
        virtual ~BreitWignerGenerator();
        double GetRandomUniform(); // restituisce num in [0,1]
        double GetRandom();
        double GetMean();
        double GetFWHM();
    protected:
    private:
        TRandom3* m_RndNum;
        double m_mean;
        double m_gamma;
};

#endif // BREITWIGNERGENERATOR_H
