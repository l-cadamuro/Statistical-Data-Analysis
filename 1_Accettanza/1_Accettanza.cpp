/*
** Questo programma stima l'accettanza geometrica e l'efficienza di un sistema
** di rivelazione di particelle costituito da tre camere attraverso una simulazione
** MonteCarlo.
**
** I parametri fondamentali delle camere (dimensioni, distanza e efficienza)
** sono definiti all'inizio del programma. Le camere sono identificate dalle
** lettere A, B, C.
**
** Per rendere più efficiente la simulazione, il programma valuta
** l'angolo solido maggiore coperto dalle camere e genera eventi solo all'interno
** di questo. Gli eventi sono generati in coordinate (theta, phi) sferiche con
** centro nel punto di produzione (0,0,0) (in coordinate cartesiane x,y,z).
**
** Per ogni evento generato si valuta se esso incide (hit) o non incide (miss)
** sulla camera; in caso di hit si valuta sulla base dell'efficienza se la particella
** è rivelata (detected) o non rivelata (undetected); in caso di miss l'evento
** è automaticamente undetected.
**
** L'output è un file di testo contenente su ogni riga un evento e tre colonne relative
** allo stato hit/miss di ogni camera.
**
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include "TRandom3.h"

// lunghezza del lato delle camere [m]
#define L_A 1.0
#define L_B 2.0
#define L_C 5.0

// distanza dall'origine delle camere [m]
#define d_A 2.0
#define d_B 5.0
#define d_C 10.0

// efficienza di ogni camera
#define Eff 0.95

// numero di eventi da generare
#define N_events 1000000

#define PI 3.14159265359


// ----------------------------------------------------------
// restituisce il massimo tra tre numeri
double FindMax (double x, double y, double z) 
{
	double max;

	max = std::max(x,y);
	max = std::max(max, z);	
	
	return max;
}


// ----------------------------------------------------------
// date le coordinate theta, phi e la distanza della camera
// dall'origine, calcola le coordinate (x,y) dell'evento
// all'intersezione con la superficie della camera e le copia
// nelle variabili puntate da x, y
void GetXY (double theta, double phi, double dist, double* x, double* y)
{
	double R;

	// calcola il raggio in coordinate sferiche dell'intersezione
	R = dist/cos(theta);

	// calcola le coordinate
	*x = R*sin(theta)*cos(phi); 
	*y = R*sin(theta)*sin(phi);

	return;
}


// ----------------------------------------------------------
int main (int argc, char** argv)
{

	std::cout << "\n\n"
		  << "     SIMULAZIONE MONTECARLO\n"
		  << "     Autore Luca Cadamuro \n\n";

	// tabella contenente i parametri delle camere
	double ChamberLD [3][2];
	ChamberLD [0][0] = L_A;
	ChamberLD [0][1] = d_A;
	ChamberLD [1][0] = L_B;
	ChamberLD [1][1] = d_B;
	ChamberLD [2][0] = L_C;
	ChamberLD [2][1] = d_C;
	
	// generatore di numeri pseudocasuali
	TRandom3* RandomNumber = new TRandom3(0);

	// file di testo con i risultati della simulazione
	std::ofstream datafile ("Risultati.txt", std::ios::out);
	datafile << "Theta	" << "Phi	"
		<< "HitA	" << "HitB	" << "HitC	"
		<< "DetA	" << "DetB	" << "DetC	"
		<< std::endl;

	// calcolo la semiapertura maggiore dei coni
	double alpha_A, alpha_B, alpha_C, alpha;
	alpha_A = atan (0.5*ChamberLD[0][0]*sqrt(2.0)/ChamberLD[0][1]);
	alpha_B = atan (0.5*ChamberLD[1][0]*sqrt(2.0)/ChamberLD[1][1]);
	alpha_C = atan (0.5*ChamberLD[2][0]*sqrt(2.0)/ChamberLD[2][1]);

	alpha = FindMax(alpha_A, alpha_B, alpha_C);

	std::cout << "Semiapertura massima: " << alpha << " rad\n\n";

	double theta, phi, x, y;
	bool IsHit[3];
	bool IsDetected[3];	

	// la generazione di eventi è in cos(theta), tra acos(0) = 1 e acos(alpha)
	double mincos = cos(alpha);

	// ciclo sul numero di eventi da generare
	for (int event = 0; event < N_events; event++)
	{

		if (event%100000 == 0)		
		{std::cout << event << std::endl;}

		// generazione casuale dell'evento in cos(theta), phi
		phi = (RandomNumber -> Rndm()) * 2 * PI;
		theta = acos ( (RandomNumber->Rndm() * (1-mincos)) + mincos );

		// ciclo sulle camere
		for (int chamber = 0; chamber < 3; chamber++)
		{
			// calcola x,y sulla camera
			GetXY (theta, phi, ChamberLD[chamber][1], &x, &y);

			// confronta la posizione con i lati della camera
			if (	
				x < -0.5*ChamberLD[chamber][0] ||
				x > 0.5*ChamberLD[chamber][0] ||
				y < -0.5*ChamberLD[chamber][0] ||
				y > 0.5*ChamberLD[chamber][0]
			   )
			{IsHit[chamber] = false;}
			else
			{IsHit[chamber] = true;}

				
			// valuta efficienza
			if (IsHit[chamber] == false)
			{
				IsDetected[chamber] = false;
			}
			else
			{
				if ((RandomNumber -> Rndm() ) <= Eff)
				{IsDetected[chamber] = true;}
				else
				{IsDetected[chamber] = false;}
			}
		}		
		
		// stampa sul file di testo i risultati
		datafile << theta << "	" << phi << "	"
		<< IsHit[0] << "	" << IsHit[1] << "	" << IsHit[2] << "	"
		<< IsDetected[0] << "	" << IsDetected[1] << "	" << IsDetected[2]
		<< std::endl;

	}

	std::cout << "\n\nSimulati " << N_events << " eventi" << std::endl;

	datafile.close();
}
