#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "TF2.h"
#include "Point.h"

class Simplex
{
    public:
        Simplex();
        Simplex(double x1, double y1, double x2, double y2, double x3, double y3);
        virtual ~Simplex();
        void SetVertex (double x1, double y1, double x2, double y2, double x3, double y3);
        void SetFunction (TF2* function); // linka la fz da minimizzare
        int GetNcalls (); // restituisce il numero di calls per la minimizzazione
        double GetDistance (Point* pi, Point* pj); // distanza tra i punti pi,pj = 1,2,3
        double EvalFunction(Point* p);
        Point* GetPoint (int npoint); // restituisce puntatore al vertice npoint = 0,1,2
        void Initialize(); // calcola tutte le variabili necessarie - da chiamare dopo
                            // l'assegnazione degli indirizzi dei punti in m_P[i]
        void Reflection();
        void Expansion();
        void Contraction(); // sono le tre operazioni base del simplesso
        void OneCall(); // è un singolo ciclo del processo di minimizzazione
                        // valuta quando chiamare le varie funzioni scritte sopra
        void Minimize(); // da chiamare per minimizzare la funzione
        Point* GetMinimumPtr(); // restituisce un puntatore al punto di minimo
        void SetVerbose (bool flag);

    protected:

    private:
        double m_alpha;
        double m_beta;
        double m_gamma; // coeff delle tre operazioni
        Point* m_P[3]; // array di puntatori a Pi, i = 1,2,3 punto
        Point* m_P1; // coordinate x,y
        Point* m_P2;
        Point* m_P3;
        Point* m_Pstar;
        Point* m_Pstarstar;
        Point* m_Pbar; // punti definiti nell'algoritmo
        bool m_verbose; // stampa i vertici durante la minimizzazione
        int m_h;
        int m_l; // indice (intero) ai punti h,l definiti nell'algoritmo
        int m_ncalls; // numero di chiamate per minimizzare la funzione
        TF2* m_f;   // la funzione da minimizzare; per rendere la classe più flessibile
                    // e non includere l'espressione di f in una funzione all'interno della classe
                    // si utilizza la TF2 di ROOT, linkata attraverso il puntatore m_f

        bool TerminatingCondition(); // restituisce true se la condizione di terminazione è vera

};

#endif // SIMPLEX_H
