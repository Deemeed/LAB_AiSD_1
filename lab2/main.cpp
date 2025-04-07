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
}
