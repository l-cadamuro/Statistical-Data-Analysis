/* Questo programma acquisisce un file di testo contenente i
** dati del decadimento, li immagazzina in un array 2d (istogramma)
** ed esegue un fit con il metodo del chi^2.
**
** Per la compilazione: occore includere la direttiva -lMinuit
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include "TFitter.h" // classe che gestisce Minuit
#include "TF2.h"

#define nThetaBin 8
#define nPhiBin 13

#define PI 3.14159265359

// sono dichiarate variabili globali per essere passate all'implementazione per minuit
double IstoData [nThetaBin][nPhiBin];
double ThBinWidth = 2./(1.*nThetaBin);
double PhiBinWidth = 2.*PI/(1.*nPhiBin);
// range: cos(theta) [-1 : 1]
// range: phi [0 : 2PI]


double fitfunc (double x, double y, double alpha, double beta, double gamma, double norm)
{
	double f;
	double costh = x;
	double sinth = sqrt(1.-x*x);
	double sin2th = 2.*x*sqrt(1.-x*x);
	double cosphi = cos(y);
	double cos2phi = cos(2.*y);

	f = norm*(0.75/PI)*( 0.5*(1.-alpha) + 0.5*(3.*alpha - 1.)*costh*costh - beta*sinth*sinth*cos2phi - sqrt(2.)*gamma*sin2th*cosphi);
   
	return f;
}

double ChiSquare (double alpha, double beta, double gamma, double norm)
{
	double chisq = 0.;
	double num, y;
	double thBinCenter, phiBinCenter;
	for (int th = 0; th < nThetaBin; th++)
        for (int phi = 0; phi < nPhiBin; phi++)
        {
			thBinCenter = ThBinWidth/2. -1. + th*ThBinWidth;
			phiBinCenter = PhiBinWidth/2. + phi*PhiBinWidth;
			y = IstoData [th][phi];
			num = y - fitfunc(thBinCenter, phiBinCenter, alpha, beta, gamma, norm);
			// ignoro i bin vuoti			
			if (y > 0)
			{chisq = chisq + num*num/y;}
	}
	return chisq;
}

// interfaccia della funzione precedente per Minuit
void minuitChiSquare( int& nDim, double* gout, double& result, double* par, int flg)
{
	result = ChiSquare (par[0], par[1], par[2], par[3]);
}


int main (int argc, char**argv)
{

	// acquisizione dati e riempimento istogramma;
	// i bin sono intervalli aperti a dx [low, up)
	double buf1, buf2;
	int ThBin, PhiBin;	
	std::ifstream dati ("MesonDecayData.txt", std::ios::in);

	while(1)
	{
		dati >> buf1;
		if (dati.eof())
		{break;}
		dati >> buf2;
		// find bin
		ThBin = floor((buf1+1.)/ThBinWidth);
		PhiBin = floor(buf2/PhiBinWidth);
		if(ThBin > nThetaBin || PhiBin > nPhiBin)
		{std::cout << "Warning: overflow in filling histogram\n";}
		else
		{IstoData[ThBin][PhiBin] += 1.;}	

	}
	dati.close();

	// gestisce la chiamata a Minuit	
	TFitter* minimizer = new TFitter(4);
	
	// non stampa a schermo alcuna informazione
	double p1 = -1;
	minimizer -> ExecuteCommand("SET PRINTOUT", &p1, 1);

	// imposta la funzione per Minuit
	minimizer->SetFCN(minuitChiSquare);	

	//SetParameter(ParNum, "PARNAME", InitalValue, InitialError, LowerLimit, UpperLimit)
	//Se LowerLimit = UpperLimit = 0, non c'è nessun limite
	minimizer -> SetParameter(0, "alpha", 0.7,1,0,0);
	minimizer -> SetParameter(1, "beta", 0.05,1,0,0);
	minimizer -> SetParameter(2, "gamma", -0.2,1,0,0);
	minimizer -> SetParameter(3, "norm", 100,1,0,0);

	// prima ricerca del minimo con il metodo del simplesso
	// è robusta e da una prima indicazione
	minimizer -> ExecuteCommand ("SIMPLEX", 0,0);
	// minimizzo usando la routine migrad
	minimizer -> ExecuteCommand ("MIGRAD", 0,0);

	double result [4];
	double error[4];
	const char* parnames [4] = {"Alpha", "Beta", "Gamma", "Norm"};	

	for (int i = 0; i < 4; i++)
	{	
		result[i] = minimizer->GetParameter(i);
		error[i] = minimizer->GetParError(i);
	}

	for (int i = 0; i < 4; i++)
	{
		std::cout << parnames[i] << " = " << result[i] << " +/- " << error[i] << std::endl;
	}

	return 1;	

}
