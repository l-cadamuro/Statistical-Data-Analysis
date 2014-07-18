#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H
#include <vector>
#include "TF2.h"
#include "TFitter.h"
#include "TMinuit.h"

// questa likelihood è stata costruita per la stima di parametri richiesta dall'esercizio
// la funzione deve avere tre parametri e due variabili

class Likelihood
{
    public:
        Likelihood();
        virtual ~Likelihood();
        void LinkData (std::vector<double>** data); // richiede un puntatore a un array di vettori
        void LinkFunction (TF2* fitfunction);
        double ComputeLogLikelihood (double* par);
        extern void MinuitLikelihood (int& nDim, double* gout, double& result, double par[], int flg);
        void Minimize();

    protected:
    private:
        std::vector<double>** m_data; // puntatore ai dati
        TF2* m_fitfunction; // funzione di fit
        double* m_parresult;
};

#endif // LIKELIHOOD_H
