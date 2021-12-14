#include "Header.h"

struct Quick {
private:

    // Хранит результат в неотсортированном виде, т.е. точки выпуклой оболочки
    unordered_set<Pair, pair_hash> hull;

    // Функция находит расстояние от прямой p1p2 до точки p
    double LineDist(const Pair& p1, const Pair& p2, const Pair& p) {

        return fabs(LineL(p1, p2, p));
    }

    // Функция поиска точек искомой оболочки
    void QuickHull(const vector<Pair>& point, const Pair& p1, const Pair& p2, const int& side) {

        // текущий индикатор
        int currentI = -1;
        // максимальное расстояние
        double maxDist = 0;
        {
            // Расстояние от прямой p1p2 до точки point[i]
            double dist = 0;

            // Найдем точку на максимальном расстояни от прямой p1p2, находящуюся на стороне side
            for (int i = 0; i < point.size(); i++) {

                // Расстояние от прямой p1p2 до точки point[i]
                dist = LineDist(p1, p2, point[i]);

                if (detSide(p1, p2, point[i]) == side && dist > maxDist) {

                    currentI = i;
                    maxDist = dist;
                }
            }
        }

        // Если не одной точки не найдено, добавляем p1 и p2 в оболочку hull
        if (currentI == -1) {
            //cout << p1.first << " " << p1.second << endl;
            //cout << p2.first << " " << p2.second << endl;

            hull.insert(p1);
            hull.insert(p2);

            return;
        }

        // Рекурсия двух частей, разделенных по point[currentI]

        QuickHull(point, point[currentI], p1, -detSide(point[currentI], p1, p2));

        QuickHull(point, point[currentI], p2, -detSide(point[currentI], p2, p1));

        //cout << "-------------------\n";
    }

    // Функция вычисления точек оболочки
    void CalcHull(const vector<Pair>& point) {

        if (point.size() < 3) {

            cout << "Convex hull not possible, n < 3\n";
            return;
        }

        int min = 0, max = 0;

        for (int i = 1; i < point.size(); ++i) {

            //Найдем самую левую точку min и самую правую точку max
            if (point[i].first < point[min].first)

                min = i;

            if (point[i].first > point[max].first)

                max = i;

            // Если таких несколько, выберем среди таких нижнюю и верхнюю соответственно
            if ((fabs(point[i].first - point[min].first) < accuracy) && (point[i].second - point[min].second) < 0)
                min = i;

            if ((fabs(point[i].first - point[max].first) < accuracy) && (point[i].second - point[max].second) > 0)
                max = i;
        }

        // Найдем точки оболочки на одной стороне прямой a[min] a[max] с помощью рекурсии
        QuickHull(point, point[min], point[max], 1);

        // Найдем точки оболочки на противоположной стороне прямой a[min] a[max] с помощью рекурсии
        QuickHull(point, point[min], point[max], -1);
    }

    void exe() {

        std::cout << "Quick:";
        vector<Pair> arr = { };
        ReadFromFile(arr, inFileQuick);
        CalcHull(arr);
        arr.clear();
        SortAndPrintHull(hull);
    }


public:

    void operator()() {
        this->exe();
    }

};




struct NonEffective {
private:

    // Хранит результат в неотсортированном виде, т.е. точки выпуклой оболочки
    unordered_set<Pair, pair_hash> hull;

    // Функция вычисления точек оболочки
    void  calcHull(vector<Pair>& point) {


        if (point.size() < 3) {

            cout << "Convex hull not possible, n < 3\n";
            return;
        }

        int min = 0, max = 0;

        for (int i = 1; i < point.size(); ++i) {

            //Найдем самую правую точку max по x
            if (point[i].first > point[max].first)
                max = i;
        }

        // лобавим первый элемент в исходную оболочку
        hull.insert(point[max]);

        // предыдущий элемент оболочки
        Pair prevPoint = point[max];
        for (auto it = point.begin() + 1; it < point.end(); ++it) {

            // Возможный следующий элемент в оболочке
            const Pair nextPoint = *it;

            // находятся ли точки на одной стороне прямой
            bool SameSide = true;
            auto iterator = point.begin();
            int side = 0;

            // найдем первую точку не лежащую на прямой
            for (; iterator < point.end(); ++iterator) {

                side = detSide(prevPoint, nextPoint, *iterator);
                if (side != 0) {
                    ++iterator;
                    break;
                }
            }

            // сравним оставшиеся точки с полученной (будут ли они по одну сторону относительно прямой prevPoint nextPoint)
            for (; iterator < point.end(); ++iterator) {

                int currPointSide = detSide(prevPoint, nextPoint, *iterator);
                if ((currPointSide != side) && (currPointSide != 0)) {
                    SameSide = false;
                    break;
                }
            }

            if (SameSide) {
                // если в hull уже есть такой элемент, то продолжаем цикл for
                if (hull.insert(nextPoint).second == false)
                    continue;
                prevPoint = nextPoint;
                it = point.begin();
            }
        }
    }

    void exe() {

        std::cout << "None effective:";
        vector<Pair> arr = { };
        ReadFromFile(arr, inFileNonEff);
        calcHull(arr);
        SortAndPrintHull(hull);
    }
public:

    void operator ()() {
        this->exe();
    }

};

int main() {

    Quick()();
    cout << "\n-----------------------------------------\n";
    NonEffective()();
    return 0;
}