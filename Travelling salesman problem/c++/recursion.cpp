#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <limits>
#include <algorithm>
#include "shared.h"

using std::ifstream;
using std::vector;
using std::cerr;
using std::ofstream;
using std::cout;

void inputMatrix(vector<vector<double>> &matrix) {
    ifstream in_file(inFileNonEff);
    if (!in_file.is_open()) {
        cerr << "error // input.txt open\n";
        return;
    }

    int amtOfVertices = 0;
    // reading from file
    in_file >> amtOfVertices;

    //cout << "Start reading\n";
    {
        vector<double> str;
        double node = 0.0;
        for (int i = 0; i < amtOfVertices; ++i) {

            for (int j = 0; j < amtOfVertices; ++j) {
                in_file >> node;
                str.push_back(node);
            }
            matrix.push_back(str);
            str.clear();
        }
    }
    //cout << "End reading\n";
}

double calcRec() {
    // matrix
    vector<vector<double>> matrix;
    inputMatrix(matrix);

    // creating sub matrix
    vector<unsigned int> vertexes = {};
    vertexes.reserve(matrix.size());
    for (int i = 1; i < matrix.size(); ++i) {
        vertexes.push_back(i);
    }

    double minWeight = std::numeric_limits<double>::max();

    do {
        double currWeight = 0.0;
        unsigned int currVertex = 0;
        for (auto &i: vertexes) {
            currWeight += matrix[currVertex][i];
            currVertex = i;
        }
        currWeight += matrix[currVertex][0];

        minWeight = std::min(currWeight, minWeight);

    } while (std::next_permutation(vertexes.begin(), vertexes.end()));

    return minWeight;
}

int main() {
    unsigned int startingTime = clock();
    cout << calcRec();
    unsigned int stopTime = clock();
    unsigned int searchTime = stopTime - startingTime;   //  exec time
    cout << "\nSearch time: " << ((double) searchTime) / CLOCKS_PER_SEC << "\n";

    return 0;
}