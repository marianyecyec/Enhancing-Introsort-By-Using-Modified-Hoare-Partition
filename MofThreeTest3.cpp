#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Global variable to track maximum space used
size_t maxSpaceUsed = 0;

// Function prototypes
void sort(vector<int>& A);
void introsort(vector<int>& A, int left, int right, int maxdepth);
void insertionsort(vector<int>& A, int left, int right);
void heapsort(vector<int>& A, int left, int right);
void build_max_heap(vector<int>& A, int left, int right);
void max_heapify(vector<int>& A, int i, int right, int offset);
int median_of_three(vector<int>& A, int left, int right);

// Custom new and delete operators to track memory usage
void* operator new(size_t size);
void operator delete(void* ptr) noexcept;

void* operator new(size_t size) {
    maxSpaceUsed += size;
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

int main() {
    // Test cases
    vector<int> sizes = {10000, 100000, 1000000, 10000000};
    vector<pair<int, int>> ranges = {{1, 1000}, {1, 10000}, {1, 100000}, {1, 1000000}};
    random_device rd;
    mt19937 gen(rd());

    for (size_t i = 0; i < sizes.size(); ++i) {
        int size = sizes[i];
        int rangeMin = ranges[i].first;
        int rangeMax = ranges[i].second;

        // Generate random array
        vector<int> A(size);
        uniform_int_distribution<int> dis(rangeMin, rangeMax);
        generate(A.begin(), A.end(), [&]() { return dis(gen); });

        // Measure execution time
        auto start = high_resolution_clock::now();
        sort(A);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        cout << "3rd Test - Size: " << size << ", Range: " << rangeMin << "-" << rangeMax 
             << ", Time: " << duration << " ms, Max Space Used: " << maxSpaceUsed << " bytes" << endl;

        // Reset max space used for the next iteration
        maxSpaceUsed = 0;
    }

    return 0;
}

void sort(vector<int>& A) {
    int maxdepth = (log(A.size()) / log(2)) * 2;
    introsort(A, 0, A.size() - 1, maxdepth);
}

void introsort(vector<int>& A, int left, int right, int maxdepth) {
    if (right - left < 20) {
        insertionsort(A, left, right);
    } else if (maxdepth == 0) {
        heapsort(A, left, right);
    } else {
        int p = median_of_three(A, left, right);
        introsort(A, left, p - 1, maxdepth - 1);
        introsort(A, p + 1, right, maxdepth - 1);
    }
}

void insertionsort(vector<int>& A, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = A[i];
        int j = i - 1;
        while (j >= left && A[j] > key) {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = key;
    }
}

void heapsort(vector<int>& A, int left, int right) {
    build_max_heap(A, left, right);
    for (int i = right; i > left; --i) {
        swap(A[left], A[i]);
        max_heapify(A, left, i - 1, left);
    }
}

void build_max_heap(vector<int>& A, int left, int right) {
    for (int i = (right + left) / 2; i >= left; --i) {
        max_heapify(A, i, right, left);
    }
}

void max_heapify(vector<int>& A, int i, int right, int offset) {
    int largest = i;
    int l = 2 * (i - offset) + 1 + offset;
    int r = 2 * (i - offset) + 2 + offset;
    if (l <= right && A[l] > A[largest]) {
        largest = l;
    }
    if (r <= right && A[r] > A[largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(A[i], A[largest]);
        max_heapify(A, largest, right, offset);
    }
}

int median_of_three(vector<int>& A, int left, int right) {
    int mid = left + (right - left) / 2;
    if (A[left] > A[mid]) {
        swap(A[left], A[mid]);
    }
    if (A[left] > A[right]) {
        swap(A[left], A[right]);
    }
    if (A[mid] > A[right]) {
        swap(A[mid], A[right]);
    }
    return mid;
}