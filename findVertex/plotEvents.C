#include "TFile.h"
#include "TH1.h"
#include "THStack.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "TVector3.h"

/*
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
*/

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
	int evIndex = 50; // REPLACE WITH YOUR FAVOURITE NUMBER (only between 1&100 doh)
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

	stringstream buffer;
	while (getline(inFile, line)){
		iss << line;
		isFirst = true;
		while (getline(iss, item, ',')){
			buffer << item;
			if (isFirst){
				buffer >> type;
				isFirst = false;
				isIndex = true;
				d = 0;
			}
			else{
				switch (type){
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
}

