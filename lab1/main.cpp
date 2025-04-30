#include "BSTree.h"
#include "tests.h"


int main() {
    /*BSTree bst1, bst2;

    bst1.insert(1);
    bst1.insert(2);
    bst1.insert(3);
    bst2.insert(2);
    bst2.insert(3);
    bst2.insert(4);

    vector<int> intersectionRes = intersectionTree(bst1, bst2);

    vector<int> unionRes = unionTree(bst1, bst2);

    std::cout << "Intersection: ";
    for (int x : intersectionRes) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    std::cout << "Union: ";
    for (int x : unionRes) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;*/

    int sizes[] = {1000, 10000, 100000};
    for (int size : sizes) {
        cout << "Size: " << size << endl;

        // Измерение времени заполнения
        measureFillTimeBST(size);
        measureFillTimeVector(size);

        // Измерение времени поиска
        measureSearchTimeBST(size);
        measureSearchTimeVector(size);

        // Измерение времени добавления и удаления
        measureAddRemoveTimeBST(size);
        measureAddRemoveTimeVector(size);

        cout << endl;
    }

    return 0;
}