#include "Simplex.h"
#include <iostream>
#include <cmath>

Simplex::Simplex()
{
    //ctor
    m_alpha = 1.0;
    m_beta = 0.5;
    m_gamma = 2.0;

    m_ncalls = 0;

    m_P1 = new Point (0,0);
    m_P2 = new Point (0,0);
    m_P3 = new Point (0,0);
    m_Pstar = new Point (0,0);
    m_Pstarstar = new Point (0,0);
    m_Pbar = new Point (0,0);

    m_P[0] = m_P1;
    m_P[1] = m_P2;
    m_P[2] = m_P3;

    m_f = NULL;
    m_verbose = false;
}

Simplex::Simplex(double x1, double y1, double x2, double y2, double x3, double y3)
{
    m_alpha = 1.0;
    m_beta = 0.5;
    m_gamma = 2.0;

    m_ncalls = 0;

    m_P1 = new Point (x1,y1);
    m_P2 = new Point (x2,y2);
    m_P3 = new Point (x3,y3);
    m_Pstar = new Point (0,0);
    m_Pstarstar = new Point (0,0);
    m_Pbar = new Point (0,0);

    m_P[0] = m_P1;
    m_P[1] = m_P2;
    m_P[2] = m_P3;

    m_f = NULL;
    m_verbose = false;
}

Simplex::~Simplex()
{
    //dtor
    delete m_P1;
    delete m_P2;
    delete m_P3;
    delete m_Pstar;
    delete m_Pstarstar;
    delete m_Pbar;
}

void Simplex::SetVertex(double x1, double y1, double x2, double y2, double x3, double y3)
{
    m_P[0] -> SetPoint (x1,y1);
    m_P[1] -> SetPoint (x2,y2);
    m_P[2] -> SetPoint (x3,y3);

    return;
}

void Simplex::SetFunction(TF2* function)
{
    m_f = function;
    return;
}

int Simplex::GetNcalls()
{
    return m_ncalls;
}

double Simplex::GetDistance(Point* pi, Point* pj)
{
    return sqrt( (pi->GetX() - pj->GetX())*(pi->GetX() - pj->GetX()) + (pi->GetY() - pj->GetY())*(pi->GetY() - pj->GetY()) );
}

double Simplex::EvalFunction(Point* p)
{
    if (m_f == NULL)
    {
        std::cout << "No function to minimize\n";
        return 0;
    }
    else
    {
        return ( m_f -> Eval( p->GetX(), p->GetY() ) );
    }

}

Point* Simplex::GetPoint(int npoint)
{
    return m_P[npoint];
}

bool Simplex::TerminatingCondition()
{
    double sum = 0;
    double val = 0;
    for (int i = 0; i < 3; i++)
    {
        val = EvalFunction(m_P[i]) - EvalFunction(m_Pbar);
        sum = sum + val*val;
    }

    if (sqrt(sum/3.) < 1.e-8)
    {return true;}

    else
    {return false;}
}

void Simplex::Initialize()
{
    // calcola l, h
    int l = 0;
    int h = 0;

    double val_l = EvalFunction(m_P[0]);
    double val_h = EvalFunction(m_P[0]);

    double val_curr;

    for (int i = 1; i < 3; i++)
    {
        val_curr = EvalFunction(m_P[i]);

        if (val_curr < val_l)
        {
            l = i;
            val_l = val_curr;
        }

        else if (val_curr > val_h)
        {
            h = i;
            val_h = val_curr;
        }
    }

    m_l = l;
    m_h = h;

    // calcola la centroide m_Pbar
    double x_c = 0;
    double y_c = 0;

    for (int i = 0; i < 3; i++)
    {
        if (i != h)
        {
            x_c = x_c + m_P[i] -> GetX();
            y_c = y_c + m_P[i] -> GetY();
        }
    }

    m_Pbar -> SetPoint(0.5*x_c, 0.5*y_c);

    return;

}

void Simplex::Reflection()
{
    m_Pstar -> SetX( (1.+m_alpha)*m_Pbar->GetX() - m_alpha*m_P[m_h]->GetX() );
    m_Pstar -> SetY( (1.+m_alpha)*m_Pbar->GetY() - m_alpha*m_P[m_h]->GetY() );
    return;
}

void Simplex::Expansion()
{
    m_Pstarstar -> SetX( m_gamma*m_Pstar->GetX() + (1.-m_gamma)*m_Pbar->GetX() );
    m_Pstarstar -> SetY( m_gamma*m_Pstar->GetY() + (1.-m_gamma)*m_Pbar->GetY() );
    return;
}

void Simplex::Contraction()
{
    m_Pstarstar -> SetX( m_beta*m_P[m_h]->GetX() + (1.-m_beta)*m_Pbar->GetX() );
    m_Pstarstar -> SetY( m_beta*m_P[m_h]->GetY() + (1.-m_beta)*m_Pbar->GetY() );
    return;
}

void Simplex::OneCall()
{
    // calcola l,h e centroide
    Initialize();

    Reflection();

    double ystar = EvalFunction(m_Pstar);
    double y_l = EvalFunction(m_P[m_l]);
    double y_h = EvalFunction(m_P[m_h]);

    // il punto riflesso sta sotto y_l
    if (ystar < y_l)
    {
        Expansion();

        // espansione riuscita
        if (EvalFunction(m_Pstarstar) < y_l)
        {
            m_P[m_h]->SetPoint(m_Pstarstar);
            return;
        }

        // espansione fallita
        else
        {
            m_P[m_h]->SetPoint(m_Pstar);
            return;
        }
     }

    // il punto riflesso non sta sotto y_l
    else
    {
        // calcola se il punto riflesso è maggiore di tutti gli altri
        int counter = 0;
        for (int i = 0; i < 3; i++)
        {
            if ( ystar > EvalFunction(m_P[i]) )
            {counter++;}
        }

        // il punto non è maggiore di tutti gli i != h
        if (counter < 2)
        {
            m_P[m_h]->SetPoint(m_Pstar);
            return;
        }

        // il punto è maggiore di tutti gli i (!)= h
        else
        {
            // se non è maggiore di h, sostituisco h
            if (counter < 3)
            {
                m_P[m_h]->SetPoint(m_Pstar);
                y_h = EvalFunction(m_P[m_h]);
            }

            Contraction();

            // il punto contratto non è maggiore di y_h
            if (EvalFunction(m_Pstarstar) <= y_h)
            {
                m_P[m_h]->SetPoint(m_Pstarstar);
                return;
            }
            // il punto contratto è maggiore di y_h
            else
            {
                double x_point_l = m_P[m_l]->GetX();
                double y_point_l = m_P[m_l]->GetY();
                for (int i = 0; i < 3; i++)
                {
                    m_P[i]->SetX(0.5*(m_P[i]->GetX() + x_point_l));
                    m_P[i]->SetY(0.5*(m_P[i]->GetY() + y_point_l));
                }
                return;
            }
        }
   }

}

void Simplex::Minimize()
{
    m_ncalls = 0; // reset del numero di calls

    int call = 0;

    int status = 0;
    // 0: call limit
    // 1: converged

    std::cout << "\nStarting Minimization\n";

    // print vertex position
    if (m_verbose)
    {
        for (int p = 0; p < 3; p++)
        {std::cout << GetPoint(p)->GetX() << " " << GetPoint(p)->GetY() << "     ";}
        std::cout << std::endl;
    }

    while(call < 200)
    {
        OneCall();

        // print vertex position
        if (m_verbose)
        {
            for (int p = 0; p < 3; p++)
            {std::cout << GetPoint(p)->GetX() << " " << GetPoint(p)->GetY() << "     ";}
            std::cout << std::endl;
        }

        if (TerminatingCondition() == true)
        {
            status = 1;
            break;
        }

        call++;
    }

    m_ncalls = call;

    if (status == 1)
    {std::cout << "SIMPLEX MINIMIZATION CONVERGED\n";}

    if (status == 0)
    {std::cout << "SIMPLEX MINIMIZATION NOT CONVERGED (CALL LIMIT)\n";}

    return;
}

Point* Simplex::GetMinimumPtr()
{
    Initialize();
    return m_P[m_l];
}

void Simplex::SetVerbose (bool flag)
{
    m_verbose = flag;
}
