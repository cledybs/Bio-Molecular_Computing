/*#include "neddleman.h"
#include <chrono>

int main() {
    //SINGLE ALIGNMENT PROJECT
    string s, t;
    Input_Strings(s, t);

    Neddleman nd(s, t);

    auto start = chrono::high_resolution_clock::now();
    nd.Neddleman_Wunch_Algorithm();
    //dot_file.close();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_nw = end - start;

    //cout << "--- MATRIX ---" << endl;
    //Print_Mat(mat);

    start = chrono::high_resolution_clock::now();
    nd.Find_Optimal_Alignment(&nd.mat[nd.n - 1][nd.m - 1], nd.n - 1, nd.m - 1);
    //coords_file.close();
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_single = end - start;


    chrono::duration<double> elapsed_all = chrono::seconds(0);
    start = chrono::high_resolution_clock::now();
    nd.All_Alignments(&nd.mat[nd.n - 1][nd.m - 1], nd.n - 1, nd.m - 1);
    end = chrono::high_resolution_clock::now();
    elapsed_all = end - start;


    s.erase(0, 1);
    t.erase(0, 1);

    align_file << "\n\n" << NUC1 << "(" << to_string(nd.n-1) << ") VS " << NUC2 << "(" << to_string(nd.m-1) << ")\n\nScore " << to_string(nd.mat[nd.n - 1][nd.m - 1].value) << "\n\n";
    align_file << "Time Neddleman_Wunch_Algorithm " << to_string(elapsed_nw.count())
               << "\nTime Find_Optimal_Alignment " << to_string(elapsed_single.count())
               << "\nTime All_Alignments " << to_string(elapsed_all.count()) << "\n\n"
               << s << "\n" << t;

    align_file.close();
}*/