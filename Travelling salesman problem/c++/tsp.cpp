#include <random>

#include "bits/stdc++.h"
#include "shared.h"

using namespace std;

//This function generates a random number between
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

        numOfCities = 0;
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
    vector<int> arr{};//(40);
    int numOfCities = 0;

    double getDistance(int city1, int city2) {
        return graph[city1][city2];
    }

//This function returns the tour length of the current order of cities
    double getTourLength() {
        auto it = cities.begin();

        it = cities.begin();
        int pcity1 = *it, ncity;

        double tourLength = getDistance(0, pcity1);
        for (it = cities.begin() + 1; it != cities.end(); it++) {
            ncity = *it;
            tourLength += getDistance(pcity1, ncity);

            pcity1 = ncity;
        }

        //adding the getDistance back to the source path
        tourLength += getDistance(pcity1, 0);
        return tourLength;
    }

//puts the nearestNeighbourTour in the vector cities
    double getNearestNeighbourTour() {
        //vector<int>::iterator it;
        int numCities = numOfCities;

        for (int i = 0; i < numCities; i++)
            arr[i] = i;

        double best;
        int bestIndex;
        for (int i = 1; i < numCities; i++) {
            best = DBL_MAX;
            for (int j = i; j < numCities; j++) {

                if (getDistance(i - 1, j) < best) {
                    best = getDistance(i, j);
                    bestIndex = j;
                }
            }
            swap(i, bestIndex);
        }
        cities.clear();

        for (int i = 1; i < numCities; i++) {
            cities.push_back(arr[i]);
            finalOrder.push_back(arr[i]);
        }

        double nearestNeighbourTourLength = getTourLength();
        return nearestNeighbourTourLength;
    }

    void swapVec(int i, int j) {
//    auto it = cities.begin();
//    int temp = *(it + i);
//    *(it + i) = *(it + j);
//    *(it + j) = temp;

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

        ofstream fs(outFileQuick);

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
                //it2 = cities.begin();
                newTourLength = getTourLength();

                if (minimalWeight > newTourLength)
                    minimalWeight = newTourLength;

                difference = newTourLength - bestTourLength;

                if (difference < 0 ||
                    (difference > 0 &&
                     getProbability(difference, temperature) > COMPARE_RATE)) { // getRandomNumber(0, 1)
                    finalOrder.clear();

                    copy(cities.begin(), cities.end(), back_inserter(finalOrder));

//                for (it = cities.begin(); it != cities.end(); it++) {
//                    finalOrder.push_back(*it);
//                }
                    bestTourLength = newTourLength;
                }
                temperature = temperature * coolingRate;
            }

            for (int &i: finalOrder) {
                fs << i << " ";
            }
            fs << "       " << bestTourLength << "        " << minimalWeight;
            fs << endl;
            shuffle(cities.begin(), cities.end(), std::mt19937(std::random_device()()));
        }

        //cout << "best solution is " << bestTourLength << "\n";
//        cout << "Weight Value: " << minimalWeight << "\n";
    }

    void printWeight() const {
        cout << "Weight Value: " << minimalWeight << "\n";
    }

};

void calcTSP() {
    //start time
    unsigned int start_time = clock();

    TSP cTSP(inFileQuick);
    cTSP.calc();

    unsigned int end_time = clock();

    cTSP.printWeight();

    unsigned int search_time = end_time - start_time; //  exec time
    cout << "Search time: " << ((float) search_time) / CLOCKS_PER_SEC << endl;
}

int main() {

    calcTSP();

    return 0;
}
