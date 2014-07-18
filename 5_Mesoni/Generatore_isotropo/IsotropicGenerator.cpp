// produce numeri pseudocasuali secondo una distribuzione isotropa

#include <iostream>
#include <fstream>
#include <cmath>
#include "TRandom3.h"	

#define PI 3.14159265359


int main (int argc, char** argv)
{

	double phi, ctheta;
	std::ofstream output ("isotropo.txt", std::ios::out | std::ios::trunc);

	TRandom3* RandomNumber = new TRandom3(0);


	for (int i = 0; i < 5000000; i++)
	{
		if (i%500000 == 0)
		{std::cout << i << std::endl;}	
		// generazione casuale dell'evento in cos(theta), phi
		phi = (RandomNumber -> Rndm()) * 2. * PI;
		ctheta = (RandomNumber->Rndm() * 2.) -1. ;
		output << ctheta << "	" << phi << std::endl;	
	}

	output.close();
	return 0;
}
