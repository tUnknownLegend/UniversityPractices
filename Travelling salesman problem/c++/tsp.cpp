#include <random>
#include "bits/stdc++.h"
#include "shared.h"

using namespace std;

//This function generates a random number in [i, j]
double getRandomNumber(double i, double j) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(i, j);
    return double(distribution(generator));
}

class TSP {
public:
    TSP() = delete;

    explicit TSP(const std::string &fileName) {
        ifstream in_file(fileName);
        if (!in_file)
            cerr << "error // input.txt open" << endl;

        in_file >> numOfCities;

        //cout << "Start reading\n";
        {
            vector<double> str;
            double node = 0.0;
            for (int i = 0; i < numOfCities; ++i) {

                for (int j = 0; j < numOfCities; ++j) {
                    in_file >> node;
                    str.push_back(node);
                }
                graph.push_back(str);
                str.clear();
            }
        }
        //cout << "End reading\n";

        arr.reserve(numOfCities);
    }

    vector<vector<double> > graph;
    double minimalWeight = DBL_MAX;

    vector<int> cities{};
    vector<int> finalOrder{};
    // arr is the array that stores the City order
    vector<int> arr{};
    int numOfCities = 0;

    double getDistance(int city1, int city2) {
        return graph[city1][city2];
    }

//This function returns the tour length of the current order of cities
    double getTourLength() {
        double tourLength = getDistance(0, cities.front());
        for (auto it = cities.begin(); it + 1 != cities.end(); it++)
            tourLength += getDistance(*(it), *(it + 1));

        tourLength += getDistance(cities.back(), 0);
        return tourLength;
    }

//puts the nearestNeighbourTour in the vector cities
    double getNearestNeighbourTour() {
        cities.clear();
        finalOrder.clear();
        for (int i = 1; i < numOfCities; i++) {
            cities.push_back(i);
            finalOrder.push_back(i);
        }

        double nearestNeighbourTourLength = getTourLength();
        return nearestNeighbourTourLength;
    }

    void swapVec(int i, int j) {
        swap(*(cities.begin() + i), *(cities.begin() + j));
    }

    //This function finds the probability of how bad the new solution is
    double getProbability(double difference, double temperature) {
        return exp(-1 * difference / temperature);
    }

    void calc() {
        //Generate the initial city tour and get the  nearestNeighbourTourLength
        double bestTourLength = getNearestNeighbourTour(); //start with a decent point

        if (minimalWeight > bestTourLength)
            minimalWeight = bestTourLength;

        double temperature = 0.9;
        const double coolingRate = 0.9;
        const double absoluteTemperature = 0.00001;
        int position1 = 0;
        int position2 = 0;
        double newTourLength = 0;
        double difference = 0;

        for (int cycleIt = 0; cycleIt < CYCLE_RATE; cycleIt++) {
            temperature = DBL_MAX; //Initial Temperature

            while (temperature > absoluteTemperature) {
                position1 = int(getRandomNumber(0, numOfCities - 1));
                position2 = int(getRandomNumber(0, numOfCities - 1));

                while (position1 == position2 || ((position1 > numOfCities - 2) || (position2 > numOfCities - 2))) {
                    position1 = int(getRandomNumber(0, numOfCities - 2));
                    position2 = int(getRandomNumber(0, numOfCities - 2));

                }
                swapVec(position1, position2);

                newTourLength = getTourLength();

                if (minimalWeight > newTourLength)
                    minimalWeight = newTourLength;

                difference = newTourLength - bestTourLength;

                if (difference < 0 ||
                    (difference > 0 &&
                     getProbability(difference, temperature) > COMPARE_RATE)) { // getRandomNumber(0, 1)
                    finalOrder.clear();

                    copy(cities.begin(), cities.end(), back_inserter(finalOrder));

                    bestTourLength = newTourLength;
                }
                temperature = temperature * coolingRate;
            }


             // debug info
            ofstream outFile(outFileQuick);
            if (!outFile) {
                cerr << "error // outFileNonEff open\n";
                return;
            }

            for (int &i: finalOrder) {
                outFile << i << " ";
            }
            outFile << "       " << bestTourLength << "        " << minimalWeight;
            outFile << endl;
            shuffle(cities.begin(), cities.end(), std::mt19937(std::random_device()()));

        }

    }

    void printWeight() const {
        cout << "Weight Value: " << minimalWeight << "\n";
    }

};

void calcTSP() {
    //start time
    TSP cTSP(inFileQuick);
    unsigned int start_time = clock();
    cTSP.calc();

    unsigned int end_time = clock();

    cTSP.printWeight();

    unsigned int search_time = end_time - start_time; //  exec time
    cout << "Search time: " << ((double) search_time) / CLOCKS_PER_SEC << endl;
}

int main() {
    calcTSP();

    return 0;
}
