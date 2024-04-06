#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

#define PATHS_LIMITED true
#define LIM_PATHS 1
#define NUCS_LIMITED false
#define LIM_NUC1 200
#define LIM_NUC2 200

#define NUC1 "FORWARD_STRINGS"
#define NUC2 "REVERSE_STRINGS"

string singleA, singleB;
unsigned int count_paths = 0;

ofstream dot_file("MULTIPLE_dotmat.txt");
ofstream coords_file("MULTIPLE_coords.txt");
ofstream align_file("MULTIPLE_alignments.txt");

struct Node {
    Node(int dato) : value(dato) {
        path.push_back(nullptr);
        path.push_back(nullptr);
        path.push_back(nullptr);
    }
    int value;
    vector<Node *> path;
};

void Print_Mat(const vector<vector<Node>> &mat) {
    int n = mat.size();
    int m = mat[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << mat[i][j].value << "\t";
        }
        cout << endl;
    }
}

void All_Alignments(Node* node, string &s, string &t, int it, int jt) {
    int num = 0;
    string tmpA = singleA;
    string tmpB = singleB;

    if(count_paths == LIM_PATHS && PATHS_LIMITED) return;

    if(it == 0 && jt == 0) {
        reverse(singleA.begin(), singleA.end());
        reverse(singleB.begin(), singleB.end());
        align_file << singleA << "\n" << singleB << "\n\n";
        count_paths++;
    }

    if(node->path[0] != nullptr){
        singleA += s[it];
        singleB += t[jt];
        All_Alignments(node->path[0], s, t, it - 1, jt - 1);
        num++;
    }
    if(node->path[1] != nullptr){
        if(num) { singleA = tmpA; singleB = tmpB; }
        singleA += s[it];
        singleB += '-';
        All_Alignments(node->path[1], s, t, it - 1, jt);
        num++;
    }
    if(node->path[2] != nullptr){
        if(num) { singleA = tmpA; singleB = tmpB; }
        singleA += '-';
        singleB += t[jt];
        All_Alignments(node->path[2], s, t, it, jt - 1);
    }

}

void Find_Optimal_Alignment(Node* node, string &s, string &t, int it, int jt) {
    coords_file << to_string(it) << " " << to_string(jt) << "\n";

    if(node->path[0] != nullptr) Find_Optimal_Alignment(node->path[0], s, t, it - 1, jt - 1);
    else if(node->path[1] != nullptr) Find_Optimal_Alignment(node->path[1], s, t, it - 1, jt);
    else if(node->path[2] != nullptr) Find_Optimal_Alignment(node->path[2], s, t, it, jt - 1);
}

void Neddleman_Wunch(string &s, string &t, const int n, const int m, vector<vector<Node>> &mat)    {
    s = "-" + s;
    t = "-" + t;

    for (int j = 0, val = 0; j < m; ++j, val -= 2) {
        mat[0][j].value = val;
        if(j) mat[0][j].path[2] = &mat[0][j - 1];
    }

    for (int i = 0, val = 0; i < n; ++i, val -= 2) {
        mat[i][0].value = val;
        if(i) mat[i][0].path[1] = &mat[i - 1][0];
    }

    string point_row;

    for (int i = 1; i < n; ++i) {
        //point_row = "";
        for (int j = 1; j < m; ++j) {
            int score = (s[i] == t[j]) ? 1 : -1;

            if (score == 1) point_row += ".";
            else point_row += "-";

            mat[i][j].value = max(mat[i - 1][j - 1].value + score,
                                  max(mat[i - 1][j].value - 2, mat[i][j - 1].value - 2));

            if (mat[i - 1][j - 1].value + score == mat[i][j].value)
                mat[i][j].path[0] = &mat[i - 1][j - 1];
            if (mat[i - 1][j].value - 2 == mat[i][j].value)
                mat[i][j].path[1] = &mat[i - 1][j];
            if (mat[i][j - 1].value - 2 == mat[i][j].value)
                mat[i][j].path[2] = &mat[i][j - 1];
        }
        //point_row += '\n';
        //dot_file << point_row;
    }
}

void Input_Strings(string &s, string &t) {
    string txt = ".txt";
    ifstream nuc1(NUC1 + txt);
    ifstream nuc2(NUC2 + txt);

    if(NUCS_LIMITED) {
        char caracter;

        int ctr = 0;
        while (nuc1.get(caracter) && ctr < LIM_NUC1) {
            s += caracter;
            ctr++;
        }

        ctr = 0;
        while (nuc2.get(caracter) && ctr < LIM_NUC2) {
            t += caracter;
            ctr++;
        }
    }
    else {
        getline(nuc1, s);
        getline(nuc2, t);
    }

    nuc1.close();
    nuc2.close();
}

void Input_Multiple_Strings(string &s, string &t) {
    string txt = ".txt";
    ifstream nuc1(NUC1 + txt);
    ifstream nuc2(NUC2 + txt);

    if(NUCS_LIMITED) {
        char caracter;

        int ctr = 0;
        while (nuc1.get(caracter) && ctr < LIM_NUC1) {
            s += caracter;
            ctr++;
        }

        ctr = 0;
        while (nuc2.get(caracter) && ctr < LIM_NUC2) {
            t += caracter;
            ctr++;
        }
    }
    else {
        getline(nuc1, s);
        getline(nuc2, t);
    }

    nuc1.close();
    nuc2.close();
}

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


    /* //SINGLE ALIGNMENT PROJECT
    string s, t;
    Input_Strings(s, t);

    int n = s.length() + 1;
    int m = t.length() + 1;
    vector<vector<Node>> mat(n, vector<Node>(m, Node(0)));

    auto start = chrono::high_resolution_clock::now();
    Neddleman_Wunch(s, t, n, m, mat);
    dot_file.close();
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
    */

    return 0;
}

