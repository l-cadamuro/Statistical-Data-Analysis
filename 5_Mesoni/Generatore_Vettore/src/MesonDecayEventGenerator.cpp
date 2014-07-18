#include "MesonDecayEventGenerator.h"
#include <cmath>

#define ALPHA 0.65
#define BETA 0.06
#define GAMMA -0.18

#define UPPERF 0.180

#define PI 3.14159265359


MesonDecayEventGenerator::MesonDecayEventGenerator()
{
    //ctor
    m_Random = new TRandom3(0);
}

MesonDecayEventGenerator::~MesonDecayEventGenerator()
{
    //dtor
    delete m_Random;
}

// la generazione è in x = cos(theta), y = phi
double MesonDecayEventGenerator::ComputeFunction(double x, double y)
{
   	double f;
	double costh = x;
	double sinth = sqrt(1.-x*x);
	double sin2th = 2.*x*sqrt(1.-x*x);
	double cosphi = cos(y);
	double cos2phi = cos(2.*y);

    f = (0.75/PI)*( 0.5*(1-ALPHA) + 0.5*(3.*ALPHA - 1)*costh*costh - BETA*sinth*sinth*cos2phi - sqrt(2)*GAMMA*sin2th*cosphi);
    return f;
}

void MesonDecayEventGenerator::GetRandomEvent(double& x, double& y)
{
    // limite superiore
    double randx, randy, randnum;

    while (1)
    {
        randx = 2.*(m_Random->Rndm()) - 1.;
        randy = (m_Random->Rndm())*2.*PI;
        randnum = (m_Random->Rndm())*UPPERF;
        if (randnum <= ComputeFunction(randx, randy))
        {
            x = randx;
            y = randy;
            break;
        }
    }

    return;
}

int MesonDecayEventGenerator::CheckUpperBound(int ntests)
{
    int noverflow = 0;
    double randx, randy;

    for (int i = 0; i < ntests; i++)
    {
        randx = 2.*(m_Random->Rndm()) -1.;
        randy = (m_Random->Rndm())*2.*PI;

        if (ComputeFunction(randx, randy) > UPPERF)
        {noverflow++;}
    }

    return noverflow;
}
