#include "BSTree.h"
#include "tests.h"


int main() {
    int sizes[] = { 1000, 10000, 100000 };
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