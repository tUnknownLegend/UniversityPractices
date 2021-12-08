#include "Header.h"

struct Quick {
private:

    // Функция находит расстояние от прямой p1p2 до точки p
    double lineDist(const Pair& p1, const Pair& p2, const Pair& p) {

        return fabs(LineL(p1, p2, p));
    }

    // Функция поиска точек искомой оболочки
    void quickHull(const vector<Pair>& point, const Pair& p1, const Pair& p2, const int& side) {

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
                dist = lineDist(p1, p2, point[i]);

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

        quickHull(point, point[currentI], p1, -detSide(point[currentI], p1, p2));

        quickHull(point, point[currentI], p2, -detSide(point[currentI], p2, p1));

        //cout << "-------------------\n";
    }

    // Функция вычисления точек оболочки
    void calcHull(const vector<Pair>& point) {

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
        quickHull(point, point[min], point[max], 1);

        // Найдем точки оболочки на противоположной стороне прямой a[min] a[max] с помощью рекурсии
        quickHull(point, point[min], point[max], -1);
    }

    double get_clockwise_angle(const Pair& p, const Pair& c) {

        return (-atan2(p.first + c.first, -(p.second + c.second)));
    }

    bool compare_points(const Pair& a, const Pair& b, const Pair& c)
    {
        return (get_clockwise_angle(a, c) < get_clockwise_angle(b, c));
    }


    void exe() {

        hull.clear();
        std::cout << "Quick:\n";
        vector<Pair> arr = { };
        ReadFromFile(arr, inFileQuick);
        calcHull(arr);

        /*
        double xc = 0.0;
        double yc = 0.0;

        for (auto it : hull) {

            xc += it.first;
            yc += it.second;
        }

        Pair c = { xc / hull.size(), yc / hull.size() };

        cout << c.first << " " << c.second;

        for (auto it : hull) {

            for (auto i : hull) {
                if (!compare_points(it, i, c))
                    swap(it, i);
            }
        }

        

        cout << "The points in Convex Hull are: " << hull.size() << " " << (*hull.begin()).first;
        for (auto it : hull)
        {
            cout << "\n(" << it.first << ", " << it.second << ") ";
        }
        */
        cout << get_clockwise_angle(*hull.begin(), *(hull.begin()));

         WriteToFile(hull, outFileQuick);
         printHull();
    }


public:

    void operator()() {
        this->exe();
    }

};




struct NonEffective {
private:

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
        hull.clear();
        std::cout << "None effective:\n";
        vector<Pair> arr = { };
        ReadFromFile(arr, inFileNonEff);
        calcHull(arr);
        WriteToFile(hull, outFileNonEff);
        printHull();
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