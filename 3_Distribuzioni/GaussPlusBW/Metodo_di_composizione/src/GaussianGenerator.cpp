#include "GaussianGenerator.h"
#include "TRandom3.h"
#include <iostream>
#include <cmath>

GaussianGenerator::GaussianGenerator()
{
    //ctor
    m_RndNum = new TRandom3(0);
    std::cout << "Default options for gaussian generator:\n"
              << "Mean: 0.0   Sigma: 1.0\n";
    m_mean = 0.0;
    m_sigma = 1.0;
    m_ntosum = 12;
    Refresh();
}


GaussianGenerator::GaussianGenerator(double mean, double sigma)
{
    //ctor
    m_RndNum = new TRandom3(0);
    m_mean = mean;
    m_sigma = sigma;
    m_ntosum = 12;
    Refresh();
}

GaussianGenerator::~GaussianGenerator()
{
    //dtor
    delete m_RndNum;
}

void GaussianGenerator::SetNtoSum(int N)
{
    m_ntosum = N;
    Refresh();
}

void GaussianGenerator::Refresh()
{
    m_offset = m_ntosum/2.0;
    m_coeff = m_sigma/sqrt(m_ntosum/12.0);
}

// numero uniforme
double GaussianGenerator::GetRandomUniform()
{
    return m_RndNum -> Rndm();
}

// num pseudocasuale con distribuzione gaussiana
double GaussianGenerator::GetRandom()
{
    double sum = 0;
    for (int i = 0; i < m_ntosum; i++)
    {
        sum = sum + GetRandomUniform();
    }
    return m_coeff* (sum - m_offset) + m_mean;
}

double GaussianGenerator::GetMean()
{
    return m_mean;
}

double GaussianGenerator::GetSigma()
{
    return m_sigma;
}
