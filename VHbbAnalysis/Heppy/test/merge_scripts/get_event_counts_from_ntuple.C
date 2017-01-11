#include <fstream>
#include <cctype>

int get_counts_from_file(string fn, bool eos = true)
{
    if(eos) fn = "root://cmseos.fnal.gov/" + fn;
    TFile *f = TFile::Open(fn.c_str());
    TH1F  *h = (TH1F*) f->Get("Count");
    int n = h->GetBinContent(1);
    delete h;
    f->Close();
    delete f;
    return n;
}

void get_event_counts_from_ntuple()
{
    // Open file
    ifstream filelist("tmp.txt");
    if(!filelist.is_open()) {cout << "ERROR: Unable to open file list tmp.txt. Run the python script to use these functions." << endl; return;}

    // Read counts from files.
    string fn;
    while(getline(filelist, fn))
    { // While there are still listings in this list... 
      // Check if the listing is a root file.
        string last_five_of_fn = fn.substr(fn.length()-5, 5);
	if(last_five_of_fn != ".root") continue;
      // Print counts and filename
	cout << get_counts_from_file(fn) << "\t" << fn << endl;
    }
}
