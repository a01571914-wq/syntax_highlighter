#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

// ============================================================
//  Bubble Sort and Related Simple Sorting Algorithms
// ============================================================

// Classic bubble sort
void bubbleSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Bubble sort with early termination if the array becomes sorted
void bubbleSortOptimized(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // already sorted, stop early
    }
}

// Bubble sort in descending order
void bubbleSortDescending(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Cocktail shaker sort - a bidirectional variant of bubble sort
void cocktailSort(vector<int>& arr) {
    int n = (int)arr.size();
    bool swapped = true;
    int start = 0, end = n - 1;

    while (swapped) {
        swapped = false;

        for (int i = start; i < end; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
        end--;

        swapped = false;
        for (int i = end - 1; i >= start; i--) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        start++;
    }
}

// Selection sort - included for comparison, since it's a similarly simple O(n^2) sort
void selectionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}

// Insertion sort - included for comparison
void insertionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Check whether a vector is sorted in ascending order
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++)
        if (arr[i - 1] > arr[i]) return false;
    return true;
}

// Generate a vector of random integers
vector<int> generateRandomVector(int size, int minVal, int maxVal, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(minVal, maxVal);
    vector<int> v(size);
    for (int& x : v) x = dist(rng);
    return v;
}

// Print a vector with a label
void printVector(const vector<int>& v, const string& label) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

// Generic timing helper that copies the input, sorts it, and reports the time taken
template <typename SortFunc>
long long timeSort(vector<int> data, SortFunc sortFunc, const string& name) {
    auto start = high_resolution_clock::now();
    sortFunc(data);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << name << " took " << duration.count() << " microseconds. Sorted correctly: "
         << (isSorted(data) ? "yes" : "no") << endl;
    return duration.count();
}

int main() {
    cout << "=== Bubble Sort and Friends ===" << endl << endl;

    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    printVector(arr, "Original array");

    vector<int> arr1 = arr;
    bubbleSort(arr1);
    printVector(arr1, "After bubbleSort (ascending)");

    vector<int> arr2 = arr;
    bubbleSortDescending(arr2);
    printVector(arr2, "After bubbleSortDescending");

    vector<int> arr3 = arr;
    bubbleSortOptimized(arr3);
    printVector(arr3, "After bubbleSortOptimized");

    vector<int> arr4 = arr;
    cocktailSort(arr4);
    printVector(arr4, "After cocktailSort");

    cout << endl << "--- Edge cases ---" << endl;
    vector<int> empty;
    bubbleSort(empty);
    cout << "Sorting empty array, size after: " << empty.size() << endl;

    vector<int> single = {42};
    bubbleSort(single);
    printVector(single, "Sorting single-element array");

    vector<int> sortedAlready = {1, 2, 3, 4, 5};
    bubbleSortOptimized(sortedAlready);
    printVector(sortedAlready, "Sorting already-sorted array (optimized)");

    vector<int> withDuplicates = {5, 3, 5, 1, 3, 2, 5, 1};
    bubbleSort(withDuplicates);
    printVector(withDuplicates, "Sorting array with duplicates");

    vector<int> negatives = {-3, 7, -1, 0, -8, 4};
    bubbleSort(negatives);
    printVector(negatives, "Sorting array with negative numbers");

    cout << endl << "--- Performance comparison (n = 2000, random data) ---" << endl;
    vector<int> bigData = generateRandomVector(2000, -10000, 10000, 123);

    timeSort(bigData, [](vector<int>& v) { bubbleSort(v); }, "bubbleSort        ");
    timeSort(bigData, [](vector<int>& v) { bubbleSortOptimized(v); }, "bubbleSortOptimized");
    timeSort(bigData, [](vector<int>& v) { cocktailSort(v); }, "cocktailSort      ");
    timeSort(bigData, [](vector<int>& v) { selectionSort(v); }, "selectionSort     ");
    timeSort(bigData, [](vector<int>& v) { insertionSort(v); }, "insertionSort     ");
    timeSort(bigData, [](vector<int>& v) { sort(v.begin(), v.end()); }, "std::sort (reference)");

    cout << endl << "--- Verifying all algorithms produce the same result ---" << endl;
    vector<int> ref = bigData;
    sort(ref.begin(), ref.end());

    vector<int> a = bigData; bubbleSort(a);
    vector<int> b = bigData; bubbleSortOptimized(b);
    vector<int> c = bigData; cocktailSort(c);
    vector<int> d = bigData; selectionSort(d);
    vector<int> e = bigData; insertionSort(e);

    cout << "bubbleSort matches reference: " << (a == ref ? "yes" : "no") << endl;
    cout << "bubbleSortOptimized matches reference: " << (b == ref ? "yes" : "no") << endl;
    cout << "cocktailSort matches reference: " << (c == ref ? "yes" : "no") << endl;
    cout << "selectionSort matches reference: " << (d == ref ? "yes" : "no") << endl;
    cout << "insertionSort matches reference: " << (e == ref ? "yes" : "no") << endl;

    return 0;
}