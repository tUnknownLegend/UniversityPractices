#include <random>
#include "bits/stdc++.h"
#include "shared.h"

using std::vector;
using std::cerr;

//  This function generates a random double in [i, j]
double GetRandomDouble(double i, double j) {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(i, j);
    return dis(gen);
}

//  This function generates a random int in [i, j]
unsigned int GetRandomInt(unsigned int i, unsigned int j) {
    std::random_device rd;  //  Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //  Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(i, j);
    return distrib(gen);
}

double GetDistance(const vector<vector<double>> &matrix, const unsigned int &v1, const unsigned int &v2) {
    return matrix[v1][v2];
}

double GetProbability(const double &diff, const double &T) {
    if (T == 0) {
        cerr << "T == 0 // GetProbability()";
        return DIVISTION_ERROR;
    }
    return exp(-diff / T);
}

double GetWeight(const vector<unsigned int> &vertexes, const vector<vector<double>> &matrix) {
    double sum = 0.0;
    for (auto i = vertexes.begin(); i + 1 != vertexes.end(); ++i)
        sum += GetDistance(matrix, *i, *(i + 1));

    sum += GetDistance(matrix, *(vertexes.end() - 1), *(vertexes.begin()));
    return sum;
}

// firstN must be less or equal to secondN
void FlipVec(vector<unsigned int> &vertexes, const unsigned int &firstN, const unsigned int &secondN) {
    for (unsigned int i = 0; i < ((secondN - firstN) + (secondN - firstN) % 2) / 2; ++i)
        std::swap(vertexes[firstN + i], vertexes[secondN - i]);
}

vector<unsigned int> GetVertexes(vector<unsigned int> vertexes) {
    unsigned int firstN = GetRandomInt(0, vertexes.size() - 1);
    unsigned int secondN = GetRandomInt(0, vertexes.size() - 1);

    while (firstN == secondN)
        secondN = GetRandomInt(0, vertexes.size() - 1);

    (firstN < secondN) ? FlipVec(vertexes, firstN, secondN) : FlipVec(vertexes, secondN, firstN);
    return vertexes;
}

void SwapWeight(vector<unsigned int> &vertexes, double &currWeight,
                vector<unsigned int> &nextVertexes, double &nextWeight) {
    vertexes = std::move(nextVertexes);
    currWeight = nextWeight;
}

bool IsTransitNeeded(const double &probability) {
    if (probability == 0 || probability == 1) {
        //cerr << "Error // IsTransitNeeded";
        return false;
    }
    double value = GetRandomDouble(0, 1);

    if (value <= probability)
        return true;

    return false;
}

double CalcSimAnneling() {
    vector<vector<double>> matrix;
    inputMatrix(matrix);

    // creating sub matrix
    vector<unsigned int> vertexes = {};
    vertexes.reserve(matrix.size());
    for (int i = 0; i < matrix.size(); ++i) {
        vertexes.push_back(i);
    }
    // shuffling vertexes
    std::shuffle(vertexes.begin(), vertexes.end(), std::mt19937(std::random_device()()));

    double currWeight = GetWeight(vertexes, matrix);
    auto temperature = TEMP_MAX;

    for (int i = 0; i < CYCLE_RATE && temperature > TEMP_MIN; ++i) {
        vector<unsigned int> nextVertexes = GetVertexes(vertexes);
        double nextWeight = GetWeight(nextVertexes, matrix);

        if (nextWeight < currWeight)
            SwapWeight(vertexes, currWeight, nextVertexes, nextWeight);
        else {
            double probability = GetProbability(nextWeight - currWeight, temperature);
            if (IsTransitNeeded(probability))
                SwapWeight(vertexes, currWeight, nextVertexes, nextWeight);
        }

        //  cooling down
        temperature *= COOLING_RATE / i;
        //temperature *= COOLING_RATE;
    }

    return currWeight;
}

double GetBestResult(const double &res1, double &res2) {
    res2 = CalcSimAnneling();
    return std::min(res1, res2);
}

double GetSimAnneling() {
    double res1 = CalcSimAnneling();;
    auto res2 = DBL_MAX;
    for (int i = 0; i < REPEAT_RATE; ++i)
        res1 = GetBestResult(res1, res2);

    return GetBestResult(res1, res2);
}

int main() {
    //MeasureFuncExecTime([]() { std::cout << CalcSimAnneling(); });
    MeasureFuncExecTime([]() { std::cout << GetSimAnneling(); });

    return 0;
}
