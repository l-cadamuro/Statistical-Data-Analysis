/* Questo programma acquisisce un file di testo contenente i
** dati del decadimento ed esegue il fit massimizzando la
** likelihood.
**
** La funzione di fit deve essere trasformata in coordinate
** "cartesiane"; infatti se espressa in (theta, phi)
** risulta normalizzata se integrata in
** dOmega = sin(theta)*dtheta*dphi (ma non in elementi cartesiani
** dtheta*dphi), e Minuit non riesce ad
** eseguire una minimizzazione corretta.
** Se si pone x = cos(theta), y = phi, la fz è normalizzata
** se integrata in dx*dy e la minimizzazione è corretta.
**
** Per la compilazione: occore includere la direttiva -lMinuit:
** c++ -lm -o Likelihood Likelihood.cpp `root-config --glibs --cflags` -lMinuit
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "TFitter.h" // classe che gestisce Minuit
#include "TF2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TNtuple.h"
#include "TLine.h"

#define PI 3.14159265359

// sono dichiarate variabili globali per essere passate all'implementazione per minuit
// x = cos(theta), y = phi
std::vector<double> x;
std::vector<double> y;

double fitfunc (double x, double y, double alpha, double beta, double gamma)
{
	double f;
	double costh = x;
	double sinth = sqrt(1.-x*x);
	double sin2th = 2.*x*sqrt(1.-x*x);
	double cosphi = cos(y);
	double cos2phi = cos(2.*y);

	f = (0.75/PI)*( 0.5*(1.-alpha) + 0.5*(3.*alpha - 1.)*costh*costh - beta*sinth*sinth*cos2phi - sqrt(2.)*gamma*sin2th*cosphi);
   
	return f;
}

// per tf2 root
Double_t fitfunc1 (Double_t * x, Double_t * par)
{
	return (fitfunc(x[0], x[1], par[0], par[1], par[2]));
}


// questa funzione calcola la likelihood sulla base dei parametri
double ComputeLogLikelihood (double alpha, double beta, double gamma)
{
	double logl = 0;
	double f;

	char help;

	for (int i = 0; i < x.size(); i++)
	{
		f = fitfunc(x.at(i), y.at(i), alpha, beta, gamma);
	
		logl = logl + log(f);

		// debug per fissare il range dei parametri	
		if(f<0)
		{
			std::cout << "x, y " << x.at(i) << " " << y.at(i) << std::endl;
			std::cout << "logl, f " << logl << " " << f << std::endl;
			std::cout << alpha << " " << beta << " " << gamma << std::endl;		
			std::cout << "i " << i << std::endl;		
			std::cin >> help;
		}    
	}
	
    return logl;
}


// interfaccia della funzione precedente per Minuit
void minuitLogL( int& nDim, double* gout, double& result, double* par, int flg)
{
	result = -1.*ComputeLogLikelihood (par[0], par[1], par[2]);
}

int main (int argc, char** argv)
{

	// acquisizione dati
	double buf1, buf2;	
	std::ifstream dati ("MesonDecayData.txt", std::ios::in);
	while(1)
	{
		dati >> buf1;
		if (dati.eof())
		{break;}
		dati >> buf2;
		x.push_back(buf1);
		y.push_back(buf2);
	}
	dati.close();

	// gestisce la chiamata a Minuit	
	TFitter* minimizer = new TFitter(3);
	
	// non stampa a schermo alcuna informazione
	//double p1 = -1;
	//minimizer -> ExecuteCommand("SET PRINTOUT", &p1, 1);

	// imposta la funzione da minimizzare per Minuit
	minimizer->SetFCN(minuitLogL);	

	//SetParameter(ParNum, "PARNAME", InitalValue, InitialError, LowerLimit, UpperLimit)
	//Se LowerLimit = UpperLimit = 0, non c'è nessun limite
	minimizer -> SetParameter(0, "alpha", 0.65,0.001,0.50,0.80);
	minimizer -> SetParameter(1, "beta", 0.063,0.001,0.04,0.075);
	minimizer -> SetParameter(2, "gamma", -0.175,0.001,-0.195,-0.165);
			
	// prima ricerca del minimo con il metodo del simplesso
	// è robusta e da una prima indicazione del valore
	minimizer -> ExecuteCommand ("SIMPLEX", 0,0);
	// minimizzo usando la routine migrad
	minimizer -> ExecuteCommand ("MIGRAD", 0,0);

	double result [3], error [3];
	const char* parnames [3] = {"Alpha", "Beta", "Gamma"};

	std::cout << "\n\nRISULTATO:\n\n";

	for (int i = 0; i < 3; i++)
	{	
		result[i] = minimizer->GetParameter(i);
		error[i] = minimizer->GetParError(i);
	}

	for (int i = 0; i < 3; i++)
	{
		std::cout << parnames[i] << " = " << result[i] << " +/- " << error[i] << std::endl;
	}

	// stampa il profilo della likelihood vs alpha
	TCanvas* c1 = new TCanvas;	
	TGraph* graph = new TGraph;
	double points = 100;
	double l, beta, alpha, gamma;
	double ymin = -1*ComputeLogLikelihood (result[0], result[1], result[2]);


	graph->SetMarkerStyle(20);
	graph->SetMarkerSize(0.5);
	graph->SetMarkerColor(kBlue);
	
	// linea usata per l'errore
	TLine* line = new TLine (0,0,0,0);
	line->SetY1(ymin + 0.5);
	line->SetY2(ymin + 0.5);
	line -> SetLineColor(kRed);

	std::cout << "\n\nDISEGNO PROFILI DELLA LIKELIHOOD\n\n";

	// beta
	graph->SetTitle("Likelihood profile vs #beta; #beta; - log(L)");

	double low = 0.03;
	double up = 0.067;
	for (int i = 0; i < points; i++)
	{
		beta = low + (up-low)*i/points;
		l = -1*ComputeLogLikelihood (result[0], beta, result[2]);
		graph -> SetPoint(i, beta, l);
	}

	line->SetX1(low);
	line->SetX2(up);

	graph -> Draw("AP");
	line->Draw("same");
	c1 -> Print ("lik_profile_vs_beta.pdf", "pdf");
	
	// gamma
	graph->SetTitle("Likelihood profile vs #gamma; #gamma; - log(L)");
	low = -0.20;	
	up = -0.165;
	for (int i = 0; i < points; i++)
	{
		gamma = low + (up-low)*i/points;
		l = -1*ComputeLogLikelihood (result[0], result[1], gamma);
		graph -> SetPoint(i, gamma, l);
	}
	
	line->SetX1(low);
	line->SetX2(up);

	graph -> Draw("ap");
	line->Draw("same");
	c1 -> Print ("lik_profile_vs_gamma.pdf", "pdf");

	// alpha
	graph->SetTitle("Likelihood profile vs #alpha; #alpha; - log(L)");
	low = 0.62;
	up = 0.70;
	for (int i = 0; i < points; i++)
	{
		alpha = low + (up-low)*i/points;
		l = -1*ComputeLogLikelihood (alpha, result[1], result[2]);
		graph -> SetPoint(i, alpha, l);
	}

	line->SetX1(low);
	line->SetX2(up);

	graph -> Draw("AP");
	line->Draw("same");
	c1 -> Print ("lik_profile_vs_alpha.pdf", "pdf");
	
}
