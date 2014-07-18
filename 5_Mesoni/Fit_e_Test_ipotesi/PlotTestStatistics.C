/* Macro di ROOT che disegna le pdf delle statistiche di test
** ottenuto dal programma TestIpotesi.cpp
*/

void PlotTestStatistics()
{
	TCanvas* c1 = new TCanvas;	
	
	TH1D* vec = new TH1D ("vec", "vec", 800, -1500, 1200);
	TH1D* isotr = new TH1D ("isotr", "isotr", 800, -1500, 1200);

	// colori di riempimento trasparenti
	TColor *green = gROOT->GetColor(kGreen);
	green->SetAlpha(0.01);
	TColor *blue = gROOT->GetColor(kBlue);
	green->SetAlpha(0.01);

	TColor *t = new TColor(253,0.,0.,1.,"bluet",0.5);
	TColor *t = new TColor(254,0.,1.,0.,"greent",0.5);

	vec -> SetLineColor (kWhite);
	vec -> SetFillColor (254);
	isotr -> SetLineColor (kWhite);
	isotr -> SetFillColor (253);


	double buf;

	std::ifstream data_vec ("data_vec_pdf.txt", std::ios::in);
	while(1)
	{
		data_vec >> buf;
		if (data_vec.eof())
		{break;}
		vec -> Fill(buf);
	}
	data_vec.close();	

	std::ifstream data_isotr ("data_isotr_pdf.txt", std::ios::in);
	while(1)
	{
		data_isotr >> buf;
		if (data_isotr.eof())
		{break;}
		isotr -> Fill(buf);
	}
	data_isotr.close();	

	TF1* gauss = new TF1 ("gauss", "[2]*TMath::Gaus(x, [0], [1])", 0, 1);	
	gauss -> SetNpx(1000);

//	gauss -> SetParLimits (1, 0., 4000.);
//	gauss -> SetParLimits (2, 0., 10000.);

	gauss->SetParameters(isotr->GetMean(), isotr->GetRMS(), isotr->Integral());
	isotr -> Fit ("gauss", "","", -1400, -1000);

	gauss->SetParameters(vec->GetMean(), vec->GetRMS(), vec->Integral());
	vec -> Fit ("gauss", "", "", 700, 1100);

	isotr -> SetTitle ("5000 eventi per sample");
	vec -> SetTitle (isotr->GetTitle());	
	isotr -> GetXaxis() -> SetTitle ("log L_{vec} - log L_{isotr}");
	isotr -> GetYaxis() -> SetTitle ("N");
	vec -> GetXaxis() -> SetTitle (isotr->GetXaxis()->GetTitle());
	vec -> GetYaxis() -> SetTitle (isotr->GetYaxis()->GetTitle());

	gStyle -> SetOptFit (0);
	gStyle -> SetOptStat(0);

	TLine* l = new TLine(934.856,0, 934.856,40);
	l -> SetLineWidth(3);
	l -> SetLineColor(kGreen+2);	

	TLegend *legenda = new TLegend (0.13, 0.64, 0.47, 0.89);	
	legenda -> SetHeader ("Pdf della statistica di test");
	legenda -> AddEntry(vec, "Decadimento vettore", "f");
	legenda -> AddEntry(isotr, "Decadimento scalare", "f");
	legenda -> AddEntry(l, "Valore misurato", "l");      

	legenda -> SetFillColor (kWhite);

	vec->Draw();
	isotr->Draw("same");	
	legenda -> Draw();	
	l->Draw();

	c1 -> Print("5_test_ipotesi.pdf", "pdf");
}
