#include "Likelihood.h"
#include <cmath>
#include "TMinuit.h"
#include "TFitter.h"

Likelihood::Likelihood()
{
    //ctor
    m_data = new std::vector<double>*[2];
}

Likelihood::~Likelihood()
{
    //dtor
    delete[] m_data;
    delete[] m_parresult;
}

void Likelihood::LinkData(std::vector<double>** data)
{
    for(int i = 0; i < 2; i++)
    {m_data[i] = data[i];}

    // controlla il numero di dati
    if (m_data[0] -> size() != m_data[1] -> size())
    {std::cout << "Warning: different number of data in vectors 0 and 1\n";}

    return;
}

void Likelihood::LinkFunction(TF2* fitfunction)
{
    m_fitfunction = fitfunction;
    m_parresult = new double [m_fitfunction->GetNpar()];
    return;
}

// la likelihood richiede in ingresso un vettore di parametri
double Likelihood::ComputeLogLikelihood(double par[])
{
    double logl = 0;

    m_fitfunction -> SetParameters(par);
    for (int i = 0; i < m_data[0] -> size(); i++)
    {
        logl = logl + log(m_fitfunction -> Eval (m_data[0] -> at(i), m_data[1] -> at(i)) );
    }

    return logl;
}

// interfaccia per usare TMinuit
extern void Likelihood::MinuitLikelihood (int& nDim, double* gout, double& result, double par[], int flg)
{
    result = ComputeLogLikelihood(par);
}

void Likelihood::Minimize()
{
    TMinuit* minimizer = new TMinuit;
    // non stampare a schermo
    double p1 = -1;
	minimizer -> ExecuteCommand("SET PRINTOUT", &p1, 1);

//	minimizer->SetFCN(MinuitLikelihood);
/*
	//SetParameter(ParNum, "PARNAME", InitalValue, InitialError, LowerLimit, UpperLimit)
	//Se LowerLimit = UpperLimit = 0, non c'è nessun limite

	minimizer -> SetParameter(0, "X", 2,1,0,0);
	minimizer -> SetParameter(1, "Y", 2,1,0,0);

	minimizer -> ExecuteCommand ("SIMPLEX", 0,0);
	minimizer -> ExecuteCommand ("MIGRAD", 0,0);

	double bestX = minimizer->GetParameter(0);
	double bestY = minimizer->GetParameter(1);

	std::cout << "risultato x,y: " << bestX << " " << bestY << std::endl;
*/
}
