#include "BreitWignerGenerator.h"
#include <cmath>

#define PI 3.14159265358979323846

BreitWignerGenerator::BreitWignerGenerator()
{
    //ctor
    m_RndNum = new TRandom3(0);
    std::cout << "Default options for breit-wigner generator:\n"
              << "Mean: 0.0   FWHM: 2.0\n";
    m_mean = 0.0;
    m_gamma = 1.0;

}

BreitWignerGenerator::BreitWignerGenerator(double mean, double fwhm)
{
    m_RndNum = new TRandom3(0);
    m_mean = mean;
    m_gamma = fwhm/2.0;

}

BreitWignerGenerator::~BreitWignerGenerator()
{
    //dtor
    delete m_RndNum;
}

// numero uniforme
double BreitWignerGenerator::GetRandomUniform()
{
    return m_RndNum -> Rndm();
}


double BreitWignerGenerator::GetRandom()
{
    return m_mean + m_gamma*tan(GetRandomUniform()*PI - 0.5*PI);
}

double BreitWignerGenerator::GetMean()
{
    return m_mean;
}

double BreitWignerGenerator::GetFWHM()
{
    return 2*m_gamma;
}
