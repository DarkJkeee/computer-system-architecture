#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;

int checkInput(string msg);
void createMatrix(int n);
void readMatrix(string path);
void getRank();

const double EPS = 1E-9;
int n, ths;
vector<bool> line_used;
int rang;
int** matrix;

int main(int argc, char* argv[])
{
	n = checkInput("Input n(dimension of matrix): ");
	ths = checkInput("Input amount of threads: ");
	if (argc == 1)
		createMatrix(n);
	else
		readMatrix(argv[1]);
	rang = n;

	cout << endl << "Your matrix: " << endl;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			cout << matrix[i][j] << " ";
		cout << endl;
	}

	getRank(); // get rank of matrix.
	cout << endl << "Rang = " << rang << endl; // write rank in console.

	// delete memory allocation.
	for (int i = 0; i < n; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

void readMatrix(string path) {
	ifstream in(path);
	matrix = new int* [n];
	for (int i = 0; i < n; i++) {
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
void getRank() {
	line_used = vector<bool>(n);
#pragma omp parallel for num_threads(ths)
	for (int i = 0; i < n; ++i) {
//		cout << omp_get_thread_num();
		int j;
		for (j = 0; j < n; ++j)
			if (!line_used[j] && abs(matrix[j][i]) > EPS)
				break;
		if (j == n)
			--rang;
		else {
			line_used[j] = true;
			for (int p = i + 1; p < n; ++p)
				matrix[j][p] /= matrix[j][i];
			for (int k = 0; k < n; ++k)
				if (k != j && abs(matrix[k][i]) > EPS)
					for (int p = i + 1; p < n; ++p)
						matrix[k][p] -= matrix[j][p] * matrix[k][i];
		}
	}
}
