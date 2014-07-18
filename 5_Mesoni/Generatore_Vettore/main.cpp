#include <iostream>
#include <fstream>
#include "MesonDecayEventGenerator.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TF2.h"
#include "TApplication.h"

#define PI 3.14159265359

using namespace std;

int main(int argc, char**argv)
{
    // create generator and test upper bound
    MesonDecayEventGenerator* g = new MesonDecayEventGenerator;
    cout << "Exceeding upper bound: " << g->CheckUpperBound(10000000) << endl;

    double a, b;

    // plot an istogram with the distribution
    TApplication* theApp = new TApplication("App", &argc, argv);
    TCanvas* c1 = new TCanvas;
    TH2D* isto = new TH2D ("isto", "isto", 50, -1, 1, 50, 0, 2*PI);
    isto->GetXaxis()->SetTitle("cos (#theta)");
    isto->GetYaxis()->SetTitle("#varphi");


    // produce a big sample for hypothesis test
    ofstream output ("MesonDecayDataBig.txt", std::ios::out | std::ios::trunc);

    for (int i = 0; i < 5000000; i++)
    {
        if (i%500000 == 0)
        {cout << i << endl;}

        // get random event and fill histogram and file
        g->GetRandomEvent(a,b);
        isto->Fill(a,b);
        output << a << "    " << b << endl;
    }

    // close output file
    output.close();


    // now produce a small output of 5000 events (the "sample")
    ofstream output_sample ("MesonDecayData.txt", std::ios::out | std::ios::trunc);

    for (int i = 0; i < 5000; i++)
    {
        if (i%1000 == 0)
        {cout << i << endl;}

        // get random event and fill histogram and file
        g->GetRandomEvent(a,b);
        output_sample << a << "    " << b << endl;
    }

    // close output file
    output_sample.close();


    // plot the distribution function over histogram and print (need to fit for normalization)
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle -> SetStatY (0.02);
    gStyle -> SetStatX (5.1);
    gStyle -> SetStatW (1.4);
    gStyle -> SetStatH (6.4);
    TF2* es5c = new TF2 ("es5c", "[0]*0.75*TMath::Pi()*( 0.5*(1-0.65) + 0.5*(3.*0.65- 1)*x*x - 0.06*(1.-x*x)*cos(2.*y) + sqrt(2)*0.18*sin(2.*acos(x))*cos(y) )", -1, 1, 0,2*PI);
    es5c -> SetParameter (0, 2500);

    es5c->SetNpx(1000);
    es5c->SetNpy(1000);
    es5c->SetLineColor(kBlack);
    isto->Fit("es5c");
    isto -> SetTitle("");
    isto->Draw("colz");
    c1->Print("5_eventdistr.pdf", "pdf");
    theApp->Run();


}
