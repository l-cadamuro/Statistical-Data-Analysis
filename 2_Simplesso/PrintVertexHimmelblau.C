// macro ROOT per disegnare le iterazioni del simplesso sulla funzione

#include <vector>

void PrintVertexHimmelblau()
{
	// open file

	std::ifstream data ("Vertex_himmelblau.txt", std::ios::in);
	
	double buf;
	std::vector<double> x1;
	std::vector<double> x2;
	std::vector<double> x3;
	std::vector<double> y1;
	std::vector<double> y2;
	std::vector<double> y3;

	// loop on file
	while (1)
	{
		data >> buf;
		if (data.eof())
		{break;}
		x1.push_back(buf);
		data >> buf;
		y1.push_back(buf);
		data >> buf;
		x2.push_back(buf);
		data >> buf;
		y2.push_back(buf);
		data >> buf;
		x3.push_back(buf);
		data >> buf;
		y3.push_back(buf);	
	}

	// functions
	TF2* f = new TF2 ("himmelblau", "(x*x + y - 11)*(x*x + y - 11) + (x + y*y - 7)*(x + y*y - 7)", -6, 6, -6, 6); // himmelblau
	gStyle->SetOptTitle(0);	
	f->SetNpx(1000);
	f->SetNpy(1000);
	f->GetXaxis()->SetTitle("x");
	f->GetYaxis()->SetTitle("y");
	TCanvas* c1 = new TCanvas;
	c1->SetLogz();	
	himmelblau->Draw("COLZ");

	
	// loop on vertex
	std::vector<TLine*> vl1;
	std::vector<TLine*> vl2;
	std::vector<TLine*> vl3;
	TLine* l1;
	TLine* l2;
	TLine* l3;	
	for (int i = 0; i < x1.size(); i++)
	{
		l1 = new TLine(0,0,0,0);
		l2 = new TLine(0,0,0,0);
		l3 = new TLine(0,0,0,0);	
		
		l1->SetLineWidth(1.5);
		l2->SetLineWidth(1.5);
		l3->SetLineWidth(1.5);

		l1->SetLineColor(kBlue -10 + i);
		l2->SetLineColor(kBlue -10 + i);
		l3->SetLineColor(kBlue -10 + i);

		l1->SetX1(x1.at(i));
		l1->SetY1(y1.at(i));
		l1->SetX2(x2.at(i));
		l1->SetY2(y2.at(i));

		l2->SetX1(x2.at(i));
		l2->SetY1(y2.at(i));
		l2->SetX2(x3.at(i));
		l2->SetY2(y3.at(i));

		l3->SetX1(x3.at(i));
		l3->SetY1(y3.at(i));
		l3->SetX2(x1.at(i));
		l3->SetY2(y1.at(i));

		l1->Draw("same");
		l2->Draw("same");
		l3->Draw("same");
	}

	double xmin[4] = {3, -2.805118, -3.779310, 3.584428};
	double ymin[4] = {2, 3.131312, -3.283186, -1.848126};

	TMarker* min[4];
	for (int i = 0; i < 4; i++)
	{
		min[i] = new TMarker (xmin[i], ymin[i], i);
		min[i] -> SetMarkerColor (kBlue);
		min[i]->SetMarkerStyle(20);
		min[i]->SetMarkerSize (1.0);
		min[i]->Draw();
	}	

}
