#include "DiffractionGrating.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265359

DiffractionGrating::DiffractionGrating(int Nslits, double Step, double L)
{
    //ctor
    m_Nslits = Nslits;
    m_Step = Step;
    m_L = L;
}

DiffractionGrating::~DiffractionGrating()
{
    //dtor
}

// distanza sullo schermo alla posizione x dalla fessura Nslit
double DiffractionGrating::GetDistance (double x, int Nslit)
{
    double deltax = x - m_Step*Nslit;
    return sqrt(m_L*m_L + deltax*deltax);
}

double DiffractionGrating::ComputeIntensity (double x)
{
    Complex SumE (0,0);
    Complex E (0,0);
    double dist;

    for (int i = 0; i < m_Nslits; i++)
    {
        dist = GetDistance(x, i);
        // l'intensità scala come la misura del cerchio, cioè come 1/r
        // quindi E = sqrt(I) scala come 1/sqrt(r)
        E.SetPolar( 1./sqrt(dist), 2.0*PI*dist );
//        E.SetPolar( 1./dist, 2.0*PI*dist );
        SumE += E;

    }

    return SumE.GetSquareModulus();

}
