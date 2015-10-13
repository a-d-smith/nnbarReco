#include "TFile.h"
#include "TH2.h"
#include "THStack.h"
#include "TAxis.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "TVector3.h"

// This is the main function which runs when the macro is called
void plotEvents(){
        // Set up some variables
	ifstream inFile;
	string line, item;
	stringstream iss;

	char type;
	bool isFirst;
	bool isIndex;
	int ev, vert, track, shower, d;
	
	std::vector<std::vector<double>> vertPos;
	std::vector<std::vector<double>> trackPos;
	std::vector<std::vector<double>> showerPos;
	
	std::vector<double> blankVect;
	blankVect.push_back(0);
	blankVect.push_back(0);
	blankVect.push_back(0);

	// Set-up the .txt file for reading
	int evIndex = 50; // REPLACE WITH YOUR FAVOURITE NUMBER (only between 1&100 doh) then make it run over all files
	std::stringstream strstrm;
	strstrm << "dat/trackAndShowerData" << evIndex << ".txt";
	std::string fileName = strstrm.str();
	char filePath[1024];
	strcpy(filePath, fileName.c_str());
	inFile.open(filePath);
	if (!inFile.is_open()){
		std::cerr << "Error: Input file could not be opened" << std::endl;
		exit(1);
	}

	// Initiate the buffer
	stringstream buffer;
	while (getline(inFile, line)){ //Get each line out of the file
		iss << line;
		isFirst = true;
		while (getline(iss, item, ',')){ //Get each item separated by a comma
			buffer << item;
			if (isFirst){
				buffer >> type;
				isFirst = false;
				isIndex = true;
				d = 0;
			}
			else{
				switch (type){ //The first characters "type" denote events, e, vertices, v, tracks, t, and showers, s.
					case 'e':
						buffer >> ev;
						std::cout << "Event : " << ev << std::endl;
						break;
					case 'v':
						if (isIndex){
							buffer >> vert;
							std::cout << "Vertex : " << vert << std::endl;
							while(vertPos.size() <= vert){
								vertPos.push_back(blankVect);
							}
							isIndex = false;
						}
						else{
							buffer >> vertPos[vert][d];
							std::cout << "v : x" << d+1 << " = " << vertPos[vert][d] << std::endl;
							d++;
						}
						break;
					case 't':
						if (isIndex){
							buffer >> track;
							std::cout << "Track : " << track << std::endl;
							while(trackPos.size() <= track){
								trackPos.push_back(blankVect);
							}
							isIndex = false;
						}
						else{
							buffer >> trackPos[track][d];
							std::cout << "t : x" << d+1 << " = " << trackPos[track][d] << std::endl;
							d++;
						}
						break;
					case 's':
						if (isIndex){
							buffer >> shower;
							std::cout << "Shower : " << shower << std::endl;
							while (showerPos.size() <= shower){
								showerPos.push_back(blankVect);
							}
							isIndex = false;
						}
						else{
							buffer >> showerPos[shower][d];
							std::cout << "s : x" << d+1 << " = " << showerPos[shower][d] << std::endl;
							d++;
						}
						break;
					default:
						std::cerr << "Invalid line type" << std::endl;
						break;
				}
			}
			buffer.clear();
		}	
		iss.clear();
	}
	inFile.close();
	// Plotting the x,y,z positions of the vertices.
	const unsigned int totTrack  = trackPos.size();
	const unsigned int totShower = showerPos.size();
	const unsigned int totEv     = totTrack + totShower;
	Double_t x[totEv];
	Double_t y[totEv];
	Double_t z[totEv];
	double minX, minY, minZ, maxX, maxY, maxZ;	

	for (int i=0; i<totTrack; i++){
		x[i] = trackPos[i][0];
		y[i] = trackPos[i][1];
		z[i] = trackPos[i][2];

		if (x[i] < minX) {minX = x[i];}
		if (y[i] < minY) {minY = y[i];}
		if (z[i] < minZ) {minZ = z[i];}
		if (x[i] > maxX) {maxX = x[i];}
		if (y[i] > maxY) {maxY = y[i];}
		if (z[i] > maxZ) {maxZ = z[i];}
	}
	for (int j=0; j<totShower; j++){
		x[totTrack + j] = showerPos[j][0];
		y[totTrack + j] = showerPos[j][1];
		z[totTrack + j] = showerPos[j][2];

		if (x[totTrack + j] < minX) {minX = x[totTrack + j];}
		if (y[totTrack + j] < minY) {minY = y[totTrack + j];}
		if (z[totTrack + j] < minZ) {minZ = z[totTrack + j];}
		if (x[totTrack + j] > maxX) {maxX = x[totTrack + j];}
		if (y[totTrack + j] > maxY) {maxY = y[totTrack + j];}
		if (z[totTrack + j] > maxZ) {maxZ = z[totTrack + j];}
	}

	TMultiGraph *mg = new TMultiGraph();

	gr = new TGraph(totEv,x,y);
	gr->SetMarkerStyle(7);
	mg->Add(gr);

	double pi = 3.14159265358979323;
	double tol = 10;
	double xCirc[100], yCirc[100];
	double theta;
	for (int i=0; i<vertPos.size(); i++){
		for (int j=0; j<100; j++){
			theta = 2*pi*j/100;
			xCirc[j] = vertPos[i][0] + tol*cos(theta);
			yCirc[j] = vertPos[i][1] + tol*sin(theta);
		}
		gr1 = new TGraph(100,xCirc,yCirc);
		gr1->SetMarkerStyle(1);
		mg->Add(gr1);
	}

	mg->Draw("AP");
}	
























