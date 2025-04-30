#include "BSTree.h"

#include <chrono>
#include <random>

#pragma once

using namespace std;

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

void fillContainer(BSTree& bst, int size) {
    for (int i = 0; i < size; ++i) {
        int key = lcg() % size * 2;
        while (!bst.insert(key)) {
            key = lcg() % size * 2;
        }
    }
}

void fillVector(vector<int>& vec, int size) {
    for (int i = 0; i < size; ++i) {
        int key = lcg() % size * 2;
        while (find(vec.begin(), vec.end(), key) != vec.end()) {
            key = lcg() % size * 2;
        }
        vec.push_back(key);
    }
}

void measureFillTimeBST(int size) {
    BSTree bst;
    auto start = chrono::high_resolution_clock::now();
    fillContainer(bst, size);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure fill time BSTree size " << size << ": " << duration << " mks" << endl;
}

void measureFillTimeVector(int size) {
    vector<int> vec;
    auto start = chrono::high_resolution_clock::now();
    fillVector(vec, size);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure fill time std::vector<int> size " << size << ": " << duration << " mks" << endl;
}

void measureSearchTimeBST(int size) {
    BSTree bst;
    fillContainer(bst, size);
    int key = lcg() % size * 2; 
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        bst.contains(key);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure search time BSTree size " << size << ": " << duration / 1000.0 << " mks" << endl;
}

void measureSearchTimeVector(int size) {
    vector<int> vec;
    fillVector(vec, size);
    int key = lcg() % size * 2;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        find(vec.begin(), vec.end(), key) != vec.end();
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure search time std::vector<int> size " << size << ": " << duration / 1000.0 << " mks" << endl;
}

void measureAddRemoveTimeBST(int size) {
    BSTree bst;
    fillContainer(bst, size);
    int key = lcg() % size * 2;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        bst.insert(key);
        bst.erase(key);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure add remove time BSTree size " << size << ": " << duration / 2000.0 << " mks" << endl;
}

void measureAddRemoveTimeVector(int size) {
    vector<int> vec;
    fillVector(vec, size);
    int key = lcg() % size * 2;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        vec.push_back(key);
        vec.pop_back();
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Measure add remove time std::vector<int> size " << size << ": " << duration / 2000.0 << " mks" << endl;
}