#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <memory>
#include <numeric>

#pragma once

using namespace std;

template<typename T, typename K>
struct Node {
	K _key;
	T _value;
	bool active = false;

	Node() : _key{}, _value{}, active(false) {}
	Node(K key, T value) : _key(key), _value(value), active(true) {}
};


template<typename T, typename K>
class HashTable {
	Node<T, K>* table;
	size_t capacity;
	size_t size;

	size_t universal_hash(const K& key, size_t mod) const;
	size_t h(K key) const;
	size_t g(K key) const;
	size_t hash(K key, size_t i) const;

	void resize();
public:
	HashTable(size_t m);
	HashTable(const HashTable& other);
	HashTable& operator=(const HashTable& other);
	~HashTable();

	size_t get_size() const;
	vector<K> keys();
	vector<K> keys() const;

	void print();
	bool insert(K key, const T& value);
	void insert_or_assign(K key, T& value);
	bool contains(T& value);
	T* search(K key);
	const T* search(K key) const;
	bool erase(K key);
	int count(K key);
};

template<typename T, typename K>
inline HashTable<T, K>::HashTable(size_t m) : capacity(m), size(0) {
	table = new Node<T, K>[capacity];
}

template<typename T, typename K>
inline HashTable<T, K>::HashTable(const HashTable& other) : capacity(other.capacity), size(other.size) {
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
		size = other.size;
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
inline void HashTable<T, K>::resize() {
	size_t new_capacity = capacity * 2;
	Node<T, K>* new_table = new Node<T, K>[new_capacity]();

	Node<T, K>* old_table = table;
	size_t old_capacity = capacity;

	table = new_table;
	capacity = new_capacity;
	size = 0;

	for (size_t i = 0; i < old_capacity; ++i) {
		if (old_table[i].active) {
			insert(old_table[i]._key, old_table[i]._value);
		}
	}

	delete[] old_table;
}

template<typename T, typename K>
inline size_t HashTable<T, K>::universal_hash(const K& key, size_t mod) const {
	if constexpr (std::is_integral_v<K>) {
		return static_cast<size_t>(key) % mod;
	}
	else if constexpr (std::is_same_v<K, std::string>) {
		size_t hash = 5381;
		for (char c : key) {
			hash = ((hash << 5) + hash) + c;
		}
		return hash % mod;
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
inline size_t HashTable<T, K>::h(K key) const {
	return universal_hash(key, capacity);
}

template<typename T, typename K>
inline size_t HashTable<T, K>::g(K key) const {
	return 1 + universal_hash(key, capacity - 1);
}

template<typename T, typename K>
inline size_t HashTable<T, K>::hash(K key, size_t i) const {
	return (h(key) + i * g(key)) % capacity;
}

template<typename T, typename K>
inline size_t HashTable<T, K>::get_size() const {
	return size;
}

template<typename T, typename K>
inline vector<K> HashTable<T, K>::keys() {
	vector<K> keys;
	for (size_t i = 0; i < capacity; ++i) {
		if (table[i].active) keys.push_back(table[i]._key);
	}

	return keys;
}

template<typename T, typename K>
inline vector<K> HashTable<T, K>::keys() const {
	vector<K> keys;
	for (size_t i = 0; i < capacity; ++i) {
		if (table[i].active) keys.push_back(table[i]._key);
	}

	return keys;
}


template<typename T, typename K>
inline void HashTable<T, K>::print() {
	for (size_t i = 0; i < capacity; ++i) {
		cout << "[" << i << "] " << table[i]._key << ":" << table[i]._value << "\n";
	}
}

template<typename T, typename K>
inline bool HashTable<T, K>::insert(K key, const T& value) {
	if (size >= capacity * 0.7) {
		resize();
	}

	for (size_t i = 0; i < capacity; ++i) {
		size_t idx = hash(key, i);
		if (!table[idx].active) {
			table[idx] = Node<T, K>(key, value);
			table[idx].active = true;
			size++;
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
inline const T* HashTable<T, K>::search(K key) const {
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
		if (table[idx].active && table[idx]._key == key) {
			table[idx].active = false;
			size--;
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