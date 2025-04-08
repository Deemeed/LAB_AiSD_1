#include <iostream>
#include <array>
#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <numeric>

#pragma once

using namespace std;


template<typename T, typename K>
struct Node {
	K _key { 0 };
	T _value { 0 };
	bool active = false;

	Node() = default;
	Node(K key, T value) : _key(key), _value(value), active(true) {}
};


template<typename T, typename K>
class HashTable {
	Node<T, K>* table;
	size_t capacity;

	size_t universal_hash(const K& key, size_t mod) const;
	size_t h(K key);
	size_t g(K key);
	size_t hash(K key, size_t i);
public:
	HashTable(size_t m);
	HashTable(size_t m, bool flag);
	HashTable(const HashTable& other);
	HashTable& operator=(const HashTable& other);
	~HashTable();

	void print();
	bool insert(K key, const T& value);
	void insert_or_assign(K key, T& value);
	bool contains(T& value);
	T* search(K key);
	bool erase(K key);
	int count(K key);
};

template<typename T, typename K>
inline HashTable<T, K>::HashTable(size_t m) : capacity(m) {
	table = new Node<T, K>[capacity];
}

template<typename T, typename K>
inline HashTable<T, K>::HashTable(size_t m, bool flag) : capacity(m) {
	table = new Node<T, K>[capacity];

	if (flag) {
		vector<unsigned char> values(256);
		iota(values.begin(), values.end(), 0);
		shuffle(values.begin(), values.end(), mt19937(random_device{}()));

		for (int i = 0; i < 256; ++i) {
			insert(static_cast<unsigned char>(i), values[i]);
		}
	}
}

template<typename T, typename K>
inline HashTable<T, K>::HashTable(const HashTable& other) : capacity(other.capacity) {
	table = new Node<T, K>[capacity];
	for (size_t i = 0; i < capacity; ++i) {
		table[i] = other.table[i];
	}
}

template<typename T, typename K>
inline HashTable<T, K>& HashTable<T, K>::operator=(const HashTable<T, K>& other) {
	if (this != &other) {
		delete[] table;
		capacity = other.capacity;
		table = new Node<T, K>[capacity];
		for (size_t i = 0; i < capacity; ++i) {
			table[i] = other.table[i];
		}
	}

	return *this;
}

template<typename T, typename K>
inline HashTable<T, K>::~HashTable() {
	delete[] table;
}

template<typename T, typename K>
inline size_t HashTable<T, K>::universal_hash(const K& key, size_t mod) const {
	if constexpr (std::is_integral_v<K>) {
		return static_cast<size_t>(key) % mod;
	}
	else if constexpr (std::is_same_v<K, const char*>) {
		size_t hash = 0;
		for (const char* p = key; *p != '\0'; ++p) {
			hash = (hash * 31 + static_cast<size_t>(*p)) % mod;
		}
		return hash;
	}
	else {
		size_t hash = 0;
		const unsigned char* p = reinterpret_cast<const unsigned char*>(&key);
		for (size_t i = 0; i < sizeof(K); ++i) {
			hash = (hash * 31 + static_cast<size_t>(p[i])) % mod;
		}
		return hash;
	}
}

template<typename T, typename K>
inline size_t HashTable<T, K>::h(K key) {
	return universal_hash(key, capacity);
}

template<typename T, typename K>
inline size_t HashTable<T, K>::g(K key) {
	return 1 + universal_hash(key, capacity - 1);
}

template<typename T, typename K>
inline size_t HashTable<T, K>::hash(K key, size_t i) {
	return (h(key) + i * g(key)) % capacity;
}

template<typename T, typename K>
inline void HashTable<T, K>::print() {
	for (size_t i = 0; i < capacity; ++i) {
		cout << "[" << i << "] " << table[i]._key << ":" << table[i]._value << "\n";
	}
}

template<typename T, typename K>
inline bool HashTable<T, K>::insert(K key, const T& value) {
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = hash(key, i);
		if (!table[idx].active) {
			table[idx] = Node<T, K>(key, value);
			return true;
		}
	}

	return false;
}

template<typename T, typename K>
inline void HashTable<T, K>::insert_or_assign(K key, T& value) {
	size_t idx = hash(key, 0);

	if (table[idx].active) table[idx]._value = value;
	else table[idx] = Node<T, K>(key, value);
}

template<typename T, typename K>
inline bool HashTable<T, K>::contains(T& value) {
	for (size_t i = 0; i < capacity; ++i) {
		if (table[i].active && table[i]._value == value) {
			return true;
		}
	}

	return false;
}

template<typename T, typename K>
inline T* HashTable<T, K>::search(K key) {
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = hash(key, i);
		if (table[idx].active && table[idx]._key == key) {
			return &table[idx]._value;
		}
	}

	return nullptr;
}

template<typename T, typename K>
inline bool HashTable<T, K>::erase(K key) {
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = hash(key, i);
		if (table[i].active && table[idx]._key == key) {
			table[idx].active = false;
			return true;
		}
	}

	return false;
}

template<typename T, typename K>
inline int HashTable<T, K>::count(K key) {
	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = hash(key, i);
		if (table[idx].active && table[idx]._key == key) {
			return static_cast<int>(i);
		}
	}

	return -1;
}


unsigned char hashPearson(const string& str, HashTable<unsigned char, unsigned char>& table) {
	unsigned char h = 0;

	for (char c : str) {
		unsigned char idx = h ^ static_cast<unsigned char>(c);
		unsigned char* value = table.search(idx);
		if (!value) {
			throw std::runtime_error("Value not found");
		}
		h = *value;
	}

	return h;
}


bool compare(const string& text1, const string& text2, HashTable<unsigned char, unsigned char>& table) {
	return hashPearson(text1, table) == hashPearson(text2, table);
}