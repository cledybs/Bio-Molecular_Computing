#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

int main() {
    int nstr = 6;
    vector<string> s(nstr), t(nstr);
    vector<vector<int>> scores;

    // MULTIPLE ALIGNMENT PROJECT
    for(int j = 0; j < nstr; ++j) {
        for (int i = 0; i < nstr; ++i) {
            Input_Multiple_Strings(s[i], t[i]);
            int n = s[i].length() + 1;
            int m = t[i].length() + 1;
            vector<vector<Node>> mat(n, vector<Node>(m, Node(0)));
            Neddleman_Wunch(s[i], t[i], n, m, mat);
            scores[j].push_back(mat[n - 1][m - 1].value);
        }
    }

    for(int j = 0; j < nstr; ++j) {
        for (int i = 0; i < nstr; ++i) {
            cout << scores[i][j];
        }
    }
    return 0;
}
