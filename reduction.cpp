#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

template <typename E>
class ParallelReduction {

    vector<E> arr;

public:

    ParallelReduction(const vector<E>& inputArr) {
        arr = inputArr;
    }

    // ================= SEQUENTIAL SUM =================
    E sequentialSum() {

        E sum = 0;

        for (size_t i = 0; i < arr.size(); i++) {
            sum += arr[i];
        }

        return sum;
    }

    // ================= PARALLEL SUM =================
    E parallelSum() {

        E sum = 0;

        #pragma omp parallel for reduction(+:sum)
        for (size_t i = 0; i < arr.size(); i++) {
            sum += arr[i];
        }

        return sum;
    }

    // ================= SEQUENTIAL MIN =================
    E sequentialMin() {

        E minElement = arr[0];

        for (size_t i = 1; i < arr.size(); i++) {

            if (arr[i] < minElement) {
                minElement = arr[i];
            }
        }

        return minElement;
    }

    // ================= PARALLEL MIN =================
    E parallelMin() {

        E minElement = numeric_limits<E>::max();

        #pragma omp parallel for reduction(min:minElement)
        for (size_t i = 0; i < arr.size(); i++) {

            if (arr[i] < minElement) {
                minElement = arr[i];
            }
        }

        return minElement;
    }

    // ================= SEQUENTIAL MAX =================
    E sequentialMax() {

        E maxElement = arr[0];

        for (size_t i = 1; i < arr.size(); i++) {

            if (arr[i] > maxElement) {
                maxElement = arr[i];
            }
        }

        return maxElement;
    }

    // ================= PARALLEL MAX =================
    E parallelMax() {

        E maxElement = numeric_limits<E>::min();

        #pragma omp parallel for reduction(max:maxElement)
        for (size_t i = 0; i < arr.size(); i++) {

            if (arr[i] > maxElement) {
                maxElement = arr[i];
            }
        }

        return maxElement;
    }

    // ================= SEQUENTIAL AVERAGE =================
    double sequentialAverage() {

        E sum = sequentialSum();

        return (double)sum / arr.size();
    }

    // ================= PARALLEL AVERAGE =================
    double parallelAverage() {

        E sum = parallelSum();

        return (double)sum / arr.size();
    }
};

int main() {

    vector<int> arr;

    // Generate large random array
    for (int i = 0; i < 1000000; i++) {
        arr.push_back(rand() % 1000);
    }

    ParallelReduction<int> pr(arr);

    double start, end;

    // ================= SUM =================
    start = omp_get_wtime();

    int seqSum = pr.sequentialSum();

    end = omp_get_wtime();

    cout << "Sequential Sum: " << seqSum << endl;
    cout << "Sequential Sum Time: "
         << end - start << " seconds\n\n";

    start = omp_get_wtime();

    int parSum = pr.parallelSum();

    end = omp_get_wtime();

    cout << "Parallel Sum: " << parSum << endl;
    cout << "Parallel Sum Time: "
         << end - start << " seconds\n\n";

    // ================= MIN =================
    start = omp_get_wtime();

    int seqMin = pr.sequentialMin();

    end = omp_get_wtime();

    cout << "Sequential Min: " << seqMin << endl;
    cout << "Sequential Min Time: "
         << end - start << " seconds\n\n";

    start = omp_get_wtime();

    int parMin = pr.parallelMin();

    end = omp_get_wtime();

    cout << "Parallel Min: " << parMin << endl;
    cout << "Parallel Min Time: "
         << end - start << " seconds\n\n";

    // ================= MAX =================
    start = omp_get_wtime();

    int seqMax = pr.sequentialMax();

    end = omp_get_wtime();

    cout << "Sequential Max: " << seqMax << endl;
    cout << "Sequential Max Time: "
         << end - start << " seconds\n\n";

    start = omp_get_wtime();

    int parMax = pr.parallelMax();

    end = omp_get_wtime();

    cout << "Parallel Max: " << parMax << endl;
    cout << "Parallel Max Time: "
         << end - start << " seconds\n\n";

    // ================= AVERAGE =================
    start = omp_get_wtime();

    double seqAvg = pr.sequentialAverage();

    end = omp_get_wtime();

    cout << "Sequential Average: " << seqAvg << endl;
    cout << "Sequential Average Time: "
         << end - start << " seconds\n\n";

    start = omp_get_wtime();

    double parAvg = pr.parallelAverage();

    end = omp_get_wtime();

    cout << "Parallel Average: " << parAvg << endl;
    cout << "Parallel Average Time: "
         << end - start << " seconds\n\n";

    return 0;
}

/*
Compile:
g++ filename.cpp -fopenmp

Run:
./a.out
*/