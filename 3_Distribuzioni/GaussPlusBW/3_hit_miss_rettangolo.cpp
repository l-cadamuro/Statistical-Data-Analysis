/*
** Programma "base" che genera i numeri secondo la
** distribuzione utilizzando una tecnica di hit&miss
** attraverso generazione su un quadrato.
**
** L'efficienza è molto bassa, soprattutto per range larghi.
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include "TRandom3.h"

#define PI 3.14159265358979323846

#define npoints 10000000

double BreitWigner (double x, double mean, double fwhm)
{
	double gamma = fwhm/2.0;
	double bw = gamma / (PI * ( gamma*gamma + (x-mean)*(x-mean) ) );
	return bw;
}

double Gaussian (double x, double mean, double sigma)
{
	double gaus = (1.0/sqrt(2.0*PI*sigma*sigma) )* exp (-0.5 * (x-mean)*(x-mean)/(sigma*sigma) );
	return gaus;
}

// somma di breit-wigner e gaussiana con pesi a, b
double Function (double x, double a, double b, double mgaus, double sigmagaus,
				 double mbw, double fwhmbw)

{
	double f = a * Gaussian(x, mgaus, sigmagaus) + b * BreitWigner(x, mbw, fwhmbw);
	return f;
}


int main (int argc, char** argv)
{
	double xlow, xhigh;
	
	// lettura range della x da riga di comando
	if (argc < 2)
	{
		std::cout << "No input from command line\nSetting [xlow; xhigh] to [-10:10]\n";
		xlow = -10.;
		xhigh = 10.;
	}	
	else
	{
		xlow = atof (argv[1]);
		xhigh = atof (argv[2]);

		std::cout << "Range of numbers: " << xlow << " : " << xhigh << std::endl;
	}

	// file di testo con i risultati
	std::ofstream output ("hit_miss_rect.txt", std::ios::out);

	// contatore dei punti validi
	int goodpoints = 0;			

	double x, y, rnd1, rnd2, yhigh, delta;

	// massimo valore della funzione è in x = 0
	yhigh = Function (0, 0.1, 0.9, 0, 20, 0, 2);
	delta = xhigh - xlow;


	TRandom3* RandomNumber = new TRandom3(0);

	for (int i = 0; i < npoints; i++)
	{
		if (i%100000 == 0)
		{std::cout << i << std::endl;}

		//genero casualmente x, y nei rispettivi range
		rnd1 = RandomNumber -> Rndm();
		rnd2 = RandomNumber -> Rndm();
		x = rnd1*delta + xlow;
		y = rnd2*yhigh;

		if (y <= Function(x, 0.1, 0.9, 0, 20, 0, 2))
		{
			output << x << std::endl;
			goodpoints++;
		}
	}

	std::cout << "Total " << goodpoints << " points on " << npoints << " generated" << std::endl;	
	output.close();

	return 0;

}
