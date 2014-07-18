/* Testa l'ipotesi di decadimento da mesone vettore
** rispetto a quella di mesone scalare
**
** La pdf della statistica di test è ottenuta
** calcolando la statistica di test su un sample
** grande (5x10^6) di eventi generati in modo isotropo
** e secondo la F da decadimento vettore.
**
** Questi dati sono raccolti in due file di testo
** che il programma analizza
*/


#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#define PI 3.14159265359

std::vector<double> theta;
std::vector<double> phi;

std::vector<double> vec_pdf;
std::vector<double> isotr_pdf;

// distribuzione per mesone vettore --  legge valori cos(theta), phi
double Vector (double x, double y)
{
	double alpha = 0.65;
	double beta = 0.06;
	double gamma = -0.18;
	double f;
	double costh = x;
	double sinth = sqrt(1.-x*x);
	double sin2th = 2.*x*sqrt(1.-x*x);
	double cosphi = cos(y);
	double cos2phi = cos(2.*y);

	f = (0.75/PI)*( 0.5*(1.-alpha) + 0.5*(3.*alpha - 1.)*costh*costh - beta*sinth*sinth*cos2phi - sqrt(2.)*gamma*sin2th*cosphi);
   
	return f;
}

// distribuzione per mesone scalare --  legge valori cos(theta), phi
double Scalar (double x, double y)
{
	double f = 1./(4.*PI);
	return f;
}

// calcola likelihood per caso vettore
double ComputeLogLVector ()
{
	double logl = 0;
	double f;

	for (int i = 0; i < theta.size(); i++)
	{
		f = Vector(theta.at(i), phi.at(i));
		logl = logl + log(f);
	}

    return logl;
}

// calcola la likelihood per caso scalare
double ComputeLogLScalar ()
{
	double logl = 0;
	double f;

	for (int i = 0; i < theta.size(); i++)
	{
		f = Scalar(theta.at(i), phi.at(i));
		logl = logl + log(f);
	}

    return logl;
}

// calcola la statistica di test
double ComputeTestStatistics()
{
	return (ComputeLogLVector() - ComputeLogLScalar());
}


int main (int argc, char** argv)
{
	double buf1, buf2;
	
	// acquisizione dati di 5x10^6 eventi (sample per pdf) - vettore
	std::ifstream dati_vec ("MesonDecayDataBig.txt", std::ios::in);
	// tot 1000 sample da 5000 eventi	
	for (int sample = 0; sample < 1000; sample ++)
	{
		if (sample % 100 == 0)
		std::cout << "Vec: " << sample << std::endl;		
		for (int ev = 0; ev < 5000; ev ++)
		{
			dati_vec >> buf1;
			dati_vec >> buf2;
			theta.push_back(buf1);
			phi.push_back(buf2);
		}
		vec_pdf.push_back(ComputeTestStatistics());
		theta.clear();
		phi.clear();
	}
	dati_vec.close();

	std::ifstream dati_isotr ("isotropo.txt", std::ios::in);
	// tot 1000 sample da 5000 eventi	
	for (int sample = 0; sample < 1000; sample ++)
	{
		if (sample % 100 == 0)
		std::cout << "Isotr: " << sample << std::endl;	
		for (int ev = 0; ev < 5000; ev ++)
		{
			dati_isotr >> buf1;
			dati_isotr >> buf2;
			theta.push_back(buf1);
			phi.push_back(buf2);
		}
		isotr_pdf.push_back(ComputeTestStatistics());
		theta.clear();
		phi.clear();
	}
	dati_isotr.close();

		
	// acquisizione campione "sperimentale"
		
	std::ifstream dati ("MesonDecayData.txt", std::ios::in);
	while(1)
	{
		dati >> buf1;
		if (dati.eof())
		{break;}
		dati >> buf2;
		theta.push_back(buf1);
		phi.push_back(buf2);
	}
	dati.close();	
	
	std::cout << "Statistica di ipotesi: " << ComputeTestStatistics() << std::endl;	
	
	std::ofstream data_vec_pdf ("data_vec_pdf.txt", std::ios::out | std::ios::trunc);
	std::ofstream data_isotr_pdf ("data_isotr_pdf.txt", std::ios::out | std::ios::trunc);
	
	for (int i = 0; i < vec_pdf.size(); i++)
	{
		data_vec_pdf << vec_pdf.at(i) << std::endl;
		data_isotr_pdf << isotr_pdf.at(i) << std::endl;
	}

	return 0;
}
