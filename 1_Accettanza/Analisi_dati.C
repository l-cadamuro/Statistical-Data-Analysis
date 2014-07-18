// calcola il num di rivelazioni (1) in un vettore 
int NumberOfOne (std::vector<int>* vec, int Nevents)
{
	int Ones = 0;
	for (int i = 0; i < Nevents; i++)
	{
		if (vec -> at(i) == 1)
		{Ones++;}
	}

	return Ones;
}

// calcola il num di eventi in cui ho avuto almeno n = 1,2,3 coincidenze 
int NCoincidences (std::vector<int>* vec1, std::vector<int>* vec2, std::vector<int>* vec3, int Nevents, int nCoinc)
{
	int detected = 0;
	int sum;
	for (int i = 0; i < Nevents; i++)
	{
		sum = vec1->at(i) + vec2->at(i) + vec3->at(i);
		if ( sum > (nCoinc -1) )
		{detected++;}
	}

	return detected;
}

void Analisi_dati()
{
	TCanvas* c1 = new TCanvas;

	std::vector<double> theta;
	std::vector<double> phi;
	std::vector<int> HitA;
	std::vector<int> HitB;
	std::vector<int> HitC;
	std::vector<int> DetA;
	std::vector<int> DetB;
	std::vector<int> DetC;
	
	std::ifstream datafile ("Risultati.txt", std::ios::in);

	double dato;
	TString header;

	double dati[1000000];

	// scarta headers
	for (int i = 0; i < 8; i++)
	{datafile >> header;}

	int counter = 0;

	// lettura dati
	while (1)
	{
		if (counter%100000 == 0)
		{std::cout << counter << std::endl;}
		// uso questo set istruzioni affinché la lettura termini
		// quando legge l'eof del file
		datafile >> dato;
		if(datafile.eof())
		{break;}
		theta.push_back(dato);
		
		datafile >> dato;
		phi.push_back(dato);

		datafile >> dato;
		HitA.push_back(dato);		

		datafile >> dato;
		HitB.push_back(dato);		

		datafile >> dato;
		HitC.push_back(dato);		

		datafile >> dato;
		DetA.push_back(dato);		

		datafile >> dato;
		DetB.push_back(dato);		

		datafile >> dato;
		DetC.push_back(dato);		
		
		counter++;
	}

	int Nevents = counter; // alla fine counter = 1000000

	gStyle->SetOptStat(0);

	TH1D* histophi = new TH1D ("", "Hits number vs #varphi", 1000, 0, 2*TMath::Pi());
	TH1D* histotheta = new TH1D ("", "Hits number vs #theta", 1000, 0, 0.35);
	
	histophi->GetXaxis()->SetTitle("#varphi [rad]");
	histophi->GetYaxis()->SetTitle("counts");

	histotheta->GetXaxis()->SetTitle("#theta [rad]");
	histotheta->GetYaxis()->SetTitle("counts");

	for (int i = 0; i < Nevents; i++)
	{
		if (HitA.at(i) == 1)		
		{
			histotheta -> Fill (theta.at(i));
			histophi -> Fill (phi.at(i));
		}
			
	} 

	histotheta -> Draw();
	c1->Print ("theta.pdf", "pdf");

	histophi -> Draw();
	c1->Print ("phi.pdf", "pdf");

	std::cout << "\n\nEV TOTALI: " << Nevents << std::endl << std::endl;
	std::cout << "HitA: " << NumberOfOne(&HitA, Nevents) << std::endl;
	std::cout << "HitB: " << NumberOfOne(&HitB, Nevents) << std::endl;
	std::cout << "HitC: " << NumberOfOne(&HitC, Nevents) << std::endl;
	std::cout << "DetA: " << NumberOfOne(&DetA, Nevents) << std::endl;
	std::cout << "DetB: " << NumberOfOne(&DetB, Nevents) << std::endl;
	std::cout << "DetC: " << NumberOfOne(&DetC, Nevents) << std::endl;
	std::cout << "\n";
	std::cout << "1 rivelazione: " << NCoincidences (&DetA, &DetB, &DetC, Nevents, 1) << std::endl;
	std::cout << "2 rivelazioni: " << NCoincidences (&DetA, &DetB, &DetC, Nevents, 2) << std::endl;
	std::cout << "3 rivelazioni: " << NCoincidences (&DetA, &DetB, &DetC, Nevents, 3) << std::endl;
}
