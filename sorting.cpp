#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

// **** Global Variables, Defines and Enuns

int const N = 112345;
int n;
int a[N];
string color[N]; // See https://en.wikipedia.org/wiki/ANSI_escape_code
bool slowMode, showSteps, statisticsMode;
double comparations, assignments, accesses, exchanges;
bool bruteStatistics;

typedef enum {
    RANDOM_ARRAY = 1, RANDOM_PERMUTATION, FROM_KEYBOARD, ALMOST_SORTED, SORTED, REVERSED, BRUTE_TEST
} ArrayTypes;

// **** General Functions ****

void HitEnterClearingConsoleAndShowingMessage();
void HitEnterToContinue();

// **** Init functions ****
void PrintInputArray();

void RandomArray() {
    int max;
    printf("Please, enter N: ");
    scanf("%d", &n);
    printf("Please, what's the maximum value of the array? ");
    scanf("%d", &max);
    printf("Creating the random array...\n");

    for (int i = 0; i < n; i++) 
        a[i] = rand() % (max+1);

    printf("Created\n");
}

void RandomPermutation() {
    printf("Please, enter N: ");
    scanf("%d", &n);
    printf("Creating the array...\n");
    for (int i = 0; i < n; i++) a[i] = i+1;
    printf("Permutating...\n");
    int j;
    for (int i = n-1; i >= 1; i--) {
        j = rand() % (i+1);
        swap(a[i], a[j]);
    }
    printf("Permutated\n");
}

void GetArrayFromKeyboard() {
    printf("Please, enter N: ");
    scanf("%d", &n);
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) scanf("%d", a+i);
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
    for (int i = 0; i < n; i++) a[i] = i+1;
    printf("Created\n");
}

void ReversedArray() {
    printf("Please, enter N: ");
    scanf("%d", &n);
    printf("Creating the array...\n");
    for (int i = 0; i < n; i++) a[i] = n-i;
}


void PrintInputArray() {
    char in;
    do {
        printf("Do you want to print it? (y/n) ");
        scanf(" %c", &in);
        if (in != 'y' && in != 'n') printf("Wrong input!\n"); 
    } while (in != 'y' && in != 'n');

    system("clear");
    if (in == 'y') {
        for (int i = 0; i < n; i++) printf("%d ", *(a+i));
        printf("\n");
        HitEnterClearingConsoleAndShowingMessage();
    }
}

// **** Sort functions ****
void PrintColoredArray();

void InsertionSort() {
    for (int i = 1; i < n; i++) {
        for (int k = 0; k < n; k++) color[k] = "0"; // default
        color[i] = "32"; // green

        int t = a[i]; accesses++; assignments++;
        int j = i-1; 

        PrintColoredArray();

        comparations++; accesses++;
        while (a[j] > t) {
            for (int i = 0; i < n; i++) color[i] = "0"; // default
            color[j+1] = color[j] = "31"; // red
            PrintColoredArray();
            a[j+1] = a[j]; accesses += 2; assignments++; exchanges++;
            PrintColoredArray();
            j--;
            comparations++; accesses++;
        }
        a[j+1] = t; accesses++; assignments++;

        for (int i = 0; i < n; i++) color[i] = "0"; // default
        color[j+1] = "32"; //green
        PrintColoredArray();
    }
}

void BruteStatisticsManager(void (*sortFunc)()) {
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

    for (int i = 0; i < n; i++) a[i] = i+1; // creating the array

    for (int k = 0; k < times; k++) {
        printf("Times executed: %d\n", k);

        for (int i = n-1; i >= 1; i--) swap(a[i], a[rand() % (i+1)]); // permutating

        sortFunc();
        system("clear");
    }

    // Taking the averege
    accesses /= times;
    assignments /= times;
    comparations /= times;
    exchanges /= times;
}

void PrintColoredArray() {
    if (showSteps) {
        for (int i = 0; i < n; i++) 
            printf("\033[1;%sm%d \033[0m ", color[i].c_str(), *(a+i));
        printf("\n");
        if (slowMode) HitEnterToContinue();
    }
}

void PrintOutputArray() {
    printf("\n\nThe output array is:\n");
    for (int i = 0; i < n; i++) printf("%d ", *(a+i));
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
               accesses, accesses / n,
               assignments,  assignments / n,
               comparations, comparations / n,
               exchanges, exchanges / n,
               log(n), n, n*log(n), n*n, n*n*n
        );
    }
}


int main() {
    srand(time(NULL));

    {   // Initializing the array
        typedef void (*InitFunc)();
        InitFunc inits[] = {
            RandomArray, RandomPermutation, GetArrayFromKeyboard, AlmostSortedArray, SortedArray, ReversedArray
        };

        int in = -1;
        printf("What would you like?\n");
        do {
            printf("0 - Exit\n"
                "1 - Random Array\n"
                "2 - Random Permutation of (1, 2...N)\n"
                "3 - Digit array\n"
                "4 - Almosted sorted array\n"
                "5 - Sorted array\n"
                "6 - Reversed array\n"
                "7 - Test lots of times\n");
            scanf("%d", &in);
            if (in < 0 || in > BRUTE_TEST) printf("%d is not valid!\n", in); 
        } while (in < 0 || in > BRUTE_TEST);
        
        if (in == 0) return 0;
        if (in != BRUTE_TEST) {
            inits[in-1]();
            PrintInputArray();
        }
        if (in == BRUTE_TEST) bruteStatistics = true;
    }

    

    {  // Sorting algorithms
        typedef void (*SortMethod)();
        SortMethod sorts[] = {
            InsertionSort
        };

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
        
        // Asking for the sort method
        int in = -1;
        printf("Which one do you want?\n");
        do {
            printf("0 - Exit\n"
                "1 - Insertion Sort\n");
            scanf("%d", &in);
            if (in < 0 || in > 1) printf("%d is not valid!\n", in);
        } while (in < 0 || in > 1);
        system("clear");
        comparations = assignments = accesses = exchanges = 0;
        getchar();
        if (!bruteStatistics) {
            sorts[in-1]();
            PrintOutputArray();
        } else BruteStatisticsManager(sorts[in-1]);
    }

    PrintStatistics();

    return 0;
}

void HitEnterClearingConsoleAndShowingMessage() {
    cin.ignore();
    fflush(stdin);
    printf("Hit enter key to continue...\n");
    getchar();
    system("clear");
}

void HitEnterToContinue() {
    getchar();
}