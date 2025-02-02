#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// **** Global Variables, Defines and Enuns

int const N = 11234567;
int n;
int a[N];
string color[N]; // See https://en.wikipedia.org/wiki/ANSI_escape_code
bool slowMode, showSteps, statisticsMode;
double comparations, assignments, accesses, exchanges;
bool bruteStatistics;

typedef enum {
	RANDOM_ARRAY = 1,
	RANDOM_PERMUTATION,
	FROM_KEYBOARD,
	ALMOST_SORTED,
	SORTED,
	REVERSED,
	BRUTE_TEST
} ArrayType;
typedef enum {
	DEFAULT = 0,
	BLACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
} Color;
typedef void (*SortMethod)();

class SortAlgorithm {
  public:
	string name;
	SortMethod func;
};

// **** General Functions ****
void HitEnterClearingConsoleAndShowingMessage();
void HitEnterToContinue();

void BruteStatisticsManager(SortAlgorithm sorting) {
	int times;

	statisticsMode = true;
	showSteps = slowMode = false;

	printf("Please, enter the array lenght: ");
	scanf("%d", &n);
	printf("How many times do you want to execute? ");
	scanf("%d", &times);

	printf("Hit ENTER to start...\n");
	getchar();
	getchar();
	system("clear");

	for (int i = 0; i < n; i++) a[i] = i + 1; // creating the array

	for (int k = 0; k < times; k++) {
		printf("Times executed: %d\n", k);

		for (int i = n - 1; i >= 1; i--) swap(a[i], a[rand() % (i + 1)]); // permutating

		sorting.func();
		system("clear");
	}

	// Taking the averege
	accesses /= times;
	assignments /= times;
	comparations /= times;
	exchanges /= times;
	printf("%s tested %d times.\n", sorting.name.c_str(), times);
}

void HitEnterClearingConsoleAndShowingMessage() {
	cin.ignore();
	fflush(stdin);
	printf("Hit enter key to continue...\n");
	getchar();
	system("clear");
}

void HitEnterToContinue() { getchar(); }

// **** Printing Functions
void PrintInputArray();
void PrintColoredArray();
void PrintOutputArray();
void PrintStatistics();

void PrintInputArray() {
	char in;
	do {
		printf("Do you want to print it? (y/n) ");
		scanf(" %c", &in);
		if (in != 'y' && in != 'n') printf("Wrong input!\n");
	} while (in != 'y' && in != 'n');

	system("clear");
	if (in == 'y') {
		for (int i = 0; i < n; i++) printf("%d ", *(a + i));
		printf("\n");
		HitEnterClearingConsoleAndShowingMessage();
	}
}

void PrintColoredArray() {
	if (showSteps) {
		for (int i = 0; i < n; i++)
			printf("\033[1;%sm%d\033[0m ", color[i].c_str(), *(a + i));
		printf("\n");
		if (slowMode) HitEnterToContinue();
	}
}

void PrintOutputArray() {
	printf("The output array is:\n");
	for (int i = 0; i < n; i++) printf("%d ", *(a + i));
	printf("\n");
}

void PrintStatistics() {
	if (statisticsMode) {
		printf("Accesses: %.0f\t\t\t%.4f * N\n"
		       "Assignments: %.0f\t\t%.4f * N\n"
		       "Comparations: %.0f\t\t%.4f * N\n"
		       "Exchanges: %.0f\t\t\t%.4f * N\n"
		       "lg(N) = %.3f\n"
		       "N = %d\n"
		       "Nlg(N) = %.3f\n"
		       "N^2 = %d\n"
		       "N^3 = %d\n",
		       accesses, accesses / n, assignments, assignments / n, comparations,
		       comparations / n, exchanges, exchanges / n, log(n), n, n * log(n), n * n,
		       n * n * n);
	}
}

// **** Painting Functions ****
inline void ResetArrayColor();
inline void ColorElement(int index, Color c);

inline void ResetArrayColor() {
	for (int i = 0; i < n; i++) ColorElement(i, DEFAULT);
}
inline void ColorElement(int index, Color c) { color[index] = to_string(c); }

// **** Init Functions ****
void RandomArray() {
	int max;
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Please, what's the maximum value of the array? ");
	scanf("%d", &max);
	printf("Creating the random array...\n");

	for (int i = 0; i < n; i++) a[i] = rand() % (max + 1);

	printf("Created\n");
}

void RandomPermutation() {
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Creating the array...\n");
	for (int i = 0; i < n; i++) a[i] = i + 1;
	printf("Permutating...\n");
	int j;
	for (int i = n - 1; i >= 1; i--) {
		j = rand() % (i + 1);
		swap(a[i], a[j]);
	}
	printf("Permutated\n");
}

void GetArrayFromKeyboard() {
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Enter %d elements: ", n);
	for (int i = 0; i < n; i++) scanf("%d", a + i);
	printf("Done\n");
}

void AlmostSortedArray() {
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Creating the array...\n");
	a[0] = n;
	for (int i = 1; i < n; i++) a[i] = i;

	printf("Created\n");
}

void SortedArray() {
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Creating the array...\n");
	for (int i = 0; i < n; i++) a[i] = i + 1;
	printf("Created\n");
}

void ReversedArray() {
	printf("Please, enter N: ");
	scanf("%d", &n);
	printf("Creating the array...\n");
	for (int i = 0; i < n; i++) a[i] = n - i;
	printf("Created\n");
}

// **** Sort Functions ****
void InsertionSort() {
	int i, j, k, t;
	for (i = 1; i < n; i++) {
		ResetArrayColor();
		ColorElement(i, GREEN);

		t = a[i];
		accesses++;
		assignments++;
		j = i - 1;

		PrintColoredArray();

		comparations++;
		accesses++;
		while (a[j] > t) {
			ResetArrayColor();
			ColorElement(j + 1, RED);
			ColorElement(j, RED);
			PrintColoredArray();
			a[j + 1] = a[j];
			accesses += 2;
			assignments++;
			PrintColoredArray();
			j--;
			comparations++;
			accesses++;
		}
		a[j + 1] = t;
		accesses++;
		assignments++;

		ResetArrayColor();
		ColorElement(j + 1, GREEN);
		PrintColoredArray();
	}
}

void SelectionSort() {
	int min, i, j, k;
	for (i = 0; i < n - 1; i++) {
		ResetArrayColor();
		min = i;
		ColorElement(i, GREEN);
		PrintColoredArray();

		for (j = i + 1; j < n; j++) {
			ResetArrayColor();
			ColorElement(j, RED);
			comparations++;
			accesses += 2;
			if (a[j] < a[min]) {
				min = j;
				ColorElement(j, BLUE);
				comparations++;
				accesses += 2;
			}
			PrintColoredArray();
		}

		ResetArrayColor();
		ColorElement(i, RED);
		ColorElement(min, RED);
		PrintColoredArray();
		swap(a[i], a[min]);
		accesses += 2;
		exchanges++;
		ColorElement(i, GREEN);
		PrintColoredArray();
	}
}

void BubbleSort() {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n - 1; j++) {
			ResetArrayColor();
			ColorElement(j, RED);
			ColorElement(j + 1, RED);
			PrintColoredArray();
			comparations++;
			accesses += 2;
			if (a[j] > a[j + 1]) {
				ColorElement(j, BLUE);
				ColorElement(j + 1, BLUE);
				swap(a[j], a[j + 1]);
				exchanges++;
				accesses += 2;
				PrintColoredArray();
			}
		}
	}
}

void BubbleSortImproved() {
	int i, j;
	bool stop = false;
	for (i = 0; i < n && !stop; i++) {
		stop = true;
		for (j = 0; j < n - 1 - i; j++) {
			ResetArrayColor();
			ColorElement(j, RED);
			ColorElement(j + 1, RED);
			PrintColoredArray();
			comparations++;
			accesses += 2;
			if (a[j] > a[j + 1]) {
				stop = false;
				ColorElement(j, BLUE);
				ColorElement(j + 1, BLUE);
				swap(a[j], a[j + 1]);
				exchanges++;
				accesses += 2;
				PrintColoredArray();
			}
		}
	}
}

void CocktailSort() {
	int i, j;
	bool stop = false;
	for (i = 0; i < n && !stop; i++) {
		stop = true;
		for (j = i; j < n - 1 - i; j++) {
			ResetArrayColor();
			ColorElement(j, RED);
			ColorElement(j + 1, RED);
			PrintColoredArray();
			accesses += 2;
			comparations++;
			if (a[j] > a[j + 1]) {
				stop = false;
				ColorElement(j, BLUE);
				ColorElement(j + 1, BLUE);
				swap(a[j], a[j + 1]);
				accesses += 2;
				exchanges++;
				PrintColoredArray();
			}
		}
		j--;
		for (; j >= i + 1 && !stop; j--) {
			ResetArrayColor();
			ColorElement(j, RED);
			ColorElement(j - 1, RED);
			PrintColoredArray();
			accesses += 2;
			comparations++;
			if (a[j - 1] > a[j]) {
				stop = false;
				ColorElement(j, BLUE);
				ColorElement(j - 1, BLUE);
				swap(a[j - 1], a[j]);
				accesses += 2;
				exchanges++;
				PrintColoredArray();
			}
		}
	}
}

void ShellSort() {
	int i, j, h, t;
	for (h = n / 2; h >= 1; h /= 2) {
		for (i = h; i < n; i++) {
			ResetArrayColor();
			ColorElement(i, GREEN);

			t = a[i];
			accesses++;
			assignments++;
			j = i;

			ColorElement(j - h, RED);
			PrintColoredArray();

			comparations += 2;
			accesses++;
			while (j >= h && t < a[j - h]) {
				ColorElement(j, RED);

				a[j] = a[j - h];
				accesses += 2;
				assignments++;
				j -= h;

				PrintColoredArray();

				ResetArrayColor();
				if (j >= h) {
					ColorElement(j - h, RED);
					comparations++;
					accesses++;
				}

				ColorElement(j, RED);
				PrintColoredArray();

				comparations++;
			}

			ResetArrayColor();

			ColorElement(j, GREEN);
			a[j] = t;
			accesses++;
			assignments++;

			PrintColoredArray();
		}
	}
}

void CountingSort() {
	int max = INT32_MIN, *c, i, j;
	for (i = 0; i < n; i++) {
		ResetArrayColor();
		ColorElement(i, RED);
		accesses++;
		if (a[i] > max) {
			max = a[i];
			accesses++;
			ColorElement(i, BLUE);
		}
		PrintColoredArray();
	}

	c = new int[max + 1];

	for (i = 0; i < max + 1; i++) c[i] = 0;

	for (i = 0; i < n; i++) {
		ResetArrayColor();
		ColorElement(i, RED);
		c[a[i]]++;
		accesses++;
		PrintColoredArray();
	}

	for (i = 0, j = 0; i < max + 1;) {
		if (c[i] == 0)
			i++;
		else {
			ResetArrayColor();
			ColorElement(j, RED);
			a[j++] = i;
			accesses++;
			assignments++;
			c[i]--;
			PrintColoredArray();
		}
	}
	delete[] c;
}

void quick(int l, int r);
void QuickSort() { quick(0, n - 1); }
void quick(int l = 0, int r = n - 1) {
	int i, j, pivot;

	if (l < r) {
		i = l - 1, j = r, pivot = a[r];
		accesses++;
		assignments++;

		ResetArrayColor();
		ColorElement(r, GREEN);
		PrintColoredArray();

		while (1) {
			accesses++;
			comparations++;
			while (a[++i] < pivot) {
				ResetArrayColor();
				ColorElement(r, GREEN);
				ColorElement(i, RED);
				PrintColoredArray();
				accesses++;
				comparations++;
			}

			ResetArrayColor();
			ColorElement(r, GREEN);
			ColorElement(i, BLUE);
			PrintColoredArray();

			accesses++;
			comparations++;
			while (a[--j] > pivot) {
				ResetArrayColor();
				ColorElement(r, GREEN);
				ColorElement(i, BLUE);
				ColorElement(j, RED);
				PrintColoredArray();
				if (j == l) break;
				accesses++;
				comparations++;
			}

			comparations++;
			if (i >= j || j == l) break;
			ResetArrayColor();
			ColorElement(r, GREEN);
			ColorElement(i, BLUE);
			ColorElement(j, BLUE);
			PrintColoredArray();
			swap(a[i], a[j]);
			exchanges++;
			accesses += 2;

			PrintColoredArray();
		}

		ResetArrayColor();
		ColorElement(i, BLUE);
		ColorElement(r, BLUE);
		PrintColoredArray();
		swap(a[i], a[r]);
		exchanges++;
		accesses += 2;
		PrintColoredArray();

		quick(l, i - 1);
		quick(i + 1, r);
	}
}

int w[N];
void merge(int l, int r);
void mergesort(int l, int r);
void MergeSort() { mergesort(0, n - 1); }
void mergesort(int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;
		ResetArrayColor();
		ColorElement(l, BLUE);
		ColorElement(m, BLUE);
		PrintColoredArray();
		mergesort(l, m);

		ResetArrayColor();
		ColorElement(m + 1, BLUE);
		ColorElement(r, BLUE);
		PrintColoredArray();
		mergesort(m + 1, r);
		merge(l, r);
	}
}
void merge(int l, int r) {
	if (l < r) {
		int i, j, k;
		int m = l + (r - l) / 2;

		for (i = l; i <= m; i++) {
			w[i] = a[i];
			accesses += 2;
			assignments++;
			ResetArrayColor();
			ColorElement(i, RED);
			PrintColoredArray();
		}
		for (j = r; j > m; i++, j--) {
			w[j] = a[i];
			accesses += 2;
			assignments++;
			ResetArrayColor();
			ColorElement(j, RED);
			PrintColoredArray();
		}

		i = k = l;
		j = r;

		for (; k <= r; k++) {
			ResetArrayColor();
			comparations++;
			accesses += 2;
			if (i > m || w[i] >= w[j]) {
				a[k] = w[j--];
				accesses += 2;
				assignments++;
			} else if (j <= m || w[i] <= w[j]) {
				a[k] = w[i++];
				accesses += 2;
				assignments++;
			}
			ColorElement(k, RED);
			PrintColoredArray();
		}
	}
}

// **** Main Function ****
int main() {
	system("clear");
	srand(time(NULL));

	{ // Initializing the array
		typedef void (*InitFunc)();
		InitFunc inits[] = { RandomArray,       RandomPermutation, GetArrayFromKeyboard,
			                 AlmostSortedArray, SortedArray,       ReversedArray };

		int in = -1;
		printf("What would you like?\n");
		do {
			printf("0 - Exit\n"
			       "1 - Random Array\n"
			       "2 - Random Permutation of (1, 2...N)\n"
			       "3 - Digit array\n"
			       "4 - Almost sorted array\n"
			       "5 - Sorted array\n"
			       "6 - Reversed array\n"
			       "7 - Test lots of times\n");
			scanf("%d", &in);
			if (in < 0 || in > BRUTE_TEST) printf("%d is not valid!\n", in);
		} while (in < 0 || in > BRUTE_TEST);

		system("clear");
		if (in == 0) return 0;
		if (in != BRUTE_TEST) {
			inits[in - 1]();
			PrintInputArray();
		}
		if (in == BRUTE_TEST) bruteStatistics = true;
	}

	{ // Sorting algorithms
		vector<SortAlgorithm> sorts = { { "Insertion Sort", InsertionSort },
			                            { "Selection Sort", SelectionSort },
			                            { "Bubble Sort", BubbleSort },
			                            { "Bubble Sort Improved", BubbleSortImproved },
			                            { "Cocktail Sort", CocktailSort },
			                            { "Shell Sort", ShellSort },
			                            { "Quick Sort", QuickSort },
			                            { "Merge Sort", MergeSort },
			                            { "Couting Sort", CountingSort } };

		if (!bruteStatistics) {
			// Asking if the user wants to display the steps or not
			char c = 0;
			printf("Do you want to display all steps? (y/n)\n");
			do {
				scanf(" %c", &c);
				if (c != 'y' && c != 'n') printf("%c is not valid!\n", c);
			} while (c != 'y' && c != 'n');
			showSteps = (c == 'y');

			// Asking for the slow mode, showing all steps
			if (showSteps) {
				printf("Do you want the slow mode? (y/n)\n");
				do {
					scanf(" %c", &c);
					if (c != 'y' && c != 'n') printf("%c is not valid!\n", c);
				} while (c != 'y' && c != 'n');
				slowMode = (c == 'y');
			}

			// Asking for the slow mode, showing all steps

			printf("Do you want the statistics mode? (y/n)\n");
			do {
				scanf(" %c", &c);
				if (c != 'y' && c != 'n') printf("%c is not valid!\n", c);
			} while (c != 'y' && c != 'n');
			statisticsMode = (c == 'y');
		} else
			statisticsMode = true;

		system("clear");
		// Asking for the sort method
		int in = -1;
		printf("Which one do you want?\n");
		do {
			printf("0 - Exit\n");
			for (int i = 0; i < sorts.size(); i++)
				printf("%d - %s\n", i + 1, sorts[i].name.c_str());

			scanf("%d", &in);
			if (in < 0 || in > sorts.size()) printf("%d is not valid!\n", in);
		} while (in < 0 || in > sorts.size());
		system("clear");
		if (in == 0) return 0;
		comparations = assignments = accesses = exchanges = 0;
		getchar();
		if (!bruteStatistics) {
			sorts[in - 1].func();
			PrintOutputArray();
		} else
			BruteStatisticsManager(sorts[in - 1]);
	}

	PrintStatistics();

	return 0;
}