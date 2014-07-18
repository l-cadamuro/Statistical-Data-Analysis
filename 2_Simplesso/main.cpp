#include <iostream>
#include "Simplex.h"
#include <limits>


using namespace std;

int main()
{
    typedef std::numeric_limits<double> dbl;
	std::cout.precision(dbl::digits10);

    Simplex* simplesso = new Simplex;
    //simplesso->SetVerbose(true);
    simplesso->SetVertex(-2000,-8000, 3000,-6000, 9000,6000);

    TF2* himmelblau = new TF2 ("himmelblau", "(x*x + y - 11)*(x*x + y - 11) + (x + y*y - 7)*(x + y*y - 7)", 0, 10);
    simplesso -> SetFunction(himmelblau);

    //TF2* rosenbrock = new TF2 ("rosenbrock", "100*(y-x*x)*(y-x*x) + (1-x)*(1-x)", 0, 10);
    //simplesso -> SetFunction(rosenbrock);

    simplesso->Minimize();

    Point* min = simplesso->GetMinimumPtr();

    std::cout << "Ncalls: " << simplesso->GetNcalls() << endl;
    std::cout << "Min x: " << min -> GetX() << " Min y: " << min -> GetY() << endl << endl << endl;

    return 0;
}
