#ifndef MESONDECAYEVENTGENERATOR_H
#define MESONDECAYEVENTGENERATOR_H

#include "TRandom3.h"

// generatore di eventi secondo la distribuzione di mesone vettore
// la distribuzione è definita all'interno della classe
//
class MesonDecayEventGenerator
{
    public:
        MesonDecayEventGenerator();
        virtual ~MesonDecayEventGenerator();
        double ComputeFunction(double x, double y);
        // produce l'evento con hit/miss e lo mette in theta e phi passati per referenza
        void GetRandomEvent (double& x, double& y);
        // test del limite superiore - per debug programma
        int CheckUpperBound(int ntests);
    protected:
    private:
        TRandom3* m_Random;
};

#endif // MESONDECAYEVENTGENERATOR_H
