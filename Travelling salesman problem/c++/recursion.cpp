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

void GetRoads(int verticesIndex, const int &amtOfVertices, vector<bool> &usedPath, vector<int> &pathes,
              vector<vector<int>> &allPathes) {
    if (verticesIndex == amtOfVertices - 1) {
        allPathes.push_back(pathes);
        return;
    }

    for (int i = 1; i <= amtOfVertices - 1; i++) {
        if (usedPath[i]) {
            continue;
        }
        pathes[verticesIndex] = i;
        usedPath[i] = true;
        GetRoads(verticesIndex + 1, amtOfVertices, usedPath, pathes, allPathes);
        usedPath[i] = false;
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

    vector<int> pathes(amtOfVertices - 1, 0);
    vector<bool> usedPath = vector<bool>(amtOfVertices, false);
    vector<vector<int>> allPathes = {};

    unsigned int startingTime = clock();

    GetRoads(0, amtOfVertices, usedPath, pathes, allPathes);

    double minWeight = std::numeric_limits<double>::max();
    vector<int> minPath(amtOfVertices - 1, 0);

    for (auto path: allPathes) {
        double weight = matrix[0][path[0]];
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

    ofstream outFile(outFileNonEff);

    if (!outFile) {
        cerr << "error // outFileNonEff open\n";
        return 0;
    }

    for (int &i: minPath)
        outFile << ++i << " ";

    cout << "Weight Value: " << minWeight << "\n";
    cout << "minimal path: ";
    for (int i: minPath)
        cout << i << " ";

    unsigned int stopTime = clock();
    unsigned int searchTime = stopTime - startingTime;   //  exec time
    cout << "\nSearch time: " << ((float) searchTime) / CLOCKS_PER_SEC << "\n";

    return 0;
}