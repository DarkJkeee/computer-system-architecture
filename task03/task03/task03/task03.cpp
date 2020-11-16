#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

int checkInput(string msg);
void createMatrix(int n);
void determinant();
void cycleDet(int j);
void readMatrix(string path);
void getRank(int j);

const double EPS = 1E-9;
int n, ths;
thread* threads;
vector<bool> line_used;
int rang;
int** matrix;

int main(int argc, char* argv[])
{
	n = checkInput("Input n(dimension of matrix): ");
	ths = checkInput("Input amount of threads: ");
	cout << endl << "Your matrix: " << endl;
	if (argc == 1)
		createMatrix(n);
	else
		readMatrix(argv[1]);
	threads = new thread[ths];
	rang = n;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}

	determinant();
	cout << endl << "Rang = " << rang << endl;

	for (int i = 0; i < n; ++i)
		delete[] matrix[i];
	delete[] matrix;
	delete[] threads;
}

void readMatrix(string path) {
	ifstream in(path);
	matrix = new int* [n];
	for (int i = 0; i < n; i++)	{
		matrix[i] = new int[n];
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			in >> matrix[i][j];
		}
	}
}
void createMatrix(int n) {
	matrix = new int* [n];
	for (int i = 0; i < n; ++i) {
		matrix[i] = new int[n];
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			matrix[i][j] = rand() % 1000;
		}
	}
}
void determinant() {
	line_used = vector<bool>(n);
	if (ths < n) {
		int k = 0;
		for (int j = 0; j < n; ++j) {
			if (k < ths) {
				threads[k] = thread(cycleDet, j);
				++k;
			}
			if (k == ths) {
				k = 0;
				for (int i = 0; i < ths; ++i)
					threads[i].join();
			}
		}
		for (int i = 0; i < k; ++i)
			threads[i].join();
		for (int j = 0; j < n; ++j)
			getRank(j);
	}
	else {
		for (int j = 0; j < n; ++j) {
			threads[j] = thread(cycleDet, j);
		}
		line_used = vector<bool>(n);
		for (int j = 0; j < n; ++j) {
			threads[j].join();
			getRank(j);
		}
	}
}
void cycleDet(int j) {
	int i;
	for (i = 0; i < n; ++i)
		if (!line_used[i] && abs(matrix[i][j]) > EPS)
			break;
	if(i != n) {
		for (int p = j + 1; p < n; ++p)
			matrix[i][p] /= matrix[i][j];
		for (int k = 0; k < n; ++k)
			if (k != i && abs(matrix[k][j]) > EPS)
				for (int p = j + 1; p < n; ++p)
					matrix[k][p] -= matrix[i][p] * matrix[k][j];
	}
}

void getRank(int j) {
	int i;
	for (i = 0; i < n; ++i)
		if (!line_used[i] && abs(matrix[i][j]) > EPS)
			break;
	if (i == n)
		--rang;
	else
		line_used[i] = true;
}

int checkInput(string msg) {
	int num;
	cout << msg;
	cin >> num;
	if (!cin.good()) {
		cout << "Wrong data" << endl;
		exit(0);
	}
	return num;
}
