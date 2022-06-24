#include <random>

#include "bits/stdc++.h"
#include "shared.h"

double originalGraph[40][40];
using namespace std;

vector<int> cities;
vector<int> finalOrder;
// arr is the array that stores the City order
int arr[40];
int numOfCities;

double distance(int city1, int city2) {
    return originalGraph[city1][city2];
}

//This function generates a random number between
double getRandomNumber(double i, double j) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(i, j);
    return double(distribution(generator));
}

//This function returns the tour length of the current order of cities
double getTourLength() {
    auto it = cities.begin();

    it = cities.begin();
    int pcity1 = *it, ncity;

    double tourLength = distance(0, pcity1);
    for (it = cities.begin() + 1; it != cities.end(); it++) {
        ncity = *it;
        tourLength += distance(pcity1, ncity);

        pcity1 = ncity;
    }

    //adding the distance back to the source path
    tourLength += distance(pcity1, 0);
    return tourLength;
}

//puts the nearestNeighbourTour in the vector cities
double getNearestNeighbourTour()
{
    //vector<int>::iterator it;
    int numCities = numOfCities;

    for (int i = 0; i < numCities; i++)
        arr[i] = i;

    double best;
    int bestIndex;
    for (int i = 1; i < numCities; i++) {
        best = DBL_MAX;
        for (int j = i; j < numCities; j++) {

            if (distance(i - 1, j) < best) {
                best = distance(i, j);
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

void swap2(int i, int j) {
    auto it = cities.begin();
    int temp = *(it + i);
    *(it + i) = *(it + j);
    *(it + j) = temp;

}

//This function finds the probability of how bad the new solution is
double getProbability(double difference, double temperature) {
    return exp(-1 * difference / temperature);
}

double mini = DBL_MAX;

int main() {
    ifstream in_file(inFileQuick);
    if (!in_file) {
        cerr << "error // input.txt open" << endl;
        return 0;
    }

    //matrix
    vector<vector<double>> matrix;
    int n = 0;
    in_file >> n;

    for (int i = 0; i < n; i++) {
        vector<double> str;

        for (int j = 0; j < n; j++) {
            double node;
            in_file >> node;
            str.push_back(node);
        }
        matrix.push_back(str);
    }

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {
            originalGraph[i][j] = matrix[i][j];
        }
    }

    vector<int>::iterator it, it2;

    numOfCities = n;

    //Generate the initial city tour and get the  nearestNeighbourTourLength
    double bestTourLength = getNearestNeighbourTour(); //start with a decent point

    if (mini > bestTourLength) mini = bestTourLength;
    double temperature, coolingRate = 0.9, absoluteTemperature = 0.00001, probability;
    int position1 = 0, position2 = 0;
    double newTourLength, difference;

    ofstream fs(outFileQuick);

    unsigned int start_time = clock();   //начальное время
    for (int rs = 0; rs < 100; rs++) {
        temperature = DBL_MAX; //Initial Temperature

        while (temperature > absoluteTemperature) {
            position1 = int(getRandomNumber(0, numOfCities - 1));
            position2 = int(getRandomNumber(0, numOfCities - 1));

            while (position1 == position2 || ((position1 > numOfCities - 2) || (position2 > numOfCities - 2))) {
                position1 = int(getRandomNumber(0, numOfCities - 2));
                position2 = int(getRandomNumber(0, numOfCities - 2));

            }
            swap2(position1, position2);
            it2 = cities.begin();
            newTourLength = getTourLength();

            if (mini > newTourLength) mini = newTourLength;
            difference = newTourLength - bestTourLength;

            if (difference < 0 ||
                (difference > 0 && getProbability(difference, temperature) > getRandomNumber(0, 1))) {
                finalOrder.clear();

                for (it = cities.begin(); it != cities.end(); it++) {
                    finalOrder.push_back(*it);
                }
                bestTourLength = newTourLength;
            }
            temperature = temperature * coolingRate;

        }

        for (int& i : finalOrder) {
            fs << i << " ";
        }
        fs << "       " << bestTourLength << "        " << mini;
        fs << endl;
        shuffle(cities.begin(), cities.end(), std::mt19937(std::random_device()()));
    }

    //cout << "best solution is " << bestTourLength << "\n";
    cout << "Weight Value: " << mini << "\n";

    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time; //  exec time
    cout << "Search time: " << ((float) search_time) / CLOCKS_PER_SEC << endl;

    return 0;
}
