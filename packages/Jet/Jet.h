#include <iostream>

class Jet{

	public:
		Jet(){};
		Jet(TLorentzVector vec, Float_t btag_csv, Int_t Id = -1, Int_t mcFlavour = 0, ){
			p = vec;
			csv = btag_csv;
			id = Id;
			flavmc = mcFlavour;
		}
		~Jet(){};


		Bool_t isBtag;
		TLorentzVector p;
		Bool_t id;
		Float_t flavmc;
		Float_t csv; 

		Bool_t  GetBtag();
		Float_t GetBtagSF(Int_t inom);

	protected:
		Float_t SF;

};
