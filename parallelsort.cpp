#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <omp.h>

using namespace std;

// Utility to generate random array
void generateArray(vector<int>& arr, int size) {
    arr.resize(size);

    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
    }
}

// Function to print first 20 elements of array
void printArray(vector<int>& arr) {
    for (int i = 0; i < 20 && i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ================= SEQUENTIAL BUBBLE SORT =================
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ================= PARALLEL BUBBLE SORT =================
// Odd-Even Transposition Sort
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        #pragma omp parallel for
        for (int j = (i % 2); j < n - 1; j += 2) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ================= MERGE FUNCTION =================
void merge(vector<int>& arr, int left, int mid, int right) {

    vector<int> temp(right - left + 1);

    int i = left;
    int j = mid + 1;
    int k = 0;

    // Merge two sorted halves
    while (i <= mid && j <= right) {

        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    // Copy remaining elements
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // Copy back to original array
    for (int i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
}

// ================= SEQUENTIAL MERGE SORT =================
void mergeSortSequential(vector<int>& arr, int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// ================= PARALLEL MERGE SORT =================
void mergeSortParallel(vector<int>& arr, int left, int right, int depth = 0) {

    if (left < right) {

        int mid = (left + right) / 2;

        // Limit thread creation
        if (depth < 4) {

            #pragma omp parallel sections
            {

                #pragma omp section
                mergeSortParallel(arr, left, mid, depth + 1);

                #pragma omp section
                mergeSortParallel(arr, mid + 1, right, depth + 1);
            }
        }
        else {

            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// ================= MAIN FUNCTION =================
int main() {

    const int SIZE = 5000;

    vector<int> original;

    generateArray(original, SIZE);

    // Create copies for each algorithm
    vector<int> a1 = original;
    vector<int> a2 = original;
    vector<int> a3 = original;
    vector<int> a4 = original;

    double t1, t2;

    // Print original array
    cout << "Original Array (First 20 Elements):\n";
    printArray(original);

    // ================= SEQUENTIAL BUBBLE SORT =================
    t1 = omp_get_wtime();

    bubbleSortSequential(a1);

    t2 = omp_get_wtime();

    cout << "\nSequential Bubble Sort Time: "
         << t2 - t1 << " seconds\n";

    cout << "Sorted Array (First 20 Elements):\n";
    printArray(a1);

    // ================= PARALLEL BUBBLE SORT =================
    t1 = omp_get_wtime();

    bubbleSortParallel(a2);

    t2 = omp_get_wtime();

    cout << "\nParallel Bubble Sort Time: "
         << t2 - t1 << " seconds\n";

    cout << "Sorted Array (First 20 Elements):\n";
    printArray(a2);

    // ================= SEQUENTIAL MERGE SORT =================
    t1 = omp_get_wtime();

    mergeSortSequential(a3, 0, SIZE - 1);

    t2 = omp_get_wtime();

    cout << "\nSequential Merge Sort Time: "
         << t2 - t1 << " seconds\n";

    cout << "Sorted Array (First 20 Elements):\n";
    printArray(a3);

    // ================= PARALLEL MERGE SORT =================
    t1 = omp_get_wtime();

    mergeSortParallel(a4, 0, SIZE - 1);

    t2 = omp_get_wtime();

    cout << "\nParallel Merge Sort Time: "
         << t2 - t1 << " seconds\n";

    cout << "Sorted Array (First 20 Elements):\n";
    printArray(a4);

    return 0;
}