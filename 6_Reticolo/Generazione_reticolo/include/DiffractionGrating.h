#ifndef DIFFRACTIONGRATING_H
#define DIFFRACTIONGRATING_H

#include "Complex.h"

/*
** Tutte le distanze sono misurate in multipli della lunghezza d'onda
** lambda della luce incidente, che è posta = 1.
** Per cui la distanza L e il passo step sono interi da esprimersi
** in multipli di lambda.
**
** L'intensità I_0, definita come l'intensità a una lambda di
** distanza dallo schermo (cioe E = sqrt(I_0)/r * exp(...); se
** r = 1, |E|^2 = I_0 ) è posta = 1 e tutte le altre intensità sono
** riferite ad essa.
**
** Il sistema di coordinate è la posizione x lungo lo schermo;
** x = 0 corrisponde all'intersezione della perpendicolare alla prima
** fenditura, e x è orientata nella direzione in cui ci sono le
** altre fenditure.
*/

class DiffractionGrating
{
    public:
        DiffractionGrating(int Nslits, double Step, double L);
        virtual ~DiffractionGrating();
        double GetDistance (double x, int Nslit);
        double ComputeIntensity (double x);
    protected:
    private:
        int m_Nslits;
        double m_Step;
        double m_L;
};

#endif // DIFFRATIONGRATING_H
