void PlotLandau()
{
	TCanvas* c1 = new TCanvas;
	TH1D * isto = new TH1D ("Landau", "Landau", 1000, -5, 20);
	std::ifstream data ("RandomLandau.txt", std::ios::in);

	double buf;
	while (1)
	{
		data >> buf;
		if (data.eof())
		break;
		isto -> Fill (buf);
	}

	data.close();

	isto -> SetFillColor (kBlue);
	isto -> SetFillStyle (3001);

	TF1 * landau = new TF1 ("landau", "[2]*TMath::Landau(x, [0],[1])", -5, 20);

	landau -> SetParName (0, "Mpv");
	landau -> SetParName (1, "Sigma");
	landau -> SetParName (2, "Norm");

	landau -> SetParameters (0., 1., 5.e4);

	gStyle -> SetOptStat (0);
	gStyle -> SetOptFit (1111);

	isto -> Fit("landau");
	isto -> Draw();

	c1 -> Print ("3_landau.pdf", "pdf");
	
}
