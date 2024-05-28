#include "nussinov.h"

int main() {
    //string r = "GGAAAUCC";
    string r = "ACUCGAUUCCGAG";
    //vector<pair<char, char>> pairs = {{'G', 'C'}, {'A', 'U'}};
    //vector<int> scores = {-1, -1};
    vector<pair<char, char>> pairs = {{'G', 'C'}, {'A', 'U'}, {'G', 'U'}};
    vector<int> scores = {-5, -4, -1};

    Nussinov nus(r, pairs, scores);
    nus.Nussinov_Algorithm();
    nus.Print_Mat();
    nus.Find_Optimal_Alignment(&nus.mat[0][nus.n - 1], 0, nus.n - 1);
    cout << nus.solution;

    return 0;
}
