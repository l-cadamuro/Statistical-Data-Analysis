#ifndef GAUSSPLUSBW_H
#define GAUSSPLUSBW_H

#include "TRandom3.h"
#include "GaussianGenerator.h"
#include "BreitWignerGenerator.h"

class GaussPlusBW
{
    public:
        GaussPlusBW();
        GaussPlusBW(double a, double mgauss, double sigmagauss, double mbw, double fwhmbw);
        virtual ~GaussPlusBW();
        double GetRandomUniform(); // restituisce num in [0,1]
        double GetRandom();
    protected:
    private:
    TRandom3* m_RndNum;
    GaussianGenerator* m_gauss;
    BreitWignerGenerator* m_bw;
    double m_a; // frazione di gaussiana
    double m_mgaus;
    double m_sigmagauss;
    double m_mbw;
    double m_fwhmbw;

};

#endif // GAUSSPLUSBW_H
