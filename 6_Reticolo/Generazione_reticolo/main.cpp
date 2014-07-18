#include <iostream>
#include "Complex.h"
#include "DiffractionGrating.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TRandom3.h"
#include <cmath>
#include <fstream>

#define PI 3.14159265359

using namespace std;

int main(int argc, char** argv)
{
    // TApplication gestisce una sessione di ROOT interattiva per controllare
    // all'istante il risultato

    int nslit = 5;
    double L = 4.5/20.;
    double S = 5;

    double width = 6;

    cout << "Parametri utilizzati:\nL: " << L << "\nS: " << S << "\nnum fenditure: " << nslit << endl;

    DiffractionGrating ret (nslit, S, L);

    TApplication* theApp = new TApplication("App", &argc, argv);
    TCanvas* c1 = new TCanvas;

    TH1D* isto = new TH1D ("isto", "isto", 1000, -1*width/2., width/2.);

    double intensity;

    // produce un istogramma calcolando l'intensità in posizioni equispaziate
    for (int i = 0; i < 1000; i++)
    {
        // inserisco offset nelle x per centrare a 0 la distribuzione
        double x = -1.*width/2.0 + (width*i/1000.) + S*(nslit-1)/2.0;
        intensity = ret.ComputeIntensity(x);
        isto->SetBinContent(i, intensity);
    }

    isto->Draw();
    theApp->Run();

    // campione di dati per riempire la matrice di correlazione
    ofstream dati ("dati_high_statistics.txt", std::ios::out);

    //double fmax = ret.ComputeIntensity(S*(nslit-1)/2.0);
    double fmax = 8.6;
    int ngood = 0;
    double x, y;

    TRandom3* rnd = new TRandom3(0);

    while (ngood < 500000)
    {
        if (ngood%500000 == 0)
            std::cout << ngood << endl;
        x = width*(rnd->Rndm()) -0.5*width + S*(nslit-1)/2.0;
        y = rnd->Rndm() * fmax;
        if (y <= ret.ComputeIntensity(x))
        {
            // sottraggo offset all'output per centrare in 0
            dati << x - S*(nslit-1)/2.0 << endl;
            ngood++;
        }
    }

    // salva campione di dati su cui effettuare l'unfolding
    ngood = 0;

    ofstream dati_sample ("dati_sample.txt", std::ios::out);

    while (ngood < 50000)
    {
        if (ngood%500000 == 0)
            std::cout << ngood << endl;
        x = width*(rnd->Rndm()) -0.5*width + S*(nslit-1)/2.0;
        y = rnd->Rndm() * fmax;
        if (y <= ret.ComputeIntensity(x))
        {
            // sottraggo offset all'output per centrare in 0
            dati_sample << x - S*(nslit-1)/2.0 << endl;
            ngood++;
        }
    }

    return 0;
}
