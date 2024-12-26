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
void introsort(vector<int>& arr, int begin, int end);
void insertionSort(vector<int>& arr, int left, int right);
int MHoare(vector<int>& data, int first, int last);
void introsortUtil(vector<int>& arr, int begin, int end, int depthLimit);


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
    vector<int> sizes = {100, 1000, 10000, 100000};
    random_device rd;
    mt19937 gen(rd());


    // 2nd Test: Partially Sorted Data
    for (size_t i = 0; i < sizes.size(); ++i) {
        int size = sizes[i];


        // Generate partially sorted array
        vector<int> arr(size);
        iota(arr.begin(), arr.begin() + size / 2, 1); // Sorted first half
        uniform_int_distribution<int> dis(1, size);
        generate(arr.begin() + size / 2, arr.end(), [&]() { return dis(gen); }); // Random second half


        // Reset max space used for the next iteration
        maxSpaceUsed = 0;


        // Measure execution time
        auto start = high_resolution_clock::now();
        introsort(arr, 0, arr.size() - 1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();


        cout << "2nd Test - Size: " << size << ", Partially Sorted, Time: " << duration 
             << " ms, Max Space Used: " << maxSpaceUsed << " bytes" << endl;
    }


    return 0;
}


void introsort(vector<int>& arr, int begin, int end) {
    int depthLimit = 2 * log2(end - begin + 1);
    introsortUtil(arr, begin, end, depthLimit);
}


void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}


int MHoare(vector<int>& data, int first, int last) {
    if (data[first] > data[last]) {
        swap(data[first], data[last]);
    }
    int pivot = data[first];
    int left = first;
    int right = last;
    while (true) {
        while (data[right] > pivot) {
            right--;
        }
        while (data[left] < pivot) {
            left++;
        }
        if (left < right) {
            swap(data[left], data[right]);
            left++;
            right--;
        } else {
            return right;
        }
    }
}


void introsortUtil(vector<int>& arr, int begin, int end, int depthLimit) {
    int size = end - begin + 1;
    if (size <= 16) {
        insertionSort(arr, begin, end);
        return;
    }
    if (depthLimit == 0) {
        make_heap(arr.begin() + begin, arr.begin() + end + 1);
        sort_heap(arr.begin() + begin, arr.begin() + end + 1);
        return;
    }
    int pivot = MHoare(arr, begin, end);  // Using Modified Hoare partitioning
    introsortUtil(arr, begin, pivot, depthLimit - 1);
    introsortUtil(arr, pivot + 1, end, depthLimit - 1);
}