/*
** Il programma calcola la costante di eulero-mascheroni
** attraverso l'espressione:
**
** gamma = lim(n->inf) { sum_(k=1)^n [1/k] - log(n) }
**
** valore teorico: 0,57721 56649 01532 86060 65120 90082 40243 10421 59335 93992 35988 05767...
*/

#include <iostream>
#include <cmath>
#include <limits>
#include <fstream>
#include <iomanip>

#define Nstep 1e11L // 10^12 max raggiunto
// la "L" al termine del numero indica che deve essere arrotondato come
// un long double e non un double
#define GAMMA_TEO 0.577215664901532860606512090082402431042159335939923598805767L

int main()
{
	long double sum = 0.;

	long int ktoshow = Nstep/10;

	// per stampare il maggior numero di cifre possibile senza arrotondamenti
	typedef std::numeric_limits<long double> dbl;
	std::cout.precision(dbl::digits10);

	std::cout << "Starting calculation\n";

	
	// utilizzo long che, sulle macchine a 64 bit, usa 8 byte vs 4 byte di int
	// nota che sulle macchine a 32 bit il max è 4 bytes 
	
	// somma eseguita da 1 a N --> meno preciso
	/*	
	for (long int k = 1; k <= Nstep; k++)
	{
		if (k%ktoshow == 0)
		{
			std::cout << "step: " << k << std::endl;
		}		
		sum = sum + 1./k;
	}
	*/

	// procede dal più piccolo al più grande, per ridurre gli errori
	for (long int k = Nstep; k >= 1; k--)
	{
		if (k%ktoshow == 0)
		{
			std::cout << "step: " << k << std::endl;
		}		
		sum = sum + 1./k;
	}


//	std::cout << sizeof(double) << " " << sizeof(long double);
	
	long double gamma = sum - log(Nstep);

	std::cout << "Gamma: " << gamma << " Sum: " << sum << std::endl;
	std::cout << "Error: " << std::abs(GAMMA_TEO - gamma) << std::endl;

	// produce un file di testo con i risultati
	// il file non è cancellato ma aggiornato
	std::ofstream output ("gamma.txt", std::ios::out | std::ios::app);

	output << Nstep << std::setprecision(log10(Nstep) + 3) << "	" << gamma << "	" << std::abs(GAMMA_TEO - gamma)
		<< "	" << sum << "	" << std::endl;
}


