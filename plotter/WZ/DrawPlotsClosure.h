#ifndef __var
#define __var

#include <vector>
#include <iostream>
#include "TString.h"

#include <string>
#include <TH1F.h>
#include <TStyle.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TROOT.h>


#include <cstdlib>
#include <vector>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
#include "TROOT.h"


TH1F* hTruth;
TH1F* hEst;
TH2D* hFRMu;
TH2D* hFREl;
TH2D* hFRMuUp;
TH2D* hFRElUp;
TH2D* hFRMuDn;
TH2D* hFRElDn;

class sample;
class var;

class sample{
	public:
		sample(){};
		sample(TString _samplename, TString _dir , TString _prefix , TString _suffix , TString _type, std::vector<sample> &sampleVector, std::vector<sample> &StrPlot, Color_t col, TString pltname, TString dCard){
			samplename = _samplename;
			dir = _dir;
			prefix = _prefix;
			suffix = _suffix;
			type = _type;
			plotColor = col;
			plotName = pltname;
			dataCard = dCard;
			if (_type != TString("justplot")){
				sampleVector.push_back(*this);
			}
			if (_type != TString("train")){					
				StrPlot.push_back(*this);
			}
		}
		~sample(){};

		//Get Methods
		TString getSamplename(){return samplename;};
		TString getPath(){return dir;};
		TString getType(){return type;};
		Color_t getColor(){return plotColor;};
		TString path(){return dir + prefix + samplename + suffix;};
		TString saveFile(){return prefix + samplename;};
		TString filename(){return prefix + samplename + suffix;};
		TString getPlotName(){return plotName;};
		TString getdataCard(){return dataCard;};
	

	protected:
		TString samplename;
		TString type;
		TString dir;
		TString prefix;
		TString suffix;
		TString dataCard;
		Color_t plotColor;
		TString plotName;
};

class var{
	public:
		var(){};
		var(TString _te, TString _la, TString _un, char _ty, std::vector<var> &varVector){
			text = _te;
			label = _la;
			units = _un;
			type = _ty;
			varVector.push_back(*this);	
		}
 		~var(){};

		//Get Methods
		TString getVar(){return text;};
		TString getLabel(){return label;};
		TString getUnits(){return units;};
		char getType(){return type;};
		Float_t* getVariableAdress(){return &(Variable);};
		Float_t getVariable() {return Variable;};

		//Overloading >, <, ==
		bool operator>(Float_t cut){
			return (this->getVariable()) > cut;}

		bool operator<(Float_t cut){
			return (this->getVariable()) < cut;}
				
		bool operator==(Float_t cut){
			return (this->getVariable()) == cut;}

		//Set Methods
		void setVar(TString thevar){ text = thevar;};
		void setLab(TString thelab){ label = thelab;};
		void setUnits(TString theunit){ units = theunit;};
		void setType(char thetype){ type = thetype;};
		
	protected:
		TString text;
		TString label;
		TString units;
		char type;
		Float_t Variable;
};

std::vector<sample> theSamples; 		 // samples
std::vector<var> theVars;    			 // variables
std::vector<sample> PlottingSamples; // plotting, all but training samples

void bdtOptions();

float temp = 0;
#endif
