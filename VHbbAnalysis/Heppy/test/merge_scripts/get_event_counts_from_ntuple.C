#include <array>
#include <fstream>
#include <cctype>

using std::array;

void get_counts_from_file(array<int, 4>& count, string fn, bool eos = true)
{
    if(eos) fn = "root://cmseos.fnal.gov/" + fn;
    TFile *f  = TFile::Open(fn.c_str());
    TH1F  *h  = (TH1F*) f->Get("Count");
    TH1F  *hw = (TH1F*) f->Get("CountWeighted");
    TH1F  *hp = (TH1F*) f->Get("CountPosWeight");
    TH1F  *hn = (TH1F*) f->Get("CountNegWeight");
    count[0] = h ->GetBinContent(1);
    count[1] = hw->GetBinContent(1);
    count[2] = hp->GetBinContent(1);
    count[3] = hn->GetBinContent(1);
    delete h; delete hw; delete hp; delete hn;
    f->Close();
    delete f;
}

void get_event_counts_from_ntuple()
{
    // Open file
    ifstream filelist("tmp.txt");
    ofstream f_output("file_counts.txt", std::ofstream::out | std::ofstream::trunc);
    if(!filelist.is_open()) {cout << "ERROR: Unable to open file list tmp.txt. Run the python script to use these functions." << endl; return;}
    if(!f_output.is_open()) {cout << "ERROR: Unable to open output file." << endl; return;}

    // Read counts from files.
    string fn;
    int nFiles  = 0;
    int nCounts = 0;
    array<int,4> count;
    cout     << "Count\tCountWeighted\tCountPosWeight\tCountNegWeight\tFileName" << endl;
    f_output << "Count\tCountWeighted\tCountPosWeight\tCountNegWeight\tFileName\n";
    while(getline(filelist, fn))
    { // While there are still listings in this list... 
      // Check if the listing is a root file.
        string last_five_of_fn = fn.substr(fn.length()-5, 5);
	if(last_five_of_fn != ".root") continue;
      // Print counts and filename
        get_counts_from_file(count, fn);
	cout     << count[0] << "\t" << count[1] << "\t" << count[2] << "\t" << count[3] << "\t" << fn << endl;
	f_output << count[0] << "\t" << count[1] << "\t" << count[2] << "\t" << count[3] << "\t" << fn << "\n";
        nFiles++;
	nCounts += count[0];
    }
    cout << "Files: " << nFiles << "\t Total Counts: " << nCounts << endl;
    cout << "Type \".q\" to exit." << endl;
}
