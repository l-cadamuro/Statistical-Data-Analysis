void plot_efficienze()
{
	TCanvas* c1 = new TCanvas;
	TGraph* graph = new TGraph;

	std::ifstream data ("efficienze_hit_miss_rettangolo.txt");

	TString stringa;

	data >> stringa;
	data >> stringa;
	data >> stringa;

	double x, n;
	int i;

	i = 0;

	while (1)
	{
		data >> x;
		if (data.eof())
		{break;}
		data >> x;
		data >> n;

		graph -> SetPoint (i, x, n/10000000.);
	
		i++;
	}

	graph -> SetMarkerStyle (20);
	graph -> SetMarkerColor (kBlue);
	graph -> SetMarkerSize (0.8);

	graph -> GetXaxis() -> SetTitle ("Semilarghezza intervallo");
	graph -> GetYaxis() -> SetTitle ("Efficienza");

	graph->SetTitle("Efficienze del metodo hit-and-miss");

	/*
	c1 -> SetLogy();
	c1 -> SetLogx();
	*/	

	graph -> Draw("apc");
	c1->Print("3_hitmiss_efficienze.pdf", "pdf");
}
