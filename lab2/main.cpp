#include "hashTable.h"

int main() {
	HashTable<unsigned char, unsigned char> pearson_table(256, true);

	string s1, s2;
	cout << "Enter string 1: ";
	getline(std::cin, s1);
	cout << "Enter string 2: ";
	getline(std::cin, s2);

	unsigned char h1 = hashPearson(s1, pearson_table);
	unsigned char h2 = hashPearson(s2, pearson_table);

	std::cout << "Hash 1: " << static_cast<int>(h1) << "\n";
	std::cout << "Hash 2: " << static_cast<int>(h2) << "\n";

	cout << (compare(s1, s2, pearson_table) ? "Same" : "Not same");

	return 0;

    /*HashTable<string, string> table(10);

    table.insert("apple", "fruit");
    table.insert("carrot", "vegetable");
    table.insert("banana", "fruit");
    table.insert("dog", "animal");

    cout << "Table:\n";
    table.print();

    string* result = table.search("carrot");
    if (result) {
        cout << "\nFind value for 'carrot': " << *result << endl;
    }
    else {
        cout << "\n'carrot' not found.\n";
    }

    string newValue = "root vegetable";
    table.insert_or_assign("carrot", newValue);

    cout << "\nNew value for 'carrot':\n";
    table.print();

    table.erase("banana");
    cout << "\nAfter deleting 'banana':\n";
    table.print();

    int collisionCount = table.count("dog");
    if (collisionCount != -1) {
        cout << "\nKey 'dog' insrted with attempt: " << collisionCount << endl;
    }
    else {
        cout << "\nKey 'dog' not found in table\n";
    }

    return 0;*/
}
