#include "neddleman.h"
#include <chrono>

int main() {
    //SINGLE ALIGNMENT PROJECT
    string s, t;
    Input_Strings(s, t);

    int n = s.length() + 1;
    int m = t.length() + 1;
    vector<vector<Node>> mat(n, vector<Node>(m, Node(0)));

    auto start = chrono::high_resolution_clock::now();
    Neddleman_Wunch(s, t, n, m, mat);
    //dot_file.close();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_nw = end - start;

    //cout << "--- MATRIX ---" << endl;
    //Print_Mat(mat);


    start = chrono::high_resolution_clock::now();
    Find_Optimal_Alignment(&mat[n - 1][m - 1], s, t, n - 1, m - 1);
    coords_file.close();
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_single = end - start;


    chrono::duration<double> elapsed_all = chrono::seconds(0);
    start = chrono::high_resolution_clock::now();
    All_Alignments(&mat[n - 1][m - 1], s, t, n - 1, m - 1);
    end = chrono::high_resolution_clock::now();
    elapsed_all = end - start;


    s.erase(0, 1);
    t.erase(0, 1);

    align_file << "\n\n" << NUC1 << "(" << to_string(n-1) << ") VS " << NUC2 << "(" << to_string(m-1) << ")\n\nScore " << to_string(mat[n - 1][m - 1].value) << "\n\n";
    align_file << "Time Neddleman_Wunch " << to_string(elapsed_nw.count())
               << "\nTime Find_Optimal_Alignment " << to_string(elapsed_single.count())
               << "\nTime All_Alignments " << to_string(elapsed_all.count()) << "\n\n"
               << s << "\n" << t;

    align_file.close();
}