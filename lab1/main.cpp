#include "BSTree.h"

int main() {
	BSTree bst;
	BSTree bst2;

	while (true) {
		int x;
		cin >> x;

		if (x == 0) break;

		bst.insert(x);
	}

	bst.print();

	bst2 = bst;
	bst2.insert(10);
	bst.print();
	bst2.print();

	return 0;
}