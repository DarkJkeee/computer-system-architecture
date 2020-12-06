#include <iostream>
#include <thread>
#include <semaphore>
#include "windows.h"
#include "process.h"

using namespace std;

int checkInput(string message);
void eatMeat(int i);
void cookMeat();

int n, m, max_m;     // variables for count of savages and slices of missionary.
thread *ths;	     // array of threads.
HANDLE sem, meatSem; // HANDLE variables for semaphores (first for stable output, second for slices of missionary). 

int main() {
	cout << "Hi, today we are going to eat a missionary...\n" << endl;
	n = checkInput("Enter n(number of savages [1; 100]): ");				  // check if entered values are correct.
	m = checkInput("Enter m(number of slices of missionary [1; 100]): ");     // check if entered values are correct.
	cout << "\nWe have " << n << " savages and " << m << " slices of missionary\nWe are ready to eat!\n" << endl;
	Sleep(4000);

	max_m = m;
	meatSem = CreateSemaphore(NULL, m + 1, m + 1, NULL); // Create semaphore for slices of meat (m + 1 for the next savage who will call a cooker.).
	sem = CreateSemaphore(NULL, 1, 1, NULL);             // Create semaphore for stable output.
	ths = new thread[n];                                 // Create array of n threads.

	// Start threads.
	for (int i = 0; i < n; ++i) 
		ths[i] = thread(eatMeat, i + 1);

	// Waiting for finishing
	for (int i = 0; i < n; ++i)
		ths[i].join();

	cout << "\nAll savages ate their meat three times!" << endl;
	delete[] ths; // delete memory which allocated for dynamic array. 
}

/// <summary>
/// Check if entered data is correct and write a message for user
/// </summary>
/// <param name="message">message for user</param>
/// <returns>value entered by user</returns>
int checkInput(string message) {
	while (true) {
		cout << message;
		int num;
		cin >> num;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "You should enter a number [1; 100]" << endl;
		}
		else if (num < 1 || num > 100) {
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "You should enter a number [1; 100]" << endl;
		}
		else
			return num;
	}
}

/// <summary>
/// Method which every savage use for their eating
/// </summary>
/// <param name="i">number of savage</param>
void eatMeat(int i) {
	for (int j = 0; j < 3; ++j) {
		WaitForSingleObject(meatSem, INFINITE); // Only m savages can take their slice of meat.
		WaitForSingleObject(sem, INFINITE);     // They take their meat one by one.
		if (m == 0)
			cookMeat();
		m--;
		cout << "Savage " << i << " takes one meat. Remain: " << m << " slices of meat" << endl; // report
		Sleep(2000);
		ReleaseSemaphore(sem, 1, NULL);         // Realease semaphor for next savage.
		WaitForSingleObject(sem, INFINITE);     // Semaphore for stable output. One by one.
		cout << "Savage " << i << " ate one meat." << endl; // Savages eat their meat together and report about it.
		ReleaseSemaphore(sem, 1, NULL);
		Sleep(1000);
	}
}

/// <summary>
/// Method which cooking a new missionary (make m equals the maximum value)
/// </summary>
void cookMeat() {
	cout << "There are no meat! The chef is cooking a new missionary... Wait, please" << endl;
	m = max_m;
	Sleep(2000);
	cout << "Now we have " << m << " new slices of meat!\n" << endl;
	ReleaseSemaphore(meatSem, max_m + 1, NULL);
	Sleep(2000);
}