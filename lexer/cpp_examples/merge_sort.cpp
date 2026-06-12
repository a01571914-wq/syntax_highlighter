#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

// ============================================================
//  Merge Sort - Standard implementation plus variants: bottom-up
//  merge sort, counting inversions, and merging k sorted lists
// ============================================================

// Merge two sorted subarrays arr[left..mid] and arr[mid+1..right] in place
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Standard top-down (recursive) merge sort
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Convenience wrapper for sorting an entire vector
void mergeSort(vector<int>& arr) {
    if (!arr.empty()) mergeSort(arr, 0, (int)arr.size() - 1);
}

// Bottom-up (iterative) merge sort - avoids recursion entirely
void mergeSortIterative(vector<int>& arr) {
    int n = (int)arr.size();
    for (int width = 1; width < n; width *= 2) {
        for (int left = 0; left < n - width; left += 2 * width) {
            int mid = left + width - 1;
            int right = min(left + 2 * width - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

// Merge sort that also counts the number of "inversions" - pairs (i, j) where
// i < j but arr[i] > arr[j]. Useful for measuring how unsorted an array is.
long long mergeAndCount(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    long long invCount = 0;
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
            invCount += (n1 - i); // all remaining elements in L are > R[j]
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    return invCount;
}

long long countInversions(vector<int>& arr, int left, int right) {
    long long count = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        count += countInversions(arr, left, mid);
        count += countInversions(arr, mid + 1, right);
        count += mergeAndCount(arr, left, mid, right);
    }
    return count;
}

long long countInversions(vector<int> arr) { // takes a copy since it sorts the array
    if (arr.empty()) return 0;
    return countInversions(arr, 0, (int)arr.size() - 1);
}

// Merge k sorted vectors into a single sorted vector using a divide-and-conquer
// approach built on top of the binary merge() function above.
vector<int> mergeTwoSortedVectors(const vector<int>& a, const vector<int>& b) {
    vector<int> result;
    result.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) result.push_back(a[i++]);
        else result.push_back(b[j++]);
    }
    while (i < a.size()) result.push_back(a[i++]);
    while (j < b.size()) result.push_back(b[j++]);
    return result;
}

vector<int> mergeKSortedLists(const vector<vector<int>>& lists, int left, int right) {
    if (left == right) return lists[left];
    if (left > right) return {};
    int mid = left + (right - left) / 2;
    vector<int> leftMerged = mergeKSortedLists(lists, left, mid);
    vector<int> rightMerged = mergeKSortedLists(lists, mid + 1, right);
    return mergeTwoSortedVectors(leftMerged, rightMerged);
}

vector<int> mergeKSortedLists(const vector<vector<int>>& lists) {
    if (lists.empty()) return {};
    return mergeKSortedLists(lists, 0, (int)lists.size() - 1);
}

// Utility functions
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++)
        if (arr[i - 1] > arr[i]) return false;
    return true;
}

vector<int> generateRandomVector(int size, int minVal, int maxVal, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(minVal, maxVal);
    vector<int> v(size);
    for (int& x : v) x = dist(rng);
    return v;
}

void printVector(const vector<int>& v, const string& label) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

int main() {
    cout << "=== Basic Merge Sort ===" << endl;
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    printVector(arr, "Original");
    mergeSort(arr);
    printVector(arr, "Sorted (recursive)");

    cout << endl << "=== Bottom-Up (Iterative) Merge Sort ===" << endl;
    vector<int> arr2 = {38, 27, 43, 3, 9, 82, 10};
    mergeSortIterative(arr2);
    printVector(arr2, "Sorted (iterative)");

    cout << endl << "=== Edge Cases ===" << endl;
    vector<int> empty;
    mergeSort(empty);
    cout << "Empty array sorted, size: " << empty.size() << endl;

    vector<int> single = {7};
    mergeSort(single);
    printVector(single, "Single element");

    vector<int> sortedAlready = {1, 2, 3, 4, 5};
    mergeSort(sortedAlready);
    printVector(sortedAlready, "Already sorted");

    vector<int> reversed = {5, 4, 3, 2, 1};
    mergeSort(reversed);
    printVector(reversed, "Reversed input");

    vector<int> duplicates = {4, 2, 4, 2, 4, 1, 1};
    mergeSort(duplicates);
    printVector(duplicates, "With duplicates");

    cout << endl << "=== Counting Inversions ===" << endl;
    vector<int> invArr = {2, 4, 1, 3, 5};
    printVector(invArr, "Array");
    cout << "Number of inversions: " << countInversions(invArr) << endl;

    vector<int> sortedInv = {1, 2, 3, 4, 5};
    cout << "Inversions in sorted array: " << countInversions(sortedInv) << endl;

    vector<int> reverseInv = {5, 4, 3, 2, 1};
    cout << "Inversions in fully reversed array: " << countInversions(reverseInv) << endl;

    cout << endl << "=== Merging K Sorted Lists ===" << endl;
    vector<vector<int>> lists = {
        {1, 4, 7},
        {2, 5, 8, 11},
        {0, 3, 6, 9, 12}
    };
    for (size_t i = 0; i < lists.size(); i++)
        printVector(lists[i], "List " + to_string(i));

    vector<int> merged = mergeKSortedLists(lists);
    printVector(merged, "Merged result");
    cout << "Is merged result sorted? " << (isSorted(merged) ? "yes" : "no") << endl;

    cout << endl << "=== Performance Comparison (n = 5000) ===" << endl;
    vector<int> big = generateRandomVector(5000, -100000, 100000, 7);

    vector<int> bigA = big;
    auto start = high_resolution_clock::now();
    mergeSort(bigA);
    auto end = high_resolution_clock::now();
    cout << "Recursive mergeSort: " << duration_cast<microseconds>(end - start).count()
         << " microseconds. Sorted: " << (isSorted(bigA) ? "yes" : "no") << endl;

    vector<int> bigB = big;
    start = high_resolution_clock::now();
    mergeSortIterative(bigB);
    end = high_resolution_clock::now();
    cout << "Iterative mergeSort:  " << duration_cast<microseconds>(end - start).count()
         << " microseconds. Sorted: " << (isSorted(bigB) ? "yes" : "no") << endl;

    vector<int> bigC = big;
    start = high_resolution_clock::now();
    sort(bigC.begin(), bigC.end());
    end = high_resolution_clock::now();
    cout << "std::sort (reference): " << duration_cast<microseconds>(end - start).count()
         << " microseconds. Sorted: " << (isSorted(bigC) ? "yes" : "no") << endl;

    cout << "Recursive and iterative results match: " << (bigA == bigC ? "yes" : "no") << endl;

    return 0;
}