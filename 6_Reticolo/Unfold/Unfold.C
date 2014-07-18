/* Questa macro esegue lo smearing gaussiano ed il successivo unfolding dei dati.
**
** Due file di testo sono necessari per l'esecuzione:
**
** 1. Un file di testo ad "alta statistica" per generare la matrice di migrazione
** 2. Un file di testo di "sample" su cui eseguire l'unfolding
**
** Entrambi i file contengono i dati "generator level" (cioè senza smearing applicato)
** e un offset distribuito gaussianamente viene sommato per eseguire lo smearing
**
*/

double Smear (double val)
{
	double binwidth = 6./100.; // range: [-3:3] su 100 bin disponibili nell'isto gen
	double csmear = 2; // multiplo della binwidth	
	
	double sigma = binwidth*csmear;
	
	// adds a random gaussian offset to the value to perform the smearing
	return (val + gRandom -> Gaus (0, sigma));
}

void Unfold()
{
	// usa gli errori degli istogrammi
	TH1::SetDefaultSumw2();

	int const nSmear=250; // numero di bin nella matrice di migrazione -- dati Smeared
	int const nGen=100; // numero di bin nella matrice di migrazione -- dati Generator (binning meno fine, v. TUnfold)
	double const xminSmear=-3.;
	double const xmaxSmear=3.;
	double const xminGen=-3.; 
	double const xmaxGen=3.; // la generazione avviene tra -3 e 3, ma la "rivelazione" (smearing) tra -2.5 e 2.5 per rimuovere gli effetti del bordo

	// ------------------ istogrammi --------------------------
	// matrice delle migrazioni
	TH2D* migrMatrix = new TH2D ("migrMatrix", "Matrice di migrazione; smeared; generator", nSmear, xminSmear, xmaxSmear, nGen, xminGen, xmaxGen);

	// dati da unfoldare
	TH1D* data_smeared = new TH1D ("data_smeared", "Spettro dopo lo smearing; x; N", nSmear, xminSmear, xmaxSmear);
	TH1D* data_smeared_clone = new TH1D ("data_smeared_clone", "Spettro dopo lo smearing; x; N", nGen, xminGen, xmaxGen); // clone del precedente ma con il binning Gen per un plot sovraimposto

	// dati a livello generatore --> solo per confronto; questo istogramma non è disponibile in una misura reale
	TH1D* data_generator = new TH1D ("data_generator", "Spettro prima dello smearing; x; N", nGen, xminGen, xmaxGen);

	// risultato dell'unfolding
	TH1D* data_unfolded = new TH1D ("data_unfolded", "Spettro dopo l'unfolding; x; N", nGen, xminGen, xmaxGen);

	// ------------------ file di dati -----------------------
	std::ifstream datafile_high_stat ("dati_high_statistics.txt");
	std::ifstream datafile_sample ("dati_sample.txt");


	// ------------------ riempi matrice di migrazione -------------
	double buf;
	int count = 0;
	
	while (1)
	{
		if (count % 100000 == 0)
		{cout << count << endl;}
		
		datafile_high_stat >> buf;
		
		if (datafile_high_stat.eof())
		{break;}

		migrMatrix->Fill(Smear(buf), buf); // x= smeared (detector); y = generator (MC)
		count++;	
	}
	
	// --------------- riempi istogramma di dati --------------------
	while (1)
	{
		if (count % 100000 == 0)
		{cout << count << endl;}		

		datafile_sample >> buf;
		if (datafile_sample.eof())
		{break;}
		
		data_generator -> Fill (buf);
		data_smeared -> Fill (Smear(buf));
		data_smeared_clone -> Fill (Smear(buf));
		
		count++;
	}

	// --------------- setup dell'unfolding ------------------------
	TUnfold unfold(migrMatrix,TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintArea);
	
	if(unfold.SetInput(data_smeared)>=10000)
	{std::cout<<"Possibili errori nei risultati dell'unfold\n";}

	// parametro di regolarizzazione + esecuzione dell'unfold
	double tau = 0.0045;
	double unfold_status;
	unfold_status = unfold.DoUnfold(tau);

	if (unfold_status >= 1.) // do unfold restituisce il max coeficiente di correlazione
	{cout << "\n\n**ATTENZIONE**\n\nErrori durante l'unfold!\nI risultati successivi non sono validi\n\n";}
	
	// ---------------- recupera i risultati dell'unfold ------------

	// binMap mappa linearmente i risultati dell'unfold nei bin dell'istogramma di output, e non salva under/overflow	
	int *binMap=new Int_t[nGen+2];
	for(Int_t i=1;i<=nGen;i++)
	{binMap[i]=i;}

	binMap[0]=-1; // no underflow
	binMap[nGen+1]=-1; // no overflow

	// recupera l'istogramma unfoldato
	unfold.GetOutput(data_unfolded,binMap);


	// ----------- disegna ----------------------------------
	data_generator->SetLineColor(kBlue);
	data_unfolded->SetLineColor(kRed);
	data_smeared_clone->SetLineColor(kGreen+1);

	data_generator->SetLineWidth(1.5);
	data_unfolded->SetLineWidth(1.5);
	data_smeared_clone->SetLineWidth(1.5);

	TLegend* l = new TLegend (0.63, 0.68, 0.89, 0.89);
	l->AddEntry(data_generator, "Pre-smearing", "l");
	l->AddEntry(data_smeared_clone, "Post-smearing (#sigma = 2#Deltax)", "l");
	l->AddEntry(data_unfolded, "Unfolded con #tau = 0.0045", "l");
	l->SetFillColor(kWhite);
	l->SetLineColor(kWhite);

	THStack* stack = new THStack ("stack", ";x;N");
	stack->Add(data_generator);
	stack->Add(data_smeared_clone);
	stack->Add(data_unfolded);

	stack->SetMinimum(0.);

	TCanvas* c1 = new TCanvas;
	stack->Draw("nostack H");
	//stack->GetHistogram()->GetXaxis()->SetTitle("x");
	stack->Draw("nostack H");
	l->Draw();
	

	// stampa aree
	cout << "AREE (no u/o flow) -- (con u/o flow)" << endl;
	cout << "Generator: " << data_generator->Integral() << " " << data_generator->Integral(0, nGen+1) << endl;
	cout << "Smeared: " << data_smeared->Integral() << " " << data_smeared->Integral(0, nSmear+1) << endl;
	cout << "Unfolded: " << data_unfolded->Integral() << " " << data_unfolded->Integral(0, nGen+1) << endl;

	// ------------------- residui dopo unfold ----------------
	
	TH1F *residui = new TH1F("residui","Bias;x;Generator - Unfolded",nGen,xminGen, xmaxGen);
	
	double err1, err2, errtot;
	for (Int_t i=1;i<=nGen;i++)
	{
		double diff = data_generator->GetBinContent(i) - data_unfolded->GetBinContent(i);
		err1 = data_generator->GetBinError(i);
		err2 = data_unfolded->GetBinError(i);
		errtot = err1 + err2;
		residui->SetBinContent(i,diff);
		residui->SetBinError(i, errtot);		
	}

	gStyle->SetOptStat(0);
	TCanvas* c2 = new TCanvas;
	TLine * ll = new TLine (-3, 0, 3, 0);
	ll->SetLineColor(kBlack);
	ll->SetLineStyle(2);
	
	residui->Draw("H");
	ll->Draw("same");
	


}
