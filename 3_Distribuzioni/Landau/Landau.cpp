/* Implementa un generatore di numeri pseudocasuali
** secondo la distribuzione di Landau.
**
** La tecnica utilizzata è quella della funzione
** inversa, sfruttando l'inversa della
** cumulativa definita all'interno del
** pacchetto Math di ROOT (a sua volta è
** l'algoritmo implementato in RANLAN nella
** CERNLIB.
**
** L'output è un file di testo contenente i
** numeri generati.
*/

#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include "Math/DistFunc.h"

int main (int argc, char** argv)
{
	std::ofstream output ("RandomLandau.txt", std::ios::out);
	TRandom3* rnd = new TRandom3(0);

	double cval, sigma;

	std::cout << "\nGeneratore di numeri pseudocasuali"
			  << "distribuiti secondo una distribuzione di Landau\n"
			  << "\nInserire i parametri della dsitribuzione:\n";
	std::cout << "Posizione del picco? ";
	std::cin >> cval;
	std::cout << "Sigma? ";
	std::cin >> sigma;
	
	int Ngenerate = 1000000;
	
	double r;
	for (int i = 0; i < Ngenerate; i++)
	{
		if (i%100000 == 0)
		{std::cout << i << std::endl;}
		if (sigma <= 0)
		{output << 0 << std::endl;}
		else
		{
			r = rnd -> Rndm();
			output <<  (cval + ROOT::Math::landau_quantile(r, sigma)) << std::endl;		
		}
	}

	return 0;
}
