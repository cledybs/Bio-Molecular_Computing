//
// Created by cledy on 5/22/2024.
//

#ifndef NUSSINOV_NUSSINOV_H
#define NUSSINOV_NUSSINOV_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

struct Node {
    Node(int dato = 0) : value(dato) {
        path.push_back(nullptr); // [0] diagonal
        path.push_back(nullptr); // [1] down
        path.push_back(nullptr); // [2] left
    }
    int value;
    vector<Node *> path;
};

class Nussinov {
public:
    Nussinov(string r, vector<pair<char, char>> pairs, vector<int> scores);
    ~Nussinov();
    void Print_Mat();
    void Find_Optimal_Alignment(Node* node, int it, int jt);
    int Alpha(char a, char b);
    void Nussinov_Algorithm();

    Node **mat;
    string r;
    int n;
    vector<pair<char, char>> pairs;
    vector<int> scores;
    string solution;
    bool isSolved;
};

Nussinov::Nussinov(string r, vector<pair<char, char>> pairs, vector<int> scores) {
    this->r = r;
    this->pairs = pairs;
    this->scores = scores;
    n = r.length();
    solution = "";
    isSolved = false;
    mat = new Node*[n];
    for (int i = 0; i < n; ++i)
        mat[i] = new Node[n];
}

Nussinov::~Nussinov() {
    for (int i = 0; i < n; ++i)
        delete[] mat[i];
    delete[] mat;
}

void Nussinov::Print_Mat() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << mat[i][j].value << "\t";
        }
        cout << endl;
    }
}

void Nussinov::Find_Optimal_Alignment(Node* node, int it, int jt) {
    solution += r.substr(it, 1) + "_____" + r.substr(jt, 1) + "\n";
    solution += "|     |\n";

    if(node->path[0] != nullptr) Find_Optimal_Alignment(node->path[0], it + 1, jt - 1);
    else if(node->path[1] != nullptr) Find_Optimal_Alignment(node->path[1], it + 1, jt);
    else if(node->path[2] != nullptr) Find_Optimal_Alignment(node->path[2], it, jt - 1);

    if (!isSolved) {
        solution[solution.length() - 16] = ' ';
        solution[solution.length() - 15] = '\\';
        solution[solution.length() - 11] = '/';
        solution[solution.length() - 10] = ' ';
        solution[solution.length() - 8] = ' ';
        solution[solution.length() - 2] = ' ';
    }
    isSolved = true;

}

int Nussinov::Alpha(char a, char b) {
    for(int i = 0; i < pairs.size(); ++i)
        if(a == pairs[i].first && b == pairs[i].second || a == pairs[i].second && b == pairs[i].first) return scores[i];
    return 0;
}

void Nussinov::Nussinov_Algorithm()    {
    for(int t = 1; t < n; ++t) {
        for(int i = 0, j = t; j < n; ++i, ++j) {
            int score = Alpha(r[i], r[j]);

            mat[i][j].value = min(mat[i + 1][j - 1].value + score,
                                  min(mat[i + 1][j].value, mat[i][j - 1].value));
            /**/
            int k_winner = INT_MAX;
            int jk, ik;
            for(int k = i + 1; k < j; ++k) {
                if (mat[i][k].value + mat[k + 1][j].value < k_winner) {
                    k_winner = mat[i][k].value + mat[k + 1][j].value;
                    jk = k;
                    ik = k + 1;
                }
            }

            mat[i][j].value = min(k_winner, mat[i][j].value);

            if (mat[i][j].value == k_winner) { // this is wrong! can't point to a non-neighbour cell
                mat[i][j].path[0] = &mat[ik][j];
                mat[i][j].path[1] = &mat[i][jk];
            }


            if (mat[i + 1][j - 1].value + score == mat[i][j].value)
                mat[i][j].path[0] = &mat[i + 1][j - 1];
            if (mat[i][j - 1].value == mat[i][j].value)
                mat[i][j].path[2] = &mat[i][j - 1];
            if (mat[i + 1][j].value == mat[i][j].value)
                mat[i][j].path[1] = &mat[i + 1][j];
        }
    }
}

#endif //NUSSINOV_NUSSINOV_H
