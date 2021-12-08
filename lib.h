#pragma once
#include<iostream> 
#include <set>
#include <fstream> 
#include <vector>
#include <iterator>

using namespace std;

// Пары координат (x; y) 
#define Pair pair<double, double>
// Точность сравнения double
#define accuracy 10e-10
// Входной файл для Quick
#define inFileQuick "problem2.txt"
// Входной файл для NonEff
#define inFileNonEff "problem2.txt"
// Выходной файл Quick
#define outFileQuick "out_Quick.txt"
// Выходной файл Noneff
#define outFileNonEff "out_NonEff.txt"

// Хранит результат, т.е. выпуклую оболочку
set<Pair> hull;

double LineL(const Pair& p1, const Pair& p2, const Pair& p) {
	return ((p.second - p1.second) * (p2.first - p1.first) -
		(p2.second - p1.second) * (p.first - p1.first));
}

// Функция находит по какю сторону относительно прямой p1p2 лежит точка p
int detSide(const Pair& p1, const Pair& p2, const Pair& p)
{
	// Данное значение определяет по какую сторону от прямой p1p2 лежит точка p
	double side = LineL(p1, p2, p);

	if (side > 0)
		return 1;

	if (side < 0)
		return -1;

	return 0;
}

	// Функия для записи координат точек из set<Pair> pr в текстовый файл
void WriteToFile(set<Pair>& pr, const string& path) {

	// Создадим ofstream для записи в файл
	ofstream fin;
	fin.open(path);
	fin << pr.size();

	for (auto it : hull)
		fin << "\n" << it.first << " " << it.second;

	fin.close();
}

// Функия для считывания координат точек из текстого файла в vector<Pair> pr
void ReadFromFile(vector<Pair>& pr, const string& path) {
	// Создадим ifstream для чтения файла
	ifstream fout;

	fout.open(path);
	int n = 0;
	fout >> n;
	pr.resize(n + 1);
	// cout << n << " " << pr.size();
	n = 0;
	// Записываем данные в vector<Pair> pr
	while (fout) {
		fout >> pr[n].first >> pr[n].second;
		++n;
	}

	fout.close();
}

// Функция для вывода результата в консоль
void printHull() {
	cout << "The points in Convex Hull are: " << hull.size();

	for (auto it : hull)
	{
		cout << "\n(" << it.first << ", " << it.second << ") ";
	}
}