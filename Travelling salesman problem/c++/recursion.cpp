#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>
#include "shared.h"

using std::string;
using std::ifstream;
using std::vector;
using std::cerr;
using std::ofstream;
using std::cout;

void Record(int verticesIndex, const int &amtOfVertices, vector<bool> &used, vector<int>& p, vector<vector<int>>& a) {
    if (verticesIndex == amtOfVertices - 1) {
        a.push_back(p);
        return;
    }
    for (int i = 1; i <= amtOfVertices - 1; i++) {
        if (used[i]) {
            continue;
        }
        p[verticesIndex] = i;
        used[i] = true;
        Record(verticesIndex + 1, amtOfVertices, used, p, a);
        used[i] = false;
    }
}

int main() {
    ifstream in_file(inFileNonEff);
    if (!in_file.is_open()) {
        cerr << "error // input.txt open\n";
        return 0;
    }

    // matrix
    vector<vector<double>> matrix;

    int amtOfVertices = 0;
    // reading from file
    in_file >> amtOfVertices;
    //cout << "Start reading\n";
    
    {
        vector<double> str;
        double node = 0.0;
        for (int i = 0; i < amtOfVertices; i++) {

            for (int j = 0; j < amtOfVertices; j++) {
                in_file >> node;
                str.push_back(node);
            }
            matrix.push_back(str);
            str.clear();
        }
    }

    //cout << "End reading\n";
    vector<int> p(amtOfVertices - 1, 0);
    vector<bool> used = vector<bool>(amtOfVertices, false);
    vector<vector<int>> a = {};

    unsigned int startingTime = clock();

    Record(0, amtOfVertices, used, p, a);

    double minWeight = std::numeric_limits<double>::max();
    vector<int> minPath(amtOfVertices - 1, 0);

    for (auto path: a) {
        double weight = 0;
        weight += matrix[0][path[0]];
        for (int j = 0; j < path.size() - 1; j++) {
            weight += matrix[path[j]][path[j + 1]];
        }
        weight += matrix[path.back()][0];
        if (weight <= minWeight) {
            minWeight = weight;
            minPath = path;
        }
    }
    minPath.insert(minPath.begin(), 0);
    minPath.push_back(0);

    // formatting
    vector<int> formatOrder;
    formatOrder.reserve(minPath.size());
    for (int i: minPath)
        formatOrder.push_back(i + 1);

    ofstream outFile(outFileNonEff);

    if (!outFile) {
        cerr << "error // outFileNonEff open\n";
        exit(1);
    }

    for (int i: formatOrder)
        outFile << i << " ";

    cout << "Weight Value: " << minWeight << "\n";
//    cout << "minimal path: ";
//    for (int i: formatOrder)
//        cout << i << " ";

    unsigned int stopTime = clock();
    unsigned int searchTime = stopTime - startingTime;   //  exec time
    cout << "Search time: " << ((float) searchTime) / CLOCKS_PER_SEC << "\n";

    return 0;
}