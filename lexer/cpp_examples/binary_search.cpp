#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;
using namespace std::chrono;

// ============================================================
//  Binary Search Algorithms 
// ============================================================

// Classic iterative binary search
int binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Classic recursive binary search
int binarySearchRecursive(const vector<int>& arr, int left, int right, int target) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binarySearchRecursive(arr, mid + 1, right, target);
    return binarySearchRecursive(arr, left, mid - 1, target);
}

// Find the first (leftmost) occurrence of target in a sorted array with duplicates
int findFirstOccurrence(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size() - 1;
    int result = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            result = mid;
            right = mid - 1; // keep searching left half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

// Find the last (rightmost) occurrence of target in a sorted array with duplicates
int findLastOccurrence(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size() - 1;
    int result = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            result = mid;
            left = mid + 1; // keep searching right half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

// Count occurrences of target using first/last occurrence
int countOccurrences(const vector<int>& arr, int target) {
    int first = findFirstOccurrence(arr, target);
    if (first == -1) return 0;
    int last = findLastOccurrence(arr, target);
    return last - first + 1;
}

// Find the index of the smallest element >= target (lower bound)
int lowerBound(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size(); // note: right == size
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < target) left = mid + 1;
        else right = mid;
    }
    return left;
}

// Find the index of the smallest element > target (upper bound)
int upperBound(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size();
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] <= target) left = mid + 1;
        else right = mid;
    }
    return left;
}

// Binary search on a rotated sorted array (no duplicates)
int searchRotated(const vector<int>& arr, int target) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;

        if (arr[left] <= arr[mid]) { // left half is sorted
            if (target >= arr[left] && target < arr[mid]) right = mid - 1;
            else left = mid + 1;
        } else { // right half is sorted
            if (target > arr[mid] && target <= arr[right]) left = mid + 1;
            else right = mid - 1;
        }
    }
    return -1;
}

// Find a peak element index (element greater than both neighbors)
int findPeakElement(const vector<int>& arr) {
    int left = 0, right = (int)arr.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] > arr[mid + 1]) right = mid;
        else left = mid + 1;
    }
    return left;
}

// Binary search for the square root of a non-negative integer (floor value)
long long integerSqrt(long long n) {
    if (n < 2) return n;
    long long left = 1, right = n, ans = 1;
    while (left <= right) {
        long long mid = left + (right - left) / 2;
        if (mid <= n / mid) { // avoid overflow: mid*mid <= n
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

// "Search insert position" - where target would be inserted to keep sorted order
int searchInsertPosition(const vector<int>& arr, int target) {
    return lowerBound(arr, target);
}

// Generate a sorted vector with possible duplicates, for testing
vector<int> generateSortedVector(int size, int maxVal, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, maxVal);
    vector<int> v(size);
    for (int& x : v) x = dist(rng);
    sort(v.begin(), v.end());
    return v;
}

// Utility to print a vector
void printVector(const vector<int>& v, const string& label) {
    cout << label << ": [";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    cout << "=== Binary Search Demo ===" << endl << endl;

    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    printVector(arr, "Sorted array");

    int target = 7;
    int idx = binarySearch(arr, target);
    cout << "Iterative search for " << target << " -> index " << idx << endl;

    idx = binarySearchRecursive(arr, 0, (int)arr.size() - 1, target);
    cout << "Recursive search for " << target << " -> index " << idx << endl;

    int missing = 8;
    idx = binarySearch(arr, missing);
    cout << "Searching for missing value " << missing << " -> index " << idx << endl;

    cout << endl << "--- Duplicate handling ---" << endl;
    vector<int> dupArr = {1, 2, 2, 2, 2, 3, 4, 4, 5, 6, 6, 6};
    printVector(dupArr, "Array with duplicates");
    int dupTarget = 2;
    cout << "First occurrence of " << dupTarget << ": " << findFirstOccurrence(dupArr, dupTarget) << endl;
    cout << "Last occurrence of " << dupTarget << ": " << findLastOccurrence(dupArr, dupTarget) << endl;
    cout << "Count of " << dupTarget << ": " << countOccurrences(dupArr, dupTarget) << endl;

    int dupTarget2 = 6;
    cout << "First occurrence of " << dupTarget2 << ": " << findFirstOccurrence(dupArr, dupTarget2) << endl;
    cout << "Last occurrence of " << dupTarget2 << ": " << findLastOccurrence(dupArr, dupTarget2) << endl;
    cout << "Count of " << dupTarget2 << ": " << countOccurrences(dupArr, dupTarget2) << endl;

    cout << endl << "--- Lower / Upper bound ---" << endl;
    cout << "lowerBound(5): " << lowerBound(dupArr, 5) << endl;
    cout << "upperBound(5): " << upperBound(dupArr, 5) << endl;
    cout << "lowerBound(0): " << lowerBound(dupArr, 0) << " (no elements < 0)" << endl;
    cout << "lowerBound(10): " << lowerBound(dupArr, 10) << " (insert at end)" << endl;

    cout << endl << "--- Rotated sorted array search ---" << endl;
    vector<int> rotated = {4, 5, 6, 7, 0, 1, 2};
    printVector(rotated, "Rotated array");
    for (int t : {0, 3, 7, 4, 8}) {
        cout << "search(" << t << ") -> " << searchRotated(rotated, t) << endl;
    }

    cout << endl << "--- Peak element ---" << endl;
    vector<int> peakArr = {1, 2, 3, 1};
    printVector(peakArr, "Array");
    int peakIdx = findPeakElement(peakArr);
    cout << "Peak found at index " << peakIdx << " with value " << peakArr[peakIdx] << endl;

    cout << endl << "--- Integer square root ---" << endl;
    for (long long n : {0LL, 1LL, 4LL, 8LL, 15LL, 1000000LL}) {
        cout << "sqrt(" << n << ") = " << integerSqrt(n) << endl;
    }

    cout << endl << "--- Search insert position ---" << endl;
    vector<int> insertArr = {1, 3, 5, 6};
    printVector(insertArr, "Array");
    for (int t : {5, 2, 7, 0}) {
        cout << "insert position for " << t << " -> " << searchInsertPosition(insertArr, t) << endl;
    }

    cout << endl << "--- Performance check on larger array ---" << endl;
    vector<int> big = generateSortedVector(100000, 1000000, 42);
    int target3 = big[big.size() / 2];

    auto start = high_resolution_clock::now();
    int found = binarySearch(big, target3);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Searched array of size " << big.size() << " for value " << target3
         << " -> found at index " << found << " in " << duration.count() << " microseconds" << endl;

    return 0;
}