#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "neddleman.h"

#define NUM_NUC 6
#define FILENAME1 "FORWARD_STRINGS"
#define FILENAME2 "REVERSE_STRINGS"

using namespace std;

void Input_Multiple_Strings(vector<string> &s, vector<string> &t) {
    string txt = ".txt";
    ifstream nuc1(FILENAME1 + txt);
    ifstream nuc2(FILENAME2 + txt);

    for(int i = 0; i < NUM_NUC; ++i) {
        getline(nuc1, s[i]);
        getline(nuc2, t[i]);
    }

    nuc1.close();
    nuc2.close();
}

void Find_Alignments(vector<string> &m, int it_max, const int size_) {
    ifstream file("alignments.txt");

    string a;
    for(int i = 0; i < (size_ - 1) * it_max * 3; ++i)
        getline(file, a);

    for(int i = 0; i < size_ - 1; ++i) {
        getline(file, a);
        if(i == 0) m.push_back(a);
        getline(file, a);
        m.push_back(a);
        getline(file, a);
    }
}

void Print_Alignments(vector<string> &m) {
    for(int i = 0; i < m.size(); ++i)
        cout << m[i] << endl;
    cout << endl;
}

void Print_Scores(vector<vector<int>> &scores) {
    for(int i = 0; i < scores.size(); ++i) {
        for(int j = 0; j < scores.size(); ++j)
            cout << scores[i][j] << '\t';
        cout << endl;
    }
    cout << endl;
}

void Star_Algorithm(vector<string> &s) {
    int n = s.size();
    vector<vector<int>> scores(n, vector<int>(n, 0));

    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i == j) continue;
            Neddleman nd(s[i], s[j]);
            nd.Neddleman_Wunch_Algorithm();
            nd.All_Alignments(&nd.mat[nd.n - 1][nd.m - 1], nd.n - 1, nd.m - 1);
            scores[i][j] = nd.mat[nd.n - 1][nd.m - 1].value;
        }
    }

    vector<int> final_scores;
    int maximo = INT_MIN;
    int it_max = -1;
    for(int i = 0; i < n; ++i) {
        final_scores.push_back(accumulate(scores[i].begin(), scores[i].end(), 0));
        if(final_scores[i] > maximo) {
            it_max = i;
            maximo = final_scores[i];
        }
    }
    align_file.close();

    //cout << it_max << endl;
    cout << "# SCORE : " << final_scores[it_max] << endl;
    //Print_Scores(scores);


    vector<string> m;
    Find_Alignments(m, it_max, n);

    int max_size = 0;
    for(auto &v : m)
        if (v.length() > max_size) max_size = v.length();

    for(auto &v : m)
        if (v.size() < max_size) v.resize(max_size, '-');

    cout << "# ALIGNMENTS : " << endl;
    Print_Alignments(m);
}

void Invert_Bases(vector<string> &s) {
    for(auto &v : s) {
        reverse(v.begin(), v.end());
        for(auto &c : v) {
            switch(c) {
                case 'A':
                    c = 'T';
                    break;
                case 'T':
                    c = 'A';
                    break;
                case 'C':
                    c = 'G';
                    break;
                case 'G':
                    c = 'C';
                    break;
            }
        }
    }
}

int main() {
    vector<string> f(NUM_NUC), r(NUM_NUC);

    Input_Multiple_Strings(f, r);

    //cout << "\n# FORWARD" << endl;
    //Star_Algorithm(f);

    //cout << "\n# REVERSE" << endl;
    //Star_Algorithm(r);

    //cout << "\n# DIRECTION PROVE" << endl;
    //f.insert(f.end(), r.begin(), r.end());
    //Star_Algorithm(f);

    cout << "\n# MERGE" << endl;
    Invert_Bases(r);
    f.insert(f.end(), r.begin(), r.end());
    Star_Algorithm(f);

    return 0;
}