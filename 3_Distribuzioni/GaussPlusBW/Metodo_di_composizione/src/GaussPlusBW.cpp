#include "GaussPlusBW.h"
#include "GaussianGenerator.h"
#include "BreitWignerGenerator.h"


GaussPlusBW::GaussPlusBW()
{
    //ctor
    m_RndNum = new TRandom3(0);
    m_gauss = new GaussianGenerator();
    m_bw = new BreitWignerGenerator();

    m_a = 0.1;
    std::cout << "Default options for gaussian+breit-wigner generator:\n"
              << "Gauss fraction 0.1   BW fraction: 0.9\n";

    m_mgaus = m_gauss -> GetMean();
    m_sigmagauss = m_gauss -> GetSigma();
    m_mbw = m_bw -> GetMean();
    m_fwhmbw = m_bw -> GetFWHM();

}

GaussPlusBW::GaussPlusBW(double a, double mgauss, double sigmagauss, double mbw, double fwhmbw)
{
    //ctor
    m_RndNum = new TRandom3(0);
    m_gauss = new GaussianGenerator(mgauss, sigmagauss);
    m_bw = new BreitWignerGenerator(mbw, fwhmbw);
    m_a = a;

    m_mgaus = m_gauss -> GetMean();
    m_sigmagauss = m_gauss -> GetSigma();
    m_mbw = m_bw -> GetMean();
    m_fwhmbw = m_bw -> GetFWHM();

}

GaussPlusBW::~GaussPlusBW()
{
    //dtor
    delete m_RndNum;
}

// numero uniforme
double GaussPlusBW::GetRandomUniform()
{
    return m_RndNum -> Rndm();
}

double GaussPlusBW::GetRandom()
{
    // produco un num causale per scegliere quale distr usare
    if (GetRandomUniform() < m_a)
    {
        return m_gauss -> GetRandom();
    }
    else
    {
        return m_bw -> GetRandom();
    }
}
