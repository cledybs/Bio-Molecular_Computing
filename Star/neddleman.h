#ifndef STAR_NEDDLEMAN_H
#define STAR_NEDDLEMAN_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

#define PATHS_LIMITED true
#define LIM_PATHS 1
#define NUCS_LIMITED false
#define LIM_NUC1 200
#define LIM_NUC2 200

#define NUC1 "FORWARD_STRINGS"
#define NUC2 "REVERSE_STRINGS"

//ofstream dot_file("MULTIPLE_dotmat.txt");
//ofstream coords_file("MULTIPLE_coords.txt");
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

class Neddleman {
    Neddleman(string s, string t);
    ~Neddleman();
    void Print_Mat();
    void All_Alignments(Node* node, int it, int jt);
    void Find_Optimal_Alignment(Node* node, int it, int jt);
    void Neddleman_Wunch(string &s, string &t);

    string singleA, singleB;
    unsigned int count_paths = 0;
    vector<vector<Node>> mat;
    string s, t;
    int n, m;
};

Neddleman::Neddleman(string s, string t) {
    this->s = s;
    this->t = t;
    n = s.length() + 1;
    m = t.length() + 1;
    mat.resize(n, vector<Node>(m, Node(0)));
}

void Neddleman::Print_Mat() {
    int n = mat.size();
    int m = mat[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << mat[i][j].value << "\t";
        }
        cout << endl;
    }
}

void Neddleman::All_Alignments(Node* node, int it, int jt) {
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
        All_Alignments(node->path[0], it - 1, jt - 1);
        num++;
    }
    if(node->path[1] != nullptr){
        if(num) { singleA = tmpA; singleB = tmpB; }
        singleA += s[it];
        singleB += '-';
        All_Alignments(node->path[1], it - 1, jt);
        num++;
    }
    if(node->path[2] != nullptr){
        if(num) { singleA = tmpA; singleB = tmpB; }
        singleA += '-';
        singleB += t[jt];
        All_Alignments(node->path[2], it, jt - 1);
    }

}

void Neddleman::Find_Optimal_Alignment(Node* node, int it, int jt) {
    //coords_file << to_string(it) << " " << to_string(jt) << "\n";

    if(node->path[0] != nullptr) Find_Optimal_Alignment(node->path[0], it - 1, jt - 1);
    else if(node->path[1] != nullptr) Find_Optimal_Alignment(node->path[1], it - 1, jt);
    else if(node->path[2] != nullptr) Find_Optimal_Alignment(node->path[2], it, jt - 1);
}

void Neddleman::Neddleman_Wunch(string &s, string &t)    {
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

#endif //STAR_NEDDLEMAN_H
