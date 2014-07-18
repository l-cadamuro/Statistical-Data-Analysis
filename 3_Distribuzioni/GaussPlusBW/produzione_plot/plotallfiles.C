// disegna insieme gli istogrammi di hit and miss e di composizione

#include <algorithm>

#define PI 3.14159265358979323846

double BreitWigner (double x, double mean, double fwhm)
{
	double gamma = fwhm/2.0;
	double bw = gamma / (PI * ( gamma*gamma + (x-mean)*(x-mean) ) );
	return bw;
}

double Gaussian (double x, double mean, double sigma)
{
	double gaus = 1.0/sqrt(2.0*PI*sigma*sigma) * exp (-0.5 * (x-mean)*(x-mean)/(sigma*sigma) );
	return gaus;
}

// somma di breit-wigner e gaussiana con pesi a, b
Double_t Function (Double_t* x, Double_t * par)

{
	double f = par[5] * ( par[0]*Gaussian(x[0], par[1], par[2]) + (1.-par[0])*BreitWigner(x[0], par[3], par[4]) );
	return f;
}


void plotallfiles()
{
	TCanvas* c1 = new TCanvas;

	std::ifstream hitmiss ("hit_miss_rect.txt", std::ios::in);	
	std::ifstream composizione ("composizione.txt", std::ios::in);

	TH1D * istohitmiss = new TH1D ("istohitmiss", "istohitmiss", 1000, -20, 20 );
	istohitmiss -> SetLineColor (kGreen);
	istohitmiss -> SetFillColor (kGreen);
	istohitmiss -> SetFillStyle (3001);
//	istohitmiss -> SetLineWidth(2);
	TH1D * istocompos = new TH1D ("istocompos", "istocompos", 1000, -20, 20 );
	istocompos -> SetLineColor (kRed);
	istocompos -> SetLineWidth(2);

	double x;

	while(1)
	{
		hitmiss >> x;
		if (hitmiss.eof())
		{break;}
		istohitmiss -> Fill(x);
	}


	while(1)
	{
		composizione >> x;
		if (composizione.eof())
		{break;}
		istocompos -> Fill(x);
	}

	hitmiss.close();
	composizione.close();

	double chitmiss, ccompos; // coefficienti overall per normalizzare le distribuzioni
	// fitto per determinare i coefficienti
	
	TF1* f = new TF1 ("f", Function, -20, 20, 6);

	f -> SetParLimits (0, 0.005, 0.2);
	f -> SetParLimits (1, -1, 1);	
	f -> SetParLimits (2, 17, 23);
	f -> SetParLimits (3, -1, 1);	
	f -> SetParLimits (4, 1, 3);	
	
	f -> SetParameters (0.1, 0, 20, 0, 2, 10000000/(0.38*1000));

	/*
	f->SetParName(0, "#alpha");
	f->SetParName(1, "x_{g}");
	f->SetParName(2, "#sigma");
	f->SetParName(3, "x_{bw}");
	f->SetParName(4, "#Gamma");
	f->SetParName(5, "Ampiezza");
	*/	

	istohitmiss -> Fit ("f", "R0");
	chitmiss = f -> GetParameter(5);

	istocompos -> Fit ("f", "R0");
	ccompos = f -> GetParameter(5);

	// scalo il secondo istogramma sul primo
	istocompos -> Scale (chitmiss/ccompos);

	// introduco un offset sul primo histo per vedere meglio le due distribuzioni
	/*for (int i = 0; i < 1000; i++)
	{
		int binval = istohitmiss -> GetBinContent(i);
		istohitmiss -> SetBinContent(i, binval + 500);
	}
	*/

	gStyle -> SetOptStat(0);
	//gStyle -> SetOptFit(1111);

	istohitmiss -> GetXaxis() -> SetTitle ("x");
	istohitmiss -> GetYaxis() -> SetTitle ("N");
	istohitmiss -> SetTitle ("");

	TLegend *legenda = new TLegend (0.95, 0.75, 0.6, 0.5);
	legenda -> SetHeader ("Metodo di generazione");
   
	legenda -> AddEntry(istohitmiss, "Hit and miss", "f");
	legenda -> AddEntry(istocompos, "Composizione", "l");
      
	legenda -> SetFillColor (kWhite);

	istohitmiss -> Draw();
	istocompos -> Draw("same");
	legenda -> Draw();
	c1->Print("3_hitmiss_vs_compos.pdf", "pdf");

}
