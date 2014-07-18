#include <iostream>
#include <fstream>
#include "TCanvas.h"
#include "TH1D.h"
#include "TApplication.h"
#include "TF1.h"
#include "GaussianGenerator.h"
#include "BreitWignerGenerator.h"
#include "GaussPlusBW.h"
#include"TStyle.h"
#include "TMath.h"

#define PI 3.14159265358979323846

using namespace std;

int main(int argc, char** argv)
{
    TApplication* theApp = new TApplication("App", &argc, argv);
    TCanvas* c1 = new TCanvas;
    TH1D* isto = new TH1D ("isto", "isto", 1000, -35, 35);

    // verifica del generatore gaussiano
    /*
    GaussianGenerator* g = new GaussianGenerator(7, 2);

    //g->SetNtoSum(50);

    for (int i = 0; i < 1000000; i++)
    {
        isto -> Fill (g->GetRandom());
    }

    TF1* f = new TF1 ("f", "[2]*TMath::Gaus(x,[0],[1])", -20,20);
    f -> SetParameters (7,2,10000);

    isto->Fit("f");
    gStyle -> SetOptFit(1111);

    isto->Draw();
    theApp -> Run();
    */

    // verifica del generatore BW
    /*
    BreitWignerGenerator* bw = new BreitWignerGenerator(3,2.5);


    for (int i = 0; i < 1000000; i++)
    {
        isto -> Fill (bw->GetRandom());
    }

    TF1* f = new TF1 ("f", "[2]*TMath::BreitWigner(x,[0],[1])", -20,20);

    f -> SetParameters(3, 2.5, 40000);
    f -> SetNpx (10000);
    */

    // genera file contenente i dati
    std::ofstream file ("composizione.txt");

    GaussPlusBW* pt = new GaussPlusBW(0.1, 0, 20, 0, 2);

    double number;

    for (int i = 0; i < 1000000; i++)
    {
        number = pt -> GetRandom();
        file << number << std::endl;

    }

    // disegna la distribuzione ed esegui fit
    /*
    TF1* f = new TF1 ("f", "[5]*( [0]/sqrt(2*TMath::Pi()*[2]*[2])*exp(-0.5*(x-[1])*(x-[1])/([2]*[2]) ) + (1.-[0])/TMath::Pi() * [4]/( (x-[3])*(x-[3]) + [4]*[4] ) )", -20, 20);
    //TF1* f = new TF1 ("f", "[5]*( [0]*TMath::Gaus(x, [1], [2]) + (1.-[0])*TMath::BreitWigner(x,[3],[4]) )", -20,20);

    f -> SetParameters(0.1, 0, 20, 0, 1, 40000);
    f -> SetNpx (10000);

    f->SetParName(0, "Fraz gauss");
    f->SetParName(1, "Media gauss");
    f->SetParName(2, "Sigma gauss");
    f->SetParName(3, "Media bw");
    f->SetParName(4, "FWHM/2 bw");
    f->SetParName(5, "Coeff overall");

    f->SetParLimits(0, 0, 0.2);
    f->SetParLimits(1, -1, 1);
    f->SetParLimits(2, 18, 21);
    f->SetParLimits(3, -1, 1);
    f->SetParLimits(4, 0.5, 1.5);

    isto->Fit("f");
    gStyle -> SetOptFit(1111);

    isto->Draw();
    theApp -> Run();
    */

    file.close();
    return 0;
}
